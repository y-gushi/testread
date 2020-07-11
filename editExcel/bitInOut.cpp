#include "bitInOut.h"

bitInOut::bitInOut() {
    for (int i = 0; i < 64; i++) {
        stocks[i] = 0;
    }
    bitoutvalue = 0;
    bitstartpos = 0;
    bitendpos = 0;
    bitpos = 0;
    stockValue = 0;
}

//上位Bitを下位Bitに（リトルエンディアン）
void bitInOut::LittleendIn(int bi, int n) {
    //リトルエンディアンでストック
    for (int j = 0; j < n; j++) {
        stocks[bitendpos] = (bi >> ((n - 1) - j)) & 0x01;
        bitendpos++;
        if (bitendpos > 63) { bitendpos = 0; }
    }
    if (bitendpos < bitstartpos) { bitpos = bitendpos + (64 - bitstartpos); }
    else { bitpos = bitendpos - bitstartpos; }
}

//上位Bitを上位Bitに（ビッグエンディアン）
void bitInOut::BigendIn(int bir, int nr) {
    for (int j = 0; j < nr; j++) {
        stocks[bitendpos] = (bir >> j) & 0x01;
        bitendpos++;
        //std::cout << "bitendpos:" << bitendpos<<"  bir:"<<bir << std::endl;
        if (bitendpos > 63) { bitendpos = 0; }//stocks = stockp;
    }
    if (bitendpos < bitstartpos) { bitpos = bitendpos + (64 - bitstartpos); }
    else { bitpos = bitendpos - bitstartpos; }
}

//配列下位Bitから取り出す、反転した値をセット
void bitInOut::LittleendOut(int setValueR) {
    stockValue = bitoutvalue;
    bitoutvalue = 0;
    for (int i = 0; i < setValueR; i++) {
        bitoutvalue += stocks[bitstartpos] << i;
        bitstartpos++;
        if (bitstartpos > 63) { bitstartpos = 0; }
    }
    if (bitendpos < bitstartpos) { bitpos = bitendpos + (64 - bitstartpos); }
    else { bitpos = bitendpos - bitstartpos; }
}

//配列上位Bitから取り出す
void bitInOut::BigendOut(int setValue) {
    stockValue = bitoutvalue;
    bitoutvalue = 0;
    for (int i = 0; i < setValue; i++) {
        bitoutvalue += stocks[bitstartpos] << (setValue - 1 - i);
        bitstartpos++;
        //std::cout << "bitstartpos:" << bitstartpos << "  setValue:" << setValue << std::endl;
        if (bitstartpos > 63) { bitstartpos = 0; }
    }
    if (bitendpos < bitstartpos) { bitpos = bitendpos + (64 - bitstartpos); }
    else { bitpos = bitendpos - bitstartpos; }
}