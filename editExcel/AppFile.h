#pragma once
#include "typechange.h"
#include "ChangeArrayNumber.h"
#include <string.h>

struct HeadingPair {
	UINT8* size;
	UINT8* baseT;
	UINT8* vt_lpstr;
	UINT8* vt_i4;
};

struct vt_vector {
	UINT8* lpstr;
	UINT32 hash;
};

class App_File {
public:
	UINT8* d;//�ǂݍ��݃f�[�^
	UINT32 dl;

	UINT8* wd;//�������݃f�[�^
	UINT32 wl;//�������݈ʒu
	UINT32 stocklen;//�ǂݍ��݃^�O�l�̒����ꎞ�ۑ�

	UINT32 p;//�ǂݍ��݈ʒu
	UINT8* prope_xmlns;
	UINT8* prope_vt;

	UINT8* Applica;
	UINT8* DocSecu;
	UINT8* ScalC;

	UINT8* Compa;
	UINT8* LinksUDate;
	UINT8* ShaDoc;
	UINT8* HyperL;
	UINT8* appVer;

	UINT8* Tvector_size;
	UINT8* Tvector_baseT;
	UINT32 Title_lp_size;

	HeadingPair* HeadRoot;

	vt_vector** Tvects;

	ArrayNumber chnum;

	App_File(UINT8* rdata,UINT32 datalen);
	~App_File();

	int readappfile();

	void addvector_lpstr(UINT8* s);

	void readTitlePare();

	UINT8** readvt_Vect();

	UINT8* readHeadPair();

	vt_vector* TreadBetweenTag(vt_vector* v);

	UINT8* readBetweenTag();

	void readproper();

	UINT8* getvalue();

	void writeappfile();

	void tagbetwrite(char* t, UINT8* v, char* ct);

	void oneStrwrite(char* str);

	void oneStrplusDoubleq(char* str, UINT8* v);

};