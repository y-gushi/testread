#pragma once

#include <stdio.h>
#include<iostream>

class bitInOut {
public:
    unsigned char stocks[64] = { 0 };
    unsigned int bitoutvalue;
    unsigned short bitstartpos;//Œ»İ“Ç‚İ‚ñ‚¾‚ÌˆÊ’u
    unsigned short bitendpos;//“Ç‚İ‚ñ‚¾Œã‚ÌˆÊ’u
    unsigned short stockValue;
    unsigned short bitpos;//“Ç‚İ‚İˆÊ’u‚Ì‹——£

    bitInOut();
    void BigendOut(int setValue);
    void LittleendOut(int setValueR);

    void LittleendIn(int bi, int n);
    void BigendIn(int bir, int nr);
};