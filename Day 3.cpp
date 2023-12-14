// Day 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
std::vector<std::string> splitString(std::string str, char delimiter) {
	std::vector<std::string> tokens;
	std::istringstream iss(str);
	std::string token;
	while (std::getline(iss, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}
int convertStringToInt(std::string intString) {
	int char_val = 0;
	char shift = '0';
	int string_size = intString.size();
	for (int i = 0; i < string_size; i++) {
		if (!isdigit(intString[i])) {
			std::cout << "string not correctly formated, encountered non-integer at position  : " << i << " and value " << intString[i] << "\n";
			return 0;
		}
		else {
			char_val = char_val + (intString[i] - shift) * pow(10, string_size - 1 - i);
		}
	}
	return char_val;
}

bool isNumber(std::string intString) {
	for (char& digit : intString) {
		if (!isdigit(digit)) {
			return false;
		}
	}
	return true;
}

int findNumber(std::vector<std::string>& schematic, std::vector<std::vector<bool> >& already_counted, int row, int col) {
	int value;
	int rows = schematic.size();
	int columns = schematic[0].size();
	int left_most = col; 
	int right_most = col;
	while (left_most >= 0 &&  isdigit(schematic[row][left_most])) {
		already_counted[row][left_most] = true;
		left_most--;
	}
	left_most++;
	while ( right_most < columns && isdigit(schematic[row][right_most])) {
		already_counted[row][right_most] = true;
		right_most++;
	}
	right_most--;
	std::string string_value = schematic[row].substr(left_most, right_most - left_most+1);
	value = convertStringToInt(string_value);
	return value;
}

int computePartNumber(std::vector<std::string> schematic) {
	int part_number = 0;
	int rows = schematic.size();
	int columns = schematic[0].size();
	std::vector<std::vector<bool> > already_counted( rows, std::vector<bool>(columns, false) );

	std::vector<std::vector<int> > shifts = { { -1,-1}, {-1,0},{-1,1},{1,1},{1,0},{1,-1},{0,-1},{0,1}};
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < columns; col++) {
			//Check if position (row,col) is a character.  
			if(schematic[row][col] == '*'){
			//if (!(schematic[row][col] == '.' || isdigit(schematic[row][col])) ) {
				int part_prod = 1;
				int adjacency_number = 0;
				for (std::vector<int>& shift : shifts) {
					int shifted_row = row+shift[0];
					int shifted_col = col + shift[1];
					if (shifted_row >= 0 && shifted_col >= 0 && shifted_row < rows && shifted_col < columns) {
						if (isdigit(schematic[shifted_row][shifted_col]) && !already_counted[shifted_row][shifted_col] )  {
							adjacency_number++;
							part_prod =part_prod* findNumber(schematic, already_counted, shifted_row, shifted_col);
							//part_number += findNumber(schematic, already_counted, shifted_row, shifted_col);
							//std::cout << "Found adjacent number " << findNumber(schematic, already_counted,shifted_row, shifted_col) << "\n";
							//std::cout << already_counted[shifted_row][shifted_col] << "\n";
						}
					}
				}
				if (adjacency_number == 2) {
					std::cout << "Row  and column " << "(" << row << "," << col << ") is a gear with ratio " << part_prod << "\n";
					part_number += part_prod;
				}

			}
		}
	}
	return part_number;
}



int main(){
	std::string input = "input.txt";
	std::vector<std::string> schematic = readFile(input);
	std::cout<< computePartNumber(schematic);



}
