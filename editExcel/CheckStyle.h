#pragma once

#include "stylewrite.h"

#include <stringapiset.h>



class checkstyle :public StyleWrite
{
public:
	UINT8* style = nullptr;//s 決定
	int stylelen = 0;

	checkstyle();
	~checkstyle();

	int searchfonts(Fonts* fs);

	int searchcellstylexfs(stylexf* fs);

	int searchcellxfs(cellxfs* fs);

	int searchcellstyle(cellstyle* fs);

	int searchfills(Fills* fs);

	bool searchborderstyle(borderstyle* f, borderstyle* fs);

	int searchborders(borders* fs);

	void CountincIiment(UINT8* cstr);

	UINT8* searchnumFmts(numFMts* fs);

	int strcompare(UINT8* sl, UINT8* sr);

	UINT8* configstyle(UINT8* num);

	char* SJIStoUTF8(char* szSJIS, char* bufUTF8, int size);

private:

	const char* zo = "zozo";
	const char *sm = "smarby";
	const char* sh = "shoplist";
	const char *be = "bee";
	const char* ms = "magaseek";
};

checkstyle::checkstyle()
{
}

checkstyle::~checkstyle()
{
}

inline int checkstyle::searchfonts(Fonts* fs){
	Fonts** f = fontRoot;
	int result = 0;
	int count = 0;
	bool flag = false;

	//std::cout << "fs theme : " << fs->color << std::endl;
	while (count < frcount) {
		result = strcompare(f[count]->sz, fs->sz);
		if (result == 0) {
			result = strcompare(f[count]->rgb, fs->rgb);
			if (result == 0) {
				result = strcompare(f[count]->charset, fs->charset);
				if (result == 0) {
					result = strcompare(f[count]->scheme, fs->scheme);
					if (result == 0) {
						result = strcompare(f[count]->color, fs->color);
						if (result == 0) {
							result = strcompare(f[count]->name, fs->name);
							if (result == 0) {
								result = strcompare(f[count]->family, fs->family);
								if (result == 0) {
									flag = true;
									break;
								}
							}
						}
					}
				}
			}
		}
		count++;
	}

	if (!flag)
		count = -1;
	else
		std::cout << "match font : " << count << std::endl;

	return count;
}

inline int checkstyle::searchcellstylexfs(stylexf* fs)
{
	stylexf** f = cellstyleXfsRoot;
	int result = 0;
	int count = 0;
	bool flag = false;

	//std::cout << "fs aA : " << fs->fillId << " " << f->fillId << std::endl;
	while (count < csXcount) {
		result = strcompare(f[count]->applyAlignment, fs->applyAlignment);
		if (result == 0) {
			result = strcompare(f[count]->applyBorder, fs->applyBorder);
			if (result == 0) {
				result = strcompare(f[count]->applyFont, fs->applyFont);
				if (result == 0) {
					result = strcompare(f[count]->applyNumberFormat, fs->applyNumberFormat);
					if (result == 0) {
						result = strcompare(f[count]->applyProtection, fs->applyProtection);
						if (result == 0) {
							result = strcompare(f[count]->Avertical, fs->Avertical);
							if (result == 0) {
								result = strcompare(f[count]->borderId, fs->borderId);
								if (result == 0) {
									result = strcompare(f[count]->fillId, fs->fillId);
									if (result == 0) {
										result = strcompare(f[count]->fontId, fs->fontId);
										if (result == 0) {
											result = strcompare(f[count]->numFmtId, fs->numFmtId);
											if (result == 0) {
												flag = true;
												std::cout << "match cellstylexfms : " << count << std::endl;
												return count;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		count++;
	}

	return -1;
}

inline int checkstyle::searchcellxfs(cellxfs* fs)
{
	cellxfs** f = cellXfsRoot;
	int result = 0;
	int count = 0;

	bool flag = false;

	//std::cout << "fs aA : " << fs->fontId << " " << f->fontId << std::endl;
	while (count < cXcount) {
		result = strcompare(f[count]->applyAlignment, fs->applyAlignment);
		if (result == 0) {
			result = strcompare(f[count]->applyBorder, fs->applyBorder);
			if (result == 0) {
				result = strcompare(f[count]->applyFont, fs->applyFont);
				if (result == 0) {
					result = strcompare(f[count]->applyNumberFormat, fs->applyNumberFormat);
					if (result == 0) {
						result = strcompare(f[count]->applyFill, fs->applyFill);
						if (result == 0) {
							result = strcompare(f[count]->Avertical, fs->Avertical);
							if (result == 0) {
								result = strcompare(f[count]->borderId, fs->borderId);
								if (result == 0) {
									result = strcompare(f[count]->fillId, fs->fillId);
									if (result == 0) {
										result = strcompare(f[count]->fontId, fs->fontId);
										if (result == 0) {
											result = strcompare(f[count]->numFmtId, fs->numFmtId);
											if (result == 0) {
												result = strcompare(f[count]->AwrapText, fs->AwrapText);
												if (result == 0) {
													result = strcompare(f[count]->xfId, fs->xfId);
													if (result == 0) {
														result = strcompare(f[count]->horizontal, fs->horizontal);
														if (result == 0) {
															flag = true;
															break;
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		count++;
	}

	if (!flag)
		count = -1;

	else
		std::cout << "match cellxfms : " << count << std::endl;

	return count;
}

//cellstyle 検索　xfid戻り
inline int checkstyle::searchcellstyle(cellstyle* fs)
{
	cellstyle** f = cellStyleRoot;
	int result = 0;
	int count = 0;
	bool flag = false;
	//std::cout << "fs name : " << fs->name << " " << f->name << std::endl;

	while (count<cscount) {
		/*if (f[count]->name) {
			result = strcompare(f[count]->xfId, fs->xfId);
			if (result == 0) {
				result = strcompare(f[count]->xruid, fs->xruid);
				if (result == 0) {
					flag = true;
					break;
				}
			}
		}*/
		if (f[count]->name) {
			result = strcompare(f[count]->name, fs->name);
			if (result == 0) {
				flag = true;
				break;
			}
		}
		count++;
	}

	if (!flag) {
		count = -1;
	}
	else {
		std::cout << "match xfid : " << f[count]->xfId << std::endl;
	}
	return count;
}

inline int checkstyle::searchfills(Fills* fs)
{
	Fills** f = fillroot;
	int result = 0;
	int count = 0;
	bool flag = false;
	bool fgflag = false;
	bool bgflag = false;	

	while (count < ficount){
		if (f[count]->patten && fs->patten){
			result = strcompare(f[count]->patten->patternType, fs->patten->patternType);
			if (result == 0)
				flag = true;		
		}else if (!f[count]->patten && !fs->patten) {
			flag = true;
		}else {
			flag = false;
		}

		if (f[count]->fg && fs->fg){
			result = strcompare(f[count]->fg->rgb, fs->fg->rgb);
			if (result == 0){
				result = strcompare(f[count]->fg->theme, fs->fg->theme);
				if (result == 0){
					result = strcompare(f[count]->fg->tint, fs->fg->tint);
					if (result == 0)
						fgflag = true;
				}
			}
		}else if (!f[count]->fg && !fs->fg)
		{
			fgflag = true;
		}else {
			fgflag = false;
		}

		if (f[count]->bg && fs->bg){
			result = strcompare(f[count]->bg->indexed, fs->bg->indexed);
			if (result == 0) {
				bgflag = true;
			}
		}else if (!f[count]->bg && !fs->bg)
		{
			bgflag = true;
		}else {
			bgflag = false;
		}

		if (flag && fgflag && bgflag)
			break;

		count++;
	}

	if (!flag || !fgflag || !bgflag){
		count = -1;
		std::cout << "not match fills : " << count << std::endl;
	}
	else
		std::cout << "match fills : " << count << std::endl;

	return count;
}

inline bool checkstyle::searchborderstyle(borderstyle* f, borderstyle* fs) {
	bool flag = false;
	int result = 0;

	if (f && fs) {
		result = strcompare(f->rgb, fs->rgb);
		if (result == 0) {
			result = strcompare(f->Auto, fs->Auto);
			if (result == 0) {
				result = strcompare(f->index, fs->index);
				if (result == 0) {
					result = strcompare(f->style, fs->style);
					if (result == 0) {
						result = strcompare(f->theme, fs->theme);
						if (result == 0) {
							result = strcompare(f->tint, fs->tint);
							if (result == 0) {
								flag = true;
							}
						}
					}
				}
			}
		}
	}
	else if (!f && !fs) {
		flag = true;
	}
	return flag;
}

inline int checkstyle::searchborders(borders* fs)
{
	borders** f = BorderRoot;
	int result = 0;
	int count = 0;
	bool lflag = false;
	bool rflag = false;
	bool bflag = false;
	bool tflag = false;
	bool dflag = false;

	while (count < bocount) {

		lflag = searchborderstyle(f[count]->left, fs->left);
		rflag = searchborderstyle(f[count]->right, fs->right);
		bflag = searchborderstyle(f[count]->bottom, fs->bottom);
		tflag = searchborderstyle(f[count]->top, fs->top);
		dflag = searchborderstyle(f[count]->diagonal, fs->diagonal);

		if (lflag && rflag && bflag && tflag && dflag)
			break;

		count++;

	}

	if (!lflag || !rflag || !tflag || !bflag || !dflag) {
		count = -1;
		std::cout << "not match border : " << count << std::endl;
	}
	else
		std::cout << "match border : " << count << std::endl;

	return count;
}

inline void checkstyle::CountincIiment(UINT8* cstr)
{
	int l = 0;
	UINT32 count = 0;
	while (cstr[l] != '\0')
		l++;

	std::cout << "count ;" << cstr << std::endl;

	count = strchange.RowArraytoNum(cstr, l);
	count++;
	cstr = strchange.InttoChar(count, &l);
	std::cout << "count plus ;" << cstr << std::endl;
}

inline UINT8* checkstyle::searchnumFmts(numFMts* fs)
{
	//numFMts* f = numFmtsRoot;
	int result = 0;
	int count = 0;
	bool flag = false;
	UINT8* nu = nullptr;

	//std::cout << "fs sz : " << fs->sz << std::endl;
	while (count < nucount) {
		result = strcompare(numFmtsRoot[count]->Code, fs->Code);
		if (result == 0) {
			flag = true;
			break;
		}
		count++;
	}

	if (!flag) {
		nu = (UINT8*)malloc(1); nu = nullptr;
		return nu;
	}
	else
		std::cout << "match numFmts : " << count << std::endl;

	size_t leng = strlen((const char*)numFmtsRoot[count]->Id) + 1;
	nu = (UINT8*)malloc(leng);
	memcpy(nu, numFmtsRoot[count]->Id, leng);

	return nu;
}

inline int checkstyle::strcompare(UINT8* sl, UINT8* sr)
{
	int result = 1;

	if (sl && sr)
		result = strcmp((const char*)sl, (const char*)sr);
	else if (!sl && !sr)
		result = 0;
	else
		result = 1;

	return result;
}

inline UINT8* checkstyle::configstyle(UINT8* num)
{
	Fonts* fo = (Fonts*)malloc(sizeof(Fonts));
	Fills* fi = (Fills*)malloc(sizeof(Fills));
	cellxfs* cx = (cellxfs*)malloc(sizeof(cellxfs));
	stylexf* csx = (stylexf*)malloc(sizeof(stylexf));
	numFMts* nf = (numFMts*)malloc(sizeof(numFMts));
	borders* bd = (borders*)malloc(sizeof(borders));
	cellstyle* cs = (cellstyle*)malloc(sizeof(cellstyle));
	UINT8* nullbox = nullptr;

	int fontnumb = 0;//検索番号
	int place = 0;//検索番号　桁数


	//--------------- 共通設定　cellXFs -------------------//

	//applyNumberFormat applyAlignment vertical wraptext numFmId
	const char* bewraptext[] = { "1","1","center","1","0" };

	//ID検索以外の値入れる
	cx->applyNumberFormat = (UINT8*)malloc(2);
	memcpy(cx->applyNumberFormat, (UINT8*)bewraptext[0], 2);
	cx->applyAlignment = (UINT8*)malloc(2);
	memcpy(cx->applyAlignment, (UINT8*)bewraptext[1], 2);
	cx->Avertical = (UINT8*)malloc(7);
	memcpy(cx->Avertical, (UINT8*)bewraptext[2], 7);
	cx->AwrapText = (UINT8*)malloc(2);
	memcpy(cx->AwrapText, (UINT8*)bewraptext[3], 2);
	cx->numFmtId = (UINT8*)malloc(2);
	memcpy(cx->numFmtId, (UINT8*)bewraptext[4], 2);

	cx->applyBorder = nullptr;
	cx->horizontal = nullptr;
	cx->applyFill = nullptr;
	cx->applyFont = nullptr;
	cx->quotePrefix = nullptr;

	//--------------- 共通設定　フォント -------------------//

	const char fname[] = "ＭＳ Ｐゴシック";
	char foname[255] = { 0 };

	char* fon = nullptr;
	fon = SJIStoUTF8((char*)fname, foname, 255);//フォントname UTF-8に変換

	const char* sharefonts[] = { "11", "2","128","minor" };//sz family charset scheme
	int nalen = 0;
	fo->sz = (UINT8*)malloc(3);
	memcpy(fo->sz, (UINT8*)sharefonts[0], 3);

	fo->name = (UINT8*)malloc(strlen(fon) + 1);
	while (fon[nalen] != '\0') {
		fo->name[nalen] = fon[nalen];
		nalen++;
	}
	fo->name[nalen] = '\0';

	fo->family = (UINT8*)malloc(2);
	memcpy(fo->family, (UINT8*)sharefonts[1], 2);
	fo->charset = (UINT8*)malloc(4);
	memcpy(fo->charset, (UINT8*)sharefonts[2], 4);
	fo->scheme = (UINT8*)malloc(6);
	memcpy(fo->scheme, (UINT8*)sharefonts[3], 6);
	fo->biu = (UINT8*)malloc(1); fo->biu = nullptr;
	fo->indexed = (UINT8*)malloc(1); fo->indexed = nullptr;


	//--------------- 共通設定　フィル -------------------//

	//patternType fgrgb
	const char patternFill[] = "solid";
	fi->patten = (FillPattern*)malloc(sizeof(FillPattern));
	fi->patten->patternType = (UINT8*)malloc(6);
	memcpy(fi->patten->patternType, (UINT8*)patternFill, 6);


	//--------------- 共通設定　ボーダー  -------------------//

	bd->left = nullptr;
	bd->right = nullptr;
	bd->top = nullptr;
	bd->bottom = nullptr;
	bd->diagonal = nullptr;


	//--------------- 共通設定　cellstyleXFs  -------------------//

	const char* bexfids[] = { "0","0","center" };
	csx->applyAlignment = (UINT8*)malloc(2);
	memcpy(csx->applyAlignment, (UINT8*)bexfids[1], 2);
	csx->applyBorder = (UINT8*)malloc(2);
	memcpy(csx->applyBorder, (UINT8*)bexfids[0], 2);
	csx->Avertical = (UINT8*)malloc(7);
	memcpy(csx->Avertical, (UINT8*)bexfids[2], 7);

	csx->applyNumberFormat = (UINT8*)malloc(2);
	strcpy_s((char*)csx->applyNumberFormat, 2,bexfids[0]);
	csx->applyProtection = (UINT8*)malloc(2);
	strcpy_s((char*)csx->applyProtection, 2, bexfids[0]);

	csx->applyFont = nullptr;	
	csx->applyFill = nullptr;
	csx->wraptext = nullptr;

	//-------------  共通設定　cellstyleXfs numFmts収集 ----------------//
	//numFmid 検索
	//numFmtId //code
	const char benumFmCode[] = "[$$-409]#,##0.00;[$$-409]#,##0.00"; //共通 numFmts code

	char codena[255] = { 0 };

	char* codefon = nullptr;
	codefon = SJIStoUTF8((char*)benumFmCode, codena, 255);//フォントname UTF-8に変換
	nf->Code = (UINT8*)malloc(strlen(codefon) + 1);
	int codelen = 0;
	while (codefon[codelen] != '\0') {
		nf->Code[codelen] = codefon[codelen];
		codelen++;
	}
	nf->Code[codelen] = '\0';

	nf->Id = nullptr;//検索用

	csx->numFmtId = searchnumFmts(nf);//numFmt検索 ID加える

	if (csx->numFmtId) {
		std::cout << "match numfmts : " << csx->numFmtId << std::endl;
		free(nf->Code); free(nf->Id); free(nf);
	}else {
		//numFmts 加える
		//フォント設定追加　必要
		size_t resiz = (size_t)numFmtsNum + 1;
		numFMts** rexfs = nullptr;

		rexfs = (numFMts**)realloc(numFmtsRoot, sizeof(numFMts) * resiz);
		numFmtsRoot = rexfs;

		UINT8* numID = numFmtsRoot[numFmtsNum - 1]->Id;//最後のID取得
		int numidLen = 0;
		while (numID[numidLen] != '\0')
			numidLen++;
		UINT32 idnumb = strchange.RowArraytoNum(numID, numidLen);//ID 数次に
		idnumb++;
		
		rexfs[numFmtsNum] = (numFMts*)malloc(sizeof(numFMts));
		rexfs[numFmtsNum]->Id = strchange.InttoChar(idnumb, &place);//ID 入力

		rexfs[numFmtsNum]->Code = (UINT8*)malloc(strlen(codefon) + 1);
		codelen = 0;
		while (codefon[codelen] != '\0') {
			rexfs[numFmtsNum]->Code[codelen] = codefon[codelen];
			codelen++;
		}
		rexfs[numFmtsNum]->Code[codelen] = '\0';

		std::cout << "numfmts id: " << idnumb << " " << rexfs[numFmtsNum]->Id << std::endl;
		std::cout << "numfmts code: " << idnumb << " " << rexfs[numFmtsNum]->Code << std::endl;

		numFmtsNum++; nucount++;
		free(numFmtsCount);
		numFmtsCount = strchange.InttoChar(numFmtsNum, &place);

		free(nf->Code); free(nf->Id); free(nf);
	}

	//bee cellstyle  <cellStyle name="標準 225" xfId="44468" xr:uid="{00000000-0005-0000-0000-0000E0750000}"/>
	//shoplist cellstyle <cellStyle name="標準" xfId="0" builtinId="0"/>
	//zozo same
	//magaseek same
	//smarby
	//-------------  共通設定　cellstyle zozo設定 ----------------//
	//buildinId customBuild name
	const char* zostyle[] = { "0","標準" };
	char zozocsname[255] = { 0 };

	cs->builtinId = (UINT8*)malloc(3);
	memcpy(cs->builtinId, (UINT8*)zostyle[0], 3);
	cs->customBuilt = nullptr;

	char* zocsname = nullptr;
	zocsname = SJIStoUTF8((char*)zostyle[1], zozocsname, 255);//フォントname UTF-8に変換
	int zocsnalen = 0;
	cs->name = (UINT8*)malloc(strlen(zocsname) + 1);
	while (zocsname[zocsnalen] != '\0') {
		cs->name[zocsnalen] = zocsname[zocsnalen];
		zocsnalen++;
	}
	cs->name[zocsnalen] = '\0';

	cs->xfId = nullptr;
	cs->xruid = nullptr;

	/*-----------------------------------
		bee style設定
	-----------------------------------*/
	//入力文字　ショップ別
	int resultshop = 0;
	resultshop = strcmp((const char*)num, be);	
	if (resultshop == 0) {
		//bee
		//fonts 設定
		//------------- ショップ別 フォント色 -----------------//
		//font sz rgb name family charset scheme

		const char befonts[] = "FF006100";
		fo->rgb = (UINT8*)malloc(9);
		memcpy(fo->rgb, (UINT8*)befonts, 9);
		fo->color = nullptr;

		//fill 設定
		//------------- ショップ別 フォント色 -----------------//
		const char befill[] = "FFC6EFCE";//他null

		fi->fg = (fgcolor*)malloc(sizeof(fgcolor));
		fi->fg->rgb = (UINT8*)malloc(9);
		memcpy(fi->fg->rgb, (UINT8*)befill, 9);
		fi->fg->theme = nullptr;
		fi->fg->tint = nullptr;
		fi->bg = nullptr;
	}


	//23
	/*-----------------------------------
	shoplist style設定 他　fillが違うだけ
	-----------------------------------*/
	resultshop = strcmp((const char*)num, sh);
	if (resultshop == 0) {
		std::cout << "shoplist" << std::endl;
		//applyNumberFormat vertical horizontal wraptext
		//const char* shwraptext[] = { "1","center","left","1" };
		//------------- ショップ別 cellXFs -----------------//
		//shoplist
		const char sholhorizen[] = "left";//vartival
		//vertical変更
		free(cx->horizontal);
	
		cx->horizontal = (UINT8*)malloc(5);//color theme
		memcpy(cx->horizontal, (UINT8*)sholhorizen, 5);

		//------------- ショップ別 フォント色 -----------------//
		//shoplist
		const char sholtheme[] = "0";//theme
		fo->color = (UINT8*)malloc(2);//color theme
		memcpy(fo->color, (UINT8*)sholtheme, 2);
		fo->rgb = nullptr;//color rgb どちらか

		//fill 設定
		//------------- ショップ別 フィル色 -----------------//
		//shoplist
		const char sholfill[] = "4";//theme 他null
		fi->fg = (fgcolor*)malloc(sizeof(fgcolor));
		fi->fg->rgb = nullptr;//rgb
		fi->fg->theme = (UINT8*)malloc(2);
		memcpy(fi->fg->theme, (UINT8*)sholfill, 2);//theme
		fi->fg->tint = nullptr;
		fi->bg = nullptr;
	}

	resultshop = strcmp((const char*)num, zo);
	if (resultshop == 0) {
		std::cout << "zozo" << std::endl;
		//------------- ショップ別 フォント色 -----------------//
		//zozo
		const char zozotheme[] = "1";//theme
		fo->color = (UINT8*)malloc(2);//color theme
		memcpy(fo->color, (UINT8*)zozotheme, 2);
		fo->rgb = nullptr;//color rgb どちらか

		//------------- ショップ別 フィル色 -----------------//
		//zozo
		// fgtheme fgtint bgindexed
		const char* zofill[] = { "4","0.79998168889431442","65" };
		fi->fg = (fgcolor*)malloc(sizeof(fgcolor));
		fi->fg->theme = (UINT8*)malloc(2);
		memcpy(fi->fg->theme, (UINT8*)zofill[0], 2);//theme
		fi->fg->tint = (UINT8*)malloc(20);
		memcpy(fi->fg->tint, (UINT8*)zofill[1], 20);//tint
		fi->fg->rgb = (UINT8*)malloc(1); fi->fg->rgb = nullptr;//rgb

		fi->bg = (bgcolor*)malloc(sizeof(bgcolor));//なければストラクトをヌルに
		fi->bg->indexed = (UINT8*)malloc(3);
		memcpy(fi->bg->indexed, (UINT8*)zofill[2], 3);//tint
	}


	resultshop = strcmp((const char*)num, sm);
	if (resultshop == 0) {
		std::cout << "smarby" << std::endl;
		//------------- ショップ別 フォント色 -----------------//
		//smarby
		const char smtheme[] = "1";//theme
		fo->color = (UINT8*)malloc(2);//color theme
		memcpy(fo->color, (UINT8*)smtheme, 2);
		fo->rgb = nullptr;//color rgb どちらか

		//------------- ショップ別 フィル色 -----------------//
		//smarby
		const char* smfill[] = { "5","0.79998168889431442","65" };
		fi->fg = (fgcolor*)malloc(sizeof(fgcolor));
		fi->fg->theme = (UINT8*)malloc(2);
		memcpy(fi->fg->theme, (UINT8*)smfill[0], 2);//theme
		fi->fg->tint = (UINT8*)malloc(20);
		memcpy(fi->fg->tint, (UINT8*)smfill[1], 20);//tint
		fi->fg->rgb = nullptr;//rgb
		fi->bg = (bgcolor*)malloc(sizeof(bgcolor));//なければストラクトをヌルに
		fi->bg->indexed = (UINT8*)malloc(3);
		memcpy(fi->bg->indexed, (UINT8*)smfill[2], 3);//tint
	}


	resultshop = strcmp((const char*)num, ms);
	if (resultshop == 0) {
		std::cout << "magaseek : " << std::endl;
		//------------- ショップ別 フォント色 -----------------//
		//magaseek
		const char mstheme[] = "1";//theme
		fo->color = (UINT8*)malloc(2);//color theme
		memcpy(fo->color, (UINT8*)mstheme, 2);
		fo->rgb = nullptr;//color rgb どちらか

		//------------- ショップ別 フィル色 -----------------//
		//magaseek
		const char* mafill[] = { "6","0.79998168889431442","65" };
		fi->fg = (fgcolor*)malloc(sizeof(fgcolor));
		fi->fg->theme = (UINT8*)malloc(2);
		memcpy(fi->fg->theme, (UINT8*)mafill[0], 2);//theme
		fi->fg->tint = (UINT8*)malloc(20);
		memcpy(fi->fg->tint, (UINT8*)mafill[1], 20);//tint
		fi->fg->rgb = nullptr;

		fi->bg = (bgcolor*)malloc(sizeof(bgcolor));//なければストラクトをヌルに
		fi->bg->indexed = (UINT8*)malloc(3);
		memcpy(fi->bg->indexed, (UINT8*)mafill[2], 3);//tint
	}

	/*フォントidの検索*/
	fontnumb = searchfonts(fo);
	if (fontnumb != -1){
		cx->fontId = strchange.InttoChar(fontnumb, &place);//一致番号入力　fontID 加える
		csx->fontId = strchange.InttoChar(fontnumb, &place);
		free(fo->sz); free(fo->rgb); free(fo->family); free(fo->charset); free(fo->scheme); free(fo->color); free(fo);
	}else {
		//フォント設定追加　必要
		size_t resiz = (size_t)fontNum + 1;
		Fonts** rexfs = nullptr;
		rexfs = (Fonts**)realloc(fontRoot, sizeof(Fonts)*resiz);

		fontRoot = rexfs;

		if (!rexfs) {
			std::cout << "not keep memory" << std::endl;
			
		}
		else {
			//for (int i = 0; i < fontNum; i++)
				//free(fontRoot[i]);
		}
		std::cout << "need add fonts" << fontNum<<std::endl;
		rexfs[fontNum] = (Fonts*)malloc(sizeof(Fonts));
		rexfs[fontNum]->sz = fo->sz;
		rexfs[fontNum]->name = fo->name;
		rexfs[fontNum]->rgb = fo->rgb;
		rexfs[fontNum]->indexed = fo->indexed;
		rexfs[fontNum]->family = fo->family;
		rexfs[fontNum]->charset = fo->charset;
		rexfs[fontNum]->scheme = fo->scheme;
		rexfs[fontNum]->color = fo->color;
		rexfs[fontNum]->biu = fo->biu;

		frcount++; fontNum++;//ボーダー数プラス

		free(fontCount);
		fontCount = strchange.InttoChar(fontNum, &place);

		cx->fontId = strchange.InttoChar(fontNum-1, &place);
		csx->fontId = strchange.InttoChar(fontNum-1, &place);
	}

	/*フィルidの検索*/
	fontnumb = searchfills(fi);
	if (fontnumb != -1)
	{
		cx->fillId = strchange.InttoChar(fontnumb, &place);//一致番号入力 フィルID加える
		csx->fillId = strchange.InttoChar(fontnumb, &place);

		free(fi->patten->patternType);
		if (fi->fg) {
			free(fi->fg->rgb); free(fi->fg->theme); free(fi->fg->tint);
		}
		if(fi->bg)
			free(fi->bg->indexed);
		free(fi->fg); free(fi->bg);
		free(fi->patten); free(fi);
	}
	else {
		//フィル設定追加　必要
		size_t resiz = (size_t)fillNum + 1;
		Fills** rexfs = nullptr;

		rexfs = (Fills**)realloc(fillroot, sizeof(Fills)*resiz);
		fillroot = rexfs;

		if (!rexfs) {
			std::cout << "not keep memory" << std::endl;
			
		}
		std::cout << "need add cellstyleXfs" << std::endl;

		rexfs[fillNum] = (Fills*)malloc(sizeof(Fills));
		rexfs[fillNum]->patten = (FillPattern*)malloc(sizeof(FillPattern));
		rexfs[fillNum]->patten->patternType = fi->patten->patternType;
		rexfs[fillNum]->fg = (fgcolor*)malloc(sizeof(fgcolor));
		rexfs[fillNum]->fg->rgb = fi->fg->rgb;
		rexfs[fillNum]->fg->theme = fi->fg->theme;
		rexfs[fillNum]->fg->tint = fi->fg->tint;
		rexfs[fillNum]->bg = (bgcolor*)malloc(sizeof(bgcolor));
		rexfs[fillNum]->bg->indexed = fi->bg->indexed;

		ficount++; fillNum++;//ボーダー数プラス

		free(fillCount);
		fillCount = strchange.InttoChar(fillNum, &place);
		cx->fillId = strchange.InttoChar(fillNum-1, &place);
		csx->fillId = strchange.InttoChar(fillNum-1, &place);
	}

	/*ボーダーidの検索*/
	fontnumb = searchborders(bd);
	if (fontnumb != -1)
	{
		cx->borderId = strchange.InttoChar(fontnumb, &place);//一致番号入力 ボーダー設定加える
		csx->borderId = strchange.InttoChar(fontnumb, &place);

		free(bd->left); free(bd->right); free(bd->top); free(bd->bottom); free(bd->diagonal); free(bd);
	}
	else {//ボーダー設定追加　必要
		size_t resiz = (size_t)borderNum + 1;
		borders** rexfs = nullptr;

		rexfs = (borders**)realloc(BorderRoot, sizeof(borders) * resiz);
		BorderRoot = rexfs;

		if (!rexfs) {
			std::cout << "not keep memory" << std::endl;
			
		}
		std::cout << "need add cellstyleXfs" << std::endl;

		rexfs[borderNum] = (borders*)malloc(sizeof(borders));
		rexfs[borderNum]->left = bd->left;
		rexfs[borderNum]->right = bd->right;
		rexfs[borderNum]->top = bd->top;
		rexfs[borderNum]->left = bd->left;
		rexfs[borderNum]->bottom = bd->bottom;
		rexfs[borderNum]->diagonal = bd->diagonal;

		bocount++; borderNum++;//ボーダー数プラス
		free(borderCount);

		borderCount = strchange.InttoChar(borderNum, &place);
		cx->borderId = strchange.InttoChar(borderNum-1, &place);
		csx->borderId = strchange.InttoChar(borderNum-1, &place);
	}


	//cellstyle xfs 番号取得
	fontnumb = searchcellstylexfs(csx);
	if (fontnumb != -1) {
		cx->xfId = strchange.InttoChar(fontnumb, &place);//一致番号入力 bee以外はこっち
		free(csx->applyAlignment); free(csx->applyBorder); free(csx->applyFont); free(csx->applyNumberFormat); free(csx->applyProtection);
		free(csx->Avertical); free(csx);
		free(cs->builtinId); free(cs->customBuilt); free(cs->name); free(cs->xruid);
	}
	else {
		//cellstyle xfs 設定追加　必要
		size_t resiz = (size_t)cellstyleXfsNum + 1;
		stylexf** rexfs = nullptr;
		rexfs = (stylexf**)realloc(cellstyleXfsRoot, sizeof(stylexf) * resiz);//元オブジェクト解放される
		cellstyleXfsRoot = rexfs;
		if (!rexfs) {
			std::cout << "not keep memory" << std::endl;
			
		}
		std::cout << "need add cellstyleXfs" << cellstyleNum << " resize: " << resiz << std::endl;
		rexfs[csXcount] = (stylexf*)malloc(sizeof(stylexf));
		rexfs[csXcount]->numFmtId = csx->numFmtId;
		rexfs[csXcount]->fontId = csx->fontId;
		rexfs[csXcount]->fillId = csx->fillId;
		rexfs[csXcount]->borderId = csx->borderId;
		rexfs[csXcount]->applyNumberFormat = csx->applyNumberFormat;
		rexfs[csXcount]->applyBorder = csx->applyBorder;
		rexfs[csXcount]->applyAlignment = csx->applyAlignment;
		rexfs[csXcount]->applyProtection = csx->applyProtection;
		rexfs[csXcount]->Avertical = csx->Avertical;//alignment vertical
		rexfs[csXcount]->applyFont = csx->applyFont;
		rexfs[csXcount]->applyFill = csx->applyFill;

		csXcount++; cellstyleXfsNum++;

		free(cellStyleXfsCount);
		cellStyleXfsCount = strchange.InttoChar(cellstyleXfsNum, &place);

		cx->xfId = strchange.InttoChar(cellstyleXfsNum-1, &place);

		//--------- cellstyle 設定追加 --------//
		cs->xfId = strchange.InttoChar(cellstyleXfsNum-1, &place);

		resiz = (size_t)cellstyleNum + 1;
		cellstyle** csre = nullptr;
		csre = (cellstyle**)realloc(cellStyleRoot, sizeof(cellstyle) * resiz);
		cellStyleRoot = csre;

		csre[cscount] = (cellstyle*)malloc(sizeof(cellstyle));
		csre[cscount]->builtinId = cs->builtinId;
		csre[cscount]->customBuilt = cs->customBuilt;
		csre[cscount]->name = cs->name;
		csre[cscount]->xfId = cs->xfId;
		csre[cscount]->xruid = cs->xruid;

		cscount++; cellstyleNum++;

		free(cellstyleCount);
		cellstyleCount = strchange.InttoChar(cellstyleNum, &place);
	}

	//style 決定
	fontnumb = searchcellxfs(cx);
	if (fontnumb != -1)
	{
		style = strchange.InttoChar(fontnumb, &place);//一致番号入力　style 決定
		stylelen = place;

		std::cout << "style : " << style << std::endl;
		free(cx->fontId);free(cx->fillId); free(cx->applyAlignment); free(cx->applyBorder); free(cx->applyFill); free(cx->applyFont);
		free(cx->applyNumberFormat);  free(cx->borderId);
		free(cx->numFmtId); free(cx->xfId); free(cx->AwrapText);free(cx->Avertical);
	}else {
		//xfID 設定追加　必要
		size_t resiz = (size_t)cellXfsNum + 1;
		cellxfs** rexfs = nullptr;

		rexfs = (cellxfs**)realloc(cellXfsRoot, (sizeof(cellxfs) * resiz));

		cellXfsRoot = rexfs;
		if (!rexfs) {
			std::cout << "not keep memory" << std::endl;
			
		}

		rexfs[cellXfsNum] = (cellxfs*)malloc(sizeof(cellxfs));
		rexfs[cellXfsNum]->fontId = cx->fontId;
		rexfs[cellXfsNum]->fillId = cx->fillId;
		rexfs[cellXfsNum]->applyAlignment = cx->applyAlignment;
		rexfs[cellXfsNum]->applyBorder = cx->applyBorder;
		rexfs[cellXfsNum]->applyFill = cx->applyFill;
		rexfs[cellXfsNum]->applyFont = cx->applyFont;
		rexfs[cellXfsNum]->applyNumberFormat = cx->applyNumberFormat;
		rexfs[cellXfsNum]->borderId = cx->borderId;
		rexfs[cellXfsNum]->numFmtId = cx->numFmtId;
		rexfs[cellXfsNum]->xfId = cx->xfId;
		rexfs[cellXfsNum]->AwrapText = cx->AwrapText;
		rexfs[cellXfsNum]->Avertical = cx->Avertical;
		rexfs[cellXfsNum]->horizontal = cx->horizontal;

		std::cout << "セル追加 : "<<rexfs[cellXfsNum]->fontId<<"  num  "<< cellXfsNum << std::endl;

		cellXfsNum++; cXcount++;
		free(cellXfsCount);
		cellXfsCount = strchange.InttoChar(cellXfsNum, &place);

		style = strchange.InttoChar(cellXfsNum-1, &place);
		//style 番号
		stylelen = place;

		std::cout << "style : " << style << std::endl;
	}
	//UINT32 stylenum = strchange.RowArraytoNum(num, strlen((const char*)num));//style 番号　数字に変換
	return nullptr;
}

inline char* checkstyle::SJIStoUTF8(char* szSJIS, char* bufUTF8, int size)
{
	wchar_t bufUnicode[100];
	int lenUnicode = MultiByteToWideChar(CP_ACP, 0, szSJIS, strlen(szSJIS) + 1, bufUnicode, 100);
	WideCharToMultiByte(CP_UTF8, 0, bufUnicode, lenUnicode, bufUTF8, size, NULL, NULL);

	return bufUTF8;
}

