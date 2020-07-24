#include "AppFile.h"

void App_File::writeappfile() {
	size_t siz = dl+100;
	wd = (UINT8*)malloc(siz);

	const char* headstr[] = { "<Properties"," xmlns=\""," xmlns:vt=\"" };
	const char* headtwo[] = { "<Application","</Application>" ,"<DocSecurity","</DocSecurity>","<ScaleCrop","</ScaleCrop>" };
	const char* headinpair[] = { "<HeadingPairs","<vt:vector"," size=\""," baseType=\"","</HeadingPairs>","</vt:vector>" };
	const char* vtvari[] = { "<vt:variant","<vt:lpstr","</vt:lpstr>","<vt:i4","</vt:i4>","</vt:variant>" };
	const char* titpair[] = { "<TitlesOfParts","</TitlesOfParts>" };
	const char* ladstr[] = { "<Company","</Company>","<LinksUpToDate","</LinksUpToDate>","<SharedDoc","</SharedDoc>","<HyperlinksChanged","</HyperlinksChanged>",
	"<AppVersion","</AppVersion>","</Properties>" };
	char clo = '>';
	const char* sla = "/>";

	oneStrwrite((char*)headstr[0]);
	oneStrplusDoubleq((char*)headstr[1], prope_xmlns);
	oneStrplusDoubleq((char*)headstr[2], prope_vt);
	wd[wl] = clo; wl++;

	tagbetwrite((char*)headtwo[0], Applica, (char*)headtwo[1]);
	tagbetwrite((char*)headtwo[2], DocSecu, (char*)headtwo[3]);
	tagbetwrite((char*)headtwo[4], ScalC, (char*)headtwo[5]);

	oneStrwrite((char*)headinpair[0]);
	if (HeadRoot) {
		if (HeadRoot->vt_i4 || HeadRoot->vt_lpstr) {
			wd[wl] = clo; wl++;
			//vector size
			oneStrwrite((char*)headinpair[1]);
			oneStrplusDoubleq((char*)headinpair[2], HeadRoot->size);
			oneStrplusDoubleq((char*)headinpair[3], HeadRoot->baseT);
			wd[wl] = clo; wl++;
			//variant
			oneStrwrite((char*)vtvari[0]);
			if (HeadRoot->vt_i4) {
				wd[wl] = clo; wl++;
				tagbetwrite((char*)vtvari[1], HeadRoot->vt_lpstr, (char*)vtvari[2]);
				oneStrwrite((char*)vtvari[5]);
			}
			else
				oneStrwrite((char*)sla);
			//variant
			oneStrwrite((char*)vtvari[0]);
			if (HeadRoot->vt_i4) {
				wd[wl] = clo; wl++;
				tagbetwrite((char*)vtvari[3], HeadRoot->vt_i4, (char*)vtvari[4]);
				oneStrwrite((char*)vtvari[5]);
			}
			else
				oneStrwrite((char*)sla);
			//close
			oneStrwrite((char*)headinpair[5]);
			oneStrwrite((char*)headinpair[4]);
		}
	}else
		oneStrwrite((char*)sla);

	oneStrwrite((char*)titpair[0]);
	if (Tvects) {
		wd[wl] = clo; wl++;
		//vector size
		oneStrwrite((char*)headinpair[1]);
		if(Tvector_size)
			oneStrplusDoubleq((char*)headinpair[2], Tvector_size);
		if(Tvector_baseT)
			oneStrplusDoubleq((char*)headinpair[3], Tvector_baseT);
		wd[wl] = clo; wl++;

		for (UINT32 i = 0; i < Title_lp_size; i++) {
			tagbetwrite((char*)vtvari[1], Tvects[i]->lpstr, (char*)vtvari[2]);
		}

		oneStrwrite((char*)headinpair[5]);

		oneStrwrite((char*)titpair[1]);
	}
	else
		oneStrwrite((char*)sla);

	tagbetwrite((char*)ladstr[0], Compa, (char*)ladstr[1]);
	tagbetwrite((char*)ladstr[2], LinksUDate, (char*)ladstr[3]);
	tagbetwrite((char*)ladstr[4], ShaDoc, (char*)ladstr[5]);
	tagbetwrite((char*)ladstr[6], HyperL, (char*)ladstr[7]);
	tagbetwrite((char*)ladstr[8], appVer, (char*)ladstr[9]);

	oneStrwrite((char*)ladstr[10]);
}

void App_File::tagbetwrite(char* t, UINT8* v, char* ct) {
	char clo = '>';
	const char* sla = "/>";

	oneStrwrite((char*)t);
	if (v) {
		wd[wl] = clo; wl++;
		oneStrwrite((char*)v);
		oneStrwrite((char*)ct);
	}
	else
		oneStrwrite((char*)sla);
	
}

void App_File::oneStrwrite(char* str) {

	int i = 0;

	while (str[i] != '\0') {
		wd[wl] = str[i];
		i++;
		wl++;
	}
}

void App_File::oneStrplusDoubleq(char* str, UINT8* v) {

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