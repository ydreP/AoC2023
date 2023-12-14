// Day 9.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

	if (intString[0] == '-') {
		for (int i = 1; i < string_size; i++) {
			if (!isdigit(intString[i])) {
				std::cout << "string not correctly formated, encountered non-integer at position  : " << i << " and value " << intString[i] << "\n";
				return 0;
			}
			else {
				char_val = char_val + (intString[i] - shift) * pow(10, string_size - 1 - i);
			}
		}
		return -char_val;

	}
	else {

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
}
//Gets a vector of strings of the form "1 2 3" and returns a vector of vector<int>s in the obvious way
std::vector<std::vector<int>>  convertInput(std::vector<std::string> input) {
	std::vector<std::vector<int>> converted_input; 
	for (auto& line : input) {
		std::vector<int> vec; 
		auto split_string = splitString(line, ' ');
		for (auto& elem : split_string) {
			vec.push_back(convertStringToInt(elem));
		}
		converted_input.push_back(vec);
	}
	return converted_input;
}
std::vector<int> pairwiseDifference(std::vector<int> vec) {
	std::vector< int> diff_vec;
	for (int i = 0; i < vec.size() - 1; i++) {
		diff_vec.push_back(vec[i + 1] - vec[i]);
	}
	return diff_vec;
}
bool isZero(std::vector<int> vector) {
	for (int& elem : vector) {
		if (elem != 0) {
			return false;
		}
	}
	return true;
}

void printVector(std::vector<int> vec) {
	for (int& val : vec) {
		std::cout << val << " ";
	}
	std::cout << std::endl; 
}


int predict(std::vector< int> historical_data) {
	int predicted_value = 0;
	std::stack<std::vector<int>> stack_of_diff;
	stack_of_diff.push(historical_data);
	auto diff = pairwiseDifference(historical_data);
	stack_of_diff.push(diff);
	while ( !isZero( stack_of_diff.top() ) ) {
		stack_of_diff.push( pairwiseDifference( stack_of_diff.top() ) );
	}
	while (stack_of_diff.size()>1 ) {
		auto top_elem = stack_of_diff.top();
		stack_of_diff.pop();
		//stack_of_diff.top().push_back(stack_of_diff.top().back() + top_elem.back()); // part 1
		stack_of_diff.top().insert(stack_of_diff.top().begin(), stack_of_diff.top()[0] - top_elem[0]); // part 2

	}

	//return stack_of_diff.top().back(); // part 1
	return stack_of_diff.top()[0];  // part 2
}

int sumOfExtrapolations(std::vector<std::vector<int>> historical_datas ) {
	int sum = 0;
	for (auto& historical_data : historical_datas) {
		sum += predict(historical_data);
	}
	return sum;
}


int main() {
	std::string input = "input.txt";
	auto data = readFile(input);
	auto historical_datas = convertInput(data);
	int line_number = 0;
	std::cout<<predict(historical_datas[0])<<std::endl;
	std::cout << sumOfExtrapolations(historical_datas);
	return 0;
}

