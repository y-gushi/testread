#pragma once

#include <stdio.h>
#include "typechange.h"
#include <stdlib.h>
#include <math.h>

class ArrayNumber {
public:
    UINT8* InttoChar(UINT32 num, int* pl);//�s��int��z���

    UINT32 ColumnArraytoNumber(UINT8* colum, int colplace);//��z��𐔎��ցi�������l�j

    UINT8* ColumnNumtoArray(UINT32 col, int* colplace);//��int32��z���

    UINT32 ColumnCharnumtoNumber(UINT32 col);//��@���������𐔎��ɕϊ� ���cols�p

    void ColumnIncliment(UINT32* c);//int�̗�ԍ���1+

    UINT32 NumbertoArray(UINT32 colnumber);//�������當��������

    UINT32 declimentCol(UINT32 c);//�������l����炷

    UINT32 RowArraytoNum(UINT8* row, int rowpl);//�s�z��𐔎���
};
