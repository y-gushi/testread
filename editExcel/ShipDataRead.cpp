#include "ShipDataRead.h"

shipinfo::shipinfo(Row* cel) {
    cells = cel;
    its = nullptr;
    day = nullptr;
    shipday = nullptr;
}

shipinfo::~shipinfo() {
    freeits();
    //free(shipday);//元で解放 no malloc
    //free(day);
    //セル引数　元で解放 cel no malloc
}

void shipinfo::shipdataRead() {
    Row* r = cells;
    UINT32 shipR = 7;
    UINT32 shipC = 'C';
    bool shipFlag = false;
    int i = 0;

    while (r) {//C7 cell get
        if (r->r == shipR) {
            while (r->cells) {
                if (r->cells->col == shipC) {
                    if (r->cells->si) {
                        shipday = r->cells->si;
                        shipFlag = true;
                    }
                    else {
                        std::cout << "this cell not have shipdata" << std::endl;
                    }
                }
                r->cells = r->cells->next;
            }
        }
        r = r->next;
    }
    if (shipFlag) {
        day = (UINT8*)malloc(7);//日付だけ
        if (day) {
            day[0] = 'P'; day[1] = 'L';

            while (shipday[i] != '-' && shipday[i] != '\0' && shipday[i] != ' ') {
                if (i > 3) {
                    day[i - 2] = shipday[i];
                }
                i++;
            }
            day[6] = '\0';
            daylen = 6;
        }
    }
}

//指定の文字までの文字列　削る
UINT8* shipinfo::outuntil(UINT8 c, UINT8* st)
{
    int j = 0;
    int i = 0;
    UINT8* p = nullptr;

    while (st[j] != c && st[j] != '\0')
        j++;

    const UINT32 msize = (UINT32)j + 1;
    p = (UINT8*)malloc(msize);

    if (j == 0) {//最初に　(　があったら　)が読み始め
        while (st[j] != ')')
            j++;
        j++;
        int startpos = 0;
        while (st[j] != '\0') {
            if (!isspace(st[j])) {
                p[startpos] = st[j];
                startpos++;
            }
            j++;
        }
        p[startpos] = '\0';
    }
    else {
        int t = 0; int h = 0;
        if (p) {
            while (h < j) {
                if (!isspace(st[h])) {
                    p[t] = st[h];
                    t++;
                }
                h++;
            }
            p[t] = '\0';
        }
    }
    free(st);
    st = nullptr;

    return p;
}

Items* shipinfo::itemtalloc() {
    return (Items*)malloc(sizeof(Items));
}

Items* shipinfo::additem(Items* r, UINT8* num, UINT8* co, UINT8* ni, UINT8* te, UINT8* el, UINT8* tw, UINT8* th, UINT8* fo, UINT8* fi, UINT8* si, UINT8* f)
{
    if (!r) {
        r = (Items*)malloc(sizeof(Items));
        r->itn = sistrcopy(num);
        r->col = sistrcopy(co);
        r->s90 = sistrcopy(ni);
        r->s100 = sistrcopy(te);
        r->s110 = sistrcopy(el);
        r->s120 = sistrcopy(tw);
        r->s130 = sistrcopy(th);
        r->s140 = sistrcopy(fo);
        r->s150 = sistrcopy(fi);
        r->s160 = sistrcopy(si);
        r->sF = sistrcopy(f);
        r->next = nullptr;
    }
    else {
        r->next = additem(r->next, num, co, ni, te, el, tw, th, fo, fi, si, f);
    }
    return r;
}

bool shipinfo::searchitemCell() {
    Row* sr = cells;

    while (sr) {
        while (sr->cells) {
            if (sr->cells->si) {
                if (strcmp((const char*)sr->cells->si, INt[0]) == 0) {
                    ITcells[0] = sr->cells->col; ITEMrow = sr->r;
                }
                else if (strcmp((const char*)sr->cells->si, INt[1]) == 0) {
                    ITcells[1] = sr->cells->col;
                }
                else if (strcmp((const char*)sr->cells->si, INt[2]) == 0) {
                    ITcells[2] = sr->cells->col;
                }
                else if (strcmp((const char*)sr->cells->si, INt[3]) == 0) {
                    ITcells[3] = sr->cells->col;
                }
                else if (strcmp((const char*)sr->cells->si, INt[4]) == 0) {
                    ITcells[4] = sr->cells->col;
                }
                else if (strcmp((const char*)sr->cells->si, INt[5]) == 0) {
                    ITcells[5] = sr->cells->col;
                }
                else if (strcmp((const char*)sr->cells->si, INt[6]) == 0) {
                    ITcells[6] = sr->cells->col;
                }
                else if (strcmp((const char*)sr->cells->si, INt[7]) == 0) {
                    ITcells[7] = sr->cells->col;
                }
                else if (strcmp((const char*)sr->cells->si, INt[8]) == 0) {
                    ITcells[8] = sr->cells->col;
                }
                else if (strcmp((const char*)sr->cells->si, INt[9]) == 0) {
                    ITcells[9] = sr->cells->col;
                }
                else if (strcmp((const char*)sr->cells->si, INt[10]) == 0) {
                    ITcells[10] = sr->cells->col;
                    return 1;
                }
            }
            sr->cells = sr->cells->next;
        }
        sr = sr->next;
    }
    return 0;
}

void shipinfo::GetItems() {
    Row* sr = cells;
    bool itemnumFlag = false;

    UINT8* INt = nullptr;
    UINT8* Colo = nullptr;
    UINT8* nine = nullptr;
    UINT8* ten = nullptr;
    UINT8* ele = nullptr;
    UINT8* twe = nullptr;
    UINT8* thr = nullptr;
    UINT8* four = nullptr;
    UINT8* fif = nullptr;
    UINT8* six = nullptr;
    UINT8* f = nullptr;

    //shipdataRead();

    searchitemCell();

    while (sr) {
        if (sr->r == ITEMrow)
            break;//スタートまでとばす
        sr = sr->next;
    }

    int j = 0;
    while (sr) {
        while (sr->cells) {

            if (sr->cells->col == ITcells[0] && ITcells[0] != 0) {//numbe
                if (sr->cells->si) {
                    sr->cells->si = outuntil('(', sr->cells->si);
                    INt = sr->cells->si;
                    itemnumFlag = true;
                }
            }
            if (sr->cells->col == ITcells[1] && itemnumFlag && ITcells[1] != 0) {//color
                if (sr->cells->si) {
                    sr->cells->si = outuntil('(', sr->cells->si);
                    Colo = sr->cells->si;
                }
                else if (sr->cells->val) {
                    Colo = sr->cells->val;
                }
            }
            if (sr->cells->col == ITcells[2] && itemnumFlag && ITcells[2] != 0) {
                if (sr->cells->si) {
                    nine = sr->cells->si;
                }
                else if (sr->cells->val) {
                    nine = sr->cells->val;
                }
            }
            if (sr->cells->col == ITcells[3] && itemnumFlag && ITcells[3] != 0) {
                if (sr->cells->si) {
                    ten = sr->cells->si;
                }
                else if (sr->cells->val) {
                    ten = sr->cells->val;
                }
            }
            if (sr->cells->col == ITcells[4] && itemnumFlag && ITcells[4] != 0) {
                if (sr->cells->si) {
                    ele = sr->cells->si;
                }
                else if (sr->cells->val) {
                    ele = sr->cells->val;
                }
            }
            if (sr->cells->col == ITcells[5] && itemnumFlag && ITcells[5] != 0) {
                if (sr->cells->si) {
                    twe = sr->cells->si;
                }
                else if (sr->cells->val) {
                    twe = sr->cells->val;
                }
            }
            if (sr->cells->col == ITcells[6] && itemnumFlag && ITcells[6] != 0) {
                if (sr->cells->si) {
                    thr = sr->cells->si;
                }
                else if (sr->cells->val) {
                    thr = sr->cells->val;
                }
            }
            if (sr->cells->col == ITcells[7] && itemnumFlag && ITcells[7] != 0) {
                if (sr->cells->si) {
                    four = sr->cells->si;
                }
                else if (sr->cells->val) {
                    four = sr->cells->val;
                }
            }
            if (sr->cells->col == ITcells[8] && itemnumFlag && ITcells[8] != 0) {
                if (sr->cells->si) {
                    fif = sr->cells->si;
                }
                else if (sr->cells->val) {
                    fif = sr->cells->val;
                }
            }
            if (sr->cells->col == ITcells[9] && itemnumFlag && ITcells[9] != 0) {
                if (sr->cells->si) {
                    six = sr->cells->si;
                }
                else if (sr->cells->val) {
                    six = sr->cells->val;
                }
            }
            if (sr->cells->col == ITcells[10] && itemnumFlag && ITcells[10] != 0) {
                if (sr->cells->si) {
                    f = sr->cells->si;
                }
                else if (sr->cells->val) {
                    f = sr->cells->val;
                }
            }
            sr->cells = sr->cells->next;
        }
        itemnumFlag = false;
        if (INt && Colo && nine && ten && ele && twe && thr && four && fif && six && f) {
            its = additem(its, INt, Colo, nine, ten, ele, twe, thr, four, fif, six, f);
            INt = nullptr; Colo = nullptr; nine = nullptr; ten = nullptr;; ele = nullptr;
            twe = nullptr; thr = nullptr; four = nullptr; fif = nullptr; six = nullptr; f = nullptr;
        }
        else {
            INt = nullptr; Colo = nullptr; nine = nullptr; ten = nullptr;; ele = nullptr;
            twe = nullptr; thr = nullptr; four = nullptr; fif = nullptr; six = nullptr; f = nullptr;
        }
        sr = sr->next;
    }
}

UINT8* shipinfo::sistrcopy(UINT8* s) {
    size_t ssiz = strlen((const char*)s);
    UINT8* sistr = nullptr;

    if (ssiz > 0) {
        ssiz += 1;
        sistr = (UINT8*)malloc(ssiz);
        strcpy_s((char*)sistr, ssiz, (char*)s);
    }

    return sistr;
}

void shipinfo::freeits() {
    struct Items* q;
    while (its) {
        q = its->next;  /* 次へのポインタを保存 */
        
        free(its);
        its = q;
    }
}