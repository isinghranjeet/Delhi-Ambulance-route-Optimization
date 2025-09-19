#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <climits>
#include <queue>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

const int INF = INT_MAX;

// ------------------- COLOR CODES -------------------
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

// ------------------- FILE READING -------------------
void readBuildings(vector<string> &buildings, const string filename) {
    ifstream infile(filename + ".txt");
    string name;
    if (infile.is_open()) {
        while (getline(infile, name)) {
            if (!name.empty()) buildings.push_back(name);
        }
    } else {
        cout << RED << "❌ File not found: " << filename << ".txt" << RESET << endl;
    }
    infile.close();
}

void readMatrix(vector<vector<int>> &adjMatrix, int numBuilding, const string filename) {
    ifstream infile(filename + ".txt");
    int dist;
    if (!infile.is_open()) {
        cout << RED << "❌ File not found: " << filename << ".txt" << RESET << endl;
        return;
    }
    adjMatrix.resize(numBuilding, vector<int>(numBuilding, INF));
    for (int i = 0; i < numBuilding; i++) {
        for (int j = 0; j < numBuilding; j++) {
            infile >> dist;
            if (dist == -1 || dist == 0) dist = INF;
            if (i == j) dist = 0;
            adjMatrix[i][j] = dist;
        }
    }
    infile.close();
}

// ------------------- PRINT HELPERS -------------------
void printHeader(const string& title) {
    cout << "\n" << CYAN << BOLD
         << "============================================================\n"
         << "   " << title << "\n"
         << "============================================================"
         << RESET << "\n";
}

void printLocations(const vector<string>& loc) {
    printHeader("📍 Available Locations");
    for (int i = 0; i < loc.size(); i++) {
        cout << YELLOW << setw(2) << i << RESET
             << " → " << GREEN << loc[i] << RESET << "\n";
    }
}

void printMatrix(const vector<vector<int>>& adjMatrix) {
    int V = adjMatrix.size();
    printHeader("📊 Adjacency Matrix (Distances in km)");
    cout << setw(6) << " ";
    for (int j = 0; j < V; j++)
        cout << CYAN << setw(5) << j << RESET;
    cout << "\n";

    for (int i = 0; i < V; i++) {
        cout << CYAN << setw(2) << i << RESET << "  ";
        for (int j = 0; j < V; j++) {
            if (adjMatrix[i][j] == INF)
                cout << RED << setw(5) << "∞" << RESET;
            else
                cout << GREEN << setw(5) << adjMatrix[i][j] << RESET;
        }
        cout << "\n";
    }
}

void printAmbulanceStatus(const vector<int>& positions,
                          const vector<string>& buildings,
                          const vector<bool>& availability) {
    printHeader("🚑 Ambulance Status");
    for (int i = 0; i < positions.size(); i++) {
        cout << " #" << i << " at " << BLUE << buildings[positions[i]] << RESET
             << " → " << (availability[i] ? GREEN "Available" RESET : RED "Busy" RESET)
             << "\n";
    }
}

// ------------------- DIJKSTRA -------------------
struct Node {
    int vertex, dist;
    bool operator>(const Node &other) const {
        return dist > other.dist;
    }
};

int dijkstra(const vector<vector<int>>& adjMatrix, int start, int end, vector<int>& prev) {
    int V = adjMatrix.size();
    vector<int> dist(V, INF);
    vector<bool> visited(V, false);
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    dist[start] = 0;
    pq.push({start, 0});
    prev.assign(V, -1);

    while (!pq.empty()) {
        int u = pq.top().vertex;
        pq.pop();
        if (visited[u]) continue;
        visited[u] = true;

        if (u == end) break;  // Early stop if destination reached

        for (int v = 0; v < V; v++) {
            if (adjMatrix[u][v] != INF && dist[u] + adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + adjMatrix[u][v];
                prev[v] = u;
                pq.push({v, dist[v]});
            }
        }
    }
    return dist[end];
}

void printPath(int start, int end, const vector<int>& prev, const vector<string>& buildings) {
    if (prev[end] == -1) {
        cout << RED << "❌ No path found.\n" << RESET;
        return;
    }
    vector<int> path;
    for (int at = end; at != -1; at = prev[at]) path.push_back(at);

    cout << "\n🛣️  " << BOLD << "Path: " << RESET;
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << YELLOW << buildings[path[i]] << RESET;
        if (i != 0) cout << " " << MAGENTA << "➔" << RESET << " ";
    }
    cout << "\n";
}

// ------------------- HOSPITAL SUGGESTION -------------------
int nearestHospital(const vector<vector<int>>& adjMatrix, int patientIndex,
                    const vector<string>& buildings, const vector<int>& hospitalIndices) {
    int bestHospital = -1, shortest = INF;
    vector<int> prev;
    for (int hospital : hospitalIndices) {
        if (hospital == patientIndex) continue;
        int dist = dijkstra(adjMatrix, patientIndex, hospital, prev);
        if (dist < shortest) {
            shortest = dist;
            bestHospital = hospital;
        }
    }
    if (bestHospital != -1) {
        cout << "🏥 Nearest Hospital: " << BLUE << buildings[bestHospital] << RESET
             << " (Distance: " << GREEN << shortest << " km" << RESET << ")\n";
    }
    return bestHospital;
}

// ------------------- DISPATCH LOGGING -------------------
void logDispatch(const string &msg) {
    ofstream log("logs.txt", ios::app);
    log << msg << "\n";
    log.close();
}

// ------------------- AMBULANCE SELECTION -------------------
int assignAmbulance(vector<vector<int>>& adjMatrix,
                    vector<int>& ambulancePositions,
                    int patientIndex,
                    const vector<string>& buildings,
                    string priority,
                    const vector<bool>& ambulanceAvailability) {
    int bestAmbulance = -1, shortestDist = INF;

    printHeader("🚑 Ambulance Candidates");
    for (int i = 0; i < ambulancePositions.size(); i++) {
        if (!ambulanceAvailability[i]) continue; // skip unavailable ambulances

        vector<int> prev;
        int dist = dijkstra(adjMatrix, ambulancePositions[i], patientIndex, prev);

        if (dist == INF) {
            cout << " → #" << i << " at " << BLUE << buildings[ambulancePositions[i]] << RESET
                 << " → " << RED << "No route." << RESET << "\n";
            continue;
        }

        float trafficFactor = (100 + rand() % 51) / 100.0f; // 1.0 - 1.5 traffic multiplier
        int adjustedDist = int(dist * trafficFactor);

        int speed = 40 + rand() % 21; // 40-60 km/h speed variation
        int etaMin = (adjustedDist * 60) / speed;

        cout << " → #" << i << " at " << BLUE << buildings[ambulancePositions[i]] << RESET
             << " → Distance: " << GREEN << adjustedDist << " km" << RESET
             << " | ETA: " << YELLOW << etaMin << " mins" << RESET << "\n";

        // Priority weighting
        float priorityWeight = 1.0;
        if (priority == "Critical") priorityWeight = 0.5;
        else if (priority == "High") priorityWeight = 0.75;

        int weightedDist = int(adjustedDist * priorityWeight);

        if (weightedDist < shortestDist) {
            shortestDist = weightedDist;
            bestAmbulance = i;
        }
    }

    if (bestAmbulance != -1) {
        cout << GREEN << "✅ Best Ambulance: #" << bestAmbulance
             << " at " << buildings[ambulancePositions[bestAmbulance]]
             << " → Weighted Distance: " << shortestDist << " km" << RESET << "\n";
        ambulancePositions[bestAmbulance] = patientIndex; // move ambulance
    } else {
        cout << RED << "❌ No ambulance can reach the patient!\n" << RESET;
    }
    return bestAmbulance;
}

// ------------------- NEW FEATURES -------------------
void updateAmbulanceAvailability(vector<bool>& availability, int busyAmb, int cycles) {
    if (busyAmb >= 0 && busyAmb < availability.size())
        availability[busyAmb] = false;
    for (int i = 0; i < availability.size(); i++) {
        if (!availability[i] && (rand() % 5 == 0))
            availability[i] = true;
    }
}

void patientFeedback() {
    string feedbacks[] = {
        "Great service! Ambulance arrived quickly.",
        "Average response time, could be better.",
        "Ambulance was late due to traffic congestion.",
        "Very professional and helpful ambulance staff.",
        "Fast and efficient service, thanks!"
    };
    int idx = rand() % 5;
    cout << CYAN << "💬 Patient Feedback: " << feedbacks[idx] << RESET << "\n";
}

// ------------------- MAIN -------------------
int main() {
    srand(time(0));
    vector<string> buildings;
    vector<vector<int>> adjMatrix;
    vector<int> prev;
    char cont = 'Y';

    readBuildings(buildings, "buildings");
    readMatrix(adjMatrix, buildings.size(), "Dataset");

    if (buildings.empty() || adjMatrix.empty()) {
        cout << YELLOW << "⚠️ Program terminated due to missing input files.\n" << RESET;
        return 1;
    }

    printHeader("🚑 AMBULANCE ROUTE OPTIMIZER - DELHI 🚑");
    printMatrix(adjMatrix);

    vector<int> hospitalIndices = {0, 3, 5}; // example hospitals
    vector<int> ambulancePositions = {0, 3, 5};
    vector<bool> ambulanceAvailability(ambulancePositions.size(), true);

    int totalPatients = 0, totalDistance = 0;

    while (cont == 'Y' || cont == 'y') {
        printLocations(buildings);
        printAmbulanceStatus(ambulancePositions, buildings, ambulanceAvailability);

        int patientIndex;
        cout << "\nEnter patient location index (0-" << buildings.size() - 1 << "): ";
        cin >> patientIndex;

        if (patientIndex < 0 || patientIndex >= buildings.size()) {
            cout << RED << "❌ Invalid input. Try again.\n" << RESET;
            continue;
        }

        string priority;
        cout << "Enter patient priority (Critical / High / Normal): ";
        cin >> priority;
        transform(priority.begin(), priority.end(), priority.begin(), ::tolower);
        priority[0] = toupper(priority[0]);

        int chosenAmb = assignAmbulance(adjMatrix, ambulancePositions, patientIndex, buildings, priority, ambulanceAvailability);
        if (chosenAmb != -1) {
            int dist = dijkstra(adjMatrix, ambulancePositions[chosenAmb], patientIndex, prev);
            cout << "\n🛣️ Shortest Distance: " << GREEN << dist << " km" << RESET << "\n";
            printPath(ambulancePositions[chosenAmb], patientIndex, prev, buildings);

            int hosp = nearestHospital(adjMatrix, patientIndex, buildings, hospitalIndices);

            string logMsg = "Patient at " + buildings[patientIndex] + " | Priority: " + priority +
                            " | Ambulance: #" + to_string(chosenAmb) +
                            " | Distance: " + to_string(dist) +
                            " | Hospital: " + (hosp == -1 ? "N/A" : buildings[hosp]);
            logDispatch(logMsg);

            totalPatients++;
            totalDistance += dist;

            ambulanceAvailability[chosenAmb] = false;
            patientFeedback();
        }

        updateAmbulanceAvailability(ambulanceAvailability, -1, 0);

        cout << "\n🔁 Find another route? (Y/N): ";
        cin >> cont;
    }

    // Final Statistics
    printHeader("📊 Session Summary");
    cout << "Patients Served: " << YELLOW << totalPatients << RESET << "\n";
    cout << "Total Distance Covered: " << GREEN << totalDistance << " km" << RESET << "\n";
    if (totalPatients > 0)
        cout << "Average Distance per Patient: "
             << BLUE << (totalDistance / totalPatients) << " km" << RESET << "\n";

    // Save summary
    ofstream summary("summary.txt");
    summary << "Patients Served: " << totalPatients << "\n";
    summary << "Total Distance Covered: " << totalDistance << " km\n";
    if (totalPatients > 0)
        summary << "Average Distance per Patient: " << (totalDistance / totalPatients) << " km\n";
    summary.close();

    cout << GREEN << "\n✅ Program ended. Logs saved to logs.txt and summary.txt. Stay safe!\n" << RESET;
    return 0;
}
