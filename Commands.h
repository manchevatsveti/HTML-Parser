#include <iostream>
#include "HTMLTable.h"

#pragma once
class Commands
{
	HTMLTable table;

	void createAddFunc();
	void createRemoveFunc();
	void createEditFunc();
	void createPrintFunc();
	void createExitFunc(const char* filename);

public:
	void initCommand();

};

