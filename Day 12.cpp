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
bool isNumber(std::string intString) {
	if (intString.empty()) {
		return false;
	}
	else {
		for (char& digit : intString) {
			if (!isdigit(digit)) {
				return false;
			}
		}
		return true;
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

bool stringContainsChar(string str, char c) {
	for (char& check : str) {
		if (check == c ) {
			return true;
		}
	}
	return false;
}


void printVector(vector<int>& v) {
	cout << "[";
	for (int& elem : v) {
		cout << elem << " ";
	}
	cout << "]" << endl;
}
//checks if it is possible to place an isolated 
// (i.e starting and ending with '.' at posistion pos-1) block of 
// length len of #'s starting at position position 

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
	//for (int i = 0; i < springs.size(); i++) {
	//	cout << springs[i] << endl;
	//	printVector(list_of_blocks[i]);
	//}
	map<string, long long> known_configurations;
	for (int i = 0; i < springs.size(); i++) {
		long long comb = possibleConfigurations(springs[i], list_of_blocks[i], known_configurations, 0,0,0);
		//cout << known_configurations.size() << endl;
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

	//Wrong answer: 162393957  162393957
}


/*
--- Day 12: Hot Springs ---
You finally reach the hot springs! You can see steam rising from secluded areas attached to the primary, ornate building.

As you turn to enter, the researcher stops you. "Wait - I thought you were looking for the hot springs, weren't you?" You indicate that this definitely looks like hot springs to you.

"Oh, sorry, common mistake! This is actually the onsen! The hot springs are next door."

You look in the direction the researcher is pointing and suddenly notice the massive metal helixes towering overhead. "This way!"

It only takes you a few more steps to reach the main gate of the massive fenced-off area containing the springs. You go through the gate and into a small administrative building.

"Hello! What brings you to the hot springs today? Sorry they're not very hot right now; we're having a lava shortage at the moment." You ask about the missing machine parts for Desert Island.

"Oh, all of Gear Island is currently offline! Nothing is being manufactured at the moment, not until we get more lava to heat our forges. And our springs. The springs aren't very springy unless they're hot!"

"Say, could you go up and see why the lava stopped flowing? The springs are too cold for normal operation, but we should be able to find one springy enough to launch you up there!"

There's just one problem - many of the springs have fallen into disrepair, so they're not actually sure which springs would even be safe to use! Worse yet, their condition records of which springs are damaged (your puzzle input) are also damaged! You'll need to help them repair the damaged records.

In the giant field just outside, the springs are arranged into rows. For each row, the condition records show every spring and whether it is operational (.) or damaged (#). This is the part of the condition records that is itself damaged; for some springs, it is simply unknown (?) whether the spring is operational or damaged.

However, the engineer that produced the condition records also duplicated some of this information in a different format! After the list of springs for a given row, the size of each contiguous group of damaged springs is listed in the order those groups appear in the row. This list always accounts for every damaged spring, and each number is the entire size of its contiguous group (that is, groups are always separated by at least one operational spring: #### would always be 4, never 2,2).

So, condition records with no unknown spring conditions might look like this:

#.#.### 1,1,3
.#...#....###. 1,1,3
.#.###.#.###### 1,3,1,6
####.#...#... 4,1,1
#....######..#####. 1,6,5
.###.##....# 3,2,1
However, the condition records are partially damaged; some of the springs' conditions are actually unknown (?). For example:

???.### 1,1,3
.??..??...?##. 1,1,3
?#?#?#?#?#?#?#? 1,3,1,6
????.#...#... 4,1,1
????.######..#####. 1,6,5
?###???????? 3,2,1
Equipped with this information, it is your job to figure out how many different arrangements of operational and broken springs fit the given criteria in each row.

In the first line (???.### 1,1,3), there is exactly one way separate groups of one, one, and three broken springs (in that order) can appear in that row: the first three unknown springs must be broken, then operational, then broken (#.#), making the whole row #.#.###.

The second line is more interesting: .??..??...?##. 1,1,3 could be a total of four different arrangements. The last ? must always be broken (to satisfy the final contiguous group of three broken springs), and each ?? must hide exactly one of the two broken springs. (Neither ?? could be both broken springs or they would form a single contiguous group of two; if that were true, the numbers afterward would have been 2,3 instead.) Since each ?? can either be #. or .#, there are four possible arrangements of springs.

The last line is actually consistent with ten different arrangements! Because the first number is 3, the first and second ? must both be . (if either were #, the first number would have to be 4 or higher). However, the remaining run of unknown spring conditions have many different ways they could hold groups of two and one broken springs:

?###???????? 3,2,1
.###.##.#...
.###.##..#..
.###.##...#.
.###.##....#
.###..##.#..
.###..##..#.
.###..##...#
.###...##.#.
.###...##..#
.###....##.#
In this example, the number of possible arrangements for each row is:

???.### 1,1,3 - 1 arrangement
.??..??...?##. 1,1,3 - 4 arrangements
?#?#?#?#?#?#?#? 1,3,1,6 - 1 arrangement
????.#...#... 4,1,1 - 1 arrangement
????.######..#####. 1,6,5 - 4 arrangements
?###???????? 3,2,1 - 10 arrangements
Adding all of the possible arrangement counts together produces a total of 21 arrangements.

For each row, count all of the different arrangements of operational and broken springs that meet the given criteria. What is the sum of those counts?



*/