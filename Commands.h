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

	bool validateIndex_addFunc(int id) const;
	bool validateIndex_editFunc(int rowId, int colId) const;
public:
	void initCommand();

};

