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

// initalizing values
struct VehicleInfo {
	string type;
	string color;
	char orien;
	int length;
	int row;
	int col;
};

// ? this converts our 2d array into a string so we can use it as a key in our hash map
string vectorToString(vector<vector<int>> board) {
	string keyString = "";
	for (int i=0; i<6; i++) {
		for (int j=0; j<6; j++) {
			keyString += to_string(board[i][j]) + ',';
		}
	}

	return keyString;
}

bool checkValid(int targetRow, int targetCol, vector<vector<int>>& board) {
	return targetRow < 6 && targetRow >= 0 && targetCol < 6 && targetCol >= 0 && board[targetRow][targetCol] == -1;
}

void checkPossibleMoves(vector<vector<int>>& board, int row, int col, int len, vector<VehicleInfo>& newVehicleInfo, char direction, queue<vector<vector<int>>>& boardStatesQueue, 
	unordered_map<string, vector<VehicleInfo>>& boardInformation, unordered_map<string, string>& listOfMoves, int carNumber, unordered_map<string, vector<vector<int>>>& parent,
	unordered_map<string, bool>& visited) {
	// ? if this is true we can move forward
	int count = 0;
	bool valid;
	bool solutionFound = false;
	vector<vector<int>> newBoardState = board;
	string key;
	string oldKey;

	if (direction == 'L') {
		valid = checkValid(row, col-1, board);
	} else if (direction == 'R') {
		valid = checkValid(row, col+len, board);
	} else if (direction == 'U') {
		valid = checkValid(row-1, col, board);
	} else {
		valid = checkValid(row+len, col, board);
	}

	while (valid) {
		// ? were valid so we can increase count
		count++;
		
		// ? move the vehicle to the direction by one
		//newBoardState[row][col] = carNumber;
		if (direction == 'L') {
			newBoardState[row][col-1] = carNumber;
			newBoardState[row][col+len-1] = -1;
			newVehicleInfo[carNumber].col = col-1;
			valid = checkValid(newVehicleInfo[carNumber].row, newVehicleInfo[carNumber].col-1, newBoardState);
			if (valid) col = newVehicleInfo[carNumber].col;
		}
		else if (direction == 'R') {
			newBoardState[row][col+len] = carNumber;
			newBoardState[row][col] = -1;
			newVehicleInfo[carNumber].col = col+1;
			valid = checkValid(newVehicleInfo[carNumber].row, newVehicleInfo[carNumber].col+len, newBoardState);
			if (valid) col = newVehicleInfo[carNumber].col;

			// todo: check if the red car can get all the way to [2][5]
			if (carNumber == 0 && newVehicleInfo[carNumber].col == 5) {
				// ! this is solution, print out path
				solutionFound = true;
				cout << "done";
				break;
			}
		}
		else if (direction == 'U') {
			newBoardState[row-1][col] = carNumber;
			newBoardState[row+len-1][col] = -1;
			newVehicleInfo[carNumber].row = row-1;
			valid = checkValid(newVehicleInfo[carNumber].row-1, newVehicleInfo[carNumber].col, newBoardState);
			if  (valid) row = newVehicleInfo[carNumber].row;

		}
		else {
			newBoardState[row+len][col] = carNumber;
			newBoardState[row][col] = -1;
			newVehicleInfo[carNumber].row = row+1;
			valid = checkValid(newVehicleInfo[carNumber].row+len, newVehicleInfo[carNumber].col, newBoardState);
			if (valid) row = newVehicleInfo[carNumber].row;
		}

		// ? convert the 2d vector to a string to use as a key
		key = vectorToString(newBoardState);
	}

	// ? assign the vehicle info to this state to the string of this state in a hash map
	boardInformation[key] = newVehicleInfo;

	// ? assign the move you made here in relation to this new state
	listOfMoves[key] = newVehicleInfo[carNumber].color + " " + to_string(count) + " " + direction;

	// ? push new state onto the queue
	if (visited[key] == false) {
		boardStatesQueue.push(newBoardState);
		visited[key] = true;
	}

	// ? sets the parent of the current board to the board it derived from
	parent[key] = board;

	if (carNumber == 0) {
		// ? printing board
		cout << "-----------------------------------------------------------------" << endl;
		for (int i=0; i<6; ++i) {
			for (int j=0; j<6; ++j) {
				cout << board[i][j] << "\t";
			}
			cout << "\n";
		}
	}

	if (solutionFound) {
		// todo: backtrack and print out the listOfMoves at each point
		vector<string> printVector;

		cout << "we did itttt!" << endl;
	}
}

// ? we pass in the number of vehicles, info about each vehicle, and the current board state holding each vehicle
void puzzleSolve(int numOfVehicles, const vector<VehicleInfo>& vehicles, vector<vector<int>> board) {
	// todo: start at 0, red car, and work up to num of vehicles, making possible moves and storing them in a queue
	queue<vector<vector<int>>> boardStatesQueue;
	unordered_map<string, vector<VehicleInfo>> boardInformation;
	unordered_map<string, string> listOfMoves;
	string key;
	vector<VehicleInfo> newVehicleInfo;
	vector<vector<int>> newBoardState;
	unordered_map<string, vector<vector<int>>> parent; // ? this will allow us to assign parents to then have us backtrack at the end
	unordered_map<string, bool> visited;

	boardStatesQueue.push(board); // ? this will be the first board
	string initialBoardString = vectorToString(board);
	boardInformation[initialBoardString] = vehicles;
	vector<vector<int>> rootBoard(6, vector<int>(6, 0));
	parent[initialBoardString] = rootBoard;

	// ! this goes depth 1 of checking vehicles for their possible moves
	while (!boardStatesQueue.empty()) {
		board = boardStatesQueue.front(); // ? getting the board at the front of the queue
		key = vectorToString(board);
		newVehicleInfo = boardInformation[key];
		for (int i=0; i</*numOfVehicles*/2; i++) {
			if (vehicles[i].orien == 'h') {
				// check left
				cout << i;
				checkPossibleMoves(board, newVehicleInfo[i].row, newVehicleInfo[i].col, newVehicleInfo[i].length, newVehicleInfo, 'L', boardStatesQueue, boardInformation, listOfMoves, i, parent, visited);
				// check right
				checkPossibleMoves(board, newVehicleInfo[i].row, newVehicleInfo[i].col, newVehicleInfo[i].length, newVehicleInfo, 'R', boardStatesQueue, boardInformation, listOfMoves, i, parent, visited);
			} else {
				// check up
				checkPossibleMoves(board, newVehicleInfo[i].row, newVehicleInfo[i].col, newVehicleInfo[i].length, newVehicleInfo, 'U', boardStatesQueue, boardInformation, listOfMoves, i, parent, visited);
				//check down
				checkPossibleMoves(board, newVehicleInfo[i].row, newVehicleInfo[i].col, newVehicleInfo[i].length, newVehicleInfo, 'D', boardStatesQueue, boardInformation, listOfMoves, i, parent, visited);
			}
		}

		// ? pop off the first element in the queue because we have finished that breadth level
		boardStatesQueue.pop();
	}
}
	
int main() {
	int numVehicle;
	cin >> numVehicle;
	unordered_map<string, ItemProperties> itemTable;
	vector<VehicleInfo> vehicles;
	//vector<Position> positions;

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

		vehicles.push_back(VehicleInfo{ type, color, orien, length, row-1, col-1 });
		/*positions.push_back(Position{ row-1, col-1 });*/
	}

	// print out data to test
	for (size_t i = 0; i < vehicles.size(); ++i) {
		cout << i << ") color=" << vehicles[i].color
			<< " type=" << vehicles[i].type
			<< " orient=" << vehicles[i].orien
			<< " len=" << vehicles[i].length
			<< " pos=(" << vehicles[i].row << "," << vehicles[i].col << ")\n";
	}

	// initialize board
	vector<vector<int>> board(6, vector<int>(6, 0));
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 6; ++j) {
			board[i][j] = -1;
		}
	}

	// fill board
	for (int i = 0; i < numVehicle; ++i) {
		int row = vehicles[i].row;
		int col = vehicles[i].col;
		int len = vehicles[i].length;
		char ori = vehicles[i].orien;

		for (int k = 0; k < len; ++k) {
			// horizontal
			if (ori == 'h') {
				board[row][col + k] = i; // location of cars and trucks in array will have their color
			}

			// vertical 
			else{
				board[row + k][col] = i;
			}
		}
	}

	// ? printing board
	for (int i=0; i<6; ++i) {
		for (int j=0; j<6; ++j) {
			cout << board[i][j] << "\t";
		}
		cout << "\n";
	}

	// call the solve function
	puzzleSolve(numVehicle, vehicles, board);

	return 0;
}