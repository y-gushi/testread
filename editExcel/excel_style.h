#pragma once
#include "typechange.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "ChangeArrayNumber.h"

struct Fonts {

	UINT8* biu;//Boldなど?

	UINT8* sz;

	UINT8* color;

	UINT8* name;

	UINT8* family;

	UINT8* charset;

	UINT8* scheme;

	UINT8* indexed;

	UINT8* rgb;

	Fonts* next;

};



/*fill内容--------------*/



struct FillPattern

{

	UINT8* patternType;

};



struct fgcolor

{

	UINT8* theme;

	UINT8* tint;

	UINT8* rgb;

};



struct bgcolor

{

	UINT8* indexed;

};



struct Fills

{

	FillPattern* patten;

	fgcolor* fg;

	bgcolor* bg;

	Fills* next;

};

/*-----------------------*/



//ボーダー内容

struct borderstyle
{
	UINT8* style;
	UINT8* theme;
	UINT8* tint;
	UINT8* rgb;
	UINT8* index;
	UINT8* Auto;
};

struct borders
{
	UINT8* diagonalUp;
	UINT8* diagonalDown;
	borderstyle* left;
	borderstyle* right;
	borderstyle* top;
	borderstyle* bottom;
	borderstyle* diagonal;
	borders* next;
};

//cellStyleXfs 内容
struct stylexf
{
	UINT8* numFmtId;
	UINT8* fontId;
	UINT8* fillId;
	UINT8* borderId;
	UINT8* applyNumberFormat;
	UINT8* applyFont;
	UINT8* applyFill;
	UINT8* applyBorder;
	UINT8* applyAlignment;
	UINT8* applyProtection;
	UINT8* Avertical;
	UINT8* wraptext;

	stylexf* next;
};

struct cellxfs
{
	UINT8* numFmtId;
	UINT8* fontId;
	UINT8* fillId;
	UINT8* borderId;
	UINT8* xfId;
	UINT8* applyNumberFormat;
	UINT8* applyFont;
	UINT8* applyFill;
	UINT8* applyBorder;
	UINT8* applyAlignment;
	UINT8* Avertical;
	UINT8* AwrapText;
	UINT8* horizontal;
	UINT8* quotePrefix;
	cellxfs* next;
};



struct cellstyle

{

	UINT8* name;

	UINT8* xfId;

	UINT8* builtinId;

	UINT8* customBuilt;

	UINT8* xruid;

	cellstyle* next;

};



struct dxfFont

{

	UINT8* b;

	UINT8* ival;

	UINT8* theme;

	UINT8* rgb;

};



struct dxfFill

{

	UINT8* rgb;

};



struct Dxf {

	dxfFont* fon;

	dxfFill* fil;

	Dxf* next;

};



struct colors

{

	UINT8* color;

	colors* next;

};



struct numFMts

{

	UINT8* Id;

	UINT8* Code;

	numFMts* next;

};



class styleread {
public:
	const char* fonts;
	const char* Efonts;
	const char* fills;
	const char* endfil;
	const char* border;
	const char* enbor;
	const char* cellstXfs;
	const char* EncsXfs;
	const char* color;
	const char* Ecolor;
	const char* Xfs;
	const char* Exfs;
	const char* CellStyl;
	const char* Ecellsty;
	const char* dxf;
	const char* Edxf;
	const char* Endstyle;
	const char* Fmts;
	const char* Efmts;

	//書き込み用
	const char* countstr = " count=\"";
	ArrayNumber strchange;
	const char* styleSheet;
	const char* extLst = "<extLst>";
	int fontcou;
	size_t strl = 0;//文字列長さ用
	UINT64 readp;
	Fonts** fontRoot;
	Fills** fillroot;
	borders** BorderRoot;
	stylexf** cellstyleXfsRoot;
	cellxfs** cellXfsRoot;
	numFMts** numFmtsRoot;
	cellstyle** cellStyleRoot;
	Dxf** dxfRoot;
	colors* colorsRoot;

	UINT8* fontCount;
	UINT32 fontNum = 0;//フォントカウント数値変換
	UINT32 frcount = 0;
	UINT8* kFonts;
	UINT8* fillCount;
	UINT32 fillNum = 0;//フィルカウント数値変換
	UINT32 ficount = 0;
	UINT8* borderCount;
	UINT32 borderNum = 0;//ボーダーカウント数値変換
	UINT32 bocount = 0;

	UINT8* cellStyleXfsCount;
	UINT32 cellstyleXfsNum = 0;
	UINT32 csXcount = 0;

	UINT8* cellXfsCount;
	UINT32 cellXfsNum = 0;
	UINT32 cXcount = 0;

	UINT8* numFmtsCount;
	UINT32 numFmtsNum = 0;
	UINT32 nucount = 0;

	UINT8* cellstyleCount;
	UINT32 cellstyleNum = 0;
	UINT32 cscount = 0;

	UINT8* dxfsCount;
	UINT32 dxfsNm = 0;
	UINT32 dxcount = 0;

	UINT8* tablestyle;
	UINT8* styleSheetStr;
	UINT8* extLstStr;

	size_t testfontcou = 0;
	size_t testfillcou = 0;

	styleread();
	~styleread();



	void freefonts(Fonts* f);



	void freefill(Fills* f);



	void freeborder(borders* f);



	void freestylexf(stylexf* f);



	void freecellxfs(cellxfs* f);



	void freenumFMts(numFMts* f);



	void freecellstyle(cellstyle* f);



	void freeDxf(Dxf* f);



	Fonts* addfonts(Fonts* f, UINT8* b, UINT8* sz, UINT8* co, UINT8* na, UINT8* fa, UINT8* cha, UINT8* sch, UINT8* rg, UINT8* ind);



	Fills* addfill(Fills* fi, FillPattern* p, fgcolor* f, bgcolor* b);



	borders* addborder(borders* b, borderstyle* l, borderstyle* r, borderstyle* t, borderstyle* bo, borderstyle* d);



	stylexf* addstylexf(stylexf* sx, UINT8* n, UINT8* fo, UINT8* fi, UINT8* bi, UINT8* an, UINT8* ab, UINT8* aa, UINT8* ap, UINT8* av, UINT8* af, UINT8* afil);



	cellxfs* addcellxfs(cellxfs* c, UINT8* n, UINT8* fo, UINT8* fi, UINT8* bi, UINT8* an, UINT8* ab, UINT8* aa, UINT8* av, UINT8* at, UINT8* ho, UINT8* afo, UINT8* afi, UINT8* xid);



	cellstyle* addcellstyle(cellstyle* c, UINT8* n, UINT8* x, UINT8* b, UINT8* cu, UINT8* xr);



	Dxf* adddxf(Dxf* d, dxfFont* Fo, dxfFill* Fi);



	colors* addcolor(colors* c, UINT8* co);



	numFMts* addnumFmts(numFMts* n, UINT8* i, UINT8* c);



	void readfontV(UINT8* dat);



	UINT8* readfonts(UINT8* sd);



	void getfillV(UINT8* d);



	UINT8* readfill(UINT8* sd);



	fgcolor* readfillFg(UINT8* dat);



	bgcolor* readbgcolor(UINT8* dat);



	borderstyle* getborV(UINT8* dat, UINT8* endT, size_t endTlen);



	void getBorder(UINT8* d);



	UINT8* readboerder(UINT8* d);



	UINT8* getValue(UINT8* d);



	void getCellStyleXfsV(UINT8* d);



	void getCellXfsV(UINT8* d);



	void readCellStyleXfs(UINT8* d);



	void readCellXfs(UINT8* d);



	void getnumFmts(UINT8* dat);



	void readnumFmt(UINT8* d);



	void getcellStyle(UINT8* d);



	void readcellStyles(UINT8* d);



	dxfFont* readdxffont(UINT8* d);



	dxfFill* readdxffill(UINT8* d);



	void getdxfs(UINT8* d);



	void readdxfs(UINT8* d);



	void getcolor(UINT8* d);



	void readcolors(UINT8* d);



	void readextLst(UINT8* d);



	void readstyle(UINT8* sdata, UINT64 sdatalen);

};