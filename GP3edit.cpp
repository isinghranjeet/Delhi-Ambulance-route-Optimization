#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <climits>
using namespace std;

const int I = INT_MAX;

// Pseudocode:
// readBuildings: it is reading the name of the locations from the text file

// Author: Jahnavi
// Inputs: a vector of strings which will store location names.
//         and name of the data file
// Outputs: None
void readBuildings(vector<string> &buildings, const string filename)
{
    ifstream infile(filename+ ".txt");
    string name;

    if (infile.is_open())
        while (getline(infile, name))
        {
            buildings.push_back(name);
        }
    else
    {
    	cout << "File not found!" << endl;

    }

    infile.close();
}

// Pseudocode:
// readMatrix: it is reading the matrix from the text file, which are
//				 the edges for each location.

// Author: Jahnavi
// Inputs: a 2D vector of integers which is the edges matrix, vector with
//		 	the name of the building, name of the text file.
// Outputs: None
void readMatrix(vector<vector<int>> &adjMatrix, vector<string> buildings,
                const string filename)
{
    vector<int> tempRow;
    int numBuilding;
    numBuilding = buildings.size();
    ifstream infile(filename+ ".txt");
    int dist;

    if (!infile.is_open())
        cout << "File not found!" << endl;
    else
    {
        for (int i = 0; i < numBuilding; i++)
        {
            for (int j = 0; j < numBuilding; j++)
            {
                infile >> dist;
                if(dist==-1)
				{
					dist = 0;
				}
                tempRow.push_back(dist);
            }
            adjMatrix.push_back(tempRow);
            tempRow.clear();
        }
    }

    infile.close();
}

// Pseudocode:
// printLocations: printing the locations to display to user

// Author: Manisha
// Inputs:  a vector with location names
// Outputs: prints the vector
void printLocations(vector<string> loc)
{
	for(int i = 0; i < loc.size(); i++)
	{
		cout << i << " is " << loc[i] << " , ";
	}
	cout << endl;
}

// Pseudocode:
// printMatrix: printing the matrixusing two for loops

// Author: Jahnavi
// Inputs: a 2D vector of integers which is the edges matrix.
// Outputs: prints the matrix in an adjacency matrix form.
void printMatrix(vector<vector<int>> adjMatrix)
{
    for (int i = 0; i < adjMatrix.size(); ++i)
    {
        for (int j = 0; j < adjMatrix[i].size(); ++j)
        {
            cout << adjMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

// Pseudocode:
// minDistance: finds vertex with minimum distance and its index

// Author: Jahnavi
// Inputs: Distances vector, vector of checked vertex
// Outputs: integer which is the minimum distance index
int minDistance(const vector<int>& dist, const vector<bool>& checked) {
    int minIndex = -1;
    int min = INT_MAX;

    for (int i = 0; i < dist.size(); i++) {
        if (!checked[i] && dist[i] <= min) {
            min = dist[i];
            minIndex = i;
        }
    }
    return minIndex;
}

// Pseudocode:
// dijkstrasAlgorithm: finds shortest path from hospital to the point user wants

// Author: Jahnavi
// Edits: cameron (wordings)
// Inputs: adjaceny matrix vector, vector with previous vertex, strat and end index
//			vector of building names;
// Outputs: Pass by reference arrays and output of shortest distance
void dijkstrasAlgorithm(const vector<vector<int>>& adjMatrix, vector<int>& prev, int startIndex, int endIndex, vector<string> buildings) {
    int V = adjMatrix.size();

    vector<bool> checked(V, false);
    vector<int> dist(V, I);

    for (int i = 0; i < V; i++) {
        dist[i] = I;
        checked[i] = false;
    }

    dist[startIndex] = 0;

    for (int j = 0; j < V - 1; j++) {
        int u = minDistance(dist, checked);

        checked[u] = true;

        for (int n = 0; n < V; n++) {
            if (!checked[n] && adjMatrix[u][n] && dist[u] != I &&
                (dist[u] + adjMatrix[u][n]) < dist[n]) {
                dist[n] = dist[u] + adjMatrix[u][n];
                prev[n] = u;
            }
        }
    }

    // Output the shortest path
    cout << "Shortest distance from the Hospital to " << buildings[endIndex] << ": " << dist[endIndex] << " miles." <<endl;

    // Output the path with predecessor
    cout << "Shortest path: ";
    int current = endIndex;
    while (current != startIndex) {
        cout << current << " <- ";
        current = prev[current];
    }
    cout << startIndex << endl;
}


//driver
// Author: jahnavi and cameron
int main()
{
    vector<string> buildings;
    vector<vector<int>> adjMatrix;
    vector<int> prev;
    
	char cont = 'Y';

    //buildings is file with location names
    readBuildings(buildings, "buildings");

    //dataset is adjacency matrix as text file
    readMatrix(adjMatrix, buildings, "Dataset");

    // Printing the adjacency matrix for cross checking 
    cout << "Adjacency Matrix:\n";
    printMatrix(adjMatrix);
    cout << endl;

    int startIndex, endIndex;

	//start index is the hospital
    startIndex = 0;

	while( cont == 'Y' or cont == 'y')
	{
		cout << endl;
		printLocations(buildings);
		cout << "Enter the end building index (0-" << buildings.size() - 1 << "): ";
    	cin >> endIndex;
    	cout << endl;

    	// Validating the input by user
    	if (startIndex < 0 or startIndex >= buildings.size() or endIndex < 0 or endIndex >= buildings.size())
		{
    	    cout << "Invalid input. Exiting... "<< endl;
    	    return 1;
    	}
	
    	prev.resize(buildings.size(), -1);
	
    	dijkstrasAlgorithm(adjMatrix, prev, startIndex, endIndex, buildings);
		
		cout << "Do you want to continue? (Y/N)" << endl;
		cin >> cont;
		cout << endl;
	}
	
    return 0;
}
