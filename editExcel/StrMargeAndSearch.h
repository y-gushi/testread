#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cctype>
#include "TagAndItems.h"
#include "ChangeArrayNumber.h"

class MargeaSearch {
public:
    MargeaSearch();

    ArrayNumber cn;//���l�@�z��ϊ�
    UINT8* tripleStrmarge(UINT8* left, UINT8* center, UINT8* right);//�R�̕����z�������
    ColorAnSize* splitColor(UINT8* cstr);//�F�@�T�C�Y�@����
    UINT8* margeRowTag(UINT32 r, UINT8* t, int* len);//�s���l�ƕ�����̌���
    UINT8* margeRowAndCol(UINT32 row, UINT32 col, UINT8* tag, int* strlen);//��s�����@���l���m
    int seachcharactors(UINT8* str, UINT8* data, UINT32 dataleng, int startpos);//�����񌟍�
    UINT8* charmarge(UINT8* a, UINT8* b, int al, int bl);//2 �����񌋍�
    int charlengsearch(UINT8* c);
    //UINT8* seachReverse(UINT8* c, UINT8* readdata, UINT64 dataleng, UINT32 startpos);//��v���镶������t����T��
};