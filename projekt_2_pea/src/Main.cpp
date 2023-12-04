#include <iostream>
#include <memory>

#include "Matrix.hpp"
#include "SWalgorithm.hpp"
#include "DataManager.hpp"


void showMenu()
{
	std::cout << "---Symulowane Wyzarzanie dla TSP---\n";
	std::cout << "1. Wczytaj dane z pliku\n";
	std::cout << "2. Wprowadz kryterium stopu\n";
	std::cout << "3. Ustaw wspolczynnik zmiany\n";
	std::cout << "4. Uruchom algorytm\n";
	std::cout << "5. Zapisz sciezke do pliku txt\n";
	std::cout << "6. Wczytaj sciezke z pliku txt i oblicz droge\n";
	std::cout << "7. Wyjdz\n\n";
	std::cout << "Wybierz opcje: ";
}

int main()
{
	std::vector<std::shared_ptr<SWalgorithm>> swAlgorithms;
	for (int i = 0; i < 3; i++) {
		swAlgorithms.push_back(std::make_shared<SWalgorithm>()); // dodanie 3 instancji obiektu swAlgorithm do wektora
	}
	auto swAlgorithmReader = make_shared<SWalgorithm>();

	auto matrix = std::make_shared<Matrix>();

	std::vector<int> readCycle = {};	// odczytany z pliku cykl

	int timeLimit = 10;	// domyœlnie ustawione kryterium na 10s
	//double aRatio[3] = { 0.99982, 0.9998, 0.995 }; // a (optymalne, wieksze, mniejsze) ftv55.xml
	//double aRatio[3] = { 0.999, 0.995, 0.9998 };   // a (optymalne, wiêksze, mniejsze) rbg358.xml
	double aRatio[3] = { 0.999, 0.995, 0.99 }; // a (optymalne, wieksze, mniejsze) ftv170.xml

	int aChoice; // wybór zapisu cyklu dla wspó³czynnika

	std::vector<int> solution;

	auto matrix2 = make_shared<Matrix>();

	int opt;
	std::string fileName, saveFileName, readFileName;

	std::vector<int> cyclev1;
	int resultv1;

	// testy
	std::vector<std::string> fileNamesForTest = { "ftv55.xml", "ftv170.xml", "rbg358.xml" };
	std::vector<std::string> outputFileNamesForTest = { "ftv55.txt", "ftv170.txt", "rbg358.xml" };
	std::vector<int> timeLimitsForTest = { 120, 240, 360 };

	int numberOfTests = 10; // 10 testow
	
	do
	{
		showMenu();
		cin >> opt;
		switch (opt)
		{
		case 1:
			std::cout << "Podaj nazwe pliku: ";
			cin >> fileName;
			matrix->loadFromXmlFile(fileName);
			break;

		case 2:
			std::cout << "Podaj kryterium stopu: ";
			cin >> timeLimit;

			break;

		case 3:
			std::cout << "Podaj trzy wspolczynniki zmiany (a):\n";
			for (int i = 0; i < 3; i++)
			{
				std::cout << "Podaj " << i + 1 << " wspolczynnik zmiany(a) : ";
				cin >> aRatio[i];
			}

			break;

		case 4:

			for (int i = 0; i < 3; i++)
			{
				swAlgorithms[i]->clear();	// wyczyszczenie poprzednich danych

				std::cout << "a: " << aRatio[i] << std::endl;
				std::cout << "Wartosc cyklu: " << swAlgorithms[i]->run(matrix, timeLimit, aRatio[i]) << std::endl;
				std::cout << "Cykl: "; DataManager::displayVector(swAlgorithms[i]->getMinCycle()); cout << std::endl;
				std::cout << "exp(-1/Tk): " << exp(-1 / swAlgorithms[i]->getTemperature()) << std::endl;
				std::cout << "Tk: " << swAlgorithms[i]->getTemperature() << std::endl;
				std::cout << "Moment czasu kiedy znaleziono: " << swAlgorithms[i]->getTimeMinCycleFoundAt() << "ns" << std::endl << std::endl;
			}
			break;

		case 5:
			std::cout << "Podaj nr wspolczynika (a), ktorego cykl chcesz zapisac do pliku (1, 2, lub 3): ";
			std::cin >> aChoice;
			std::cout << "Podaj nazwe pliku, do ktorego chcesz zapisac: ";
			std::cin >> saveFileName;
			DataManager::saveFile(swAlgorithms[aChoice - 1]->getMinCycle(), saveFileName);
			break;

		case 6:
			std::cout << "Podaj nazwe pliku, ktory chcesz odczytac: ";
			std::cin >> readFileName;
			readCycle = DataManager::readFile(readFileName);
			std::cout << "Obliczona droga na podstawie tabeli kosztow: " << swAlgorithmReader->calculateCost(matrix, readCycle) << std::endl;
			std::cout << std::endl;
			break;

		case 7:
			return 0;
			break;

		case 8:
			cyclev1 = swAlgorithmReader->generateBFSSolution(matrix);
			DataManager::saveFile(cyclev1, "greedy.txt");
			resultv1 = swAlgorithmReader->calculateCost(matrix, cyclev1);
			std::cout << "Wartosc greedy: " << resultv1;
			break;

		case 9:

			for(int test = 0; test < numberOfTests; test++)
			{
				for (int i = 0; i < 1; i++)
				{
					matrix->loadFromXmlFile(fileNamesForTest[i]);
					for (int j = 0; j < 3; j++)
					{
						swAlgorithms[j]->clear();	// wyczyszczenie poprzednich danych
						swAlgorithms[j]->run(matrix, timeLimitsForTest[i], aRatio[j]);
						DataManager::saveFileTest(swAlgorithms[j]->getMinCycleWeight(),
							swAlgorithms[j]->getTimeMinCycleFoundAt(), aRatio[j], outputFileNamesForTest[i]);
					}
				}
			}
			
		default:
			std::cout << "Niepoprawny nr opcji!\n";
			break;
		}
	} while (opt != 7);
	
	
	return 0;
}