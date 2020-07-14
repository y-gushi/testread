#pragma once

#include "typechange.h"
#include <stdlib.h>

#define windowsize 8192//8192//12288
#define LEVEL 8192

class slidewndow {
public:
    UINT32 Distancelength;//��v��������

    UINT32 distanceexval;//�����g���r�b�g�̒l
    UINT32 distanceexbit;//�����g���r�b�g�̃r�b�g��

    UINT32 mileexval;//���������ϊ��p�̒l
    UINT32 mileexbit;//�����g�������̃r�b�g��

    UINT32* milestock;//�ϊ������l�̊i�[ before short
    UINT32* mileRoot;
    UINT32 milestocksize;
    UINT32* distancestock;//�����̕ϊ������l before short
    UINT32* distanceRoot;
    UINT32 distancestocksize;
    UINT32* mile_extension_stock;//�����g���r�b�g�i�[ before short
    UINT32* mile_extensionRoot;
    UINT32* distance_extenshon_stock;//�����g���r�b�g�i�[ before short
    UINT32* distance_extenshonRoot;

    UINT32 maxmilestock;//�X�g�b�N�̍Œ�
    UINT64 stocknum;//�X�g�b�N��

    UINT32 freecount;//�m�F�p
    UINT32 exdis = 0;//�m�F�p
    UINT32 exmil = 0;//�m�F�p

    slidewndow();
    ~slidewndow();
    void milefree();
    UINT32 slidesearch(unsigned char* p, UINT64 len, UINT64 spos);
    //UINT32 slidesearch(unsigned char* p, UINT32 len, UINT32 spos);
    UINT32 CurposSearch(unsigned char* ps, UINT32 pp, UINT32 l);
    UINT32 MileBitnumSearch(UINT32 l);//�����g���r�b�g���̎Q��
    UINT32 DistanceBitnumSearch(UINT32 l);//�����g���r�b�g���̎Q��
    UINT32 DistanceExSearch(UINT32 l);//�����𕄍��p���l�ɕϊ�
    UINT32 MileExSearch(UINT32 m);//�����𕄍��p���l�ɕϊ�
};