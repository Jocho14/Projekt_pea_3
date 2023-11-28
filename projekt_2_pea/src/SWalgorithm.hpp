#pragma once

#include "Matrix.hpp"
#include <memory>
#include <vector>
#include <cmath>

class SWalgorithm
{
public:
	SWalgorithm();
	~SWalgorithm();

	// metoda uruchamiająca algorytm
	int run(const shared_ptr<Matrix>& matrix, int timeLimit, double a);

	// metoda zwracająca minimalny cykl
	std::vector<int> getMinCycle();

	// metoda obliczająca koszt cyklu
	int calculateCost(const shared_ptr<Matrix>& matrix, const std::vector<int>& solution);

	double getTemperature();

	double getProbability();

private:

	// metoda zamieniająca miejscami losowo wybrane wierzchołki
	void swap(std::vector<int>& solution);

	// metoda generująca początkowe rozwiązanie
	std::vector<int> generateInitialSolution(int n);

private:
	// minimalna waga cyklu
	int minCycleWeight;

	// minimalny cykl
	std::vector<int> minCycle;

	// temperatura (początkowa)
	double temperature;

	// e^(-1/temperature)
	double probability;

	// temperatura minimalna
	double minTemperature;
};