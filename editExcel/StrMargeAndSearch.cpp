#include "StrMargeAndSearch.h"
#include "SearchItemNUm.h"
#include <iostream>

MargeaSearch::MargeaSearch()
{
}
//�R�̕����z�������
UINT8* MargeaSearch::tripleStrmarge(UINT8* left, UINT8* center, UINT8* right)
{
    int i = 0;
    int j = 0;
    int t = 0;

    while (left[i] != '\0')
        i++;
    while (center[j] != '\0')
        j++;
    while (right[t] != '\0')
        t++;
    UINT32 msize = (UINT32)i + (UINT32)j + (UINT32)t;
    UINT32 mlsize = msize + 1;
    UINT8* c = (UINT8*)malloc(sizeof(UINT8) * mlsize);

    if (c) {
        for (int h = 0; h < i; h++)
        {
            c[h] = left[h];
        }

        for (int k = 0; k < j; k++)
        {
            c[k + i] = center[k];
        }

        for (int l = 0; l < t; l++)
        {
            c[l + i + j] = right[l];
        }

        c[msize] = '\0';

        return c;
    }
    
    return nullptr;
}
//�F�@�T�C�Y�@����
ColorAnSize* MargeaSearch::splitColor(UINT8* cstr) {
    //�F�@�T�C�Y�@����
    int sp = 0; int spl = 0; int sc = 0;
    UINT32 widec = 0; UINT32 widec_t = 0;
    int i = 0;
    int strlen = 0;
    bool flag = false;
    int skakko = 0;
    int ekakko = 0;
    //UINT8** cands = nullptr;
    ColorAnSize* cas=(ColorAnSize*)malloc(sizeof(ColorAnSize));

    while (cstr[strlen] != '\0')
        strlen++;
    UINT32 msize = (UINT32)strlen + 1;
    //std::cout << "split �O : "<< strlen << std::endl;
    UINT8* split = (UINT8*)malloc(msize);
    UINT8* splitspace = (UINT8*)malloc(msize);
    UINT8* scol = (UINT8*)malloc(msize);
    UINT8* ssiz = (UINT8*)malloc(msize);

    if (split && scol && ssiz) {
        if (strlen > 6) {
            i = 0;
            while (cstr[spl] != '\0') {//�y�@�z�Ƃ΂� 3byte
                widec = 0;
                if (spl > 2) {
                    widec = (cstr[spl-2] << 16);
                    widec_t = (cstr[spl - 1] << 8);
                    widec += widec_t + cstr[spl];
                    widec = widec & (0xFFFFFF);
                }
                
                if (widec == 0xe38090) {
                    flag = true;
                    skakko = spl-2;
                    break;
                }
                spl++;
            }
        }
        if (flag) {
            while (i < skakko) {
                split[i] = cstr[i];
                i++;
            }
            while (widec != 0xe38091) {
                widec = 0;
                widec = (cstr[spl] << 16);
                widec_t = (cstr[spl + 1] << 8);
                widec += widec_t + cstr[spl + 2];
                widec = widec & (0xFFFFFF);
                spl++;
            }
            spl += 2;
            while (cstr[spl] != '\0') {
                split[i] = cstr[spl];
                i++; spl++;
            }
            flag = false;
        }
        else {
            while (cstr[i] != '\0') {
                widec = 0;
                split[i] = cstr[i];
                i++;
            }
        }
        split[i] = '\0';

        //�S�p���؁[���@E38080 �폜
        i = 0;
        int j = 0;
        while (split[i] != '\0') {
            widec = 0;
            if (i > 2) {
                widec = (cstr[i - 2] << 16);
                widec += (cstr[i - 1] << 8);
                widec += cstr[i];
                widec = widec & (0xFFFFFF);
            }
            if (widec == 0xe38080) {//�S�p�X�y�[�X
                //2�O�߂� ���
                j -= 3;
            }
            else {
                splitspace[j] = split[i];
            }
            j++; i++;
        }
        splitspace[j] = '\0';

        //std::cout << "color : " << split << std::endl;
        searchItemNum itemchange(nullptr);

        spl = 0;

        sc = strlen;
        while (splitspace[spl] != '\0') {//�T�C�Y�I���ʒu
            if (splitspace[spl] == 'c' && splitspace[spl + 1] == 'm')
                break;
            spl++;
        }

        int colorend = 0;//�J���[�I���ʒu

        if (splitspace[spl - 1] == 'F') {
            cas->size = (UINT8*)malloc(2);// �T�C�YF
            cas->size[0] = 'F'; cas->size[1] = '\0';
            colorend = spl - 1;
        }
        else {
            int pos = spl;//�T�C�Y�I���ʒu
            spl--;
            while (std::isdigit(splitspace[spl])) {//�����̊�
                spl--;
            }
            colorend = spl + 1;
            sc = pos - (spl+1);
            msize = (UINT32)sc + 1;
            cas->size = (UINT8*)malloc(msize);
            //�T�C�Y�擾
            spl++;
            sc = 0;
            while (spl < pos) {
                if(!std::isspace(splitspace[spl]))
                    cas->size[sc] = splitspace[spl];
                sc++; spl++;
            }
            cas->size[sc] = '\0';
        }        
        //std::cout << "�T�C�Y�@���� : " << cas->size<<" "<<spl << std::endl;
        //�J���[�擾
        spl = 0;
        while (spl<colorend) {// ()�Ƃ΂�
            if (splitspace[spl] == '(') {
                while (splitspace[spl] != ')')
                    spl++;
            }
            /*if (std::isdigit(split[spl]) && spl > 2) {//����
                ssiz[sc] = split[spl]; sc++;
            }*/
            else if (!isspace(splitspace[spl]) && splitspace[spl] != '#' && splitspace[spl] != ')') {//�X�y�[�X & #
                scol[sp] = splitspace[spl]; sp++;
            }
            if (spl != colorend)
                spl++;
        }
        scol[sp] = '\0';

        /*while (splitspace[sp - 1] == 128 || splitspace[sp - 1] == 227) {// �X�y�[�X�Ƃ΂�
            sp--;
        }*/        

        msize = (UINT32)sp + 1;
        cas->color = (UINT8*)malloc(msize);

        if (cas->color) {

            for (int k = 0; k < sp; k++)
                cas->color[k] = scol[k];
            cas->color[sp] = '\0';

            std::string color = itemchange.CharChenge(cas->color);
            std::cout << "split���� : " << color << " size : " << cas->size << std::endl;

            free(scol); free(split); free(ssiz); free(splitspace);
        }
    }

    return cas;
}
//�s���l�ƕ�����̌���
UINT8* MargeaSearch::margeRowTag(UINT32 r, UINT8* t, int* len)
{
    unsigned char* row_num = nullptr;
    int row_place = 0;
    //�s�ԍ���z���
    row_num = cn.InttoChar(r, &row_place);

    int i = 0;
    while (t[i] != '\0')
        i++;
    row_num = charmarge(t, row_num, i, row_place);

    *len = i + row_place;

    return row_num;
}
//�����񌟍�
int MargeaSearch::seachcharactors(UINT8* str, UINT8* data, UINT32 dataleng, int startpos)
{
    int i = 0;
    while (str[i] != '\0')
        i++;//������̐�
    UINT32 msize = (UINT32)i + 1;
    UINT8* sp = (UINT8*)malloc(msize);//�����X���C�h�p
    memset(sp, 0, msize);

    UINT32 p = (UINT32)startpos;

    while (p < dataleng) {
        for (int j = 0; j < i - 1; j++) {
            sp[j] = sp[j + 1];
        }
        sp[i - 1] = data[p];//�Ō�ɕt��������
        sp[i] = '\0';//������I�[��ǉ�
        p++;//�ʒu�ړ�

        int result = strncmp((const char*)sp, (const char*)str, i);
        if (result == 0) {
            //std::cout << "strings match" << c << std::endl;
            free(sp);
            return p;
        }

    }

    free(sp);
    return -1;
}
//2 �����񌋍�
UINT8* MargeaSearch::charmarge(UINT8* a, UINT8* b, int al, int bl)
{
    UINT32 msize = (UINT32)al + (UINT32)bl + 1;
    UINT8* c = (UINT8*)malloc(msize);

    for (int i = 0; i < al; i++)
    {
        c[i] = a[i];
    }

    for (int i = 0; i < bl; i++)
    {
        c[i + al] = b[i];
    }

    c[al + bl] = '\0';

    return c;
}
//��s�����@���l���m
UINT8* MargeaSearch::margeRowAndCol(UINT32 row, UINT32 col, UINT8* tag, int* strlen)
{
    unsigned char* column = nullptr;

    unsigned char* rowandcol = nullptr;
    unsigned char* tagandrc = nullptr;

    int col_place = 0;
    //��ԍ���char�z���
    column = cn.ColumnNumtoArray(col, &col_place);

    unsigned char* row_num = nullptr;
    int row_place = 0;

    //�s�ԍ���z���
    row_num = cn.InttoChar(row, &row_place);

    //�s�񌋍�
    rowandcol = charmarge(column, row_num, col_place, row_place);

    int i = 0;
    int j = 0;

    while (tag[i] != '\0')
        i++;

    while (rowandcol[j] != '\0')
        j++;

    //tag ����
    tagandrc = charmarge(tag, rowandcol, i, j);
    //������̕�����
    *strlen = i + j;

    return tagandrc;
}
//�������J�E���g
int MargeaSearch::charlengsearch(UINT8* c)
{
    int i = 0;
    while (c[i] != '\0')
        i++;
    return i;
}
//��v���镶������t����T���@���̈ʒu��Ԃ� �Ȃ���΁@nullptr
UINT8* seachReverse(UINT8* c, UINT8* readdata, UINT64 dataleng, UINT32 startpos)
{
    int i = 0;
    //������̐�
    while (c[i] != '\0')
        i++;
    UINT32 msize = (UINT32)i + 1;
    UINT8* searchpos = (UINT8*)malloc(msize);//�����X���C�h�p

    int datapos = startpos;

    if (searchpos) {
        while (datapos > 0) {
            for (int j = i - 1; j > 0; j--) {
                searchpos[j] = searchpos[j - 1];
            }
            searchpos[0] = readdata[datapos];//�Ō�ɕt��������
            searchpos[i] = '\0';//������I�[��ǉ�
            datapos--;//�ʒu�ړ�
            msize = (UINT32)i - 1;

            int result = strncmp((char const*)searchpos, (char const*)c, msize);
            if (result == 0) {
                datapos += i + 1;
                for (int k = 0; k < 6; k++) {
                    searchpos[i + k] = readdata[datapos];
                    datapos++;
                }
                searchpos[i + 6] = '\0';
                return searchpos;
            }
        }
    }
    
    return nullptr;
}