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


int getGameID(std::string raw_game_data) {
	std::regex pattern("\\d+");
	std::string game_id;
	std::sregex_iterator it(raw_game_data.begin(), raw_game_data.end(), pattern);
	std::sregex_iterator end;
	while (it != end) {
		std::smatch match = *it;
		game_id = match[0];
		++it;
	}
	return convertStringToInt(game_id);
}

std::pair<std::string, int> getDraw(std::string raw_draw_data) {
	std::pair<std::string, int> draw;
	std::string pattern_string = "blue|red|green|\\d+";
	std::regex pattern(pattern_string);
	std::sregex_iterator it(raw_draw_data.begin(), raw_draw_data.end(), pattern);
	std::sregex_iterator end;
	while (it != end) {
		std::smatch match = *it;
		if (isNumber(match[0])) {
			draw.second = convertStringToInt(match[0]);
		}
		else {
			draw.first = match[0];
		}
		++it;
	}


	return draw;
}


int computeGames(std::vector<std::string> games, bool part_2) {
	std::map<std::string, int> allowed_values = { {"red",12}, {"green",13} ,{"blue",14} };
	int sum = 0;
	for (std::string& game : games) {
		std::map<std::string, int> minimal_values = { {"red",0}, {"green",0} ,{"blue",0} };
		int product = 1;
		auto split_game = splitString(game, ':');
		int game_id = getGameID(split_game[0]);
		//std::cout << "Game " << game_id << "\n";
		auto raw_rounds = splitString(split_game[1], ';');
		int turn = 1;
		bool possible_game = true;
		for (std::string& raw_round : raw_rounds) {
			auto round = splitString(raw_round, ',');
			//std::cout << "Round " << turn << " is playing. \n";
			//std::cout << "Elf draws ";
			for (auto& draw_data : round) {
				auto draw = getDraw(draw_data);
				minimal_values[draw.first] = std::max(minimal_values[draw.first], draw.second);
				//std::cout << draw.first << " " << draw.second<< " ";
				if (draw.second > allowed_values[draw.first]) {
					possible_game = false;
				}

			}
			//std::cout<< "from the bag \n";
			turn++;
		}
		for (const auto& key_val : minimal_values) {
			product = product * key_val.second;
		}
		std::cout << "Power of the game is: " << product << "\n";
		if (part_2) {
			sum += product;
		}
		else {
			if (possible_game) {
				//std::cout << "Game " << game_id << " is possible \n";
				sum += game_id;
			}
		}

	}
	return sum;
}



int main() {
	std::string input = "input.txt";
	std::vector<std::string> games = readFile(input);
	std::cout << "Sum of allowed games are : " << computeGames(games,true);


}
