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
	int run();

	// czyszczenie pól algorytmu
	void clear();

	// inicjalizacja populacji
	void initializePopulation(int size);

	// metoda krzyzowania OX  
	std::pair<std::vector<int>, std::vector<int>> orderCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2);

	// Metoda krzy¿owania sequentialConstructiveCrossover która wywo³uje 2 krotnie metode SCX w celu otrzymania 2 potomków
	std::pair<std::vector<int>, std::vector<int>> sequentialConstructiveCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2);

	// metoda krzy¿owania SCX zwracaj¹ca 1 potomka
	std::vector<int> SCX(const std::vector<int>& parent1, const std::vector<int>& parent2);

	// metoda mutacji przez insercjê
	void insertionMutation(std::vector<int>& chromosome);

	// selekcja turniejowa
	std::vector<int> tournamentSelection(const std::vector<std::vector<int>>& population);

public:
	// metoda zwracaj¹ca minimalny cykl
	std::vector<int> getBestChromosome_();

	// metoda zwracaj¹ca wartoœc minimalnego cyklu
	int getBestChromosomeWeight_() const;

	// metoda zwracaj¹ca czas znalezienie najlepszego osobnika
	long long getTimeBestChromosomeFoundAt() const;

	// metoda zwracaj¹ca liczbe osobników w turnieju
	int getTournamentCount() const;

	// metoda zwracaj¹ca losowego osobnika (cykl)
	std::vector<int> getRandomPath(int size);

public:
	void setMatrix(std::shared_ptr<Matrix>& matrix);

	// ustawienie kryterium czasu
	void setTimeLimit(int timeLimit);

	// ustawienie wielkoœci populacji pocz¹tkowej
	void setPopulationSize(int populationSize);

	// ustawienie wspolczynnika krzyzowania
	void setCrossingFactor(double crossingFactor);

	// ewentualne ustawienie wspolczynnika mutacji
	void setMutationFactor(double mutationFactor);

	// ustawienie metody krzy¿owania
	void setCrossingMethod(int choice);

	// ustawienie liczby osobników w selekcji turniejowej
	void setTournamentCount(int tournamentCount);

private:
	// metoda generuj¹ca losow¹ œcie¿kê
	std::vector<int> generateRandomPath(int size);

	// metoda do obliczania kosztu œciezki
	int calculateCost(const std::vector<int>& solution);

	// metoda sprawdzaj¹ca czy gen ju¿ jest w chromosomie
	bool isLegitimate(const std::vector<int>& descendant, int node);

	// metoda znajduj¹ca kolejny gen w stosunku do aktualnego, którego nie ma w chromosomie
	int genNextNode(const std::vector<int>& parent, const std::vector<int>& descendant, int currentNode);
	
	// metoda znajduj¹ca najbli¿szy gen, którego nie ma w chromosomie
	int findFirstLegitimate(const std::vector<int>& descendant);

private:
	// macierz
	std::shared_ptr<Matrix> matrix_;

	// populacja
	std::vector<std::vector<int>> population_;

	// wielkoœæ populacji
	int populationSize_;

	// kryterium stopu (w sekundach)
	long long timeLimit_;

	// czas znalezienia najlepszego rozwi¹zania
	long long timeBestChromosomeFoundAt_;

	// waga minimalnego cyklu (najlepszy chromosom)
	int bestChromosomeWeight_;

	// minimalny cykl (droga najlepszego chromosomu)
	std::vector<int> bestChromosome_;

	// liczba osobników w selekcji turniejowej
	int tournamentCount_;

	// metoda krzy¿owania
	std::function<std::pair<std::vector<int>, std::vector<int>>(const std::vector<int>& parent1, const std::vector<int>& parent2)> crossingMethod_;

	// wspó³czynnik krzy¿owania
	double crossingFactor_;

	// metoda mutacji
	std::function<void(std::vector<int>& chromosome)> mutationMethod_;

	// wspó³czynnik mutacji
	double mutationFactor_;
}; 