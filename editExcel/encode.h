#pragma once
#include "bitInOut.h"
#include "slidewindow.h"
#include "deflate.h"
#include "nodeandtab.h"
#include <stdio.h>

#define huff_of_huff_limit 5
#define mile_literal_limit 13
#define limit_stock_max 10

class encoding {
public:

    bitInOut bits;

    unsigned short hht[19] = { 0 };
    UINT8 htc;//�v�f�J�E���g�悤
    UINT32 datalen;

    struct tab* fir;
    struct tab* lir;

    struct tnode** so;
    struct tnode** sso;

    struct tnode** lev;
    struct tnode** llev;

    struct tab* hh;//�����̕����e�[�u��
    struct tab* hhh;
    struct tnode** huh;//�|�C���^ �����̕����e�[�u���@huh[hcc]�@�v�f����`�ɕϐ��g���Ȃ�
    struct tnode** huhc;//�|�C���^ �����̕����e�[�u���@�e���l�Q�Ɨp�@huhc[hccc]�@�v�f����`�ɕϐ��g���Ȃ�

    UINT32 maxhh;

    unsigned char* encoderv;
    unsigned char* rvp;
    UINT32 limstocksize;

    UINT32 hccc;
    struct tab* hhc;//�����̕����J�E���g�p�|�C���^
    UINT32 hcc;//���l�̎�ނ̃J�E���g

    UINT32 tc;//�c���[�̗v�f��
    UINT32 tcc;//�c���[�̗v�f���@�R�s�[�p
    UINT32 lc;//�����l�c���[�̗v�f��
    UINT32 lcc;//�����l�c���[�̗v�f���@�R�s�[�p
    UINT32 maxlen;//�����̒����̍ő�l
    UINT32 lmaxlen;//���������̒����̍ő�l
    UINT32 k;//�g���r�b�g�̊m�F�p
    UINT32 mc;//�����̒l�̃J�E���g
    UINT32 l;//�����̒l�̃J�E���g
    UINT32 lb;//�����̊g���l�̃J�E���g

    encoding();
    ~encoding();
    void huhtable(struct tnode* a[], UINT8 b);
    UINT32* litrals(UINT32 d);
    void fixedcompress(slidewndow* s);
    void headerwrite(unsigned char HLIT, unsigned char HDIST, unsigned char BI, unsigned char BT);
    void makeSign(slidewndow* s, deflate* defl);
    int compress(unsigned char* data, UINT64 dataleng);
    void write(FILE* f);


};