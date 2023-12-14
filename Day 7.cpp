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
		//std::cout << hand_and_bid[0] << " " << hand_and_bid[1]<< "\n";

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
		//std::cout << hand.hand << " has a joker!";
		//if there is a joker and there is only one or two type of cards then this hand is 5 of a kind
		if (count.size() == 1 || count.size() == 2) {
			//std::cout << " and is 5 of a kind! \n";
			return 7;
		}
		
		//if there is a joker and there are three types of cards 
		if (count.size() == 3) {
			for (auto& key_val : count) {
				// we either have three of a kind and in this case the joker makes this four of a kind 
				if (key_val.second == 3) {
					//std::cout << " and is 4 of a kind! \n";
					return 6;
				}
			}
			// or two pairs, which either makes this a four of a kind or a full house
			// if we have two jokers then we can make this a four of a kind
			if (count[joker] == 2) {
				//std::cout << " and is 4 of a kind! \n";
				return 6;
			}
			//if we only have one joker then we can only make this a full house
			else if (count[joker] == 1) {
				//std::cout << " and is a full house! \n";
				return 5;
			}

		}
		//if there is a joker and there are four types of cards then this hand is a three of a kind
		if (count.size() == 4) {
			//std::cout << " and is 3 of a kind! \n";
			return 4;
		}
		//if there is a joker and there are 5 types of cards then this hand is a pair
		if (count.size() == 5) {
			//std::cout << " and is a pair! \n";
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
	std::cout << totalWinnings(sorted_hands); // Wrong answer is : 251436044 246944544

	//std::cout << "Hands in sorted order are: \n";
	//for (auto& hand : sorted_hands) {
	//	std::cout << hand.hand << "\n";
	//}

}





/*

Every hand is exactly one type. From strongest to weakest, they are:

Five of a kind, where all five cards have the same label: AAAAA
Four of a kind, where four cards have the same label and one card has a different label: AA8AA
Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
Three of a kind, where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
Two pair, where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
One pair, where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
High card, where all cards' labels are distinct: 23456
Hands are primarily ordered based on type; for example, every full house is stronger than any three of a kind.

To make things a little more interesting, the Elf introduces one additional rule. 
Now, J cards are jokers - wildcards that can act like whatever card would make the hand the strongest type possible.
To balance this, J cards are now the weakest individual cards, weaker even than 2. 
The other cards stay in the same order: A, K, Q, T, 9, 8, 7, 6, 5, 4, 3, 2, J.
J cards can pretend to be whatever card is best for the purpose of determining hand type; for example, 
QJJQ2 is now considered four of a kind. However, for the purpose of breaking ties between two hands of the same type, 
J is always treated as J, not the card it's pretending to be: JKKK2 is weaker than QQQQ2 because J is weaker than Q.

Now, the above example goes very differently:

32T3K 765
T55J5 684
KK677 28
KTJJT 220
QQQJA 483
32T3K is still the only one pair; it doesn't contain any jokers, so its strength doesn't increase.
KK677 is now the only two pair, making it the second-weakest hand.
T55J5, KTJJT, and QQQJA are now all four of a kind! T55J5 gets rank 3, QQQJA gets rank 4, and KTJJT gets rank 5.
With the new joker rule, the total winnings in this example are 5905.

Using the new joker rule, find the rank of every hand in your set. What are the new total winnings?

*/