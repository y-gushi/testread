#include "RowColumn.h"

void Ctags::colsSettingPlus(cols* nc) {
    cols* ncols = cls;
    UINT32 inminnum = 0;
    UINT32 inmaxnum = 0;
    inminnum = NA.ColumnArraytoNumber(ncols->min, strlen((char*)nc->min));//文字を数字
    inminnum = NA.ColumnCharnumtoNumber(inminnum);//文字の数値を番号に
    inmaxnum = NA.ColumnArraytoNumber(ncols->max, strlen((char*)nc->max));//文字を数字
    inmaxnum = NA.ColumnCharnumtoNumber(inmaxnum);//文字の数値を番号に
    int res = 0;
    bool flg = false;

    if (!cls) {//cls nullそのまま入れる
        //cls 最後尾に追加 メモリ確保しながら
        colspushlast(nc);
        flg = true;
    }
    else {//cls あれば設定確認
        while (ncols->next) {//cols　最後参照
            flg = checkcols(ncols, nc, inmaxnum, inminnum);
            if (flg)
                break;//設定一致or範囲内更新
            ncols = ncols->next;
        }
    }

    if (!flg) {
        //cls 最後尾に追加 メモリ確保しながら
        colspushlast(nc);
    }
}

bool Ctags::checkcols(cols* bcl, cols* ncl,UINT32 nma,UINT32 nmi) {
    UINT32 minnum = 0;
    UINT32 maxnum = 0;
    minnum = NA.ColumnArraytoNumber(bcl->min, strlen((char*)bcl->min));//文字を数字
    minnum = NA.ColumnCharnumtoNumber(minnum);//文字の数値を番号に
    maxnum = NA.ColumnArraytoNumber(bcl->max, strlen((char*)bcl->max));//文字を数字
    maxnum = NA.ColumnCharnumtoNumber(maxnum);//文字の数値を番号に
    bool bflg = false; bool cflg = false; bool hflg = false; bool sflg = false; bool wflg = false;
    int bres = 1; int cres = 1; int hres = 1; int sres = 1; int wres = 1;
    //min max の間　更新か否か
    if (minnum <= nmi && maxnum >= nma) {
        if (bcl->bestffit && !ncl->bestffit)
            bflg = false;
        else if (!bcl->bestffit && ncl->bestffit)
            bflg = false;
        else if (!bcl->bestffit && !ncl->bestffit)
            bflg = true;
        else {
            bres = strcmp((char*)bcl->bestffit, (char*)ncl->bestffit);
            if (bres == 0)
                bflg = true;
        }

        if (bcl->customwidth && !ncl->customwidth)
            cflg = false;
        else if (!bcl->customwidth && ncl->customwidth)
            cflg = false;
        else if (!bcl->customwidth && !ncl->customwidth)
            cflg = true;
        else {
            cres = strcmp((char*)bcl->customwidth, (char*)ncl->customwidth);
            if (bres == 0)
                cflg = true;
        }

        if (bcl->hidden && !ncl->hidden)
            hflg = false;
        else if (!bcl->hidden && ncl->hidden)
            hflg = false;
        else if (!bcl->hidden && !ncl->hidden)
            hflg = true;
        else {
            hres = strcmp((char*)bcl->hidden, (char*)ncl->hidden);
            if (bres == 0)
                hflg = true;
        }

        if (bcl->style && !ncl->style)
            sflg = false;
        else if (!bcl->style && ncl->style)
            sflg = false;
        else if (!bcl->style && !ncl->style)
            sflg = true;
        else {
            sres = strcmp((char*)bcl->style, (char*)ncl->style);
            if (bres == 0)
                sflg = true;
        }

        if (bcl->width && !ncl->width)
            wflg = false;
        else if (!bcl->width && ncl->width)
            wflg = false;
        else if (!bcl->width && !ncl->width)
            wflg = true;
        else {
            wres = strcmp((char*)bcl->width, (char*)ncl->width);
            if (bres == 0)
                wflg = true;
        }

        if (bflg && cflg && hflg && sflg && wflg)//設定一致　更新なし
            return true;
        else {//設定違う　更新必要
            insertcols(bcl, ncl, nmi, nma);
            return true;
        }
    }
    //min max 範囲なし　そのまま後ろに追加
    return false;
}

void Ctags::insertcols(cols* bc, cols* nc,UINT32 mi,UINT32 ma) {
    cols* st = nullptr;

    if(bc->next)
        st = bc->next;

    //現在のcol max 更新
    UINT32 num = mi - 1;
    int pl = 0;
    UINT8* maxnewstr = NA.InttoChar(num,&pl);
    free(bc->max);
    bc->max = maxnewstr;

    //次のcol min更新 stあれば
    if(st){
        UINT32 mnum = ma + 1;
        UINT8* minnewstr = NA.InttoChar(mnum, &pl);
        free(st->min);
        st->min = minnewstr;//現在のcol max 更新
    }   

    bc->next = nc;
    nc->parrent = bc;
    nc->next = st;    
}

void Ctags::colspushlast(cols* n) {
    size_t msiz = 0;
    UINT8* Hi = nullptr;
    UINT8* Bf = nullptr;
    UINT8* Cuw = nullptr;
    UINT8* wid = nullptr;
    UINT8* STy = nullptr;
    UINT8* maxcol = nullptr;
    UINT8* mincol = nullptr;

    if (n->bestffit) {
        msiz = strlen((const char*)n->bestffit) + 1;
        Bf = (UINT8*)malloc(sizeof(UINT8) * msiz);
        strcpy_s((char*)Bf, msiz, (char*)n->bestffit);
    }
    if (n->width) {
        msiz = strlen((const char*)n->width) + 1;
        wid = (UINT8*)malloc(sizeof(UINT8) * msiz);
        strcpy_s((char*)wid, msiz, (char*)n->width);
    }
    if (n->customwidth) {
        msiz = strlen((const char*)n->customwidth) + 1;
        Cuw = (UINT8*)malloc(sizeof(UINT8) * msiz);
        strcpy_s((char*)Cuw, msiz, (char*)n->customwidth);
    }
    if (n->style) {
        msiz = strlen((const char*)n->style) + 1;
        STy = (UINT8*)malloc(sizeof(UINT8) * msiz);
        strcpy_s((char*)STy, msiz, (char*)n->style);
    }
    if (n->hidden) {
        msiz = strlen((const char*)n->hidden) + 1;
        Hi = (UINT8*)malloc(sizeof(UINT8) * msiz);
        strcpy_s((char*)Hi, msiz, (char*)n->hidden);
    }
    if (n->max) {
        msiz = strlen((const char*)n->max) + 1;
        maxcol = (UINT8*)malloc(sizeof(UINT8) * msiz);
        strcpy_s((char*)maxcol, msiz, (char*)n->max);
    }
    if (n->min) {
        msiz = strlen((const char*)n->min) + 1;
        mincol = (UINT8*)malloc(sizeof(UINT8) * msiz);
        strcpy_s((char*)mincol, msiz, (char*)n->min);
    }
    cls = addcolatyle(cls, nullptr, mincol, maxcol, wid, STy, Hi, Bf, Cuw);//cols 追加
}