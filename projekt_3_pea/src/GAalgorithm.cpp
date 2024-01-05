/*
*   Julian Zalewski 263996
*   
*/

#include "GAalgorithm.hpp"

#include <unordered_map>
#include <vector>

#include <algorithm>

#include <chrono>
#include <ctime>
#include <random>

#include <climits>
#include <cmath>
#include <cstdlib>

#include <iostream>

GAalgorithm::GAalgorithm()
    : populationSize_(100)
    , population_()
    , timeLimit_(240)
    , crossingFactor_(0.8)
    , mutationFactor_(0.01)
    , crossingMethod_(std::bind(&GAalgorithm::orderCrossover, this, std::placeholders::_1, std::placeholders::_2))
    , mutationMethod_(std::bind(&GAalgorithm::mutationMethod, this))
    , bestChromosome_()
    , bestChromosomeWeight_(INT_MAX)
    //, timeMinCycleFoundAt_(0.0)
{
    srand(time(nullptr));
}

int GAalgorithm::run(const shared_ptr<Matrix>& matrix)
{
    clear();    // wyczyszczenie dotychczasowych wartoœci
    initializePopulation(matrix->getSize()); // inicjalizacja populacji

    auto startTime = std::chrono::steady_clock::now();

    while (true) // Pêtla g³ówna algorytmu
    {
        // Aktualizacja czasu
        auto currentTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count() >= timeLimit_)
            break;

        std::vector<std::vector<int>> newPopulation;    // stworzenie nowej populacji
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        while (newPopulation.size() < populationSize_) // sprawdzenie czy nowa populacja jest mniejsza ni¿ aktualna                                             
        {                                               //(tak aby ka¿da populacja liczy³a tyle samo osobników)
            // Selekcja rodziców
            std::uniform_int_distribution<> dis_ind(0, populationSize_ - 1);
            int parent1Idx = dis_ind(gen);
            int parent2Idx = dis_ind(gen);

            /*std::vector<int> parent1 = population_[parent1Idx];
            std::vector<int> parent2 = population_[parent2Idx];*/
            std::vector<int> parent1 = tournamentSelection(matrix, 4);
            std::vector<int> parent2 = tournamentSelection(matrix, 4);

            // Krzy¿owanie z zadanym prawdopodobieñstwem
            if (dis(gen) < crossingFactor_)
            {
                auto offspring = crossingMethod_(parent1, parent2);
                newPopulation.push_back(offspring.first);
                if (newPopulation.size() < populationSize_)
                    newPopulation.push_back(offspring.second);
            }
            else
            {
                newPopulation.push_back(parent1); // Dodajemy rodziców bez krzy¿owania
                if (newPopulation.size() < populationSize_)
                    newPopulation.push_back(parent2);
            }
        }

        population_ = newPopulation;

        // Aktualizacja najlepszego rozwi¹zania
        for (auto& chromosome : population_)
        {
            int cost = calculateCost(matrix, chromosome);
            if (cost < bestChromosomeWeight_)
            {
                bestChromosome_ = chromosome;
                bestChromosomeWeight_ = cost;
            }
        }
        std::cout << bestChromosomeWeight_ << '\n';
    }
    
    return bestChromosomeWeight_;
}

std::vector<int> GAalgorithm::tournamentSelection(const shared_ptr<Matrix>& matrix, int tournamentSize) 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, populationSize_ - 1);

    int bestIdx = dis(gen);
    int bestCost = calculateCost(matrix, population_[bestIdx]);

    for (int i = 1; i < tournamentSize; ++i) 
    {
        int idx = dis(gen);
        int cost = calculateCost(matrix, population_[idx]);
        if (cost < bestCost) 
        {
            bestCost = cost;
            bestIdx = idx;
        }
    }

    return population_[bestIdx];
}

std::vector<int> GAalgorithm::getBestChromosome_()
{
    return bestChromosome_;
}

int GAalgorithm::getBestChromosomeWeight_() const
{
    return bestChromosomeWeight_;
}

long long GAalgorithm::getTimeMinCycleFoundAt() const
{
    //return timeMinCycleFoundAt_;
    return 0;
}

void GAalgorithm::initializePopulation(int numberOfCities)
{
    population_.clear(); // wyczyszczenie dotychczasowej populacji

    for (int i = 0; i < populationSize_; i++)
    {
        population_.push_back(generateRandomPath(numberOfCities));
    }
}

std::vector<int> GAalgorithm::generateRandomPath(int numberOfCities)
{
    std::vector<int> path(numberOfCities);

    // Wype³nienie wektora kolejnymi liczbami (miastami)
    for (int i = 0; i < numberOfCities; ++i) {
        path[i] = i;
    }

    // losowa zamiana miast w œcie¿ce
    std::random_device rd;
    std::mt19937 random(rd());

    std::shuffle(path.begin(), path.end(), random);
    return path;
}

std::vector<int> GAalgorithm::getRandomPath(int size)
{
    return generateRandomPath(size);
}

int GAalgorithm::calculateCost(const shared_ptr<Matrix>& matrix, const std::vector<int>& solution)
{
    int cost = 0;
    for (size_t i = 0; i < solution.size() - 1; ++i)
    {
        cost += (*matrix)[solution[i]][solution[i + 1]];
    }
    cost += (*matrix)[solution.back()][solution.front()]; // Powrót do pocz¹tkowego wierzcho³ka
    return cost;
}

std::pair<std::vector<int>, std::vector<int>> GAalgorithm::orderCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2) {
    int size = parent1.size();
    std::vector<int> offspring1(size, -1);
    std::vector<int> offspring2(size, -1);

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> dis(0, size - 2);
    int start = dis(g);
    int end = dis(g) + 1;  // Zapewnienie, ¿e segment bêdzie mieæ co najmniej d³ugoœæ 1

    if (start > end) {
        std::swap(start, end);
    }

    // Skopiowanie segmentu z pierwszego rodzica do pierwszego potomka
    for (int i = start; i <= end; ++i) {
        offspring1[i] = parent1[i];
    }

    // Skopiowanie segmentu z drugiego rodzica do drugiego potomka
    for (int i = start; i <= end; ++i) {
        offspring2[i] = parent2[i];
    }

    // Uzupe³nienie potomka1 elementami z rodzica2
    int current = (end + 1) % size;
    for (int i = 0; i < size; ++i) {
        int gene = parent2[(end + 1 + i) % size];
        if (std::find(offspring1.begin(), offspring1.end(), gene) == offspring1.end()) {
            offspring1[current] = gene;
            current = (current + 1) % size;
        }
    }

    // Uzupe³nienie potomka2 elementami z rodzica1
    current = (end + 1) % size;
    for (int i = 0; i < size; ++i) {
        int gene = parent1[(end + 1 + i) % size];
        if (std::find(offspring2.begin(), offspring2.end(), gene) == offspring2.end()) {
            offspring2[current] = gene;
            current = (current + 1) % size;
        }
    }

    return { offspring1, offspring2 };
}

std::pair<std::vector<int>, std::vector<int>> GAalgorithm::modifiedOrderCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2) {
    int size = parent1.size();
    std::vector<int> offspring1(size, -1), offspring2(size, -1);

    // Ustawienie generatora liczb losowych
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> dis(0, size - 1);

    // Losowanie dwóch punktów krzy¿owania
    int start = dis(g);
    int end = dis(g);
    if (start > end) {
        std::swap(start, end);
    }

    // Kopiowanie segmentów miêdzy punktami krzy¿owania
    for (int i = start; i <= end; ++i) {
        offspring1[i] = parent1[i];
        offspring2[i] = parent2[i];
    }

    // Wype³nienie potomka 1
    int index1 = (end + 1) % size;
    int index2 = index1;
    for (int i = 0; i < size; ++i) {
        int current = (end + 1 + i) % size;
        if (std::find(offspring1.begin() + start, offspring1.begin() + end + 1, parent2[current]) == offspring1.begin() + end + 1) {
            offspring1[index1] = parent2[current];
            index1 = (index1 + 1) % size;
        }
        if (std::find(offspring2.begin() + start, offspring2.begin() + end + 1, parent1[current]) == offspring2.begin() + end + 1) {
            offspring2[index2] = parent1[current];
            index2 = (index2 + 1) % size;
        }
    }

    return { offspring1, offspring2 };
}

void GAalgorithm::mutationMethod()
{
    return;
}

void GAalgorithm::setTimeLimit(int timeLimit)
{
    timeLimit_ = timeLimit;
}

void GAalgorithm::setPopulationSize(int populationSize)
{
    populationSize_ = populationSize;
}

void GAalgorithm::setCrossingFactor(double crossingFactor)
{
    crossingFactor_ = crossingFactor;
}

void GAalgorithm::setMutationFactor(double mutationFactor)
{
    mutationFactor_ = mutationFactor;
}

void GAalgorithm::setCrossingMethod(int choice)
{
    if (choice == 1)
    {
        std::bind(&GAalgorithm::orderCrossover, crossingMethod_, std::placeholders::_1, std::placeholders::_2);
    }
    else
    {
        std::bind(&GAalgorithm::modifiedOrderCrossover, crossingMethod_, std::placeholders::_1, std::placeholders::_2);
    }
}

void GAalgorithm::clear()
{
    bestChromosome_.clear();
    bestChromosomeWeight_ = INT_MAX;
    population_.clear();
    //timeMinCycleFoundAt_ = 0;
}