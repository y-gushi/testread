#pragma once

#include "SearchItemNUm.h"
#include "TagAndItems.h"

class shipinfo {
public:
    shipinfo(Row* cel);

    ~shipinfo();

    const char* INt[11] = { "ITEM NO","DESCRIPTION/COL","90CM","100CM","110CM","120CM","130CM","140CM","150CM","160CM","F" };

    UINT32 ITcells[11] = { 0,0,0,0,0,0,0,0,0,0,0 };
    UINT32 ITEMrow = 0;
    UINT8* shipday = nullptr;
    UINT8* day = nullptr;
    int daylen = 0;

    Row* cells;
    MargeaSearch searchtag;

    Items* its = nullptr;

    void shipdataRead();//shipï∂éöåüçı
    UINT8* outuntil(UINT8 c, UINT8* st);

    struct Items* itemtalloc();
    struct Items* additem(Items* r, UINT8* num, UINT8* co, UINT8* ni, UINT8* te, UINT8* el, UINT8* tw, UINT8* th, UINT8* fo, UINT8* fi, UINT8* si, UINT8* f);
    bool searchitemCell();
    void GetItems();
    UINT8* StrInit();
    void freeItem(Items* t);
};