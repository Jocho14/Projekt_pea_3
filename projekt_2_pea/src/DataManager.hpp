#pragma once

#include <vector>
#include <string>

class DataManager 
{
public:
	static std::vector<int> readFile(const std::string& fileName);
	static void saveFile(std::vector<int> cycle);
	static void displayVector(std::vector<int> cycle);
};