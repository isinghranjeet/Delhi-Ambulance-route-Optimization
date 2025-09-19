# 🚑 Ambulance Route Optimizer - Delhi

A C++ program that simulates and optimizes emergency ambulance dispatch in Delhi using Dijkstra’s algorithm. It reads building names and a city distance matrix from files and dynamically assigns the most suitable ambulance to patients based on location, priority, traffic conditions, and availability.

---

## 📌 Features

- 📍 Displays all available buildings/locations from file  
- 📊 Visualizes the adjacency matrix of distances between locations  
- 🚑 Assigns ambulances based on shortest weighted path using Dijkstra’s algorithm  
- 🏥 Recommends the nearest hospital to patient location  
- ⚖️ Priority-based dispatching: `Critical`, `High`, or `Normal`  
- 🧭 Simulates traffic and ETA based on random speed and conditions  
- 💬 Provides random patient feedback after each dispatch  
- 📁 Logs all dispatches and generates a session summary  

---

## 🛠️ Project Structure

```
.
├── main.cpp             # C++ source file (your main program)
├── buildings.txt        # List of building/location names
├── Dataset.txt          # Adjacency matrix of distances (-1 for no path)
├── logs.txt             # Dispatch logs (generated)
├── summary.txt          # Summary of session (generated)
└── README.md            # Project documentation
```

---

## 📥 Input Files

### `buildings.txt`
List of all locations/buildings (one per line):

```
AIIMS
Connaught Place
Karol Bagh
Fortis Hospital
India Gate
Apollo Hospital
...
```

### `Dataset.txt`
Adjacency matrix representing distances (in km) between locations.

- `-1` indicates no direct route.  
- Diagonal entries (distance to itself) are `0`.  
- The matrix must be square (NxN) where N = number of buildings.

Example:
```
0 10 -1 15 -1 30
10 0 5 -1 20 -1
-1 5 0 -1 -1 10
15 -1 -1 0 10 5
-1 20 -1 10 0 -1
30 -1 10 5 -1 0
```

---

## 🚀 How to Run

1. **Compile the program** using g++ (or any C++ compiler):

```bash
g++ -o ambulance main.cpp
```

2. **Make sure** `buildings.txt` and `Dataset.txt` are in the same directory.

3. **Run the executable:**

```bash
./ambulance
```

4. **Follow the prompts** to input patient location and priority.

---

## 🎯 Sample Usage

```
🚑 AMBULANCE ROUTE OPTIMIZER - DELHI 🚑

📍 Available Locations
 0 → AIIMS
 1 → Connaught Place
 2 → Karol Bagh
 ...

Enter patient location index (0-5): 2
Enter patient priority (Critical / High / Normal): Critical

🚑 Ambulance Candidates
 → #0 at AIIMS → Distance: 8 km | ETA: 10 mins
 → #1 at Fortis Hospital → Distance: 5 km | ETA: 6 mins

✅ Best Ambulance: #1 at Fortis Hospital → Weighted Distance: 3 km

🛣️  Path: Fortis Hospital ➔ Karol Bagh
🏥 Nearest Hospital: Apollo Hospital (Distance: 10 km)

💬 Patient Feedback: Very professional and helpful ambulance staff.
```

---

## 📦 Output Files

- **`logs.txt`** — Contains a log of each ambulance dispatch  
- **`summary.txt`** — Summary of total patients served and distances  

---

## 📈 Session Summary Example

```
📊 Session Summary
Patients Served: 3
Total Distance Covered: 26 km
Average Distance per Patient: 8 km

✅ Program ended. Logs saved to logs.txt and summary.txt. Stay safe!
```

---

## 🧠 Algorithms Used

- **Dijkstra’s Algorithm**: For computing shortest paths between nodes in a graph  
- **Priority Weighting**: Priority affects the "cost" to optimize ambulance selection  
- **Simulated Traffic**: Randomized traffic multiplier and ETA for realism  

---

## 🔧 Future Improvements

- 🌐 GUI or Web Integration  
- 📡 Real-time GPS or traffic APIs  
- 🗺️ Interactive map visualization  
- 🧑‍⚕️ Staff allocation or hospital capacity tracking  
