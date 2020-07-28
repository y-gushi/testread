#include "RowColumn.h"

void Ctags::readsheetrels(UINT8* data, UINT32 datalen) {
	const char* headstr = "<Relationships";//14
	rd = data;
	rdl = datalen;

	UINT8 one[15] = { 0 };

	while (data[relp] != '>')
		relp++;
	relp++;
	relhstr = (UINT8*)malloc(sizeof(UINT8) * (relp + 1));

	for (UINT32 i = 0; i < relp; i++) {
		relhstr[i] = data[i];
	}
	relhstr[relp] = '\0';

	int res = 0;

	do
	{
		for (int i = 0; i < 13; i++) {
			one[i] = one[i + 1];
		}
		one[13] = data[relp]; relp++;

		res = strncmp((char*)one, headstr, 14);
		if (res == 0)
			read_Relations();

	} while (res != 0);
}

void Ctags::read_Relations() {
	const char* relstrs = "xmlns=\"";//7
	const char* relationstr = "<Relationship";//13
	UINT8 one[8] = { 0 };
	UINT8 two[14] = { 0 };

	int res = 0;

	while(rd[relp]!='>')
	{
		for (int i = 0; i < 6; i++) {
			one[i] = one[i + 1];
		}
		one[6] = rd[relp]; relp++;

		res = strncmp((char*)one, relstrs, 7);
		if (res == 0)
			relsxmlns=getvalue();
	}

	do
	{
		for (int i = 0; i < 12; i++) {
			two[i] = two[i + 1];
		}
		two[6] = rd[relp]; relp++;

		res = strncmp((char*)two, relationstr, 13);
		if (res == 0)
			relsxmlns = getvalue();
	} while (res != 0);
}

sheetRels* Ctags::addrels(sheetRels* sr, UINT8* id, UINT8* t) {
	if (!sr) {
		sr = (sheetRels*)malloc(sizeof(sheetRels));
		sr->id = id;
		sr->target = t;
		sr->next = nullptr;
	}
	else {
		sr->next = addrels(sr->next, id, t);
	}

	return sr;
}

void Ctags::freerels() {
	sheetRels* p;
	while (relroot) {
		p = relroot->next;
		free(relroot->id);
		free(relroot->target);
		relroot = p;
	}
}

void Ctags::read_Relationship() {
	const char* valstr[] = { "Id=\"","Target=\"" };//4 8

	UINT8 one[5] = { 0 };
	UINT8 two[9] = { 0 };

	int res = 0;

	UINT8* id = nullptr;
	UINT8* tar = nullptr;

	while (rd[relp] != '>')
	{
		for (int i = 0; i < 7; i++) {
			two[i] = two[i + 1];
			if(i<3)
				one[i] = one[i + 1];
		}
		two[7] = one[3] = rd[relp]; relp++;

		res = strncmp((char*)one, valstr[0], 4);
		if (res == 0)
			id = getvalue();

		res = strncmp((char*)two, valstr[1], 8);
		if (res == 0)
			tar = getvalue();
	}

	relroot = addrels(relroot, id, tar);
}

void Ctags::writerels() {
	const char* relstrs[] = { "<Relationships"," xmlns=\"","<Relationship"," Id=\""," Target=\"","</Relationships>" };
	char clo = '>';
	const char* sla = "/>";

	size_t newsiz = rdl;
	rwd = (UINT8*)malloc(sizeof(UINT8) * newsiz);

	rels_oneStrwrite(relhstr);

	rels_oneStrwrite((UINT8*)relstrs[0]);
	rels_Doubleqwrite((UINT8*)relstrs[1], relsxmlns);
	rwd[wl] = clo; wl++;

	sheetRels* sr = relroot;
	while (sr) {
		rels_oneStrwrite((UINT8*)relstrs[2]);
		rels_Doubleqwrite((UINT8*)relstrs[3], sr->id);
		rels_Doubleqwrite((UINT8*)relstrs[4], sr->target);
		rels_oneStrwrite((UINT8*)sla);
		sr = sr->next;
	}
	rels_oneStrwrite((UINT8*)relstrs[5]);	
}

void Ctags::rels_Doubleqwrite(UINT8* str, UINT8* v) {

	int i = 0;
	UINT8 d = '"';

	while (str[i] != '\0') {
		rwd[wl] = str[i];
		i++;
		wl++;
	}
	i = 0;

	while (v[i] != '\0') {
		rwd[wl] = v[i];
		i++; wl++;
	}

	rwd[wl] = d;
	wl++;
}

//tagèëÇ´çûÇ›
void Ctags::rels_oneStrwrite(UINT8* str) {

	int i = 0;

	while (str[i] != '\0') {
		rwd[wl] = str[i];
		i++;
		wl++;
	}
}