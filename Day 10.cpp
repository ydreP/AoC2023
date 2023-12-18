// Day 10.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <set>
#include <map>
#include <algorithm>
#include <climits>
#include <queue>
#include <cctype>
#include <regex>
#include <tuple>
#include <numeric>
#include <stack>

std::vector<std::string> readFile(std::string str) {
	std::vector< std::string > input;
	std::ifstream inputFile(str);
	if (!inputFile.is_open()) {
		// Handle the error if the file couldn't be opened
		std::cerr << "Error opening the file." << std::endl;
		return input;
	}
	std::string line;
	while (std::getline(inputFile, line)) {
		if (line != "") {
			// Process the line here
			input.push_back(line);
		}
	}
	inputFile.close();
	return input;
}
void printPair(std::pair<int, int> point) {
	std::cout << "(" << point.first << "," << point.second << ") \n";
}
void printGrid(std::vector<std::string> grid) {
	for (std::string& row : grid) {
		std::cout << row << std::endl;
	}
}

std::pair<int, int> findStartingPoint(std::vector<std::string>& grid) {
	std::pair<int, int> position;
	for (int row = 0; row < grid.size(); row++) {
		for (int col = 0; col < grid[0].size(); col++) {
			if (grid[row][col] == 'S') {
				position.first = row;
				position.second = col;
			}
		}
	}
	return position;
}

std::vector<std::pair<int, int>> findNeighbors(std::vector<std::string>& grid, std::pair<int, int> current_pos) {
	std::map<char, std::vector<std::pair<int, int> > > tile_edges =
	{
		{'|',{ {1,0}, {-1,0}  } },
		{'-',{ {0,1}, {0,-1}  } },
		{'L',{ {-1,0},{0,1 }  } },
		{'J',{ {-1,0},{0,-1}  } },
		{'7',{ {1,0}, {0,-1}  } },
		{'F',{ {1,0}, {0,1 }  } }
	};

	std::vector<std::pair<int, int>> neighbors;
	std::vector<std::vector<int>> shifts = { {1,0},{0,1},{-1,0},{0,-1} };
	int row_pos = current_pos.first; 
	int col_pos = current_pos.second;
	//for each neighbor
	for (auto& shift : shifts) {
		if (row_pos+shift[0] >= 0 && row_pos + shift[0] < grid.size() && col_pos+shift[1] >= 0 && col_pos + shift[1] < grid[0].size()) {
			auto neighbor_edges
				= tile_edges[ grid[row_pos + shift[0]][col_pos + shift[1]] ];
			for (std::pair<int, int>& edge : neighbor_edges) {
				if (shift[0] + edge.first == 0 && shift[1] + edge.second == 0) {
					neighbors.push_back({ row_pos + shift[0] ,col_pos + shift[1] });
				}
			}
		}
	}
	return neighbors;
}



std::vector< std::pair<int, int>> findLoop(std::vector<std::string> grid) {
	std::map<char, std::vector<std::pair<int, int> > > tile_edges =
	{
		{'|',{ {1,0}, {-1,0}  } },
		{'-',{ {0,1}, {0,-1}  } },
		{'L',{ {-1,0},{0,1 }  } },
		{'J',{ {-1,0},{0,-1}  } },
		{'7',{ {1,0}, {0,-1}  } },
		{'F',{ {1,0}, {0,1 }  } }
	};

	std::vector< std::pair<int, int>> loop;

	//Keep track of all visited places
	std::vector<std::vector<bool>> is_visited(grid.size(), std::vector<bool>(grid[0].size(), false));

	//Find starting point
	std::pair <int, int> current_position = findStartingPoint(grid);
	//Add it to the loop
	loop.push_back(current_position);
	//Add starting position to visited nodes
	is_visited[current_position.first][current_position.second] = true;
	bool first_loop = true;
	// Inital step (will we need to determine what type S is? 	)
	auto first_neighbors = findNeighbors(grid, current_position);
	current_position = first_neighbors[0];
	is_visited[current_position.first][current_position.second] = true;
	loop.push_back(current_position);
	//int steps = 0;
	while(  (  first_loop || grid[current_position.first][current_position.second] != 'S') ) {
		char tile = grid[current_position.first][current_position.second];
		auto neighbor_edges = tile_edges[tile];
		for (std::pair<int, int> neighbor : neighbor_edges) {
			if ( !is_visited[current_position.first + neighbor.first][current_position.second + neighbor.second]) {
				current_position = { current_position.first + neighbor.first,current_position.second + neighbor.second };
				loop.push_back(current_position);
				is_visited[current_position.first ][current_position.second ] = true;
				break;
			}
			if (!first_loop  && grid[current_position.first + neighbor.first][current_position.second + neighbor.second] == 'S') {
				return loop;
			}
		}
		first_loop = false;
	}
	return loop;
}



char determineStartType(std::vector<std::pair<int, int>> loop) {
	std::map<char, std::vector<std::pair<int, int> > > tile_edges =
	{
		{'|',{ {1,0}, {-1,0}  } },
		{'-',{ {0,1}, {0,-1}  } },
		{'L',{ {-1,0},{0,1 }  } },
		{'J',{ {-1,0},{0,-1}  } },
		{'7',{ {1,0}, {0,-1}  } },
		{'F',{ {1,0}, {0,1 }  } }
	};

	std::vector<std::pair<int, int>> edges 
		= { {loop[1].first -loop[0].first ,  loop[1].second- loop[0].second },
			{ loop.back().first- loop[0].first , loop.back().second - loop[0].second  } };
	for (auto& key_val : tile_edges) {
		if (key_val.second == edges) {
			return key_val.first;
		}
	}
	std::reverse(edges.begin(), edges.end());
	for (auto& key_val : tile_edges) {
		if (key_val.second == edges) {
			return key_val.first;
		}
	}
	return ' ';
}
int computeInteriorOfRow(std::string& grid_row) {
	int interior = 0;
	std::vector<int> walls;
	// Possible walls:  '|', 'L-----7', 'F----J' (L---J is not for instance)
	int pos = 0;
	std::string wall_patterns = "[|]|L-*7|F-*J";
	std::regex pattern(wall_patterns);
	std::sregex_iterator it(grid_row.begin(), grid_row.end(), pattern);
	std::sregex_iterator end;
	while (it != end) {
		std::smatch match = *it;
		walls.push_back(match.position());
		++it;
	}
	for (int i = 0; i < walls.size() ; i = i + 2) {
		for (int j = walls[i]; j < walls[i + 1]; j++) {
			interior = (grid_row[j] == '.') ? interior + 1 : interior;
		}
	}
	return interior;
}

int countInsides(std::vector<std::string>& grid) {
	int total=0;
	for (std::string& row : grid) {
		total += computeInteriorOfRow(row);
	}
	return total;
}
void modifyGrid(std::vector<std::string>& grid, std::vector<std::pair<int, int>> loop) {
	for (int row = 0; row < grid.size(); row++) {
		for (int col = 0; col < grid[0].size(); col++) {
			std::pair<int, int> target = { row,col };
			auto it = std::find(loop.begin(), loop.end(), target);
			if (it == loop.end()) {
				grid[row][col] = '.';
			}
		}
	}
}

int main() {
	std::string input = "input.txt";
	auto grid = readFile(input);
	auto loop= findLoop(grid);
	std::cout << "Length of loop: " << loop.size()<<std::endl;
	grid[loop[0].first][loop[0].second] = determineStartType(loop);
	modifyGrid(grid, loop);
	std::cout << "Number of interior vertices: "<< countInsides(grid);
}
