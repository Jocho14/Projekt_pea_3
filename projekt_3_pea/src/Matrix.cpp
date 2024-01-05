#include "Matrix.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <tinyxml2.h> // biblioteka do odczytywania poszczeg�lnych atrybut�w w pliku .xml

using namespace tinyxml2;
using namespace std;

Matrix::Matrix() 
	: size(0), matrix(nullptr) 
{
}

Matrix::Matrix(const int size_)
	: size(size_)
{
	matrix = new int* [size_];
	for (int i = 0; i < size_; i++)
	{
		matrix[i] = new int[size_];
	}

	// inicjalizacja macierzy warto�ci� pocz�tkow� -1
	for (int i = 0; i < size_; i++)
	{
		for (int j = 0; j < size; j++)
		{
			matrix[i][j] = -1;
		}
	}
}

Matrix::~Matrix()
{
	clear();
}

void Matrix::loadFromFile(const string& fileName_)
{
	ifstream userDataFile; 

	userDataFile.open(fileName_);

	if (!userDataFile) {
		cout << "Nie mozna otworzyc pliku: " << fileName_ << endl;
		return;  
	}

	int newSize;
	userDataFile >> newSize; // wczytanie wielko�ci macierzy z 1 linii z pliku

	int** newMatrix = new int*[newSize];

	// inicjalizacja macierzy danymi z pliku
	for (int i = 0; i < newSize; i++)
	{
		newMatrix[i] = new int[newSize];
		for (int j = 0; j < newSize; j++)
		{
			userDataFile >> newMatrix[i][j];
		}
	}

	// Czyszczenie dotychczasowej macierzy
	clear();

	// Przypisanie nowej macierzy do sk�adowej pola klasy wskazuj�cej na macierz
	matrix = newMatrix;
	size = newSize;

	userDataFile.close();
}

void Matrix::loadFromXmlFile(const string& fileName_)
{
	XMLDocument doc;
	XMLError eResult = doc.LoadFile(fileName_.c_str());
	if (eResult != XML_SUCCESS)		// sprawdzenie czy nast�pi� b��d przy wczytywaniu pliku
	{
		cout << "Error" << endl;
		return;
	}

	auto pGraph = doc.FirstChildElement("travellingSalesmanProblemInstance")->FirstChildElement("graph");	// przej�cie do atrybutu graph
	if (!pGraph) // sprawdzenie czy istnieje atrybut graph
	{
		cout << "Error" << endl;
		return;
	}

	int newSize = 0;
	for (auto pVertex = pGraph->FirstChildElement("vertex"); pVertex != nullptr; pVertex = pVertex->NextSiblingElement("vertex")) 
	{
		newSize++; // zwi�kszanie rozmiaru w zale�no�ci od ilo�ci wyst�pie� vertex
	}

	int** newMatrix = new int* [newSize];

	auto pVertex = pGraph->FirstChildElement("vertex");
	for (int i = 0; i < newSize; i++, pVertex = pVertex->NextSiblingElement("vertex")) 
	{
		newMatrix[i] = new int[newSize];
		auto pEdge = pVertex->FirstChildElement("edge");

		for (int j = 0; j < newSize && pEdge != nullptr; j++, pEdge = pEdge->NextSiblingElement("edge")) 
		{
			double cost;

			// odczytanie warto�ci atrybutu cost
			pEdge->QueryDoubleAttribute("cost", &cost);

			// konwersja na int
			newMatrix[i][j] = static_cast<int>(cost);
		}
	}

	// Czyszczenie dotychczasowej macierzy
	clear();

	// Przypisanie nowej macierzy
	matrix = newMatrix;
	size = newSize;
}

void Matrix::generateRandomData(const int size_, const int minValue, const int maxValue)
{
	int** newMatrix = new int* [size_];

	for (int i = 0; i < size_; i++)
	{
		newMatrix[i] = new int[size_];
		for (int j = 0; j < size_; j++)
		{
			if (i == j)
			{
				newMatrix[i][j] = -1; // Ustawienie warto�ci elementu na -1 w przypdaku, gdy jest to element le��cy na przek�tnej macierzy
			}
			else
			{
				newMatrix[i][j] = rand() % (maxValue - minValue + 1) + minValue; //przypisanie losowej warto�ci do nowego elementu macierzy
			}
		}
	}

	// czyszczenie dotychczasowej macierzy
	clear();

	// Pzrypisanie nowej macierzy do sk�adowej pola klasy wskazuj�cej na macierz
	matrix = newMatrix;
	size = size_;
}

void Matrix::clear()
{
	if (matrix == nullptr)
	{
		return;
	}

	for (int i = 0; i < size; i++) {
		delete[] matrix[i];
	}

	delete[] matrix;
	matrix = nullptr;
	size = 0;
}

int* Matrix::operator[](int index)
{
	return matrix[index];
}

int Matrix::getSize()
{
	return size;
}

void Matrix::display()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << matrix[i][j] << ' ';
		}
		cout << endl;
	}
}