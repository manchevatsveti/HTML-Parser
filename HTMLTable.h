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
	int const MAX_LENGTH = 200;
	const int STARS_LEN = 2;
	const int MAX_ASCIICODE_LEN = 3;
	const char SEP = ',';
	const char ENTITY_BEG = '&';
	const char ENTITY_BEG_NUM = '#';//in entities the numbers start with '#'
}

namespace HelperFunctions {
	const char* interpretText(const char* str);//not object related
	char entityConvert(std::stringstream& ss);//not object related
	bool isDigit(char ch);//not object related

	bool validOpeningChar(std::ifstream& ifs);
}

enum class CellType {
	header,
	cell
};

class HTMLTable
{
private:
	struct Field {
		char fieldText[MAX_LENGTH_FIELD];
		CellType type = CellType::cell; 

		void writeFieldToFile(std::ofstream& ofs);

		bool createCell(std::ifstream& ifs, CellType type);
		void changeField(const Field& other);
	};

	struct Row {
		Field row[MAX_LENGTH_ROW] = { "" };
		int cellCount = 0;

		bool createRow(std::ifstream& ifs);

		void addCell(const char* data, CellType type, int cellIndex);
		int getCellLen(int id) const;
		void print(const int* colsLen, int fullRowLen) const;
		void align(int n) const;

		void changeText(int id,const char* newValue); 
		const Field& getField(int id) const;
		void writeRowToFile(std::ofstream& ofs, int fullRowLen);
		void changeRow(const Row& newRow);
		void clearRow(int maxCols);
	};

	Row table[MAX_LENGTH_TABLE];
	int rows;
	int maxCols;
	
	bool readTableFromFile(std::ifstream& ifs);

	bool createTable(std::ifstream& ifs);

	void shiftRowsBack(int id);
	void shiftRowsFront(int id);

	void writeTableToFile(std::ofstream& ofs);

	void calculateMaxColLen(int*& arr) const;
	void calculateColumns();

public:
	int getRows() const;
	int getMaxCols() const;

	bool readTable(const char* filename);

	void editRow(int rowId, int colId, const char* newValue);
	void add(int rowId, const char* newValue);//let's assume the ne column values are separeted by ','
	void remove(int rowId);
	void print() const;

	void writeToFile(const char* filename);
};

