#pragma once

#include "ChangeArrayNumber.h"
#include "shareRandW.h"
#include "TagAndItems.h"

class Ctags {
public:
    Ctags(UINT8* decorddata, UINT64 datalen, shareRandD* shdata);

    ~Ctags();

    ArrayNumber NA;
    selection* SCT = nullptr;

    const char* Ctag = "<c";
    const char* Vtag = "<v>";
    const char* endVtag = "</v>";
    const char* SVend = "</sheetViews>";//13
    const char* endC = "</c>";
    const char* Ftag = "<f";
    const char* endFtag = "</f>";
    const char* Rowtag = "<row";
    //const char* sheetPr = "<sheetPr";//8
    const char* sheetPrEnd = "</sheetPr>";//10
    const char* codename = "codeName=\"";//10
    const char* dement = "<dimension ref=\"";//16
    const char* pane = "<pane";//5
    const char* Coltag = "<col ";//5
    const char* endtag = "</cols>";//7
    const char* slashend = "/>";//2

    //row tag values
    const char* Rr = "r=\"";//3
    const char* Rspans = "spans=\"";//7
    const char* Rs = "s=\"";//3
    const char* RcustomFormat = "customFormat=\"";//14
    const char* Rht = "ht=\"";//4
    const char* RcustomHeight = "customHeight=\"";//14
    const char* RthickBot = "thickBot=\"";//10

    //cols tag values
    const char* Colmin = "min=\"";//5
    const char* Colmax = "max=\"";//5
    const char* Colswidth = "width=\"";//7
    const char* ColS = "style=\"";//7
    const char* ColcW = "customWidth=\"";//13
    const char* Colbf = "bestFit=\"";//9
    const char* Colhid = "hidden=\"";//8

    //c tag f values
    const char* Fref = "ref=\"";
    const char* Fsi = "si=\"";
    const char* Ft = "t=\"";

    const char* sheetend = "</sheetData>";//12
    const char* startSV = "<sheetView";//10

    worksheetV* wsV = nullptr;
    SheetViews* ShV = nullptr;
    SheetPr* Pr = nullptr;
    Row* rows = nullptr;
    selection* sct = nullptr;
    demention* dm = nullptr;
    cols* cls = nullptr;
    Pane* Panes = nullptr;

    UINT64 p = 0;
    UINT8* fstr = nullptr;

    UINT8* headXML = nullptr;
    UINT8* dimtopane = nullptr;
    UINT8* sFPr = nullptr;
    UINT8* margeCellCount = nullptr;
    UINT32 margeCellNum = 0;

    MargeCell* margeCellRoot = nullptr;
    UINT8* MC;

    UINT8* data;
    UINT8* wd;
    UINT64 dlen = 0;
    shareRandD* sh;
    UINT32 maxcol = 0;

    UINT32 stocklen = 0;//value stock str len

    int sClen = 0;//diment
    int sRlen = 0;//diment
    int eClen = 0;//diment
    int eRlen = 0;//diment

    void GetCtagValue();
    void GetSelectionPane();

    selection* getselection(selection* se);

    void Getrow();
    void readsheetviews();
    void GetSheetPr();

    void Getcols();
    void getcolv();

    Pane* GetPane(Pane* pa);

    C* addCtable(C* c, UINT8* tv, UINT8* sv, UINT8* si, UINT32 col, UINT8* v, F* fv);
    cols* addcolatyle(cols* cs, UINT8* min, UINT8* max, UINT8* W, UINT8* sty, UINT8* hid, UINT8* bF, UINT8* cuW);
    cols* coltalloc();
    selection* SLTaddtable(selection* s, UINT8* pv, UINT8* av, UINT8* sv);
    void GetDiment(UINT8* d);
    Row* addrows(Row* row, UINT32 r, UINT8* spanS, UINT8* spanE, UINT8* ht, UINT8* thickBot, UINT8* s, UINT8* customFormat, UINT8* customHeight, C* cell);
    Row* searchRow(Row* r, UINT32 newrow);
    Pane* addpanetable(Pane* p, UINT8* x, UINT8* y, UINT8* tl, UINT8* ap, UINT8* sta);
    F* getFtag();
    void getfinalstr();

    MargeCell* addmargecell(MargeCell* m, UINT8* s, UINT8* e);

    void getCtag();
    void xmlheaderRead();
    void readworksheet();
    void readsheetPr();
    UINT8* getvalue();
    UINT8* getVorftag(const char* tag, UINT32 taglen, UINT32* size);
    UINT8* getSi(UINT8* v, UINT32 vl);

    void getMargeCell();

    void Ctablefree(C* c);
    void Rowtablefree();
    void selectfree();
    void colfree();
    void panefree();
    void Ctableprint(C* c);
    void sheetread();

    const char* closetag = "\"/>";
    int writep = 0;

    void addcelldata(UINT8* row, UINT8* col, UINT8* t, UINT8* s, UINT8* v, F* f, UINT8* si);
    void writesheetdata();
    void writeheadpart();
    void writecols();
    void writeDiment();
    void writeSelection();
    void writecells();
    void writec(C* ctag, UINT8* ROW);
    void writefinal();

    void oneStrplusDoubleq(UINT8* str, UINT8* v);

    void oneStrwrite(UINT8* str);
};
