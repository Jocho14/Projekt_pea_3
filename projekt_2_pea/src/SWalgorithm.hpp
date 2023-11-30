#pragma once

#include "Matrix.hpp"
#include <memory>
#include <vector>
#include <cmath>

class SWalgorithm
{
public:
	// konstruktor
	SWalgorithm();

	// destruktor (domyœlny)
	~SWalgorithm() = default;

	// metoda uruchamiaj¹ca algorytm
	int run(const shared_ptr<Matrix>& matrix, int timeLimit, double a);

	// metoda zwracaj¹ca minimalny cykl
	std::vector<int> getMinCycle();

	// metoda obliczaj¹ca koszt cyklu
	int calculateCost(const shared_ptr<Matrix>& matrix, const std::vector<int>& solution);

	// metoda zwracaj¹ca temperature
	double getTemperature();

	// metoda zwracaj¹ca prawdopodobieñstwo wed³ug wzoru exp(-delta/Tk)
	double getProbability();

	// metoda zwracaj¹ca wartoœc minimalnego cyklu
	int getMinCycleWeight();

	// czyszczenie pól algorytmu
	void clear(); 

private:

	// metoda zamieniaj¹ca miejscami losowo wybrane wierzcho³ki
	void swap(std::vector<int>& solution);

	// metoda generuj¹ca pocz¹tkowe rozwi¹zanie
	std::vector<int> generateInitialSolution(int n);

private:
	// minimalna waga cyklu
	int minCycleWeight;

	// minimalny cykl
	std::vector<int> minCycle;

	// temperatura (pocz¹tkowa)
	double temperature;

	// e^(-1/temperature)
	double probability;

	// temperatura minimalna
	double minTemperature;
};