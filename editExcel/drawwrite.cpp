#include "draw_edit.h"

void DrawEdit::drawWrite() {
	const char* headstr[] = { "<xdr:wsDr"," xmlns:xdr=\""," xmlns:a=\"" };
	const char* oneCellstr[] = { "<xdr:oneCellAnchor>" };
	const char* fromstr[] = { "<xdr:from>","<xdr:col>","</xdr:col>","<xdr:colOff>","</xdr:colOff>","<xdr:row>","</xdr:row>",
		"<xdr:rowOff>","</xdr:rowOff>","</xdr:from>" };
	const char* extstr[] = { "<xdr:ext"," cx=\""," cy=\"" };
	const char* picstr[] = { "<xdr:pic>","</xdr:pic>" };
	const char* nvprstr[] = { "<xdr:nvPicPr>","<xdr:cNvPr"," id=\""," name=\"","<a:extLst>","<a:ext"," uri=\"","<a16:creationId",
	" xmlns:a16=\""," id=\"","</a:ext>","</a:extLst>","</xdr:cNvPr>" };
	const char* cnvprstr[] = { "<xdr:cNvPicPr>","<a:picLocks"," noChangeAspect=\""," noChangeArrowheads=\"","</xdr:cNvPicPr>","</xdr:nvPicPr>" };
	const char* blipstr[] = { "<xdr:blipFill>","<a:blip"," xmlns:r=\""," r:embed=\""," cstate=\"","<a:extLst>","<a:ext"," uri=\"",
	"<a14:useLocalDpi"," xmlns:a14=\"","</a:ext>","</a:extLst>","</a:blip>" };
	const char* srcstr[] = { "<a:srcRect","<a:stretch>","<a:fillRect","</a:stretch>","</xdr:blipFill>" };
	const char* spPrstr[] = { "<xdr:spPr"," bwMode=\"","<a:xfrm>","<a:off"," x=\""," y=\"","<a:ext"," cx=\""," cy=\"","</a:xfrm>" };
	const char* pestgeostr[] = { "<a:prstGeom"," prst=\"","<a:avLst","</a:prstGeom>","<a:noFill" };
	const char* spexstr[] = { "<a:extLst>","<a:ext"," uri=\"","<a14:hiddenFill"," xmlns:a14=\"","<a:solidFill>","<a:srgbClr",
	" val=\"","</a:solidFill>","</a14:hiddenFill>","</a:ext>","</a:extLst>","</xdr:spPr>","</xdr:pic>"};
	const char* laststr[] = { "<xdr:clientData","</xdr:oneCellAnchor>","</xdr:wsDr>" };
	char clo = '>';
	const char* sla = "/>";

	size_t newsiz = dl + 1000;
	wd = (UINT8*)malloc(sizeof(UINT8) * dl);

	oneStrwrite((char*)tstr);

	oneStrwrite((char*)headstr[0]);
	if (x_xdr || x_a) {
		if (x_xdr)
			oneStrplusDoubleq((char*)headstr[1], x_xdr);
		if (x_a)
			oneStrplusDoubleq((char*)headstr[2], x_a);
		wd[wl] = clo; wl++;
	}
	else {
		oneStrwrite((char*)sla);
	}

	Anchor* an = Anroot;
	while (an) {
		oneStrwrite((char*)oneCellstr[0]);

		oneStrwrite((char*)fromstr[0]);

		oneStrwrite((char*)fromstr[1]);
		oneStrwrite((char*)an->f->col);
		oneStrwrite((char*)fromstr[2]);

		oneStrwrite((char*)fromstr[3]);
		oneStrwrite((char*)an->f->colOff);
		oneStrwrite((char*)fromstr[4]);

		oneStrwrite((char*)fromstr[5]);
		oneStrwrite((char*)an->f->row);
		oneStrwrite((char*)fromstr[6]);

		oneStrwrite((char*)fromstr[7]);
		oneStrwrite((char*)an->f->rowOff);
		oneStrwrite((char*)fromstr[8]);

		oneStrwrite((char*)fromstr[9]);

		oneStrwrite((char*)extstr[0]);
		if (an->ex) {
			if (an->ex->cx)
				oneStrplusDoubleq((char*)extstr[1], an->ex->cx);
			if (an->ex->cy)
				oneStrplusDoubleq((char*)extstr[2], an->ex->cy);
		}		
		oneStrwrite((char*)sla);

		//----pic
		oneStrwrite((char*)picstr[0]);

		oneStrwrite((char*)nvprstr[0]);

		oneStrwrite((char*)nvprstr[1]);
		if (an->p->p->Pr) {
			if (an->p->p->Pr->id)
				oneStrplusDoubleq((char*)nvprstr[2], an->p->p->Pr->id);
			if (an->p->p->Pr->name)
				oneStrplusDoubleq((char*)nvprstr[3], an->p->p->Pr->name);
			wd[wl] = clo; wl++;
		}
		else {
			oneStrwrite((char*)sla);
		}		

		oneStrwrite((char*)nvprstr[4]);

		//---a:ext
		oneStrwrite((char*)nvprstr[5]);
		if (an->p->p->Pr->exLst) {
			if (an->p->p->Pr->exLst->uri)
				oneStrplusDoubleq((char*)nvprstr[6], an->p->p->Pr->exLst->uri);
			wd[wl] = clo; wl++;

			oneStrwrite((char*)nvprstr[7]);
			if (an->p->p->Pr->exLst->a16) {
				if (an->p->p->Pr->exLst->a16->crexmlns_a16)
					oneStrplusDoubleq((char*)nvprstr[8], an->p->p->Pr->exLst->a16->crexmlns_a16);
				if (an->p->p->Pr->exLst->a16->id)
					oneStrplusDoubleq((char*)nvprstr[9], an->p->p->Pr->exLst->a16->id);
			}
		}				
		oneStrwrite((char*)sla);

		oneStrwrite((char*)nvprstr[10]);
		oneStrwrite((char*)nvprstr[11]);
		oneStrwrite((char*)nvprstr[12]);

		// <xdr:cNvPicPr>
		oneStrwrite((char*)cnvprstr[0]);
		oneStrwrite((char*)cnvprstr[1]);
		if (an->p->p->cPr) {
			if (an->p->p->cPr->noChangeAspect)
				oneStrplusDoubleq((char*)cnvprstr[2], an->p->p->cPr->noChangeAspect);
			if (an->p->p->cPr->noChangeArrowheads)
				oneStrplusDoubleq((char*)cnvprstr[3], an->p->p->cPr->noChangeArrowheads);
		}		
		oneStrwrite((char*)sla);
		oneStrwrite((char*)cnvprstr[4]);
		oneStrwrite((char*)cnvprstr[5]);

		//--<xdr:blipFill>--書き込み
		oneStrwrite((char*)blipstr[0]);
		oneStrwrite((char*)blipstr[1]);
		if (an->p->b->Bli) {
			if (an->p->b->Bli->xmlns_r)
				oneStrplusDoubleq((char*)blipstr[2], an->p->b->Bli->xmlns_r);
			if (an->p->b->Bli->r_embed)
				oneStrplusDoubleq((char*)blipstr[3], an->p->b->Bli->r_embed);
			if (an->p->b->Bli->cstate)
				oneStrplusDoubleq((char*)blipstr[4], an->p->b->Bli->cstate);
			wd[wl] = clo; wl++;
		}
		else {
			oneStrwrite((char*)sla);
		}
		
		//extLst
		oneStrwrite((char*)blipstr[5]);
		oneStrwrite((char*)blipstr[6]);
		if (an->p->b->Bli->ext) {
			if (an->p->b->Bli->ext->uri)
				oneStrplusDoubleq((char*)blipstr[7], an->p->b->Bli->ext->uri);
			wd[wl] = clo; wl++;
			oneStrwrite((char*)blipstr[8]);
			if (an->p->b->Bli->ext->a14_useLocalDpi_xmln)
				oneStrplusDoubleq((char*)blipstr[9], an->p->b->Bli->ext->a14_useLocalDpi_xmln);
		}
		oneStrwrite((char*)sla);

		oneStrwrite((char*)blipstr[10]);
		oneStrwrite((char*)blipstr[11]);
		oneStrwrite((char*)blipstr[12]);

		//srcrect ないやつある
		oneStrwrite((char*)srcstr[0]);
		if(an->p->b->srcRect)
			oneStrwrite((char*)an->p->b->srcRect);//tag 不明
		else
			oneStrwrite((char*)sla);
		//stretch
		oneStrwrite((char*)srcstr[1]);
		oneStrwrite((char*)srcstr[2]);
		if (an->p->b->strtch)
			oneStrwrite((char*)an->p->b->srcRect);//tag 不明
		else
			oneStrwrite((char*)sla);
		oneStrwrite((char*)srcstr[3]);

		oneStrwrite((char*)srcstr[4]);

		//----spPr 書き込み
		oneStrwrite((char*)spPrstr[0]);

		if(an->p->s->bwMode)
			oneStrplusDoubleq((char*)spPrstr[1], an->p->s->bwMode);
		wd[wl] = clo; wl++;
		//xfrm
		oneStrwrite((char*)spPrstr[2]);
		oneStrwrite((char*)spPrstr[3]);
		if (an->p->s->xfrm->a_off_x)
			oneStrplusDoubleq((char*)spPrstr[4], an->p->s->xfrm->a_off_x);	
		if (an->p->s->xfrm->a_off_y)
			oneStrplusDoubleq((char*)spPrstr[5], an->p->s->xfrm->a_off_y);
		oneStrwrite((char*)sla);

		oneStrwrite((char*)spPrstr[6]);
		if (an->p->s->xfrm->a_ext_cx)
			oneStrplusDoubleq((char*)spPrstr[7], an->p->s->xfrm->a_ext_cx);
		if (an->p->s->xfrm->a_ext_cy)
			oneStrplusDoubleq((char*)spPrstr[8], an->p->s->xfrm->a_ext_cy);
		oneStrwrite((char*)sla);
		oneStrwrite((char*)spPrstr[9]);

		//a:prstGeom
		if (an->p->s->geom) {
			oneStrwrite((char*)pestgeostr[0]);
			if (an->p->s->geom->prst)
				oneStrplusDoubleq((char*)pestgeostr[1], an->p->s->geom->prst);
			wd[wl] = clo; wl++;

			oneStrwrite((char*)pestgeostr[2]);
			if (an->p->s->geom->avLst) {
				//tag 不明
			}
			else
				oneStrwrite((char*)sla);
			oneStrwrite((char*)pestgeostr[3]);
		}
		
		//noFill
		oneStrwrite((char*)pestgeostr[4]);
		if (an->p->s->noFill) {
			//tag 不明
		}
		else {
			oneStrwrite((char*)sla);
		}

		//a:extLst
		if (an->p->s->extLst) {
			oneStrwrite((char*)spexstr[0]);
			oneStrwrite((char*)spexstr[1]);
			if (an->p->s->extLst->uri)
				oneStrplusDoubleq((char*)spexstr[2], an->p->s->extLst->uri);
			wd[wl] = clo; wl++;

			oneStrwrite((char*)spexstr[3]);
			if (an->p->s->extLst->a14_hiddenFill) {
				if (an->p->s->extLst->a14_hiddenFill->a14_hiddenFill_xmlns)
					oneStrplusDoubleq((char*)spexstr[4], an->p->s->extLst->a14_hiddenFill->a14_hiddenFill_xmlns);
				wd[wl] = clo; wl++;
			}
			else {
				oneStrwrite((char*)sla);
			}
			
			//solidFill
			oneStrwrite((char*)spexstr[5]);
			if (an->p->s->extLst->a14_hiddenFill) {
				oneStrwrite((char*)spexstr[6]);
				if (an->p->s->extLst->a14_hiddenFill->a_srgbClr_val)
					oneStrplusDoubleq((char*)spexstr[7], an->p->s->extLst->a14_hiddenFill->a_srgbClr_val);
			}			
			oneStrwrite((char*)sla);

			oneStrwrite((char*)spexstr[8]);
			oneStrwrite((char*)spexstr[9]);
			oneStrwrite((char*)spexstr[10]);
			oneStrwrite((char*)spexstr[11]);
			//--a:extLstまで
		}		

		oneStrwrite((char*)spexstr[12]);
		oneStrwrite((char*)spexstr[13]);// /pic

		//<xdr:clientData/>
		oneStrwrite((char*)laststr[0]);		
		if (an->clientD) {
			//tag 不明
		}
		else {
			oneStrwrite((char*)sla);
		}

		oneStrwrite((char*)laststr[1]);

		an = an->next;
	}
	oneStrwrite((char*)laststr[2]);//最終綴じ
}

void DrawEdit::oneStrwrite(char* str) {

	int i = 0;

	while (str[i] != '\0') {
		wd[wl] = str[i];
		i++;
		wl++;
	}
}

void DrawEdit::oneStrplusDoubleq(char* str, UINT8* v) {

	int i = 0;
	UINT8 d = '"';

	while (str[i] != '\0') {
		wd[wl] = str[i];
		i++;
		wl++;
	}
	i = 0;

	while (v[i] != '\0') {
		wd[wl] = v[i];
		i++; wl++;
	}

	wd[wl] = d;
	wl++;
}
