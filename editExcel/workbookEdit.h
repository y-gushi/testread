#pragma once
#include "typechange.h"
#include <string.h>
#include <stdlib.h>
#include "ChangeArrayNumber.h"

struct workb {
	UINT8* xmlns;
	UINT8* r;
	UINT8* mc;
	UINT8* x15;
	UINT8* xr;
	UINT8* xr6;
	UINT8* xr10;
	UINT8* xr2;
	UINT8* Ignorable;
};

struct fileV {
	UINT8* appName;
	UINT8* lastEdited;
	UINT8* lowestEdited;
	UINT8* rupBuild;
};

struct workbPr {
	UINT8* codeName;
	UINT8* defaultThemeVersion;
};

struct mc_choice {
	UINT8* Requires;
};

struct x15ac_absPath {
	UINT8* x15ac;
	UINT8* url;
};

struct workView {
	UINT8* xWindow;
	UINT8* yWindow;
	UINT8* windowWidth;
	UINT8* windowHeight;
	UINT8* tabRatio;
	UINT8* firstSheet;
	UINT8* activeTab;
	UINT8* uid;
};

struct wb_sheets {
	UINT8* name;
	UINT8* sheetId;
	UINT8* id;
	wb_sheets* next;
};

struct extReferences {
	UINT8* id;
	extReferences* next;
};

struct dfnames {
	UINT8* name;
	UINT8* localSheetId;
	UINT8* hidden;
	UINT8* dfname;
	dfnames* next;
};

struct feature {
	UINT8* name;
	feature* next;
};

struct ext {
	UINT8* xcalcf;
	UINT8* uri;
	feature* calcFeature;
};

struct revisionptr {
	UINT8* revIDLastSave;
	UINT8* documentId;
	UINT8* coauthVersionLast;
	UINT8* coauthVersionMax;
	UINT8* uidLastSave;
};

class WorkBook_edi {
public:
	UINT8* d;
	UINT32 dl;

	UINT8* wd;
	UINT32 wl;
	UINT32 p;

	UINT32 sheetIdMax;

	UINT32 stocklen;
	UINT32 shsize;
	UINT32 shcount;

	UINT8* titlestr;
	workb* wbroot;
	fileV* fVroot;
	workbPr* wProot;

	workView* wVroot;
	wb_sheets** wbshroot;
	extReferences* exRroot;
	dfnames* dfnroot;
	feature* fearoot;
	ext* exroot;
	revisionptr* reviPtrroot;

	mc_choice* mcho;
	x15ac_absPath* xfifteen;
	UINT8* mc_al;

	UINT8* calcPr;

	WorkBook_edi(UINT8* data, UINT32 dlen, UINT32 Sheetsize);
	~WorkBook_edi();

	void readworkbook();
	void xmltitleread();
	void readhead();
	void freeworkbook();
	void readfilever();
	void freefileversion();
	void readworkbookpr();
	void freeworkbookPr();
	void readAlterContent();
	void freeAlterContent();
	void readmcchoice();
	void readXrRevisionPr();
	void freeXrRevisionPr();
	void readbookviewg();
	void freebookviewg();
	void readsheets();
	feature* addfeat(feature* fe, UINT8* n);
	UINT8* getvalue();
	UINT8* getridNum();
	void getsheetsV();
	void addsheets(wb_sheets* wbs, UINT8* n, UINT8* s, UINT8* r);
	void freesheets();
	void readEReference();
	void getereference();
	extReferences* addextReference(extReferences* ex, UINT8* id);
	void freeextReference();
	void readdefinedName();
	void getdefinName();
	dfnames* adddfnames(dfnames* dn, UINT8* n, UINT8* l, UINT8* h, UINT8* v);
	void freefnames();
	void readcalcpr();
	void readextLst();
	void getextLst();
	void freefeature(feature* f);
	void freeextLst();
	void readXcalcf();
	void getfeature();
	void writeworkbook();
	void oneStrwrite(char* str);
	void oneStrplusDoubleq(char* str, UINT8* v);
};