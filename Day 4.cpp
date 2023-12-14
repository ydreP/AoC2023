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
/*
Get's a string of the form 
Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
returns a pair of vectors of the form
< 41 48 83 86 17 , 83 86  6 31 17  9 48 53 > 
*/
std::pair< std::vector<std::string>, std::vector<std::string> > parseCard(std::string raw_card) {
	std::pair< std::vector<std::string>, std::vector<std::string> > card;
	auto split_raw_card = splitString(raw_card, ':').back();
	//std::cout << split_raw_card << "\n";
	auto card_numbers = splitString(split_raw_card, '|');
	//std::cout << card_numbers[0] << " " << card_numbers[1] << "\n";
	auto winning_numbers_with_white_space = splitString(card_numbers[0],' ');
	auto numbers_you_have_with_white_space = splitString(card_numbers[1],' ');
	std::vector<std::string > winning_numbers;
	std::vector<std::string > numbers_you_have;

	for (std::string& winning_number : winning_numbers_with_white_space) {

		if (winning_number.length() != 0) {
			//std::cout << "Is numeric : " << winning_number << "\n";
			winning_numbers.push_back(winning_number);
		}
		//else {
		//	std::cout << "Is white space : " << winning_number << "\n";
		//}
	}
	for (std::string& number_you_have : numbers_you_have_with_white_space) {
		if (number_you_have.length() != 0) {
			numbers_you_have.push_back(number_you_have);
		}
	}
	card.first = winning_numbers;
	card.second = numbers_you_have;
	return card;
}

/*
Get's a list of strings of the form
Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
returns the number of points.
*/
int computeCardPoints(std::vector< std::string> cards) {
	int points=0;
	int card_number = 0;
	std::vector<int> number_of_cards(cards.size(), 1);
	for (std::string& card : cards) {
		auto winning_and_own_numbers = parseCard(card);
		auto winning_numbers = winning_and_own_numbers.first;
		auto own_numbers = winning_and_own_numbers.second; 
		//std::cout << "Card " << card_number << "\n";
		int number_of_matches = 0;
		for (std::string& own_number : own_numbers) {
			//std::cout << "Checking own number: " << own_number << "\n";
			for (std::string& winning_number : winning_numbers) {
				//std::cout << " checking winning number: " << winning_number;
				if (own_number == winning_number) {
					//std::cout << " it's a match! \n";
					number_of_matches++;
				}
				//else {
				//	std::cout << " no match! \n";
				//}
			}
		}
		if (number_of_matches > 0 && card_number <cards.size()-1) {
			for (int i = 1; i <= number_of_matches; i++) {
				number_of_cards[card_number + i]+= number_of_cards[card_number ];
			}
			//points += pow(2, number_of_matches - 1);
		}

		card_number++;
	}
	for (int& card_amount : number_of_cards) {
		points += card_amount;
	}
	return points;
}

int main() {
	std::string input = "input.txt";
	std::vector< std::string> cards = readFile(input);
	std::cout<<computeCardPoints(cards);
}
