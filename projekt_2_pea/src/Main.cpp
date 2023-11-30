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
	for (int i = 0; i < 3; ++i) {
		swAlgorithms.push_back(std::make_shared<SWalgorithm>()); // dodanie 3 instancji obiektu swAlgorithm do wektora
	}

	auto swAlgorithmReader = make_shared<SWalgorithm>();

	auto matrix = std::make_shared<Matrix>();
	std::vector<int> readCycle = {};	// odczytany z pliku cykl

	int timeLimit = 10;	// domyœlnie ustawione kryterium na 10s
	double aRatio[3] = { 0.99999, 0.99998, 0.99997 }; // domyœlnie ustawione wspó³czynniki a

	int aChoice; // wybór zapisu cyklu dla wspó³czynnika

	int opt;
	std::string fileName, saveFileName, readFileName;

	
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
				std::cout << "Podaj " << i+1 << " wspolczynnik zmiany(a) : ";
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
				std::cout << "exp(-1/Tk): " << exp(-1/swAlgorithms[i]->getTemperature()) << std::endl;
				std::cout << "Tk: " << swAlgorithms[i]->getTemperature() << std::endl << std::endl;
			}
			break;

		case 5:
			std::cout << "Podaj nr wspolczynika (a), ktorego cykl chcesz zapisac do pliku (1, 2, lub 3): ";
			std::cin >> aChoice;
			std::cout << "Podaj nazwe pliku, do ktorego chcesz zapisac: ";
			std::cin >> saveFileName;
			DataManager::saveFile(swAlgorithms[aChoice-1]->getMinCycle(), saveFileName);
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

		default:
			std::cout << "Niepoprawny nr opcji!\n";
			break;
		}
	} while (opt != 7);
	
	
	return 0;
}