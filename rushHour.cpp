#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>

using namespace std;

struct ItemProperties {
	string color;
	char orien;
	int row;
	int column;
};


void BFS(int start, int num, const vector<vector<int>>& adj) {
	// vector to keep track of visited nodes
	vector<bool> visited(num, false);
	// store nodes to be visited
	queue<int> q;

	// mark starting node as visited and then queue
	visited[start] = true;
	q.push(start);

	while (!q.empty()) {
		int current = q.front();
		q.pop();

		// explore all unvisited neighbors or current node
		for (int neighbor : adj[current]) {
			if (!visited[neighbor]) {
				visited[neighbor] = true;
				q.push(neighbor);
			}
		}
	}
}
	
// initalizing values
struct VehicleInfo {
	string type;
	string color;
	char orien;
	int length;
};

struct Position {
	int row;
	int col;
};
	
int main() {
	int numVehicle;
	cin >> numVehicle;
	unordered_map<string, ItemProperties> itemTable;
	vector<VehicleInfo> vehicles;
	vector<Position> positions;

	for (int i = 0; i < numVehicle; ++i) {
		string type, color;
		char orien;
		int row, col;
		int length;

		// getting input
		cin >> type >> color >> orien >> row >> col;

		// assigning length
		if (type == "car") {
			length = 2;
		}

		else {
			length = 3;
		}

		vehicles.push_back(VehicleInfo{ type, color, orien, length });
		positions.push_back(Position{ row-1, col-1 });
	}

	// print out data to test
	for (size_t i = 0; i < vehicles.size(); ++i) {
		cout << i << ") color=" << vehicles[i].color
			<< " type=" << vehicles[i].type
			<< " orient=" << vehicles[i].orien
			<< " len=" << vehicles[i].length
			<< " pos=(" << positions[i].row-1 << "," << positions[i].col-1 << ")\n";
	}

	// initialize board
	string board[6][6]; // ! i changed the board to a string so we can assign each location the color of the car
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 6; ++j) {
			board[i][j] = "empty";
		}
	}

	// fill board
	for (int i = 0; i < numVehicle; ++i) {
		int row = positions[i].row;
		int col = positions[i].col;
		int len = vehicles[i].length;
		char ori = vehicles[i].orien;
		string carColor = vehicles[i].color;

		for (int k = 0; k < len; ++k) {
			// horizontal
			if (ori == 'h') {
				board[row][col + k] = carColor; // location of cars and trucks in array will have their color
			}

			// vertical 
			else{
				board[row + k][col] = carColor;
			}
		}
	}

	return 0;
}