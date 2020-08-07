#include "workbookEdit.h"
#include <iostream>
WorkBook_edi::WorkBook_edi(UINT8* data,UINT32 dlen,UINT32 Sheetsize)
{
	d = data;
	dl = dlen;

	wd = nullptr;
	p = 0;
	wl = 0;
	shsize = Sheetsize;

	stocklen = 0;

	titlestr = nullptr;

	wbroot = nullptr;
	fVroot = nullptr;
	wProot = nullptr;
	mcho = nullptr;
	xfifteen = nullptr;
	mc_al = nullptr;

	wVroot = nullptr;
	wbshroot = nullptr;
	exRroot = nullptr;
	fearoot = nullptr;
	exroot = nullptr;
	reviPtrroot = nullptr;
	calcPr = nullptr;
	dfnroot = nullptr;
	shcount = 0;
	sheetIdMax = 0;
}

WorkBook_edi::~WorkBook_edi()
{
	free(calcPr);
	free(titlestr);

	freeextLst();
	freeworkbook();
	freeworkbookPr();
	freeXrRevisionPr();
	freeextReference();
	freeAlterContent();
	freesheets();
	freefnames();
	freebookviewg();
	freefileversion();
}

void WorkBook_edi::readworkbook(){
	const char* headstr = "<workbook";//9
	const char* filever = "<fileVersion";//12
	const char* workbpr = "<workbookPr";//11
	const char* altconte = "<mc:AlternateContent";//20
	const char* xr_revistr = "<xr:revisionPtr";//15
	const char* wbstr = "<bookViews";//10
	const char* sheetsstr = "<sheets";//7
	const char* eR = "<externalReferences";//19
	const char* dName = "<definedNames>";//14
	const char* calcP = "<calcPr";//7
	const char* exLsstr = "<extLst>";//8
	const char* lasttag = "</workbook>";//11

	UINT8 he[10] = { 0 };
	UINT8 fi[13] = { 0 };
	UINT8 wbpr[12] = { 0 };
	UINT8 malt[21] = { 0 };
	UINT8 revi[16] = { 0 };
	UINT8 wvi[11] = { 0 };
	UINT8 sh[8] = { 0 };
	UINT8 exr[20] = { 0 };
	UINT8 dna[15] = { 0 };
	UINT8 exl[9] = { 0 };
	UINT8 las[12] = { 0 };

	int res = 0;

	xmltitleread();
	readhead();

	do
	{
		for (int i = 0; i < 19; i++) {
			malt[i] = malt[i + 1];
			if(i<10)
				wbpr[i] = wbpr[i + 1];
			if(i<14)
				revi[i] = revi[i + 1];
			if(i<9)
				wvi[i] = wvi[i + 1];
			if(i<6)
				sh[i] = sh[i + 1];
			if(i<18)
				exr[i] = exr[i + 1];
			if(i<13)
				dna[i] = dna[i + 1];
			if(i<7)
				exl[i] = exl[i + 1];
			if (i < 10)
				las[i] = las[i + 1];
			if (i < 11)
				fi[i] = fi[i + 1];
		}
		fi[11] = las[10] = malt[19] =exr[19] = wbpr[10] = revi[14] = wvi[9] = sh[6] = exr[18] = dna[13] = exl[7] = d[p];
		p++;			

		res = strncmp((char*)fi, filever, 12);
		if (res == 0)
			readfilever();

		res = strncmp((char*)wbpr, workbpr, 11);
		if (res == 0) {
			readworkbookpr();
			std::cout << "end read :" <<d[p]<<d[p+1] << d[p + 2] << d[p + 3] << d[p + 4] << d[p + 5] << d[p + 6] << d[p + 7] << d[p + 8] 
				<< d[p + 9] << d[p + 10] << d[p + 11] << d[p + 12] << d[p + 13] << std::endl;
		}

		res = strncmp((char*)malt, altconte, 20);
		if (res == 0) {
			std::cout << "alt read :" <<altconte<< std::endl;
			readAlterContent();
		}			

		res = strncmp((char*)revi, xr_revistr, 15);
		if (res == 0)
			readXrRevisionPr();

		res = strncmp((char*)wvi, wbstr, 10);
		if (res == 0)
			readbookviewg();

		res = strncmp((char*)sh, sheetsstr, 7);
		if (res == 0)
			readsheets();

		res = strncmp((char*)exr, eR, 19);
		if (res == 0)
			readEReference();

		res = strncmp((char*)dna, dName, 14);
		if (res == 0)
			readdefinedName();

		res = strncmp((char*)sh, calcP, 7);
		if (res == 0)
			readcalcpr();

		res = strncmp((char*)exl, exLsstr, 8);
		if (res == 0)
			readextLst();

		res = strncmp((char*)las, lasttag, 11);

	} while (res!=0);

}

void WorkBook_edi::xmltitleread() {
	while (d[p] != '>') {
		p++;
	}
	p++;

	size_t mlen = p + 1;

	titlestr = (UINT8*)malloc(mlen);

	for (UINT32 i = 0; i < p; i++) {
		titlestr[i] = d[i];
	}
	titlestr[p] = '\0';
}

void WorkBook_edi::readhead() {
	const char* headstr[] = { "xmlns=\"","xmlns:r=\"","xmlns:mc=\"","xmlns:x15=\"","xmlns:xr=\"","xmlns:xr6=\"","xmlns:xr10=\""
	,"xmlns:xr2=\"","mc:Ignorable=\"" };
	//7 9 10 11 10 11 12 11 14

	UINT8 one[8] = { 0 };
	UINT8 two[10] = { 0 };
	UINT8 thre[11] = { 0 };
	UINT8 fou[12] = { 0 };
	UINT8 five[13] = { 0 };
	UINT8 six[15] = { 0 };

	UINT8* xm = nullptr;
	UINT8* xm_r = nullptr;
	UINT8* xm_mc = nullptr;
	UINT8* xm_x15 = nullptr;
	UINT8* xm_xr = nullptr;
	UINT8* xm_xr6 = nullptr;
	UINT8* xm_xr10 = nullptr;
	UINT8* xm_xr2 = nullptr;
	UINT8* ig = nullptr;

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 13; i++) {
			six[i] = six[i + 1];
			if (i < 6)
				one[i] = one[i + 1];
			if (i < 8)
				two[i] = two[i + 1];
			if (i < 9)
				thre[i] = thre[i + 1];
			if (i < 10)
				fou[i] = fou[i + 1];
			if (i < 11)
				five[i] = five[i + 1];
		}
		six[13] = one[6] = two[8] = thre[9] = fou[10] = five[11] = d[p];
		p++;

		res = strncmp((char*)six, headstr[8], 14);
		if (res == 0)
			ig = getvalue();

		res = strncmp((char*)fou, headstr[7], 11);
		if (res == 0)
			xm_xr2 = getvalue();

		res = strncmp((char*)five, headstr[6], 12);
		if (res == 0)
			xm_xr10 = getvalue();

		res = strncmp((char*)fou, headstr[5], 11);
		if (res == 0)
			xm_xr6 = getvalue();

		res = strncmp((char*)thre, headstr[4], 10);
		if (res == 0)
			xm_xr = getvalue();

		res = strncmp((char*)fou, headstr[3], 11);
		if (res == 0)
			xm_x15 = getvalue();

		res = strncmp((char*)thre, headstr[2], 10);
		if (res == 0)
			xm_mc = getvalue();

		res = strncmp((char*)one, headstr[0], 7);
		if (res == 0)
			xm=getvalue();

		res = strncmp((char*)two, headstr[1], 9);
		if (res == 0)
			xm_r = getvalue();
	}

	wbroot = (workb*)malloc(sizeof(workb));
	wbroot->Ignorable = ig;
	wbroot->mc = xm_mc;
	wbroot->r = xm_r;
	wbroot->x15 = xm_x15;
	wbroot->xmlns = xm;
	wbroot->xr = xm_xr;
	wbroot->xr10 = xm_xr10;
	wbroot->xr2 = xm_xr2;
	wbroot->xr6 = xm_xr6;
}

void WorkBook_edi::freeworkbook() {
	if (wbroot) {
		free(wbroot->Ignorable);
		free(wbroot->mc);
		free(wbroot->r);
		free(wbroot->x15);
		free(wbroot->xmlns);
		free(wbroot->xr);
		free(wbroot->xr10);
		free(wbroot->xr2);
		free(wbroot->xr6);
	}
	free(wbroot);
}
void WorkBook_edi::readfilever() {
	const char* filever[] = { "appName=\"" ,"lastEdited=\"" ,"lowestEdited=\"" ,"rupBuild=\"" };
	//9 12 14 10

	UINT8 one[10] = { 0 };
	UINT8 two[13] = { 0 };
	UINT8 thre[15] = { 0 };
	UINT8 fou[11] = { 0 };

	UINT8* ap = nullptr;
	UINT8* le = nullptr;
	UINT8* low = nullptr;
	UINT8* rb = nullptr;

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 13; i++) {
			thre[i] = thre[i + 1];
			if (i < 8)
				one[i] = one[i + 1];
			if (i < 11)
				two[i] = two[i + 1];
			if (i < 9)
				fou[i] = fou[i + 1];
		}
		one[8] = two[11] = thre[13] = fou[9] = d[p];
		p++;

		res = strncmp((char*)fou, filever[3], 10);
		if (res == 0)
			rb = getvalue();

		res = strncmp((char*)thre, filever[2], 14);
		if (res == 0)
			low = getvalue();

		res = strncmp((char*)two, filever[1], 12);
		if (res == 0)
			le = getvalue();

		res = strncmp((char*)one, filever[0], 9);
		if (res == 0)
			ap = getvalue();
	}

	fVroot = (fileV*)malloc(sizeof(fileV));
	fVroot->appName = ap;
	fVroot->lastEdited = le;
	fVroot->lowestEdited = low;
	fVroot->rupBuild = rb;
}

void  WorkBook_edi::freefileversion() {
	if (fVroot) {
		free(fVroot->appName);
		free(fVroot->lastEdited);
		free(fVroot->lowestEdited);
		free(fVroot->rupBuild);
	}
	free(fVroot);
}

void WorkBook_edi::readworkbookpr() {
	const char* workbpr[] = { "codeName=\"" ,"defaultThemeVersion=\"" };
	//10 21
	UINT8 one[11] = { 0 };
	UINT8 two[22] = { 0 };

	UINT8* cn = nullptr;
	UINT8* dt = nullptr;

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 20; i++) {
			two[i] = two[i + 1];
			if (i < 9)
				one[i] = one[i + 1];
		}
		one[9] = two[20] = d[p];
		p++;

		res = strncmp((char*)one, workbpr[0], 10);
		if (res == 0)
			cn = getvalue();

		res = strncmp((char*)two, workbpr[1], 21);
		if (res == 0)
			dt = getvalue();
	}

	wProot = (workbPr*)malloc(sizeof(workbPr));
	wProot->codeName = cn;
	wProot->defaultThemeVersion = dt;
}

void  WorkBook_edi::freeworkbookPr() {
	if (wProot) {
		free(wProot->codeName);
		free(wProot->defaultThemeVersion);
	}
	free(wProot);
}

void WorkBook_edi::readAlterContent() {
	const char* mcstr = "xmlns:mc=\"";//10
	const char* choic = "<mc:Choice";//10
	const char* etag = "</mc:AlternateContent>";//22

	UINT8 one[11] = { 0 };
	UINT8 two[11] = { 0 };
	UINT8 thre[23] = { 0 };

	UINT8* mc = nullptr;
	mc_choice* choi = nullptr;
	x15ac_absPath* xf = nullptr;

	int res = 0;

	while (d[p] != '>') {
		
		for (int i = 0; i < 9; i++) {
			one[i] = one[i + 1];
		}
		one[9] = d[p];
		p++;

		res = strncmp((char*)one, mcstr, 10);
		if (res == 0)
			mc_al = getvalue();
	}

	do
	{
		for (int i = 0; i < 21; i++) {
			thre[i] = thre[i + 1];
			if (i < 9)
				two[i] = two[i + 1];
		}
		thre[21] = two[9] = d[p];
		p++;

		res = strncmp((char*)two, choic, 10);
		if (res == 0)
			readmcchoice();

		res = strncmp((char*)thre, etag, 22);

	} while (res != 0);

}

void  WorkBook_edi::freeAlterContent() {
	free(mc_al);
	if (xfifteen) {
		free(xfifteen->url);
		free(xfifteen->x15ac);
		free(xfifteen);
	}
	if (mcho) {
		free(mcho->Requires);
		free(mcho);
	}
}

void WorkBook_edi::readmcchoice() {
	const char* req = "Requires=\"";//10
	
	UINT8 one[11] = { 0 };

	UINT8* rq = nullptr;
	UINT8* Fi = nullptr;
	UINT8* ur = nullptr;

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 9; i++) {
			one[i] = one[i + 1];
		}
		one[9] = d[p];
		p++;

		res = strncmp((char*)one, req, 10);
		if (res == 0)
			rq = getvalue();
	}
	if (d[p-1] != '/') {
		const char* mcstr[] = { "xmlns:x15ac=\"","url=\"","</mc:Choice>" };
		//13 5 12
		UINT8 two[14] = { 0 };
		UINT8 thre[6] = { 0 };
		UINT8 fou[13] = { 0 };

		do
		{
			for (int i = 0; i < 12; i++) {
				two[i] = two[i + 1];
				if (i < 11)
					fou[i] = fou[i + 1];
				if (i < 4)
					thre[i] = thre[i + 1];
			}
			fou[11] = thre[4] = two[12] = d[p];
			p++;

			res = strncmp((char*)two, mcstr[0], 13);
			if (res == 0)
				Fi=getvalue();

			res = strncmp((char*)thre, mcstr[1], 5);
			if (res == 0)
				ur = getvalue();

			res = strncmp((char*)fou, mcstr[2], 12);

		} while (res != 0);
	}

	mcho = (mc_choice*)malloc(sizeof(mc_choice));
	mcho->Requires = rq;
	
	xfifteen = (x15ac_absPath*)malloc(sizeof(x15ac_absPath));
	xfifteen->url = ur;
	xfifteen->x15ac = Fi;
}

void WorkBook_edi::readXrRevisionPr() {
	const char* mcstr[] = { "revIDLastSave=\"","documentId=\"","xr6:coauthVersionLast=\"","xr6:coauthVersionMax=\"","xr10:uidLastSave=\"" };
	//15 12 23 22 18
	UINT8 one[16] = { 0 };
	UINT8 two[13] = { 0 };
	UINT8 thre[24] = { 0 };
	UINT8 fou[23] = { 0 };
	UINT8 five[19] = { 0 };

	UINT8* rid = nullptr;
	UINT8* did = nullptr;
	UINT8* cvl = nullptr;
	UINT8* cvm = nullptr;
	UINT8* uls = nullptr;

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 22; i++) {
			thre[i] = thre[i + 1];
			if (i < 14)
				one[i] = one[i + 1];
			if (i < 11)
				two[i] = two[i + 1];
			if (i < 21)
				fou[i] = fou[i + 1];
			if (i < 17)
				five[i] = five[i + 1];
		}
		one[14] = two[11] = thre[22] = fou[21] = five[17] = d[p];
		p++;

		res = strncmp((char*)one, mcstr[0], 15);
		if (res == 0)
			rid = getvalue();

		res = strncmp((char*)two, mcstr[1], 12);
		if (res == 0)
			did = getvalue();

		res = strncmp((char*)thre, mcstr[2], 23);
		if (res == 0)
			cvl = getvalue();

		res = strncmp((char*)fou, mcstr[3], 22);
		if (res == 0)
			cvm = getvalue();

		res = strncmp((char*)five, mcstr[4], 18);
		if (res == 0)
			uls = getvalue();
	}

	reviPtrroot = (revisionptr*)malloc(sizeof(revisionptr));
	reviPtrroot->coauthVersionLast = cvl;
	reviPtrroot->coauthVersionMax = cvm;
	reviPtrroot->revIDLastSave = rid;
	reviPtrroot->documentId = did;
	reviPtrroot->uidLastSave = uls;
}

void WorkBook_edi::freeXrRevisionPr() {
	if (reviPtrroot) {
		free(reviPtrroot->coauthVersionLast);
		free(reviPtrroot->coauthVersionMax);
		free(reviPtrroot->revIDLastSave);
		free(reviPtrroot->documentId);
		free(reviPtrroot->uidLastSave);
	}
	free(reviPtrroot);
}

void WorkBook_edi::readbookviewg() {
	const char* filever[] = { "xWindow=\"" ,"yWindow=\"" ,"windowWidth=\"" ,"windowHeight=\"",
		"tabRatio=\"" ,"firstSheet=\"" ,"activeTab=\"" ,"xr2:uid=\"" };
	//9 9 13 14 10 12 11 9
	const char* endt = "</bookViews>";//12

	UINT8 one[10] = { 0 };
	UINT8 two[14] = { 0 };
	UINT8 thre[15] = { 0 };
	UINT8 fou[11] = { 0 };
	UINT8 five[13] = { 0 };
	UINT8 six[12] = { 0 };

	UINT8* xw = nullptr;
	UINT8* yw = nullptr;
	UINT8* ww = nullptr;
	UINT8* wh = nullptr;
	UINT8* tr = nullptr;
	UINT8* fs = nullptr;
	UINT8* at = nullptr;
	UINT8* uid = nullptr;

	int res = 0;

	while (d[p] != '>') {
			p++;
	}
	if (d[p-1] != '/') {
		p++;
		while (d[p] != '>') {
			for (int i = 0; i < 13; i++) {
				thre[i] = thre[i + 1];
				if (i < 8)
					one[i] = one[i + 1];
				if (i < 12)
					two[i] = two[i + 1];
				if (i < 9)
					fou[i] = fou[i + 1];
				if (i < 11)
					five[i] = five[i + 1];
				if (i < 10)
					six[i] = six[i + 1];
			}
			one[8] = two[12] = thre[13] = fou[9] = five[11] = six[10] = d[p];
			p++;

			res = strncmp((char*)one, filever[0], 9);
			if (res == 0)
				xw = getvalue();

			res = strncmp((char*)one, filever[1], 9);
			if (res == 0)
				yw = getvalue();

			res = strncmp((char*)two, filever[2], 13);
			if (res == 0)
				ww = getvalue();

			res = strncmp((char*)thre, filever[3], 14);
			if (res == 0)
				wh = getvalue();

			res = strncmp((char*)fou, filever[4], 10);
			if (res == 0)
				tr = getvalue();

			res = strncmp((char*)five, filever[5], 12);
			if (res == 0)
				fs = getvalue();

			res = strncmp((char*)six, filever[6], 11);
			if (res == 0)
				at = getvalue();

			res = strncmp((char*)one, filever[7], 9);
			if (res == 0)
				uid = getvalue();

			res = strncmp((char*)five, endt, 12);
		}
	}

	wVroot = (workView*)malloc(sizeof(workView));
	wVroot->activeTab = at;
	wVroot->firstSheet = fs;
	wVroot->tabRatio = tr;
	wVroot->uid = uid;
	wVroot->windowHeight = wh;
	wVroot->windowWidth = ww;
	wVroot->xWindow = xw;
	wVroot->yWindow = yw;
}


void WorkBook_edi::freebookviewg() {
	if (wVroot) {
		free(wVroot->activeTab);
		free(wVroot->firstSheet);
		free(wVroot->tabRatio);
		free(wVroot->uid);
		free(wVroot->windowHeight);
		free(wVroot->windowWidth);
		free(wVroot->xWindow);
		free(wVroot->yWindow);
	}
	free(wVroot);
}

void WorkBook_edi::readsheets() {
	const char* sheetsstr = "<sheet";//6
	const char* endt = "</sheets>";//9
	
	wbshroot = (wb_sheets**)malloc(sizeof(wb_sheets*) * shsize);
	shcount = 0;

	UINT8 one[7] = { 0 };
	UINT8 two[10] = { 0 };
	
	int res = 0;

	while (d[p] != '>') {
		p++;
	}
	if (d[p-1] != '/') {
		do
		{
			for (int i = 0; i < 8; i++) {
				two[i] = two[i + 1]; 
				if (i < 5)
					one[i] = one[i + 1];
			}
			one[5] = two[8] = d[p];
			p++;

			res = strncmp((char*)one, sheetsstr, 6);
			if (res == 0) {
				getsheetsV();
			}

			res = strncmp((char*)two, endt, 9);

		} while (res!=0);
	}

}

void WorkBook_edi::getsheetsV() {
	const char* sheetsstr[] = { "name=\"" ,"sheetId=\"" ,"r:id=\"" };//6 9 6

	UINT8 one[7] = { 0 };
	UINT8 two[10] = { 0 };
	UINT8* na = nullptr;
	UINT8* sid = nullptr;
	UINT8* rid = nullptr;
	int res = 0;
	ArrayNumber ar;

	while (d[p] != '>') {

		for (int i = 0; i < 8; i++) {
			two[i] = two[i + 1];
			if (i < 5)
				one[i] = one[i + 1];
		}
		one[5] = two[8] = d[p];
		p++;

		res = strncmp((char*)one, sheetsstr[0], 6);
		if (res == 0)
			na=getvalue();

		res = strncmp((char*)two, sheetsstr[1], 9);
		if (res == 0)
			sid = getvalue();

		res = strncmp((char*)one, sheetsstr[2], 6);
		if (res == 0)
			rid = getridNum();
	}
	//シートIDの最大値
	UINT32 sto = ar.RowArraytoNum(sid, strlen((char*)sid));
	if (sheetIdMax < sto) {
		sheetIdMax = sto;
	}
	wbshroot[shcount] = (wb_sheets*)malloc(sizeof(wb_sheets)); 
	wbshroot[shcount]->id = rid;
	wbshroot[shcount]->name = na;
	wbshroot[shcount]->sheetId = sid;

	shcount++;
}

void WorkBook_edi::addsheets(wb_sheets* wbs,UINT8* n,UINT8* s,UINT8* r) {
	
	wb_sheets** nsh = (wb_sheets**)realloc(wbshroot, shsize+1);

	nsh[shsize] = (wb_sheets*)malloc(sizeof(wb_sheets));
	nsh[shsize]->name = n;
	nsh[shsize]->sheetId = s;
	nsh[shsize]->id = r;
	shsize++;

	wbshroot = nsh;	
}

void WorkBook_edi::freesheets() {
	for (UINT32 i = 0; i < shsize; i++) {
		free(wbshroot[i]->id);
		free(wbshroot[i]->name);
		free(wbshroot[i]->sheetId);
		free(wbshroot[i]);
	}
}

void WorkBook_edi::readEReference() {
	const char* exrefestr[] = { "<externalReference","</externalReferences>" };//18 21

	UINT8 one[19] = { 0 };
	UINT8 two[22] = { 0 };

	int res = 0;

	while (d[p] != '>') {
		p++;
	}
	if (d[p-1] != '/') {
		do
		{
			for (int i = 0; i < 20; i++) {
				two[i] = two[i + 1];
				if (i < 17)
					one[i] = one[i + 1];
			}
			one[17] = two[20] = d[p];
			p++;

			res = strncmp((char*)one, exrefestr[0], 17);
			if (res == 0)
				getereference();

			res = strncmp((char*)two, exrefestr[1], 21);

		} while (res != 0);
	}
}

void WorkBook_edi::getereference() {
	const char* exrefestr = "r:id=\"";//6

	UINT8 one[7] = { 0 };
	UINT8* id = nullptr;
	int res = 0;

	while (d[p] != '>') {

		for (int i = 0; i < 5; i++) {
			one[i] = one[i + 1];
		}
		one[5] = d[p];
		p++;

		res = strncmp((char*)one, exrefestr, 6);
		if (res == 0)
			id = getvalue();
	}

	exRroot = addextReference(exRroot, id);
}

extReferences* WorkBook_edi::addextReference(extReferences* ex, UINT8* id) {
	if (!ex) {
		ex = (extReferences*)malloc(sizeof(extReferences));
		ex->id = id;
		ex->next = nullptr;
	}
	else
		ex->next = addextReference(ex->next, id);

	return ex;
}

void WorkBook_edi::freeextReference() {
	extReferences* p;
	while (exRroot) {
		p = exRroot->next;
		free(exRroot->id);
		free(exRroot);
		exRroot = p;
	}
}

void WorkBook_edi::readdefinedName() {
	const char* defNamestr = "<definedName";//12
	const char* et = "</definedNames>";//15

	UINT8 one[13] = { 0 };
	UINT8 two[16] = { 0 };

	int res = 0;

	if (d[p-1] != '/') {
		do
		{
			for (int i = 0; i < 14; i++) {
				two[i] = two[i + 1];
				if (i < 11)
					one[i] = one[i + 1];
			}
			one[11] = two[14] = d[p];
			p++;

			res = strncmp((char*)one, defNamestr, 12);
			if (res == 0)
				getdefinName();

			res = strncmp((char*)two, et, 15);

		} while (res != 0);
	}

}

void WorkBook_edi::getdefinName() {
	const char* defNamestr[] = { "name=\"" ,"localSheetId=\"" ,"hidden=\"","</definedName>" };
	//6 14 8 14
	UINT8 one[7] = { 0 };
	UINT8 two[15] = { 0 };
	UINT8 thre[9] = { 0 };

	UINT8* na = nullptr;
	UINT8* ls = nullptr;
	UINT8* hi = nullptr;
	UINT8* dn = nullptr;
	int res = 0;

	while (d[p] != '>') {

		for (int i = 0; i < 13; i++) {
			two[i] = two[i + 1];
			if (i < 5)
				one[i] = one[i + 1];
			if (i < 7)
				thre[i] = thre[i + 1];
		}
		thre[7] = two[13] = one[5] = d[p];
		p++;

		res = strncmp((char*)one, defNamestr[0], 6);
		if (res == 0)
			na = getvalue();

		res = strncmp((char*)two, defNamestr[1], 14);
		if (res == 0)
			ls = getvalue();

		res = strncmp((char*)thre, defNamestr[2], 8);
		if (res == 0)
			hi = getvalue();
	}
	size_t stle = 0;
	if (d[p-1] != '/') {
		p++;
		while (d[p + stle]!='<') {
			stle++;
		}
		dn = (UINT8*)malloc(sizeof(UINT8)*(stle + 1));

		for (size_t i = 0; i < stle; i++) {
			dn[i] = d[p];
			p++;
		}
		dn[stle] = '\0';
	}

	dfnroot = adddfnames(dfnroot, na,ls,hi,dn);
}

dfnames* WorkBook_edi::adddfnames(dfnames* dn, UINT8* n, UINT8* l, UINT8* h, UINT8* v) {
	if (!dn) {
		dn = (dfnames*)malloc(sizeof(dfnames));
		dn->name = n;
		dn->localSheetId = l;
		dn->hidden = h;
		dn->dfname = v;
		dn->next = nullptr;
	}
	else {
		dn->next = adddfnames(dn->next, n, l, h, v);
	}

	return dn;
}

void WorkBook_edi::freefnames() {
	dfnames* p;
	while (dfnroot) {
		p = dfnroot->next;
		free(dfnroot->name);
		free(dfnroot->dfname);
		free(dfnroot->localSheetId);
		free(dfnroot->hidden);
		free(dfnroot);
		dfnroot = p;
	}
}

void WorkBook_edi::readcalcpr() {
	const char* castr = "calcId=\"";//8

	UINT8 one[9] = { 0 };

	int res = 0;

	while (d[p] != '>') {

		for (int i = 0; i < 7; i++) {
			one[i] = one[i + 1];
		}
		one[7] = d[p];
		p++;

		res = strncmp((char*)one, castr, 8);
		if (res == 0)
			calcPr = getvalue();
	}

}

void WorkBook_edi::readextLst() {
	const char* lststr[] = { "<ext","</extLst>" };//4 9

	UINT8 one[5] = { 0 };
	UINT8 two[10] = { 0 };

	int res = 0;
	do
	{
		for (int i = 0; i < 8; i++) {
			two[i] = two[i + 1];
			if (i < 3)
				one[i] = one[i + 1];
		}
		one[3] = two[8] = d[p];
		p++;

		res = strncmp((char*)one, lststr[0], 4);
		if (res == 0)
			getextLst();

		res = strncmp((char*)two, lststr[1], 9);

	} while (res != 0);

}

void WorkBook_edi::getextLst() {
	const char* lststr[] = { "xmlns:xcalcf=\"" ,"uri=\"" ,"<xcalcf:calcFeatures","</ext>" };//14 5 20 6
	
	UINT8 one[15] = { 0 };
	UINT8 two[6] = { 0 };
	UINT8 thre[21] = { 0 };
	UINT8 fou[7] = { 0 };

	UINT8* xc = nullptr;
	UINT8* uri = nullptr;
	feature* fe = nullptr;

	int res = 0;

	while (d[p] != '>') {
		
		for (int i = 0; i < 13; i++) {
			one[i] = one[i + 1];
			if (i < 4)
				two[i] = two[i + 1];
		}
		one[13] = two[4] = d[p];
		p++;

		res = strncmp((char*)one, lststr[0], 14);
		if (res == 0)
			xc=getvalue();

		res = strncmp((char*)two, lststr[1], 5);
		if (res == 0)
			uri = getvalue();
	}

	if (d[p-1] != '/') {
		do
		{
			for (int i = 0; i < 19; i++) {
				thre[i] = thre[i + 1];
				if (i < 5)
					fou[i] = fou[i + 1];
			}
			thre[19] = fou[5] = d[p];
			p++;

			res = strncmp((char*)thre, lststr[2], 20);
			if (res == 0)
				readXcalcf();

			res = strncmp((char*)fou, lststr[3], 6);

		} while (res != 0);
	}

	exroot = (ext*)malloc(sizeof(ext));
	exroot->xcalcf = xc;
	exroot->uri = uri;
	exroot->calcFeature = fearoot;


}
void WorkBook_edi::freefeature(feature* f) {
	feature* p;
	while (f) {
		p = f->next;
		free(f->name);
		free(f);
		f = p;
	}
}

void WorkBook_edi::freeextLst() {
	if (exroot) {
		free(exroot->uri);
		free(exroot->xcalcf);
	}
	freefeature(exroot->calcFeature);
	free(exroot);
}

void WorkBook_edi::readXcalcf() {
	const char* lststr[] = { "<xcalcf:feature" ,"</xcalcf:calcFeatures>" };//15 22

	UINT8 one[16] = { 0 };
	UINT8 two[23] = { 0 };

	int res = 0;

	while (d[p] != '>') {
		p++;
	}
	if(d[p-1]!='/'){
		p++;
		do
		{
			for (int i = 0; i < 21; i++) {
				two[i] = two[i + 1];
				if (i < 14)
					one[i] = one[i + 1];
			}
			two[21] = one[14] = d[p];
			p++;

			res = strncmp((char*)one, lststr[0], 15);
			if (res == 0)
				getfeature();

			res = strncmp((char*)two, lststr[1], 22);

		} while (res!=0);		
	}

}

feature* WorkBook_edi::addfeat(feature* fe, UINT8* n) {
	if (!fe) {
		fe = (feature*)malloc(sizeof(feature));
		fe->name = n;
		fe->next = nullptr;
	}
	else {
		fe->next = addfeat(fe->next, n);
	}

	return fe;
}

void WorkBook_edi::getfeature() {
	const char* nstr = "name=\"";//6

	UINT8 one[7] = { 0 };

	UINT8* na = nullptr;
	int res = 0;

	while (d[p] != '>') {

		for (int i = 0; i < 5; i++) {
			one[i] = one[i + 1];
		}
		one[5] = d[p];
		p++;

		res = strncmp((char*)one, nstr, 6);
		if (res == 0)
			na = getvalue();
	}

	fearoot = addfeat(fearoot, na);
}

UINT8* WorkBook_edi::getvalue() {

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

UINT8* WorkBook_edi::getridNum() {

	UINT32 len = 0;
	UINT32 stl = 0;
	UINT8* Sv = nullptr;

	while (d[p + len] != '"') {
		if (d[p+len] <='9'&& d[p+len]>='0')
			stl++;
		len++;
	}
	len++;

	stocklen = stl;

	UINT32 ssize = stl + 1;

	Sv = (UINT8*)malloc(ssize);
	stl = 0;
	std::cout << "\n";
	for (UINT32 i = 0; i < len; i++) {
		if (d[p] <= '9' && d[p] >= '0') {
			Sv[stl] = d[p]; stl++;
		}
		std::cout << d[p];
		p++;
	}

	Sv[stl] = '\0';

	return Sv;
}