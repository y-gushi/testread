#include "shareRandW.h"

void shareRandD::siwrite() {
	char header[] = "<sst xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\"";
	char sicoun[] = " count=\"";
	char uniq[] = " uniqueCount=\"";
	char endshar[] = "</sst>";

	UINT32 datalen = UINT32(sdlen) + 4000;//�f�[�^��

	wd = (char*)malloc(datalen);

	char rtag[] = "<r>";
	char rend[] = "</r>";
	char phone[] = "<phoneticPr fontId=\"";
	char cl = '>';
	char nodatatag[] = "<t/>";
	char slaend[] = "/>";

	Tvalue* rot=nullptr;
	rPhtag* ror = nullptr;
	rPrtag* ropr = nullptr;

	Tvalue* tstock = nullptr;
	rPrtag* rpstock = nullptr;
	size_t tcou = 0;
	size_t rprcount = 0;
	bool tflag = false;
	//�w�b�_�[��������
	oneStrwrite(header);
	//�J�E���g����������
	int pl = 0;
	UINT8* sic = st.InttoChar(sicount, &pl);
	oneStrplusDoubleq(sicoun, sic);
	free(sic);
	//���j�[�N����������
	UINT8* siu = st.InttoChar(siunique, &pl);
	oneStrplusDoubleq(uniq, siu);
	free(siu);
	//�^�O��
	wd[wlen] = cl; wlen++;

	for (UINT32 i = 0; i < mycount; i++) {
		tstock=rot = sis[i]->Ts;
		ror = sis[i]->rp;
		rpstock=ropr = sis[i]->rPr;

		tcou = 0;
		rprcount = 0;

		oneStrwrite((char*)tagSi);//si �X�^�[�g�^�O

		//t��rpr�̐��̊m�F
		while (tstock) {
			tcou++;
			tstock = tstock->next;
		}
		while (rpstock) {
			rprcount++;
			rpstock = rpstock->next;
		}
		// <t>����
		if (rot) {
			if (ropr&&tcou>rprcount) {
				oneStrwrite(rtag);
			}
			//�ŏ���<t>��������
			if (rot && tcou > rprcount) {
				writeT(rot);
				rot = rot->next;//t ����
			}
			if (ropr && tcou > rprcount) {
				oneStrwrite(rend);
			}
			//rPr�̏�������
			if (ropr) {
				while (ropr) {
					oneStrwrite(rtag);
					writerpr(ropr, rot);			
					oneStrwrite(rend);

					ropr = ropr->next;
					if (rot)
						rot = rot->next;
				}
			}
			if (ror) {
				while (ror) {
					writerf(ror);
					ror = ror->next;
				}
			}

			if (sis[i]->phonetic) {
				oneStrplusDoubleq(phone, sis[i]->phonetic);
				oneStrwrite(slaend);
			}
		}
		else {
			oneStrwrite(nodatatag);
		}		
			
		oneStrwrite((char*)esi);// si �G���h�^�O
		tflag = false;
	}
	oneStrwrite(endshar);
}

void shareRandD::writerpr(rPrtag* ro, Tvalue* ts) {
	char col[] = "<color";
	char sz[] = "<sz";
	char rfon[] = "<rFont";
	char fami[] = "<family";
	char cha[] = "<charset";
	char sche[] = "<scheme";
	char rstart[] = "<rPr>";
	char endr[] = "</rPr>";
	char ttag[] = "<t>";
	char endt[] = "</t>";
	char valt[] = " val=\"";
	char the[] = " theme=\"";
	char r[] = " rgb=\"";
	char clos[] = "/>";
	char cl = '>';
	char sb[] = " sb=\"";
	char eb[] = " eb=\"";

	oneStrwrite(rstart);

	if (ro->sz) {
		oneStrwrite(sz);
		oneStrplusDoubleq(valt, ro->sz);
		oneStrwrite(clos);
	}
	if (ro->theme || ro->rgb) {
		oneStrwrite(col);
		if (ro->theme)
			oneStrplusDoubleq(the, ro->theme);
		if (ro->rgb)
			oneStrplusDoubleq(r, ro->rgb);
		oneStrwrite(clos);
	}
	if (ro->rFont) {
		oneStrwrite(rfon);
		oneStrplusDoubleq(valt, ro->rFont);
		oneStrwrite(clos);
	}
	if (ro->family) {
		oneStrwrite(fami);
		oneStrplusDoubleq(valt, ro->family);
		oneStrwrite(clos);
	}
	if (ro->charset) {
		oneStrwrite(cha);
		oneStrplusDoubleq(valt, ro->charset);
		oneStrwrite(clos);
	}
	if (ro->scheme) {
		oneStrwrite(sche);
		oneStrplusDoubleq(valt, ro->scheme);
		oneStrwrite(clos);
	}
	//rpr��
	oneStrwrite(endr);
	// <t>��������
	if (ts) {
		writeT(ts);
	}
}

void shareRandD::writerf(rPhtag* ro) {
	char col[] = "<color";
	char sz[] = "<sz";
	char rfon[] = "<rFont";
	char fami[] = "<family";
	char cha[] = "<charset";
	char sche[] = "<scheme";
	char rstart[] = "<rPh";
	char endr[] = "</rPh>";
	char ttag[] = "<t>";
	char endt[] = "</t>";
	char valt[] = " val=\"";
	char the[] = " theme=\"";
	char r[] = " rgb=\"";
	char clos[] = "/>";
	char cl = '>';
	char sb[] = " sb=\"";
	char eb[] = " eb=\"";
	
	oneStrwrite(rstart);
	if (ro->sb) {
		oneStrplusDoubleq(sb, ro->sb);
	}
	if(ro->eb)
		oneStrplusDoubleq(eb, ro->eb);
	wd[wlen] = cl; wlen++;

	if (ro->t) {
		oneStrwrite(ttag);
		oneStrwrite((char*)ro->t);
		oneStrwrite(endt);
	}

	oneStrwrite(endr);
}

void shareRandD::writeT(Tvalue* t) {
	char ttag[] = "<t";
	char endt[] = "</t>";
	char xml[] = " xml:space=\"";
	char cl = '>';
	char sl[] = "/>";

	oneStrwrite(ttag);
	if (t) {
		if (t->xm) {
			oneStrplusDoubleq(xml, t->xm);
		}
		wd[wlen] = cl; wlen++;
		if (t->Tv)
			oneStrwrite((char*)t->Tv);
		oneStrwrite(endt);
	}
	else {
		oneStrwrite(sl);
	}	
}

void shareRandD::oneStrwrite(char* str) {

	int i = 0;

	while (str[i] != '\0') {
		wd[wlen] = str[i];
		i++;
		wlen++;
	}
}

void shareRandD::oneStrplusDoubleq(char* str, UINT8* v) {

	int i = 0;
	UINT8 d = '"';

	while (str[i] != '\0') {
		wd[wlen] = str[i];
		i++;
		wlen++;
	}
	i = 0;

	while (v[i] != '\0') {
		wd[wlen] = v[i];
		i++; wlen++;
	}

	wd[wlen] = d;
	wlen++;
}