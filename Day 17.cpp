#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <algorithm>
#include <climits>
#include <limits>
#include <utility>
#include <cctype>
#include <queue>
#include <chrono>
#include <set>
#include <unordered_set>
using namespace std;

// Didn't figure out how to do this one, so I essentially borrowed
//  the solution from this one https://github.com/vss2sn/advent_of_code/blob/master/2023/cpp/day_17a.cpp
// I also learned a lot from this person's solution 

const int INF = numeric_limits<int>::max();
struct Vertex{
	int row;
	int col;
	Vertex() : row(0), col(0) {}
	Vertex(const int r, const int c) : row(r), col(c) {}
	Vertex operator + (const Vertex& p) const {
		return Vertex(p.row + row, p.col + col);
	}
	bool operator<(const Vertex& other) const {
		return (row < other.row) || ((row == other.row) && (col < other.col));
	}
	bool operator == (const Vertex& p) const {
		return row == p.row && col == p.col;
	}
};

 map<Vertex, vector<Vertex>> directions{
	{Vertex(1,0), {Vertex(1,0),Vertex(0,1),Vertex(0,-1)}},
	{Vertex(-1,0), {Vertex(-1,0),Vertex(0,1),Vertex(0,-1)}},
	{Vertex(0,1), {Vertex(1,0),Vertex(-1,0),Vertex(0,1)}},
	{Vertex(0,-1), {Vertex(1,0),Vertex(-1,0),Vertex(0,-1)}},
};

struct State {
	Vertex pos = Vertex(0, 0);
	Vertex direction = Vertex(0,1);
	int cons_moves = 0;
	size_t heat_loss = 0;
	bool operator == (const State& state) const {
		return state.pos == pos && state.cons_moves == cons_moves && state.direction == direction;
	}
};

struct Comparator {
	bool operator() (const State& s1, const State& s2) const {
		return s1.heat_loss > s2.heat_loss;
	}
};
struct hasher {
	std::size_t operator() (const State& state) const {
		return std::hash<int>()(state.pos.row) ^ (std::hash<int>()(state.pos.col) << 1);
	}
};
vector<string> readFile(string str) {
	vector< string > input;
	ifstream inputFile(str);
	if (!inputFile.is_open()) {
		// Handle the error if the file couldn't be opened
		cerr << "Error opening the file." << endl;
		return input;
	}
	string line;
	while (getline(inputFile, line)) {
		input.push_back(line);
	}
	inputFile.close();
	return input;
}


vector<vector<int>> convertToGrid(vector<string> input) {
	vector<vector<int>> grid; 
	for (int row = 0; row < input.size(); row++) {
		vector<int> new_row; 
		for (int col = 0; col < input[0].size(); col++) {
			new_row.push_back(input[row][col] - '0');
		}
		grid.push_back(new_row);
	}
	return grid;
}


bool is_inside(vector<vector<int>>& grid, Vertex c) {
	return c.row >= 0 && c.row < grid.size() && c.col >= 0 && c.col < grid[0].size();
}

vector<Vertex> findShortestPath(vector<vector<int>>& grid, Vertex start, Vertex end) {
	map<Vertex, int> distances;
	map<Vertex, Vertex> previous;
	priority_queue< State, vector<State>, Comparator > pq;
	std::unordered_set<State, hasher> seen;

	distances[start] = 0;
	State state;

	state.pos = start;
	state.direction = Vertex(0,1);
	state.cons_moves = 0;
	state.heat_loss = 0;
	pq.push(state);
	state.direction = Vertex(1,0);
	pq.push(state);

	while (!pq.empty()) {
		//Get the best state: 
		State state = pq.top();
		pq.pop();
		if (seen.find(state) != seen.end()) {
			continue;
		}
		seen.insert(state);

		if (state.pos == end) {
			Vertex coord = state.pos;
			vector<Vertex> shortest_path;
			shortest_path.push_back(state.pos);
			Vertex previous_coord = previous[coord];
			while (! (previous_coord == start) ) {
				shortest_path.push_back(previous_coord);
				coord = previous[previous_coord];
				previous_coord = coord;

			}
			shortest_path.push_back(previous_coord);
			cout << state.heat_loss;
			return shortest_path;
		}
		// this is the modification for part 2, remove the if statement for part 1
		if (state.cons_moves <  3) { 
			Vertex new_pos = state.pos + state.direction;
			if (is_inside(grid, new_pos)) {
				State new_state;
				new_state.pos = new_pos;
				new_state.direction = state.direction;
				new_state.heat_loss = state.heat_loss + grid[new_pos.row][new_pos.col];
				new_state.cons_moves = state.cons_moves + 1;
				pq.push(new_state);
			}
		}
		else {
			for (Vertex& step : directions[state.direction]) {
				Vertex new_pos = state.pos + step;
				if (is_inside(grid, new_pos)) {
					if (step == state.direction) {
						// change this if statement to state.cons_moves <  3 for part 1
						if (state.cons_moves <  9) { 
							//cout << "Hello!" << state.cons_moves<<endl;
							State new_state;
							new_state.pos = new_pos;
							new_state.direction = step;
							new_state.heat_loss = state.heat_loss + grid[new_pos.row][new_pos.col];
							new_state.cons_moves = state.cons_moves + 1;
							pq.push(new_state);
						}
					}
					else {
						State new_state;
						new_state.pos = new_pos;
						new_state.direction = step;
						new_state.heat_loss = state.heat_loss + grid[new_pos.row][new_pos.col];
						new_state.cons_moves = 0;
						pq.push(new_state);
					}
				}
			}
		}
		

	}

}
int main() {
	string input = "input.txt";
	vector<string> data = readFile(input);
	vector<vector<int>> grid = convertToGrid(data);
	findShortestPath(grid, Vertex(0, 0), Vertex(grid.size() - 1, grid[0].size() - 1));


}

