/*
*   Julian Zalewski 263996
*
*/

#pragma once

#include "Matrix.hpp"
#include <memory>
#include <vector>
#include <cmath>
#include <functional>

class GAalgorithm
{
public:
	// konstruktor
	GAalgorithm();

	// destruktor (domyœlny)
	~GAalgorithm() = default;

	// metoda uruchamiaj¹ca algorytm
	int run(const shared_ptr<Matrix>& matrix);

	// inicjalizacja populacji
	void initializePopulation(int numberOfCities);

	// metoda OX krzyzowania 
	std::pair<std::vector<int>, std::vector<int>> orderCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2);

	// metoda MOX krzyzowania
	std::pair<std::vector<int>, std::vector<int>> modifiedOrderCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2);

	// selekcja turniejowa
	std::vector<int> tournamentSelection(const shared_ptr<Matrix>& matrix, int tournamentSize);

	// metoda mutacji
	void mutationMethod();
	
	// czyszczenie pól algorytmu
	void clear();

	// metoda zwracaj¹ca minimalny cykl
	std::vector<int> getBestChromosome_();

	// metoda zwracaj¹ca wartoœc minimalnego cyklu
	int getBestChromosomeWeight_() const;

	// metoda zwracaj¹ca moment czasu w którym znaleziono najlepsze rozwi¹zanie
	long long getTimeMinCycleFoundAt() const;

public:
	// ustawienie kryterium czasu
	void setTimeLimit(int timeLimit);

	// ustawienie wielkoœci populacji pocz¹tkowej
	void setPopulationSize(int populationSize);

	// ustawienie wspolczynnika krzyzowania
	void setCrossingFactor(double crossingFactor);

	//// ewentualne ustawienie wspolczynnika mutacji
	void setMutationFactor(double mutationFactor);

	//// ustawienie metody krzy¿owania
	void setCrossingMethod(int choice);

	//// ewentualne ustawienie metody mutacji
	//void setMutationMethod(int choice);

	std::vector<int> getRandomPath(int size);

private:
	std::vector<int> generateRandomPath(int size);

	//metoda do obliczania kosztu œciezki
	int calculateCost(const shared_ptr<Matrix>& matrix, const std::vector<int>& solution);

private:
	// kryterium stopu (w sekundach)
	long long timeLimit_;

	// wielkoœæ populacji
	int populationSize_;

	// populacja
	std::vector<std::vector<int>> population_;

	// wspó³czynnik krzy¿owania
	double crossingFactor_;

	// wspó³czynnik mutacji
	double mutationFactor_;

	// metoda krzy¿owania
	std::function<std::pair<std::vector<int>, std::vector<int>>(const std::vector<int>& parent1, const std::vector<int>& parent2)> crossingMethod_;

	// metoda mutacji
	std::function<void()> mutationMethod_;

private:
	// waga minimalnego cyklu (najlepszy chromosom)
	int bestChromosomeWeight_;

	// minimalny cykl (droga najlepszego chromosomu)
	std::vector<int> bestChromosome_;

	// moment czasu, w którym odnaleziono najlepsze rozwi¹zanie
	//long long timeMinCycleFoundAt_;
}; 