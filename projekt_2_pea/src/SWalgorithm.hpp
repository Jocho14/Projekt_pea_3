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
	int run(const shared_ptr<Matrix>& matrix, const int timeLimit, const double a);

	// metoda generuj¹ca pocz¹tkowe rozwi¹zanie przy u¿yciu metody zach³annej
	std::vector<int> generateBFSSolution(const shared_ptr<Matrix>& matrix);

	// metoda obliczaj¹ca koszt cyklu
	int calculateCost(const shared_ptr<Matrix>& matrix, const std::vector<int>& solution);

	// czyszczenie pól algorytmu
	void clear(); 

	// metoda zwracaj¹ca minimalny cykl
	std::vector<int> getMinCycle();

	// metoda zwracaj¹ca temperature
	double getTemperature();

	// metoda zwracaj¹ca prawdopodobieñstwo wed³ug wzoru exp(-delta/Tk)
	double getProbability();

	// metoda zwracaj¹ca wartoœc minimalnego cyklu
	int getMinCycleWeight();

	// metoda zwracaj¹ca moment czasu w którym znaleziono najlepsze rozwi¹zanie
	long long getTimeMinCycleFoundAt();

private:
	// metoda przemieszczaj¹ca losowy wierzcho³ek w losowe miejsce
	void insert(std::vector<int>& solution);

private:
	// kolejne metody poœwiêcone s¹ wyliczeniu pocz¹tkowej temperatury
	// metoda pomocnicza do obliczenia sumy exp kosztów
	double expSum(const shared_ptr<Matrix>& matrix, double T, bool isMax);
	
	// metoda do obliecznia chi(T) dla danego T
	double chi(const shared_ptr<Matrix>& matrix, double T);

	// metoda do znalezienia temperatury pocz¹tkowej
	double findInitialTemperature(const shared_ptr<Matrix>& matrix, double x0, double epsilon, double p);

private:
	// waga minimalnego cyklu
	int minCycleWeight;

	// minimalny cykl
	std::vector<int> minCycle;

	// moment czasu, w którym odnaleziono najlepsze rozwi¹zanie
	long long timeMinCycleFoundAt;

	// temperatura (pocz¹tkowa)
	double temperature;

	// temperatura minimalna
	double minTemperature;

	// e^(-1/temperature) prawdopodobieñstwo wyboru gorszego rozwi¹zania
	double probability;
}; 