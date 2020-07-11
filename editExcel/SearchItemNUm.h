#pragma once

#include "StrMargeAndSearch.h"
#include "TagAndItems.h"
#include "RowColumn.h"
#include "unitchange.h"

struct MatchColrs
{
    UINT8* itemnum;
    UINT8* color;
    MatchColrs* next;
};

class searchItemNum {

public:

    searchItemNum(struct Items* itemstruct, Ctags* cs);
    ~searchItemNum();

    bool searchitemNumber(UINT8* uniq, char* one, char* two, char* three, char* four, char* style, char* celstyle);

    void colorsearch(Row* inrow, Items* IT, UINT8* itn, char* s);

    void freerootmacht(MatchColrs* m);

    UINT8* strtalloc(UINT8* s);

    UINT32 startR = 0;
    UINT32 inputColum = 0;
    UINT8* incolumn = nullptr;
    struct Items* its;
    Ctags* Cels;
    ArrayNumber changenum;
    MargeaSearch Mstr;
    MatchColrs* rootMat = nullptr;
    MatchColrs* addmatches(MatchColrs* m, UINT8* i, UINT8* c);
    Items* addItems(Items* base, Items* itm);
    char* CharChenge(UINT8* cc);
};
