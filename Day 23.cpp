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
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
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
struct Vertex {
	int row;
	int col;
	vector<pair<Vertex,int>> neighbors;
	Vertex(int r, int c) : row(r), col(c) {}
	Vertex operator + (Vertex& c) {
		return Vertex(row + c.row, col + c.col);
	}
	bool operator == (const Vertex& c) const {
		return row == c.row && col == c.col;
	}
};
std::ostream& operator << (std::ostream& os, const Vertex& s) {
	os << "(" << s.row << "," << s.col << ")";
	return os;
}
struct hasher {
	std::size_t operator() (const Vertex& vertex) const {
		return std::hash<int>()(vertex.row) ^ (std::hash<int>()(vertex.col) << 1);
	}
};
bool inBounds(Vertex& v, vector<string>& grid) {
	return v.row >= 0 && v.row < grid.size() && v.col >= 0 && v.col < grid[0].size();
}
bool isJunction(vector<string>& grid, Vertex v) {
	// look for patterns of the form 
	// #v#  or  >.>  or  #v# or #v#
	// >.>      #v#      >.#    #.> 
	//					 #v#    #v#
	vector<Vertex> shifts = {
		Vertex(1,0),
		Vertex(-1,0),
		Vertex(0,1),
		Vertex(0,-1)
	};
	for (Vertex& shift : shifts) {
		Vertex new_vertex = v + shift;
		if (!inBounds(new_vertex, grid) ) {
			return false;
		}
	}
	int row = v.row;
	int col = v.col;
	return (grid[row][col + 1] == '>' && grid[row][col - 1] == '>' && grid[row - 1][col] == 'v')
		|| (grid[row][col + 1] == '>' && grid[row][col - 1] == '>' && grid[row + 1][col] == 'v')
		|| (grid[row][col - 1] == '>' && grid[row + 1][col] == 'v' && grid[row - 1][col] == 'v')
		|| (grid[row][col + 1] == '>' && grid[row + 1][col] == 'v' && grid[row - 1][col] == 'v');
}
//Help struct to count the distances between two junctions
struct State {
	Vertex vertex;
	int weight;
	State() : vertex(Vertex(0,0)) ,weight(0) {}
	State(Vertex v, int steps) : vertex(v) , weight(steps) {}
};

// Collapse the grid into a DAG with vertices corresponding to junctions and the start/target vertex.
void findJunctionGraph(vector<string>& grid, Vertex root, Vertex end, unordered_set<Vertex, hasher>& seen, unordered_map<Vertex, vector<pair<Vertex, int>>, hasher>& dag) {
	//do a BFS and find all junctions (see the boolean function isJunction). 
	//Forms a smaller directed graph with vertices (0,1), "junctions", and (grid.size() - 1, grid[0].size() - 2)
	vector<Vertex> shifts = {
	Vertex(1,0),
	Vertex(-1,0),
	Vertex(0,1),
	Vertex(0,-1)
	};
	queue<State> q;
	State start;
	start.vertex = root;
	if (root == Vertex(0, 1)) {
		start.weight = -1;
	}
	else {
		start.weight = 0;
	}
	q.push(start);
	seen.insert(start.vertex);
	Vertex left(0, -1);
	Vertex up(-1, 0);
	Vertex right(0, 1);
	Vertex down(1, 0);
	while (!q.empty()) {
		State current = q.front();
		//If it is a junction, we add an edge from root to current.vertex with weight current.step+1 or current.step+2 (depending on the value of 
		if (  current.vertex == end ) {
			dag[root].push_back({ end,current.weight+1});
			dag[end];
			break;
		}
		else if ((isJunction(grid, current.vertex) || current.vertex==end)&& !(current.vertex == root) ) {
			dag[root].push_back({ current.vertex,current.weight +2});
			findJunctionGraph(grid, current.vertex, end, seen, dag);
		}
		q.pop();
		for (Vertex shift : shifts) {
			Vertex new_vertex = current.vertex + shift;
			Vertex s = shift;
			if (inBounds(new_vertex, grid) && grid[new_vertex.row][new_vertex.col] != '#' 
				&& seen.count(new_vertex)==0     ) {
				if (grid[new_vertex.row][new_vertex.col] == '>' &&  shift ==right   ) {
					State s(new_vertex, current.weight + 1);
					seen.insert(new_vertex);
					new_vertex = new_vertex + right;
					State s_right(new_vertex, current.weight + 1);
					seen.insert(new_vertex);
					q.push(s_right);
				}
				else if (  grid[new_vertex.row][new_vertex.col] == 'v' &&  shift == down  ){
					State s(new_vertex, current.weight + 1);
					seen.insert(new_vertex);
					new_vertex = new_vertex + down;
					State s_down(new_vertex, current.weight + 1);
					seen.insert(new_vertex);
					q.push(s_down);
				}
				else if(grid[new_vertex.row][new_vertex.col] == '.') {
					State s(new_vertex, current.weight + 1);
					seen.insert(new_vertex);
					q.push(s);
				}
			}
		}
	}
}

//This converts the DAG from part 1.
unordered_map<Vertex, vector<pair<Vertex, int>>, hasher> convertToUndirected(
	const unordered_map<Vertex, vector<pair<Vertex, int>>, hasher>& dag) {
	unordered_map<Vertex, vector<pair<Vertex, int>>, hasher> undirectedGraph;
	// Iterate through each vertex in the original DAG
	for (const auto& vertexEntry : dag) {
		const Vertex& currentVertex = vertexEntry.first;
		const vector<pair<Vertex, int>>& neighbors = vertexEntry.second;
		// Add the current vertex to the undirected graph
		undirectedGraph[currentVertex];
		// Iterate through each neighbor of the current vertex
		for (const auto& neighbor : neighbors) {
			// Add the edge from the current vertex to its neighbor
			undirectedGraph[currentVertex].push_back(neighbor);
			// Add the reverse edge from the neighbor to the current vertex
			undirectedGraph[neighbor.first].push_back({ currentVertex, neighbor.second });
		}
	}
	return undirectedGraph;
}


//I've seen a lot of people use bitsets/bitmaps to create a proper hash but I have no idea how to do that.
string createToken(unordered_set<Vertex, hasher> seen) {
	string token = "";
	for (const auto& v : seen) {
		token += "(" + to_string(v.row) + "," + to_string(v.col) + ")";
	}
	return token;
}
bool canReach(unordered_map<Vertex, vector<pair<Vertex, int>>, hasher>& graph, Vertex start, Vertex end, unordered_set<Vertex, hasher>& seen) {
	unordered_set<Vertex, hasher> visited = seen;
	queue<Vertex> q; 
	q.push(start); 
	visited.insert(start);
	while (!q.empty()) {
		Vertex current = q.front();
		q.pop();
		if (current == end) {
			return true;
		}
		for (auto& weighted_edge : graph[current]) {
			if (visited.count(weighted_edge.first) == 0) {
				q.push(weighted_edge.first);
				visited.insert(weighted_edge.first);
			}
		}
	}
	return false;
}
//This is really slow but outputs the right answer (after some time) don't know how to optimize this particular implementation
// can maybe be improved using a stack DFS instead. 
int longestPath(unordered_map<Vertex, vector<pair<Vertex, int>>, hasher>& graph,  Vertex& start, Vertex& end, 
					map<string, int>& DP, unordered_set<Vertex, hasher>& seen) {
	string token =  createToken(seen);
	if (DP.count(token) > 0) {
		return DP[token];
	}
	if (start == end) {
		return 1;
	}
	//These 2 if statements are for pruning.
	if (!graph[end].empty()) {
		//There is only one edge going out from the target vertex
		if (start == graph[end][0].first) {
			return graph[end][0].second;
		}
		int count = 0;
	}
	//Check if I can reach the vertex end if I already have visited a large proportion of the vertices. 
	if (seen.size() > graph.size()/3) {
		if (!canReach(graph, start, end, seen)) {
			DP[token] = 0;
			return 0;
		}
	}
	seen.insert(start);
	int len = 0;
	for (pair<Vertex, int>& edge : graph[start]) {
		if (seen.count(edge.first) == 0) {
			seen.insert(edge.first);
			len = max(len, edge.second + longestPath(graph, edge.first, end, DP, seen));
			seen.erase(edge.first);
		}
	}
	DP[token] = len;
	return len;
}


int main() {
	string input = "input.txt";
	vector<string> grid = readFile(input);
	Vertex start(0, 1);
	Vertex end(grid.size() - 1, grid[0].size() - 2);
	unordered_set<Vertex, hasher> seen;
	seen.insert(start);
	unordered_map<Vertex, vector<pair<Vertex, int>>, hasher> dag;
	findJunctionGraph(grid, start,end ,seen, dag);
	seen.clear();
	seen.insert(start);
	map<string, int> DP;
	cout<<longestPath(dag, start, end, DP, seen)-1<<endl;  // Counting the end twice 
	DP.clear();
	seen.clear();
	auto undirected = convertToUndirected(dag);
	cout << longestPath(undirected, start, end, DP, seen) <<endl; // no double counting here, takes longer than I want to (~2.5 hours) but it outputs the right answer
	// correct answer: 6738
}
