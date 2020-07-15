#include "RowColumn.h"

//セルデータの追加　row span更新
void Ctags::addcelldata(UINT8* row, UINT8* col, UINT8* t, UINT8* s, UINT8* v, F* f, UINT8* si) {
    Row* ro = nullptr;
    bool replace = false;
    int rp = 0;//桁数
    UINT8 nomals[] = "1402";

    while (row[rp] != '\0')
        rp++;
    UINT32 rn = NA.RowArraytoNum(row, rp);//行配列から数字

    rp = 0;//桁数
    while (col[rp] != '\0')
        rp++;
    UINT32 cn = NA.ColumnArraytoNumber(col, rp);//spanよう　列文字数字

    if (cn > maxcol) {//diment 最大値更新
        maxcol = cn;
        dm->eC = NA.ColumnNumtoArray(maxcol, &rp);
        replace = true;
    }

    ro = searchRow(rows, rn);//row位置検索
    if (!ro) {
        //std::cout << "row なし 追加" << std::endl;
        int cnpl = 0;//桁数　使わない

        UINT32 newcn = NA.ColumnCharnumtoNumber(cn);//列番号連番へ　入れた列
        UINT8* charcn = NA.InttoChar(newcn, &cnpl);//intを配列へ

        UINT8* spanSt = charcn;//行スタート位置
        UINT8* spanEn = charcn;//行終わり
        UINT8 HT[] = "29.25";
        UINT8* hT = (UINT8*)malloc(6);
        strcpy_s((char*)hT, 6, (const char*)HT);
        UINT8* thick = nullptr;
        UINT8* rs = nullptr;
        UINT8* CF = nullptr;
        UINT8 CH[] = "1";//customhigh
        UINT8* cH = (UINT8*)malloc(2);
        strcpy_s((char*)cH, 2, (const char*)CH);
        rows = addrows(rows, rn, spanSt, spanEn, hT, thick, rs, CF, cH, nullptr);

        ro = searchRow(rows, rn);

        //cell null データ入れる
        //std::cout << " セル追加 rowなし : " << ro->r << " 行 " << cn << std::endl;
        //incolnum = NA.NumbertoArray(colnum_start);//数字を文字数時に
        ro->cells = addCtable(ro->cells, t, s, si, cn, v, f);
    }
    else {
        //rowあり
        //std::cout << " row あり　追加 v : " << v << std::endl;

        //入れるcellの前がなければ入れる
        C* Croot = ro->cells;
        while (Croot->next) {
            Croot = Croot->next;
        }

        UINT32 incolnum = Croot->col;//現在の最終行
        NA.ColumnIncliment(&incolnum);

        while (incolnum <= cn) {
            if (incolnum == cn) {
                //std::cout << " セル追加 前あり　row : " << ro->r << " 行 " << incolnum << std::endl;
                //incolnum = NA.NumbertoArray(colnum_start);//数字を文字数時に
                ro->cells = addCtable(ro->cells, t, s, si, incolnum, v, f);
            }
            else {
                //std::cout << " セル追加　row : " << ro->r << " 行 " << incolnum << " s " << s << std::endl;
                //incolnum = NA.NumbertoArray(colnum_start);//数字を文字数時に
                UINT8* n1 = nullptr;
                UINT8* n2 = nullptr;
                UINT8* n3 = nullptr;
                F* n4 = nullptr;
                UINT8* sv = (UINT8*)malloc(5);
                strcpy_s((char*)sv, 5, (const char*)nomals);
                ro->cells = addCtable(ro->cells, n1, sv, n2, incolnum, n3, n4);
            }
            NA.ColumnIncliment(&incolnum);
            //colnum_start++;
        }
    }

    //ro->cells = addCtable(ro->cells, t, s, si, cn, v, f);//セル情報検索
    cn = NA.ColumnCharnumtoNumber(cn);//列番号連番へ　入れた列

    if (replace) {//最大値更新で　cols style確認
        cols* ncols = cls;
        while (ncols->next)//cols　最後参照
            ncols = ncols->next;
        int j = 0;
        while (ncols->max[j] != '\0')
            j++;
        UINT32 ncn = NA.RowArraytoNum(ncols->max, j);//最終max文字列 数字に変換
        if (cn > ncn) {//cols max 小さい
            UINT8 wi[] = "9"; UINT8 styl[] = "1";
            size_t colmemsiz = strlen((const char*)col) + 1;
            UINT8* mincol = (UINT8*)malloc(colmemsiz);
            strcpy_s((char*)mincol, colmemsiz, (const char*)col);

            UINT8* maxcol = (UINT8*)malloc(colmemsiz);
            strcpy_s((char*)maxcol, colmemsiz, (const char*)col);

            UINT8* Hi = nullptr;
            UINT8* Bf = nullptr;
            UINT8* Cuw = nullptr;

            cls = addcolatyle(cls, col, col, wi, styl, Hi, Bf, Cuw);//cols 追加
        }
    }

    rp = 0;//桁数
    while (ro->spanE[rp] != '\0')
        rp++;
    UINT32 nnc = NA.RowArraytoNum(ro->spanE, rp);//span 文字列数字に
    if (cn > nnc) {//新しい文字列が大きい場合　更新
        UINT8* newspan = NA.InttoChar(cn, &rp);
        ro->spanE = newspan;
        //std::cout << " new span : " << ro->spanE << std::endl;
    }
}

void Ctags::writesheetdata() {
    size_t msize = size_t(dlen) + 6000;

    wd = (UINT8*)malloc(msize);//メモリサイズ変更　書き込み用
    //std::cout << "データ更新" << p << std::endl;
    p = 0;

    if (wd) {
        while (headXML[p] != '\0') {
            wd[p] = headXML[p];
            p++;
        }
    }

    writeDiment();
    writeSelection();
    writecols();
    writecells();
    writefinal();

    //std::cout << "size : " << p << " dlen : " << dlen << std::endl;
}

//diment書き込み
void Ctags::writeDiment() {
    const char* dimstart = "<dimension ref=\"";
    //std::cout << "ディメント更新" << p << std::endl;
    while (dimstart[writep] != '\0') {
        wd[p] = dimstart[writep]; p++; writep++;
    }writep = 0;
    while (dm->sC[writep] != '\0') {//スタート列
        wd[p] = dm->sC[writep]; p++; writep++;
    }writep = 0;
    while (dm->sR[writep] != '\0') {//スタート行
        wd[p] = dm->sR[writep]; p++; writep++;
    }writep = 0;
    wd[p] = ':'; p++;
    while (dm->eC[writep] != '\0') {//終了列
        wd[p] = dm->eC[writep]; p++; writep++;
    }writep = 0;
    while (dm->eR[writep] != '\0') {//終了行
        wd[p] = dm->eR[writep]; p++; writep++;
    }writep = 0;
    //std::cout << "ディメント更新" << dm->eC << std::endl;
}
//cols書き込み
void Ctags::writecols() {
    const char* startcols = "<cols>";
    const char* colstr[] = { "<col min=\"","\" max=\"","\" width=\"","\" style=\""
        ,"\" bestFit=\"","\" hidden=\"","\" customWidth=\"" };

    //std::cout << "コルス更新" << p << std::endl;
    const char* endcols = "</cols>";
    int writep = 0;

    cols* Col = cls;
    while (startcols[writep] != '\0') {
        wd[p] = startcols[writep]; p++; writep++;
    }
    writep = 0;
    while (Col) {
        while (colstr[0][writep] != '\0') {
            wd[p] = colstr[0][writep]; p++; writep++;
        }writep = 0;
        while (Col->min[writep] != '\0') {//min+head
            wd[p] = Col->min[writep]; p++; writep++;
        }writep = 0;
        while (colstr[1][writep] != '\0') {
            wd[p] = colstr[1][writep]; p++; writep++;
        }writep = 0;
        while (Col->max[writep] != '\0') {//max
            wd[p] = Col->max[writep]; p++; writep++;
        }writep = 0;
        while (colstr[2][writep] != '\0') {
            wd[p] = colstr[2][writep]; p++; writep++;
        }writep = 0;
        while (Col->width[writep] != '\0') {//width
            wd[p] = Col->width[writep]; p++; writep++;
        }writep = 0;
        if (Col->style) {
            while (colstr[3][writep] != '\0') {
                wd[p] = colstr[3][writep]; p++; writep++;
            }writep = 0;
            while (Col->style[writep] != '\0') {//style
                wd[p] = Col->style[writep]; p++; writep++;
            }writep = 0;
        }
        if (Col->hidden) {
            while (colstr[5][writep] != '\0') {
                wd[p] = colstr[5][writep]; p++; writep++;
            }writep = 0;
            while (Col->hidden[writep] != '\0') {//hidden
                wd[p] = Col->hidden[writep]; p++; writep++;
            }writep = 0;
        }
        if (Col->bestffit) {
            while (colstr[4][writep] != '\0') {
                wd[p] = colstr[4][writep]; p++; writep++;
            }writep = 0;
            while (Col->bestffit[writep] != '\0') {//hidden
                wd[p] = Col->bestffit[writep]; p++; writep++;
            }writep = 0;
        }
        if (Col->customwidth) {
            while (colstr[6][writep] != '\0') {
                wd[p] = colstr[6][writep]; p++; writep++;
            }writep = 0;
            while (Col->customwidth[writep] != '\0') {//hidden
                wd[p] = Col->customwidth[writep]; p++; writep++;
            }writep = 0;
        }
        while (closetag[writep] != '\0') {
            wd[p] = closetag[writep]; p++; writep++;
        }writep = 0;
        Col = Col->next;
    }
    while (endcols[writep] != '\0') {
        wd[p] = endcols[writep]; p++; writep++;
    }writep = 0;
}
//selection pane書きこみ
void Ctags::writeSelection() {
    const char* selpane[] = { "<selection"," activeCell=\"","\" sqref=\"","</sheetView>","</sheetViews>", " pane=\"" };
    const char* panes[] = { "<pane"," xSplit=\""," ySplit=\""," topLeftCell=\""," activePane=\""," state=\"" };
    std::cout << "セレクション更新" << p << std::endl;

    // <sheetview書き込み
    while (dimtopane[writep] != '\0') {
        wd[p] = dimtopane[writep]; p++; writep++;
    }writep = 0;

    Pane* Pa = Panes;
    // <pane 書き込み
    // <pane xSplit="176" ySplit="11" topLeftCell="HI75" activePane="bottomRight" state="frozen"/>
    while (Pa) {
        while (panes[0][writep] != '\0') {// <pane
            wd[p] = panes[0][writep]; p++; writep++;
        }writep = 0;

        if (Pa->xSp) {
            while (panes[1][writep] != '\0') {// <pane
                wd[p] = panes[1][writep]; p++; writep++;
            }writep = 0;
            while (Pa->xSp[writep] != '\0') {//pane
                wd[p] = Pa->xSp[writep]; p++; writep++;
            }writep = 0;
            wd[p] = '"'; p++;
        }
        if (Pa->ySp) {
            while (panes[2][writep] != '\0') {// <pane
                wd[p] = panes[2][writep]; p++; writep++;
            }writep = 0;
            while (Pa->ySp[writep] != '\0') {//pane
                wd[p] = Pa->ySp[writep]; p++; writep++;
            }writep = 0;
            wd[p] = '"'; p++;
        }
        if (Pa->tLeftC) {
            while (panes[3][writep] != '\0') {// <pane
                wd[p] = panes[3][writep]; p++; writep++;
            }writep = 0;
            while (Pa->tLeftC[writep] != '\0') {//pane
                wd[p] = Pa->tLeftC[writep]; p++; writep++;
            }writep = 0;
            wd[p] = '"'; p++;
        }
        if (Pa->activP) {
            while (panes[4][writep] != '\0') {// <pane
                wd[p] = panes[4][writep]; p++; writep++;
            }writep = 0;
            while (Pa->activP[writep] != '\0') {//pane
                wd[p] = Pa->activP[writep]; p++; writep++;
            }writep = 0;
            wd[p] = '"'; p++;
        }
        if (Pa->state) {
            while (panes[5][writep] != '\0') {// <pane
                wd[p] = panes[5][writep]; p++; writep++;
            }writep = 0;
            while (Pa->state[writep] != '\0') {//pane
                wd[p] = Pa->state[writep]; p++; writep++;
            }writep = 0;
            wd[p] = '"'; p++;
        }
        wd[p] = '/'; p++;
        wd[p] = '>'; p++;
        Pa = Pa->next;
    }

    selection* Sel = sct;
    // <selection 書き込み
    while (Sel) {
        while (selpane[0][writep] != '\0') {
            wd[p] = selpane[0][writep]; p++; writep++;
        }writep = 0;
        if (Sel->p) {
            while (selpane[5][writep] != '\0') {
                wd[p] = selpane[5][writep]; p++; writep++;
            }writep = 0;
            while (Sel->p[writep] != '\0') {//pane
                wd[p] = Sel->p[writep]; p++; writep++;
            }writep = 0;
            wd[p] = '"'; p++;
        }

        while (selpane[1][writep] != '\0') {
            wd[p] = selpane[1][writep]; p++; writep++;
        }writep = 0;
        if (Sel->a) {
            while (Sel->a[writep] != '\0') {//activcell
                wd[p] = Sel->a[writep]; p++; writep++;
            }writep = 0;
        }

        while (selpane[2][writep] != '\0') {
            wd[p] = selpane[2][writep]; p++; writep++;
        }writep = 0;
        if (Sel->s) {
            while (Sel->s[writep] != '\0') {//sqref
                wd[p] = Sel->s[writep]; p++; writep++;
            }writep = 0;
        }

        while (closetag[writep] != '\0') {
            wd[p] = closetag[writep]; p++; writep++;
        }writep = 0;
        Sel = Sel->next;
    }

    while (selpane[3][writep] != '\0') {
        wd[p] = selpane[3][writep]; p++; writep++;
    }writep = 0;
    while (selpane[4][writep] != '\0') {
        wd[p] = selpane[4][writep]; p++; writep++;
    }writep = 0;
    // <sheetFormatPr write
    while (sFPr[writep] != '\0') {
        wd[p] = sFPr[writep]; p++; writep++;
    }writep = 0;
}
//セルデータ書き込み
void Ctags::writecells() {
    const char* sheetstart = "<sheetData>";
    const char* shend = "</sheetData>";
    std::cout << "セル更新" << p << std::endl;

    int Place = 0;
    while (sheetstart[writep] != '\0') {
        wd[p] = sheetstart[writep]; p++; writep++;
    }writep = 0;

    const char* rstart = "<row r=\"";
    const char* rspa = "\" spans=\"";
    const char* rS = "\" s=\"";
    const char* rtaght = "\" ht=\"";
    const char* rcH = "\" customHeight=\"";
    const char* rcF = "\" customFormat=\"";
    const char* rtB = "\" thickBot=\"";
    const char* rend = "\">";
    const char* Rend = "</row>";

    Row* Ro = rows;

    while (Ro) {
        while (rstart[writep] != '\0') {
            wd[p] = rstart[writep]; p++; writep++;
        }writep = 0;
        UINT8* RN = NA.InttoChar(Ro->r, &Place);//数字　配列変換
        while (RN[writep] != '\0') {// r
            wd[p] = RN[writep]; p++; writep++;
        }writep = 0;
        if (Ro->spanS) {//ない場合ある
            while (rspa[writep] != '\0') {//span
                wd[p] = rspa[writep]; p++; writep++;
            }writep = 0;

            while (Ro->spanS[writep] != '\0') {// spanstart
                wd[p] = Ro->spanS[writep]; p++; writep++;
            }writep = 0;
            wd[p] = ':'; p++;
        }
        if (Ro->spanE) {
            while (Ro->spanE[writep] != '\0') {// spanend
                wd[p] = Ro->spanE[writep]; p++; writep++;
            }writep = 0;
        }
        if (Ro->s) {
            while (rS[writep] != '\0') {
                wd[p] = rS[writep]; p++; writep++;
            }writep = 0;
            while (Ro->s[writep] != '\0') {// s
                wd[p] = Ro->s[writep]; p++; writep++;
            }writep = 0;
        }
        if (Ro->customFormat) {
            while (rcF[writep] != '\0') {// customformat
                wd[p] = rcF[writep]; p++; writep++;
            }writep = 0;
            while (Ro->customFormat[writep] != '\0') {//r
                wd[p] = Ro->customFormat[writep]; p++; writep++;
            }writep = 0;
        }
        if (Ro->ht) {
            while (rtaght[writep] != '\0') {// ht
                wd[p] = rtaght[writep]; p++; writep++;
            }writep = 0;
            while (Ro->ht[writep] != '\0') {//r
                wd[p] = Ro->ht[writep]; p++; writep++;
            }writep = 0;
        }
        if (Ro->customHeight) {
            while (rcH[writep] != '\0') {//custumhigh
                wd[p] = rcH[writep]; p++; writep++;
            }writep = 0;
            while (Ro->customHeight[writep] != '\0') {//r
                wd[p] = Ro->customHeight[writep]; p++; writep++;
            }writep = 0;
        }
        if (Ro->thickBot) {
            while (rtB[writep] != '\0') {// thickbot
                wd[p] = rtB[writep]; p++; writep++;
            }writep = 0;
            while (Ro->thickBot[writep] != '\0') {//r
                wd[p] = Ro->thickBot[writep]; p++; writep++;
            }writep = 0;
        }
        while (rend[writep] != '\0') {// ">
            wd[p] = rend[writep]; p++; writep++;
        }writep = 0;

        UINT8* rne = NA.InttoChar(Ro->r, &Place);//配列に変更
        writec(Ro->cells, rne);

        while (Rend[writep] != '\0') {// ">
            wd[p] = Rend[writep]; p++; writep++;
        }writep = 0;

        Ro = Ro->next;
    }
    while (shend[writep] != '\0') {
        wd[p] = shend[writep]; p++; writep++;
    }writep = 0;
}
// cタグ　書き込み
void Ctags::writec(C* ctag, UINT8* ROW) {
    const char* cstart = "<c r=\"";
    const char* ctags = "\" s=\"";
    const char* ctagt = "\" t=\"";
    const char* ftag = "<f";
    const char* ftagt = " t=\"";
    const char* ftagref = "\" ref=\"";
    const char* ftagsi = "\" si=\"";
    const char* ftagend = "</f>";
    const char* vtagend = "</v>";
    const char* vclose = "<v/>";
    const char* ctagend = "</c>";
    const char* cvend = "\">";
    int place = 0;

    C* cl = ctag;
    while (cl) {
        while (cstart[writep] != '\0') {// thickbot
            wd[p] = cstart[writep]; p++; writep++;
        }writep = 0;
        UINT8* Col = NA.ColumnNumtoArray(cl->col, &place);
        while (Col[writep] != '\0') {//r 列
            wd[p] = Col[writep]; p++; writep++;
        }writep = 0;
        while (ROW[writep] != '\0') {//r 行
            wd[p] = ROW[writep]; p++; writep++;
        }writep = 0;
        if (cl->s) {
            while (ctags[writep] != '\0') {// s
                wd[p] = ctags[writep]; p++; writep++;
            }writep = 0;
            while (cl->s[writep] != '\0') {// s
                wd[p] = cl->s[writep]; p++; writep++;
            }writep = 0;
        }
        if (cl->t) {//t あり
            while (ctagt[writep] != '\0') {// t
                wd[p] = ctagt[writep]; p++; writep++;
            }writep = 0;
            while (cl->t[writep] != '\0') {// t
                wd[p] = cl->t[writep]; p++; writep++;
            }writep = 0;
        }
        if (cl->f) {//Fあり
            while (cvend[writep] != '\0') {// ">
                wd[p] = cvend[writep]; p++; writep++;
            }writep = 0;
            while (ftag[writep] != '\0') {// f
                wd[p] = ftag[writep];
                p++; writep++;
            }writep = 0;
            if (cl->f->t) {
                while (ftagt[writep] != '\0') {// t
                    wd[p] = ftagt[writep]; p++; writep++;
                }writep = 0;
                while (cl->f->t[writep] != '\0') {// t
                    wd[p] = cl->f->t[writep];
                    p++; writep++;
                }writep = 0;
            }
            if (cl->f->ref) {
                while (ftagref[writep] != '\0') {// f ref
                    wd[p] = ftagref[writep]; p++; writep++;
                }writep = 0;
                while (cl->f->ref[writep] != '\0') {// f ref
                    wd[p] = cl->f->ref[writep]; p++; writep++;
                }writep = 0;
            }
            if (cl->f->si) {
                while (ftagsi[writep] != '\0') {// f si
                    wd[p] = ftagsi[writep]; p++; writep++;
                }writep = 0;
                while (cl->f->si[writep] != '\0') {// f si
                    wd[p] = cl->f->si[writep]; p++; writep++;
                }writep = 0;
            }
            if (cl->f->val) {//f vあり 計算式
                if (cl->f->t) {//tあり ">閉じ v
                    wd[p] = '"';
                    p++;
                }
                wd[p] = '>';//tなし
                p++;

                while (cl->f->val[writep] != '\0') {//f val
                    wd[p] = cl->f->val[writep]; p++; writep++;
                }writep = 0;
                while (ftagend[writep] != '\0') {// f end
                    wd[p] = ftagend[writep]; p++; writep++;
                }writep = 0;
            }
            else {
                while (closetag[writep] != '\0') {//vなし "/>とじ
                    wd[p] = closetag[writep]; p++; writep++;
                }writep = 0;
            }
        }
        if (cl->val) {//V あり
            if (!cl->f) {//fない場合閉じる
                while (cvend[writep] != '\0') {// ">
                    wd[p] = cvend[writep]; p++; writep++;
                }writep = 0;
            }
            while (Vtag[writep] != '\0') {// v
                wd[p] = Vtag[writep]; p++; writep++;
            }writep = 0;
            while (cl->val[writep] != '\0') {// v val
                wd[p] = cl->val[writep]; p++; writep++;
            }writep = 0;
            while (vtagend[writep] != '\0') {// /v
                wd[p] = vtagend[writep]; p++; writep++;
            }writep = 0;

            while (ctagend[writep] != '\0') {// /c end
                wd[p] = ctagend[writep]; p++; writep++;
            }writep = 0;
        }
        else {//V なし　クローズ
            //<c r="JK13" s="1525" t="str">
            //<f t="shared" ref="JK13:JK18" si="7">IF(JI13=JJ13,"","×")</f>
            //<v/>
            //</c>
            if (cl->f) {
                while (vclose[writep] != '\0') {// <v/> end
                    wd[p] = vclose[writep]; p++; writep++;
                }writep = 0;
                while (ctagend[writep] != '\0') {// /c end
                    wd[p] = ctagend[writep]; p++; writep++;
                }writep = 0;
            }
            else {
                while (closetag[writep] != '\0') {
                    wd[p] = closetag[writep]; p++; writep++;
                }writep = 0;
            }
        }
        cl = cl->next;
    }
}
//最終文字列書き込み
void Ctags::writefinal() {
    while (fstr[writep] != '\0') {// /v
        wd[p] = fstr[writep]; p++; writep++;
    }writep = 0;
}

UINT8* Ctags::StrInit()
{
    UINT8* p = (UINT8*)malloc(1);
    p = nullptr;
    return p;
}