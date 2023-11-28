#pragma once

#include <memory>
#include <string>

using namespace std;

class Matrix
{
private:
	// Rozmiar macierzy
	int size;

	// Macierz
	int** matrix;

public:
	// Domyœlny konstruktor 
	Matrix();

	// Konstruktor macierzy o zadanej wielkoœci size
	Matrix(const int size_);

	// Destruktor macierzy
	~Matrix();

	// Metoda do wczytywania danych z pliku
	void loadFromFile(const string& fileName_);

	// Metoda do wczytywania danych z pliku XML
	void loadFromXmlFile(const string& fileName_);

	// Metoda generuj¹ca losowe dane macierzy
	void generateRandomData(const int size_, const int min, const int max);

	// Przeci¹¿enie operatora []
	int* operator[](int index);

	// Metoda do czyszczenia macierzy
	void clear();

	// Metoda do zwracania wielkoœci macierzy
	int getSize();

	// Metoda do wyœwietlania zawartoœci macierzy
	void display();
};