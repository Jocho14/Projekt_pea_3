/*
*   Julian Zalewski 263996
*   
*/

#include "GAalgorithm.hpp"

#include <unordered_map>
#include <vector>

#include <algorithm>
#include <numeric>

#include <chrono>
#include <ctime>
#include <random>

#include <climits>
#include <limits>
#include <cmath>
#include <cstdlib>

#include <iostream>

GAalgorithm::GAalgorithm()
    : matrix_()
    , populationSize_(2000)
    , population_()
    , timeLimit_(240)
    , timeBestChromosomeFoundAt_(0)
    , crossingFactor_(0.8)
    , mutationFactor_(0.01)
    , crossingMethod_(std::bind(&GAalgorithm::orderCrossover, this, std::placeholders::_1, std::placeholders::_2))
    , mutationMethod_(std::bind(&GAalgorithm::insertionMutation, this, std::placeholders::_1))
    , bestChromosome_()
    , bestChromosomeWeight_(INT_MAX)
    , tournamentCount_(9)
{
    srand(time(nullptr));
}

int GAalgorithm::run()
{
    clear();    // wyczyszczenie dotychczasowych wartoœci
    if (!matrix_) // sprawdzenie czy macierz zosta³a ustawiona
    {
        return 0;
    }

    initializePopulation(matrix_->getSize()); // inicjalizacja populacji

    int crossingChromosomesCount = populationSize_ * crossingFactor_; // liczba chromosomów, które bêd¹ siê krzy¿owaæ
    int mutationChromosomesCount = populationSize_ * mutationFactor_; // liczba chromosomów, które bêd¹ mutowaæ

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

        // selekcja nowej populacji
        for (int i = 0; i < populationSize_; i++)
        {
            auto chromosome = tournamentSelection(population_);
            newPopulation.push_back(chromosome);
        }

        // krzy¿owanie dok³adnie zadanej liczby chromosomów (rozmiar populacji * wspó³czynnik krzy¿owania)
        for (int i = 0; i < crossingChromosomesCount; i += 2)
        {
            auto parent1 = newPopulation[i];
            auto parent2 = newPopulation[i + 1];

            auto decendants = crossingMethod_(parent1, parent2);

            newPopulation[i] = decendants.first;
            newPopulation[i + 1] = decendants.second;
        }

        // mutacja tylko skrzy¿owanych chromosomów
        for (int i = 0; i < mutationChromosomesCount; i++)
        {
            int chromosome = gen() % crossingChromosomesCount;
            mutationMethod_(newPopulation[chromosome]);
        }

        population_ = newPopulation;
        // Aktualizacja najlepszego rozwi¹zania
        for (const auto& chromosome : population_)
        {
            int cost = calculateCost(chromosome);
            if (cost < bestChromosomeWeight_)
            {
                auto exactTime = std::chrono::steady_clock::now();
                timeBestChromosomeFoundAt_ = std::chrono::duration_cast<std::chrono::nanoseconds>(exactTime - startTime).count();
                timeBestChromosomeFoundAt_ /= 1000000; // przeliczenie na sekundy

                bestChromosome_ = chromosome;
                bestChromosomeWeight_ = cost;
            }
        }
        std::cout << bestChromosomeWeight_ << '\n';
    }
    
    return bestChromosomeWeight_;
}

void GAalgorithm::clear()
{
    bestChromosome_.clear();
    bestChromosomeWeight_ = INT_MAX;
    population_.clear();
    timeBestChromosomeFoundAt_ = 0;
}

void GAalgorithm::initializePopulation(int numberOfCities)
{
    population_.clear(); // wyczyszczenie dotychczasowej populacji

    for (int i = 0; i < populationSize_; i++)
    {
        population_.push_back(generateRandomPath(numberOfCities));
    }
}

std::pair<std::vector<int>, std::vector<int>> GAalgorithm::orderCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2)
{
    int size = parent1.size();

    std::vector<int> descendand1(size, -1);
    std::vector<int> descendand2(size, -1);

    std::random_device rd;
    std::mt19937 g(rd());

    int start = g() % (size - 1);
    int end = (g() % (size - 1)) + 1; // Zapewnienie, ¿e segment bêdzie mieæ co najmniej d³ugoœæ 1

    if (start > end) {          // zapewnienie ¿e pocz¹tek jest mniejszy od koñca
        std::swap(start, end);
    }

    // Skopiowanie segmentu z pierwszego rodzica do pierwszego potomka
    for (int i = start; i <= end; i++) {
        descendand1[i] = parent1[i];
    }

    // Skopiowanie segmentu z drugiego rodzica do drugiego potomka
    for (int i = start; i <= end; i++) {
        descendand2[i] = parent2[i];
    }

    // Uzupe³nienie potomka1 elementami z rodzica2
    int current = (end + 1) % size;
    for (int i = 0; i < size; i++) {
        int gene = parent2[(end + 1 + i) % size];
        if (std::find(descendand1.begin(), descendand1.end(), gene) == descendand1.end()) {
            descendand1[current] = gene;
            current = (current + 1) % size;
        }
    }

    // Uzupe³nienie potomka2 elementami z rodzica1
    current = (end + 1) % size;
    for (int i = 0; i < size; i++) {
        int gene = parent1[(end + 1 + i) % size];
        if (std::find(descendand2.begin(), descendand2.end(), gene) == descendand2.end()) {
            descendand2[current] = gene;
            current = (current + 1) % size;
        }
    }

    return { descendand1, descendand2 };
}

std::pair<std::vector<int>, std::vector<int>> GAalgorithm::sequentialConstructiveCrossover(const std::vector<int>& parent1, const std::vector<int>& parent2) {
    std::vector<int> offspring1 = SCX(parent1, parent2);
    std::vector<int> offspring2 = SCX(parent2, parent1);

    return { offspring1, offspring2 };
}

bool GAalgorithm::isLegitimate(const std::vector<int>& descendant, int node) 
{
    return std::find(descendant.begin(), descendant.end(), node) == descendant.end();
}

int GAalgorithm::findFirstLegitimate(const std::vector<int>& descendant)
{
    for (int i = 0; i < matrix_->getSize(); i++) {
        if (std::find(descendant.begin(), descendant.end(), i) == descendant.end()) {
            return i;
        }
    }
    return -1;
}

int GAalgorithm::genNextNode(const std::vector<int>& parent, const std::vector<int>& descendant, int currentNode) 
{
    auto it = std::find(parent.begin(), parent.end(), currentNode);

    if (it != parent.end())  // SprawdŸ, czy currentNode zosta³ znaleziony w wektorze parent
    {
        for (auto nextNodeIt = it; nextNodeIt != parent.end(); ++nextNodeIt)
        {
            int nextNode = *nextNodeIt;
            if (isLegitimate(descendant, nextNode))
            {
                return nextNode;
            }
        }
    }

    return -1;
}

std::vector<int> GAalgorithm::SCX(const std::vector<int>& parent1, const std::vector<int>& parent2) 
{
    int size = parent1.size();
    std::vector<int> descendant(size, -1);

    int currentNode = parent1[0]; // wybierz 1 wierzcho³ek jako 1 gen potomka 
    descendant[0] = currentNode;

    for (int i = 1; i < size; i++) 
    {
        int nextNode1 = genNextNode(parent1, descendant, currentNode); // znajdz kolejny nieodwiedzony gen
        int nextNode2 = genNextNode(parent2, descendant, currentNode);

        int chosenNode;
        if (nextNode1 != -1 && nextNode2 != -1) 
        {
            // Spoœród 2 krawêdzi wybierz tê o najni¿szym koszcie
            int c1 = (*matrix_)[currentNode][nextNode1];
            int c2 = (*matrix_)[currentNode][nextNode2];

            chosenNode = (c1 < c2) ? nextNode1 : nextNode2;
        }
        else if (nextNode1 != -1 && nextNode2 == -1) 
        {
            int firstLegitimate = findFirstLegitimate(descendant);
            chosenNode = ((*matrix_)[currentNode][nextNode1] < (*matrix_)[currentNode][firstLegitimate]) ? nextNode1 : firstLegitimate;
        }
        else if (nextNode2 != -1 && nextNode1 == -1)
        {
            int firstLegitimate = findFirstLegitimate(descendant);
            chosenNode = ((*matrix_)[currentNode][nextNode2] < (*matrix_)[currentNode][firstLegitimate]) ? nextNode2 : firstLegitimate;
        }
        else 
        {
            chosenNode = findFirstLegitimate(descendant); // jeœli ¿aden z rodziców nie ma nieodwiedzonego genu, znajdŸ brakuj¹cy
        }

        descendant[i] = chosenNode;
        currentNode = chosenNode;
    }
    return descendant;
}

void GAalgorithm::insertionMutation(std::vector<int>& chromosome) {
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int x = 0; x < 10; x++)
    {
        int i = gen() % chromosome.size();
        int j = gen() % chromosome.size();
        while (i == j)
        {
            j = rand() % chromosome.size();
        }

        int valueToInsert = chromosome[i];

        chromosome.erase(chromosome.begin() + i);
        chromosome.insert(chromosome.begin() + j, valueToInsert);
    }
}

std::vector<int> GAalgorithm::tournamentSelection(const std::vector<std::vector<int>>& population)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<int> selectedIndices;
    selectedIndices.reserve(tournamentCount_);

    // Losowanie bez zwracania
    while (selectedIndices.size() < tournamentCount_)
    {
        int idx = gen() % populationSize_;
        // sprawdzenie czy dany osobnik ju¿ bra³ udzia³ w turnieju
        if (std::find(selectedIndices.begin(), selectedIndices.end(), idx) == selectedIndices.end())
        {
            selectedIndices.push_back(idx);
        }
    }

    // przypisanie najlepszego osobnika jako 0 element
    int bestIdx = selectedIndices[0];
    int bestCost = calculateCost(population_[bestIdx]);

    // znalezienie najlepszego osobnika ze wszystkich bior¹cych udzia³ w turnieju
    for (int i = 1; i < tournamentCount_; i++) {
        int idx = selectedIndices[i];
        int cost = calculateCost(population_[idx]);

        if (cost < bestCost) {
            bestCost = cost;
            bestIdx = idx;
        }
    }

    return population[bestIdx];
}

std::vector<int> GAalgorithm::getBestChromosome_()
{
    return bestChromosome_;
}

int GAalgorithm::getBestChromosomeWeight_() const
{
    return bestChromosomeWeight_;
}

long long GAalgorithm::getTimeBestChromosomeFoundAt() const
{
    return timeBestChromosomeFoundAt_;
}

int GAalgorithm::getTournamentCount() const
{
    return tournamentCount_;
}

std::vector<int> GAalgorithm::getRandomPath(int size)
{
    return generateRandomPath(size);
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

int GAalgorithm::calculateCost(const std::vector<int>& solution)
{
    int cost = 0;
    for (size_t i = 0; i < solution.size() - 1; ++i)
    {
        cost += (*matrix_)[solution[i]][solution[i + 1]];
    }
    cost += (*matrix_)[solution.back()][solution.front()]; // Powrót do pocz¹tkowego wierzcho³ka
    return cost;
}

void GAalgorithm::setMatrix(std::shared_ptr<Matrix>& matrix)
{
    matrix_ = matrix;
}

void GAalgorithm::setTimeLimit(int timeLimit)
{
    timeLimit_ = timeLimit;
}

void GAalgorithm::setTournamentCount(int tournamentCount)
{
    tournamentCount_ = tournamentCount;
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
        crossingMethod_ = std::bind(&GAalgorithm::orderCrossover, this, std::placeholders::_1, std::placeholders::_2);
    }
    else
    {
        crossingMethod_ = std::bind(&GAalgorithm::sequentialConstructiveCrossover, this, std::placeholders::_1, std::placeholders::_2);
    }
}
