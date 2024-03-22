#include "HTMLTable.h"

namespace HTMLTableTypes {
	const char* TABLE = "table";
	const char* HEADER = "th";
	const char* ROW = "tr";
	const char* CELL = "td";

	const char* END_TABLE = "/table";
	const char* END_HEADER = "/th";
	const char* END_ROW = "/tr";
	const char* END_CELL = "/td";
}

void HTMLTable::editRow(int rowId, int colId, const char* newValue)
{
	if (rowId < 1 || rowId > rows || colId < 1 || colId > maxCols || !newValue) {
		std::cerr << "Invalid cell parameters.";
	}
	rowId--;
	colId--;

	table[rowId].changeText(colId, HelperFunctions::interpretText(newValue));//the columns and rows start from 1

	if (table[rowId].cellCount <= colId) {
		table[rowId].cellCount = colId;
	}
}

void HTMLTable::add(int rowId, const char* newValue)
{
	if (rowId < 1 || rowId > rows + 1) {//beacuse the columns and row start from 1
		std::cerr << "Invalid index to add a row to." <<std::endl;
		return;
	}

	if (!newValue) {
		std::cerr << "Invalid new value."<<std::endl;
		return;
	}
	
	rowId--;//make the index start from 0

	if(rowId != rows)//if we add at the very end there is no need for shifting
		shiftRowsBack(rowId);

	table[rowId].clearRow(maxCols);

	std::stringstream ss(newValue);

	int cellId = 0;

	while (!ss.eof()) {

		if (cellId >= MAX_LENGTH_ROW) {
			break;
		}

		char buff[MAX_LENGTH];
		ss.getline(buff, MAX_LENGTH, SEP);

		table[rowId].addCell(HelperFunctions::interpretText(buff), CellType::cell, cellId);
		cellId++;
	}

	table[rowId].cellCount = cellId;

	rows++;
	if (table[rowId].cellCount > maxCols) maxCols = table[rowId].cellCount;
}

void HTMLTable::remove(int rowId)
{
	if (rowId < 1 || rowId > rows) {//beacuse the columns and row start from 1
		std::cerr << "Invalid index to remove a row from." << std::endl;
		std::cout << std::endl;
		return;
	}
	rowId--;

	shiftRowsFront(rowId);
	rows--;

	calculateColumns();
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

int HTMLTable::getRows() const
{
	return rows;
}

int HTMLTable::getMaxCols() const
{
	return maxCols;
}

bool HTMLTable::readTable(const char* filename)
{
	std::ifstream ifs(filename, std::ios::in);

	if (!ifs.is_open()) {
		std::cerr << "Invalid filename.";
		return false;
	}

	return readTableFromFile(ifs);
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
}

void HTMLTable::calculateColumns()
{
	int maxLen = 0;

	for (int i = 0; i < rows; i++) {
		if (table[i].cellCount > maxLen) {
			maxLen = table[i].cellCount;
		}
	}

	if (maxLen != maxCols) {
		maxCols = maxLen;
	}
}

bool HTMLTable::readTableFromFile(std::ifstream& ifs)
{
	char buff[MAX_LENGTH];
	if (!HelperFunctions::validOpeningChar(ifs)) {
		std::cerr << "Invalid file format.";
		return false;
	}

	ifs.getline(buff, MAX_LENGTH, '>');
	if (strcmp(buff,HTMLTableTypes::TABLE) == 0) {
		return createTable(ifs);
	}
	return false;
}

bool HTMLTable::createTable(std::ifstream& ifs)
{
	int rowIndex = 0;
	int maxColsLen = 0;

	char buff[MAX_LENGTH] = "";

	while (strcmp(buff, HTMLTableTypes::END_TABLE) != 0) {

		if (!HelperFunctions::validOpeningChar(ifs)) {
			std::cerr << "Invalid file format.";
			return false;
		}

		ifs.getline(buff, MAX_LENGTH, '>');

		if (ifs.eof()) {//if the getline function raised the eof bit the next lines are useless
			std::cerr << "Invalid file format.";
			return false;
		}

		if (strcmp(buff, HTMLTableTypes::ROW) == 0) {
			if (!table[rowIndex].createRow(ifs)) {
				return false;
			}

			if (maxColsLen < table[rowIndex].cellCount)
				maxColsLen = table[rowIndex].cellCount;

			rowIndex++;
		}
		//eof??
	}
	rows = rowIndex;
	maxCols = maxColsLen;
	return true;
}

void HTMLTable::shiftRowsBack(int id)
{
	if (id < 0 || id > rows) {
		std::cerr << "Invalid input." <<std::endl;
		return;
	}
	for (int i = rows; i > id; i--) {
		table[i].clearRow(maxCols);
		table[i].changeRow(table[i-1]);
	}
}

void HTMLTable::shiftRowsFront(int id)
{
	for (int i = id; i < rows; i++) {
		table[i].clearRow(maxCols);
		table[i].changeRow(table[i + 1]);
	}
}

void HTMLTable::writeToFile(const char* filename)
{
	std::ofstream ofs(filename, std::ios::out);

	if (!ofs.is_open()) {
		std::cerr << "Unable to open file for writing!";
		return;
	}

	writeTableToFile(ofs);
}

void HTMLTable::writeTableToFile(std::ofstream& ofs)
{
	ofs << "<" << HTMLTableTypes::TABLE << ">" << std::endl;

	for (int i = 0; i < rows; i++) {
		table[i].writeRowToFile(ofs, maxCols);
	}

	ofs << "<" << HTMLTableTypes::END_TABLE << ">" << std::endl;
}

bool HTMLTable::Row::createRow(std::ifstream& ifs)
{
	int cellIndex = 0;

	char buff[MAX_LENGTH] = "";

	while (strcmp(buff, HTMLTableTypes::END_ROW) != 0) {

		if (!HelperFunctions::validOpeningChar(ifs)) {
			std::cerr << "Invalid file format.";
			return false;
		}

		ifs.getline(buff, MAX_LENGTH, '>');

		if (ifs.eof()) {//if the getline function raised the eof bit the next lines are useless
			std::cerr << "Invalid file format.";
			return false;
		}

		if (strcmp(buff, HTMLTableTypes::HEADER) == 0) {
			if (!row[cellIndex].createCell(ifs, CellType::header))
				return false;

			cellIndex++;
		}
		if (strcmp(buff, HTMLTableTypes::CELL) == 0) {
			if (!row[cellIndex].createCell(ifs, CellType::cell))
				return false;

			cellIndex++;
		}
	}
	cellCount = cellIndex;

	return true;
}

void HTMLTable::Row::addCell(const char* data, CellType type, int cellIndex)
{
	strcpy(row[cellIndex].fieldText, data);
	row[cellIndex].type = type;
}

int HTMLTable::Row::getCellLen(int id) const
{
	if(row[id].type == CellType::cell) return strlen(row[id].fieldText);

	else return strlen(row[id].fieldText) + STARS_LEN;
}

void HTMLTable::Row::align(int n) const
{
	for (int i = 0; i < n; i++) {
		std::cout << " ";
	}
}

void HTMLTable::Row::changeText(int id, const char* newValue)
{
	if (newValue) {
		strcpy(row[id].fieldText, newValue);
	}
	else {
		std::cerr << "Inavild new value.";
	}
}

const HTMLTable::Field& HTMLTable::Row::getField(int id) const
{
	return row[id];
}

void HTMLTable::Row::writeRowToFile(std::ofstream& ofs, int fullRowLen)
{
	ofs << "<" << HTMLTableTypes::ROW << ">" << std::endl;

	for (int i = 0; i < fullRowLen; i++) {
		row[i].writeFieldToFile(ofs);
		//ofs << row[i].fieldText;
	}

	ofs << "<" << HTMLTableTypes::END_ROW << ">" << std::endl;
}

void HTMLTable::Row::changeRow(const Row& newRow)
{
	int cols = newRow.cellCount >= cellCount ? newRow.cellCount : cellCount;

	for (int i = 0; i < cols; i++) {

		row[i].changeField(newRow.row[i]);
	}
	cellCount = newRow.cellCount;
}

void HTMLTable::Row::clearRow(int maxCols)
{
	for (int i = 0; i < maxCols; i++) {
		row[i].changeField({ "hello", CellType::cell});
	}
}

void HTMLTable::Row::print(const int* colsLen, int fullRowLen) const
{
	for (int i = 0; i < fullRowLen; i++) {
		if (i == 0)
			std::cout << "| ";

		if (row[i].type == CellType::header) {
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

void HTMLTable::Field::writeFieldToFile(std::ofstream& ofs)
{
	if (type == CellType::header) {
		ofs << "<" << HTMLTableTypes::HEADER << ">";
		ofs << fieldText;
		ofs << "<" << HTMLTableTypes::END_HEADER << ">" << std::endl;
	}
	else if (type == CellType::cell) {
		ofs << "<" << HTMLTableTypes::CELL << ">";
		ofs << fieldText;
		ofs << "<" << HTMLTableTypes::END_CELL << ">" << std::endl;
	}
}

bool HTMLTable::Field::createCell(std::ifstream& ifs, CellType newType)
{
	char buff[MAX_LENGTH];
	ifs.getline(buff, MAX_LENGTH, '<');

	if (ifs.eof()) {
		std::cerr << "Invalid file format.";
		return false;
	}

	strcpy(buff, HelperFunctions::interpretText(buff));//becasue the arrays are static we can rewrite the true text on the old buff

	char closingAttrb[MAX_LENGTH];
	ifs.getline(closingAttrb, MAX_LENGTH, '>');

	if (newType == CellType::header) {
		if (strcmp(closingAttrb, HTMLTableTypes::END_HEADER) != 0) {
			std::cerr << "Invalid file format.";
			return false;
		}
	}

	if (newType == CellType::cell) {
		if (strcmp(closingAttrb, HTMLTableTypes::END_CELL) != 0) {
			std::cerr << "Invalid file format.";
			return false;
		}
	}

	strcpy(fieldText, buff);
	type = newType;
	return true;
}

void HTMLTable::Field::changeField(const Field& other)
{
	strcpy(fieldText, other.fieldText);
	type = other.type;
}

const char* HelperFunctions::interpretText(const char* str)
{
	char buff[MAX_LENGTH_FIELD] = "";
	int id = 0;

	std::stringstream ss(str);

	while (true) {
		char temp = ss.get();

		if (ss.eof()) break;

		if (temp != ENTITY_BEG) {
			buff[id++] = temp;
		}
		else {
			buff[id++] = entityConvert(ss);
		}
	}
	return buff;
}

char HelperFunctions::entityConvert(std::stringstream& ss)
{
	int ASCIICode = 0;
	char temp = ss.get();

	if (temp != ENTITY_BEG_NUM) {//this means the '&' is not a beginning of entity, it is just a symbol
		ss.seekg(-1, std::ios::cur);//continue reading like normally;
		return ENTITY_BEG;
	}

	for (int i = 0; i < MAX_ASCIICODE_LEN; i++) {
		temp = ss.get();

		if (!isDigit(temp)) {
			ss.seekg(-1, std::ios::cur);
			break;
		}

		ASCIICode = ASCIICode * 10 + (temp - '0');
	}

	return (char)ASCIICode;
}

bool HelperFunctions::isDigit(char ch)
{
	return (ch >= '0' && ch <= '9');
}

bool HelperFunctions::validOpeningChar(std::ifstream& ifs)
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
