#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#pragma warning (disable: 4996)
#pragma once

namespace {
	int const MAX_LENGTH_FIELD = 50;
	int const MAX_LENGTH_ROW = 15;
	int const MAX_LENGTH_TABLE = 100;
	int const MAX_LENGTH = 1024;
	const int STARS_LEN = 2;
	const char SEP = ',';
}

class HTMLTable
{
private:
	struct Field {
		char fieldText[MAX_LENGTH_FIELD];
		char type = 'c'; // h - header, c - cell

		void writeFieldToFile(std::ofstream& ofs);
	};

	struct Row {
		Field row[MAX_LENGTH_ROW] = { "" };
		int cellCount = 0;

		void addCell(const char* data, char type, int cellIndex);
		int getCellLen(int id) const;
		void print(const int* colsLen, int fullRowLen) const;
		void align(int n) const;

		void changeText(int id,const char* newValue); 
		const Field& getField(int id) const;
		void changeCell(int id, const Field& newCell);
		void writeRowToFile(std::ofstream& ofs, int fullRowLen);
	};
	Row table[MAX_LENGTH_TABLE];
	int rows;
	int maxCols;
	
	bool readTableFromFile(std::ifstream& ifs);

	bool createTable(std::ifstream& ifs);
	bool createRow(std::ifstream& ifs, int rowIndex);
	bool createCell(std::ifstream& ifs, int rowIndex, int cellIndex, char type);
	bool validOpenindChar(std::ifstream& ifs);

	void shiftRowsBack(int id);
	void shiftRowsFront(int id);
	void changeRow(int id, const Row& newRow);

	void writeTableToFile(std::ofstream& ofs);

	void calculateMaxColLen(int*& arr) const;
	void calculateColumns();

public:
	bool readTable(const char* filename);

	void editRow(int rowId, int colId, const char* newValue);
	void add(int rowId, const char* newValue);//let's assume the ne column values are separeted by ','
	void remove(int rowId);
	void print() const;

	void writeToFile(const char* filename);
};

