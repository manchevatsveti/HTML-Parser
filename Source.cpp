#include <iostream>
#include <fstream>
#include "HTMLTable.h"
#include "Commands.h"
const int MAX_FILENAME = 100;

int main() {

	Commands c;
	c.initCommand();

	/*HTMLTable table;
	table.readTable(filename);

	table.print();

	table.editRow(3, 2, "newValue");
	std::cout<< std::endl;

	table.print();

	table.add(5, "Aleks Love,123,eho,eho1,eho2,eho3");
	std::cout << std::endl;

	table.print();

	table.remove(5);
	std::cout << std::endl;

	table.print();

	table.writeToFile("output.txt");*/
	//std::cout << table.maxCols <<" "<< table.rows;
}
