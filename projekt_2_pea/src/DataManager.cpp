#include "DataManager.hpp"
#include <fstream>
#include <iostream>
#include <cmath>


std::vector<int> DataManager::readFile(const std::string& fileName)
{
	std::vector<int> cycle;
	std::ifstream inDataFile;

	inDataFile.open(fileName);

	if (!inDataFile) {
		std::cout << "Nie mozna otworzyc pliku: " << fileName << std::endl;
		return { 0 };
	}

	int size;
	inDataFile >> size; // wczytanie d³ugoœci cyklu z 1 linii z pliku

	int city;
	for (int i = 0; i < size && inDataFile >> city; i++)
	{
		cycle.push_back(city);
	}

	inDataFile.close();

	return cycle;
}

void DataManager::saveFile(std::vector<int> cycle, std::string fileName)
{
	const std::string result = fileName;
	std::ofstream outDataFile(result, std::ios_base::app);

	if (!outDataFile.is_open())
	{
		std::cout << "Nie mozna otworzyc pliku: " << result << std::endl;
		return;
	}

	// Zapisz iloœæ elementów w wektorze do pliku
	outDataFile << cycle.size() << std::endl;

	// Zapisz elementy wektora do pliku
	for (const auto& city : cycle) {
		outDataFile << city << std::endl;
	}
	outDataFile << cycle.at(0);

	outDataFile.close();
}

void DataManager::saveSeparators(std::string fileName)
{
	const std::string result = fileName;
	std::ofstream outDataFile(fileName, std::ios_base::app);

	if (!outDataFile.is_open())
	{
		std::cout << "Nie mozna otworzyc pliku: " << result << std::endl;
		return;
	}

	outDataFile << "\n=============================\n";

	outDataFile.close();
}

void DataManager::displayVector(std::vector<int> cycle)
{
	std::cout << cycle.size() << std::endl;

	for (const auto& city : cycle)
	{
		std::cout << city << std::endl;
	}
	std::cout << cycle.at(0);
}

void DataManager::saveFileTest(int minCycle, long long time, double a, double tk, const std::string& fileName) 
{
	std::ofstream outDataFile(fileName, std::ios_base::app);

	if (!outDataFile.is_open())
	{
		std::cout << "Nie mozna otworzyc pliku: " << fileName << std::endl;
		return;
	}

	outDataFile << "minimalny cykl: " << minCycle << ", czas: " << time << ", wsp (a): " << a <<", tk: " << tk <<", exp(-1/Tk)" << exp(-1/tk) << std::endl;
	outDataFile.close();
}