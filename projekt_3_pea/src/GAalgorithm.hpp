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

	// destruktor (domy�lny)
	~GAalgorithm() = default;

	// metoda uruchamiaj�ca algorytm
	int run();

	// czyszczenie p�l algorytmu
	void clear();

	// inicjalizacja populacji
	void initializePopulation(int size);

	// metoda krzyzowania OX  
	std::pair<std::vector<int>, std::vector<int>> orderCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2);

	// Metoda krzy�owania sequentialConstructiveCrossover kt�ra wywo�uje 2 krotnie metode SCX w celu otrzymania 2 potomk�w
	std::pair<std::vector<int>, std::vector<int>> sequentialConstructiveCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2);

	// metoda krzy�owania SCX zwracaj�ca 1 potomka
	std::vector<int> SCX(const std::vector<int>& parent1, const std::vector<int>& parent2);

	// metoda mutacji przez insercj�
	void insertionMutation(std::vector<int>& chromosome);

	// selekcja turniejowa
	std::vector<int> tournamentSelection(const std::vector<std::vector<int>>& population);

public:
	// metoda zwracaj�ca minimalny cykl
	std::vector<int> getBestChromosome_();

	// metoda zwracaj�ca warto�c minimalnego cyklu
	int getBestChromosomeWeight_() const;

	// metoda zwracaj�ca czas znalezienie najlepszego osobnika
	long long getTimeBestChromosomeFoundAt() const;

	// metoda zwracaj�ca liczbe osobnik�w w turnieju
	int getTournamentCount() const;

	// metoda zwracaj�ca losowego osobnika (cykl)
	std::vector<int> getRandomPath(int size);

public:
	void setMatrix(std::shared_ptr<Matrix>& matrix);

	// ustawienie kryterium czasu
	void setTimeLimit(int timeLimit);

	// ustawienie wielko�ci populacji pocz�tkowej
	void setPopulationSize(int populationSize);

	// ustawienie wspolczynnika krzyzowania
	void setCrossingFactor(double crossingFactor);

	// ewentualne ustawienie wspolczynnika mutacji
	void setMutationFactor(double mutationFactor);

	// ustawienie metody krzy�owania
	void setCrossingMethod(int choice);

	// ustawienie liczby osobnik�w w selekcji turniejowej
	void setTournamentCount(int tournamentCount);

private:
	// metoda generuj�ca losow� �cie�k�
	std::vector<int> generateRandomPath(int size);

	// metoda do obliczania kosztu �ciezki
	int calculateCost(const std::vector<int>& solution);

	// metoda sprawdzaj�ca czy gen ju� jest w chromosomie
	bool isLegitimate(const std::vector<int>& descendant, int node);

	// metoda znajduj�ca kolejny gen w stosunku do aktualnego, kt�rego nie ma w chromosomie
	int genNextNode(const std::vector<int>& parent, const std::vector<int>& descendant, int currentNode);
	
	// metoda znajduj�ca najbli�szy gen, kt�rego nie ma w chromosomie
	int findFirstLegitimate(const std::vector<int>& descendant);

private:
	// macierz
	std::shared_ptr<Matrix> matrix_;

	// populacja
	std::vector<std::vector<int>> population_;

	// wielko�� populacji
	int populationSize_;

	// kryterium stopu (w sekundach)
	long long timeLimit_;

	// czas znalezienia najlepszego rozwi�zania
	long long timeBestChromosomeFoundAt_;

	// waga minimalnego cyklu (najlepszy chromosom)
	int bestChromosomeWeight_;

	// minimalny cykl (droga najlepszego chromosomu)
	std::vector<int> bestChromosome_;

	// liczba osobnik�w w selekcji turniejowej
	int tournamentCount_;

	// metoda krzy�owania
	std::function<std::pair<std::vector<int>, std::vector<int>>(const std::vector<int>& parent1, const std::vector<int>& parent2)> crossingMethod_;

	// wsp�czynnik krzy�owania
	double crossingFactor_;

	// metoda mutacji
	std::function<void(std::vector<int>& chromosome)> mutationMethod_;

	// wsp�czynnik mutacji
	double mutationFactor_;
}; 