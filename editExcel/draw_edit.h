#pragma once
#include "typechange.h"
#include <stdlib.h>
#include <string.h>

struct xdr_from
{
	UINT8* col;
	UINT8* colOff;
	UINT8* row;
	UINT8* rowOff;
};

struct a16_creation
{
	UINT8* crexmlns_a16;
	UINT8* id;
};

struct xdr_ext
{
	UINT8* uri;
	a16_creation* a16;
};

struct xfr_cNvPr
{
	UINT8* id;
	UINT8* name;
	xdr_ext* exLst;
};

struct xdr_cNvPicPr
{
	UINT8* noChangeAspect;
	UINT8* noChangeArrowheads;
};

struct xdr_nvPicPr
{
	xfr_cNvPr* Pr;
	xdr_cNvPicPr* cPr;
};

struct spPr_a_extLst
{
	UINT8* uri;
	UINT8* a14_hiddenFill_xmlns;
	UINT8* a_srgbClr_val;
};

struct spPr_exLst
{
	UINT8* uri;
	spPr_a_extLst* a14_hiddenFill;
};

struct spPr_prstGeom
{
	UINT8* prst;
	UINT8* avLst;
};

struct spPr_xfrm
{
	UINT8* a_off_x;
	UINT8* a_off_y;
	UINT8* a_ext_cx;
	UINT8* a_ext_cy;
};

struct blip_extLst
{
	UINT8* uri;
	UINT8* a14_useLocalDpi_xmln;
};

struct xdr_spPr
{
	UINT8* bwMode;
	spPr_xfrm* xfrm;
	spPr_prstGeom* geom;
	spPr_exLst* extLst;
	UINT8* noFill;
};

struct blip
{
	UINT8* xmlns_r;
	UINT8* r_embed;
	UINT8* cstate;
	blip_extLst* ext;
};

struct blipFill
{
	blip* Bli;
	UINT8* srcRect;//
	UINT8* strtch;//èoåªÇ»Çµ
};

struct xdr_pic {
	blipFill* b;
	xdr_spPr* s;
	xdr_nvPicPr* p;
};

struct oneCellAnchor_ext
{
	UINT8* cx;
	UINT8* cy;
};

struct Anchor {
	xdr_from* f;
	xdr_pic* p;
	oneCellAnchor_ext* ex;
	UINT8* clientD;
	Anchor* next;
};

struct drawRels {
	UINT8* id;
	UINT8* target;
	UINT8* Type;
	drawRels* next;
};

class DrawEdit {
public:
	UINT8* d;
	UINT32 dl;
	UINT32 p;

	UINT8* wd;
	UINT32 wl;

	UINT32 stocklen;

	UINT8* tstr;
	UINT8* x_xdr;
	UINT8* x_a;
	//a_extLst* aexLst;
	//spPr_exLst* spex;
	Anchor* Anroot;

	UINT8* relhstr;//sheetrels head str
	UINT32 relp;//relsì«Ç›çûÇ›à íu
	UINT8* relsxmlns;
	drawRels* relroot;
	UINT8* rd;//decode data
	UINT32 rdl;//deco data leng
	//rels write data
	UINT8* rwd;
	UINT32 rwl;

	DrawEdit(UINT8* data,UINT32 datalen);
	~DrawEdit();

	void readdraw();

	void read_xdr_wsDr();

	Anchor* addAnchor(Anchor* a, xdr_from* f, xdr_pic* pi, oneCellAnchor_ext* ex, UINT8* cl);

	void readoneAnchor();

	void freeAnchor();

	void freeoneCell(oneCellAnchor_ext* oc);

	void freespPr_a_extLst(spPr_a_extLst* sae);

	void freespPr_exLst(spPr_exLst* se);

	void freespPr_xfrm(spPr_xfrm* sx);

	void freespPr_prstGeom(spPr_prstGeom* sp);

	void freespPr(xdr_spPr* xs);

	void freepic(xdr_pic* xp);

	void freefrom(xdr_from* f);

	void freeblipFill(blipFill* bf);

	void freeblip(blip* b);

	void freeblip_extLst(blip_extLst* be);

	UINT8* read_clientData();

	xdr_from* read_Xdrfrom();

	oneCellAnchor_ext* read_xdrext();

	xdr_pic* read_xdrPic();

	xdr_spPr* read_spPr();

	UINT8* xdr_readnofill();

	spPr_prstGeom* readprstGeom();

	UINT8* read_spPr_avLst();

	spPr_exLst* read_spPr_exLst(spPr_exLst* sp);

	spPr_exLst* read_spPr_ext();

	spPr_a_extLst* read_spPr_a14hidd(spPr_a_extLst* aex);

	UINT8* read_solidFill();

	UINT8* read_a_srgbClr();

	spPr_xfrm* read_axfrm();

	UINT8** read_aoff();

	UINT8** read_aext();

	xdr_nvPicPr* read_cNvPr(xdr_nvPicPr* pic);

	xfr_cNvPr* read_cNvPrV(xfr_cNvPr* pic);

	xdr_ext* read_exLst();

	xdr_ext* read_ext(xdr_ext* ext);

	a16_creation* read_creationid();

	xdr_cNvPicPr* read_cNvPicPr();

	xdr_cNvPicPr* read_piclocks(xdr_cNvPicPr* cn);

	blipFill* read_blipFill();

	UINT8* read_stretch();

	UINT8* read_fillrect();

	UINT8* read_srcRect();

	UINT8* read_useLocalDpi();

	blip_extLst* read_blipext();

	blip_extLst* read_blipexLst(blip_extLst* bex);

	blip* read_blip();

	UINT8* getvalue();
	UINT8* readBetweenTag();
	void drawWrite();
	void oneStrwrite(char* str);
	void oneStrplusDoubleq(char* str, UINT8* v);
	void readdrawrels(UINT8* data, UINT32 datalen);
	void read_Relations();
	drawRels* addrels(drawRels* sr, UINT8* id, UINT8* t, UINT8* ty);

	void freerels();
	void read_Relationship();
	void writerels();
	void rels_Doubleqwrite(UINT8* str, UINT8* v);
	void rels_oneStrwrite(UINT8* str);
};