#include "SWalgorithm.hpp"

#include <climits>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <iostream>

SWalgorithm::SWalgorithm()
    : minCycleWeight(INT_MAX)
    , minCycle()
	, temperature(100000.0)
    , minTemperature(0.00001)
    , probability(0)
{
    srand(time(nullptr));
}

int SWalgorithm::run(const shared_ptr<Matrix>& matrix, int timeLimit, double a) 
{
    auto startTime = std::chrono::steady_clock::now();  // rozpoczêcie odliczania czasu

    std::vector<int> currentCycle = generateInitialSolution(matrix->getSize());  // wygenerowanie pocz¹tkowego rozwi¹zania
    double currentCycleWeight = calculateCost(matrix, currentCycle);    // obliczenie kosztu pocz¹tkowego rozwi¹zania

    temperature = currentCycleWeight * a;  // obliczenie pocz¹tkowej temperatury

    minCycle = currentCycle;               // Ustawienie minimalnego cyklu na pocz¹tkowy cykl
    minCycleWeight = currentCycleWeight;   // ustawienie minimalnego kosztu cyklu na pocz¹tkow¹ wartoœæ rozwi¹zania

    while (temperature > minTemperature) // Kryterium stopu - temperatura koñcowa
    { 
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

        if (elapsedTime >= timeLimit) // Sprawdzenie kryterium czasowego
        { 
            break;
        }

        std::vector<int> newSolution = currentCycle; // stworzenie nowego cyklu
        swap(newSolution);         // zamiana miast w nowym cyklu

        double newCost = calculateCost(matrix, newSolution);  // liczenie nowego kosztu
        double costDifference = newCost - currentCycleWeight;       // ró¿nica temperatur delta y

        probability = exp(-costDifference / temperature);   // obliczenie prawdopodobieñstwa

        if (costDifference < 0 || (probability > ((double)rand() / RAND_MAX))) // sprawdzenie czy nowy koszt jest mniejszy od aktualnego
        {                                                                       // lub prawdopodobieñstwo jest wiêksze ni¿ losowa wartoœæ
            currentCycle = newSolution;                                         // w celu wyjœcia z optimum lokalnego
            currentCycleWeight = newCost;

            if (newCost < minCycleWeight)  // sprawdzenie czy nowy koszt jest wiêkszy ni¿ dotychczasowy
            {
                minCycle = newSolution;     // ustawienie minimalnego cyklu na nowy cykl
                minCycleWeight = newCost;   // ustawienie wartoœci minimalnego cyklu na wartoœæ nowego cyklu (najlepsze rozwiazanie)
            }
        }
        temperature *= a; // Sch³adzanie
    }
    return minCycleWeight; // najlepsze rozwi¹zanie
}

int SWalgorithm::calculateCost(const shared_ptr<Matrix>& matrix, const std::vector<int>& solution)
{
    int cost = 0;
    for (size_t i = 0; i < solution.size() - 1; ++i) 
    {
        cost += (*matrix)[solution[i]][solution[i + 1]];
    }
    cost += (*matrix)[solution.back()][solution.front()]; // Powrót do pocz¹tkowego wierzcho³ka
    return cost;
}

std::vector<int> SWalgorithm::generateInitialSolution(int n) 
{
    std::vector<int> solution(n);
    for (int i = 0; i < n; ++i) 
    {
        solution[i] = i;
    }
    std::random_shuffle(solution.begin(), solution.end()); // Losowa permutacja wierzcho³ków
    return solution;
}

void SWalgorithm::swap(std::vector<int>& solution) 
{
    int i = rand() % solution.size();
    int j = rand() % solution.size();
    while (i == j) 
    {
        j = rand() % solution.size();
    }
    std::swap(solution[i], solution[j]);

    /*for (int k = 0; k < (solution.size() / 2); k++)
    {
        int i = rand() % solution.size();
        int j = rand() % solution.size();

        while (i == j)
        {
            j = rand() % solution.size();
        }

        std::swap(solution[i], solution[j]);
    }*/
}

std::vector<int> SWalgorithm::getMinCycle()
{
    return minCycle;
}

double SWalgorithm::getTemperature()
{
    return temperature;
}

double SWalgorithm::getProbability()
{
    return probability;
}

int SWalgorithm::getMinCycleWeight()
{
    return minCycleWeight;
}

void SWalgorithm::clear()
{
    minCycle.clear();
    minCycleWeight = INT_MAX;
    temperature = 100000.0;
}