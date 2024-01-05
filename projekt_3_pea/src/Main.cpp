#include <iostream>
#include <memory>

#include "Matrix.hpp"
#include "GAalgorithm.hpp"
#include "DataManager.hpp"


void showMenu()
{
	std::cout << "---Algorytm genetyczny dla TSP---\n\n";
	std::cout << "1. Wczytaj dane z pliku\n";
	std::cout << "2. Wprowadz kryterium stopu\n";
	std::cout << "3. Ustaw wielkosc populacji poczatkowej\n";
	std::cout << "   ------------------------------------\n";
	std::cout << "4. Ustaw wspolczynnik krzyzowania\n";
	std::cout << "5. Ustaw wspolczynnik mutacji\n";
	std::cout << "   ------------------------------------\n";
	std::cout << "6. Wybierz metode krzyzowania\n";
	std::cout << "7. Wybierz metode mutacji\n";
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
	// obiekt wykorzystany tylko do policzenia œcie¿ki
	auto gaAlgorithm = make_shared<GAalgorithm>();

	// macierz s¹siedztw miast
	auto matrix = std::make_shared<Matrix>();

	// pliki (wczytaj xml, zapisz œcie¿ke txt, odczytaj œcie¿ke z txt)
	std::string xmlFileName, saveFileName, readFileName;

	// TESTOWANIE
	std::vector<int> parent1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	std::vector<int> parent2 = { 9, 3, 7, 8, 2, 6, 5, 1, 4 };

	std::pair<std::vector<int>, std::vector<int>> descendands;
	
		// Wynik order crossover (poprawny)
		// poczatek (index): 3
		// koniec (index): 6
		// Potomek 1 : 3 -> 8 -> 2 -> 4 -> 5 -> 6 -> 7 -> 1 -> 9
		// Potomek 2 : 3 -> 4 -> 7 -> 8 -> 2 -> 6 -> 5 -> 9 -> 1
		
		// Wynik
		// Potomek 1: 
		// Potomek 2:
	//

	// testy
	int numberOfTests = 5; 
	std::vector<int> timeLimitsForTest				 = { 120, 240, 360 };
	std::vector<std::string> inputFileTest			 = { "ftv47.xml", "ftv170.xml", "rbg403.xml" };
	std::vector<std::string> outputFileMinWeightTest = { "ftv47.txt", "ftv170.txt", "rbg403.txt" };
	std::vector<std::string> outputFileMinCycleTest  = { "ftv47Cycle.txt", "ftv170Cycle.txt", "rbg403Cycle.txt" };

	//////////////////////////////////////////////////
	// dotyczasowe wnioski i parametry				//
	// populacja:					2000            //
	// liczba osobnikow w turnieju: 4				//
	// wspolczynnik krzyzowania:	0.8				//
	// Wyniki										//
	// ftv170 - 4349, 240s  (2000 populacja)		//
	// rbg430 - 3360, 360s  (1000 populacja)		//
	// ftv47  - 1846, 120s  (2000 populacja)		//
	//////////////////////////////////////////////////

	// domyœlnie ustawione kryterium na 60s
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
			std::cout << "2. Modified order crossover (MOX)\n";

			do 
			{
				std::cout << "Wybierz metode: ";
				cin >> crossingOpt;
			} while (crossingOpt != 1 && crossingOpt != 2);

			gaAlgorithm->setCrossingMethod(crossingOpt);

			break;

		case 7:
			
			break;

		case 8:
			cout << "algorytm wtrakcie wykonywania...\n";
			cout << "Najkroszta sciezka: " << gaAlgorithm->run(matrix) << '\n';

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
			
		default:
			std::cout << "Niepoprawny nr opcji!\n";
			break;
		}
	} while (opt != 9);
	
	
	return 0;
}