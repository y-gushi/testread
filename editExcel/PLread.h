#pragma once
#include <fstream>
#include "TagAndItems.h"
#include "Header.h"
#include "shareRandW.h"
#include "RowColumn.h"
#include "ShipDataRead.h"

class PLRead {
private:
	const char* ef = "file open error";
	const char* shares = "sharedStrings.xml";

public:

	std::ifstream inf;
	Items* erroritem;
	size_t fnlen;

	PLRead();
	~PLRead();

	void freeItem(Items* t);

	Items* packingread(char* fn);

};