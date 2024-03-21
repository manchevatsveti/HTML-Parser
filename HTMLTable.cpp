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

void HTMLTable::editRow(int rowId, int colId, const char* newValue)
{
	if (rowId < 1 || rowId > rows || colId < 1 || colId > maxCols || !newValue) {
		std::cerr << "Invalid cell parameters.";
	}
	rowId--;
	colId--;

	table[rowId].changeText(colId, newValue);//the columns and rows start from 1

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
	
	rowId--;

	if(rowId != rows)//if we add at the very end there is no need for shifting
		shiftRowsBack(rowId);

	clearRow(rowId);

	std::stringstream ss(newValue);

	int cellId = 0;

	while (!ss.eof()) {

		if (cellId >= MAX_LENGTH_ROW) {
			break;
		}

		char buff[MAX_LENGTH];
		ss.getline(buff, MAX_LENGTH, SEP);

		table[rowId].addCell(buff, 'c', cellId);
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
	if (!validOpenindChar(ifs)) {
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

		if (!validOpenindChar(ifs)) {
			std::cerr << "Invalid file format.";
			return false;
		}

		ifs.getline(buff, MAX_LENGTH, '>');

		if (ifs.eof()) {//if the getline function raised the eof bit the next lines are useless
			std::cerr << "Invalid file format.";
			return false;
		}

		if (strcmp(buff, HTMLTableTypes::ROW) == 0) {
			if (!createRow(ifs, rowIndex)) {
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

bool HTMLTable::createRow(std::ifstream& ifs,int rowIndex)
{
	int cellIndex = 0;

	char buff[MAX_LENGTH] = "";

	while (strcmp(buff, HTMLTableTypes::END_ROW) != 0) {

		if (!validOpenindChar(ifs)) {
			std::cerr << "Invalid file format.";
			return false;
		}

		ifs.getline(buff, MAX_LENGTH, '>');

		if (ifs.eof()) {//if the getline function raised the eof bit the next lines are useless
			std::cerr << "Invalid file format.";
			return false;
		}

		if (strcmp(buff, HTMLTableTypes::FIELD) == 0) {
			if (!createCell(ifs, rowIndex, cellIndex, 'h'))
				return false;

			cellIndex++;
		}
		if (strcmp(buff, HTMLTableTypes::CELL) == 0) {
			if (!createCell(ifs, rowIndex, cellIndex, 'c')) 
				return false;
			
			cellIndex++;
		}
	}
	table[rowIndex].cellCount = cellIndex;

	return true;
}

bool HTMLTable::createCell(std::ifstream& ifs,int rowIndex, int cellIndex, char type)
{
	char buff[MAX_LENGTH];
	ifs.getline(buff, MAX_LENGTH, '<');

	if (ifs.eof()) {
		std::cerr << "Invalid file format.";
		return false;
	}

	char closingAttrb[MAX_LENGTH];
	ifs.getline(closingAttrb, MAX_LENGTH, '>');

	if (type == 'f') {
		if (strcmp(closingAttrb, HTMLTableTypes::END_FIELD) != 0) {
			std::cerr << "Invalid file format.";
			return false;
		}
	}

	if (type == 'c') {
		if (strcmp(closingAttrb, HTMLTableTypes::END_CELL) != 0) {
			std::cerr << "Invalid file format.";
			return false;
		}
	}
	table[rowIndex].addCell(buff, type, cellIndex);
	return true;
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

void HTMLTable::shiftRowsBack(int id)
{
	if (id < 0 || id >= rows) {
		std::cerr << "Invalid input." <<std::endl;
		return;
	}
	for (int i = rows; i > id; i--) {
		changeRow(i, table[i-1]);
	}
}

void HTMLTable::shiftRowsFront(int id)
{
	for (int i = id; i < rows; i++) {
		changeRow(i, table[i + 1]);
	}
}

void HTMLTable::clearRow(int id)
{
	for (int i = 0; i < maxCols; i++) {
		table[id].changeCell(i, { "",'c' });
	}
}

void HTMLTable::changeRow(int id, const Row& newRow)
{
	int rows = newRow.cellCount > table[id].cellCount ? newRow.cellCount : table[id].cellCount;

	for (int i = 0; i < rows; i++) {
		table[id].changeCell(i, newRow.getField(i));
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

void HTMLTable::Row::changeCell(int id, const Field& newCell)
{
	if (id < 0 || id >= MAX_LENGTH_ROW) {
		std::cout << "Invalid index.";
	}

	row[id].type = newCell.type;
	strcpy(row[id].fieldText, newCell.fieldText);
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

void HTMLTable::Field::writeFieldToFile(std::ofstream& ofs)
{
	if (type == 'h') {
		ofs << "<" << HTMLTableTypes::FIELD << ">";
		ofs << fieldText;
		ofs << "<" << HTMLTableTypes::END_FIELD << ">" << std::endl;
	}
	else if (type == 'c') {
		ofs << "<" << HTMLTableTypes::CELL << ">";
		ofs << fieldText;
		ofs << "<" << HTMLTableTypes::END_CELL << ">" << std::endl;
	}
}
