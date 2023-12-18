#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <vector>
#include <tuple>
#include <queue>
#include <map> 
#include <set>
using namespace std;
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
//represent beams as vector<pair<int,int>> 

struct particle {
	int row;
	int col; 
	pair<int, int> direction;
	bool has_moved = false;
};

void printParticle(particle& p) {
	cout << "Position (" << p.row << "," << p.col << "). Direction (" << p.direction.first << "," << p.direction.second << ")" << endl;
}
void printCovered(vector<string>& grid, set<pair<int, int>> covered) {
	for (int row = 0; row < grid.size(); row++) {
		for (int col = 0; col < grid.size(); col++) {
			auto search = covered.find({ row,col });
			if (search != covered.end()) {
				cout << '#';
			}
			else {
				cout << '.';
			}
		}
		cout << endl;
	}
}
void fill(set<pair<int, int>>& covered, int start_row, int start_col, int end_row, int end_col,vector<string>&grid) {
		int delta_row = (abs(end_row - start_row) > 0) ? ceil((end_row - start_row) / abs(end_row - start_row)) : 0;
		int delta_col = (abs(end_col - start_col) > 0) ? ceil((end_col - start_col) / abs(end_col - start_col)) : 0;
		int next_row = start_row;
		int next_col = start_col;
		while (next_row != end_row || next_col != end_col) {
			if (next_row >= 0 && next_row < grid.size() && next_col >= 0 && next_col < grid[0].size()) {
				covered.insert({ next_row,next_col });
			}
			next_row += delta_row;
			next_col += delta_col;
		}
		if (end_row >= 0 && end_row < grid.size() && end_col >= 0 && end_col < grid[0].size()) {
			covered.insert({ end_row,end_col });
		}
}
pair<int, int> getClosestObstacle(particle& b, vector<string>&grid) {
	int next_row = b.row+b.direction.first;
	int next_col = b.col+b.direction.second;
	//if we move along a column 
	if (b.direction.first != 0) {
		while (next_row>=0 && next_row<grid.size() && next_col >= 0 && next_col < grid[0].size() &&
				(grid[next_row][next_col] == '.' || grid[next_row][next_col] == '|')) {
			next_row += b.direction.first;
			next_col += b.direction.second;
		}
		if (next_row < 0 || next_row >= grid.size()) {
			return { -1,-1 };
		}
		else {
			return { next_row , next_col };
		}
	} // if we move along a row
	else if(b.direction.second != 0) {
		while ( next_row >= 0 && next_row < grid.size() && next_col >= 0 && next_col < grid[0].size() &&
			(grid[next_row][next_col] == '.' || grid[next_row][next_col] == '-') ) {
			next_row += b.direction.first;
			next_col += b.direction.second;
		}
		if (next_col < 0 || next_col >= grid.size()) {
			return { -1,-1 };
		}
		else {
			return { next_row, next_col };
		}
	}
	return { next_row,next_col };
}
void changeDirection(particle& b, char mirror) {
	if (mirror == '\\') {
		if (b.direction.first != 0) {
			if (b.direction.first > 0) { // if the beam travels this way ↓  and hits \  it will travel this way →  
				b.direction = { 0,1 };
			}
			else {// if the beam travels this way  ↑  and hits \  it will travel ←
				b.direction = { 0,-1 };
			}
		}
		else if (b.direction.second != 0) {
			if (b.direction.second != 0) {
				if (b.direction.second > 0) { // if the beam travels this way →  and hits \  it will travel ↓
					b.direction = { 1,0 };
				}
				else { // if the beam travels this way ← and hits \  it will travel ↑
					b.direction = { -1,0 };
				}
			}
		}
	}
	else if (mirror == '/') {
		if (b.direction.first != 0) {
			if (b.direction.first > 0) { // if the beam travels this way ↓  and hits /  it will travel this way ←   
				b.direction = { 0,-1 };
			}
			else {// if the beam travels this way  ↑  and hits /  it will travel →
				b.direction = { 0,1 };
			}
		}
		else if (b.direction.second != 0) {
			if (b.direction.second != 0) {
				if (b.direction.second > 0) { // if the beam travels this way →  and hits /  it will travel ↑
					b.direction = { -1,0 };
				}
				else { // if the beam travels this way ← and hits /  it will travel  ↓
					b.direction = { 1,0 };
				}
			}
		}
	}
}

void createNewParticle(vector<particle>& beams, vector<string>& grid, int row ,int col) {
	// if '-' (we know that beam travels along a column)
	particle first_particle;
	particle second_particle;
	if (grid[row][col] == '-') {
		first_particle.direction = { 0,-1 };
		second_particle.direction = { 0,1 };
	}
	// if '|' (we know that beam travels along columns) create a new beam
	else if (grid[row][col] == '|') {
		first_particle.direction = { -1,0 };
		second_particle.direction = { 1,0 };
	}

	first_particle.row = row;
	first_particle.col = col;
	second_particle.row = row;
	second_particle.col = col;
	beams.push_back(first_particle);
	beams.push_back(second_particle);
}
void printPair(pair<int, int> pos) {
	cout << "(" << pos.first << "," << pos.second << ")" << endl;
}

// This can probably be optimized
int simulate (vector<string>& grid, int init_row, int init_col, pair<int,int> init_dir ) {
	particle init_beam;
	init_beam.row = init_row;
	init_beam.col = init_col;
	init_beam.direction = init_dir;

	vector<particle> beams;
	beams.push_back(init_beam);
	particle current_beam;
	set<pair<int, int>> covered;
	// Might be possible to find a better termination condition than:  
	// if there are more beams active than there are sites we terminate
	while (!beams.empty() && beams.size()<grid.size()*grid[0].size()) {
		//grab the first element in the list 
		particle& current_beam = beams.front();
		pair<int, int> closest_obstacle = getClosestObstacle(current_beam, grid);
		int new_row = closest_obstacle.first;
		int new_col = closest_obstacle.second;
		// if there is no obstacle 
		if (new_row == -1 && new_col == -1 ) {
			// check direction of beam and then fill all of the sites in that direction. 
			if (current_beam.direction.first != 0) { // Either ↓ or ↑ 
				if (current_beam.direction.first > 0)  { // If beam travels ↓ we fill all of the sites downwards
					fill(covered, current_beam.row, current_beam.col, grid.size(), current_beam.col,grid);
				}
				else if (current_beam.direction.first < 0) {// If beam travels ↑ we fill all of the sites upwards
					fill(covered, current_beam.row, current_beam.col, 0, current_beam.col, grid);
				}    
			}
			else if (current_beam.direction.second != 0) { //  Either ← or →
				if (current_beam.direction.second > 0) { // If beam travels → we fill all of the sites forwards
					fill(covered, current_beam.row, current_beam.col, current_beam.row, grid[0].size(), grid);
				}
				else if (current_beam.direction.second < 0) {// If beam travels ← we fill all of the sites backwards
					fill(covered, current_beam.row, current_beam.col, current_beam.row, 0, grid);
				}
			}
			// then remove beam 
			beams.erase(beams.begin());
		}
		else {
			fill(covered, current_beam.row, current_beam.col, new_row, new_col, grid);
			if (grid[new_row][new_col] == '-' || grid[new_row][new_col] == '|') {
				createNewParticle(beams, grid, new_row,new_col);
				beams.erase(beams.begin());
			}
			else {
				current_beam.row = new_row;
				current_beam.col = new_col;
				changeDirection(current_beam, grid[new_row][new_col]);
			}
		}
	}
	return covered.size();
}



int main() {
	string input = "input.txt";
	vector<string> grid = readFile(input);
	// part 1
	cout<<simulate(grid,0,-1, {0,1})<<endl;
	// part 2 (slow but works, probably due to the slow simulate method)
	int largest = 0;
	int n;
	int m;
	for (int row = 0; row < grid.size(); row++) {
		n = simulate(grid, row, -1, { 0,1 });
		m = simulate(grid, row, grid[0].size(), { 0,-1 });
		largest = max(largest, max(n, m));
		cout << "Row " << row << endl;
	}
	for (int col = 0; col < grid[0].size(); col++) {
		n = simulate(grid, -1, col, { 1,0 });
		m = simulate(grid, grid.size(), col, {-1,0});
		largest = max(largest, max(n, m));
		cout << "Col " << col << endl;
	}
	cout << largest;
}
