#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>
std::map< char, int> card_order = {
	{'J',1},
	{'2',2},
	{'3',3},
	{'4',4},
	{'5',5},
	{'6',6},
	{'7',7},
	{'8',8},
	{'9',9},
	{'T',10},
	{'J',11},
	{'Q',12},
	{'K',13},
	{'A',14}
};
struct Hand {
	std::string hand;
	int bid=0;
};
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
			std::cout << "String: " << intString << "not correctly formated, encountered non - integer at position : " << i << " and value " << intString[i] << "\n";
			return 0;
		}
		else {
			char_val = char_val + (intString[i] - shift) * pow(10, string_size - 1 - i);
		}
	}
	return char_val;
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
std::vector<Hand> generateHands(std::vector<std::string> data) {
	std::vector<Hand> hands;
	for (auto& line : data) {
		std::vector<std::string> hand_and_bid = splitString(line, ' ');
		Hand hand;
		hand.hand = hand_and_bid[0];
		hand.bid = convertStringToInt(hand_and_bid[1]);
		hands.push_back(hand);
	}
	return hands;
}
int determineType(Hand hand) {
	std::map<char, int> count;
	char joker = 'J';
	for (auto& card : hand.hand) {
		count[card]++;
	}
	auto it = count.find(joker);

	if (it != count.end() ) {
		//if there is a joker and there is only one or two type of cards then this hand is 5 of a kind
		if (count.size() == 1 || count.size() == 2) {
			return 7;
		}
		//if there is a joker and there are three types of cards 
		if (count.size() == 3) {
			for (auto& key_val : count) {
				// we either have three of a kind and in this case the joker makes this four of a kind 
				if (key_val.second == 3) {
					return 6;
				}
			}
			// or two pairs, which either makes this a four of a kind or a full house
			// if we have two jokers then we can make this a four of a kind
			if (count[joker] == 2) {
				return 6;
			}
			//if we only have one joker then we can only make this a full house
			else if (count[joker] == 1) {
				return 5;
			}
		}
		//if there is a joker and there are four types of cards then this hand is a three of a kind
		if (count.size() == 4) {
			return 4;
		}
		//if there is a joker and there are 5 types of cards then this hand is a pair
		if (count.size() == 5) {
			return 2;
		}
	} else {
		if (count.size() == 1) {
			return 7;
		} else if (count.size() == 2) {
			for (auto& key_val : count) {
				if (key_val.second == 2 || key_val.second == 3) {
					return 5;
				}
				if (key_val.second == 4 || key_val.second == 1) {
					return 6;
				}
			}
		} else if (count.size() == 3) {	// if there are 3 types of cards then 
			for (auto& key_val : count) {
				// we either have three of a kind 
				if (key_val.second == 3) {
					return 4;
				}
			}
			// or two pairs
			return 3;
		} else if (count.size() == 4) {
			return 2;
		}
		if (count.size() == 5) {
			return 1;
		}
	}
	return 0;
}
// Returns true if hand_1 is better than hand_2, i.e. hand_1>hand_2
bool compareHands(Hand hand_1, Hand hand_2) {
	int type_1 = determineType(hand_1);
	int type_2 = determineType(hand_2);
	if (type_1 == 0 ) {
		std::cout << "Uhoh! Hand " << hand_1.hand <<  " has an invalid type! \n";
	}
	if (type_2 == 0) {
		std::cout << "Uhoh! Hand " << hand_2.hand << " has an invalid type! \n";
	}

	if (type_1 > type_2) {
		return true;
	}
	else if (type_1 < type_2) {
		return false;
	}
	else {
		std::string h_1 = hand_1.hand;
		std::string h_2 = hand_2.hand;
		for (int i = 0; i < h_1.size(); i++) {
			if (card_order[h_1[i]] > card_order[h_2[i]]) {
				return true;
			}
			else if (card_order[h_1[i]] < card_order[h_2[i]] ) {
				return false;
			}
		}
	}
}
std::vector<Hand> rankHands(std::vector<Hand> unsorted) {
	auto arr = unsorted;

	int i, j;
	bool swapped;
	for (i = 0; i < unsorted.size(); i++) {
		swapped = false;
		for (j = 0; j < arr.size() - i - 1; j++) {
			if ( compareHands(arr[j],arr[j+1] ) ) {
				iter_swap(arr.begin() + j, arr.begin() + j+1);
				swapped = true;
			}
		}
		// If no two elements were swapped
		// by inner loop, then break
		if (swapped == false)
			break;
	}
	return arr;
}
int totalWinnings(std::vector<Hand> hands) {
	int winnings = 0;
	for (int i = 0; i < hands.size(); i++) {
		winnings += (i + 1) * hands[i].bid;
	}
	return winnings;
}
int main(){
	std::string input = "input.txt";
	auto data = readFile(input);
	auto hands = generateHands(data);
	std::cout << hands.size() << "\n";
	for (auto& hand : hands) {
		determineType(hand);
	}
	auto sorted_hands = rankHands(hands);
	std::cout << totalWinnings(sorted_hands); 
}
