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

void showPath(std::vector<int> solution)
{
	for (const auto& node : solution)
	{
		cout << node << " -> ";
	}
}

int main()
{
	const int NUMBEROFALGORITHMS = 3;
	std::vector<std::shared_ptr<SWalgorithm>> swAlgorithms;
	for (int i = 0; i < NUMBEROFALGORITHMS; i++) {
		swAlgorithms.push_back(std::make_shared<SWalgorithm>()); // dodanie 3 instancji obiektu swAlgorithm do wektora
	}

	// obiekt wykorzystany tylko do policzenia œcie¿ki
	auto swAlgorithmReader = make_shared<SWalgorithm>();

	// macierz s¹siedztw miast
	auto matrix = std::make_shared<Matrix>();

	// domyœlnie ustawione kryterium na 60s
	int timeLimit = 60;	

	//double coolingRatio[numberOfAlgorithms] = { 0.999, 0.9997, 0.99965, 0.995, 0.99 };
	double coolingRatio[NUMBEROFALGORITHMS] = { 0.999, 0.9995, 0.99 };
	int coolingChoice; // wybór zapisu cyklu dla wspó³czynnika

	// pliki (wczytaj xml, zapisz œcie¿ke txt, odczytaj œcie¿ke z txt)
	std::string xmlFileName, saveFileName, readFileName;
	std::vector<int> readCycle;	// odczytany z pliku cykl

	// metoda zach³anna
	std::vector<int> greedyCycle;
	int greedyWeight;

	// testy
	int numberOfTests = 5; 
	std::vector<int> timeLimitsForTest				 = { 120, 240, 360 };
	std::vector<std::string> inputFileTest			 = { "ftv55.xml", "ftv170.xml", "rbg358.xml" };
	std::vector<std::string> outputFileMinWeightTest = { "ftv55.txt", "ftv170.txt", "rbg358.txt" };
	std::vector<std::string> outputFileMinCycleTest  = { "ftv55Cycle.txt", "ftv170Cycle.txt", "rbg358Cycle.txt" };
	
	int opt;
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
				cin >> coolingRatio[i];
			}
			break;

		case 4:
			for (int i = 0; i < NUMBEROFALGORITHMS; i++)
			{
				std::cout << "a: "			   << coolingRatio[i] << std::endl;
				std::cout << "Wartosc cyklu: " << swAlgorithms[i]->run(matrix, timeLimit, coolingRatio[i]) << std::endl;
				std::cout << "Cykl: "; DataManager::displayVector(swAlgorithms[i]->getMinCycle());    cout << std::endl;
				std::cout << "exp(-1/Tk): "    << exp(-1 / swAlgorithms[i]->getTemperature())              << std::endl;
				std::cout << "Tk: "            << swAlgorithms[i]->getTemperature()                        << std::endl;
				std::cout << "Moment czasu: "  << swAlgorithms[i]->getTimeMinCycleFoundAt() << "ns"        << std::endl << std::endl;
			}
			break;

		case 5:
			std::cout << "Podaj nr wspolczynika (a), ktorego cykl chcesz zapisac do pliku (1, 2, lub 3): ";
			std::cin >> coolingChoice;
			if (coolingChoice < 1 && coolingChoice > NUMBEROFALGORITHMS)
			{
				break;
			}

			std::cout << "Podaj nazwe pliku, do ktorego chcesz zapisac: ";
			std::cin >> saveFileName;

			DataManager::saveFile(swAlgorithms[coolingChoice - 1]->getMinCycle(), saveFileName);
			break;

		case 6:
			std::cout << "Podaj nazwe pliku, ktory chcesz odczytac: ";
			std::cin >> readFileName;
			readCycle = DataManager::readFile(readFileName);
			std::cout << "Obliczona droga na podstawie macierzy: " << swAlgorithmReader->calculateCost(matrix, readCycle) << std::endl;
			std::cout << std::endl;
			break;

		case 7:
			return 0;
			break;

		case 8:
			greedyCycle = swAlgorithmReader->generateBFSSolution(matrix);
			DataManager::saveFile(greedyCycle, "greedy.txt");

			greedyWeight = swAlgorithmReader->calculateCost(matrix, greedyCycle);
			std::cout << "Wartosc greedy: " << greedyWeight;
			break;

		case 9:
			std::cout << "W trakcie testowania...\n";
			for(int test = 0; test < numberOfTests; test++)
			{
				for (int i = 0; i < 3; i++)
				{
					matrix->loadFromXmlFile(inputFileTest[i]);
					for (int j = 0; j < NUMBEROFALGORITHMS; j++)
					{
						swAlgorithms[j]->run(matrix, timeLimitsForTest[i], coolingRatio[j]);

						DataManager::saveFileTest(swAlgorithms[j]->getMinCycleWeight(),
							swAlgorithms[j]->getTimeMinCycleFoundAt(), coolingRatio[j], swAlgorithms[j]->getTemperature(), outputFileMinWeightTest[i]);

						DataManager::saveSeparators(outputFileMinCycleTest[i]); // dodanie separatora
						DataManager::saveFile(swAlgorithms[j]->getMinCycle(), outputFileMinCycleTest[i]);
						DataManager::saveSeparators(outputFileMinCycleTest[i]); // dodanie separatora
					}
				}
			}
			break;
			
		default:
			std::cout << "Niepoprawny nr opcji!\n";
			break;
		}
	} while (opt != 7);
	
	
	return 0;
}