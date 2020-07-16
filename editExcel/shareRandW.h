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
    Si* addSitable(Si* s, UINT8* str);
    void Sitablefree(Si* s);
    void ReadShare();//si<t>������z��֕ۑ�
};