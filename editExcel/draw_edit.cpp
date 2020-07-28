#include "draw_edit.h"

DrawEdit::DrawEdit(UINT8* data, UINT32 datalen)
{
	d = data;
	dl = datalen;
	wd = nullptr;
	wl = 0;
	p = 0;
	stocklen = 0;
	Anroot = nullptr;
	tstr = nullptr;
	x_xdr = nullptr;
	x_a = nullptr;

	relhstr = nullptr;
	relp = 0;
	relsxmlns = nullptr;
	relroot = nullptr;
	rd = nullptr;
	rdl = 0;
	rwd = nullptr;
	rwl = 0;
}

DrawEdit::~DrawEdit()
{
	free(tstr);
	freeAnchor();
	free(x_xdr);
	free(x_a);

	freerels();

	free(relhstr);
	free(relsxmlns);
	free(rwd);
}

void DrawEdit::readdraw()
{
	const char* heads[] = { "<xdr:wsDr","<xdr:oneCellAnchor>","</xdr:wsDr>" };//9 19 11

	UINT8 one[10] = { 0 };
	UINT8 two[20] = { 0 };
	UINT8 thr[12] = { 0 };

	int res = 0;
	while (d[p] != '>')
		p++;
	p++;
	tstr = (UINT8*)malloc(sizeof(UINT8) * (p + 1));

	for (UINT32 i = 0; i < p; i++) {
		tstr[i] = d[i];
	}
	tstr[p] = '\0';

	do
	{
		for (int i = 0; i < 8; i++) {
			one[i] = one[i + 1];
		}
		one[8] = d[p]; p++;

		res = strncmp((char*)one, heads[0], 9);
		if (res == 0)
			read_xdr_wsDr();

	} while (res!=0);

	do
	{
		for (int i = 0; i < 18; i++) {
			two[i] = two[i + 1];
			if(i<10)
				thr[i] = thr[i + 1];
		}
		thr[10] = two[18] = d[p]; p++;

		res = strncmp((char*)two, heads[1], 19);
		if (res == 0)
			readoneAnchor();

		res = strncmp((char*)thr, heads[2], 11);

	} while (res != 0);
}

void DrawEdit::read_xdr_wsDr() {
	const char* xm[] = { "xmlns:xdr=\"","xmlns:a=\"" };//11 9

	UINT8 one[12] = { 0 };
	UINT8 two[10] = { 0 };

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 10; i++) {
			one[i] = one[i + 1];
			if (i < 8)
				two[i] = two[i + 1];
		}
		one[10] = two[8] = d[p]; p++;

		res = strncmp((char*)one, xm[0], 11);
		if (res == 0)
			x_xdr = getvalue();

		res = strncmp((char*)two, xm[1], 9);
		if (res == 0)
			x_a = getvalue();
	}

}

Anchor* DrawEdit::addAnchor(Anchor* a, xdr_from* f, xdr_pic* pi, oneCellAnchor_ext* ex, UINT8* cl) {
	if (!a) {
		a = (Anchor*)malloc(sizeof(Anchor));
		a->f = f;
		a->ex = ex;
		a->p = pi;
		a->clientD = cl;
		a->next = nullptr;
	}
	else {
		a->next = addAnchor(a->next, f, pi, ex, cl);
	}

	return a;
}

void DrawEdit::readoneAnchor() {
	const char* xdrstr[] = { "<xdr:from>","<xdr:pic>","</xdr:oneCellAnchor>","<xdr:ext","<xdr:clientData" };
	//10 9 20 8 15
	UINT8 one[11] = { 0 };
	UINT8 two[10] = { 0 };
	UINT8 thr[21] = { 0 };
	UINT8 fou[9] = { 0 };
	UINT8 fiv[16] = { 0 };

	xdr_pic* pi = nullptr;
	xdr_from* fro = nullptr;
	oneCellAnchor_ext *oex= nullptr;
	UINT8* clientData = nullptr;

	int res = 0;

	do
	{
		for (int i = 0; i < 19; i++) {
			thr[i] = thr[i + 1];
			if(i<9)
				one[i] = one[i + 1];
			if (i < 8)
				two[i] = two[i + 1];
			if (i < 7)
				fou[i] = fou[i + 1];
			if (i < 14)
				fiv[i] = fiv[i + 1];
		}
		fiv[14] = one[9] = fou[7] = two[8] = thr[19] = d[p]; p++;

		res = strncmp((char*)one, xdrstr[0], 10);
		if (res == 0)
			fro=read_Xdrfrom();

		res = strncmp((char*)two, xdrstr[1], 9);
		if (res == 0)
			pi=read_xdrPic();

		res = strncmp((char*)fou, xdrstr[3], 8);
		if (res == 0)
			oex=read_xdrext();

		res = strncmp((char*)fiv, xdrstr[4], 15);
		if (res == 0)
			clientData = read_clientData();

		res = strncmp((char*)thr, xdrstr[2], 20);

	} while (res != 0);

	Anroot = addAnchor(Anroot, fro, pi, oex, clientData);
}

void DrawEdit::freeAnchor() {
	Anchor* p;
	while (Anroot) {
		p = Anroot->next;
		freefrom(Anroot->f);
		freepic(Anroot->p);
		freeoneCell(Anroot->ex);
		free(Anroot->clientD);
		Anroot = p;
	}	
}

void DrawEdit::freeoneCell(oneCellAnchor_ext* oc) {
	if (oc) {
		free(oc->cx);
		free(oc->cy);
	}
}
void DrawEdit::freespPr_a_extLst(spPr_a_extLst* sae) {
	if (sae) {
		free(sae->a14_hiddenFill_xmlns);
		free(sae->uri);
		free(sae->a_srgbClr_val);
	}	
}
void DrawEdit::freespPr_exLst(spPr_exLst* se) {
	if (se) {
		free(se->uri);
		freespPr_a_extLst(se->a14_hiddenFill);
	}
}
void DrawEdit::freespPr_xfrm(spPr_xfrm* sx) {
	if (sx) {
		free(sx->a_off_x);
		free(sx->a_off_y);
		free(sx->a_ext_cx);
		free(sx->a_ext_cy);
	}
}
void DrawEdit::freespPr_prstGeom(spPr_prstGeom* sp) {
	if (sp) {
		free(sp->prst);
		free(sp->avLst);
	}
}
void DrawEdit::freespPr(xdr_spPr* xs) {
	if (xs) {
		free(xs->bwMode);
		free(xs->noFill);
		freespPr_xfrm(xs->xfrm);
		freespPr_prstGeom(xs->geom);
		freespPr_exLst(xs->extLst);
	}
}
void DrawEdit::freepic(xdr_pic* xp) {
	if (xp) {
		freeblipFill(xp->b);
		freespPr(xp->s);
	}	
}
void DrawEdit::freefrom(xdr_from* f) {
	if (f) {
		free(f->col);
		free(f->colOff);
		free(f->row);
		free(f->rowOff);
	}	
}

void DrawEdit::freeblipFill(blipFill* bf) {
	if (bf) {
		free(bf->srcRect);
		free(bf->strtch);
		freeblip(bf->Bli);
	}
}

void DrawEdit::freeblip(blip* b) {
	if (b) {
		free(b->xmlns_r);
		free(b->r_embed);
		free(b->cstate);
		freeblip_extLst(b->ext);
	}
}
void DrawEdit::freeblip_extLst(blip_extLst* be) {
	if (be) {
		free(be->a14_useLocalDpi_xmln);
		free(be->uri);
	}
}

UINT8* DrawEdit::read_clientData() {
	while (d[p] != '>') {
		p++;
	}
	if (d[p - 1] == '/') {
		return nullptr;
	}
	return nullptr;
}

xdr_from* DrawEdit::read_Xdrfrom() {
	const char* xdrstr[] = { "<xdr:col>","<xdr:colOff>","<xdr:row>","<xdr:rowOff>","</xdr:from>" };
	//9 12 9 12 11
	UINT8 one[10] = { 0 };
	UINT8 two[13] = { 0 };
	UINT8 thr[12] = { 0 };

	UINT8* col = nullptr;
	UINT8* coff = nullptr;
	UINT8* row = nullptr;
	UINT8* rowoff = nullptr;

	xdr_from* fr = nullptr;
	int res = 0;

	do
	{
		for (int i = 0; i < 11; i++) {
			two[i] = two[i + 1]; 
			if (i < 8)
				one[i] = one[i + 1];
			if (i < 10)
				thr[i] = thr[i + 1];
		}
		one[8] = two[11] = thr[10] = d[p]; p++;

		res = strncmp((char*)one, xdrstr[0], 9);
		if (res == 0)
			col= readBetweenTag();

		res = strncmp((char*)two, xdrstr[1], 12);
		if (res == 0)
			coff = readBetweenTag();

		res = strncmp((char*)one, xdrstr[2], 9);
		if (res == 0)
			row = readBetweenTag();

		res = strncmp((char*)two, xdrstr[3], 12);
		if (res == 0)
			rowoff = readBetweenTag();

		res = strncmp((char*)thr, xdrstr[4], 11);

	} while (res != 0);

	fr = (xdr_from*)malloc(sizeof(xdr_from));
	fr->col = col;
	fr->colOff = coff;
	fr->row = row;
	fr->rowOff = rowoff;

	return fr;
}

oneCellAnchor_ext* DrawEdit::read_xdrext() {
	const char* xm[] = { "cx=\"","cy=\"" };//4 4

	UINT8 one[5] = { 0 };

	UINT8* x = nullptr;
	UINT8* y = nullptr;
	oneCellAnchor_ext* oe = nullptr;

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 3; i++) {
			one[i] = one[i + 1];
		}
		one[3] = d[p]; p++;

		res = strncmp((char*)one, xm[0], 4);
		if (res == 0)
			x = getvalue();

		res = strncmp((char*)one, xm[1], 4);
		if (res == 0)
			y = getvalue();
	}
	oe = (oneCellAnchor_ext*)malloc(sizeof(oneCellAnchor_ext));
	oe->cx = x;
	oe->cy = y;

	return oe;
}

xdr_pic* DrawEdit::read_xdrPic() {
	const char* xdrstr[] = { "<xdr:nvPicPr>","<xdr:blipFill>","<xdr:spPr","</xdr:pic>" };
	//13 14 9 10
	UINT8 one[14] = { 0 };
	UINT8 two[15] = { 0 };
	UINT8 thr[10] = { 0 };
	UINT8 fou[11] = { 0 };

	int res = 0;
	blipFill* bF = nullptr;
	xdr_nvPicPr* xp = nullptr;
	xdr_spPr* ssp = nullptr;

	xdr_pic* pic = nullptr;

	do
	{
		for (int i = 0; i < 13; i++) {
			two[i] = two[i + 1];
			if (i < 12)
				one[i] = one[i + 1];
			if (i < 8)
				thr[i] = thr[i + 1];
			if (i < 9)
				fou[i] = fou[i + 1];
		}
		one[12] = two[13] = thr[8] = fou[9] = d[p]; p++;

		res = strncmp((char*)one, xdrstr[0], 12);
		if (res == 0)
			xp= read_cNvPr(xp);

		res = strncmp((char*)two, xdrstr[1], 14);
		if (res == 0)
			bF=read_blipFill();

		res = strncmp((char*)thr, xdrstr[2], 9);
		if (res == 0)
			ssp=read_spPr();

		res = strncmp((char*)fou, xdrstr[3], 10);

	} while (res != 0);

	pic = (xdr_pic*)malloc(sizeof(xdr_pic));
	pic->b = bF;
	pic->p = xp;
	pic->s = ssp;

	return pic;
}

xdr_spPr* DrawEdit::read_spPr() {
	const char* nvpicstr[] = { "bwMode=\"","<a:xfrm>","<a:prstGeom","<a:noFill","<a:extLst>","</xdr:spPr>" };
	//8 8 11 9 10 11
	UINT8 one[9] = { 0 };
	UINT8 two[12] = { 0 };
	UINT8 thr[10] = { 0 };
	UINT8 fou[11] = { 0 };

	UINT8* nf = nullptr;
	UINT8* bm = nullptr;
	spPr_exLst* spPr_extroot = nullptr;
	spPr_xfrm* spPr_xfroot = nullptr;
	spPr_prstGeom* geo = nullptr;

	xdr_spPr* ssp = nullptr;
	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 7; i++) {
			one[i] = one[i + 1];
		}
		one[7] = d[p]; p++;

		res = strncmp((char*)one, nvpicstr[0], 8);
		if (res == 0)
			bm = getvalue();
	}

	do
	{
		for (int i = 0; i < 10; i++) {
			two[i] = two[i + 1];
			if (i < 8)
				thr[i] = thr[i + 1];
			if (i < 9)
				fou[i] = fou[i + 1];
			if (i < 7)
				one[i] = one[i + 1];
		}
		one[7] = two[10] = thr[8] = fou[9] = d[p]; p++;

		res = strncmp((char*)one, nvpicstr[1], 8);
		if (res == 0)
			spPr_xfroot=read_axfrm();

		res = strncmp((char*)two, nvpicstr[2], 11);
		if (res == 0)
			geo = readprstGeom();

		res = strncmp((char*)thr, nvpicstr[3], 9);//
		if (res == 0)
			nf = xdr_readnofill();

		res = strncmp((char*)fou, nvpicstr[4], 10);// <xdr:spPr bwMode="auto">ì«Ç›çûÇ›
		if (res == 0)
			spPr_extroot =read_spPr_exLst(spPr_extroot);

		res = strncmp((char*)two, nvpicstr[5], 11);

	} while (res != 0);

	ssp = (xdr_spPr*)malloc(sizeof(xdr_spPr));
	ssp->bwMode = bm;
	ssp->extLst = spPr_extroot;
	ssp->xfrm = spPr_xfroot;
	ssp->geom = geo;
	ssp->noFill = nf;

	return ssp;
}

UINT8* DrawEdit::xdr_readnofill() {
	while (d[p] != '>') {
		p++;
	}
	if (d[p - 1] == '/') {
		return nullptr;
	}
	return nullptr;
}

spPr_prstGeom* DrawEdit::readprstGeom() {
	const char* extstr = "prst=\"";//6

	UINT8 one[7] = { 0 };

	UINT8* go = nullptr;
	UINT8* av = nullptr;
	spPr_prstGeom* geo = nullptr;
	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 5; i++) {
			one[i] = one[i + 1];
		}
		one[5] = d[p]; p++;

		res = strncmp((char*)one, extstr, 6);
		if (res == 0)
			go = getvalue();
	}
	if (d[p - 1] != '/') {
		const char* avstr = "<a:avLst";//8
		UINT8 two[9] = { 0 };
		do
		{
			for (int i = 0; i < 7; i++) {
				two[i] = two[i + 1];
			}
			two[7] = d[p]; p++;

			res = strncmp((char*)two, avstr, 8);
			if (res == 0)
				av = read_spPr_avLst();
		} while (res!=0);
	}

	geo = (spPr_prstGeom*)malloc(sizeof(spPr_prstGeom));
	geo->prst = go;
	geo->avLst = av;

	return geo;
}

UINT8* DrawEdit::read_spPr_avLst() {
	while (d[p] != '>') {
		p++;
	}
	if (d[p - 1] == '/') {
		return nullptr;
	}
	return nullptr;
}

spPr_exLst* DrawEdit::read_spPr_exLst(spPr_exLst* sp) {
	const char* extstr[] = { "<a:ext","</a:extLst>" };
	//6 11
	UINT8 one[7] = { 0 };
	UINT8 two[12] = { 0 };

	int res = 0;

	do
	{
		for (int i = 0; i < 10; i++) {
			two[i] = two[i + 1];
			if (i < 5)
				one[i] = one[i + 1];
		}
		one[5] = two[10] = d[p]; p++;

		res = strncmp((char*)one, extstr[0], 6);
		if (res == 0)
			sp=read_spPr_ext();

		res = strncmp((char*)two, extstr[1], 11);

	} while (res != 0);

	return sp;
}

spPr_exLst* DrawEdit::read_spPr_ext() {
	const char* extstr[] = { "<a14:hiddenFill","uri=\"","</a:ext>" };
	//15 5 8
	UINT8 one[16] = { 0 };
	UINT8 two[6] = { 0 };
	UINT8 thr[9] = { 0 };

	UINT8* uri = nullptr;
	spPr_exLst* spex = nullptr;
	spPr_a_extLst* aex=nullptr;
	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 4; i++) {
			two[i] = two[i + 1];
		}
		two[4] = d[p]; p++;

		res = strncmp((char*)two, extstr[1], 5);
		if (res == 0)
			uri = getvalue();
	}

	do
	{
		for (int i = 0; i < 14; i++) {
			one[i] = one[i + 1];
			if (i < 7)
				thr[i] = thr[i + 1];
		}
		thr[7] = one[14] = d[p]; p++;

		res = strncmp((char*)one, extstr[0], 15);
		if (res == 0)
			aex =read_spPr_a14hidd(aex);

		res = strncmp((char*)thr, extstr[2], 8);

	} while (res != 0);

	spex = (spPr_exLst*)malloc(sizeof(spPr_exLst));
	spex->uri = uri;
	spex->a14_hiddenFill = aex;

	return spex;
}

spPr_a_extLst* DrawEdit::read_spPr_a14hidd(spPr_a_extLst* aex) {
	const char* extstr[] = { "xmlns:a14=\"","id=\"" };
	//11 4
	UINT8 one[12] = { 0 };
	UINT8 two[5] = { 0 };

	UINT8* a14hi = nullptr;
	UINT8* id = nullptr;
	UINT8* srgb = nullptr;
	spPr_exLst* spex = nullptr;
	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 10; i++) {
			one[i] = one[i + 1];
			if (i < 3)
				two[i] = two[i + 1];
		}
		one[10] = two[3] = d[p]; p++;

		res = strncmp((char*)one, extstr[0], 11);
		if (res == 0)
			a14hi = getvalue();

		res = strncmp((char*)two, extstr[1], 4);
		if (res == 0)
			id = getvalue();
	}
	if (d[p - 1] != '/') {
		const char* nvpicstr = "<a:solidFill>";
		//13
		UINT8 thr[14] = { 0 };
		int res = 0;
		do
		{
			for (int i = 0; i < 12; i++) {
				thr[i] = thr[i + 1];
			}
			thr[12] = d[p]; p++;

			res = strncmp((char*)thr, nvpicstr, 13);
			if (res == 0)
				srgb = read_solidFill();

		} while (res != 0);
	}
	
	aex = (spPr_a_extLst*)malloc(sizeof(spPr_a_extLst));
	aex->a14_hiddenFill_xmlns = a14hi;
	aex->a_srgbClr_val = srgb;
	aex->uri = id;

	return aex;
}

UINT8* DrawEdit::read_solidFill() {
	const char* nvpicstr[] = { "</a:solidFill>","<a:srgbClr" };
	//14 10
	UINT8 one[15] = { 0 };
	UINT8 two[11] = { 0 };
	
	int res = 0;
	UINT8* srgb = nullptr;
	do
	{
		for (int i = 0; i < 13; i++) {
			one[i] = one[i + 1];
			if(i<9)
				two[i] = two[i + 1];
		}
		one[13] = two[9] = d[p]; p++;

		res = strncmp((char*)two, nvpicstr[1], 10);
		if (res == 0)
			srgb= read_a_srgbClr();

		res = strncmp((char*)one, nvpicstr[0], 14);

	} while (res != 0);

	return srgb;
}

UINT8* DrawEdit::read_a_srgbClr() {
	const char* srgbstr = "val=\"";//5

	UINT8 one[6] = { 0 };

	int res = 0;
	UINT8* srgb = nullptr;
	do
	{
		for (int i = 0; i < 4; i++) {
			one[i] = one[i + 1];
		}
		one[4] = d[p]; p++;

		res = strncmp((char*)one, srgbstr, 5);
		if (res == 0)
			srgb = getvalue();

	} while (res != 0);

	return srgb;
}

spPr_xfrm* DrawEdit::read_axfrm() {
	const char* nvpicstr[] = { "<a:off","<a:ext","</a:xfrm>" };
	//6 6 9
	UINT8 one[7] = { 0 };
	UINT8 two[10] = { 0 };

	int res = 0;

	UINT8** xy = nullptr;
	UINT8** cxy = nullptr;
	spPr_xfrm* xf = nullptr;

	do
	{
		for (int i = 0; i < 8; i++) {
			two[i] = two[i + 1];
			if (i < 5)
				one[i] = one[i + 1];
		}
		one[5] = two[8] = d[p]; p++;

		res = strncmp((char*)one, nvpicstr[0], 6);
		if (res == 0)
			xy=read_aoff();

		res = strncmp((char*)one, nvpicstr[1], 6);
		if (res == 0)
			cxy=read_aext();

		res = strncmp((char*)two, nvpicstr[2], 9);

	} while (res != 0);

	xf = (spPr_xfrm*)malloc(sizeof(spPr_xfrm));
	xf->a_off_x = xy[0];
	xf->a_off_y = xy[1];
	xf->a_ext_cx = cxy[0];
	xf->a_ext_cy = cxy[1];

	return xf;
}

UINT8** DrawEdit::read_aoff() {
	const char* cnvstr[] = { "x=\"","y=\""};
	//3 3
	UINT8 one[4] = { 0 };

	UINT8* x = nullptr;
	UINT8* y = nullptr;
	UINT8** xy = (UINT8**)malloc(sizeof(UINT8*) * 2);

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 2; i++) {
			one[i] = one[i + 1];
		}
		one[2] = d[p]; p++;

		res = strncmp((char*)one, cnvstr[0], 3);
		if (res == 0)
			x = getvalue();

		res = strncmp((char*)one, cnvstr[1], 3);
		if (res == 0)
			y = getvalue();
	}
	xy[0] = x;
	xy[1] = y;

	return xy;
}

UINT8** DrawEdit::read_aext() {
	const char* cnvstr[] = { "cx=\"","cy=\"" };
	//4 4
	UINT8 one[5] = { 0 };

	UINT8* cx = nullptr;
	UINT8* cy = nullptr;
	UINT8** cxy = (UINT8**)malloc(sizeof(UINT8*) * 2);
	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 3; i++) {
			one[i] = one[i + 1];
		}
		one[3] = d[p]; p++;

		res = strncmp((char*)one, cnvstr[0], 4);
		if (res == 0)
			cx = getvalue();

		res = strncmp((char*)one, cnvstr[1], 4);
		if (res == 0)
			cy = getvalue();
	}
	cxy[0] = cx;
	cxy[1] = cy;

	return cxy;
}
// <xdr:nvPicPr> åüçı
xdr_nvPicPr* DrawEdit::read_cNvPr(xdr_nvPicPr* pic) {
	const char* nvpicstr[] = { "<xdr:cNvPr","<xdr:cNvPicPr>","</xdr:nvPicPr>" };
	//10 14 14
	UINT8 one[11] = { 0 };
	UINT8 two[15] = { 0 };

	xfr_cNvPr* pi = nullptr;
	xdr_cNvPicPr* cnv = nullptr;	
	int res = 0;

	do
	{
		for (int i = 0; i < 13; i++) {
			two[i] = two[i + 1];
			if(i<9)
			one[i] = one[i + 1];				
		}
		one[9] = two[13] = d[p]; p++;

		res = strncmp((char*)one, nvpicstr[0], 10);
		if (res == 0)
			pi = read_cNvPrV(pi);

		res = strncmp((char*)two, nvpicstr[1], 14);
		if (res == 0)
			cnv = read_cNvPicPr();

		res = strncmp((char*)two, nvpicstr[2], 14);

	} while (res != 0);

	pic = (xdr_nvPicPr*)malloc(sizeof(xdr_nvPicPr));
	pic->Pr = pi;
	pic->cPr = cnv;

	return pic;
}

xfr_cNvPr* DrawEdit::read_cNvPrV(xfr_cNvPr* pic) {
	const char* cnvstr[] = { "<a:extLst>","</xdr:cNvPr>","id=\"","name=\"" };
	//10 12 4 6 11
	UINT8 one[11] = { 0 };
	UINT8 two[13] = { 0 };
	UINT8 thr[5] = { 0 };
	UINT8 fou[7] = { 0 };
	UINT8 fiv[12] = { 0 };

	UINT8* id = nullptr;
	UINT8* na = nullptr;
	
	xdr_ext* ex = nullptr;
	xfr_cNvPr* cn = nullptr;
	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 5; i++) {
			fou[i] = fou[i + 1];
			if (i < 3)
				thr[i] = thr[i + 1];
		}
		thr[3] = fou[5] = d[p]; p++;

		res = strncmp((char*)thr, cnvstr[2], 4);
		if (res == 0)
			id = getvalue();

		res = strncmp((char*)fou, cnvstr[3], 6);
		if (res == 0)
			na = getvalue();
	}

	do
	{
		for (int i = 0; i < 11; i++) {
			two[i] = two[i + 1];
			if (i < 9)
				one[i] = one[i + 1];
		}
		one[9] = two[11] = d[p]; p++;

		res = strncmp((char*)one, cnvstr[0], 10);
		if (res == 0)
			ex = read_exLst();

		res = strncmp((char*)two, cnvstr[1], 12);

	} while (res!=0);

	cn = (xfr_cNvPr*)malloc(sizeof(xfr_cNvPr));
	cn->id = id;
	cn->name = na;
	cn->exLst = ex;

	return cn;
}

xdr_ext* DrawEdit::read_exLst() {
	const char* extstr[] = { "<a:ext","</a:extLst>" };
	//6 11
	UINT8 one[7] = { 0 };
	UINT8 two[12] = { 0 };
	xdr_ext* ex = nullptr;
	int res = 0;

	do
	{
		for (int i = 0; i < 10; i++) {
			two[i] = two[i + 1];
			if (i < 5)
				one[i] = one[i + 1];
		}
		one[5] = two[10] = d[p]; p++;

		res = strncmp((char*)one, extstr[0], 6);
		if (res == 0)
			ex=read_ext(ex);

		res = strncmp((char*)two, extstr[1], 11);

	} while (res != 0);

	return ex;
}

xdr_ext* DrawEdit::read_ext(xdr_ext* ext) {
	const char* extstr[] = { "<a16:creationId","uri=\"","</a:ext>" };
	//15 5 8
	UINT8 one[16] = { 0 };
	UINT8 two[6] = { 0 };
	UINT8 thr[9] = { 0 };

	UINT8* uri = nullptr;
	a16_creation* a16 = nullptr;
	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 4; i++) {
			two[i] = two[i + 1];
		}
		two[4] = d[p]; p++;

		res = strncmp((char*)two, extstr[1], 5);
		if (res == 0)
			uri = getvalue();
	}

	do
	{
		for (int i = 0; i < 14; i++) {
			one[i] = one[i + 1];
			if (i < 7)
				thr[i] = thr[i + 1];
		}
		thr[7] = one[14] = d[p]; p++;

		res = strncmp((char*)one, extstr[0], 15);
		if (res == 0)
			a16=read_creationid();

		res = strncmp((char*)thr, extstr[2], 8);

	} while (res != 0);

	ext = (xdr_ext*)malloc(sizeof(xdr_ext));
	ext->a16 = a16;
	ext->uri = uri;

	return ext;
}

a16_creation* DrawEdit::read_creationid() {
	const char* extstr[] = { "xmlns:a16=\"","id=\"" };
	//11 4
	UINT8 one[12] = { 0 };
	UINT8 two[5] = { 0 };

	UINT8* a16 = nullptr;
	UINT8* id = nullptr;

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 10; i++) {
			one[i] = one[i + 1];
			if(i<3)
				two[i] = two[i + 1];
		}
		one[10] = two[3] = d[p]; p++;

		res = strncmp((char*)one, extstr[0], 11);
		if (res == 0)
			a16 = getvalue();

		res = strncmp((char*)two, extstr[1], 4);
		if (res == 0)
			id = getvalue();
	}

	a16_creation* a16_id = (a16_creation*)malloc(sizeof(a16_creation));
	a16_id->crexmlns_a16 = a16;
	a16_id->id = id;

	return a16_id;
}

xdr_cNvPicPr* DrawEdit::read_cNvPicPr() {
	const char* nvpicstr[] = { "<a:picLocks","</xdr:cNvPicPr>" };
	//11 15
	UINT8 one[12] = { 0 };
	UINT8 two[16] = { 0 };
	xdr_cNvPicPr* cnv = nullptr;

	int res = 0;

	do
	{
		for (int i = 0; i < 14; i++) {
			two[i] = two[i + 1];
			if (i < 10)
				one[i] = one[i + 1];
		}
		two[14] = one[10] = d[p]; p++;

		res = strncmp((char*)one, nvpicstr[0], 11);
		if (res == 0)
			cnv= read_piclocks(cnv);

		res = strncmp((char*)two, nvpicstr[1], 15);

	} while (res != 0);

	return cnv;
}

xdr_cNvPicPr* DrawEdit::read_piclocks(xdr_cNvPicPr* cn) {
	const char* extstr[] = { "noChangeAspect=\"","noChangeArrowheads=\"" };
	//16 20
	UINT8 one[17] = { 0 };
	UINT8 two[21] = { 0 };

	UINT8* nca = nullptr;
	UINT8* ncah = nullptr;

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 19; i++) {
			two[i] = two[i + 1]; 
			if (i < 15)
				one[i] = one[i + 1];
		}
		one[15] = two[19] = d[p]; p++;

		res = strncmp((char*)one, extstr[0], 16);
		if (res == 0)
			nca = getvalue();

		res = strncmp((char*)two, extstr[1], 20);
		if (res == 0)
			ncah = getvalue();
	}

	cn = (xdr_cNvPicPr*)malloc(sizeof(xdr_cNvPicPr));
	cn->noChangeAspect = nca;
	cn->noChangeArrowheads = ncah;

	return cn;
}

blipFill* DrawEdit::read_blipFill() {
	const char* nvpicstr[] = { "<a:blip","</xdr:blipFill>","<a:srcRect","<a:stretch" };
	//7 15 10 10
	UINT8 one[8] = { 0 };
	UINT8 two[16] = { 0 };
	UINT8 fou[11] = { 0 };
	blip* bl = nullptr;
	UINT8* sR = nullptr;
	UINT8* st = nullptr;

	blipFill* bf = nullptr;
	int res = 0;

	do
	{
		for (int i = 0; i < 14; i++) {
			two[i] = two[i + 1];
			if (i < 6)
				one[i] = one[i + 1];
			if (i < 9)
				fou[i] = fou[i + 1];

		}
		one[6] = two[14] = fou[9] = d[p]; p++;

		res = strncmp((char*)one, nvpicstr[0], 7);
		if (res == 0)
			bl = read_blip();

		res = strncmp((char*)fou, nvpicstr[2], 10);
		if (res == 0)
			sR = read_srcRect();

		res = strncmp((char*)fou, nvpicstr[3], 10);
		if (res == 0)
			st = read_stretch();

		res = strncmp((char*)two, nvpicstr[1], 15);

	} while (res != 0);

	bf = (blipFill*)malloc(sizeof(blipFill));
	bf->Bli = bl;
	bf->srcRect = sR;
	bf->strtch = st;

	return bf;
}

UINT8* DrawEdit::read_stretch() {
	const char* strstr[] = { "<a:fillRect","</a:stretch>" };//11 12

	UINT8 one[12] = { 0 };
	UINT8 two[13] = { 0 };

	int res = 0;

	UINT8* blip_fillRect = nullptr;

	do
	{
		for (int i = 0; i < 11; i++) {
			two[i] = two[i + 1];
			if(i<10)
				one[i] = one[i + 1];
		}
		one[10] = two[11] = d[p]; p++;

		res = strncmp((char*)one, strstr[0], 11);
		if (res == 0)
			blip_fillRect=read_fillrect();

		res = strncmp((char*)two, strstr[1], 12);

	} while (res != 0);

	return blip_fillRect;
}

UINT8* DrawEdit::read_fillrect() {
	while (d[p] != '>') {
		p++;
	}
	if (d[p - 1] == '/') {
		return nullptr;
	}
	return nullptr;
}

UINT8* DrawEdit::read_srcRect() {
	while (d[p] != '>') {
		p++;
	}
	if (d[p - 1] == '/') {
		return nullptr;
	}
	return nullptr;
}

UINT8* DrawEdit::read_useLocalDpi() {
	const char* extstr[] = { "xmlns:a14=\"","id=\"" };
	//11 4
	UINT8 one[12] = { 0 };
	UINT8 two[5] = { 0 };

	UINT8* a14 = nullptr;
	UINT8* id = nullptr;

	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 10; i++) {
			one[i] = one[i + 1];
			if (i < 3)
				two[i] = two[i + 1];
		}
		one[10] = two[3] = d[p]; p++;

		res = strncmp((char*)one, extstr[0], 11);
		if (res == 0)
			a14 = getvalue();

		res = strncmp((char*)two, extstr[1], 4);
		if (res == 0)
			id = getvalue();
	}

	return a14;
}

blip_extLst* DrawEdit::read_blipext() {
	const char* extstr[] = { "<a14:useLocalDpi","uri=\"","</a:ext>" };
	//16 5 8
	UINT8 one[17] = { 0 };
	UINT8 two[6] = { 0 };
	UINT8 thr[9] = { 0 };

	UINT8* uri = nullptr;
	UINT8* a14 = nullptr;
	blip_extLst* bexl;
	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 4; i++) {
			two[i] = two[i + 1];
		}
		two[4] = d[p]; p++;

		res = strncmp((char*)two, extstr[1], 5);
		if (res == 0)
			uri = getvalue();
	}

	do
	{
		for (int i = 0; i < 15; i++) {
			one[i] = one[i + 1];
			if (i < 7)
				thr[i] = thr[i + 1];
		}
		thr[7] = one[15] = d[p]; p++;

		res = strncmp((char*)one, extstr[0], 16);
		if (res == 0)
			a14=read_useLocalDpi();

		res = strncmp((char*)thr, extstr[2], 8);

	} while (res != 0);

	bexl = (blip_extLst*)malloc(sizeof(blip_extLst));
	bexl->a14_useLocalDpi_xmln = a14;
	bexl->uri = uri;

	return bexl;
}

blip_extLst* DrawEdit::read_blipexLst(blip_extLst* bex) {
	const char* extstr[] = { "<a:ext","</a:extLst>" };
	//6 11
	UINT8 one[7] = { 0 };
	UINT8 two[12] = { 0 };

	int res = 0;

	do
	{
		for (int i = 0; i < 10; i++) {
			two[i] = two[i + 1];
			if (i < 5)
				one[i] = one[i + 1];
		}
		one[5] = two[10] = d[p]; p++;

		res = strncmp((char*)one, extstr[0], 6);
		if (res == 0)
			bex=read_blipext();

		res = strncmp((char*)two, extstr[1], 11);

	} while (res != 0);

	return bex;
}

blip* DrawEdit::read_blip() {
	const char* nvpicstr[] = { "<a:extLst>","</a:blip>","xmlns:r=\"","r:embed=\"","cstate=\"" };
	//10 10 9 9 8
	UINT8 one[11] = { 0 };
	//UINT8 two[16] = { 0 };
	UINT8 thr[10] = { 0 };
	UINT8 fou[9] = { 0 };

	UINT8* xm = nullptr;
	UINT8* em = nullptr;
	UINT8* cs = nullptr;
	blip* bl = nullptr;;
	blip_extLst* ex = nullptr;
	int res = 0;

	while (d[p] != '>') {
		for (int i = 0; i < 8; i++) {
			thr[i] = thr[i + 1];
			if (i < 7)
				fou[i] = fou[i + 1];
		}
		thr[8] = fou[7] = d[p]; p++;

		res = strncmp((char*)thr, nvpicstr[2], 9);
		if (res == 0)
			xm = getvalue();

		res = strncmp((char*)thr, nvpicstr[3], 9);
		if (res == 0)
			em = getvalue();

		res = strncmp((char*)fou, nvpicstr[4], 8);
		if (res == 0)
			cs = getvalue();
	}

	if (d[p - 1] != '/') {
		do
		{
			for (int i = 0; i < 9; i++) {
				one[i] = one[i + 1];
			}
			one[9] = d[p]; p++;

			res = strncmp((char*)one, nvpicstr[0], 10);
			if (res == 0)
				ex= read_blipexLst(ex);

			//res = strncmp((char*)one, nvpicstr[1], 10);

		} while (res != 0);
	}

	bl = (blip*)malloc(sizeof(blip));
	bl->xmlns_r = xm;
	bl->r_embed = em;
	bl->cstate = cs;
	bl->ext = ex;

	return bl;
}

UINT8* DrawEdit::getvalue() {

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

UINT8* DrawEdit::readBetweenTag() {

	if (d[p-1] == '/') {
		return nullptr;//É^ÉOèIóπ
	}
	size_t l = 0;

	while (d[p + l] != '<')
		l++;

	if (l == 0)
		return nullptr;//no value

	UINT8* tagv = (UINT8*)calloc(l + 1, sizeof(UINT8));

	for (size_t i = 0; i < l; i++) {
		tagv[i] = d[p];
		p++;
	}
	tagv[l] = '\0';

	return tagv;
}