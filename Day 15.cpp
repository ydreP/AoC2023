// Day 15.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 
#include <vector>
#include <cmath>
#include <algorithm>
#include <tuple>
#include <numeric>
#include <regex>

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
int hashAlgorithm(string& str) {
	int current_value = 0;
	for (char& c : str) {
		current_value += c;
		current_value *= 17;
		current_value %= 256;
	}
	return current_value;
}
int sumInitSequenece(vector<string>& init_seq) {
	int sum = 0;
	for (string& str : init_seq) {
		sum += hashAlgorithm(str);
	}
	return sum;
}


int findPosition(vector<pair<string, int>>& box, string& lens) {
	if (box.empty()) {
		return -1;
	}
	for (int i = 0; i < box.size(); i++) {
		if (box[i].first == lens) {
			return i;
		}
	}
	return -1;
}

void modify(vector< vector<pair<string, int>> >& hash_map, vector<string>& instructions) {
	string lens = instructions[0];
	vector<pair<string, int>>& box = hash_map[hashAlgorithm(lens)];
	string modifier = instructions[1];
	int value = 0;
	int index = 0;
	//If modifier = "=" then we add/update (lens,value) this to the box
	if (modifier == "=") {
		value = stoi(instructions[2]);
		if (box.empty()) {
			box.push_back({ lens,value });
		}
		else {
			//returns -1 if lens is not in the box
			index = findPosition(box, lens);
			// if the lens is already in the box we update the value;
			if (index != -1) {
				box[index].second = value;
			}
			else {
				box.push_back({ lens,value });
			}
		}
	} // If modifier = "-" then we remove it (if it is present) 
	else if (modifier == "-") {
		index = findPosition(box, lens);
		if (index != -1) {
			box.erase(box.begin() + index);
		}
	}
}

vector<vector<pair<string, int>> > HASHMAP(vector<string>& init_seq) {
	// Outer vector corresponds to the boxex 
	// Inner vector corresponds to the elements of each box
	vector< vector<pair<string, int>> > hash_map(256);
	string token_string = "[a-z]+|-|=|\\d+";
	regex pattern(token_string);
	int hash_val = 0;
	for (string& str : init_seq) {
		sregex_iterator pattern_it(str.begin(), str.end(), pattern);
		sregex_iterator end;
		vector<string> val;
		while (pattern_it != end) {
			smatch match = *pattern_it;
			val.push_back(match.str());
			pattern_it++;
		}
		modify(hash_map, val);
	}
	return hash_map;
}


int computeFocusingPower(vector<vector<pair<string, int>> >& hash_map) {
	int box_number = 1;
	int slot_number = 1;
	int sum = 0;
	for (vector<pair<string, int>>& box : hash_map) {
		slot_number = 1;
		for (pair<string, int>& lens : box) {
			sum += box_number * slot_number * lens.second;
			slot_number++;
		}
		box_number++;
	}
	return sum;
}

int main() {
	string input = "input.txt";
	vector<string> data = readFile(input);
	vector<string> init_seq = splitString(data[0], ',');
	bool part1 = false;
	if (part1) {
		cout << sumInitSequenece(init_seq);
	}
	else {
		auto hash_map = HASHMAP(init_seq);
		cout << computeFocusingPower(hash_map);
	}
}
