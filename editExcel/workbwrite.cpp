#include "workbookEdit.h"

void WorkBook_edi::writeworkbook() {
	const char* headstr[] = { "<workbook"," xmlns=\""," xmlns:r=\""," xmlns:mc=\""," xmlns:x15=\""," xmlns:xr=\"",
	" xmlns:xr6=\""," xmlns:xr10=\""," xmlns:xr2=\""," mc:Ignorable=\"" };
	const char* filver[] = { "<fileVersion"," appName=\""," lastEdited=\""," lowestEdited=\""," rupBuild=\"" };
	const char* wbPr[] = { "<workbookPr"," codeName=\""," defaultThemeVersion=\"" };
	const char* mcstr[] = { "<mc:AlternateContent"," xmlns:mc=\"","<mc:Choice"," Requires=\"","<x15ac:absPath"," xmlns:x15ac=\"",
	" url=\"","</mc:Choice>","</mc:AlternateContent>" };
	const char* reviPtr[] = { "<xr:revisionPtr"," revIDLastSave=\""," documentId=\""," xr6:coauthVersionLast=\""," xr6:coauthVersionMax=\"",
	" xr10:uidLastSave=\"" };
	const char* bview[] = { "<bookViews>","<workbookView"," xWindow=\""," yWindow=\""," windowWidth=\""," windowHeight=\""," tabRatio=\"",
	" firstSheet=\""," activeTab=\""," xr2:uid=\"","</bookViews>" };
	const char* sheetstr[] = { "<sheets>","<sheet"," name=\""," sheetId=\""," r:id=\"","</sheets>" };
	const char* exrefe[] = { "<externalReferences>","<externalReference"," r:id=\"","</externalReferences>" };
	const char* defname[] = { "<definedNames>","<definedName" ," name=\""," localSheetId=\""," hidden=\"","</definedName>","</definedNames>" };
	const char* calcstr[] = { "<calcPr"," calcId=\"" };
	const char* exlststr[] = { "<extLst>","<ext"," xmlns:xcalcf=\""," uri=\"","<xcalcf:calcFeatures>","<xcalcf:feature"," name=\"","</xcalcf:calcFeatures>",
	"</ext>","</extLst>" };
	const char* laststr = "</workbook>";
	char clo = '>';
	char sla[] = "/>";

	size_t siz = dl + 1000;
	wd = (UINT8*)malloc(siz);

	oneStrwrite((char*)titlestr);
	oneStrwrite((char*)headstr[0]);

	if (wbroot->xmlns)
		oneStrplusDoubleq((char*)headstr[1], wbroot->xmlns);
	if (wbroot->r)
		oneStrplusDoubleq((char*)headstr[2], wbroot->r);
	if (wbroot->mc)
		oneStrplusDoubleq((char*)headstr[3], wbroot->mc);
	if (wbroot->x15)
		oneStrplusDoubleq((char*)headstr[4], wbroot->x15);
	if (wbroot->xr)
		oneStrplusDoubleq((char*)headstr[5], wbroot->xr);
	if (wbroot->xr6)
		oneStrplusDoubleq((char*)headstr[6], wbroot->xr6);
	if (wbroot->xr10)
		oneStrplusDoubleq((char*)headstr[7], wbroot->xr10);
	if (wbroot->xr2)
		oneStrplusDoubleq((char*)headstr[8], wbroot->xr2);
	if (wbroot->Ignorable)
		oneStrplusDoubleq((char*)headstr[9], wbroot->Ignorable);
	wd[wl] = clo; wl++;

	if (fVroot) {
		oneStrwrite((char*)filver[0]);
		if (fVroot->appName)
			oneStrplusDoubleq((char*)filver[1], fVroot->appName);
		if (fVroot->lastEdited)
			oneStrplusDoubleq((char*)filver[2], fVroot->lastEdited);
		if (fVroot->lowestEdited)
			oneStrplusDoubleq((char*)filver[3], fVroot->lowestEdited);
		if (fVroot->rupBuild)
			oneStrplusDoubleq((char*)filver[4], fVroot->rupBuild);
		oneStrwrite(sla);
	}

	if (wProot) {
		oneStrwrite((char*)wbPr[0]);
		if(wProot->codeName)
			oneStrplusDoubleq((char*)wbPr[1], wProot->codeName);
		if (wProot->defaultThemeVersion)
			oneStrplusDoubleq((char*)wbPr[2], wProot->defaultThemeVersion);
		oneStrwrite(sla);
	}

	if (mc_al) {
		oneStrwrite((char*)mcstr[0]);
		if(mc_al)
			oneStrplusDoubleq((char*)mcstr[1], mc_al);
		wd[wl] = clo; wl++;

		if (mcho) {
			oneStrwrite((char*)mcstr[2]);
			if (mcho->Requires || xfifteen) {
				if (mcho->Requires)
					oneStrplusDoubleq((char*)mcstr[3], mcho->Requires);
				wd[wl] = clo; wl++;
				if (xfifteen) {
					oneStrwrite((char*)mcstr[4]);
					if (xfifteen->x15ac)
						oneStrplusDoubleq((char*)mcstr[5], xfifteen->x15ac);
					if (xfifteen->url)
						oneStrplusDoubleq((char*)mcstr[6], xfifteen->url);
					oneStrwrite(sla);
				}
				oneStrwrite((char*)mcstr[7]);
			}
			else
			{
				oneStrwrite(sla);
			}
			
		}
		oneStrwrite((char*)mcstr[8]);
	}

	if (reviPtrroot) {
		oneStrwrite((char*)reviPtr[0]);
		if(reviPtrroot->revIDLastSave)
			oneStrplusDoubleq((char*)reviPtr[1], reviPtrroot->revIDLastSave);
		if (reviPtrroot->documentId)
			oneStrplusDoubleq((char*)reviPtr[2], reviPtrroot->documentId);
		if (reviPtrroot->coauthVersionLast)
			oneStrplusDoubleq((char*)reviPtr[3], reviPtrroot->coauthVersionLast);
		if (reviPtrroot->coauthVersionMax)
			oneStrplusDoubleq((char*)reviPtr[4], reviPtrroot->coauthVersionMax);
		if (reviPtrroot->uidLastSave)
			oneStrplusDoubleq((char*)reviPtr[5], reviPtrroot->uidLastSave);
		oneStrwrite(sla);
	}

	if (wVroot) {
		oneStrwrite((char*)bview[0]);

		oneStrwrite((char*)bview[1]);
		if(wVroot->xWindow)
			oneStrplusDoubleq((char*)bview[2], wVroot->xWindow);
		if (wVroot->yWindow)
			oneStrplusDoubleq((char*)bview[3], wVroot->yWindow);
		if (wVroot->windowWidth)
			oneStrplusDoubleq((char*)bview[4], wVroot->windowWidth);
		if (wVroot->windowHeight)
			oneStrplusDoubleq((char*)bview[5], wVroot->windowHeight);
		if (wVroot->tabRatio)
			oneStrplusDoubleq((char*)bview[6], wVroot->tabRatio);
		if (wVroot->firstSheet)
			oneStrplusDoubleq((char*)bview[7], wVroot->firstSheet);
		if (wVroot->activeTab)
			oneStrplusDoubleq((char*)bview[8], wVroot->activeTab);
		if (wVroot->uid)
			oneStrplusDoubleq((char*)bview[9], wVroot->uid);
		oneStrwrite(sla);

		oneStrwrite((char*)bview[10]);
	}

	if (wbshroot) {
		oneStrwrite((char*)sheetstr[0]);

		wb_sheets* wbsroo = wbshroot;
		while (wbsroo) {
			oneStrwrite((char*)sheetstr[1]);
			if(wbsroo->name)
				oneStrplusDoubleq((char*)sheetstr[2], wbsroo->name);
			if (wbsroo->sheetId)
				oneStrplusDoubleq((char*)sheetstr[3], wbsroo->sheetId);
			if (wbsroo->id)
				oneStrplusDoubleq((char*)sheetstr[4], wbsroo->id);
			oneStrwrite(sla);
			wbsroo = wbsroo->next;
		}
		oneStrwrite((char*)sheetstr[5]);
	}

	if (exRroot) {
		oneStrwrite((char*)exrefe[0]);

		extReferences* exrroo = exRroot;
		while (exrroo) {
			oneStrwrite((char*)exrefe[1]);
			if(exrroo->id)
				oneStrplusDoubleq((char*)exrefe[2], exrroo->id);
			oneStrwrite(sla);
			exrroo = exrroo->next;
		}

		oneStrwrite((char*)exrefe[3]);
	}

	if (dfnroot) {
		oneStrwrite((char*)defname[0]);

		dfnames* dfroo = dfnroot;
		while (dfroo) {
			oneStrwrite((char*)defname[1]);
			if (dfroo->name)
				oneStrplusDoubleq((char*)defname[2], dfroo->name);
			if (dfroo->localSheetId)
				oneStrplusDoubleq((char*)defname[3], dfroo->localSheetId);
			if (dfroo->hidden)
				oneStrplusDoubleq((char*)defname[4], dfroo->hidden);
			wd[wl] = clo; wl++;
			oneStrwrite((char*)dfroo->dfname);
			oneStrwrite((char*)defname[5]);
			dfroo = dfroo->next;
		}

		oneStrwrite((char*)defname[6]);
	}

	if (calcPr) {
		oneStrwrite((char*)calcstr[0]);
		oneStrplusDoubleq((char*)calcstr[1], calcPr);
		oneStrwrite(sla);
	}

	if (exroot) {
		oneStrwrite((char*)exlststr[0]);

		oneStrwrite((char*)exlststr[1]);
		if(exroot->xcalcf)
			oneStrplusDoubleq((char*)exlststr[2], exroot->xcalcf);
		if (exroot->uri)
			oneStrplusDoubleq((char*)exlststr[3], exroot->uri);
		wd[wl] = clo; wl++;

		feature* froo = exroot->calcFeature;
		if (froo) {
			oneStrwrite((char*)exlststr[4]);
			while (froo) {
				oneStrwrite((char*)exlststr[5]);
				if(froo->name)
					oneStrplusDoubleq((char*)exlststr[6], froo->name);
				oneStrwrite(sla);

				froo = froo->next;
			}
			oneStrwrite((char*)exlststr[7]);
		}
		oneStrwrite((char*)exlststr[8]);

		oneStrwrite((char*)exlststr[9]);
	}

	oneStrwrite((char*)laststr);
}

void WorkBook_edi::oneStrwrite(char* str) {

	int i = 0;

	while (str[i] != '\0') {
		wd[wl] = str[i];
		i++;
		wl++;
	}
}

void WorkBook_edi::oneStrplusDoubleq(char* str, UINT8* v) {

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