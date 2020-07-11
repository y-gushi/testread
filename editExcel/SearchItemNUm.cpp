#include "SearchItemNUm.h"

searchItemNum::searchItemNum(Items* itemstruct, Ctags* cs) {
    its = itemstruct;
    Cels = cs;
    Mstr = MargeaSearch();
    rootMat = nullptr;
}

searchItemNum::~searchItemNum() {

}

bool searchItemNum::searchitemNumber(UINT8* uniq,inputtxt* iptxt,int sicounts) {
    Row* sr = nullptr;
    sr = Cels->rows;
    Items* Item = nullptr;
    Item = its;
    UINT8* nr = nullptr;//�������͍s�@�z��
    int result = 0;
    const char* SaT[] = { "s", "1221" };
    bool incellflag = false;
    UINT8* matchItem = nullptr;

    while (sr) {//�i�Ԍ���
        Item = its;
        while (Item) {
            if (sr->cells) {
                if (sr->cells->si) {
                    result = strcmp((const char*)sr->cells->si, (const char*)Item->itn);//�i�ԁ@�V�[�g�f�[�^�̔�r
                    if (result == 0) {
                        matchItem = Item->itn;//��v�i�΂�R�s�[
                        C* cn = nullptr;
                        cn = sr->next->cells;//�i�Ԏ��̍s
                        //�ŏI�񌟍�
                        while (cn->next) {
                            cn = cn->next;
                        }
                        inputColum = cn->col;//���ŏI��
                        changenum.ColumnIncliment(&inputColum);//�ŏI��@+1
                        nr = changenum.InttoChar(sr->next->r, &result);//���͍s
                        incolumn = changenum.ColumnNumtoArray(inputColum, &result);//�ŏI��@�z��ϊ�
                        startR = sr->next->r;//�i�Ԏ��̍s
                        incellflag = true;
                        break;
                    }
                }
            }
            Item = Item->next;
        }
        if (incellflag)
            break;
        sr = sr->next;
    }
    //���̓e�L�X�g�Ō�̎Q�Ƃ܂Ői�߂�
    if (iptxt) {
        while (iptxt->next)
            iptxt = iptxt->next;
    }
    
    if (!incellflag)
        return false;//�i�Ԉ�v�Ȃ�
    else {
        std::cout << "��v�i�� : " << Item->itn << std::endl;
        int rowslide = startR;
        Cels->addcelldata(nr, incolumn, (UINT8*)SaT[0], (UINT8*)SaT[1], uniq, nullptr, nullptr);//�ŏ��̈��ɕύX ���C���̓��t�ǉ�
        //�T�u������ǉ�
        if (iptxt) {
            //�T�u���͕���������΁@�Z���ɒǉ�
            while (iptxt) {//��납�����
                rowslide--;
                UINT8* srow = changenum.InttoChar(rowslide, &result);
                if (!iptxt->sinum)
                    return false;
                std::cout << "���͕��� : " << iptxt->sinum << std::endl;
                Cels->addcelldata(srow, incolumn, (UINT8*)SaT[0], (UINT8*)SaT[1], iptxt->sinum, nullptr, nullptr);
                iptxt = iptxt->parrent;
            }
        }
    }

    Item = its;

    while (Item) {//item �i�Ԉ�v�@�S�J���[
        sr = Cels->rows;
        result = strcmp((const char*)matchItem, (const char*)Item->itn);//�i�ԁ@�V�[�g�f�[�^�̔�r
        if (result == 0) {
            colorsearch(sr, Item, Item->itn);
        }
        Item = Item->next;
    }

    return incellflag;
}

void searchItemNum::colorsearch(Row* inrow, Items* IT, UINT8* itn) {
    const char* sizetable[] = { "090","100","110","120","130","140","150","160","F" };
    const char* style[] = { "1206","28","29" };
    UINT8 middle[] = "1206";
    Row* color = inrow;// = (Row*)malloc(sizeof(Row))
    UINT8* nextColor = (UINT8*)malloc(1); nextColor = nullptr;
    UINT8* nextSize = (UINT8*)malloc(1); nextSize = nullptr;
    UINT8* nowColor = (UINT8*)malloc(1); nowColor = nullptr;
    UINT8* nowSize = (UINT8*)malloc(1); nowSize = nullptr;
    UINT8* beforeColor = (UINT8*)malloc(1); beforeColor = nullptr;
    UINT8* beforeSize = (UINT8*)malloc(1); beforeSize = nullptr;
    ColorAnSize* CandS;
    Items* ite = (Items*)malloc(sizeof(Items));
    ite = IT;
    bool b = false;
    bool n = false;
    int result = 0;
    UINT32 i = 0;//�Z��������
    int matchflag = 0;//������@������v����
    unitC sear;//�S�p�ϊ�

    while (color) {//�X�^�[�g�ʒu�܂ňړ�
        if (color->r == startR)
            break;
        color = color->next;
    }

    if (color && color->next) {
        color = color->next;//�i�Ԏ��̍s
    }
    std::cout << "��v�i�� : " << itn << std::endl;

    while (color) {
        if (color->cells) {
            if (color->cells->si) {//������@����
                i = 0;
                while (color->cells->si[i] != '\0')//�������J�E���g
                    i++;

                //�S�p�ϊ�
                ite->col = sear.changenumber(ite->col);

                char* shcol = CharChenge(ite->col);
                //std::cout << "�p�b�L���O�@�S�p�ϊ��F" << shcol << std::endl;

                color->cells->si = sear.changeKana(color->cells->si);//���ȕϊ�
                color->cells->si = sear.changenumber(color->cells->si);

                ite->col = sear.slipNum(ite->col);//�A�������@�폜

                char* acol = CharChenge(color->cells->si);
                std::cout << "���ȑS�p�ϊ��F" << acol << std::endl;
                shcol = CharChenge(ite->col);
                std::cout << "�p�b�L���O�@�A�������@�폜�F" << shcol << std::endl;

                matchflag = Mstr.seachcharactors(ite->col, color->cells->si, i, 0);//�Z���J���[�ƃA�C�e���̔�r�@��������
                if (matchflag != -1) {
                    CandS = Mstr.splitColor(color->cells->si);//�J���[�ƃT�C�Y�̕���
                    if (CandS->color && CandS->size) {
                        nowColor = CandS->color; nowSize = CandS->size;
                    }
                    else {
                        nowColor = nullptr; nowSize = nullptr;
                    }
                    if (color->next) {
                        if (color->next->cells) {
                            if (color->next->cells->si) {
                                i = 0;
                                while (color->next->cells->si[i] != '\0')//�������J�E���g
                                    i++;
                                //���̐F�S�p���p�ϊ�
                                color->next->cells->si = sear.changenumber(color->next->cells->si);
                                matchflag = Mstr.seachcharactors(ite->col, color->next->cells->si, i, 0);//�Z���J���[�ƃA�C�e���̔�r�@��������
                                if (matchflag != -1) {
                                    ColorAnSize* nCandnS = Mstr.splitColor(color->next->cells->si);//�J���[�ƃT�C�Y�̕���
                                    nextColor = nCandnS->color;
                                    nextSize = nCandnS->size;
                                }
                                else {
                                    nextColor = nullptr;
                                    nextSize = nullptr;
                                }
                            }
                            else {
                                nextColor = nullptr; nextSize = nullptr;
                            }
                        }
                        else {
                            nextColor = nullptr; nextSize = nullptr;
                        }
                    }
                    else {
                        nextColor = nullptr; nextSize = nullptr;
                    }
                    if (nowColor && nowSize) {
                        result = strcmp((const char*)nowColor, (const char*)ite->col);//�J���[��r
                        if (result == 0) {//�J���[��v

                            for (int i = 0; i < 9; i++) {//�T�C�Y���[�v
                                result = strcmp((const char*)nowSize, (const char*)sizetable[i]);
                                if (result == 0) {//�T�C�Y��v
                                   
                                    UINT8* sizeMatch = nullptr;//�T�C�Y�ۑ��p
                                    switch (i) {//�Y���T�C�Y
                                    case 0:sizeMatch = ite->s90; break;
                                    case 1:sizeMatch = ite->s100; break;
                                    case 2:sizeMatch = ite->s110; break;
                                    case 3:sizeMatch = ite->s120; break;
                                    case 4:sizeMatch = ite->s130; break;
                                    case 5:sizeMatch = ite->s140; break;
                                    case 6:sizeMatch = ite->s150; break;
                                    case 7:sizeMatch = ite->s160; break;
                                    case 8:sizeMatch = ite->sF; break;
                                    default:
                                        sizeMatch=(UINT8*)malloc(1);
                                        sizeMatch = nullptr; 
                                        break;
                                    }
                                    if (sizeMatch) {
                                        rootMat = addmatches(rootMat, itn, nowColor);//�J���[�A�T�C�Y��v�ŕۑ�

                                        UINT8* nr = changenum.InttoChar(color->r, &result);
                                        Cels->addcelldata(nr, incolumn, nullptr, (UINT8*)style[0], sizeMatch, nullptr, nullptr);

                                        /*
                                        if (beforeColor) {
                                            result = strcmp((const char*)nowColor, (const char*)beforeColor);
                                            if (result == 0) {
                                                b = true;
                                            }
                                            else {
                                                b = false;
                                            }
                                        }
                                        else {
                                            b = false;
                                        }
                                        if (nextColor) {
                                            result = strcmp((const char*)nowColor, (const char*)nextColor);
                                            if (result == 0) {
                                                n = true;
                                            }
                                            else {
                                                n = false;
                                            }
                                        }
                                        else {
                                            n = false;
                                        }
                                        if (b && n) {//�^�񒆁@��������
                                            UINT8* nr = changenum.InttoChar(color->r, &result);
                                            Cels->addcelldata(nr, incolumn, nullptr, (UINT8*)style[1], sizeMatch, nullptr, nullptr);
                                        }
                                        else if (b && !n) {//�����@��������
                                            UINT8* nr = changenum.InttoChar(color->r, &result);
                                            Cels->addcelldata(nr, incolumn, nullptr, (UINT8*)style[2], sizeMatch, nullptr, nullptr);
                                        }
                                        else {//��@��������
                                            UINT8* nr = changenum.InttoChar(color->r, &result);
                                            Cels->addcelldata(nr, incolumn, nullptr, (UINT8*)style[0], sizeMatch, nullptr, nullptr);
                                        }
                                        */
                                    }
                                }
                            }
                        }
                    }
                }

            }
            if (color->cells->si) {
                beforeColor = nowColor;
                beforeSize = nowSize;
            }
            else {
                beforeColor = nullptr;
                beforeSize = nullptr;
            }
        }
        color = color->next;
    }
}

MatchColrs* searchItemNum::addmatches(MatchColrs* m, UINT8* i, UINT8* c)
{
    if (!m) {
        m = (MatchColrs*)malloc(sizeof(MatchColrs));
        if (m) {
            m->itemnum = i;
            m->color = c;
            m->next = nullptr;
        }
    }
    else {
        m->next = addmatches(m->next, i, c);
    }
    return m;
}

Items* searchItemNum::addItems(Items* base, Items* itm)
{
    if (!base) {
        base = (Items*)malloc(sizeof(Items));
        if (base) {
            base->itn = itm->itn;
            base->col = itm->col;
            base->next = nullptr;
        }
    }
    else {
        base->next = addItems(base->next, itm);
    }

    return base;
}
