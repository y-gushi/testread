#pragma once

#include "typechange.h"
#include "bitInOut.h"
#include "nodeandtab.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <inttypes.h>

#define limsize 4
class deflate {
public:

    int fugoDataSize;

    bitInOut bit;

    int k;

    struct tnode** limithuffstock;//���������n�t�}���̃X�g�b�N

    int limitnum;

    int stocktree;//�i�[��

    deflate();
    ~deflate();

    struct tnode* talloc(void);
    struct tab* ttalloc(void);
    struct tnode* addtree(struct tnode* p, UINT32 n, UINT32* c);
    struct tab* addtab(struct tab* t, UINT32 n, UINT32* c);
    struct tab* addhuftab(struct tab* t, struct tnode* p, UINT32* c, UINT32* s);
    void treeprint(struct tnode* p, UINT32* m);
    void tabprint(struct tab* p);
    struct tab* makehuhu(struct tnode m, struct tab* n, UINT32* c);
    struct tab* huhu(struct tnode* n[], struct tnode* m[], UINT32 t, UINT32 l, UINT32 lim, UINT32 llim);
    struct tab* runmake(struct tab* s, UINT32 c, UINT32 n);
    struct tab* runlen(struct tab* a);
    //�c���[�S�̂�chi��1�v���X
    void treeplas(struct tnode* p);
    //tnode�̎Q�Ƃ����
    struct tnode* tabcopy(struct tnode* p, struct tab s);
    //qsort:v[left]...v[rigth]���~���Ƀ\�[�g����
    void quicksort(struct tnode* v[], int left, int right);

    //���l�ŏ����Ƀ\�[�g����
    void shellsort(struct tnode* v[], int n);
    //�����ŏ����Ƀ\�[�g����
    void shellsortFugo(struct tnode* v[], int n);
    void swap(struct tnode* v[], int i, int j);
    //�n�t�}���������������߂�
    struct tnode* treemake(struct tnode* r, struct tnode* l);
    //�n�t�}������������U��
    void makehuff(struct tnode* h[], UINT32 f, UINT32 l);
    //�n�t�}���������������߂�(���������t���j
    struct tnode* limitedtreemake(struct tnode* r, struct tnode* l, UINT32 lim);
    //tab���
    void tabfree(struct tab* t);
    //node���
    void nodefree(struct tnode* tn);

};
