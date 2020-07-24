#include "RowColumn.h"

Ctags::Ctags(UINT8* decorddata, UINT64 datalen, shareRandD* shdata) {
    data = decorddata;
    dlen = datalen;
    sh = shdata;
    wd = nullptr;
}

Ctags::~Ctags() {

    free(sFPr);

    free(dm->sC);
    free(dm->sR);
    free(dm->eC);
    free(dm->eR);
    free(dm);

    free(margeCellCount);

    free(wd);
    free(drawing_id);
    free(phoneticPr_fontId);

    Rowtablefree();
    selectfree();
    panefree();
    colfree();

    freeheadxms();
    freemarge();
    freePr();
    freeconditional();
    freesheetviews();
    freesheetFormatPr();
    freepagemargin();
    freepagesetup();
}

void Ctags::sheetread() {

    xmlheaderRead();

    GetSheetPr();

    GetSheetViews();

    Getcols();// <sheetdata まで

    GetCtagValue();

    getfinalstr();

}

//c tag add table
C* Ctags::addCtable(C* c, UINT8* tv, UINT8* sv, UINT8* si, UINT32 col, UINT8* v, F* fv) {

    if (!c) {
        c = (C*)malloc(sizeof(C));

        if (c) {
            c->val = v;
            c->t = tv;
            c->s = sv;
            c->si = si;
            c->col = col;
            c->f = fv;
            c->next = nullptr;
        }
    }
    else if (col == c->col) {
        c->val = v;
        c->t = tv;
        c->s = sv;
        c->si = si;
        c->col = col;
        c->f = fv;
    }
    else if (col < c->col) {
        C* p = (C*)malloc(sizeof(C));
        if (p && c) {
            p->val = c->val;
            p->t = c->t;
            p->s = c->s;
            p->si = c->si;
            p->col = c->col;
            p->f = c->f;
            p->next = c->next;
        }
        c = (C*)malloc(sizeof(C));
        if (c) {
            c->val = v;
            c->t = tv;
            c->s = sv;
            c->si = si;
            c->col = col;
            c->f = fv;
            c->next = p;
        }
    }
    else {
        c->next = addCtable(c->next, tv, sv, si, col, v, fv);
    }
    return c;
}

void Ctags::GetCtagValue() {

    int endresult = 1;

    UINT8 rs[5] = { 0 };// <row
    UINT8 sr[3] = { 0 };// <c 
    UINT8 shend[13] = { 0 };//f 

    while (data[p] != '>') {// スラッシュ検出
        if (data[p] == '/') {
            break;
        }
        p++;
    }

    if (data[p] != '/') {
        while (endresult != 0) {
            for (int j = 0; j < 12 - 1; j++) {
                shend[j] = shend[j + 1];
                if (j < 4 - 1)
                    rs[j] = rs[j + 1];
                if (j < (2 - 1))
                    sr[j] = sr[j + 1];
            }

            shend[12 - 1] = rs[4 - 1] = sr[2 - 1] = data[p];
            shend[12] = rs[4] = sr[2] = '\0';
            p++;

            endresult = strncmp((char const*)shend, sheetend, 12);// </sheetdata>

            if (strncmp((char const*)rs, Rowtag, 4) == 0)
                Getrow();//row tag get

            if (strncmp((char const*)sr, Ctag, 2) == 0)// <c match
                getCtag();
        }
    }
}

UINT8* Ctags::getvalue() {

    UINT32 len = 0;
    UINT8* Sv = nullptr;

    while (data[p + len] != '"')
        len++;

    stocklen = len;
    UINT32 ssize = len + 1;

    Sv = (UINT8*)malloc(ssize);

    for (UINT32 i = 0; i < len; i++) {
        Sv[i] = data[p]; p++;
    }

    Sv[len] = '\0';

    return Sv;
}

UINT8* Ctags::getSi(UINT8* v, UINT32 vl) {

    UINT32 Vnum = 0;
    UINT8* Vsi = nullptr;

    Vnum = NA.RowArraytoNum(v, vl);

    UINT32 sistrlen = 0;
    UINT32 Tstrlen = 0;

    Tvalue* RootSi = sh->sis[Vnum]->Ts;

    if (RootSi) {
        while (RootSi) {
            Tstrlen = 0;
            if (RootSi->Tv) {
                while (RootSi->Tv[Tstrlen] != '\0') {
                    sistrlen++; Tstrlen++;
                }
            }
            RootSi = RootSi->next;
        }
        RootSi = sh->sis[Vnum]->Ts;

        if (sistrlen > 0) {
            size_t slen = sistrlen + 1;
            Vsi = (UINT8*)malloc(slen);

            sistrlen = 0;

            while (RootSi) {
                Tstrlen = 0;
                if (RootSi->Tv) {
                    while (RootSi->Tv[Tstrlen] != '\0') {
                        Vsi[sistrlen] = RootSi->Tv[Tstrlen];
                        sistrlen++; Tstrlen++;
                    }
                }
                RootSi = RootSi->next;
            }
            Vsi[sistrlen] = '\0';
        }
    }

    return Vsi;
}

F* Ctags::getFtag() {

    F* fvs = (F*)malloc(sizeof(F));

    UINT8 reftag[6] = { 0 };
    UINT8 ttag[4] = { 0 };
    UINT8 sitag[5] = { 0 };

    UINT8* Tv = nullptr;
    UINT8* refv = nullptr;
    UINT8* siv = nullptr;
    UINT8* fv = nullptr;

    UINT32 flen = 0;

    while (data[p] != '>') {
        for (int j = 0; j < 5 - 1; j++) {
            reftag[j] = reftag[j + 1];
            if (j < 4 - 1)
                sitag[j] = sitag[j + 1];
            if (j < 3 - 1)
                ttag[j] = ttag[j + 1];
        }

        reftag[5 - 1] = sitag[4 - 1] = ttag[3 - 1] = data[p];
        reftag[5] = sitag[4] = ttag[3] = '\0';
        p++;

        if (strncmp((char const*)reftag, Fref, 5) == 0)// ref macth
            refv = getvalue();

        if (strncmp((char const*)sitag, Fsi, 4) == 0)// si macth
            siv = getvalue();

        if (strncmp((char const*)ttag, Ft, 2) == 0)// t macth
            Tv = getvalue();
    }

    if (data[p - 1] == '/') {
        //v no
    }
    else {
        p++;// > next
        fv = getVorftag(endFtag, 4, &flen);
    }

    fvs->ref = refv;
    fvs->si = siv;
    fvs->t = Tv;
    fvs->val = fv;

    return fvs;
}

UINT8* Ctags::getVorftag(const char* tag, UINT32 taglen, UINT32* size) {

    //memset(endFtag, 0, msize);
    UINT32 len = 0;
    UINT8* Vv = nullptr;

    while (data[p + len] != '<')
        len++;

    *size = len;//
    size_t l = (size_t)len + 1;

    Vv = (UINT8*)malloc(l);

    UINT32 i = 0;

    while (i < len) {
        Vv[i] = data[p];
        i++; p++;
    }
    Vv[len] = '\0';

    return Vv;
}

void Ctags::getCtag() {

    UINT8 endtag[5] = { 0 };// </c>
    UINT8 ttag[4] = { 0 };// t=" s=" r="　<v>
    UINT8 ftag[3] = { 0 };// <f

    UINT8* Tv = nullptr;
    UINT8* Sv = nullptr;
    UINT8* Vv = nullptr;
    UINT8* col = nullptr;
    UINT8* row = nullptr;
    F* fval = nullptr;
    Row* rn = nullptr;

    int endresult = 1;
    int result = 0;
    UINT32 collen = 0;
    UINT32 rowlen = 0;
    UINT32 colnum = 0; UINT32 rownum = 0;
    UINT32 vlen = 0;
    UINT32 tlen = 0;

    while (endresult != 0) {
        for (int j = 0; j < 4 - 1; j++) {
            endtag[j] = endtag[j + 1];
            if (j < 3 - 1)
                ttag[j] = ttag[j + 1];
            if (j < (2 - 1))
                ftag[j] = ftag[j + 1];
        }
        endtag[4 - 1] = ttag[3 - 1] = ftag[2 - 1] = data[p];
        endtag[4] = ttag[3] = ftag[2] = '\0';
        p++;

        endresult = strncmp((char const*)endtag, endC, 4);// </c>

        result = strncmp((char const*)ftag, slashend, 2);
        if (result == 0)
            endresult = 0;

        result = strncmp((char const*)ttag, Ft, 3);
        if (result == 0) {// t= match
            Tv = getvalue();
            tlen = stocklen;
        }

        result = strncmp((char const*)ttag, Rs, 3);
        if (result == 0)//s= get
            Sv = getvalue();

        if (strncmp((char const*)ttag, Rr, 3) == 0) {//r= get
            while (data[p - 1] != '"')
                p++;

            while (data[p + collen + rowlen] != '"') {//rowcol get
                if (data[p + collen + rowlen] > 64)//col
                    collen++;
                else//row
                    rowlen++;
            }

            UINT32 csize = collen + 1;
            UINT32 rsize = rowlen + 1;
            col = (UINT8*)malloc(csize);
            row = (UINT8*)malloc(rsize);

            for (UINT32 i = 0; i < collen; i++) {
                col[i] = data[p]; p++;
            }

            for (UINT32 i = 0; i < rowlen; i++) {
                row[i] = data[p]; p++;
            }
            col[collen] = '\0'; row[rowlen] = '\0';
            colnum = NA.ColumnArraytoNumber(col, collen);
            rownum = NA.RowArraytoNum(row, rowlen);

            free(col); free(row);
        }

        if (strncmp((char const*)ttag, Vtag, 3) == 0) // <v> get
            Vv = getVorftag(endVtag, 4, &vlen);

        if (strncmp((char const*)ftag, Ftag, 2) == 0) // <f get
            fval = getFtag();
    }

    UINT8* Vsi = nullptr;

    //t=s si in
    if (Tv) {
        if (Tv[0] == 's' && tlen == 1)
            Vsi = getSi(Vv, vlen);//si in
    }
    rn = searchRow(rows, rownum);

    if (rn) {
        rn->cells = addCtable(rn->cells, Tv, Sv, Vsi, colnum, Vv, fval);
    }
    else {
        //row nothing need make new row
    }
}

void Ctags::xmlheaderRead() {
    const char* worksheet = "<worksheet";//10

    UINT8 wo[11] = { 0 };
    int result = 1;

    while (result != 0) {
        for (int i = 0; i < 9; i++) {
            wo[i] = wo[i + 1];
        }
        wo[9] = data[p];
        wo[10] = '\0';
        p++;

        result = strncmp((char*)wo, worksheet, 10);
        if (result == 0) {
            readworksheet();
        }
    }
}

void Ctags::readworksheet() {
    const char* worstr[] = { "xmlns=\"","xmlns:r=\"","xmlns:mc=\"","xmlns:x14ac=\"","xmlns:xr=\"",
        "xmlns:xr2=\"","xmlns:xr3=\"","mc:Ignorable=\"","xr:uid=\"" };//7 9 10 13 10 11 11 14  8文字

    UINT8 xmln[8] = { 0 };
    UINT8 r[10] = { 0 };
    UINT8 mcxr[11] = { 0 };
    UINT8 x14[14] = { 0 };
    UINT8 xr23[12] = { 0 };
    UINT8 ign[15] = { 0 };
    UINT8 uid[9] = { 0 };

    UINT8* Xmln = nullptr;
    UINT8* R = nullptr;
    UINT8* Mc = nullptr;
    UINT8* X14 = nullptr;
    UINT8* Xr = nullptr;
    UINT8* Xr2 = nullptr;
    UINT8* Xr3 = nullptr;
    UINT8* Ign = nullptr;
    UINT8* UId = nullptr;

    while (data[p] != '>') {
        for (int j = 0; j < 14 - 1; j++) {
            ign[j] = ign[j + 1];
            if (j < 13 - 1)
                x14[j] = x14[j + 1];
            if (j < 11 - 1)
                xr23[j] = xr23[j + 1];
            if (j < 10 - 1)
                mcxr[j] = mcxr[j + 1];
            if (j < 9 - 1)
                r[j] = r[j + 1];
            if (j < 8 - 1)
                uid[j] = uid[j + 1];
            if (j < 7 - 1)
                xmln[j] = xmln[j + 1];
        }

        ign[14 - 1] = x14[13 - 1] = xr23[11 - 1] = mcxr[10 - 1] = r[9 - 1] = uid[8 - 1] = xmln[7 - 1] = data[p];
        ign[14] = x14[13] = xr23[11] = mcxr[10] = r[9] = uid[8] = xmln[7] = '\0';
        p++;

        if (strncmp((char const*)xmln, worstr[0], 7) == 0)
            Xmln = getvalue();

        if (strncmp((char const*)uid, worstr[8], 8) == 0)
            UId = getvalue();

        if (strncmp((char const*)r, worstr[1], 9) == 0)
            R = getvalue();

        if (strncmp((char const*)mcxr, worstr[4], 10) == 0)
            Xr = getvalue();

        if (strncmp((char const*)mcxr, worstr[2], 10) == 0)
            Mc = getvalue();

        if (strncmp((char const*)xr23, worstr[6], 11) == 0)
            Xr3 = getvalue();

        if (strncmp((char const*)xr23, worstr[5], 11) == 0)
            Xr2 = getvalue();

        if (strncmp((char const*)ign, worstr[7], 14) == 0)
            Ign = getvalue();

        if (strncmp((char const*)x14, worstr[3], 13) == 0)
            X14 = getvalue();
    }
    wsV = (worksheetV*)malloc(sizeof(worksheetV));
    wsV->Ignorable = Ign;
    wsV->r = R;
    wsV->xmlns = Xmln;
    wsV->uid = UId;
    wsV->xr = Xr;
    wsV->xr2 = Xr2;
    wsV->xr3 = Xr3;
    wsV->mc = Mc;
    wsV->x14ac = X14;
}

void Ctags::freeheadxms() {
    if (wsV) {
        free(wsV->Ignorable);
        free(wsV->r);
        free(wsV->xmlns);
        free(wsV->uid);
        free(wsV->xr);
        free(wsV->xr2);
        free(wsV->xr3);
        free(wsV->mc);
        free(wsV->x14ac);
    }
}

void Ctags::readsheetPr() {
    /*
    <sheetPr filterMode="1">
<tabColor rgb="FFFFFF00"/>
<pageSetUpPr fitToPage="1"/>
</sheetPr>

<sheetPr transitionEvaluation="1"/>
*/
    const char* tc[] = { "tint=\"","rgb=\"","theme=\"","filterMode=\"","transitionEvaluation=\"","codeName=\"" };//6 5 7 12 22 10
    const char* pagest = "fitToPage=\"";//11
    const char* shPrEnd = "</sheetPr>";//10

    UINT8 epr[7] = { 0 };
    UINT8 them[8] = { 0 };
    UINT8 rg[6] = { 0 };
    UINT8 pr[12] = { 0 };
    UINT8 pa[11] = { 0 };
    UINT8 fil[13] = { 0 };
    UINT8 tE[23] = { 0 };

    UINT8* tin = nullptr;
    UINT8* Rgb = nullptr;
    UINT8* fit = nullptr;
    UINT8* Tm = nullptr;
    UINT8* Filt = nullptr;
    UINT8* trEv = nullptr;
    UINT8* coN = nullptr;

    int resu = 1;
    int ere = 1;

    if (data[p] == '/') {

    }
    else {
        while (data[p] != '>') {
            for (int j = 0; j < 22 - 1; j++) {
                tE[j] = tE[j + 1];
                if (j < 12 - 1)
                    fil[j] = fil[j + 1];
                if (j < 10 - 1)
                    pa[j] = pa[j + 1];
            }
            tE[21] = fil[11] = data[p];
            tE[22] = fil[12] = '\0';
            p++;

            resu = strncmp((char const*)fil, tc[3], 12);
            if (resu == 0)
                Filt = getvalue();

            resu = strncmp((char const*)tE, tc[4], 22);
            if (resu == 0)
                trEv = getvalue();

            resu = strncmp((char const*)pa, tc[5], 10);
            if (resu == 0)
                coN = getvalue();
        }
        if (data[p - 1] == '/') {

        }
        else {
            while (ere != 0) {
                for (int j = 0; j < 12 - 1; j++) {
                    fil[j] = fil[j + 1];
                    if (j < 11 - 1)
                        pr[j] = pr[j + 1];
                    if (j < 10 - 1)
                        pa[j] = pa[j + 1];
                    if (j < 7 - 1)
                        them[j] = them[j + 1];
                    if (j < 6 - 1)
                        epr[j] = epr[j + 1];
                    if (j < 5 - 1)
                        rg[j] = rg[j + 1];
                }
                them[6] = fil[11] = pr[11 - 1] = pa[10 - 1] = epr[6 - 1] = rg[5 - 1] = data[p];
                them[7] = fil[12] = pr[11] = pa[10] = epr[6] = rg[5] = '\0';
                p++;

                resu = strncmp((char const*)pr, pagest, 11);
                if (resu == 0)
                    fit = getvalue();

                resu = strncmp((char const*)epr, tc[0], 6);
                if (resu == 0)
                    tin = getvalue();

                resu = strncmp((char const*)rg, tc[1], 5);
                if (resu == 0)
                    Rgb = getvalue();

                resu = strncmp((char const*)them, tc[2], 7);
                if (resu == 0)
                    Tm = getvalue();

                ere = strncmp((char const*)pa, shPrEnd, 10);
            }
        }

        Pr = (SheetPr*)malloc(sizeof(SheetPr));
        Pr->tint = tin;
        Pr->fitToPage = fit;
        Pr->rgb = Rgb;
        Pr->theme = Tm;
        Pr->filterMode = Filt;
        Pr->transitionEvaluation = trEv;
    }
}

void Ctags::freePr() {
    if (Pr) {
        free(Pr->tint);
        free(Pr->fitToPage);
        free(Pr->rgb);
        free(Pr->theme);
        free(Pr->filterMode);
        free(Pr->transitionEvaluation);
    }
}

// <sheetPr get
void Ctags::GetSheetPr() {
    /* <sheetPr>
<tabColor tint="0.4" theme="7"/>
<pageSetUpPr fitToPage="1"/>
</sheetPr>*/
    const char* shPr = "<sheetPr";//8    
    const char* diment = "<dimension ref=\"";//16

    UINT8 epr[17] = { 0 };
    UINT8 pr[9] = { 0 };
    p = 0;//sheetdata

    int ucode = 1;//dimention <sheetPr
    int result = 0;

    UINT8* Dim = nullptr;

    while (ucode != 0) {
        for (int j = 0; j < 16 - 1; j++) {
            epr[j] = epr[j + 1];
            if (j < 8 - 1)
                pr[j] = pr[j + 1];
        }
        epr[15] = pr[8 - 1] = data[p];
        p++;

        result = strncmp((char const*)pr, shPr, 8);
        if (result == 0)
            readsheetPr();

        ucode = strncmp((char const*)epr, diment, 16);
        if (ucode == 0) {
            Dim = getvalue();
            GetDiment(Dim);
        }
    }

}

//demention get
void Ctags::GetDiment(UINT8* d) {

    dm = (demention*)malloc(sizeof(demention));
    dm->sC = nullptr;
    dm->sR = nullptr;
    dm->eC = nullptr;//demention end 列
    dm->eR = nullptr;//demention end 行

    // <dimension ref="A1:EJ128"/>を検索
    size_t collen = 0;
    size_t rowlen = 0;
    size_t dimstrlen = 0;

    while (d[dimstrlen] != ':' && d[dimstrlen] != '\0') {//文字数確認
        if (d[dimstrlen] > 64)//列番号
            sClen++;
        else//行番号
            sRlen++;
        dimstrlen++;
    }

    dm->sC = (UINT8*)calloc(sClen + 1, sizeof(UINT8));//demention start 列
    dm->sR = (UINT8*)calloc(sRlen + 1, sizeof(UINT8));//demention start 行
    sClen = 0; sRlen = 0;
    for (size_t i = 0; i < dimstrlen; i++) {
        if (d[i] > 64) {
            dm->sC[sClen] = d[i];
            sClen++;
        }
        else {
            dm->sR[sRlen] = d[i];
            sRlen++;
        }
    }
    dm->sC[sClen] = '\0'; dm->sR[sRlen] = '\0';

    if (d[dimstrlen] == ':') {
        dimstrlen++;//':' 次へ
        size_t stp = dimstrlen;//後半スタート位置

        while (d[dimstrlen] != '\0') {//endcell
            if (d[dimstrlen] > 64) //列番号
                eClen++;
            else//行番号
                eRlen++;
            dimstrlen++;
        }
        dm->eC = (UINT8*)calloc(eClen + 1, sizeof(UINT8));//demention end 列
        dm->eR = (UINT8*)calloc(eRlen + 1, sizeof(UINT8));//demention end 行
        eClen = 0; eRlen = 0;
        for (size_t i = stp; i < (dimstrlen); i++) {
            if (d[i] > 64) {
                dm->eC[eClen] = d[i];
                eClen++;
            }
            else {
                dm->eR[eRlen] = d[i];
                eRlen++;
            }
        }
        dm->eC[eClen] = '\0'; dm->eR[eRlen] = '\0';

        maxcol = NA.ColumnArraytoNumber(dm->eC, eClen);//現max列
    }
}

Row* Ctags::addrows(Row* row, UINT32 r, UINT8* spanS, UINT8* spanE, UINT8* ht, UINT8* thickBot, UINT8* s, UINT8* customFormat, UINT8* customHeight, C* cell)
{
    if (!row) {
        row = (Row*)malloc(sizeof(Row));

        if (row) {
            row->s = s;
            row->spanS = spanS;
            row->spanE = spanE;
            row->ht = ht;
            row->thickBot = thickBot;
            row->r = r;
            row->customFormat = customFormat;
            row->customHeight = customHeight;
            row->cells = cell;
            row->next = nullptr;
        }
    }
    else if (r < row->r) {
        Row* newr = (Row*)malloc(sizeof(Row));

        //別メモリ保存
        newr->s = row->s;
        newr->spanS = row->spanS;
        newr->spanE = row->spanE;
        newr->ht = row->ht;
        newr->thickBot = row->thickBot;
        newr->r = row->r;
        newr->customFormat = row->customFormat;
        newr->customHeight = row->customHeight;
        newr->cells = row->cells;
        newr->next = row->next;

        row->s = s;
        row->spanS = spanS;
        row->spanE = spanE;
        row->ht = ht;
        row->thickBot = thickBot;
        row->r = r;
        row->customFormat = customFormat;
        row->customHeight = customHeight;
        row->cells = cell;
        row->next = newr;
    }
    else if (r == row->r) {//行番号同じ　更新
        row->s = s;
        row->spanS = spanS;
        row->spanE = spanE;
        row->ht = ht;
        row->thickBot = thickBot;
        row->r = r;
        row->customFormat = customFormat;
        row->customHeight = customHeight;
        row->cells = cell;
    }
    else {
        row->next = addrows(row->next, r, spanS, spanE, ht, thickBot, s, customFormat, customHeight, cell);
    }
    return row;

}

//row番号（文字列）でテーブル検索
Row* Ctags::searchRow(Row* r, UINT32 newrow) {

    Row* sr = r;

    while (sr) {
        if (newrow == sr->r) {
            return sr;
        }
        sr = sr->next;
    }

    sr = nullptr;
    return sr;
}

//row内容取得 配列に p >まで
void Ctags::Getrow() {

    UINT8 cusf[15] = { 0 };
    UINT8 rb[11] = { 0 };
    UINT8 spa[8] = { 0 };
    UINT8 ht[5] = { 0 };
    UINT8 rs[4] = { 0 };
    int vlen = 0; UINT32 rownum = 0;

    //値用
    UINT8* s = nullptr;
    UINT8* r = nullptr;
    UINT8* h = nullptr;
    UINT8* sps = nullptr;
    UINT8* spe = nullptr;
    UINT8* tb = nullptr;
    UINT8* ch = nullptr;
    UINT8* cf = nullptr;

    while (data[p] != '>') {
        for (int j = 0; j < 14 - 1; j++) {
            cusf[j] = cusf[j + 1];
            if (j < 10 - 1)
                rb[j] = rb[j + 1];
            if (j < 7 - 1)
                spa[j] = spa[j + 1];
            if (j < 4 - 1)
                ht[j] = ht[j + 1];
            if (j < 3 - 1)
                rs[j] = rs[j + 1];
        }

        cusf[14 - 1] = rb[10 - 1] = spa[7 - 1] = ht[4 - 1] = rs[3 - 1] = data[p];
        rs[3] = '\0';
        p++;

        if (strncmp((char const*)rs, Rs, 3) == 0 && strncmp((char const*)spa, Rspans, 7) != 0)
            s = getvalue();

        if (strncmp((char const*)rs, Rr, 3) == 0) {
            r = getvalue();
            rownum = NA.RowArraytoNum(r, stocklen);
            free(r);
        }

        if (strncmp((char const*)ht, Rht, 4) == 0 && strncmp((char const*)cusf, RcustomHeight, 14) != 0)
            h = getvalue();

        if (strncmp((char const*)spa, Rspans, 7) == 0) {
            vlen = 0;
            while (data[p + vlen] != ':')//span 最初
                vlen++;

            UINT32 vsize = (UINT32)vlen + 1;
            sps = (UINT8*)malloc(vsize);
            for (int i = 0; i < vlen; i++) {
                sps[i] = data[p];
                p++;
            }
            sps[vlen] = '\0';
            p++;

            vlen = 0;

            while (data[p + vlen] != '"')//span 終わり
                vlen++;

            vsize = (UINT32)vlen + 1;
            spe = (UINT8*)malloc(vsize);
            for (int i = 0; i < vlen; i++) {
                spe[i] = data[p];
                p++;
            }

            spe[vlen] = '\0';
            //std::cout << " span : " << sps << " : " << spe << std::endl;
        }

        if (strncmp((char const*)rb, RthickBot, 10) == 0)
            tb = getvalue();

        if (strncmp((char const*)cusf, RcustomHeight, 14) == 0)
            ch = getvalue();

        if (strncmp((char const*)cusf, RcustomFormat, 14) == 0)
            cf = getvalue();
    }
    rows = addrows(rows, rownum, sps, spe, h, tb, s, cf, ch, nullptr);
}

void Ctags::readsvValues() {
    /* <sheetView tabSelected="1" workbookViewId="0"/>変更必要 */
    const char* shview[] = { "zoomScaleNormal=\"","workbookViewId=\"","tabSelected=\"","<pane","<selection " };
    const char* endtag = "</sheetView>";//12
    //17 16 13 16 6 11
    UINT8 zsn[18] = { 0 };
    UINT8 wbV[17] = { 0 };
    UINT8 tbs[14] = { 0 };
    UINT8 pan[6] = { 0 };
    UINT8 sel[12] = { 0 };
    UINT8 clo[13] = { 0 };

    UINT8* zSN = nullptr;
    UINT8* WbV = nullptr;
    UINT8* tS = nullptr;
    UINT8* wVI = nullptr;
    Pane* pa = nullptr;
    selection* Sele = nullptr;

    int res = 1;

    while (data[p] != '>') {
        if (data[p] == '/') {
            break;
        }

        for (int j = 0; j < 17 - 1; j++) {
            zsn[j] = zsn[j + 1];
            if (13 - 1 > j)
                tbs[j] = tbs[j + 1];
            if (16 - 1 > j)
                wbV[j] = wbV[j + 1];
        }
        wbV[16 - 1] = tbs[13 - 1] = zsn[17 - 1] = data[p];
        p++;

        if (strncmp((char const*)zsn, shview[0], 17) == 0)
            zSN = getvalue();

        if (strncmp((char const*)wbV, shview[1], 16) == 0)
            WbV = getvalue();

        if (strncmp((char const*)tbs, shview[2], 13) == 0)
            tS = getvalue();
    }

    if (data[p] != '/') {
        while (res != 0) {
            for (int j = 0; j < 12 - 1; j++) {
                clo[j] = clo[j + 1];
                if (5 - 1 > j)
                    pan[j] = pan[j + 1];
                if (11 - 1 > j)
                    sel[j] = sel[j + 1];
            }
            pan[5 - 1] = sel[11 - 1] = clo[12 - 1] = data[p];
            p++;

            if (strncmp((char const*)pan, shview[3], 5) == 0)
                pa = GetPane(pa);

            if (strncmp((char const*)sel, shview[4], 11) == 0)
                Sele = getselection(Sele);

            res = strncmp((char const*)clo, endtag, 12);
        }
    }

    ShV = (SheetViews*)malloc(sizeof(SheetViews));
    ShV->pan = pa;
    ShV->selec = Sele;
    ShV->tabSelected = tS;
    ShV->workbookViewId = WbV;
    ShV->zoomScaleNormal = zSN;
}

void Ctags::freesheetviews() {
    if (ShV) {
        free(ShV->pan);
        free(ShV->selec);
        free(ShV->tabSelected);
        free(ShV->workbookViewId);
        free(ShV->zoomScaleNormal);
    }
}
//SheetViews 検索
void Ctags::GetSheetViews() {

    const char* startSV = "<sheetViews>";//12

    UINT8 PN[13] = { 0 };

    int PNlen = 0;
    int res = 1;
    int panetagres = 0;

    while (res != 0)
    {//diment /> to <sheetView まで
        for (int j = 0; j < 12 - 1; j++) {//文字数カウント
            PN[j] = PN[j + 1];
        }
        PN[12 - 1] = data[p];//最後に付け加える
        p++;

        res = strncmp((char const*)PN, startSV, 12);
        if (res == 0)
            readsheetView();
    }
}
//sheetview 検索
void Ctags::readsheetView() {
    const char* startSV = "<sheetView";//10

    UINT8 PN[11] = { 0 };

    int PNlen = 0;
    int res = 1;
    int panetagres = 0;

    while (res != 0)
    {//diment /> to <sheetView まで
        for (int j = 0; j < 10 - 1; j++) {//文字数カウント
            PN[j] = PN[j + 1];
        }
        PN[10 - 1] = data[p];//最後に付け加える
        p++;

        res = strncmp((char const*)PN, startSV, 10);
        if (res == 0)
            readsvValues();
    }
}

selection* Ctags::getselection(selection* seLe) {
    /*
    <selection/>
    <selection pane="topRight"/>
    <selection pane="bottomLeft"/>
    <selection pane="bottomRight" sqref="B191" activeCell="B191"/>
    */
    UINT8 at[13] = { 0 };
    UINT8 pn[7] = { 0 };
    UINT8 sr[8] = { 0 };

    const char* ecp = "pane=\"";//6
    const char* active = "activeCell=\"";//12
    const char* sqref = "sqref=\"";//7

    UINT8* pa = nullptr;
    UINT8* ac = nullptr;
    UINT8* sq = nullptr;
    int panelen = 0;
    int activeCelllen = 0;
    int sqreflen = 0;

    if (data[p] == '/') {//tag終了
        //sct = SLTaddtable(sct, pa, ac, sq);
        return nullptr;
    }
    else {
        while (data[p] != '>') {
            for (int j = 0; j < 12 - 1; j++) {
                at[j] = at[j + 1];//active
                if (j < (6 - 1))
                    pn[j] = pn[j + 1];//pane
                if (j < (7 - 1))
                    sr[j] = sr[j + 1];//sqref
            }
            sr[7 - 1] = pn[6 - 1] = at[12 - 1] = data[p];
            p++;

            if (strncmp((char const*)pn, ecp, 6) == 0) {//pane="
                pa = getvalue();
            }

            if (strncmp((char const*)at, active, 12) == 0) {//activeCell="
                ac = getvalue();
            }

            if (strncmp((char const*)sr, sqref, 7) == 0) {//sqref="
                sq = getvalue();
            }
        }
    }
    seLe = SLTaddtable(seLe, pa, ac, sq);//構造体へコピー

    return seLe;
}

Pane* Ctags::GetPane(Pane* pa) {

    const char* PaneTags[] = { "xSplit=\"","ySplit=\"","topLeftCell=\"","activePane=\"","state=\"" };

    //8 8 13 12 7
    UINT8 XYs[8] = { 0 };
    UINT8 tpl[13] = { 0 };
    UINT8 acP[12] = { 0 };
    UINT8 stt[7] = { 0 };
    UINT8* X; UINT8* Y;
    UINT8* tL; UINT8* ap;
    UINT8* s;

    X = nullptr; Y = nullptr; tL = nullptr;
    ap = nullptr; s = nullptr;

    while (data[p] != '>') {
        for (int j = 0; j < 13 - 1; j++) {
            tpl[j] = tpl[j + 1];//topLeftCell

            if (j < (12 - 1))
                acP[j] = acP[j + 1];//activePane

            if (j < (8 - 1))
                XYs[j] = XYs[j + 1];//x,ysplit

            if (j < (7 - 1))
                stt[j] = stt[j + 1];//state=
        }

        tpl[13 - 1] = acP[12 - 1] = XYs[8 - 1] = stt[7 - 1] = data[p];
        p++;

        if ((strncmp((const char*)tpl, PaneTags[2], 13)) == 0)
            tL = getvalue();

        if ((strncmp((const char*)acP, PaneTags[3], 12)) == 0)
            ap = getvalue();

        if ((strncmp((const char*)XYs, PaneTags[0], 8)) == 0)
            X = getvalue();

        if ((strncmp((const char*)XYs, PaneTags[1], 8)) == 0)
            Y = getvalue();

        if ((strncmp((const char*)stt, PaneTags[4], 7)) == 0)
            s = getvalue();
    }

    pa = addpanetable(pa, X, Y, tL, ap, s);

    return pa;
}

Pane* Ctags::addpanetable(Pane* p, UINT8* x, UINT8* y, UINT8* tl, UINT8* ap, UINT8* sta) {

    if (!p) {
        p = (Pane*)malloc(sizeof(Pane));
        if (p) {
            p->xSp = x;
            p->ySp = y;
            p->tLeftC = tl;
            p->activP = ap;
            p->state = sta;
            p->next = nullptr;
        }
    }
    else {
        p->next = addpanetable(p->next, x, y, tl, ap, sta);
    }

    return p;
}

selection* Ctags::SLTaddtable(selection* s, UINT8* pv, UINT8* av, UINT8* sv) {

    if (!s) {
        s = (selection*)malloc(sizeof(selection));
        s->p = pv;
        s->a = av;
        s->s = sv;
        s->next = nullptr;
    }
    else {
        s->next = SLTaddtable(s->next, pv, av, sv);
    }

    return s;
}

void Ctags::getcolv() {
    UINT8 mimax[5] = { 0 };
    UINT8 wist[7] = { 0 };
    UINT8 ccw[13] = { 0 };
    UINT8 cbf[9] = { 0 };
    UINT8 chid[8] = { 0 };

    unsigned char* min = nullptr;
    unsigned char* max = nullptr;
    unsigned char* style = nullptr;
    unsigned char* bestf = nullptr;
    unsigned char* hidd = nullptr;
    unsigned char* colw = nullptr;
    unsigned char* cuw = nullptr;

    while (data[p] != '>') {
        for (int j = 0; j < 13 - 1; j++) {
            ccw[j] = ccw[j + 1];

            if (j < (9 - 1))
                cbf[j] = cbf[j + 1];//bestFit
            if (j < (8 - 1))
                chid[j] = chid[j + 1];//hidden
            if (j < (7 - 1))
                wist[j] = wist[j + 1];//style width
            if (j < (5 - 1))
                mimax[j] = mimax[j + 1];//min max
        }

        ccw[13 - 1] = cbf[9 - 1] = chid[8 - 1] = wist[7 - 1] = mimax[5 - 1] = data[p];
        p++;

        int compere = strncmp((const char*)ccw, (const char*)ColcW, 13);

        if (compere == 0)
            cuw = getvalue();

        compere = strncmp((const char*)cbf, (const char*)Colbf, 9);
        if (compere == 0)//bestFit get
            bestf = getvalue();

        compere = strncmp((const char*)chid, (const char*)Colhid, 8);
        if (compere == 0)//hidden get
            hidd = getvalue();

        compere = strncmp((const char*)wist, (const char*)ColS, 7);
        if (compere == 0)//style get
            style = getvalue();

        compere = strncmp((const char*)wist, (const char*)Colswidth, 7);
        if (compere == 0)//width get
            colw = getvalue();

        compere = strncmp((const char*)mimax, (const char*)Colmax, 5);
        if (compere == 0)//width get
            max = getvalue();

        compere = strncmp((const char*)mimax, Colmin, 5);
        if (compere == 0)//width get
            min = getvalue();
    }

    cls = addcolatyle(cls, nullptr, min, max, colw, style, hidd, bestf, cuw);

}

void Ctags::getsheetFormatPr() {
    const char* shfpr[] = { "defaultRowHeight=\"","defaultColWidth=\"","x14ac:dyDescent=\"" };//18 17 17

    UINT8 dr[19] = { 0 };
    UINT8 dc[18] = { 0 };

    UINT8* dRH = nullptr;
    UINT8* dCW = nullptr;
    UINT8* x14 = nullptr;

    if (data[p] == '/') {

    }
    else {
        while (data[p] != '>') {
            for (int j = 0; j < 18 - 1; j++) {
                dr[j] = dr[j + 1];
                if (j < 17 - 1)
                    dc[j] = dc[j + 1];
            }

            dr[18 - 1] = dc[17 - 1] = data[p];
            dr[18] = dc[17] = '\0';
            p++;//位置移動

            if (strncmp((char const*)dr, shfpr[0], 18) == 0)// ref macth
                dRH = getvalue();

            if (strncmp((char const*)dc, shfpr[1], 17) == 0)// si macth
                dCW = getvalue();

            if (strncmp((char const*)dc, shfpr[2], 17) == 0)// t macth
                x14 = getvalue();
        }
    }
    sfopr = (sheetFPr*)malloc(sizeof(sheetFPr));
    sfopr->dColW = dCW;
    sfopr->dRowH = dRH;
    sfopr->dD = x14;
}

void Ctags::freesheetFormatPr() {
    if (sfopr) {
        free(sfopr->dColW);
        free(sfopr->dRowH);
        free(sfopr->dD);
    }
}
//Col tag and sheetformatPrの検索 sheetdataまで
void Ctags::Getcols() {
    const char* shFpr = "<sheetFormatPr";//14文字
    //colない時あるので<sheetData>はじまりまで読む
    const char* startcelld = "<sheetData";//10
    const char* Coltag = "<col ";//5

    UINT8 search[7] = { 0 };
    UINT8 Scoltag[5] = { 0 };
    UINT8 sfpr[15] = { 0 };
    UINT8 ent[11] = { 0 };

    int result = 0;
    int endresult = 1;
    int sfplen = 0;

    while (endresult != 0) {

        for (int j = 0; j < 14 - 1; j++) {
            sfpr[j] = sfpr[j + 1];
            if (j < (10 - 1))
                ent[j] = ent[j + 1];
            if (j < (5 - 1))
                Scoltag[j] = Scoltag[j + 1];
        }
        ent[9] = sfpr[14 - 1] = Scoltag[5 - 1] = data[p];
        p++;

        result = strncmp((char const*)sfpr, shFpr, 14);
        if (result == 0)
            getsheetFormatPr();

        result = strncmp((char const*)Scoltag, Coltag, 5);
        if (result == 0)
            getcolv();

        endresult = strncmp((char const*)ent, startcelld, 10);
    }
}

cols* Ctags::addcolatyle(cols* cs, cols* csP, UINT8* min, UINT8* max, UINT8* W, UINT8* sty, UINT8* hid, UINT8* bF, UINT8* cuW) {

    if (!cs) {
        cs = (cols*)malloc(sizeof(cols));
        cs->min = min;
        cs->max = max;
        cs->width = W;
        cs->style = sty;
        cs->hidden = hid;
        cs->bestffit = bF;
        cs->customwidth = cuW;
        cs->next = nullptr;
        cs->parrent = csP;
    }
    else
        cs->next = addcolatyle(cs->next, cs, min, max, W, sty, hid, bF, cuW);

    return cs;
}

condiFormat* Ctags::addcondiFormat(condiFormat* cf, UINT8* sq, cfRule* cr) {
    if (!cf) {
        cf = (condiFormat*)malloc(sizeof(condiFormat));
        cf->cfR = cr;
        cf->sqref = sq;
        cf->next = nullptr;
    }
    else {
        cf->next = addcondiFormat(cf->next, sq, cr);
    }
    return cf;
}

cfvo* addcfvo(cfvo* cfv, UINT8* t, UINT8* v) {
    if (!cfv) {
        cfv = (cfvo*)malloc(sizeof(cfvo));
        cfv->type = t;
        cfv->val = v;
        cfv->next = nullptr;
    }
    else {
        cfv->next = addcfvo(cfv->next, t, v);
    }

    return cfv;
}

cfcolor* addcfcolor(cfcolor* cfc, UINT8* t, UINT8* r) {
    if (!cfc) {
        cfc = (cfcolor*)malloc(sizeof(cfcolor));
        cfc->theme = t;
        cfc->rgb = r;
        cfc->next = nullptr;
    }
    else {
        cfc->next = addcfcolor(cfc->next, t, r);
    }
    return cfc;
}
//cfvo read
cfvo* Ctags::readcfvo(cfvo* cv) {
    const char* cfru[] = { "type=\"","val=\"" };//6 5

    UINT8 typ[7] = { 0 };
    UINT8 val[6] = { 0 };

    UINT8* Typ = nullptr;
    UINT8* Va = nullptr;

    int result = 0;

    if (data[p] == '/') {
        cv = nullptr;
    }
    else {
        while (data[p] != '>') {
            for (int j = 0; j < 6 - 1; j++) {
                typ[j] = typ[j + 1];
                if (j < (5 - 1))
                    val[j] = val[j + 1];
            }
            typ[6 - 1] = val[5 - 1] = data[p];
            p++;

            result = strncmp((char const*)typ, cfru[0], 6);
            if (result == 0)
                Typ = getvalue();

            result = strncmp((char const*)val, cfru[1], 5);
            if (result == 0)
                Va = getvalue();
        }
    }
    cv = addcfvo(cv, Typ, Va);
    return cv;
}
//read cfcolor
cfcolor* Ctags::readcfcol(cfcolor* cco) {
    const char* cfru[] = { "rgb=\"","theme=\"" };//5 7 6

    UINT8 the[8] = { 0 };
    UINT8 rgb[6] = { 0 };

    UINT8* Them = nullptr;
    UINT8* Rgb = nullptr;

    int result = 0;

    while (data[p] != '>') {
        for (int j = 0; j < 7 - 1; j++) {
            the[j] = the[j + 1];
            if (j < (5 - 1))
                rgb[j] = rgb[j + 1];
        }
        the[7 - 1] = rgb[5 - 1] = data[p];
        p++;

        result = strncmp((char const*)the, cfru[1], 7);
        if (result == 0)
            Them = getvalue();

        result = strncmp((char const*)rgb, cfru[0], 5);
        if (result == 0)
            Rgb = getvalue();
    }

    cco = addcfcolor(cco, Them, Rgb);

    return cco;
}

cfRule* Ctags::addcfRule(cfRule* cr, UINT8* t, UINT8* dx, UINT8* pri, UINT8* op, UINT8* fo, cfvo* cf, cfcolor* cc) {
    if (!cr) {
        cr = (cfRule*)malloc(sizeof(cfRule));
        cr->type = t;
        cr->dxfId = dx;
        cr->priority = pri;
        cr->opara = op;
        cr->formula = fo;
        cr->vo = cf;
        cr->ccolor = cc;
        cr->next = nullptr;
    }
    else {
        cr->next = addcfRule(cr->next, t, dx, pri, op, fo, cf, cc);
    }
    return cr;
}

cfRule* Ctags::readcfRule(cfRule* cfr) {
    const char* cfru[] = { "type=\"","priority=\"","dxfId=\"","operator=\"","<formula>","<cfvo","<color " };//6 10 7 10 9 5 7
    const char* etag = "</cfRule>";//9

    UINT8 typ[7] = { 0 };
    UINT8 prio[11] = { 0 };
    UINT8 dxf[8] = { 0 };
    UINT8 formu[10] = { 0 };
    UINT8 cfv[6] = { 0 };

    UINT8* Typ = nullptr;
    UINT8* Prio = nullptr;
    UINT8* dxId = nullptr;
    UINT8* Oper = nullptr;
    UINT8* Fomu = nullptr;
    cfvo* fvo = nullptr;
    cfcolor* fcol = nullptr;

    int result = 0;
    int endresult = 1;

    while (endresult != 0) {

        for (int j = 0; j < 10 - 1; j++) {
            prio[j] = prio[j + 1];
            if (j < (9 - 1))
                formu[j] = formu[j + 1];
            if (j < (7 - 1))
                dxf[j] = dxf[j + 1];
            if (j < (6 - 1))
                typ[j] = typ[j + 1];
            if (j < (5 - 1))
                cfv[j] = cfv[j + 1];
        }
        prio[10 - 1] = formu[9 - 1] = dxf[7 - 1] = typ[6 - 1] = cfv[5 - 1] = data[p];
        p++;

        result = strncmp((char const*)typ, cfru[0], 6);
        if (result == 0)
            Typ = getvalue();

        result = strncmp((char const*)prio, cfru[1], 10);
        if (result == 0)
            Prio = getvalue();

        result = strncmp((char const*)dxf, cfru[2], 7);
        if (result == 0)
            dxId = getvalue();

        result = strncmp((char const*)prio, cfru[3], 10);
        if (result == 0)
            Oper = getvalue();

        result = strncmp((char const*)formu, cfru[4], 9);
        if (result == 0)
            Fomu = readformula();

        result = strncmp((char const*)cfv, cfru[5], 5);
        if (result == 0)
            fvo = readcfvo(fvo);

        result = strncmp((char const*)dxf, cfru[6], 7);
        if (result == 0)
            fcol = readcfcol(fcol);

        endresult = strncmp((char const*)formu, etag, 9);
    }
    cfr = addcfRule(cfr, Typ, dxId, Prio, Oper, Fomu, fvo, fcol);

    return cfr;
}

UINT8* Ctags::readformula() {
    if (data[p] == '/')
        return nullptr;

    size_t forlen = 0;
    while (data[p + forlen] != '<')
        forlen++;

    UINT8* formu = (UINT8*)malloc(forlen + 1);

    for (size_t fo = 0; fo < forlen; fo++) {
        formu[fo] = data[p + fo];
    }
    formu[forlen] = '\0';

    return formu;
}
//conditionalFormatting read
void Ctags::readconditional() {
    const char* crule[] = { "<cfRule","</cfRule>","</conditionalFormatting>","sqref=\"" };//7 9 24 7

    UINT8 cfr[8] = { 0 };
    UINT8 ecfr[10] = { 0 };
    UINT8 etag[25] = { 0 };

    UINT8* Sqr = nullptr;
    cfRule* cfR = nullptr;

    int result = 0;
    int endresult = 1;

    while (endresult != 0) {

        for (int j = 0; j < 24 - 1; j++) {
            etag[j] = etag[j + 1];
            if (j < (9 - 1))
                ecfr[j] = ecfr[j + 1];
            if (j < (7 - 1))
                cfr[j] = cfr[j + 1];
        }
        etag[24 - 1] = ecfr[9 - 1] = cfr[7 - 1] = data[p];
        p++;

        result = strncmp((char const*)cfr, crule[3], 7);
        if (result == 0)
            Sqr = getvalue();

        result = strncmp((char const*)cfr, crule[0], 7);
        if (result == 0)
            cfR = readcfRule(cfR);

        endresult = strncmp((char const*)etag, crule[2], 24);
    }

    condF = addcondiFormat(condF, Sqr, cfR);
}

void Ctags::freeconditional() {
    condiFormat* p;
    while (condF) {
        p = condF->next;
        free(condF->sqref);
        freecfr(condF->cfR);
        condF = p;
    }
}
void Ctags::freecfr(cfRule* c) {
    cfRule* p;
    while (c) {
        p = c->next;
        free(c->formula);
        free(c->opara);
        free(c->priority);
        free(c->type);
        free(c->dxfId);
        freeVo(c->vo);
        freecfcolor(c->ccolor);
        c = p;
    }
}
void Ctags::freeVo(cfvo* cf) {
    cfvo* p;
    while (cf) {
        p = cf->next;
        free(cf->type);
        free(cf->val);
        cf = p;
    }
}
void  Ctags::freecfcolor(cfcolor* c) {
    cfcolor* p;
    while (c) {
        p = c->next;
        free(c->rgb);
        free(c->theme);
        c = p;
    }
}

void Ctags::readPageSetup() {
    const char* setval[] = { "paperSize=\"","orientation=\"","r:id=\"" };//11 13 6
    UINT8 ps[12] = { 0 };
    UINT8 ori[14] = { 0 };
    UINT8 rid[7] = { 0 };

    UINT8* pSi = nullptr;
    UINT8* Ori = nullptr;
    UINT8* RId = nullptr;

    int result = 1;

    while (data[p] != '>') {
        for (int j = 0; j < 13 - 1; j++) {
            ori[j] = ori[j + 1];
            if (j < (11 - 1))
                ps[j] = ps[j + 1];
            if (j < (6 - 1))
                rid[j] = rid[j + 1];
        }
        ori[12] = ps[11 - 1] = rid[6 - 1] = data[p];
        p++;

        result = strncmp((const char*)ps, setval[0], 11);//papersiZe
        if (result == 0)
            pSi = getvalue();

        result = strncmp((const char*)ori, setval[1], 13);//
        if (result == 0)
            Ori = getvalue();

        result = strncmp((const char*)rid, setval[2], 6);//
        if (result == 0)
            RId = getvalue();
    }

    pasetup = (pagesetup*)malloc(sizeof(pagesetup));
    pasetup->orientation = Ori;
    pasetup->paperSize = pSi;
    pasetup->rid = RId;

}

void Ctags::freepagesetup() {
    if (pasetup) {
        free(pasetup->orientation);
        free(pasetup->paperSize);
        free(pasetup->rid);
    }
}

void Ctags::readPageMargin() {
    const char* setval[] = { "left=\"","right=\"","top=\"","bottom=\"","header=\"","footer=\"" };//6 7 5 8 8 8

    UINT8 l[7] = { 0 };
    UINT8 r[8] = { 0 };
    UINT8 t[6] = { 0 };
    UINT8 bo[9] = { 0 };

    UINT8* Le = nullptr;
    UINT8* Ri = nullptr;
    UINT8* To = nullptr;
    UINT8* Bo = nullptr;
    UINT8* He = nullptr;
    UINT8* Fo = nullptr;

    int result = 1;

    while (result != 0) {
        for (int j = 0; j < 8 - 1; j++) {
            bo[j] = bo[j + 1];
            if (j < (7 - 1))
                r[j] = r[j + 1];
            if (j < (6 - 1))
                l[j] = l[j + 1];
            if (j < (5 - 1))
                t[j] = t[j + 1];
        }
        bo[7] = r[7 - 1] = l[6 - 1] = t[5 - 1] = data[p];
        p++;

        result = strncmp((const char*)l, setval[0], 6);//papersiZe
        if (result == 0)
            Le = getvalue();

        result = strncmp((const char*)r, setval[1], 7);//
        if (result == 0)
            Ri = getvalue();

        result = strncmp((const char*)t, setval[2], 5);//
        if (result == 0)
            To = getvalue();

        result = strncmp((const char*)bo, setval[3], 8);//
        if (result == 0)
            Bo = getvalue();

        result = strncmp((const char*)bo, setval[4], 8);//
        if (result == 0)
            He = getvalue();

        result = strncmp((const char*)bo, setval[5], 8);//
        if (result == 0)
            Fo = getvalue();
    }

    pmargin = (pagemargin*)malloc(sizeof(pagemargin));
    pmargin->left = Le;
    pmargin->right = Ri;
    pmargin->top = To;
    pmargin->bottom = Bo;
    pmargin->header = He;
    pmargin->footer = Fo;
}
void Ctags::freepagemargin() {
    if (pmargin) {
        free(pmargin->left);
        free(pmargin->right);
        free(pmargin->top);
        free(pmargin->bottom);
        free(pmargin->header);
        free(pmargin->footer);
    }
}
// last string copy
void Ctags::getfinalstr() {

    UINT32 s = UINT32(dlen) - UINT32(p);
    UINT64 i = 0;
    const char* margeinfo = "<mergeCells count=\"";//19char
    const char* marge = "<mergeCell";//10char
    const char* endtag = "</worksheet>";//12char
    const char* conditionary = "<conditionalFormatting";//22
    const char* lasttags[] = { "<phoneticPr","<pageMargins","<pageSetup","<drawing r:id=\"" };//11 12 10 15

    UINT8 sm[20] = { 0 };
    UINT8 Sm[11] = { 0 };
    UINT8 cf[23] = { 0 };
    UINT8 Ent[13] = { 0 };
    UINT8 pPr[12] = { 0 };
    UINT8 Dr[16] = { 0 };

    int result = 0;
    int mresult = 0;

    do
    {
        for (int j = 0; j < 22 - 1; j++) {
            cf[j] = cf[j + 1];
            if (j < (19 - 1))
                sm[j] = sm[j + 1];
            if (j < (12 - 1))
                Ent[j] = Ent[j + 1];
            if (j < (11 - 1))
                pPr[j] = pPr[j + 1];
            if (j < (10 - 1))
                Sm[j] = Sm[j + 1];
            if (j < (15 - 1))
                Dr[j] = Dr[j + 1];
        }
        Dr[14] = cf[21] = Ent[11] = Sm[10 - 1] = sm[19 - 1] = data[p];
        p++;

        result = strncmp((const char*)Sm, marge, 10);//marge cell
        if (result == 0)
            getMargeCell();

        result = strncmp((const char*)Dr, lasttags[3], 15);
        if (result == 0)
            drawing_id = getvalue();

        result = strncmp((const char*)Sm, lasttags[2], 10);
        if (result == 0)
            readPageSetup();

        result = strncmp((const char*)Ent, lasttags[1], 12);
        if (result == 0)
            readPageMargin();

        result = strncmp((const char*)pPr, lasttags[0], 11);
        if (result == 0)
            phoneticPr_fontId = getvalue();

        result = strncmp((const char*)cf, conditionary, 22);
        if (result == 0)// <conditionalFormatting
            readconditional();

        result = strncmp((const char*)Ent, endtag, 12);//end worksheet

    } while (result != 0);
}

MargeCell* Ctags::addmargecell(MargeCell* m, UINT8* s, UINT8* e) {
    if (!m) {
        m = (MargeCell*)malloc(sizeof(MargeCell));
        m->scell = s;
        m->ecell = e;
        m->next = nullptr;
    }
    else
        m->next = addmargecell(m->next, s, e);

    return m;
}

void Ctags::getMargeCell() {
    const char ref[] = "ref=\"";//5文字
    const char* cou = "count=\"";//7文字
    const char* et = "</mergeCells>";//13文字

    UINT8 Ref[6] = { 0 };
    UINT8 Co[8] = { 0 };
    UINT8 Et[14] = { 0 };
    int result = 0;
    int eres = 1;

    UINT8* marcell = nullptr;
    UINT8** spl = nullptr;
    UINT8* stacell = nullptr;
    UINT8* endcell = nullptr;

    while (eres != 0) {
        for (int j = 0; j < 13 - 1; j++) {
            Et[j] = Et[j + 1];
            if (j < 6)
                Co[j] = Co[j + 1];
            if (j < 4)
                Ref[j] = Ref[j + 1];
        }
        Et[12] = Co[6] = Ref[5 - 1] = data[p];
        p++;

        result = strncmp((const char*)Co, cou, 7);
        if (result == 0)
            margeCellCount = getvalue();

        result = strncmp((const char*)Ref, ref, 5);
        if (result == 0) {//marge cell
            marcell = getvalue();
            spl = splitref(marcell);//マージセル文字分ける 
            margeCellRoot = addmargecell(margeCellRoot, spl[0], spl[1]);
        }

        eres = strncmp((const char*)Et, et, 13);
    }
}

void Ctags::freemarge() {
    MargeCell* p;
    while (margeCellRoot) {
        p = margeCellRoot->next;
        free(margeCellRoot->scell);
        free(margeCellRoot->ecell);
        margeCellRoot = p;
    }
}

UINT8** Ctags::splitref(UINT8* s) {

    UINT8** marcell = (UINT8**)malloc(sizeof(UINT8*) * 2);
    UINT8* stacell = nullptr;
    UINT8* endcell = nullptr;

    //文字分ける
    size_t mlen = 0;
    while (s[mlen] != ':' && s[mlen] != '\0')
        mlen++;

    if (s[mlen] != ':') {
        marcell[0] = s;
        marcell[1] = nullptr;
        return marcell;
    }
    else {
        stacell = (UINT8*)malloc(mlen + 1);
        for (size_t i = 0; i < mlen; i++)
            stacell[i] = s[i];
        stacell[mlen] = '\0';

        mlen++;//:skip

        size_t  elen = mlen;//後半スタート位置

        while (s[mlen] != '\0')
            mlen++;

        size_t memsiz = mlen - elen;

        endcell = (UINT8*)malloc(memsiz + 1);

        for (size_t i = 0; i < memsiz; i++) {
            endcell[i] = s[elen];
            elen++;
        }
        endcell[memsiz] = '\0';

        marcell[0] = stacell;
        marcell[1] = endcell;
        free(s);
    }
    return marcell;
}

void Ctags::Ctablefree(C* c) {
    C* q = nullptr;
    C* rc = nullptr;
    rc = c;

    while (rc) {
        q = rc->next;
        free(rc->val);
        free(rc->t);
        free(rc->s);
        free(rc->si);

        if (rc->f) {
            free(rc->f->ref);
            free(rc->f->si);
            free(rc->f->t);
            free(rc->f->val);
        }
        free(rc->f);
        free(rc);

        rc = q;
    }
}

void Ctags::Rowtablefree() {

    Row* q;

    while (rows) {
        Ctablefree(rows->cells);
        //free(rows->cells);
        q = rows->next;
        free(rows->customFormat);
        free(rows->s);
        free(rows->customHeight);
        free(rows->ht);
        free(rows->spanE);
        free(rows->spanS);
        free(rows->thickBot);

        free(rows);

        rows = q;
    }
}

void Ctags::selectfree() {
    selection* q;
    while (sct) {
        q = sct->next;

        free(sct->a);
        free(sct->p);
        free(sct->s);
        free(sct);

        sct = q;
    }
}

void Ctags::colfree() {

    cols* q;

    while (cls) {

        q = cls->next;
        free(cls->bestffit);
        free(cls->customwidth);
        free(cls->hidden);
        free(cls->max);
        free(cls->min);
        free(cls->style);
        free(cls->width);
        free(cls);

        cls = q;
    }
}

void Ctags::panefree() {

    Pane* q;

    while (Panes) {
        q = Panes->next;
        free(Panes->activP);
        free(Panes->state);
        free(Panes->tLeftC);
        free(Panes->ySp);
        free(Panes->xSp);
        free(Panes);
        Panes = q;
    }
}