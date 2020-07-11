#pragma once

#include <stdio.h>
#include<iostream>

class bitInOut {
public:
    unsigned char stocks[64] = { 0 };
    unsigned int bitoutvalue;
    unsigned short bitstartpos;//���ݓǂݍ��񂾎��̈ʒu
    unsigned short bitendpos;//�ǂݍ��񂾌�̈ʒu
    unsigned short stockValue;
    unsigned short bitpos;//�ǂݍ��݈ʒu�̋���

    bitInOut();
    void BigendOut(int setValue);
    void LittleendOut(int setValueR);

    void LittleendIn(int bi, int n);
    void BigendIn(int bir, int nr);
};