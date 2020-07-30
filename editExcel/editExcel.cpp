#include "deflate.h"
#include "Header.h"
#include "timeget.h"
#include "slidewindow.h"
#include "CRC.h"
#include "encode.h"
#include "decord.h"
#include "compressdata.h"
#include "SearchItemNUm.h"
#include "ShipDataRead.h"
#include "ZipFilewrite.h"
#include "shareRandW.h"
#include "RowColumn.h"
#include <codecvt>
#include "atlbase.h"
#include "atlstr.h"
#include "comutil.h"
#include "StyleWrite.h"
#include <locale.h>
#include "CheckStyle.h"

/*
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
*/

#define BUFSIZE  256

char* SJIStoUTF8(char* szSJIS, char* bufUTF8, int size) {

    wchar_t bufUnicode[BUFSIZE];

    int lenUnicode = MultiByteToWideChar(CP_ACP, 0, szSJIS, strlen(szSJIS) + 1, bufUnicode, BUFSIZE);

    WideCharToMultiByte(CP_UTF8, 0, bufUnicode, lenUnicode, bufUTF8, size, NULL, NULL);

    return bufUTF8;

}

std::string UTF8toSjis(std::string srcUTF8) {
    //Unicodeへ変換後の文字列長を得る
    int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, NULL, 0);

    //必要な分だけUnicode文字列のバッファを確保
    wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

    //UTF8からUnicodeへ変換
    MultiByteToWideChar(CP_UTF8, 0, srcUTF8.c_str(), srcUTF8.size() + 1, bufUnicode, lenghtUnicode);

    //ShiftJISへ変換後の文字列長を得る
    int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

    //必要な分だけShiftJIS文字列のバッファを確保
    char* bufShiftJis = new char[lengthSJis];

    //UnicodeからShiftJISへ変換
    WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

    std::string strSJis(bufShiftJis);

    delete[] bufUnicode;
    delete[] bufShiftJis;

    return strSJis;
}

char* convchar(wchar_t* a) {

    size_t origsize = wcslen(a) + 1;

    size_t convertedChars = 0;
    const size_t newsize = origsize * 2;
    char* nstring = new char[newsize];
    wcstombs_s(&convertedChars, nstring, newsize, a, _TRUNCATE);

    return nstring;
}

int main(char* fname[], int i) {

    // メモリリーク検出
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    char Zfn[] = "C:/Users/ryo19/OneDrive/デスクトップ/excelfiles/【1 在庫状況】.xlsx";
    char mfile[] = "C:/Users/ryo19/OneDrive/デスクトップ/teststyle.xml"; //テスト書き出し
    char recd[] = "C:/Users/ryo19/OneDrive/デスクトップ/Centraldata"; //テスト書き出し
    char wridata[] = "C:/Users/ryo19/OneDrive/デスクトップ/xmls/";

    char shares[] = "sharedStrings.xml";
    char stylefn[] = "xl/styles.xml";

    /*
    // Get current flag
    int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
    // Turn on leak-checking bit.
    tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
    // Set flag to the new value.
    _CrtSetDbgFlag(tmpFlag);
    */
    /*-----------------------
       shareシート読み込み
     -----------------------*/

    CenterDerect* cddata = nullptr;

    std::ifstream Zr(Zfn, std::ios::in | std::ios::binary);
    if (!Zr) {
        std::cout << "not open file" << std::endl;
        return 0;
    }

    HeaderRead* hr2 = new HeaderRead(Zfn);

    hr2->endread(&Zr);//終端コードの読み込み
    DeflateDecode* decShare = new DeflateDecode(&Zr);

    while (hr2->filenum < hr2->ER->centralsum) {
        cddata = hr2->centeroneread(hr2->readpos, hr2->ER->size, hr2->ER->centralnum, shares, &Zr);

        if (cddata)
            break;
    }

    if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
        hr2->localread(cddata->localheader, &Zr);//sharesstringsの読み込み
        decShare->dataread(hr2->LH->pos, cddata->nonsize);
    }
    shareRandD* sharray = new shareRandD(decShare->ReadV, decShare->readlen);//share 再初期化 検索用に呼び出し
    sharray->getSicount();//get si count
    sharray->ReadShare();//文字列読み込み
    delete hr2;
    delete decShare;

    /*-----------------------
   スタイルシート読み込み
   -----------------------*/

    hr2 = new HeaderRead(Zfn);

    hr2->endread(&Zr);//終端コードの読み込み
    
    DeflateDecode* Sdeco = new DeflateDecode(&Zr);

    while (hr2->filenum < hr2->ER->centralsum) {

        cddata = hr2->centeroneread(hr2->readpos, hr2->ER->size, hr2->ER->centralnum, stylefn, &Zr);
        if (cddata)
            break;
        hr2->freeheader();
    }

    if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
        hr2->localread(cddata->localheader, &Zr);//sharesstringsの読み込み
        Sdeco->dataread(hr2->LH->pos, cddata->nonsize);
    }
    FILE* f = nullptr;
    fopen_s(&f, mfile, "wb");
    if (!f)
        printf("ファイルを開けませんでした");

    checkstyle* sr = new checkstyle();

    sr->readstyle(Sdeco->ReadV, Sdeco->readlen);

    int stylec = 0;
    //bee 1012
    //magaseek 710
    //smarby 1038
    //zozo 711
    //shoplist 715
    ArrayNumber* changeStr = new ArrayNumber;
    std::cout << std::endl;
    std::cout << "style 3893 cellXfs 検索" << std::endl;
    std::cout << std::endl;
    if (sr->cellXfsRoot[3893]->numFmtId)
        std::cout << "numFmtId : " << sr->cellXfsRoot[3893]->numFmtId << std::endl;
    if (sr->cellXfsRoot[3893]->fontId)
        std::cout << "fontId : " << sr->cellXfsRoot[3893]->fontId << std::endl;
    if (sr->cellXfsRoot[3893]->fillId)
        std::cout << "fillId : " << sr->cellXfsRoot[3893]->fillId << std::endl;
    if (sr->cellXfsRoot[3893]->borderId)
        std::cout << "borderId : " << sr->cellXfsRoot[3893]->borderId << std::endl;
    if (sr->cellXfsRoot[3893]->xfId)
        std::cout << "xfId : " << sr->cellXfsRoot[3893]->xfId << std::endl;
    if (sr->cellXfsRoot[3893]->applyNumberFormat)
        std::cout << "applyNumberFormat : " << sr->cellXfsRoot[3893]->applyNumberFormat << std::endl;
    if (sr->cellXfsRoot[3893]->applyFont)
        std::cout << "applyFont : " << sr->cellXfsRoot[3893]->applyFont << std::endl;
    if (sr->cellXfsRoot[3893]->applyFill)
        std::cout << "applyFill : " << sr->cellXfsRoot[3893]->applyFill << std::endl;
    if (sr->cellXfsRoot[3893]->applyBorder)
        std::cout << "applyBorder : " << sr->cellXfsRoot[3893]->applyBorder << std::endl;
    if (sr->cellXfsRoot[3893]->applyAlignment)
        std::cout << "applyAlignment : " << sr->cellXfsRoot[3893]->applyAlignment << std::endl;
    if (sr->cellXfsRoot[3893]->Avertical)
        std::cout << "vertical : " << sr->cellXfsRoot[3893]->Avertical << std::endl;
    if (sr->cellXfsRoot[3893]->horizontal)
        std::cout << "horizontal : " << sr->cellXfsRoot[3893]->horizontal << std::endl;
    if (sr->cellXfsRoot[3893]->AwrapText)
        std::cout << "wrapText : " << sr->cellXfsRoot[3893]->AwrapText << std::endl;

    int place = 0;
    //fontid 検索
    while (sr->cellXfsRoot[3893]->fontId[place] != '\0')
        place++;
    UINT32 fontnum = changeStr->RowArraytoNum(sr->cellXfsRoot[3893]->fontId, place);
    stylec = 0;

    Fonts* srf = sr->fontRoot[fontnum];

    std::cout << std::endl;
    std::cout << "フォント検索 " << fontnum << std::endl;
    std::cout << std::endl;
    if (srf->sz)
        std::cout << "sz : " << srf->sz << std::endl;
    if (srf->color)
        std::cout << "theme : " << srf->color << std::endl;
    if (srf->rgb)
        std::cout << "rgb : " << srf->rgb << std::endl;
    if (srf->name)
        std::cout << "name : " << srf->name << std::endl;
    if (srf->family)
        std::cout << "family : " << srf->family << std::endl;
    if (srf->charset)
        std::cout << "charset : " << srf->charset << std::endl;
    if (srf->scheme)
        std::cout << "scheme : " << srf->scheme << std::endl;
    if (srf->rgb)
        std::cout << "rgb : " << srf->rgb << std::endl;
    //fillid 検索
    place = 0;

    while (sr->cellXfsRoot[3893]->fillId[place] != '\0')
        place++;
    fontnum = changeStr->RowArraytoNum(sr->cellXfsRoot[3893]->fillId, place);
    stylec = 0;

    Fills* fils = sr->fillroot[fontnum];

    std::cout << std::endl;
    std::cout << "fill検索 " << fontnum << std::endl;
    std::cout << std::endl;
    if (fils->patten)
        std::cout << "patternType : " << fils->patten->patternType << std::endl;
    if (fils->fg) {
        if (fils->fg->rgb)
            std::cout << "fgColor rgb : " << fils->fg->rgb << std::endl;
        if (fils->fg->theme)
            std::cout << "fgColor theme : " << fils->fg->theme << std::endl;
        if (fils->fg->tint)
            std::cout << "fgColor tint : " << fils->fg->tint << std::endl;
    }
    if (fils->bg) {
        if (fils->bg->indexed)
            std::cout << "bgColor indexed : " << fils->bg->indexed << std::endl;
    }
    //ボーダー 検索
    place = 0;
    while (sr->cellXfsRoot[3893]->borderId[place] != '\0')
        place++;
    fontnum = changeStr->RowArraytoNum(sr->cellXfsRoot[3893]->borderId, place);
    stylec = 0;

    std::cout << std::endl;
    std::cout << "border検索 " << fontnum << std::endl;
    std::cout << std::endl;
    if (sr->BorderRoot[fontnum]->left)
        std::cout << "left style : " << sr->BorderRoot[fontnum]->left->style << std::endl;
    else
        std::cout << "left 何もなし " << std::endl;
    if (sr->BorderRoot[fontnum]->right)
        std::cout << "right style : " << sr->BorderRoot[fontnum]->right->style << std::endl;
    else
        std::cout << "right 何もなし " << std::endl;
    if (sr->BorderRoot[fontnum]->bottom)
        std::cout << "bottom style : " << sr->BorderRoot[fontnum]->top->style << std::endl;
    else
        std::cout << "bottom 何もなし " << std::endl;
    if (sr->BorderRoot[fontnum]->top)
        std::cout << "top style : " << sr->BorderRoot[fontnum]->top->style << std::endl;
    else
        std::cout << "top 何もなし " << std::endl;
    //xfId 検索
    place = 0;


    while (sr->cellXfsRoot[3893]->xfId[place] != '\0')
        place++;
    fontnum = changeStr->RowArraytoNum(sr->cellXfsRoot[3893]->xfId, place);
    stylexf* cxs = sr->cellstyleXfsRoot[fontnum];

    stylec = 0;

    std::cout << std::endl;
    std::cout << "xfIdでcellstyleXfs 検索 " << fontnum << std::endl;
    std::cout << std::endl;
    if (cxs->numFmtId)
        std::cout << "numFmtId : " << cxs->numFmtId << std::endl;
    if (cxs->fontId)
        std::cout << "fontId : " << cxs->fontId << std::endl;
    if (cxs->fillId)
        std::cout << "fillId : " << cxs->fillId << std::endl;
    if (cxs->borderId)
        std::cout << "borderId : " << cxs->borderId << std::endl;
    if (cxs->applyNumberFormat)
        std::cout << "applyNumberFormat : " << cxs->applyNumberFormat << std::endl;
    if (cxs->applyFont)
        std::cout << "applyFont : " << cxs->applyFont << std::endl;
    if (cxs->applyBorder)
        std::cout << "applyBorder : " << cxs->applyBorder << std::endl;
    if (cxs->applyAlignment)
        std::cout << "applyAlignment : " << cxs->applyAlignment << std::endl;
    if (cxs->applyProtection)
        std::cout << "applyProtection : " << cxs->applyProtection << std::endl;
    if (cxs->Avertical)
        std::cout << "vertical : " << cxs->Avertical << std::endl;
    if (cxs->wraptext)
        std::cout << "wraptext : " << cxs->wraptext << std::endl;
    if (cxs->applyFill)
        std::cout << "applyFill : " << cxs->applyFill << std::endl;
    if (cxs->applyFont)
        std::cout << "applyFont : " << cxs->applyFill << std::endl;
    //cellstyleXfs のnumfmid をnumFmtsから検索
    place = 0;
    stylec = 0;
    int res = 0;
    for (int nu = 0; nu < sr->numFmtsNum; nu++) {
        res = strcmp((const char*)sr->numFmtsRoot[nu]->Id, (const char*)(cxs->numFmtId));
        if (res == 0) {
            res = nu;
            break;
        }
    }
    std::cout << std::endl;
    std::cout << "numFmtId検索 " << cxs->numFmtId << std::endl;
    std::cout << std::endl;
    if (sr->numFmtsRoot[res]->Id)
        std::cout << "numFmtId : " << sr->numFmtsRoot[res]->Id << std::endl;
    if (sr->numFmtsRoot[res]->Code)
        std::cout << "code : " << sr->numFmtsRoot[res]->Code << std::endl;


    //xfidでcellstyle検索　sr->cellXfsRoot[3893]->xfId
    for (int nu = 0; nu < sr->cellstyleNum; nu++) {
        res = strcmp((const char*)sr->cellStyleRoot[nu]->xfId, (const char*)sr->cellXfsRoot[3893]->xfId);
        if (res == 0) {
            res = nu;
            break;
        }
    }

    searchItemNum sIN(nullptr,nullptr);
    std::cout << std::endl;
    std::cout << "xfID一致 : " << res << std::endl;
    std::cout << std::endl;
    if (sr->cellStyleRoot[res]->builtinId)
        std::cout << "builtinId : " << sr->cellStyleRoot[res]->builtinId << std::endl;
    if (sr->cellStyleRoot[res]->customBuilt)
        std::cout << "customBuilt : " << sr->cellStyleRoot[res]->customBuilt << std::endl;
    if (sr->cellStyleRoot[res]->name) {
        char* csnb = sIN.CharChenge(sr->cellStyleRoot[res]->name);
        std::cout << "name : " << csnb << std::endl;
    }
        
    if (sr->cellStyleRoot[res]->xfId)
        std::cout << "xfId : " << sr->cellStyleRoot[res]->xfId << std::endl;
    if (sr->cellStyleRoot[res]->xruid)
        std::cout << "xruid : " << sr->cellStyleRoot[res]->xruid << std::endl;

    UINT32 styleleng = Sdeco->readlen;//style 解凍データ長
    UINT8 be[] = "zozo";    

    sr->configstyle(be);
    sr->styledatawrite(styleleng);

    for (UINT64 i = 0; i < sr->wdlen;i++)
        fwrite(&sr->wd[i], sizeof(char), 1, f);

    delete sr;

    delete Sdeco;
    
    /*-----------------------

   発注到着シート読み込み

   -----------------------*/
    /*
    DeflateDecode* Hdeco;

    char sheet[] = "worksheets/sheet";
    encoding* enc = nullptr;

    bool t = false;
    Ctags* mh;//発注到着　cell データ読み込み
    CDdataes* slideCDdata = hr2->saveCD;//ファイル名検索用

    hr2->readpos = hr2->ER->position;//読み込み位置初期化

    hr2->filenum = 0;//レコード数初期化

    int result = 0;

    while (hr2->filenum < hr2->ER->centralsum) {
        //ファイル名 sheet 部分一致検索

        cddata = hr2->centeroneread(hr2->readpos, hr2->ER->size, hr2->ER->centralnum, sheet, &Zr);

        if (cddata) {
            std::cout << "sheet一致ファイルネーム：" << cddata->filename << std::endl;



            hr2->localread(cddata->localheader, &Zr);//"worksheets/sheet"に一致するファイルの中身検索

            Hdeco = new DeflateDecode(&Zr);//解凍
            Hdeco->dataread(hr2->LH->pos, cddata->nonsize);//解凍　データ読み込み

            std::cout << "decode len : "<<cddata->nonsize << std::endl;
            
            mh = new Ctags(Hdeco->ReadV, Hdeco->readlen, sharray);//シートデータ読み込み
            mh->sheetread();
            mh->writesheetdata();

            encoding* enc = new encoding;//圧縮
            enc->compress(Hdeco->ReadV, Hdeco->readlen);//データ圧縮
            delete enc;
                        
            hr2->freeLH();
            //delete mh;
            delete Hdeco;//デコードデータ　削除 
        }
        hr2->freeheader();        
    }

    std::cout << "end item search" << std::endl;
    */
    delete sharray;

    delete hr2;

    Zr.close();

    fclose(f);

    //_CrtDumpMemoryLeaks();//メモリ リーク レポートを表示

    return 0;

}