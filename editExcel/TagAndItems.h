#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "typechange.h"

struct worksheetV {
    UINT8* xmlns;
    UINT8* r;
    UINT8* mc;
    UINT8* x14ac;
    UINT8* xr;
    UINT8* xr2;
    UINT8* xr3;
    UINT8* Ignorable;
    UINT8* uid;
};

struct SheetPr {
    UINT8* tint;
    UINT8* rgb;
    UINT8* fitToPage;
};
//-----------------------
struct cfvo {
    UINT8* type;
    UINT8* val;
};

struct cfcolor {
    UINT8* rgb;
    UINT8* theme;
};

struct cfRule {
    UINT8* type;
    UINT8* priority;
    UINT8* dxfId;
    UINT8* opara;
    UINT8* formula;
    cfvo* vo;
    cfcolor* ccolor;
};
//------------------------
struct Items {
    UINT8* itn = nullptr;
    UINT8* col = nullptr;
    UINT8* s90 = nullptr;
    UINT8* s100 = nullptr;
    UINT8* s110 = nullptr;
    UINT8* s120 = nullptr;
    UINT8* s130 = nullptr;
    UINT8* s140 = nullptr;
    UINT8* s150 = nullptr;
    UINT8* s160 = nullptr;
    UINT8* sF = nullptr;
    Items* next;
};

struct F {
    UINT8* val = nullptr;
    UINT8* t = nullptr;
    UINT8* ref = nullptr;
    UINT8* si = nullptr;
};

struct C {
    UINT8* val = nullptr;
    UINT8* t = nullptr;
    UINT8* s = nullptr;
    UINT8* si = nullptr;
    UINT32 col = 0;
    F* f = nullptr;
    C* next;
};

struct Row {
    UINT32 r = 0;
    UINT8* spanS = nullptr;
    UINT8* spanE = nullptr;
    UINT8* ht = nullptr;
    UINT8* thickBot = nullptr;
    UINT8* s = nullptr;
    UINT8* customFormat = nullptr;
    UINT8* customHeight = nullptr;
    C* cells = nullptr;
    Row* next = nullptr;
};

struct cols {
    UINT8* min = nullptr;
    UINT8* max = nullptr;
    UINT8* width = nullptr;
    UINT8* style = nullptr;
    UINT8* hidden = nullptr;
    UINT8* bestffit = nullptr;
    UINT8* customwidth = nullptr;
    cols* next;
};

struct selection {
    UINT8* p = nullptr;
    UINT8* a = nullptr;
    UINT8* s = nullptr;
    selection* next;
};

struct demention {
    UINT8* sC = nullptr;
    UINT8* sR = nullptr;
    UINT8* eC = nullptr;
    UINT8* eR = nullptr;
};

struct Pane {
    UINT8* xSp = nullptr;
    UINT8* ySp = nullptr;
    UINT8* tLeftC = nullptr;
    UINT8* activP = nullptr;
    UINT8* state = nullptr;
    Pane* next = nullptr;
};

struct ColorAnSize {
    UINT8* color;
    UINT8* size;
};

struct MargeCell {
    UINT8* scell;
    UINT8* ecell;
    MargeCell* next;
};

struct SheetViews {
    UINT8* zoomScaleNormal;
    UINT8* workbookViewId;
    UINT8* tabSelected;
    UINT8* workbookViewId;
    Pane* pan;
    selection* selec;
};