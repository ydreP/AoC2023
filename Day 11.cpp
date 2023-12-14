// Day 11.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

using namespace std;

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


pair<vector<int>, vector<int>> findEmptyRowsAndCols(vector<string> grid) {
	bool is_empty;
	vector<int> rows;
	vector<int> cols;
	char empty_char = '.';
	for (int row = 0; row < grid.size(); row++) {
		is_empty = true; 
		for (int col = 0; col < grid[0].size(); col++) {
			is_empty = is_empty && (grid[row][col] == empty_char);
		}
		if (is_empty) {
			rows.push_back(row);
		}
	}
	for (int col = 0; col < grid[0].size(); col++) {
		is_empty = true;
		for (int row = 0; row < grid.size(); row++) {
			is_empty = is_empty && (grid[row][col] == empty_char);
		}
		if (is_empty) {
			cols.push_back(col);
		}
	}
	return { rows, cols };
}
vector<pair<int, int>> findGalaxies(vector<string> grid) {
	vector<pair<int, int>> galaxy_coordinates; 
	for (int row = 0; row < grid.size(); row++) {
		for (int col = 0; col < grid[0].size(); col++) {
			if (grid[row][col] == '#') {
				galaxy_coordinates.push_back({ row,col });
			}
		}
	}
	return galaxy_coordinates;
}


vector<string> expandUniverse(vector<string> grid, pair<vector<int>, vector<int> > empty_rows_and_columns) {
	auto rows = empty_rows_and_columns.first;
	auto cols = empty_rows_and_columns.second;
	vector<string> expanded_universe_by_columns;
	for (auto& line : grid) {
		string expanded_row = "";
		for (int col = 0; col < line.size(); col++) {
			auto it = std::find(cols.begin(), cols.end(), col);
			if (it != cols.end()) {
				expanded_row += ".";
			}
			expanded_row += line[col];
		}
		expanded_universe_by_columns.push_back(expanded_row);
	}
	vector<string> expanded_universe;
	string empty_row(expanded_universe_by_columns[0].size(),'.');	
	for (int row = 0; row < expanded_universe_by_columns.size(); row++) {
		auto it = std::find(rows.begin(), rows.end(), row);
		if (it != rows.end()) {
			expanded_universe.push_back(empty_row);
		}
		expanded_universe.push_back(expanded_universe_by_columns[row]);
	}
	return expanded_universe;
}

int computeTotalDistances( vector<pair<int, int>> galaxies) {
	int total_distance = 0;
	for (int i = 0; i < galaxies.size(); i++) {
		for (int j = i + 1; j < galaxies.size(); j++) {
			total_distance += abs(galaxies[i].first - galaxies[j].first) + abs(galaxies[i].second - galaxies[j].second);
		}
	}
	return total_distance;
}
void printPair(std::pair<int, int> point) {
	std::cout << "(" << point.first << "," << point.second << ") \n";
}

long long computeTotalDistances(vector<pair<int, int>> galaxies, pair<vector<int>,vector<int>> empty_rows_and_cols, int expansion_factor) {
	long long total_distance = 0;
	vector<int> empty_rows = empty_rows_and_cols.first;
	vector<int> empty_cols = empty_rows_and_cols.second;
	cout << " Empty rows: \n";
	for (int row : empty_rows) {
		cout << row << " ";
	}
	cout << endl;
	cout << " Empty cols: \n";
	for (int col : empty_cols) {
		cout << col << " ";
	}
	cout<<endl;

	for (int i = 0; i < galaxies.size(); i++) {
		for (int j = i + 1; j < galaxies.size(); j++) {
			int number_of_empty_rows_and_cols=0; 
			for (int row : empty_rows) {
				if(row> min(galaxies[i].first, galaxies[j].first) && row < max(galaxies[i].first, galaxies[j].first)){
					number_of_empty_rows_and_cols++;
				}
			}
			for (int col : empty_cols) {
				if (col > min(galaxies[i].second, galaxies[j].second) && col < max(galaxies[i].second, galaxies[j].second)) {
					number_of_empty_rows_and_cols++;
				}
			}
			total_distance += abs(galaxies[i].first - galaxies[j].first) + abs(galaxies[i].second - galaxies[j].second)+(expansion_factor-1)*number_of_empty_rows_and_cols;
		}
	}
	return total_distance;
}



int main() {
	string input = "input.txt";
	vector<string> universe = readFile(input);

	auto empty_rows_and_cols = findEmptyRowsAndCols(universe);
	auto galaxies = findGalaxies(universe);
	cout << computeTotalDistances(galaxies, empty_rows_and_cols, 1000000)<<endl;



}
