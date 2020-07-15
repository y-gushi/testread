#include "RowColumn.h"

Ctags::Ctags(UINT8* decorddata, UINT64 datalen, shareRandD* shdata){
    data = decorddata;
    dlen = datalen;
    sh = shdata;
    wd = nullptr;
    MC = nullptr;
}

Ctags::~Ctags(){
    free(headXML);
    free(dimtopane);

    free(sFPr);

    free(dm->sC);
    free(dm->sR);
    free(dm->eC);
    free(dm->eR);
    free(dm);

    free(MC);
    free(margeCellCount);

    free(fstr);
    free(wd);

    Rowtablefree();
    selectfree();
    panefree();
    colfree();
}

void Ctags::sheetread() {

    GetSheetPr();

    GetDiment();

    GetSelectionPane();
    Getcols();

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
    else if (col == c->col) {//列番号同じ　情報更新
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

UINT8* Ctags::getvalue() {

    UINT32 len = 0;
    UINT8* Sv = nullptr;

    while (data[p + len] != '"')
        len++;

    stocklen = len;
    UINT32 ssize = len + 1;

    Sv = (UINT8*)malloc(ssize);

    for (UINT32 i = 0; i < len; i++){
        Sv[i] = data[p]; p++;
    }

    Sv[len] = '\0';

    return Sv;

}

UINT8* Ctags::getSi(UINT8* v, UINT32 vl) {

    UINT32 Vnum = 0;
    UINT8* Vsi = nullptr;

    Vnum = NA.RowArraytoNum(v, vl);//数字に変換

    UINT32 sistrlen = 0;//si タグ内 <t>たぐの足した文字長さ  //UINT8* Vsi = sh->sis[Vnum]->Ts;//share str into struct　<t>何個かある
    UINT32 Tstrlen = 0;

    Si* RootSi = sh->sis[Vnum];//親　参照

    while (RootSi) {//t タグ　総合文字列　長さ確認
        Tstrlen = 0;
        while (RootSi->Ts[Tstrlen] != '\0') {
            sistrlen++; Tstrlen++;
        }

        RootSi = RootSi->next;
    }

    RootSi = sh->sis[Vnum];

    if (sistrlen > 0) {
        sistrlen = sistrlen + 1;
        Vsi = (UINT8*)malloc(sistrlen);//メモリ確保

        sistrlen = 0;

        while (RootSi) {//tタグ足し合わせる
            Tstrlen = 0;//初期化
            while (RootSi->Ts[Tstrlen] != '\0') {
                Vsi[sistrlen] = RootSi->Ts[Tstrlen];
                sistrlen++; Tstrlen++;
            }
            RootSi = RootSi->next;
        }
        Vsi[sistrlen] = '\0';
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
        p++;//位置移動

        if (strncmp((char const*)reftag, Fref, 5) == 0)// ref macth
            refv = getvalue();

        if (strncmp((char const*)sitag, Fsi, 4) == 0)// si macth
            siv = getvalue();

        if (strncmp((char const*)ttag, Ft, 2) == 0)// t macth
            Tv = getvalue();
    }

    if (data[p - 1] == '/') {
        //vなし
    }
    else {
        p++;// > の次から
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

    *size = len;//文字長さ　参照入力
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

    UINT8 endtag[5] = { 0 };// </c>検索
    UINT8 ttag[4] = { 0 };// t=" s=" r="　<v> 検索
    UINT8 ftag[3] = { 0 };// <f 検索

    UINT8* Tv = nullptr;
    UINT8* Sv = nullptr;
    UINT8* Vv = nullptr;
    UINT8* col = nullptr;
    UINT8* row = nullptr;
    F* fval = nullptr;
    Row* rn = nullptr;//検索用

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
        p++;//位置移動

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

            while (data[p + collen + rowlen] != '"') {//列行 get
                if (data[p + collen + rowlen] > 64)//列
                    collen++;
                else//行
                    rowlen++;
            }

            UINT32 csize = collen + 1;
            UINT32 rsize = rowlen + 1;
            col = (UINT8*)malloc(csize);
            row = (UINT8*)malloc(rsize);

            for (UINT32 i = 0; i < collen; i++){
                col[i] = data[p]; p++;
            }

            for (UINT32 i = 0; i < rowlen; i++) {
                row[i] = data[p]; p++;
            }
            col[collen] = '\0'; row[rowlen] = '\0';
            colnum = NA.ColumnArraytoNumber(col, collen);//文字数値変換
            rownum = NA.RowArraytoNum(row, rowlen);//数字変換

            free(col); free(row);
        }

        if (strncmp((char const*)ttag, Vtag, 3) == 0) // <v> get
            Vv = getVorftag(endVtag, 4, &vlen);

        if (strncmp((char const*)ftag, Ftag, 2) == 0) // <f get
            fval = getFtag();
    }

    UINT8* Vsi = nullptr;

    //t=sの場合 si入れる
    if (Tv) {
        if (Tv[0] == 's' && tlen == 1)
            Vsi = getSi(Vv, vlen);//si文字列入れる
    }
    rn = searchRow(rows, rownum);

    if (rn) {
        rn->cells = addCtable(rn->cells, Tv, Sv, Vsi, colnum, Vv, fval);
    }
    else {
        //row 番号なし need make new row
    }

}

// <sheetPr get
void Ctags::GetSheetPr() {
    /* <sheetPr>
<tabColor tint="0.4" theme="7"/>
<pageSetUpPr fitToPage="1"/>
</sheetPr>*/

//const char* shPr = "<sheetPr";//8
//const char* shPrEnd = "</sheetPr>";//10
    const char* diment = "<dimension";//10
    UINT8 pr[10] = { 0 };
    p = 0;//sheetdata
    int ucode = 0;//dimention <sheetPr
    int result = 0;

    do {
        for (int j = 0; j < 10 - 1; j++) {
            pr[j] = pr[j + 1];
        }
        pr[10 - 1] = data[p + ucode];
        ucode++;

        result = strncmp((char const*)pr, diment, 10);

    } while (result != 0);

    ucode -= 10;

    UINT32 msize = (UINT32)ucode + 1;

    headXML = (UINT8*)malloc(msize);

    if (headXML) {
        for (int i = 0; i < ucode; i++) {//dimention
            headXML[i] = data[p];
            p++;
        }
        headXML[ucode] = '\0';
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

//demention get

void Ctags::GetDiment() {

    UINT8 sd[17] = { 0 };
    int spos = 0;

    dm = (demention*)malloc(sizeof(demention));
    dm->sC = (UINT8*)malloc(5);//demention start 列
    memset(dm->sC, 0, 5);
    dm->sR = (UINT8*)malloc(5);//demention start 行
    memset(dm->sR, 0, 5);
    dm->eC = (UINT8*)malloc(5);//demention end 列
    memset(dm->eC, 0, 5);
    dm->eR = (UINT8*)malloc(5);//demention end 行
    memset(dm->eR, 0, 5);

    // <dimension ref="A1:EJ128"/>を検索

    while (spos < dlen) {// : までコピー
        for (int j = 0; j < 16 - 1; j++) {
            sd[j] = sd[j + 1];
        }
        sd[16 - 1] = data[p];//最後に付け加える
        p++;

        if (strncmp((char const*)sd, dement, 16) == 0)
            break;
    }

    while (data[p] != ':') {//startcell
        if (data[p] > 64) {//列番号
            dm->sC[sClen] = data[p];
            sClen++;
        }
        else {//行番号
            dm->sR[sRlen] = data[p];
            sRlen++;
        }
        p++;
    }

    dm->sC[sClen] = '\0'; dm->sR[sRlen] = '\0';
    p++;//':' 次へ

    while (data[p] != '"') {//endcell
        if (data[p] > 64) {//列番号
            dm->eC[eClen] = data[p];
            eClen++;
        }
        else {//行番号
            dm->eR[eRlen] = data[p];
            eRlen++;
        }
        p++;
    }

    dm->eC[eClen] = '\0'; dm->eR[eRlen] = '\0';

    maxcol = NA.ColumnArraytoNumber(dm->eC, eClen);//現max列
}

//selection tag get　テーブルに
void Ctags::GetSelectionPane() {

    const char* pane = "<pane";//5
    const char* select = "<selection";//10
    UINT8 sve[14] = { 0 };
    UINT8 PN[11] = { 0 };
    UINT8 slcs[11] = { 0 };
    int PNlen = 0;
    UINT8 PT[6] = { 0 };

    int res = 1;
    int panetagres = 0;

    while (res != 0)
    {//diment /> to <sheetView まで
        for (int j = 0; j < 10 - 1; j++) {//文字数カウント
            PN[j] = PN[j + 1];
        }
        PN[10 - 1] = data[p + PNlen];//最後に付け加える
        PNlen++;

        res = strncmp((char const*)PN, startSV, 10);
    }
    //std::cout << "data[p + PNlen] : " << data[p + PNlen] << std::endl;
    if (data[p + PNlen] == 's') {// <sheetViews>tag
        PNlen += 3;// <sheetViews> 閉じタグとばす
    }
    while (data[p + PNlen - 1] != '>')//文字数カウント sheetView >閉じまで
        PNlen++;

    UINT32 msize = (UINT32)PNlen + 1;
    dimtopane = (UINT8*)malloc(msize);
    if (dimtopane) {
        for (int i = 0; i < PNlen; i++) {///to <sheetView 文字列コピー
            dimtopane[i] = data[p];
            p++;
        }
        dimtopane[PNlen] = '\0';
        //std::cout << "cim to pane" << dimtopane << std::endl;
    }

    res = 1;
    int seleresult = 0;

    while (res != 0)//selectionPane vals get
    {
        for (int j = 0; j < 13 - 1; j++) {
            sve[j] = sve[j + 1];
            if (j < 10 - 1) {
                slcs[j] = slcs[j + 1];
            }
            if (j < 5 - 1) {
                PT[j] = PT[j + 1];
            }
        }
        PT[5 - 1] = sve[13 - 1] = slcs[10 - 1] = data[p];
        p++;

        seleresult = strncmp((char const*)slcs, select, 10);
        panetagres = strncmp((char const*)PT, pane, 5);
        if (panetagres == 0) {// <pane tag get
            //std::cout << "get selection  pane" << std::endl;
            GetPane();
        }
        if (seleresult == 0) {
            getselection();
        }

        res = strncmp((char const*)sve, SVend, 13);// </sheetViews>で抜ける
    }
}

void Ctags::getselection() {
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
        //sct = SLTaddtable(sct, pa, ac, sq);//すべてnullptrわたす
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
            sr[7 - 1] = pn[6 - 1] = at[12 - 1] = data[p];//最後に付け加える
            p++;

            if (strncmp((char const*)pn, ecp, 6) == 0) {//pane=" 一致
                panelen = 0;
                while (data[p + panelen] != '"')
                    panelen++;//文字数カウント
                UINT32 pasize = (UINT32)panelen + 1;
                pa = (UINT8*)malloc(pasize);
                for (int i = 0; i < panelen; i++) {
                    pa[i] = data[p];//pane str
                    p++;
                }
                pa[panelen] = '\0';
            }

            if (strncmp((char const*)at, active, 12) == 0) {//activeCell=" 一致
                activeCelllen = 0;
                while (data[p + activeCelllen] != '"')
                    activeCelllen++;//文字数カウント
                UINT32 acsize = (UINT32)activeCelllen + 1;
                ac = (UINT8*)malloc(acsize);
                for (int i = 0; i < activeCelllen; i++) {
                    ac[i] = data[p];//pane str
                    p++;
                }
                ac[activeCelllen] = '\0';
            }

            if (strncmp((char const*)sr, sqref, 7) == 0) {//sqref=" 一致
                sqreflen = 0;
                while (data[p + sqreflen] != '"')
                    sqreflen++;//文字数カウント
                UINT32 sqsize = (UINT32)sqreflen + 1;
                sq = (UINT8*)malloc(sqsize);
                for (int i = 0; i < sqreflen; i++) {
                    sq[i] = data[p];//pane str
                    p++;
                }
                sq[sqreflen] = '\0';
            }
        }
        //if (pa || ac || sq)
    }
    sct = SLTaddtable(sct, pa, ac, sq);//構造体へコピー
    
}

void Ctags::GetPane() {

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

    Panes = addpanetable(Panes, X, Y, tL, ap, s);

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

    cls = addcolatyle(cls, min, max, colw, style, hidd, bestf, cuw);

}

//col tag get
void Ctags::Getcols() {
    UINT8 search[7] = { 0 };
    UINT8 Scoltag[5] = { 0 };
    int result = 0;
    int coltaglen = 7;
    int endresult = 1;
    int sfplen = 0;

    //sheetFormatPr 文字列

    while (data[p + sfplen] != '>')
        sfplen++;

    sfplen++;

    UINT32 sfsize = (UINT32)sfplen + 1;

    sFPr = (UINT8*)malloc(sfsize);

    for (int j = 0; j < sfplen; j++) {
        sFPr[j] = data[p];
        p++;
    }

    sFPr[sfplen] = '\0';

    while (endresult != 0) {

        for (int j = 0; j < coltaglen - 1; j++) {
            search[j] = search[j + 1];
            if (j < (5 - 1))
                Scoltag[j] = Scoltag[j + 1];
        }

        search[coltaglen - 1] = Scoltag[5 - 1] = data[p];
        p++;

        result = strncmp((char const*)Scoltag, Coltag, 5);

        if (result == 0)
            getcolv();

        endresult = strncmp((char const*)search, endtag, 7);

    }
}

cols* Ctags::addcolatyle(cols* cs, UINT8* min, UINT8* max, UINT8* W, UINT8* sty, UINT8* hid, UINT8* bF, UINT8* cuW) {

    if (!cs) {
        cs = coltalloc();
        cs->min = min;
        cs->max = max;
        cs->width = W;
        cs->style = sty;
        cs->hidden = hid;
        cs->bestffit = bF;
        cs->customwidth = cuW;
        cs->next = nullptr;
    }
    else
        cs->next = addcolatyle(cs->next, min, max, W, sty, hid, bF, cuW);

    return cs;
}

cols* Ctags::coltalloc() {
    return (cols*)malloc(sizeof(cols));
}

void Ctags::Ctableprint(C* c) {

    C* p = c;
    while (p) {
        if (p->val)
            std::cout << " val : " << p->val;
        if (p->t)
            std::cout << " t : " << p->t;
        if (p->s)
            std::cout << " s : " << p->s;
        if (p->col)
            std::cout << " col : " << p->col;
        if (p->f)
            std::cout << " f : " << p->f;
        std::cout << std::endl;

        p = p->next;
    }
}

// last string copy
void Ctags::getfinalstr() {

    UINT32 s = UINT32(dlen) - UINT32(p);
    UINT64 i = 0;
    const char* margeinfo = "<mergeCells count=\"";//19char
    const char* marge = "<mergeCell ref=\"";//16char

    UINT8 sm[20] = { 0 };
    UINT8 Sm[17] = { 0 };

    UINT32 fstrsize = s + 1;
    fstr = (UINT8*)malloc(fstrsize);
    int result = 0;
    int mresult = 0;

    MC = nullptr;

    while (p < dlen) {
        for (int j = 0; j < 19 - 1; j++) {
            sm[j] = sm[j + 1];
            if (j < (16 - 1))
                Sm[j] = Sm[j + 1];//marge count
        }
        Sm[16 - 1] = sm[19 - 1] = fstr[i] = data[p];
        p++; i++;

        result = strncmp((const char*)sm, margeinfo, 19);
        mresult = strncmp((const char*)Sm, marge, 16);

        if (result == 0) {//marge cell
            margeCellCount = getvalue();
        }

        if (mresult == 0) {
            UINT32 len = UINT32(i);

            while (data[p] != '"') {
                fstr[i] = data[p];
                p++; i++;
            }
            len = (UINT32)i -(UINT32)len;//string num
            size_t ml = (size_t)len + 1;
            MC = (UINT8*)malloc(ml);

            for (UINT32 j = 0; j < len; j++)
                MC[j] = data[p - len + j];
            MC[len] = '\0';
        }
    }
    fstr[s] = '\0';

    /* <mergeCells count="15">
    <mergeCell ref="E11:G11"/>
    <mergeCell ref="E12:G12"/>
    <mergeCell ref="O13:P13"/>
    <mergeCell ref="E15:M15"/>
    <mergeCell ref="B15:B16"/>
    <mergeCell ref="C15:C16"/>
    <mergeCell ref="D15:D16"/>
    <mergeCell ref="N15:N16"/>
    <mergeCell ref="O15:O16"/>
    <mergeCell ref="P15:P16"/>
    <mergeCell ref="B2:P2"/>
    <mergeCell ref="B3:P3"/>
    <mergeCell ref="B4:P4"/>
    <mergeCell ref="B5:P5"/>
    <mergeCell ref="B6:P6"/>
    </mergeCells> */
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

    UINT8 Ref[6] = { 0 };
    int result = 0;

    UINT8* marcell = nullptr;
    UINT8* stacell = nullptr;
    UINT8* endcell = nullptr;

    while (data[p] != '>') {
        for (int j = 0; j < 5 - 1; j++) {
            Ref[j] = Ref[j + 1];
        }
        Ref[5 - 1] = data[p];
        p++;

        result = strncmp((const char*)Ref, ref, 5);
        if (result == 0) {//marge cell
            marcell = getvalue();

            //文字分ける
            size_t mlen = 0;
            while (marcell[mlen] != ':')
                mlen++;

            stacell = (UINT8*)malloc(mlen+1);
            for (size_t i = 0; i < mlen; i++)
                stacell[i] = marcell[i];
            stacell[mlen] = '\0';

            mlen++;//:skip

            size_t  elen= mlen;//後半スタート位置
            while (marcell[mlen] != '\0')
                mlen++;

            size_t memsiz = mlen - elen;
            
            endcell = (UINT8*)malloc(memsiz+1);
            elen = 0;
            for (size_t i = 0; i < memsiz; i++) {
                endcell[i] = marcell[elen];
                elen++;
            }
            endcell[memsiz] = '\0';

            free(marcell);
        }
    }    
    margeCellRoot = addmargecell(margeCellRoot, stacell, endcell);
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