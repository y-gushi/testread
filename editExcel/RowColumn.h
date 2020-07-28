#pragma once

#include "ChangeArrayNumber.h"
#include "shareRandW.h"
#include "TagAndItems.h"

struct sheetRels {
    UINT8* id;
    UINT8* target;
    sheetRels* next;
};

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

    worksheetV* wsV = nullptr;
    SheetViews* ShV = nullptr;
    SheetPr* Pr = nullptr;
    Row* rows = nullptr;
    selection* sct = nullptr;
    demention* dm = nullptr;
    cols* cls = nullptr;
    Pane* Panes = nullptr;
    sheetFPr* sfopr = nullptr;
    condiFormat* condF = nullptr;
    pagemargin* pmargin = nullptr;
    pagesetup* pasetup = nullptr;
    UINT8* phoneticPr_fontId = nullptr;
    UINT8* drawing_id = nullptr;

    UINT64 p = 0;

    UINT8* sFPr = nullptr;
    UINT8* margeCellCount = nullptr;
    UINT32 margeCellNum = 0;

    MargeCell* margeCellRoot = nullptr;

    UINT8* data;
    UINT8* wd;
    UINT64 dlen = 0;
    shareRandD* sh;
    UINT32 maxcol = 0;

    UINT32 stocklen = 0;//value stock str len

    UINT8* relhstr;//sheetrels head str
    UINT32 relp;//rels“Ç‚Ýž‚ÝˆÊ’u
    UINT8* relsxmlns;
    sheetRels* relroot;
    UINT8* rd;
    UINT32 rdl;
    //rels write data
    UINT8* rwd;
    UINT32 wl;

    int sClen = 0;//diment
    int sRlen = 0;//diment
    int eClen = 0;//diment
    int eRlen = 0;//diment

    void GetCtagValue();

    selection* getselection(selection* seLe);

    void Getrow();
    void readsvValues();

    void freesheetviews();
    void GetSheetViews();
    void readsheetView();
    void GetSheetPr();

    void Getcols();
    cols* addcolatyle(cols* cs, cols* csP, UINT8* min, UINT8* max, UINT8* W, UINT8* sty, UINT8* hid, UINT8* bF, UINT8* cuW);
    condiFormat* addcondiFormat(condiFormat* cf, UINT8* sq, cfRule* cr);
    cfvo* readcfvo(cfvo* cv);
    void getcolv();

    void getsheetFormatPr();

    void freesheetFormatPr();

    Pane* GetPane(Pane* pa);

    C* addCtable(C* c, UINT8* tv, UINT8* sv, UINT8* si, UINT32 col, UINT8* v, F* fv);

    selection* SLTaddtable(selection* s, UINT8* pv, UINT8* av, UINT8* sv);
    void GetDiment(UINT8* d);
    Row* addrows(Row* row, UINT32 r, UINT8* spanS, UINT8* spanE, UINT8* ht, UINT8* thickBot, UINT8* s, UINT8* customFormat, UINT8* customHeight, C* cell);
    Row* searchRow(Row* r, UINT32 newrow);
    Pane* addpanetable(Pane* p, UINT8* x, UINT8* y, UINT8* tl, UINT8* ap, UINT8* sta);
    F* getFtag();
    cfcolor* readcfcol(cfcolor* cco);
    cfRule* addcfRule(cfRule* cr, UINT8* t, UINT8* dx, UINT8* pri, UINT8* op, UINT8* fo, cfvo* cf, cfcolor* cc);
    cfRule* readcfRule(cfRule* cfr);
    UINT8* readformula();
    void readconditional();
    void freeconditional();
    void freecfr(cfRule* c);

    void freeVo(cfvo* cf);

    void freecfcolor(cfcolor* c);

    void readPageSetup();
    void freepagesetup();
    void readPageMargin();
    void freepagemargin();
    void getfinalstr();

    MargeCell* addmargecell(MargeCell* m, UINT8* s, UINT8* e);

    void getCtag();
    void xmlheaderRead();
    void readworksheet();
    void freeheadxms();
    void readsheetPr();
    void freePr();
    UINT8* getvalue();
    UINT8* getVorftag(const char* tag, UINT32 taglen, UINT32* size);
    UINT8* getSi(UINT8* v, UINT32 vl);

    void getMargeCell();

    void freemarge();

    UINT8** splitref(UINT8* s);

    void Ctablefree(C* c);
    void Rowtablefree();
    void selectfree();
    void colfree();
    void panefree();

    void sheetread();

    const char* closetag = "\"/>";
    int writep = 0;

    void addcelldata(UINT8* row, UINT8* col, UINT8* t, UINT8* s, UINT8* v, F* f, UINT8* si);
    void writesheetdata();
    UINT8* writeheadpart();
    void writecols();
    void writeDiment();
    void writeSelection();
    void writecells();
    void writec(C* ctag, UINT8* ROW);
    void writefinal();

    void oneStrplusDoubleq(UINT8* str, UINT8* v);

    void oneStrwrite(UINT8* str);
    void readsheetrels(UINT8* data, UINT32 datalen);
    void read_Relations();
    sheetRels* addrels(sheetRels* sr, UINT8* id, UINT8* t);
    void freerels();
    void read_Relationship();
    void writerels();
    void rels_Doubleqwrite(UINT8* str, UINT8* v);
    void rels_oneStrwrite(UINT8* str);
    void colsSettingPlus(cols* nc);
    bool checkcols(cols* bcl, cols* ncl, UINT32 nma, UINT32 nmi);
    void insertcols(cols* bc, cols* nc, UINT32 mi, UINT32 ma);
    void colspushlast(cols* n);
};
