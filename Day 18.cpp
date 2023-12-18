// Day 18.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
vector<string> splitString(string str, char delimiter) {
	vector<string> tokens;
	istringstream iss(str);
	string token;
	while (getline(iss, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

struct Vertex {
	long long row;
	long long col;
	Vertex() : row(0), col(0) {}
	Vertex(const long long r, const long long c) : row(r), col(c) {}
	Vertex operator + (const Vertex& p) const {
		return Vertex(p.row + row, p.col + col);
	}
	Vertex operator - (const Vertex& p) const {
		return Vertex(row,col) +Vertex(-p.row , -p.col );
	}
	Vertex operator * (const long long c) const {
		return Vertex(row * c, col * c);
	}
	bool operator<(const Vertex& other) const {
		return (row < other.row) || ((row == other.row) && (col < other.col));
	}
	bool operator == (const Vertex& p) const {
		return row == p.row && col == p.col;
	}
};

struct hasher {
	std::size_t operator() (const Vertex& vertex) const {
		return std::hash<int>()(vertex.row) ^ (std::hash<int>()(vertex.col) << 1);
	}
};


struct Edge {
	pair<Vertex, Vertex> edge; 
	string color;
	Edge(const pair<Vertex,Vertex> edge,const string color) : edge(edge), color(color){}
	Edge operator + (const Vertex& v) const {
		return Edge({ edge.first + v, edge.second + v }, color);
	}
	Edge operator - (const Vertex& v) const {
		return Edge({ edge.first , edge.second  }, color) + Vertex({ -v.row,-v.col }) ;
	}

};
void printVertex(Vertex v) {
	cout << "(" << v.row << "," << v.col << ")";
}
void printEdge(Edge e) {
	Vertex v_1 = e.edge.first;
	Vertex v_2 = e.edge.second;
	printVertex(v_1);
	cout << "->" ;
	printVertex(v_2);
	cout << " edge color: " << e.color;
}
// Sanity check function
void drawGrid(vector<Vertex> vertices, Vertex dimensions) {
	for (int row = 0; row <= dimensions.row; row++) {
		for (int col = 0; col <= dimensions.col; col++) {
			Vertex v(row, col);
			auto it = find(vertices.begin(), vertices.end(), v);
			if (it != vertices.end()) {
				cout << '#';
			}
			else {
				cout << '.';
			}
		}
		cout << endl;
	}
}
// A sanity check function
void findIntersections(vector<Vertex> vertices) {
	for (int i = 0; i < vertices.size(); i++) {
		for (int j = i + 1; j < vertices.size(); j++) {
			if (vertices[i] == vertices[j]) {
				cout << "Vertices " << endl;
				printVertex(vertices[i]);
				cout << " and ";
				printVertex(vertices[j]);
				cout << "intersect" << endl;
			}
		}
	}
}

// A lot of this is unnecessary after part 2. 
tuple<vector<Vertex>, vector<Edge>, Vertex > dig(vector<string> instructions, bool part_2) {
	map<char, Vertex> convertInstruction={
	{'U',Vertex(-1,0)},
	{'D',Vertex(1,0)},
	{'L',Vertex(0,-1)},
	{'R',Vertex(0,1)}
	};	
	map<char, char> part2_conversion = {
		{'0','R'},
		{'1','D'},
		{'2','L'},
		{'3','U'}
	};
	Vertex start(0, 0);
	vector<Vertex> vertices;
	vertices.push_back(start);
	vector<Edge> edges;
	// Parse and first run through
	int min_row = 0;
	int max_row = 0;
	int min_col = 0;
	int max_col = 0;
	int number_of_steps = 0;
	Vertex direction;
	string color;
	for (string& instruction : instructions) {
		Vertex last_vertex = vertices.back();
		if (!part_2) {
			vector<string> split_instructions = splitString(instruction, ' ');
			direction = convertInstruction[split_instructions[0][0]];
			number_of_steps = stoi(split_instructions[1]);
			 color = split_instructions[2];
		}
		else {
			//This part 2 piece is much better and can be used for part 1 as well
			// if one refactors apropriately 
			vector<string> split_instructions = splitString(instruction, '#');
			string hexa = split_instructions[1];
			color = hexa;
			hexa.pop_back();
			direction = convertInstruction[part2_conversion[hexa.back()]];
			number_of_steps = stoi(hexa.substr(0, 5),0,16);
			Vertex new_vertex = last_vertex + direction*(number_of_steps);
			vertices.push_back(new_vertex);
			last_vertex = new_vertex;
		}
		int step = 0;
		while(step<number_of_steps && !part_2){
			Vertex new_vertex = last_vertex + direction;
			min_row = min(min_row, (int) new_vertex.row);
			max_row = max(max_row, (int) new_vertex.row);
			min_col = min(min_col, (int) new_vertex.col);
			max_col = max(max_col, (int) new_vertex.col);
			Edge new_edge({ last_vertex, new_vertex },color);
			vertices.push_back(new_vertex);
			edges.push_back(new_edge);
			last_vertex = new_vertex;
			step++;
			//cout << vertices.size() << endl;
		}
	}
	//Then we shift everything so that the vertex with the smallest row and col is at (0,0) 
	Vertex min_vertex(min_row, min_col);
	Vertex max_vertex(max_row - min_row, max_col - min_col);
	vector<Vertex> shifted_vertices; 
	vector<Edge> shifted_edges; 
	for (Vertex v : vertices) {
		Vertex new_vertex = v - min_vertex;
		shifted_vertices.push_back(new_vertex);
	}
	for (Edge e : edges) {
		Edge new_edge = e - min_vertex;
		shifted_edges.push_back(new_edge);
	}
	return { vertices,edges, max_vertex };
}

bool is_inside(Vertex& interior, Vertex& dimensions) {
	return interior.row >= 0 && interior.row <= dimensions.row && interior.col>=0 && interior.col <= dimensions.col;
}
// flood fill algorithm for part 1 using a BFS type method
set<Vertex> fill(unordered_set<Vertex, hasher>& loop, Vertex& dimensions, Vertex start) {
	unordered_set<Vertex, hasher> visited;
	set<Vertex> interior_vertices;
	vector<Vertex> shifts = {
		Vertex(1,0),
		Vertex(-1,0),
		Vertex(0,1),
		Vertex(0,-1)
	};
	queue<Vertex> q;
	q.push(start);
	visited.insert(start);
	while (!q.empty()) {
		Vertex current_vertex = q.front();
		interior_vertices.insert(current_vertex);
		q.pop();
		for (Vertex shift : shifts) {
			Vertex shifted_vertex = current_vertex + shift;
			if (is_inside(shifted_vertex, dimensions) 
				&& loop.find(shifted_vertex) ==loop.end() 
				&& visited.find(shifted_vertex)==visited.end()) {
				visited.insert(shifted_vertex);
				q.push(shifted_vertex);
			}
		}
	}
	return interior_vertices;
}
//Finds an interior vertex and then calls fill(...) which is basically a BFS search of the interior.
set<Vertex> findInterior(vector<Vertex> loop, Vertex dimensions) {
	
	set<Vertex> interior;
	vector<Vertex> slice;
	unordered_set<Vertex, hasher> loop_hash;
	for (Vertex& v : loop) {
		loop_hash.insert(v);
	}

	for (int row = 1; row < dimensions.row; row++) {
		int number_of_walls = 0;
		bool is_inside = false;
		for (int col = 0 ; col <= dimensions.col; col++) {
			Vertex boundary_vertex(row, col);
			auto is_loop = find(loop.begin(), loop.end(), boundary_vertex);
			if (is_loop != loop.end()) {
				slice.push_back(boundary_vertex);
			}
		}
		if (slice.size() == 2 && abs(slice[0].col - slice[1].col) > 1) {
			break;
		}
		else {
			slice.clear();
		}
	}
	interior = fill(loop_hash, dimensions, slice[0] + Vertex(0, 1));
	return interior;
}

// had to rethink for part 2, googled and found the shoelace formula+pick's theorem
void shoelaceAlgorithm(vector<Vertex>& loop) {
	// I use twice the area to avoid rounding errors when dividing by 2 at every iteration
	long double double_area = 0; 
	long long boundary_vertices = 0;
	for (int i = 0; i < loop.size() - 1; i++) {
		Vertex first = loop[i];
		Vertex second = loop[i + 1];
		double_area += (first.row * second.col - second.row * first.col);
		boundary_vertices += (first.row == second.row) ? abs(first.col - second.col) : abs(first.row - second.row);
	}
	double_area += loop.back().row * loop[0].col - loop[0].row * loop.back().col;
	long long interior_points = abs(double_area/2) - boundary_vertices/2 + 1 ;
	boundary_vertices += (loop[0].row == loop.back().row) ?  abs(loop[0].col - loop.back().col) : abs(loop[0].row - loop.back().row);
	cout << interior_points+ boundary_vertices   << endl;
	// Area = interior vertices +boundary_integers/2-1, pick's theorem
	
	/*
	double_area = 0;
	boundary_vertices = 0;
	This part is superfluous but it was a sanity check
	reverse(loop.begin(), loop.end());
	for (int i = 0; i < loop.size() - 1; i++) {
		Vertex first = loop[i];
		Vertex second = loop[i + 1];
		double_area += (first.row * second.col - second.row * first.col) ;
		boundary_vertices += (first.row == second.row) ? abs(first.col - second.col) : abs(first.row - second.row);
	}
	double_area += loop.back().row * loop[0].col - loop[0].row * loop.back().col;
	boundary_vertices += (loop[0].row == loop.back().row) ? abs(loop[0].col - loop.back().col) : abs(loop[0].row - loop.back().row);
	interior_points = abs(double_area / 2) - boundary_vertices / 2 + 1;
	cout << interior_points + boundary_vertices << endl; */
}

int main() {
	string input = "input.txt";
	vector<string> instructions = readFile(input);
	auto result = dig(instructions,true);
	vector<Vertex> boundary = get<0>(result);
	vector<Edge> edges = get<1>(result);
	Vertex dimensions = get<2>(result);
	shoelaceAlgorithm(boundary);
	// Right answer: 54662804037719
}