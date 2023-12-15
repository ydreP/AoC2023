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
#include <thread>
#include <chrono>

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
//basic parsing method, converts each line into a string and a vector of ints
pair<vector<string>, vector<vector<int> > > parseInput(vector<string> input) {
	vector<vector<int>> number_of_damaged_springs;
	vector<string> springs;
	for (auto& line : input) {
		vector<int> nbrs;
		auto split_line = splitString(line, ' ');
		springs.push_back(split_line[0]);
		auto array_of_chars = splitString(split_line[1], ',');
		for (auto& elem: array_of_chars) {
			nbrs.push_back(convertStringToInt(elem));
		}
		number_of_damaged_springs.push_back(nbrs);
	}
	return { springs, number_of_damaged_springs };
}
/*
pos - position in the configuration 
block_pos - position among the number of blocks
block_len - current length of block of #'s
*/
long long possibleConfigurations(string configuration, vector<int> block_sizes,
	map<string, long long>& known_configurations,int pos, int block_pos, int block_len) {
	vector<char> possible_insertions = { '#','.' };
	//construct key for memoization
	string key = "("+ to_string(pos) + "," + to_string(block_pos) + "," + to_string(block_len)+")";
	//cout << key << endl;
	long long combinations = 0;
	auto it = known_configurations.find(key);
	
	if (it != known_configurations.end()) {
		return known_configurations[key];
	}
	//If we are past the last element of the configuration 
	if (pos >= configuration.size()) {
		// If we have managed to fill in everything correctly 
		// before the last position 
		if (block_pos >= block_sizes.size() && block_len == 0) {
			return 1;
		} // if we are at the last block and this is also correct
		else if (  (block_pos ==  block_sizes.size()-1) && block_sizes[block_pos] == block_len) {
			return 1;
		}
		else {
			return 0;
		}
	}

	//If the current position is a '?'
	if (configuration[pos] == '?') {
		// Then we can "insert" (we don't change the string configuration) '.' or  '#' at this position
		for (char& c : possible_insertions) {
			// if we choose to insert a '.'
			if (c == '.') {
				// then either: 
				// 1) we aren't currently on a block of #'s
				if (block_len == 0) {
					//... and then we move to the next position
					combinations += possibleConfigurations(configuration, block_sizes, known_configurations, pos + 1, block_pos, 0);
				}
				//2) or we see a block that we expect 
				else if ( block_len > 0 && block_pos < block_sizes.size() && block_sizes[block_pos]== block_len ) {
					// then we move look for the next position and the next block position
					combinations += possibleConfigurations(configuration, block_sizes, known_configurations, pos + 1, block_pos+1, 0);
				}
			}// if we choose to insert a '#' instead
			else {
				combinations += possibleConfigurations(configuration, block_sizes, known_configurations, pos + 1, block_pos , block_len+1);
			}
		}
	} // if the current position is a '.'
	else if (configuration[pos] == '.') {
		if (block_len == 0) {
			//... and then we move to the next position
			combinations += possibleConfigurations(configuration, block_sizes, known_configurations, pos + 1, block_pos, 0);
		}
		//2) and we see a block that we expect 
		else if (block_len > 0 && block_pos < block_sizes.size() && block_sizes[block_pos] == block_len) {
			// then we move look for the next position and the next block position
			combinations += possibleConfigurations(configuration, block_sizes, known_configurations, pos + 1, block_pos + 1, 0);
		}
	}//If the current position is a '#'
	else {
		combinations += possibleConfigurations(configuration, block_sizes, known_configurations, pos + 1, block_pos, block_len+1);
	}

	known_configurations[key] = combinations;

	return combinations;

}
long long totalNumberOfCombinations(vector<string> springs, vector<vector<int>> list_of_blocks, bool part_2) {
	long long sum = 0;
	vector<string> new_springs;
	vector<vector<int>> new_list_of_blocks;
	if (part_2) {
		for (int i = 0; i < springs.size(); i++) {
			string new_spring;
			vector<int> new_block;
			for (int j = 0; j < 5; j++) {
				new_spring += springs[i];
				new_spring += "?";
				for (int& val : list_of_blocks[i]) {
					new_block.push_back(val);
				}
			}
			new_spring.erase(new_spring.end() - 1);
			new_springs.push_back(new_spring);
			new_list_of_blocks.push_back(new_block);
		}
		springs = new_springs;
		list_of_blocks = new_list_of_blocks;
	}
	map<string, long long> known_configurations;
	for (int i = 0; i < springs.size(); i++) {
		long long comb = possibleConfigurations(springs[i], list_of_blocks[i], known_configurations, 0,0,0);
		known_configurations.clear();
		sum += comb;
	}
	return sum;
}

int main() {
	string input = "input.txt";
	vector<string> data = readFile(input);
	auto parsed_data = parseInput(data);
	auto springs = parsed_data.first;
	auto block_sizes = parsed_data.second;
	cout<< totalNumberOfCombinations(springs, block_sizes,true);
}
