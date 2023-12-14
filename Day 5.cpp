// Day 5.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
#include <chrono>

class Map {
private:
	std::vector<std::vector<long long>> maps;
	std::string name;
public: 
	Map(std::vector<std::vector<long long>> input ,std::string name) {
		maps = input;
		this->name = name;
	}
	long long mapSeed(long long seed) {
		for (std::vector< long long> map : maps) {
			long long destination_range_start = map[0];
			long long source_range_start = map[1];
			long long range_length = map[2];
			if ( seed >= source_range_start && seed < source_range_start + range_length) {
				return destination_range_start + seed - source_range_start;
			}
		}
		return seed;
	}
	std::vector<std::pair<long long, long long>> mapInterval(std::pair< long long, long long > interval) {

		//std::cout << "Mapping interval " << "(" << interval.first << "," << interval.second << ")" << std::endl;
		std::vector<std::pair<long long, long long>> mapped_intervals;
		long long lower = interval.first;
		long long upper = interval.second;
		for (std::vector< long long> map : maps) {
			long long destination_range_start = map[0];
			long long source_range_start = map[1];
			long long range_length = map[2];
			// if [lower,upper] \subseteq [source_range_start, source_range_start + range_length-1] 
			// then map to the interval [destination_range_start+lower-source_range_start, destination_range_start+upper-source_range_start]
			if (source_range_start <= lower && upper <= source_range_start + range_length-1) {
				//std::cout << "The interval is contained in one of the maps\n";
				std::pair<long long, long long> new_interval 
					= { destination_range_start + lower - source_range_start,destination_range_start + upper - source_range_start };
				mapped_intervals.push_back(new_interval);
				return mapped_intervals;
			}
			/* If [lower,upper] \supset [source_range_start, source_range_start + range_length-1] , then [lower,upper] maps to
				f([lower,source_range_start-1]) 
					\cup [destination_range_start, destination_range_start + range_length-1]
					\cup f([source_range_start+range_length,upper])*/
			if (lower < source_range_start && source_range_start + range_length <= upper) {
				//std::cout << "The interval contains one of the maps\n";
				std::pair<long long, long long> new_interval 
					= { destination_range_start, destination_range_start + range_length -1}; 
				std::pair<long long, long long> unmapped_lower_interval = { lower,source_range_start - 1 };
				std::pair<long long, long long> unmapped_upper_interval = { source_range_start + range_length,upper };
				auto mapped_lower_intervals = mapInterval(unmapped_lower_interval);
				auto mapped_upper_intervals = mapInterval(unmapped_upper_interval);
				mapped_intervals.push_back(new_interval);
				mapped_intervals.insert(mapped_intervals.end(), mapped_upper_intervals.begin(), mapped_upper_intervals.end());
				mapped_intervals.insert(mapped_intervals.end(), mapped_lower_intervals.begin(), mapped_lower_intervals.end());
				return mapped_intervals;
			}
			/* if[lower, upper] \cap[source_range_start, source_range_start + range_length-1] \neq \emptyset
			  but[lower, upper] \not \subset (or) \not \supset [source_range_start, source_range_start + range_length]
			 then either: */ 

			/* 1) lower < source_range_start < upper < source_range_start + range_length,
					f([lower,upper]) = 
					f([lower,source_range_start-1]) 
					\cup [destination_range_start, destination_range_start + upper - source_range_start]
					*/
			if (lower < source_range_start && upper >= source_range_start && upper < source_range_start + range_length) {
				//std::cout << "The interval intersects one of the maps  on the lower piece " << "("<<source_range_start <<","<< upper <<
					//")"<<std::endl;
				std::pair<long long, long long> unmapped_interval 
					= { lower, source_range_start - 1 };
				std::pair<long long, long long> new_interval 
					= { destination_range_start, destination_range_start + upper - source_range_start };
				auto re_mapped_intervals = mapInterval(unmapped_interval);
				//std::cout << "The intersected part maps to (" << new_interval.first << "," << new_interval.second << ")" << std::endl;
				mapped_intervals.push_back(new_interval);
				mapped_intervals.insert(mapped_intervals.end(), re_mapped_intervals.begin(), re_mapped_intervals.end());
				return mapped_intervals;
			}
			/* 2) source_range_start < lower < source_range_start + range_length < upper, and [lower, upper] maps to
					[destination_range_start+lower-source_range_start,destination_range_start+range_length-1] 
					\cup f([source_range_start+range_length,upper])
					*/
			if (source_range_start <= lower && lower <source_range_start+range_length && source_range_start + range_length <= upper) {
				//std::cout << "The interval intersects one of the map on the upper piece\n";
				std::pair<long long, long long> unmapped_interval 
					= { source_range_start+range_length, upper };
				std::pair<long long, long long> new_interval 
					= { destination_range_start + lower - source_range_start, destination_range_start + range_length - 1 };
				auto re_mapped_intervals = mapInterval(unmapped_interval);
				mapped_intervals.push_back(new_interval);
				mapped_intervals.insert(mapped_intervals.end(), re_mapped_intervals.begin(), re_mapped_intervals.end());
				return mapped_intervals;
			}
		}
		// if [lower, upper] \cap [source_range_start, source_range_start + range_length] = \emptyset then map to [lower, upper]
		mapped_intervals.push_back(interval);
		return mapped_intervals;
	}

	void printMap() {
		std::cout << name << std::endl;
		for (std::vector< long long>& map : maps) {
			for (long long& value : map) {
				std::cout << value << " ";
			}
			std::cout << std::endl;
		}
	}
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
long long convertStringToInt(std::string intString) {
	long long char_val = 0;
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
bool containsNumeric(const std::string& str) {
	for (char ch : str) {
		if (isdigit(ch)) {
			return true;  // Found a numeric character
		}
	}
	return false;  // No numeric character found
}
std::vector< long long > convertToIntVector(std::vector<std::string> input) {
	std::vector<long long> seeds;
	for (auto& seed_number : input) {
		if (isNumber(seed_number)) {
			seeds.push_back(convertStringToInt(seed_number));
		}
	}
	return seeds;
}
std::vector < Map > generateMaps(std::vector< std::string > input) {
	std::vector < Map > maps;
	int index = 1;
	char targetChar = ':';
	while (index < input.size()) {
		auto it = std::find(input[index].begin(), input[index].end(), targetChar);
		// If string contains the character : then it is the header of a map.
		if (it != input[index].end()) {
			//std::cout << "Found map! : " << input[index] << "\n";
			//If it is a header of a map we proceed with gathering all the maps until we encounter another header
			int map_index = index + 1;
			std::vector< std::vector<long long> > map_values;
			while (map_index < input.size() && containsNumeric(input[map_index])) {
				//std::cout << "Map: " << input[map_index] << "\n";
				auto map_values_string = splitString(input[map_index], ' ');
				auto map_value = convertToIntVector(map_values_string);
				map_values.push_back(map_value);
				map_index++;
			}
			Map map(map_values,input[index]);
			maps.push_back(map);
			index = map_index;
		}
		else {
			index++;
		}
	}
	return maps;
}
std::vector<long long> findLocations(std::vector<Map> maps, std::vector< long long> seeds) {
	std::vector<long long> locations =seeds;
	for (Map& map : maps) {
		std::vector<long long> tmp_locations;
		for(long long& location : locations) {
			tmp_locations.push_back(map.mapSeed(location));
		}
		locations = tmp_locations;
	}
	return locations; 
}


std::vector<std::pair<long long, long long> > findLocationsIntervals(std::vector<Map> maps, std::vector< std::pair<long long,long long>> seeds_intervals) {
	std::vector<std::pair<long long, long long> > intervals = seeds_intervals;
	for (Map& map : maps) {
		std::vector<std::pair<long long, long long> > tmp_intervals;
		for (auto& interval : intervals) {
			//std::cout << "Mapping the interval " << "(" << interval.first << "," << interval.second << ")" <<std::endl;
			auto new_intervals = map.mapInterval(interval);
			//std::cout << "maps to " << std::endl;
			for (auto& new_interval : new_intervals) {
				//std::cout << "(" << new_interval.first << "," << new_interval.second << ") " << std::endl;
				tmp_intervals.push_back(new_interval);
			}
			//std::cout << " by map " << std::endl;
			//map.printMap();
		}
		intervals = tmp_intervals;
	}
	return intervals;
}




int main(){

	auto start_time = std::chrono::high_resolution_clock::now();
	std::string input = "input.txt";
	auto data = readFile(input);
	// clean some data
	auto seed_data = splitString(splitString(data[0], ':')[1], ' ');
	// generate seeds
	auto seeds = convertToIntVector(seed_data);
	std::vector<std::pair<long long, long long> > seed_intervals;
	//std::cout << std::endl;
	for (int i = 0; i < seeds.size()-1; i=i+2) {
		std::pair<long long, long long> new_interval;
		new_interval.first = seeds[i];
		new_interval.second = seeds[i] + seeds[i+1] - 1;
		seed_intervals.push_back(new_interval);
	}

	//generate maps
	std::vector<Map> maps = generateMaps(data);
	long long minimal_loc = 9223372036854775807LL;
	std::vector<std::pair<long long, long long> > test_interval; 
	//test_interval.push_back(seed_intervals[0]);
	//findLocationsIntervals(maps, test_interval);
	//for (auto interval : seed_intervals) {
	//	std::cout << "Intervals to be mapped: (" << interval.first << "," << interval.second << ")" << std::endl;
	//}


	auto final_intervals = findLocationsIntervals(maps, seed_intervals);
	//std::cout << " Number of final intervals : " << final_intervals.size() << std::endl;
	for (auto& interval : final_intervals) {
		//std::cout << "(" << interval.first << "," << interval.second << ")\n";
		minimal_loc = std::min(minimal_loc, interval.first); 
		
	}
	std::cout <<"Minimal location: "<< minimal_loc << std::endl; //Wrong answer: 52752791, 206777808 Correct answer: 136096660 

	//std::cout << "Test: " << findLocations(maps, { 0 })[0] <<std::endl;

	// Stop measuring time
	auto end_time = std::chrono::high_resolution_clock::now();
	// Calculate the duration in microseconds
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
	// Output the duration
	std::cout << "Time taken: " << (double) duration.count()/1000000 << " seconds." << std::endl;
	return 0;
}
