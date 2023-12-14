// Day 8.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
#include <numeric>
#include <iostream>

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
/*Given a string of the form 
AAA = (BBB,CCC)
return the vector {AAA,BBB,CCC}
*/
std::vector<std::string> parseLine(std::string line) {
	//std::cout << "Parsing line " << line << "\n";
	std::vector<std::string> parsed;
	auto split = splitString(line, '=');
	auto node = split[0];
	auto edges = splitString(split[1], ',');
	const std::string charsToRemove = ", )(";
	node.erase(std::remove_if(node.begin(), node.end(),
		[&charsToRemove](char c) { return charsToRemove.find(c) != std::string::npos; }),
		node.end());
	edges[0].erase(std::remove_if(edges[0].begin(), edges[0].end(),
		[&charsToRemove](char c) { return charsToRemove.find(c) != std::string::npos; }),
		edges[0].end());
	edges[1].erase(std::remove_if(edges[1].begin(), edges[1].end(),
		[&charsToRemove](char c) { return charsToRemove.find(c) != std::string::npos; }),
		edges[1].end());
	parsed.push_back(node); 
	parsed.push_back(edges[0]);
	parsed.push_back(edges[1]);
	return parsed;
}
std::map<std::string, std::pair<std::string, std::string> > generateGraph(std::vector<std::string > input ) {
	std::map<std::string, std::pair<std::string, std::string> > graph; 
	for (int i = 1; i < input.size(); i++) {
		auto nodes_and_edges = parseLine(input[i]);
		std::pair<std::string, std::string> edges; 
		edges.first = nodes_and_edges[1];
		edges.second = nodes_and_edges[2];
		graph[nodes_and_edges[0]] = edges;
	}
	return graph;
}



void traverse(std::map<std::string, std::pair<std::string, std::string> > graph, 
				std::string instructions,std::string start, std::string target) {
	int step = 0;
	int number_of_instructions = instructions.size();
	std::string current_node = start;
	while (current_node!=target) {
		if (instructions[step % number_of_instructions] == 'L') {
			current_node = graph[current_node].first;
		}
		else {
			current_node = graph[current_node].second;
		}
		step++;
	}
	std::cout << "Reached target after " << step << " steps";
}

void traverseSimultaneous(std::map<std::string, std::pair<std::string, std::string> > graph,
	std::string instructions, char start, char target) {
	//find all nodes ending with A.
	std::vector<std::string> nodes;
	for (auto& node_and_edges : graph) {
		if (node_and_edges.first.back() == start) {
			//add key_val.first to nodes. 
			nodes.push_back(node_and_edges.first);
		}
	}
	std::vector<long long> steps;
	int number_of_instructions = instructions.size();
	for (auto& node : nodes) {
		int step = 0;
		std::string current_node = node;
		bool found_target = false;
		while (!found_target) {
			if (instructions[step % number_of_instructions] == 'L') {
				current_node = graph[current_node].first;
				found_target = (current_node.back() == target);
			}
			else {
				current_node = graph[current_node].second;
				found_target = (current_node.back() == target);
			}
			step++;
		}
		steps.push_back(step);
		std::cout << "Reached target after " << step << " steps \n";
	}
	long long least_common_multiple=steps[0]; 
	for (long long& value : steps) {
		least_common_multiple = std::lcm(value, least_common_multiple);
	}
	std::cout << least_common_multiple;
}



int main(){
	std::string input = "input.txt";
	auto data = readFile(input);
	auto graph = generateGraph(data);
	auto instructions = data[0];
	std::string start = "AAA";
	std::string target = "ZZZ";
	//traverse(graph, instructions, start,target);
	char start_char = 'A';
	char target_char = 'Z';
	traverseSimultaneous(graph, instructions, start_char, target_char);
}