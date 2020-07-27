#pragma once
#include "typechange.h"
#include <stdlib.h>
#include <string.h>

struct relations {
	UINT8* Id;
	UINT8* type;
	UINT8* target;
	relations* next;
};
class workb_rels {
public:
	UINT8* d;
	UINT32 dl;

	UINT8* tstr;
	UINT8* xmlns;

	UINT32 p;

	UINT32 stocklen;

	UINT8* wd;
	UINT32 wl;

	relations* relroot;

	workb_rels(UINT8* data,UINT32 dlen);
	~workb_rels();

	void readwbrels();
	void writewbrel();
	void freerel();
	relations* addrel(relations* r, UINT8* id, UINT8* ty, UINT8* ta);
	void readRelat();
	void readrelations();
	UINT8* getvalue();
	void oneStrwrite(char* str);
	void oneStrplusDoubleq(char* str, UINT8* v);
};
