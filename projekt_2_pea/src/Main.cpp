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
	auto swAlgorithm = std::make_shared<SWalgorithm>() ;
	auto matrix = std::make_shared<Matrix>();
	std::vector<int> readCycle = {};

	int timeLimit = 10;	// domyœlnie ustawione kryterium na 10s
	double a = 0.9999;		// domyœlnie ustawiony wspó³czynnik na 0.9999

	int opt;
	std::string fileName;

	
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
			std::cout << "Podaj wspolczynnik zmiany (a): ";
			cin >> a;
			break;

		case 4:
			std::cout << "Wartosc cyklu: " << swAlgorithm->run(matrix, timeLimit, a) << std::endl;
			std::cout << "Cykl: "; DataManager::displayVector(swAlgorithm->getMinCycle()); cout << std::endl;
			std::cout << "exp(-1/Tk): " << swAlgorithm->getProbability() << std::endl;
			std::cout << "Tk: " << swAlgorithm->getTemperature() << std::endl;
			break;

		case 5:
			DataManager::saveFile(swAlgorithm->getMinCycle());
			std::cout << "Zapisano do pliku\n";
			break;

		case 6:
			readCycle = DataManager::readFile("wynik.txt");
			std::cout << "Obliczona droga na podstawie tabeli kosztow: " << swAlgorithm->calculateCost(matrix, readCycle) << std::endl;
			break;

		case 7:
			return 0;
			break;

		default:
			std::cout << "Niepoprawny nr opcji!\n";
			break;
		}
	} while (opt != 7);
	
	
	return 0;
}