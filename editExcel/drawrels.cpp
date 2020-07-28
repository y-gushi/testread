#include "draw_edit.h"

void DrawEdit::readdrawrels(UINT8* data, UINT32 datalen) {
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

void DrawEdit::read_Relations() {
	const char* relstrs = "xmlns=\"";//7
	const char* relationstr = "<Relationship";//13
	UINT8 one[8] = { 0 };
	UINT8 two[14] = { 0 };

	int res = 0;

	while (rd[relp] != '>')
	{
		for (int i = 0; i < 6; i++) {
			one[i] = one[i + 1];
		}
		one[6] = rd[relp]; relp++;

		res = strncmp((char*)one, relstrs, 7);
		if (res == 0)
			relsxmlns = getvalue();
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

drawRels* DrawEdit::addrels(drawRels* sr, UINT8* id, UINT8* t, UINT8* ty) {
	if (!sr) {
		sr = (drawRels*)malloc(sizeof(drawRels));
		sr->id = id;
		sr->target = t;
		sr->Type = ty;
		sr->next = nullptr;
	}
	else {
		sr->next = addrels(sr->next, id, t, ty);
	}

	return sr;
}

void DrawEdit::freerels() {
	drawRels* p;
	while (relroot) {
		p = relroot->next;
		free(relroot->id);
		free(relroot->target);
		free(relroot->Type);
		relroot = p;
	}
}

void DrawEdit::read_Relationship() {
	const char* valstr[] = { "Id=\"","Target=\"","Type=\"" };//4 8 6

	UINT8 one[5] = { 0 };
	UINT8 two[9] = { 0 };
	UINT8 thr[7] = { 0 };

	int res = 0;

	UINT8* id = nullptr;
	UINT8* tar = nullptr;
	UINT8* ty = nullptr;

	while (rd[relp] != '>')
	{
		for (int i = 0; i < 7; i++) {
			two[i] = two[i + 1];
			if (i < 3)
				one[i] = one[i + 1];
			if (i < 5)
				thr[i] = thr[i + 1];
		}
		two[7] = one[3] = thr[5] = rd[relp]; relp++;

		res = strncmp((char*)one, valstr[0], 4);
		if (res == 0)
			id = getvalue();

		res = strncmp((char*)two, valstr[1], 8);
		if (res == 0)
			tar = getvalue();

		res = strncmp((char*)two, valstr[2], 6);
		if (res == 0)
			ty = getvalue();
	}

	relroot = addrels(relroot, id, tar,ty);
}

void DrawEdit::writerels() {
	const char* relstrs[] = { "<Relationships"," xmlns=\"","<Relationship"," Id=\""," Target=\""," Type=\"","</Relationships>" };
	char clo = '>';
	const char* sla = "/>";

	size_t newsiz = rdl;
	rwd = (UINT8*)malloc(sizeof(UINT8) * newsiz);

	rels_oneStrwrite(relhstr);

	rels_oneStrwrite((UINT8*)relstrs[0]);
	rels_Doubleqwrite((UINT8*)relstrs[1], relsxmlns);
	rwd[wl] = clo; wl++;

	drawRels* sr = relroot;
	while (sr) {
		rels_oneStrwrite((UINT8*)relstrs[2]);
		rels_Doubleqwrite((UINT8*)relstrs[3], sr->id);
		rels_Doubleqwrite((UINT8*)relstrs[4], sr->Type);
		rels_Doubleqwrite((UINT8*)relstrs[5], sr->target);
		rels_oneStrwrite((UINT8*)sla);
		sr = sr->next;
	}
	rels_oneStrwrite((UINT8*)relstrs[6]);
}

void DrawEdit::rels_Doubleqwrite(UINT8* str, UINT8* v) {

	int i = 0;
	UINT8 d = '"';

	while (str[i] != '\0') {
		rwd[rwl] = str[i];
		i++;
		rwl++;
	}
	i = 0;

	while (v[i] != '\0') {
		rwd[rwl] = v[i];
		i++; wl++;
	}

	rwd[rwl] = d;
	wl++;
}

//tagèëÇ´çûÇ›
void DrawEdit::rels_oneStrwrite(UINT8* str) {

	int i = 0;

	while (str[i] != '\0') {
		rwd[rwl] = str[i];
		i++;
		wl++;
	}
}