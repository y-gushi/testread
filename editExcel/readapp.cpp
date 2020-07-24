
#include "AppFile.h"

App_File::App_File(UINT8* rdata, UINT32 datalen)
{
	d = rdata;
	dl = datalen;
	wd = nullptr;
	wl = 0;

	p = 0;
	stocklen = 0;
	Title_lp_size = 0;

	prope_xmlns = nullptr;
	prope_vt = nullptr;
	Applica = nullptr;
	DocSecu = nullptr;
	Tvector_size = nullptr;
	Tvector_baseT = nullptr;
	Tvects = nullptr;
	HeadRoot = nullptr;
	ScalC = nullptr;

	Compa = nullptr;
	LinksUDate = nullptr;
	ShaDoc = nullptr;
	HyperL = nullptr;
	appVer = nullptr;
}

App_File::~App_File()
{
	free(prope_xmlns);
	free(prope_vt);
	free(Applica);
	free(DocSecu);
	free(Tvector_size);
	free(Tvector_baseT);
	free(ScalC);
	/*
	*/
	for (UINT32 i = 0; i < Title_lp_size; i++) {
		if (Tvects)
			free(Tvects[i]->lpstr);
		free(Tvects[i]);
	}
	free(Compa);
	free(LinksUDate);
	free(ShaDoc);
	free(HyperL);
	free(appVer);

	if (HeadRoot) {
		free(HeadRoot->baseT);
		free(HeadRoot->size);
		free(HeadRoot->vt_i4);
		free(HeadRoot->vt_lpstr);
	}

	free(wd);
}

int App_File::readappfile() {
	const char* appheadstr[] = { "<Properties","<Application","<DocSecurity","<ScaleCrop","<HeadingPairs","<TitlesOfParts>" };//11 12 12 10 13 15
	const char* laststr[] = { "<LinksUpToDate","<SharedDoc","<HyperlinksChanged","<AppVersion","<Company" };//14 10 18 11 8
	const char* endt = "</Properties>";//13文字

	UINT8 prop[12] = { 0 };
	UINT8 Ap[13] = { 0 };
	UINT8 Hea[14] = { 0 };
	UINT8 ent[16] = { 0 };
	UINT8 sc[11] = { 0 };

	UINT8 lutd[15] = { 0 };
	UINT8 hlc[19] = { 0 };
	UINT8 com[9] = { 0 };

	int res = 0;

	do {
		for (int i = 0; i < 17; i++) {
			hlc[i] = hlc[i + 1];
			if (i < 14)
				ent[i] = ent[i + 1];
			if (i < 13)
				lutd[i] = lutd[i + 1];
			if (i < 12)
				Hea[i] = Hea[i + 1];
			if (i < 11)
				Ap[i] = Ap[i + 1];
			if (i < 10)
				prop[i] = prop[i + 1];
			if (i < 9)
				sc[i] = sc[i + 1];
			if (i < 7)
				com[i] = com[i + 1];
		}
		lutd[13] = hlc[17] = com[7] = ent[14] = Hea[12] = Ap[11] = prop[10] = sc[9] = d[p];
		p++;

		res = strncmp((char*)com, laststr[4], 8);
		if (res == 0)
			Compa = readBetweenTag();

		res = strncmp((char*)prop, laststr[3], 11);
		if (res == 0)
			appVer = readBetweenTag();

		res = strncmp((char*)hlc, laststr[2], 18);
		if (res == 0)
			HyperL = readBetweenTag();

		res = strncmp((char*)sc, laststr[1], 10);
		if (res == 0)
			ShaDoc = readBetweenTag();

		res = strncmp((char*)lutd, laststr[0], 14);
		if (res == 0)
			LinksUDate = readBetweenTag();

		res = strncmp((char*)prop, appheadstr[0], 11);
		if (res == 0)
			readproper();

		res = strncmp((char*)Ap, appheadstr[1], 12);
		if (res == 0)
			Applica=readBetweenTag();

		res = strncmp((char*)Ap, appheadstr[2], 12);
		if (res == 0)
			DocSecu = readBetweenTag();

		res = strncmp((char*)sc, appheadstr[3], 10);
		if (res == 0)
			ScalC = readBetweenTag();

		res = strncmp((char*)Hea, appheadstr[4], 13);
		if (res == 0)
			readHeadPair();

		res = strncmp((char*)ent, appheadstr[5], 15);
		if (res == 0)
			readTitlePare();

		res = strncmp((char*)Hea, endt, 13);//エンドタグ

	} while (res!=0);

	return 1;
}
//シートタイトル名プラス
void App_File::addvector_lpstr(UINT8* s) {
	UINT32 msize = Title_lp_size+1;
	UINT32 has = 0;

	vt_vector** stock = (vt_vector**)realloc(Tvects,sizeof(vt_vector*)* msize);

	Tvects = stock;

	for (size_t i = 0; i < strlen((char*)s); i++) {
		has += s[i];
	}

	Tvects[Title_lp_size] = (vt_vector*)malloc(sizeof(vt_vector));
	Tvects[Title_lp_size]->lpstr = s;
	Tvects[Title_lp_size]->hash = has;

	Title_lp_size++;
}

void App_File::readTitlePare() {
	const char* headstr = "<vt:vector";// 10
	const char* et = "</TitlesOfParts>";//16

	UINT8 i4[11] = { 0 };
	UINT8** sb = nullptr;//size baseType 入力

	UINT32 cou = 0;
	int pl = 0;

	int res = 0;
	do
	{
		for (int i = 0; i < 9; i++) {
			i4[i] = i4[i + 1];
		}
		i4[9] = d[p];
		p++;

		res = strncmp((char*)i4, headstr, 10);
		if (res == 0)
			sb=readvt_Vect();

	} while (res != 0);

	//サイズがあればlpstr読み込む
	if (sb) {
		//サイズ分配列確保
		Tvector_size = sb[0];
		Tvector_baseT = sb[1];
		Title_lp_size = chnum.RowArraytoNum(sb[0], strlen((char*)sb[0]));
		Tvects = (vt_vector**)calloc(Title_lp_size,sizeof(vt_vector*));

		const char* vtstr = "<vt:lpstr";// 9

		UINT8 endt[17] = { 0 };
		UINT8 lps[10] = { 0 };

		int res = 0;
		do
		{
			for (int i = 0; i < 15; i++) {
				endt[i] = endt[i + 1];
				if (i < 8)
					lps[i] = lps[i + 1];
			}
			endt[15] = lps[8] = d[p];
			p++;

			res = strncmp((char*)lps, vtstr, 9);
			if (res == 0) {
				Tvects[cou] = TreadBetweenTag(Tvects[cou]);
				cou++;
			}

			res = strncmp((char*)endt, et, 16);//終了タグ </TitlesOfParts>

		} while (res != 0);
	}
}

vt_vector* App_File::TreadBetweenTag(vt_vector* v) {
	UINT32 ha = 0;

	while (d[p] != '>') {
		if (d[p] == '/') {
			return nullptr;//タグ終了
		}
		p++;
	}
	p++;

	v = (vt_vector*)malloc(sizeof(vt_vector));
	size_t l = 0;

	while (d[p + l] != '<')
		l++;

	if (l == 0)
		return nullptr;//no value

	UINT8* tagv = (UINT8*)malloc(l + 1);

	for (size_t i = 0; i < l; i++) {
		tagv[i] = d[p];
		ha += d[p];
		p++;
	}
	tagv[l] = '\0';

	v->lpstr = tagv;
	v->hash = ha;

	return v;
}

UINT8** App_File::readvt_Vect() {
	const char* vectstr[] = { "size=\"","baseType=\"" };//6 10

	UINT8 si[7] = { 0 };
	UINT8 ba[11] = { 0 };

	int res = 0;

	UINT8** sb = (UINT8**)calloc(2,sizeof(UINT8*));

	do
	{
		if (d[p] == '/')
			break;

		for (int i = 0; i < 9; i++) {
			ba[i] = ba[i + 1];
			if (i < 5)
				si[i] = si[i + 1];
		}
		ba[9] = si[5] = d[p];
		p++;

		res = strncmp((char*)si, vectstr[0], 6);
		if(res==0)
			sb[0] = getvalue();

		res = strncmp((char*)ba, vectstr[1], 10);
		if (res == 0)
			sb[1] = getvalue();

	} while (d[p] != '>');

	return sb;
}

UINT8* App_File::readHeadPair() {
	while (d[p] != '>') {
		if (d[p] == '/') {
			return nullptr;//タグ終了
		}
		p++;
	}
	p++;
	const char* headstr = "<vt:vector";// 10
	const char* ent = "</HeadingPairs>";//15
	const char* vtstr[] = { "<vt:lpstr","<vt:i4" };// 9 6

	UINT8 i4[7] = { 0 };
	UINT8 endt[16] = { 0 };
	UINT8 vec[11] = { 0 };
	UINT8 lps[10] = { 0 };

	UINT8* lp = nullptr;
	UINT8* ifo = nullptr;
	UINT8** vals = nullptr;

	int res = 0;
	do
	{
		for (int i = 0; i < 14; i++) {
			endt[i] = endt[i + 1];
			if(i<9)
				vec[i] = vec[i + 1];
			if (i < 8)
				lps[i] = lps[i + 1];
			if (i < 5)
				i4[i] = i4[i + 1];
		}
		vec[9] = lps[8] = endt[14] = i4[5]= d[p];
		p++;

		res = strncmp((char*)vec, headstr, 10);
		if (res == 0)
			vals=readvt_Vect();

		res = strncmp((char*)lps, vtstr[0], 9);
		if (res == 0)
			lp = readBetweenTag();

		res = strncmp((char*)i4, vtstr[1], 6);
		if (res == 0)
			ifo = readBetweenTag();

		res = strncmp((char*)endt, ent, 15);//終了タグ </HeadingPairs>

	} while (res!=0);

	HeadRoot = (HeadingPair*)malloc(sizeof(HeadingPair));
	HeadRoot->size = vals[0];
	HeadRoot->baseT = vals[1];
	HeadRoot->vt_lpstr = lp;
	HeadRoot->vt_i4 = ifo;

	return nullptr;
}

UINT8* App_File::readBetweenTag() {

	while (d[p] != '>') {
		if (d[p] == '/') {
			return nullptr;//タグ終了
		}
		p++;
	}
	p++;
	size_t l=0;

	while (d[p + l] != '<')
		l++;

	if (l == 0)
		return nullptr;//no value

	UINT8* tagv = (UINT8*)calloc(l+1, sizeof(UINT8));

	for (size_t i = 0; i < l; i++) {
		tagv[i] = d[p];
		p++;
	}
	tagv[l] = '\0';	

	return tagv;
}

void App_File::readproper() {
	const char* valuestr[] = { "xmlns=\"","xmlns:vt=\"" };//7 10

	UINT8 xms[8] = { 0 };
	UINT8 vt[11] = { 0 };

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 9; i++) {
			vt[i] = vt[i + 1];
			if (i < 6)
				xms[i] = xms[i + 1];
		}
		vt[9] = xms[6] = d[p];
		p++;

		res = strncmp((char*)vt, valuestr[1], 10);
		if (res == 0)
			prope_vt=getvalue();

		res= strncmp((char*)xms, valuestr[0], 7);
		if (res == 0)
			prope_xmlns = getvalue();
	}
}

UINT8* App_File::getvalue() {

	UINT32 len = 0;
	UINT8* Sv = nullptr;

	while (d[p + len] != '"')
		len++;

	stocklen = len;
	UINT32 ssize = len + 1;

	Sv = (UINT8*)malloc(ssize);

	for (UINT32 i = 0; i < len; i++) {
		Sv[i] = d[p]; p++;
	}

	Sv[len] = '\0';

	return Sv;
}