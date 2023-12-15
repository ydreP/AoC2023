// Day 13.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <tuple>
#include <numeric>

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
struct pattern {
	vector<string> grid; 
};
void printPattern(pattern& p) {
	for (const string& row : p.grid) {
		cout << row << endl;
	}
}
vector<pattern> generatePatterns(vector<string> input) {
	vector<string> grid; 
	vector<pattern> patterns;
	for (string& line : input) {
		grid.push_back(line);
		if (line.empty()) {
			grid.erase(grid.end() - 1);
			pattern p;
			p.grid = grid;
			vector<string> empty;
			grid = empty;
			patterns.push_back(p);
		}
	}
	pattern p;
	p.grid = grid;
	patterns.push_back(p);

	return patterns;
}
bool isReflected(string first, string second) {
	reverse(first.begin(), first.end());
	for (int i = 0; i < min(first.size(), second.size()); i++) {
		if (first[i] != second[i]) {
			return false;
		}
	}
	return true;
}
vector<string> transposed(vector<string> grid) {
	vector<string> transposed_grid;
	for (size_t col = 0; col < grid[0].size(); col++) {
		string transposed;
		for (size_t row = 0; row < grid.size(); row++) {
			transposed += grid[row][col];
		}
		transposed_grid.push_back(transposed);
	}
	return transposed_grid;
}
vector<pair<string, int>> findReflection(pattern p) {
	pair<string, int> reflection;
	vector<pair<string, int>> reflections;
	auto grid = p.grid;
	int number_of_columns = grid[0].size();
	//reflection along a column
	for (int split_index = 1; split_index < number_of_columns ; split_index++) {
		bool split = true;
		for (string& line : grid) {
			string left_split = line.substr(0, split_index);
			string right_split = line.substr(split_index);
			split = split && isReflected(left_split, right_split);
		}
		if (split) {
			reflection = { "col",split_index };
			reflections.push_back(reflection);
		}
	}
	// reflection along a row 
	vector<string> transposed_grid = transposed(grid);
	int number_of_rows = transposed_grid[0].size();
	for (int split_index = 1; split_index < number_of_rows ; split_index++) {
		bool split = true;
		for (string& line : transposed_grid) {
			string left_split = line.substr(0, split_index);
			string right_split = line.substr(split_index);
			split = split && isReflected(left_split, right_split);
		}
		if (split) {
			reflection = { "row",split_index };
			reflections.push_back(reflection);
		}
	}
	return reflections;
}
vector<pair<string,int>> cleanPattern(pattern p) {
	auto original_reflection = findReflection(p);
	for (size_t row = 0; row < p.grid.size(); row++) {
		for (size_t col = 0; col < p.grid[0].size(); col++) {
			pattern temp_pattern;
			temp_pattern.grid = p.grid;
			temp_pattern.grid[row][col] = (temp_pattern.grid[row][col] == '.') ? '#' : '.';
			auto new_reflection = findReflection(temp_pattern);
			auto it =find(new_reflection.begin(), new_reflection.end(), original_reflection[0]);
			if (new_reflection.size() > 1 ) {
				return new_reflection;
			}
			else if (it == new_reflection.end() && !new_reflection.empty()) {
				return new_reflection;
			}
		}
	}
	return original_reflection;
}
// part 1, here all patterns only have one line of reflection
int addAll(vector<pattern>& patterns) {
	int sum = 0;
	for (pattern& p : patterns) {
		auto reflection = findReflection(p)[0];
		if (reflection.first == "col") {
			sum += reflection.second;
		}
		else if( reflection.first == "row") {
			sum += reflection.second * 100;
		}
	}
	return sum;
}
int addAllPart2(vector<pattern>& patterns) {
	int sum = 0;
	for (pattern& p : patterns) {
		auto reflection = findReflection(p)[0];
		auto cleaned_reflections = cleanPattern(p);
		auto it =find(cleaned_reflections.begin(), cleaned_reflections.end(), reflection);
		if (it != cleaned_reflections.end()) {
			cleaned_reflections.erase(it);
		}
		reflection = cleaned_reflections[0];
		if (reflection.first == "col") {
			sum += reflection.second;
		}
		else if (reflection.first == "row") {
			sum += reflection.second * 100;
		}
	}
	return sum;

}
int main() {
	string input = "input.txt";
	auto data = readFile(input);
	vector<pattern> patterns = generatePatterns(data);
	cout << addAll(patterns) << endl;
	cout << addAllPart2(patterns)<<endl;
}