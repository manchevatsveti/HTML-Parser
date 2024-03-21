#include "Commands.h"

namespace {
	const char* CONSOLE_MESSAGE = "Enter a number for one of the folowing commands:\n1. add\n2. remove\n3. edit\n4. print\n5. exit";
	const int MAX_FILENAME = 100;
}

void Commands::createAddFunc()
{
	int rowId;
	char newValues[MAX_LENGTH];
	std::cout << "Add row at index: " << std::endl;
	std::cin >> rowId;

	if (!validateIndex_addFunc(rowId)) return;

	std::cout << "Enter the values of the row, separeted by commas - ','. " << std::endl;
	std::cin >> newValues;
	std::cout << std::endl;

	table.add(rowId, (const char*)newValues);
}

void Commands::createRemoveFunc()
{
	int rowId;
	std::cout << "Remove row at index: " << std::endl;
	std::cin >> rowId;

	table.remove(rowId);
}

void Commands::createEditFunc()
{
	int rowId,colId;
	char newValue[MAX_LENGTH];
	std::cout << "Edit cell at row and column: " << std::endl;
	std::cin >> rowId >> colId;

	if (!validateIndex_editFunc(rowId,colId)) return;

	std::cout << "Enter the new value of the cell." << std::endl;
	std::cin >> newValue;
	std::cout << std::endl;

	table.editRow(rowId, colId,(const char*) newValue);
}

void Commands::createPrintFunc()
{
	table.print();
	std::cout << std::endl;
}

void Commands::createExitFunc(const char* filename)
{
	table.writeToFile(filename);
}

bool Commands::validateIndex_addFunc(int id) const
{
	if (id < 1 || id > table.getRows() + 1) {//beacuse the columns and row start from 1
		std::cerr << "Invalid index to add a row to." << std::endl;
		return false;
	}
	return true;
}

bool Commands::validateIndex_editFunc(int rowId, int colId) const
{
	if (rowId < 1 || rowId > table.getRows()) {
		std::cerr << "Invalid row index." << std::endl;
		return false;
	}
	if (colId < 1 || colId > table.getMaxCols()) {
		std::cerr << "Invalid column index." << std::endl;
		return false;
	}
	return true;
}

void Commands::initCommand()
{
	while (true) {
		char filename[100];

		std::cin >> filename;

		if (!table.readTable(filename)) {
			std::cerr << "Try entering the filename again." << std::endl;
		}
		else {
			while (true) {
				std::cout << CONSOLE_MESSAGE << std::endl;

				int input;
				std::cin >> input;

				switch (input) {
				case 1:
					createAddFunc();
					break;
				case 2:
					createRemoveFunc();
					break;
				case 3:
					createEditFunc();
					break;
				case 4:
					createPrintFunc();
					break;
				case 5:
					createExitFunc(filename);
					return;
				default:
					std::cerr << "Invalid input" << std::endl;
				}

			}
		}
	}
}

	

