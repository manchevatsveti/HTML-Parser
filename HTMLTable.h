#include <fstream>
#include <iostream>
#include <iomanip>
#pragma warning (disable: 4996)
#pragma once

namespace {
	int const MAX_LENGTH_FIELD = 50;
	int const MAX_LENGTH_ROW = 30;
	int const MAX_LENGTH_TABLE = 300;
	int const MAX_LENGTH = 100;
	const int STARS_LEN = 2;
}

class HTMLTable
{
	struct Field {
		char fieldText[MAX_LENGTH_FIELD];
		char type; // h - header, c - cell
	};

	struct Row {
		Field row[MAX_LENGTH_ROW] = { "" };
		int cellCount = 0;// ?

		void addCell(const char* data, char type, int cellIndex);
		int getCellLen(int id) const;
		void print(const int* colsLen, int fullRowLen) const;
		void align(int n) const;
	};
private:
	Row table[MAX_LENGTH_TABLE];
	int rows;
	int maxCols;
	
	void readTableFromFile(std::ifstream& ifs);
	void createTable(std::ifstream& ifs);
	void createRow(std::ifstream& ifs, int rowIndex);
	void createCell(std::ifstream& ifs, int rowIndex, int cellIndex, char type);
	bool validOpenindChar(std::ifstream& ifs);

	void calculateMaxColLen(int*& arr) const;

public:
	void readTable(const char* filename);
	void print() const;
};

