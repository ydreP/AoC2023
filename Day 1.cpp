#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <regex>
std::vector<std::string> read_file(std::string str) {
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
std::vector<std::string> split_string(std::string str, char delimiter) {
	std::vector<std::string> tokens;
	std::istringstream iss(str);
	std::string token;
	while (std::getline(iss, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}
int convertCharToInt(std::string intString) {
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
int sumOfCalibrationValues(std::vector<std::string> calibration_document) {
	std::vector < std::string> integers_from_doc;
	std::map<std::string, std::string> string_to_int_map = { {"one" , "1"},{"two","2"},{"three","3"},{"four","4"},
		{"five","5"},{"six","6"},{"seven","7"},{"eight","8"},{"nine","9"} };
	//form regex expression pattern
	std::string pattern_string = "(?=(one|two|three|four|five|six|seven|eight|nine|\\d))";
	std::regex pattern(pattern_string);
	// extract integer subsequence
	for (std::string& calibration_string : calibration_document) {
		std::string integers_from_line;
		std::sregex_iterator it(calibration_string.begin(), calibration_string.end(), pattern);
		std::sregex_iterator end;
		while (it != end) {
			std::smatch match = *it;
			if (string_to_int_map.count(match[1]) > 0) {
				integers_from_line += string_to_int_map[match[1]];
			}
			else {
				integers_from_line += match[1];
			}
			++it;
		}
		integers_from_doc.push_back(integers_from_line);
	}
	int sum = 0;
	for (std::string& line : integers_from_doc) {
		std::string vals;
		if (line.size() > 1) {
			char first = line[0];
			char last = line.back();
			std::string tmp_vals(&first, 1);
			tmp_vals += last;
			vals = tmp_vals;
		}
		else {
			char first = line[0];
			char last = line.back();
			std::string tmp_vals(&first, 1);
			tmp_vals += last;
			vals = tmp_vals;
		}
		sum = sum + convertCharToInt(vals);
	}

	return sum;
}

int main()
{
	std::string file_name = "input.txt"; // test.txt
	auto data = read_file(file_name);
	std::cout << sumOfCalibrationValues(data);
}
