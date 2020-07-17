#pragma once

//#include "SearchItemNUm.h"
#include "ChangeArrayNumber.h"
#include "CRC.h"
#include "Header.h"
/*
<rPr>
<sz val="11"/>
<color theme="1"/>
<rFont val="�l�r �o�S�V�b�N"/>
<family val="2"/>
<charset val="128"/>
<scheme val="minor"/>
</rPr>

<rPh sb="5" eb="7">
*/
struct rPhtag {
    UINT8* sb;
    UINT8* eb;
    UINT8* t;
    rPhtag* next;
};

struct Tvalue {
    UINT8* Tv;
    UINT8* xm;
    Tvalue* next;
};

struct Si {
    Tvalue* Ts;
    rPhtag* rp;
    UINT8* phonetic;
};

class shareRandD {
public:
    int sicount = 0;//r �^�O�̐�
    int siunique = 0;//si�̐�
    int sicount_place = 0;
    int siunique_place = 0;
    int mycount = 0;//string index 0�n��
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
    Si** sis = nullptr;//Si�z��
    int Tcount = 0;//Si �\���̒��@t ��

    ArrayNumber st;
    crc Crc;
    UINT32 buckcrc = 0;
    UINT64 dp = 0;//�ǂݍ��݈ʒu

    char** inputsinum;//���͕�����Si�i���o�[
    int subcount;//���͕�����J�E���g

    int freecount=0;//�m�F�p

    shareRandD(UINT8* d, UINT64 l);
    ~shareRandD();
    void getSicount();
    char* writeSubstr(UINT8* d, char* s);
    UINT8* writeshare(UINT8* instr, int instrlen, char* subone, char* subtwo, char* subthree, char* subfour);
    void searchSi(char* ipto, char* iptt, char* iptth, char* iptf);
    //unique and count get

    //share str plus
    void sirPhfree(rPhtag* s);
    void siTvfree(Tvalue* s);
    void ReadShare();
    Si* getSi();
    rPhtag* getrPh(rPhtag* rpt);
    Tvalue* addT(Tvalue* t, UINT8* v, UINT8* x);
    rPhtag* addrPh(rPhtag* r, UINT8* s, UINT8* e, UINT8* t);
    Tvalue* getTtagValue(Tvalue* tvs);
    UINT8* getTValue();
    UINT8* getphoneticPr();
    UINT8* getValue();
    //si<t>������z��֕ۑ�
};