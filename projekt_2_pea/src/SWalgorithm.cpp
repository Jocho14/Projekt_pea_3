#include "SWalgorithm.hpp"

#include <climits>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <iostream>
#include <unordered_map>

SWalgorithm::SWalgorithm()
    : minCycleWeight(INT_MAX)
    , minCycle()
	, temperature(1000.0)
    , minTemperature(0)
    , probability(0)
    , timeMinCycleFoundAt(0)
{
    srand(time(nullptr));
}
// algorytm na ere
// temperature wyrzuciæ z ery
int SWalgorithm::run(const shared_ptr<Matrix>& matrix, int timeLimit, double a) 
{
    auto startTime = std::chrono::steady_clock::now();  // rozpoczêcie odliczania czasu

    auto currentCycle = generateBFSSolution(matrix);                    // wygenerowanie pocz¹tkowego rozwi¹zania
    //auto currentCycle = generateStartSolution(matrix->getSize());
    double currentCycleWeight = calculateCost(matrix, currentCycle);    // obliczenie kosztu pocz¹tkowego rozwi¹zania

    temperature = findInitialTemperature(matrix, 0.95, 0.001, 1); // obliczenie pocz¹tkowej temperatury (0.8, 0.001, 1)
    std::cout << "temperatura = : " << temperature;

    minCycle = currentCycle;               // Ustawienie minimalnego cyklu na pocz¹tkowy cykl
    minCycleWeight = currentCycleWeight;   // ustawienie minimalnego kosztu cyklu na pocz¹tkow¹ wartoœæ rozwi¹zania
    // 1000 era dla ftv55.xml
    int era = 1000; // 1000 iteracji dla ka¿dego puntku temperatury, w celu znalezienia najlepszego rozwi¹zania
    while (temperature > minTemperature) // Kryterium stopu - temperatura koñcowa
    {
        int eraIterator = 0;

        while (eraIterator < era)
        {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();

            if (elapsedTime >= timeLimit) // Sprawdzenie kryterium czasowego
            {
                return minCycleWeight;
            }

            std::vector<int> newSolution = currentCycle; // stworzenie nowego cyklu

            insert(newSolution);  // zamiana miast w nowym cyklu najlepsze dla ftv170
            //swap(newSolution);

            double newCost = calculateCost(matrix, newSolution);  // liczenie nowego kosztu
            double costDifference = newCost - currentCycleWeight;       // ró¿nica temperatur delta y

            probability = exp(-costDifference / temperature);   // obliczenie prawdopodobieñstwa

            if (costDifference < 0 || (probability > ((double)rand() / RAND_MAX))) // sprawdzenie czy nowy koszt jest mniejszy od aktualnego
            {                                                                       // lub prawdopodobieñstwo jest wiêksze ni¿ losowa wartoœæ
                currentCycle = newSolution;                                         // w celu wyjœcia z optimum lokalnego
                currentCycleWeight = newCost;

                if (newCost < minCycleWeight)  // sprawdzenie czy nowy koszt jest wiêkszy ni¿ dotychczasowy
                {
                    auto exactTime = std::chrono::steady_clock::now();
                    timeMinCycleFoundAt = std::chrono::duration_cast<std::chrono::nanoseconds>(exactTime - startTime).count();

                    minCycle = newSolution;     // ustawienie minimalnego cyklu na nowy cykl
                    minCycleWeight = newCost;   // ustawienie wartoœci minimalnego cyklu na wartoœæ nowego cyklu (najlepsze rozwiazanie)
                }
            }
            eraIterator++;
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

std::vector<int> SWalgorithm::generateBFSSolution(const shared_ptr<Matrix>& matrix)
{
    std::vector<int> result;
    std::unordered_map<int, bool> visited;

    for (int city = 0; city < matrix->getSize(); city++)
    {
        visited[city] = false;
    }

    int minimum = INT_MAX;

    int city = 0;
    int node = 0;
    visited[0] = true;
    result.push_back(city);

    int i = 1;
    while (i < matrix->getSize())
    {
        for (int smallest = 0; smallest < matrix->getSize(); smallest++)
        {
            if ((*matrix)[city][smallest] < minimum && visited[smallest] == false && city != smallest)
            {
                minimum = (*matrix)[city][smallest];
                node = smallest;
            }
        }
        visited[node] = true;
        minimum = INT_MAX;
        result.push_back(node);
        city = node;

        i++;
    }
    return result;
}

std::vector<int> SWalgorithm::generateStartSolution(const int n) 
{
    std::vector<int> solution(n);
    for (int i = 0; i < n; ++i) 
    {
        solution[i] = i;
    }
    std::random_shuffle(solution.begin(), solution.end()); // Losowa permutacja wierzcho³ków
    return solution;
}

double SWalgorithm::expSum(const shared_ptr<Matrix>& matrix, double T, bool isMax)
{
        double sum = 0.0;
        for (int i = 0; i < matrix->getSize(); ++i)
        {
            for (int j = 0; j < matrix->getSize(); ++j)
            {
                if (i != j) {
                    double cost = static_cast<double>((*matrix)[i][j]);
                    sum += exp((isMax ? -cost : cost) / T);
                }
            }
        }
        return sum;
}

double SWalgorithm::chi(const shared_ptr<Matrix>& matrix, double T)
{
    double sumMax = expSum(matrix, T, true);
    double sumMin = expSum(matrix, T, false);
    return sumMax / sumMin;
}

double SWalgorithm::findInitialTemperature(const shared_ptr<Matrix>& matrix, double x0, double epsilon, double p)
{
    double Tn = 1.0;
    double Tnext;
    double chiTn = chi(matrix, Tn);

    while (fabs(chiTn - x0) > epsilon)
    {
        Tnext = Tn * pow(log(chiTn) / log(x0), 1.0 / p);
        Tn = Tnext;
        chiTn = chi(matrix, Tn);
    }

    return Tn;
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
}

void SWalgorithm::insert(vector<int>& solution) {

    int i = rand() % solution.size();
    int j = rand() % solution.size();
    while (i == j)
    {
        j = rand() % solution.size();
    }
   
    int valueToInsert = solution[i];
 
    solution.erase(solution.begin() + i);
   
    solution.insert(solution.begin() + j, valueToInsert);
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

long long SWalgorithm::getTimeMinCycleFoundAt()
{
    return timeMinCycleFoundAt;
}

void SWalgorithm::clear()
{
    minCycle.clear();
    minCycleWeight = INT_MAX;
    temperature = 1000.0;
    timeMinCycleFoundAt = 0;
}