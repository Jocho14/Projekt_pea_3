#pragma once

#include <vector>
#include <string>

class DataManager 
{
public:
	// metoda do odczytywania wyniku (cyklu) z pliku
	static std::vector<int> readFile(const std::string& fileName);

	// metod do zapisywania wyniku (cyklu) do pliku
	static void saveFile(std::vector<int> cycle, std::string fileName);

	// metoda do wyœwietlania cyklu w konsoli
	static void displayVector(std::vector<int> cycle);

	// metoda do dodania speratorów w pliku tekstowym
	static void saveSeparators(std::string fileName);

	// metoda do zapisywania danych do sprawozdania
	static void saveFileTest(int minCycle, long long time, double a, double tk, const std::string& fileName);

	// metoda dla ga
	static void saveFileTestForGA(int bestChromosomeWeight, int tournamentCount, int populationSize, long long time, const std::string& fileName);
};