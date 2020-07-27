#pragma once
#include "typechange.h"
#include <string.h>
#include <stdlib.h>

struct Default {
	UINT8* Extension;
	UINT8* ContentType;
	Default* next;
};

struct Override {
	UINT8* PartName;
	UINT8* ContentType;
	UINT32 hash;
	Override* next;
};

class contentEdit {
public:
	UINT8* d;
	UINT32 dl;
	UINT32 p;

	UINT8* wd;
	UINT32 wl;

	UINT8* titstr;
	UINT8* xmlns;
	UINT32 stocklen;
	UINT32 hashstock;
	Default* droot;
	Override* oroot;

	UINT32 ovesize;
	UINT32 defsize;

	contentEdit(UINT8* data, UINT32 datalen);
	~contentEdit();
	void contentwrite();
	void contentread();
	void readxmlns();
	Default* adddefa(Default* de, UINT8* e, UINT8* c);
	Override* addove(Override* o, UINT8* p, UINT8* c, UINT32 h);
	void freedef();
	void freeove();
	void readdefault();
	void readover();
	UINT8* getvalue();
	UINT8* getvalueandhash();
	void oneStrwrite(char* str);
	void oneStrplusDoubleq(char* str, UINT8* v);
};