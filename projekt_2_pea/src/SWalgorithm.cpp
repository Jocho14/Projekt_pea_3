#include "SWalgorithm.hpp"

#include <climits>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>

SWalgorithm::SWalgorithm()
    : minCycleWeight(INT_MAX)
    , minCycle()
	, temperature(10000.0)
    , minTemperature(0.0001)
    , probability(1)
{
    srand(time(nullptr));
}

SWalgorithm::~SWalgorithm()
{

}

int SWalgorithm::run(const shared_ptr<Matrix>& matrix, int timeLimit, double a) 
{
    auto startTime = std::chrono::steady_clock::now();  // rozpoczêcie odliczania czasu

    std::vector<int> currentSolution = generateInitialSolution(matrix->getSize());
    double currentCost = calculateCost(matrix, currentSolution);

    minCycle = currentSolution;
    minCycleWeight = currentCost;

    // Obliczenie temperatury pocz¹tkowej (zaimplementowaæ logike obliczania!!!!!)

    while (temperature > minTemperature) // Kryterium stopu - temperatura koñcowa
    { 
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

        if (elapsedTime >= timeLimit) // Sprawdzenie kryterium czasowego
        { 
            break;
        }

        std::vector<int> newSolution = currentSolution;
        swap(newSolution);

        double newCost = calculateCost(matrix, newSolution);  // liczenie nowego kosztu
        double costDifference = newCost - currentCost;       // ró¿nica temperatur delta y

        probability = exp(-costDifference / temperature);

        if (costDifference < 0 || (probability > ((double)rand() / RAND_MAX))) 
        {
            currentSolution = newSolution;
            currentCost = newCost;

            if (newCost < minCycleWeight) 
            {
                minCycle = newSolution;
                minCycleWeight = newCost;
            }
        }
        temperature *= a; // Sch³adzanie
    }
    return minCycleWeight; // najlepsze rozwi¹zanie
}

int SWalgorithm::calculateCost(const shared_ptr<Matrix>& matrix, const std::vector<int>& solution) {
    int cost = 0;
    for (size_t i = 0; i < solution.size() - 1; ++i) {
        cost += (*matrix)[solution[i]][solution[i + 1]];
    }
    cost += (*matrix)[solution.back()][solution.front()]; // Powrót do pocz¹tkowego wierzcho³ka
    return cost;
}

std::vector<int> SWalgorithm::generateInitialSolution(int n) {
    std::vector<int> solution(n);
    for (int i = 0; i < n; ++i) {
        solution[i] = i;
    }
    std::random_shuffle(solution.begin(), solution.end()); // Losowa permutacja wierzcho³ków
    return solution;
}

void SWalgorithm::swap(std::vector<int>& solution) {
    int i = rand() % solution.size();
    int j = rand() % solution.size();
    while (i == j) {
        j = rand() % solution.size();
    }
    std::swap(solution[i], solution[j]);
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