#include "Content_Type_edit.h"

contentEdit::contentEdit(UINT8* data, UINT32 datalen) {
	d = data;
	dl = datalen;
	p = 0;
	wd = nullptr;
	wl = 0;
	stocklen = 0;
	ovesize = 0;
	defsize = 0;
	hashstock = 0;
	titstr = nullptr;
	xmlns = nullptr;
	droot = nullptr;
	oroot = nullptr;
}

contentEdit::~contentEdit() {
	freedef();
	freeove();
	free(titstr);
	free(xmlns);
	free(wd);
}

void contentEdit::contentwrite() {
	const char* topstr[] = { "<Types"," xmlns=\"" };
	const char* defstr[] = { "<Default"," Extension=\""," ContentType=\"" };
	const char* ovestr[] = { "<Override"," PartName=\""," ContentType=\"" };
	const char* laststr = "</Types>";
	char clo = '>';
	char sla[] = "/>";

	size_t siz = dl + 1000;
	wd = (UINT8*)malloc(siz);

	oneStrwrite((char*)titstr);

	oneStrwrite((char*)topstr[0]);
	oneStrplusDoubleq((char*)topstr[1], xmlns);
	wd[wl] = clo; wl++;

	Default* de = droot;
	while (de) {
		oneStrwrite((char*)defstr[0]);
		if(de->Extension)
			oneStrplusDoubleq((char*)defstr[1], de->Extension);
		if (de->ContentType)
			oneStrplusDoubleq((char*)defstr[2], de->ContentType);
		oneStrwrite(sla);
		de = de->next;
	}

	Override* ov = oroot;
	while (ov) {
		oneStrwrite((char*)ovestr[0]);
		if (ov->PartName)
			oneStrplusDoubleq((char*)ovestr[1], ov->PartName);
		if (ov->ContentType)
			oneStrplusDoubleq((char*)ovestr[2], ov->ContentType);
		oneStrwrite(sla);
		ov = ov->next;
	}

	oneStrwrite((char*)laststr);
}

void contentEdit::contentread() {
	const char* def[] = { "<Types","<Default","<Override","</Types>" };
	//6 8 9 8

	UINT8 one[7] = { 0 };
	UINT8 two[9] = { 0 };
	UINT8 thr[10] = { 0 };

	UINT8* ex = nullptr;
	UINT8* con = nullptr;

	int res = 0;

	while (d[p] != '>')
		p++;
	p++;
	titstr = (UINT8*)malloc(sizeof(UINT8) * (p + 1));

	for (UINT32 i = 0; i < p; i++) {
		titstr[i] = d[i];
	}
	titstr[p] = '\0';

	do
	{
		for (int i = 0; i < 8; i++) {
			thr[i] = thr[i + 1];
			if (i < 7)
				two[i] = two[i + 1];
			if (i < 5)
				one[i] = one[i + 1];
		}
		one[5] = two[7] = thr[8] = d[p]; p++;

		res = strncmp((char*)one, def[0], 6);
		if (res == 0)
			readxmlns();

		res = strncmp((char*)two, def[1], 8);
		if (res == 0)
			readdefault();

		res = strncmp((char*)thr, def[2], 9);
		if (res == 0)
			readover();

		res = strncmp((char*)two, def[3], 8);

	} while (res!=0);
}

void contentEdit::readxmlns() {
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
			xmlns = getvalue();
	}
}

Default* contentEdit::adddefa(Default* de, UINT8* e, UINT8* c) {
	if (!de) {
		de = (Default*)malloc(sizeof(Default));
		de->Extension = e;
		de->ContentType = c;
		de->next = nullptr;
	}
	else {
		de->next = adddefa(de->next, e, c);
	}

	return de;
}

Override* contentEdit::addove(Override* o, UINT8* p, UINT8* c,UINT32 h) {
	if (!o) {
		o = (Override*)malloc(sizeof(Override));
		o->PartName = p;
		o->ContentType = c;
		o->hash = h;
		o->next = nullptr;
	}
	else {
		o->next = addove(o->next, p, c,h);
	}

	return o;
}

void contentEdit::freedef() {
	Default* p;
	while (droot) {
		p = droot->next;
		if (droot) {
			free(droot->Extension);
			free(droot->ContentType);
		}
		free(droot);
		droot = p;
	}
}

void contentEdit::freeove() {
	Override* p;
	while (oroot) {
		p = oroot->next;
		if (oroot) {
			free(oroot->PartName);
			free(oroot->ContentType);
		}
		free(oroot);
		oroot = p;
	}
}

void contentEdit::readdefault() {
	const char* def[] = { "Extension=\"","ContentType=\"" };//11 13

	UINT8 one[12] = { 0 };
	UINT8 two[14] = { 0 };

	UINT8* ex = nullptr;
	UINT8* con = nullptr;

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 12; i++) {
			two[i] = two[i + 1];
			if (i < 10)
				one[i] = one[i + 1];
		}
		one[10] = two[12] = d[p]; p++;

		res = strncmp((char*)one, def[0], 11);
		if (res == 0)
			ex=getvalue();

		res = strncmp((char*)two, def[1], 13);
		if (res == 0)
			con = getvalue();
	}
	droot = adddefa(droot, ex, con);
}

void contentEdit::readover() {
	const char* ove[] = { "PartName=\"","ContentType=\"" };//10 13

	UINT8 one[11] = { 0 };
	UINT8 two[14] = { 0 };

	UINT8* pa = nullptr;
	UINT8* con = nullptr;
	UINT32 ha = 0;

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 12; i++) {
			two[i] = two[i + 1];
			if (i < 9)
				one[i] = one[i + 1];
		}
		one[9] = two[12] = d[p]; p++;

		res = strncmp((char*)one, ove[0], 10);
		if (res == 0) {
			pa = getvalueandhash();
			ha = hashstock;
		}

		res = strncmp((char*)two, ove[1], 13);
		if (res == 0)
			con = getvalue();
	}

	oroot = addove(oroot, pa, con, ha);
}

UINT8* contentEdit::getvalue() {

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

UINT8* contentEdit::getvalueandhash() {

	UINT32 len = 0;
	UINT8* Sv = nullptr;
	UINT32 hashsize = 548;

	while (d[p + len] != '"')
		len++;

	stocklen = len;
	UINT32 ssize = len + 1;

	Sv = (UINT8*)malloc(ssize);
	hashstock = 0;

	for (UINT32 i = 0; i < len; i++) {
		Sv[i] = d[p];
		if(d[p]<0x3A)
			hashstock += d[p];
		p++;
	}
	//hashstock = hashstock % hashsize;

	Sv[len] = '\0';

	return Sv;
}

void contentEdit::oneStrwrite(char* str) {

	int i = 0;

	while (str[i] != '\0') {
		wd[wl] = str[i];
		i++;
		wl++;
	}
}

void contentEdit::oneStrplusDoubleq(char* str, UINT8* v) {

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