#pragma once

//#include "SearchItemNUm.h"
#include "ChangeArrayNumber.h"
#include "CRC.h"
#include "Header.h"

struct Si {
    UINT8* Ts = nullptr;
    Si* next = nullptr;
};

class shareRandD {
public:
    int sicount = 0;//r タグの数
    int siunique = 0;//siの数
    int sicount_place = 0;
    int siunique_place = 0;
    int mycount = 0;//string index 0始り
    const char* tagT = "<t";
    const char* tagSi = "<si>";
    const char* esi = "</si>";//5
    const char* esst = "</sst>";//6
    UINT8* sd;
    UINT64 sdlen;
    UINT8* countstr = nullptr;
    UINT8* uniqstr = nullptr;
    UINT32 writeleng = 0;
    UINT8** Sharestr = nullptr;
    Si** sis = nullptr;//Si配列
    int Tcount = 0;//Si 構造体中　t 数

    ArrayNumber st;
    crc Crc;
    UINT32 buckcrc = 0;

    char** inputsinum;//入力文字列Siナンバー
    int subcount;//入力文字列カウント

    int freecount=0;//確認用

    shareRandD(UINT8* d, UINT64 l);
    ~shareRandD();
    void getSicount();
    char* writeSubstr(UINT8* d, char* s);
    UINT8* writeshare(UINT8* instr, int instrlen, char* subone, char* subtwo, char* subthree, char* subfour);
    void searchSi(char* ipto, char* iptt, char* iptth, char* iptf);
    //unique and count get

    //share str plus
    Si* addSitable(Si* s, UINT8* str);
    void Sitablefree(Si* s);
    void ReadShare();//si<t>文字列配列へ保存
};