#include <iostream>
#include <memory>

#include "Matrix.hpp"
#include "GAalgorithm.hpp"
#include "DataManager.hpp"


void showMenu()
{
	std::cout << "---Algorytm genetyczny dla ATSP---\n\n";
	std::cout << "1. Wczytaj dane z pliku\n";
	std::cout << "2. Wprowadz kryterium stopu\n";
	std::cout << "3. Ustaw wielkosc populacji poczatkowej\n";
	std::cout << "   ------------------------------------\n";
	std::cout << "4. Ustaw wspolczynnik krzyzowania\n";
	std::cout << "5. Ustaw wspolczynnik mutacji\n";
	std::cout << "   ------------------------------------\n";
	std::cout << "6. Wybierz metode krzyzowania\n";
	std::cout << "   ------------------------------------\n";
	std::cout << "8. URUCHOM ALGORYTM\n";
	std::cout << "   ------------------------------------\n";
	std::cout << "9. Wyjdz\n\n";
	std::cout << "Wybierz opcje: ";
}

void showPath(std::vector<int> solution)
{
	for (const auto& node : solution)
	{
		cout << node << " -> ";
	}
}

int main()
{
	auto gaAlgorithm = make_shared<GAalgorithm>();

	// macierz s¹siedztw miast
	auto matrix = std::make_shared<Matrix>();

	// plik (wczytaj xml)
	std::string xmlFileName;

	/******************************************************************************************************************/
	// TESTOWANIE
	std::vector<int> parent1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::vector<int> parent2 = { 9, 3, 7, 8, 2, 6, 5, 1, 4 };

	std::pair<std::vector<int>, std::vector<int>> descendands;

	const int NUMBEROFALGORITHMS = 3;
	std::vector<std::shared_ptr<GAalgorithm>> gaAlgorithms;
	for (int i = 0; i < NUMBEROFALGORITHMS; i++) {
		gaAlgorithms.push_back(std::make_shared<GAalgorithm>()); // dodanie 3 instancji obiektu swAlgorithm do wektora
	}

	int bestChromosome;
	int startingTournamentCount = 5;
	int numberOfTests = 10; 
	std::vector<int> timeLimitsForTest				 = { 120, 240, 360 };
	std::vector<std::string> inputFileTest			 = { "ftv47.xml", "ftv170.xml", "rbg403.xml" };
	std::vector<std::string> outputFileMinWeightTest = { "ftv47.txt", "ftv170.txt", "rbg403.txt" };
	std::vector<std::string> outputFileMinCycleTest  = { "ftv47Cycle.txt", "ftv170Cycle.txt", "rbg403Cycle.txt" };
	std::vector<int> testPopulations = { 2000, 2500, 3000, 3500, 4000, 4500, 5000 };

	std::vector<int> parent1SCX = { 0, 4, 6, 2, 5, 3, 1 };
	std::vector<int> parent2SCX = { 0, 5, 1, 3, 2, 4, 6 };
	std::vector<int> resultSCX;

	//////////////////////////////////////////////////////////////////////////////
	// dotyczasowe wnioski i parametry											//
	// populacja:					2000										//
	// wspolczynnik krzyzowania:	0.8											//
	// Wyniki stare							 								    //
	// ftv47  - 1789, 120s  (2000 populacja), lb osobników w turnieju: 3		//														
	// ftv170 - 4349, 240s  (2000 populacja), lb osobników w turnieju: 9		//
	// rbg430 - 3221, 360s  (2000 populacja), lb osobników w turnieju: 15		//
	// Wynik nowe																//
	// ftv47  - , 120s  (500 populacja), lb osobników w turnieju: 4				//
	// ftv170 - 3010, 240s  (2000 populacja), lb osobników w turnieju: 9	insercja	//
	// rbg430 - 2787, 360s  (500 populacja), lb osobników w turnieju: 15		//
	//////////////////////////////////////////////////////////////////////////////
	/******************************************************************************************************************/

	int timeLimit = 60;
	int populationSize;
	double crossingFactor;
	double mutationFactor;
	
	int opt, crossingOpt;
	do
	{
		showMenu();
		cin >> opt;
		switch (opt)
		{
		case 1:
			std::cout << "Podaj nazwe pliku: ";
			cin >> xmlFileName;
			matrix->loadFromXmlFile(xmlFileName);
			gaAlgorithm->setMatrix(matrix);
			//matrix->display();
			break;

		case 2:
			std::cout << "Podaj kryterium stopu: ";
			cin >> timeLimit;
			gaAlgorithm->setTimeLimit(timeLimit);
			break;

		case 3:
			std::cout << "Podaj wielkosc populacji poczatkowej: ";
			cin >> populationSize;
			gaAlgorithm->setPopulationSize(populationSize);
			break;

		case 4:
			std::cout << "Podaj wspolczynnik krzyzowania: ";
			cin >> crossingFactor;
			gaAlgorithm->setCrossingFactor(crossingFactor);
			break;

		case 5:
			std::cout << "Podaj wspolczynnik mutacji: ";
			cin >> mutationFactor;
			gaAlgorithm->setMutationFactor(mutationFactor);
			break;

		case 6:
			std::cout << "Metody krzyzowania: \n";
			std::cout << "1. Order crossover (OX) \n";
			std::cout << "2. Sequential Constructive Crossover (SCX)\n";

			do 
			{
				std::cout << "Wybierz metode: ";
				cin >> crossingOpt;
			} while (crossingOpt != 1 && crossingOpt != 2);

			gaAlgorithm->setCrossingMethod(crossingOpt);

			break;

		case 8:
			cout << "algorytm wtrakcie wykonywania...\n";
			cout << "Najkroszta sciezka: " << gaAlgorithm->run() << '\n';
			cout << "Czas znalezienia: " << gaAlgorithm->getTimeBestChromosomeFoundAt() << "ms\n";

			break;

		case 9:
			/*for (int i = 0; i < 10; i++)
{
	showPath(gaAlgorithm->getRandomPath(6));
	std::cout << "\n-------------\n";
}*/
			std::cout << "Potomek 1: ";
			descendands = gaAlgorithm->orderCrossover(parent1, parent2);
			showPath(descendands.first);

			std::cout << '\n';
			std::cout << "Potomek 2: ";
			showPath(descendands.second);
			break;

		case 10:
			// Testy
			for (int i = 0; i < numberOfTests; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					matrix->loadFromXmlFile(inputFileTest[j]);
					gaAlgorithms[j]->setTimeLimit(timeLimitsForTest[j]);
					bestChromosome = gaAlgorithms[j]->run();

					DataManager::saveFileTestForGA(bestChromosome, gaAlgorithms[j]->getTournamentCount(), 600, gaAlgorithm->getTimeBestChromosomeFoundAt(), outputFileMinWeightTest[j]);

					gaAlgorithms[j]->setTournamentCount(gaAlgorithms[j]->getTournamentCount() + 2);
				}
			}
			break;

		case 11:
			std::cout << "Podaj liczbe osobnikow do turnieju: ";
			std::cin >> startingTournamentCount;
			gaAlgorithm->setTournamentCount(startingTournamentCount);
			break;

		case 12:
			std::cout << "testuje...\n";
			matrix->loadFromXmlFile("ftv170.xml");
			gaAlgorithm->setMatrix(matrix);
			
			for (int i = 0; i < 21; i++)
			{
				gaAlgorithm->setPopulationSize(testPopulations[i % 3]);
				bestChromosome = gaAlgorithm->run();
				DataManager::saveFileTestForGA(bestChromosome, gaAlgorithms[i]->getTournamentCount(), testPopulations[i % 3], gaAlgorithm->getTimeBestChromosomeFoundAt(), "ftv170.txt");
			}

		case 13:
			matrix->loadFromFile("testMatrix.txt");
			matrix->display();
			gaAlgorithm->setMatrix(matrix);
			gaAlgorithm->run();
			/*resultSCX = gaAlgorithm->SCX(parent1SCX, parent2SCX);
			std::cout << '\n';
			std::cout << "======\n";
			DataManager::displayVector(resultSCX);
			std::cout << "======\n";*/
			
		default:
			std::cout << "Niepoprawny nr opcji!\n";
			break;
		}
	} while (opt != 9);
	
	
	return 0;
}