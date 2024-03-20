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
	std::cout << std::endl;

	table.remove(rowId);
}

void Commands::createEditFunc()
{
	int rowId,colId;
	char newValue[MAX_LENGTH];
	std::cout << "Edit cell at row and column: " << std::endl;
	std::cin >> rowId >> colId;
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

void Commands::initCommand()
{
	char filename[100];

	std::cin >> filename;

	table.readTable(filename);

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
			std::cerr << "Invalid input"<<std::endl;
		}

	}
}

