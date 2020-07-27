#include "workbook_rel_edi.h"

workb_rels::workb_rels(UINT8* data, UINT32 dlen)
{
	d = data;
	dl = dlen;

	wd = nullptr;
	wl = 0;

	p = 0;
	stocklen = 0;

	relroot = nullptr;
}

workb_rels::~workb_rels()
{
	free(wd);
	free(tstr);
	freerel();
}

void workb_rels::readwbrels()
{
	const char* headstr[] = { "<Relationships","<Relationship","</Relationships>" };//14 13 16

	UINT8 one[15] = { 0 };
	UINT8 two[14] = { 0 };
	UINT8 thr[17] = { 0 };

	int res = 0;
	while (d[p] != '>')
		p++;
	p++;
	tstr = (UINT8*)malloc(sizeof(UINT8) * (p + 1));

	for (UINT32 i = 0; i < p; i++) {
		tstr[i] = d[i];
	}
	tstr[p] = '\0';

	do
	{
		for (int i = 0; i < 13; i++) {
			one[i] = one[i + 1];
		}
		one[13] = d[p]; p++;

		res = strncmp((char*)one, headstr[0], 14);
		if (res == 0)
			readrelations();

	} while (res != 0);

	do
	{
		for (int i = 0; i < 15; i++) {
			thr[i] = thr[i + 1];
			if (i < 12)
				two[i] = two[i + 1];
		}
		two[12] = thr[15] = d[p]; p++;

		res = strncmp((char*)two, headstr[1], 13);
		if (res == 0)
			readRelat();

		res = strncmp((char*)thr, headstr[2], 16);

	} while (res != 0);

}

void workb_rels::writewbrel() {
	const char* heads[] = { "<Relationships"," xmlns=\"" };
	const char* relstr[] = { "<Relationship"," Id=\""," Type=\""," Target=\"","</Relationships>" };
	char clo = '>';
	char sla[] = "/>";

	size_t siz = dl + 1000;
	wd= (UINT8*)malloc(sizeof(UINT8)*siz);

	oneStrwrite((char*)tstr);

	oneStrwrite((char*)heads[0]);
	oneStrplusDoubleq((char*)heads[1], xmlns);
	wd[wl] = clo; wl++;

	relations* r=relroot;
	while (r) {
		oneStrwrite((char*)relstr[0]);
		if(r->Id)
		oneStrplusDoubleq((char*)relstr[1], r->Id);
		if (r->type)
			oneStrplusDoubleq((char*)relstr[2], r->type);
		if (r->target)
			oneStrplusDoubleq((char*)relstr[3], r->target);
		oneStrwrite(sla);
		r = r->next;
	}

	oneStrwrite((char*)relstr[4]);
}

void workb_rels::freerel() {
	relations* p;
	while (relroot) {
		p = relroot->next;
		if (relroot) {
			free(relroot->Id);
			free(relroot->type);
			free(relroot->target);
		}
		free(relroot);
		relroot = p;
	}
}

relations* workb_rels::addrel(relations* r, UINT8* id, UINT8* ty, UINT8* ta) {
	if (!r) {
		r = (relations*)malloc(sizeof(relations));
		r->Id = id;
		r->type = ty;
		r->target = ta;
		r->next = nullptr;
	}
	else {
		r->next = addrel(r->next, id, ty, ta);
	}

	return r;
}

void workb_rels::readRelat() {
	const char* xm[] = { "Id=\"","Type=\"","Target=\"" };//4 6 8

	UINT8 one[5] = { 0 };
	UINT8 two[7] = { 0 };
	UINT8 thr[9] = { 0 };

	UINT8* id = nullptr;
	UINT8* ty = nullptr;
	UINT8* ta = nullptr;

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 7; i++) {
			thr[i] = thr[i + 1];
			if(i<5)
				two[i] = two[i + 1];
			if (i < 3)
				one[i] = one[i + 1];
		}
		one[3] = two[5] = thr[7] = d[p]; p++;

		res = strncmp((char*)one, xm[0], 4);
		if (res == 0)
			id = getvalue();

		res = strncmp((char*)two, xm[1], 6);
		if (res == 0)
			ty = getvalue();

		res = strncmp((char*)thr, xm[2], 8);
		if (res == 0)
			ta = getvalue();
	}

	relroot = addrel(relroot, id, ty, ta);
}

void workb_rels::readrelations() {
	const char* xm = "xmlns=\"";//7

	UINT8 one[8] = { 0 };

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 6; i++) {
			one[i] = one[i + 1];
		}
		one[6] = d[p]; p++;

		res = strncmp((char*)one, xm, 7);
		if (res == 0)
			xmlns=getvalue();
	}

}

UINT8* workb_rels::getvalue() {

	UINT32 len = 0;
	UINT8* Sv = nullptr;

	while (d[p + len] != '"')
		len++;

	stocklen = len;
	UINT32 ssize = len + 1;

	Sv = (UINT8*)malloc(ssize);

	for (UINT32 i = 0; i < len; i++) {
		Sv[i] = d[p];
		p++;
	}

	Sv[len] = '\0';

	return Sv;
}

void workb_rels::oneStrwrite(char* str) {

	int i = 0;

	while (str[i] != '\0') {
		wd[wl] = str[i];
		i++;
		wl++;
	}
}

void workb_rels::oneStrplusDoubleq(char* str, UINT8* v) {

	int i = 0;
	UINT8 d = '"';

	while (str[i] != '\0') {
		wd[wl] = str[i];
		i++;
		wl++;
	}
	i = 0;

	while (v[i] != '\0') {
		wd[wl] = v[i];
		i++; wl++;
	}

	wd[wl] = d;
	wl++;
}