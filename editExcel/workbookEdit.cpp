#include "workbookEdit.h"

WorkBook_edi::WorkBook_edi()
{
}

WorkBook_edi::~WorkBook_edi()
{
}

void readworkbook(){
	const char* headstr = "<workbook";
	const char* filever = "<fileVersion";
	const char* workbpr = "<workbookPr";
	const char* altconte = "<mc:AlternateContent";
	const char* xr_revistr = "<xr:revisionPtr";
	const char* wbstr = "<bookViews";
	const char* sheetsstr = "<sheets";
	const char* eR = "<externalReferences";
	const char* dName = "<definedNames>";
	const char* calcP = "<calcPr";
	const char* exLsstr = "<extLst>";
	const char* lasttag = "</workbook>";
}

void readhead() {
	const char* headstr[] = { "xmlns=\"","xmlns:r=\"","xmlns:mc=\"","xmlns:x15=\"","xmlns:xr=\"","xmlns:xr6=\"","xmlns:xr10=\""
	,"xmlns:xr2=\"","mc:Ignorable=\"" };

	while (d[p] != '>') {

	}
}
void readfilever() {
	const char* filever[] = { "appName=\"" ,"lastEdited=\"" ,"lowestEdited=\"" ,"rupBuild=\"" };
	while (d[p] != '>') {

	}
}

void readworkbookpr() {
	const char* workbpr[] = { "codeName=\"" ,"defaultThemeVersion=\"" };
	while (d[p] != '>') {

	}
}

void readAlterContent() {
	const char* mcstr[] = { "xmlns:mc=\"" };
	while (d[p] != '>') {

	}
	if (d[p] != '/') {
		readmcchoice();
	}
}

void readmcchoice() {
	const char* mcstr[] = { "revIDLastSave=\"","<x15ac:absPath","xmlns:x15ac=\"","url=\"","</mc:Choice>" };
	const char* etag = "</mc:AlternateContent>";
	while (d[p] != '>') {

	}
	if (d[p] != '/') {

	}
}

void readXrRevisionPr() {
	const char* mcstr[] = { "revIDLastSave=\"""documentId=\"","xr6:coauthVersionLast=\"","xr6:coauthVersionMax=\"","xr10:uidLastSave=\"" };
	while (d[p] != '>') {

	}
}

void readbookviewg() {
	const char* filever[] = { "<workbookView","xWindow=\"" ,"yWindow=\"" ,"windowWidth=\"" ,"windowHeight=\"",
		"tabRatio=\"" ,"firstSheet=\"" ,"activeTab=\"" ,"xr2:uid=\"" };
	const char* endt = "</bookViews>";
	while (d[p] != '>') {

	}
	if (d[p] != '/') {

	}
}

void readsheets() {
	const char* sheetsstr[] = { "<sheet","name=\"" ,"sheetId=\"" ,"r:id=\"" };
	const char* endt = "</sheets>";
	while (d[p] != '>') {

	}
	if (d[p] != '/') {

	}
	getsheetsV();
}

void getsheetsV() {
	const char* sheetsstr[] = { "name=\"" ,"sheetId=\"" ,"r:id=\"" };
	while (d[p] != '>') {

	}
}

void readEReference() {
	const char* exrefestr[] = { "<externalReference","</externalReferences>" };
	getereference();
}

void getereference() {
	const char* exrefestr = "r:id=\"";
	while (d[p] != '>') {

	}
}
void readdefinedName() {
	const char* defNamestr = "<definedName";
	getdefinName();
}

void getdefinName() {
	const char* defNamestr[] = { "name=\"" ,"localSheetId=\"" ,"hidden=\"","</definedName>" };
}

void readcalcpr() {
	const char* castr = "calcId=\"";
}

void readextLst() {
	const char* lststr[] = { "<ext","</extLst>" };
	getextLst();
}

void getextLst() {
	const char* lststr[] = { "<ext","xmlns:xcalcf=\"" ,"uri=\"" ,"<xcalcf:calcFeatures","</ext>" };
	while (d[p] != '>') {

	}
}

void readXcalcf() {
	const char* lststr[] = { "name=\"" ,"</xcalcf:calcFeatures>" };
}

UINT8* WorkBook_edi::getvalue() {

	UINT32 len = 0;
	UINT8* Sv = nullptr;

	while (d[p + len] != '"')
		len++;

	stocklen = len;
	UINT32 ssize = len + 1;

	Sv = (UINT8*)malloc(ssize);

	for (UINT32 i = 0; i < len; i++) {
		Sv[i] = d[p]; p++;
	}

	Sv[len] = '\0';

	return Sv;
}