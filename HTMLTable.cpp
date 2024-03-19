#include "HTMLTable.h"

namespace HTMLTableTypes {
	const char* TABLE = "table";
	const char* FIELD = "th";
	const char* ROW = "tr";
	const char* CELL = "td";

	const char* END_TABLE = "/table";
	const char* END_FIELD = "/th";
	const char* END_ROW = "/tr";
	const char* END_CELL = "/td";
}

void HTMLTable::print() const
{
	int* maxColLen = new int [maxCols] {0};
	calculateMaxColLen(maxColLen);

	for (int i = 0; i < rows; i++) {
		table[i].print(maxColLen, maxCols);
		std::cout << std::endl;
	}

	delete[] maxColLen;
}

void HTMLTable::readTable(const char* filename)
{
	std::ifstream ifs(filename, std::ios::in);

	if (!ifs.is_open()) {
		std::cerr << "Invalid filename.";
		return;
	}

	readTableFromFile(ifs);
}

void HTMLTable::calculateMaxColLen(int*& arr) const
{
	for (int i = 0 ; i < maxCols ; i++) {
		for (int j = 0; j < rows; j++) {
			if (table[j].getCellLen(i) > arr[i]) {
				arr[i] = table[j].getCellLen(i);
			}
		}
	}

	/*for (int i = 0; i < 4; i++) {
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;*/
}

void HTMLTable::readTableFromFile(std::ifstream& ifs)
{
	char buff[MAX_LENGTH];
	if (!validOpenindChar(ifs))
		std::cerr << "Invalid file format.";

	ifs.getline(buff, MAX_LENGTH, '>');
	if (strcmp(buff,HTMLTableTypes::TABLE) == 0) {
		createTable(ifs);
	}
	//what happens if it is not a table
}

void HTMLTable::createTable(std::ifstream& ifs)
{
	int rowIndex = 0;
	int maxColsLen = 0;

	char buff[MAX_LENGTH] = "";

	while (strcmp(buff, HTMLTableTypes::END_TABLE) != 0) {

		if (!validOpenindChar(ifs)) {
			std::cerr << "Invalid file format.";
			return;
		}

		ifs.getline(buff, MAX_LENGTH, '>');

		if (ifs.eof()) {//if the getline function raised the eof bit the next lines are useless
			std::cerr << "Invalid file format.";
			return;
		}

		if (strcmp(buff, HTMLTableTypes::ROW) == 0) {
			createRow(ifs,rowIndex);

			if (maxColsLen < table[rowIndex].cellCount)
				maxColsLen = table[rowIndex].cellCount;

			rowIndex++;
		}
		//eof??
	}
	rows = rowIndex;
	maxCols = maxColsLen;
}

void HTMLTable::createRow(std::ifstream& ifs,int rowIndex)
{
	int cellIndex = 0;

	char buff[MAX_LENGTH] = "";

	while (strcmp(buff, HTMLTableTypes::END_ROW) != 0) {

		if (!validOpenindChar(ifs)) {
			std::cerr << "Invalid file format.";
			return;
		}

		ifs.getline(buff, MAX_LENGTH, '>');

		if (ifs.eof()) {//if the getline function raised the eof bit the next lines are useless
			std::cerr << "Invalid file format.";
			return;
		}

		if (strcmp(buff, HTMLTableTypes::FIELD) == 0) {
			createCell(ifs, rowIndex, cellIndex , 'h');
			cellIndex++;
		}
		if (strcmp(buff, HTMLTableTypes::CELL) == 0) {
			createCell(ifs, rowIndex, cellIndex, 'c');
			cellIndex++;
		}
	}
	table[rowIndex].cellCount = cellIndex;
}

void HTMLTable::createCell(std::ifstream& ifs,int rowIndex, int cellIndex, char type)
{
	char buff[MAX_LENGTH];
	ifs.getline(buff, MAX_LENGTH, '<');

	if (ifs.eof()) {
		std::cerr << "Invalid file format.";
		return;
	}

	char closingAttrb[MAX_LENGTH];
	ifs.getline(closingAttrb, MAX_LENGTH, '>');

	if (type == 'f') {
		if (strcmp(closingAttrb, HTMLTableTypes::END_FIELD) != 0) {
			std::cerr << "Invalid file format.";
			return;
		}
	}

	if (type == 'c') {
		if (strcmp(closingAttrb, HTMLTableTypes::END_CELL) != 0) {
			std::cerr << "Invalid file format.";
			return;
		}
	}
	table[rowIndex].addCell(buff, type, cellIndex);
}

bool HTMLTable::validOpenindChar(std::ifstream& ifs)
{
	while (true) {
		char temp = ifs.get();

		if (ifs.eof()) {
			return false;
		}

		if (temp != '\n' && temp != '\t' && temp != ' ') {
			ifs.seekg(-1, std::ios::cur);
			break;
		}
	}

	return ifs.get() == '<';
}

void HTMLTable::Row::addCell(const char* data, char type, int cellIndex)
{
	strcpy(row[cellIndex].fieldText, data);
	row[cellIndex].type = type;
}

int HTMLTable::Row::getCellLen(int id) const
{
	if(row[id].type == 'c') return strlen(row[id].fieldText);

	else return strlen(row[id].fieldText) + STARS_LEN;
}

void HTMLTable::Row::align(int n) const
{
	for (int i = 0; i < n; i++) {
		std::cout << " ";
	}
}

void HTMLTable::Row::print(const int* colsLen, int fullRowLen) const
{
	for (int i = 0; i < fullRowLen; i++) {
		if (i == 0)
			std::cout << "| ";

		if (row[i].type == 'h') {
			std::cout << "*" << (row[i].fieldText) << "*";
			
			align(colsLen[i] - (strlen(row[i].fieldText) + STARS_LEN));
		}
		else {
			std::cout << (row[i].fieldText);

			align(colsLen[i] - strlen(row[i].fieldText));
		}
		std::cout << " | ";
	}
}

