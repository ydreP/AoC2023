// Day 6.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

std::vector<long long> numberOfVictories(std::vector< long long> race_durations, std::vector< long long> records) {
    std::vector< long long > number_of_victories;
    for (int i = 0; i < race_durations.size(); i++) {
        long long ways_to_win = floor(sqrt(pow(race_durations[i], 2) / 4 - records[i])) + ceil(sqrt(pow(race_durations[i], 2) / 4 - records[i]));
        number_of_victories.push_back(ways_to_win);
    }
    return number_of_victories;
}

int main() {
    std::vector<long long > test_duration = {71530}; 
    std:: vector<long long> test_records = {940200};
    std::vector<long long > input_duration = { 46807866 };
    std::vector<long long> input_records = { 214117714021024 };


    auto number_of_victories = numberOfVictories(input_duration, input_records);
    int prod = 1;
    for (auto& win : number_of_victories) {
        std::cout << win << " ";
        prod *= win;
    }
    std::cout << "\n";
    std::cout << "Number of ways to win : " << prod; 
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
