// LargeTextSearch.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <array>
#include <filesystem>

#include "sorted_vector.hpp"

namespace fs = std::experimental::filesystem;

int main()
{
	std::string fileName = "words.txt";

	// Get the file size (by way of API)
	std::size_t fileSize = fs::file_size(fs::path(fileName));
	
	// Preallocate 
	lts::sorted_vector<std::string> dictionary(fileSize);

	std::cout << "Reading file " << fileName << '\n';

	// Load dictionary into memory
	{
		int x = 0;
		std::ifstream textFile(fileName);

		std::string line;
		while (std::getline(textFile, line))
			dictionary.insert(std::move(line));
	}

	dictionary.sort();
	std::string input;

	std::cout << "Search string: " << input;
	std::cin >> input;

	std::cout << "Retrieving at most 20 strings starting with " << input << "...\n";

	// Retrieve 20 matches starting with input string
	auto matches = dictionary.find_matches(input, 20);
	for (auto match : matches)
		std::cout << "Match: " << static_cast<std::string>(match) << '\n';

	std::cout << "Done.\n";
	std::cin.get();
    return 0;
}

