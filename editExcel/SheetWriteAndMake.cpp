#include "RowColumn.h"

//�Z���f�[�^�̒ǉ��@row span�X�V
void Ctags::addcelldata(UINT8* row, UINT8* col, UINT8* t, UINT8* s, UINT8* v, F* f, UINT8* si) {
    Row* ro = nullptr;
    bool replace = false;
    int rp = 0;//����
    UINT8 nomals[] = "1402";

    while (row[rp] != '\0')
        rp++;
    UINT32 rn = NA.RowArraytoNum(row, rp);//�s�z�񂩂琔��

    rp = 0;//����
    while (col[rp] != '\0')
        rp++;
    UINT32 cn = NA.ColumnArraytoNumber(col, rp);//span�悤�@�񕶎�����

    if (cn > maxcol) {//diment �ő�l�X�V
        maxcol = cn;
        dm->eC = NA.ColumnNumtoArray(maxcol, &rp);
        replace = true;
    }

    ro = searchRow(rows, rn);//row�ʒu����
    if (!ro) {
        //std::cout << "row �Ȃ� �ǉ�" << std::endl;
        int cnpl = 0;//�����@�g��Ȃ�

        UINT32 newcn = NA.ColumnCharnumtoNumber(cn);//��ԍ��A�Ԃց@���ꂽ��
        UINT8* charcn = NA.InttoChar(newcn, &cnpl);//int��z���

        size_t spsiz = cnpl + 1;
        UINT8* spanSt = (UINT8*)malloc(spsiz);//�s�X�^�[�g�ʒu
        strcpy_s((char*)spanSt, spsiz, (char*)charcn);

        UINT8* spanEn = (UINT8*)malloc(spsiz);//�s�I���
        strcpy_s((char*)spanEn, spsiz, (char*)charcn);
        free(charcn);

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

        //cell null �f�[�^�����
        //std::cout << " �Z���ǉ� row�Ȃ� : " << ro->r << " �s " << cn << std::endl;
        //incolnum = NA.NumbertoArray(colnum_start);//�����𕶎�������
        ro->cells = addCtable(ro->cells, t, s, si, cn, v, f);
    }
    else {
        //row����
        //std::cout << " row ����@�ǉ� v : " << v << std::endl;

        //�����cell�̑O���Ȃ���Γ����
        C* Croot = ro->cells;
        while (Croot->next) {
            Croot = Croot->next;
        }

        UINT32 incolnum = Croot->col;//���݂̍ŏI�s
        NA.ColumnIncliment(&incolnum);

        while (incolnum <= cn) {
            if (incolnum == cn) {
                //std::cout << " �Z���ǉ� �O����@row : " << ro->r << " �s " << incolnum << std::endl;
                //incolnum = NA.NumbertoArray(colnum_start);//�����𕶎�������
                ro->cells = addCtable(ro->cells, t, s, si, incolnum, v, f);
            }
            else {
                //std::cout << " �Z���ǉ��@row : " << ro->r << " �s " << incolnum << " s " << s << std::endl;
                //incolnum = NA.NumbertoArray(colnum_start);//�����𕶎�������
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

    //ro->cells = addCtable(ro->cells, t, s, si, cn, v, f);//�Z����񌟍�
    cn = NA.ColumnCharnumtoNumber(cn);//��ԍ��A�Ԃց@���ꂽ��

    if (replace) {//�ő�l�X�V�Ł@cols style�m�F
        cols* ncols = cls;
        while (ncols->next)//cols�@�Ō�Q��
            ncols = ncols->next;
        int j = 0;
        while (ncols->max[j] != '\0')
            j++;
        UINT32 ncn = NA.RowArraytoNum(ncols->max, j);//�ŏImax������ �����ɕϊ�
        if (cn > ncn) {//cols max ������
            UINT8 wi[] = "9"; UINT8 styl[] = "1";
            size_t colmemsiz = strlen((const char*)col)+1;
            UINT8* mincol = (UINT8*)malloc(colmemsiz);
            strcpy_s((char*)mincol, colmemsiz, (const char*)col);

            UINT8* maxcol = (UINT8*)malloc(colmemsiz);
            strcpy_s((char*)maxcol, colmemsiz, (const char*)col);

            UINT8* Hi = nullptr;
            UINT8* Bf = nullptr;
            UINT8* Cuw = nullptr;

            cls = addcolatyle(cls, mincol, maxcol, wi, styl, Hi, Bf, Cuw);//cols �ǉ�
        }
    }

    rp = 0;//����
    while (ro->spanE[rp] != '\0')
        rp++;
    UINT32 nnc = NA.RowArraytoNum(ro->spanE, rp);//span �����񐔎���
    if (cn > nnc) {//�V���������񂪑傫���ꍇ�@�X�V
        UINT8* newspan = NA.InttoChar(cn, &rp);
        ro->spanE = newspan;
        //std::cout << " new span : " << ro->spanE << std::endl;
    }
}

void Ctags::writesheetdata() {
    size_t msize = size_t(dlen) + 6000;

    wd = (UINT8*)malloc(msize);//�������T�C�Y�ύX�@�������ݗp
    //std::cout << "�f�[�^�X�V" << p << std::endl;
    p = 0;

    if (wd) {
        while (headXML[p] != '\0') {
            wd[p] = headXML[p];
            p++;
        }
    }
    writeheadpart();
    writeDiment();
    writeSelection();
    writecols();
    writecells();
    writefinal();

    //std::cout << "size : " << p << " dlen : " << dlen << std::endl;
}

void Ctags::writeheadpart() {
    const char* shpr[] = { "<sheetPr>","<tabColor","rgb=\""," tint=\"","<pageSetUpPr"," fitToPage=\"","</sheetPr>" };
    UINT8 clo = '>';
    UINT8 sla[] = "/>";

    // <sheetPr write
    oneStrwrite((UINT8*)shpr[0]);
    if (Pr) {
        oneStrwrite((UINT8*)shpr[1]);
        if (Pr->rgb)
            oneStrplusDoubleq((UINT8*)shpr[2], Pr->rgb);
        if (Pr->tint)
            oneStrplusDoubleq((UINT8*)shpr[3], Pr->tint);
        oneStrwrite(sla);

        if (Pr->fitToPage) {
            oneStrwrite((UINT8*)shpr[4]);
            oneStrplusDoubleq((UINT8*)shpr[2], Pr->fitToPage);
            oneStrwrite(sla);
        }
    }
    oneStrwrite((UINT8*)shpr[6]);
}

//diment��������
void Ctags::writeDiment() {
    const char* dimstart = "<dimension ref=\"";
    //std::cout << "�f�B�����g�X�V" << p << std::endl;
    if (dm) {
        while (dimstart[writep] != '\0') {
            wd[p] = dimstart[writep]; p++; writep++;
        }writep = 0;
        if (dm->sC) {
            while (dm->sC[writep] != '\0') {//�X�^�[�g��
                wd[p] = dm->sC[writep]; p++; writep++;
            }writep = 0;
        }
        if (dm->sR) {
            while (dm->sR[writep] != '\0') {//�X�^�[�g�s
                wd[p] = dm->sR[writep]; p++; writep++;
            }writep = 0;
        }
        wd[p] = ':'; p++;
        if (dm->eC) {
            while (dm->eC[writep] != '\0') {//�I����
                wd[p] = dm->eC[writep]; p++; writep++;
            }writep = 0;
        }
        if (dm->eR) {
            while (dm->eR[writep] != '\0') {//�I���s
                wd[p] = dm->eR[writep]; p++; writep++;
            }writep = 0;
        }
    }
    
    //std::cout << "�f�B�����g�X�V" << dm->eC << std::endl;
}
//cols��������
void Ctags::writecols() {
    const char* startcols = "<cols>";
    const char* colstr[] = { "<col min=\"","\" max=\"","\" width=\"","\" style=\""
        ,"\" bestFit=\"","\" hidden=\"","\" customWidth=\"" };

    //std::cout << "�R���X�X�V" << p << std::endl;
    const char* endcols = "</cols>";
    int writep = 0;

    cols* Col = cls;
    if (Col) {
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
    
}
//selection pane��������
void Ctags::writeSelection() {
    const char* selpane[] = { "<selection"," activeCell=\"","\" sqref=\"","</sheetView>","</sheetViews>", " pane=\"" ,"<sheetViews>","<sheetView"};
    const char* svvalue[] = { " zoomScaleNormal=\""," workbookViewId=\""," tabSelected=\""};
    const char* panes[] = { "<pane"," xSplit=\""," ySplit=\""," topLeftCell=\""," activePane=\""," state=\"" };
    UINT8 clo = '>';
    UINT8 sla[] = "/>";

    // <sheetview��������
    oneStrwrite((UINT8*)selpane[6]);
    if (ShV) {
        oneStrwrite((UINT8*)selpane[7]);
        if (ShV->zoomScaleNormal)
            oneStrplusDoubleq((UINT8*)svvalue[0], ShV->zoomScaleNormal);
        if (ShV->workbookViewId)
            oneStrplusDoubleq((UINT8*)svvalue[1], ShV->workbookViewId);
        if (ShV->tabSelected)
            oneStrplusDoubleq((UINT8*)svvalue[2], ShV->tabSelected);
        wd[p] = clo; p++;
    }
    else
        oneStrwrite(sla);

    Pane* Pa = ShV->pan;
    // <pane ��������
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

    selection* Sel = ShV->selec;
    // <selection ��������
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
    if (ShV)
        oneStrwrite((UINT8*)selpane[3]);// </sheetView>

    oneStrwrite((UINT8*)selpane[4]);

    // <sheetFormatPr write
    while (sFPr[writep] != '\0') {
        wd[p] = sFPr[writep]; p++; writep++;
    }writep = 0;
}
//�Z���f�[�^��������
void Ctags::writecells() {
    const char* sheetstart = "<sheetData>";
    const char* shend = "</sheetData>";
    std::cout << "�Z���X�V" << p << std::endl;

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
        UINT8* RN = NA.InttoChar(Ro->r, &Place);//�����@�z��ϊ�
        while (RN[writep] != '\0') {// r
            wd[p] = RN[writep]; p++; writep++;
        }writep = 0;
        if (Ro->spanS) {//�Ȃ��ꍇ����
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

        UINT8* rne = NA.InttoChar(Ro->r, &Place);//�z��ɕύX
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
// c�^�O�@��������
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
        while (Col[writep] != '\0') {//r ��
            wd[p] = Col[writep]; p++; writep++;
        }writep = 0;
        while (ROW[writep] != '\0') {//r �s
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
        if (cl->t) {//t ����
            while (ctagt[writep] != '\0') {// t
                wd[p] = ctagt[writep]; p++; writep++;
            }writep = 0;
            while (cl->t[writep] != '\0') {// t
                wd[p] = cl->t[writep]; p++; writep++;
            }writep = 0;
        }
        if (cl->f) {//F����
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
            if (cl->f->val) {//f v���� �v�Z��
                if (cl->f->t) {//t���� ">�� v
                    wd[p] = '"';
                    p++;
                }
                wd[p] = '>';//t�Ȃ�
                p++;

                while (cl->f->val[writep] != '\0') {//f val
                    wd[p] = cl->f->val[writep]; p++; writep++;
                }writep = 0;
                while (ftagend[writep] != '\0') {// f end
                    wd[p] = ftagend[writep]; p++; writep++;
                }writep = 0;
            }
            else {
                while (closetag[writep] != '\0') {//v�Ȃ� "/>�Ƃ�
                    wd[p] = closetag[writep]; p++; writep++;
                }writep = 0;
            }
        }
        if (cl->val) {//V ����
            if (!cl->f) {//f�Ȃ��ꍇ����
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
        else {//V �Ȃ��@�N���[�Y
            //<c r="JK13" s="1525" t="str">
            //<f t="shared" ref="JK13:JK18" si="7">IF(JI13=JJ13,"","�~")</f>
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
//�ŏI�����񏑂�����
void Ctags::writefinal() {
    while (fstr[writep] != '\0') {// /v
        wd[p] = fstr[writep]; p++; writep++;
    }writep = 0;
}

void Ctags::oneStrplusDoubleq(UINT8* str, UINT8* v) {

    int i = 0;
    UINT8 d = '"';

    while (str[i] != '\0') {
        wd[p] = str[i];
        i++;
        p++;
    }
    i = 0;

    while (v[i] != '\0') {
        wd[p] = v[i];
        i++; p++;
    }

    wd[p] = d;
    p++;
}

//tag��������
void Ctags::oneStrwrite(UINT8* str) {

    int i = 0;

    while (str[i] != '\0') {
        wd[p] = str[i];
        i++;
        p++;
    }
}