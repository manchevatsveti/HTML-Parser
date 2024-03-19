#include <iostream>
#include <fstream>
#include "HTMLTable.h"
const int MAX_FILENAME = 100;

int main() {
	char filename[MAX_FILENAME];
	
	std::cin >> filename;

	HTMLTable table;
	table.readTable(filename);

	table.print();
	
	//std::cout << table.maxCols <<" "<< table.rows;
}
