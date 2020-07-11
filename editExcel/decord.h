#pragma once

#include "typechange.h"
#include "bitInOut.h"
#include "crc.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define BASE 65521
struct decodenode { /*�؂̃m�[�h*/
    UINT32 fugo = 0;/*�e�L�X�g�ւ̃|�C���^*/
    UINT32 fugolen = 0;
    UINT32 val = 0;
    UINT32 count = 0;
    struct decodenode* left = nullptr;
    struct decodenode* right = nullptr;
};

struct DecSelect;

class Hclen {
public:
    UINT32 hclenv;
    UINT32 huffHclen;
    UINT32 hufflen;
    Hclen();
    void dswap(Hclen v[], int n);
    void dmakehuff(Hclen h[]);
};

class DeflateDecode {
private:
    std::ifstream* fr;
public:
    crc cr;
    unsigned int crc;
    unsigned char* ReadV;
    unsigned char* ReadVroot;
    unsigned char rev;//File read�f�[�^
    UINT32 length = 0;
    UINT32 left = 0, right = 0;//�����̕������ŏ��l�A�ꏊ�@/�n�t�}���������ŏ��l�A�ő�l
    UINT32 count = 0;
    UINT32 k = 0;
    UINT32 stock = 0;
    UINT32 n = 1;
    UINT32 pu = 1;
    UINT32 hu = 0;
    UINT32 miles = 0;//���������̒l
    UINT32 len = 0;//�����t���̒l
    unsigned int adler32 = 0;//�A�h���[�ǂݍ��ݒl�̊i�[

    UINT8 BFINAL;//1bit�@�f�[�^�̍Ō�̃u���b�N�ł���ꍇ�ɂ̂ݐݒ�
    UINT8 BTYPE;//2bit �r�b�g���]01�Œ�n�t�}�� 10�J�X�^���n�t�}��

    unsigned long adlerR = 1L;//adler�v�Z�l

    unsigned short HLIT;
    unsigned short HDIST;
    unsigned char HCLEN;

    Hclen obh[19];
    Hclen h;
    //std::vector<unsigned int> hufftable;//�����e�[�u��
    UINT32* hufftable = nullptr;
    UINT32 hufftablepos = 0;

    UINT32* lentable = nullptr;
    UINT32 lentablepos = 0;
    UINT32* hufval = nullptr;
    UINT32* lenval = nullptr;
    UINT32* lenh = nullptr;
    UINT32* lenlen = nullptr;

    unsigned int* signs;
    UINT32 signSize;

    struct decodenode* root = NULL;
    struct decodenode* lenroot = NULL;
    struct decodenode* hufhuf = NULL;

    UINT32 readlen;//�ǂݍ��񂾃f�[�^�̃T�C�Y
    bitInOut bit;

    DeflateDecode(std::ifstream* f);
    ~DeflateDecode();
    struct decodenode* dalloc(void);
    struct decodenode* daddtree(struct decodenode* p, UINT32 w, UINT32 a, UINT32 b);
    void freetree(decodenode* p);
    UINT32* copy(UINT32 s);
    void dtreeprint(struct decodenode* p);
    struct decodenode* dsearchVal(struct decodenode* n);
    UINT32 dsearch_node(UINT32 x, struct decodenode* node, UINT32 u);
    UINT64 dataread(UINT64 position, UINT32 size);
    void headdataread();
    int haffmanread();//�w�b�_�[&�����̓ǂݍ���
    void milesearch(UINT32 mil);
    void distancesearch(UINT32 dis);
    void byteread();//1�o�C�g�ǂݍ���
    DeflateDecode* memorykeep(DeflateDecode* d);//�f�R�[�h�\���́@�N���X�̈���m��
    char* copyfn(char* f);
    //DecSelect* nextadd(DecSelect* ds, char fn[], UINT64 po,char *d,UINT32 c);//next�Ƀv���X
    DecSelect* nextadd(DecSelect* ds, DeflateDecode* decdata, char* fn);

    UINT32 searchLiteral(unsigned int n, UINT8 leng);
    void FixedRead();
    void CustomRead();
    void noCompressRead();
};

//�f�R�[�h�f�[�^�̍\����
struct DecSelect
{
    DeflateDecode* decorddata;//decord object
    char* Fn;// filename
    DecSelect* next;//next struct
};
