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

#include "AppFile.h"
#include "workbookEdit.h"
#include "Content_Type_edit.h"
#include "workbook_rel_edi.h"

#include <stdlib.h>
#include <crtdbg.h>

#define _CRTDBG_MAP_ALLOC


#define BUFSIZE  256

std::wstring multi_to_wide_capi(std::string const& src)
{
    std::size_t converted{};
    std::vector<wchar_t> dest(src.size(), L'\0');
    if (::_mbstowcs_s_l(&converted, dest.data(), dest.size(), src.data(), _TRUNCATE, ::_create_locale(LC_ALL, "jpn")) != 0) {
        throw std::system_error{ errno, std::system_category() };
    }
    dest.resize(std::char_traits<wchar_t>::length(dest.data()));
    dest.shrink_to_fit();
    return std::wstring(dest.begin(), dest.end());
}

std::string wide_to_utf8_cppapi(std::wstring const& src)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(src);
}

std::string multi_to_utf8_cppapi(std::string const& src)
{
    auto const wide = multi_to_wide_capi(src);
    return wide_to_utf8_cppapi(wide);
}

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
    char PLfile[] = "C:/Users/ryo19/OneDrive/デスクトップ/excelfiles/LY200212-SHIP ゾゾ.xlsx"; //テスト書き出し
    char mfile[] = "C:/Users/ryo19/OneDrive/デスクトップ/testapp.xml"; //テスト書き出し
    char recd[] = "C:/Users/ryo19/OneDrive/デスクトップ/Centraldata"; //テスト書き出し
    char wridata[] = "C:/Users/ryo19/OneDrive/デスクトップ/xmls/";

    char shares[] = "sharedStrings.xml";
    char stylefn[] = "xl/styles.xml";
    char sheetname[] = "worksheets/sheet1.xml";
    
    WCHAR wStrW[] = _T("2020,ship,5/19着");
    //変換文字列格納バッファ
    wchar_t intxto[] = _T("2020,shiptyt着");

    std::string conv = wide_to_utf8_cppapi(intxto);//utf8へ
    const char* newinput = conv.c_str();
    char* Mnstring = convchar(intxto);

    //変換文字列格納バッファ
   //char	inMainstr[50];
    char	wStrC[250];
    size_t wLen = 0;
    errno_t err = 0;

    //ロケール指定
    setlocale(LC_ALL, "japanese");

    //変換
    err = wcstombs_s(&wLen, wStrC, 250, intxto, _TRUNCATE);
    char* sMstr = (char*)malloc(100);
    char* inMainstr = (char*)malloc(100);
    sMstr = SJIStoUTF8(wStrC, sMstr, 100);

    int wlen = 0;
    while (sMstr[wlen] != '\0') {
        inMainstr[wlen] = sMstr[wlen];
        wlen++;
    }
    inMainstr[wlen] = '\0';
    std::cout << "wchar to char : " << inMainstr << " " << wLen << std::endl;
    
    
    //------------- パッキングリスト--------------------//
    std::ifstream PLR(PLfile, std::ios::in | std::ios::binary);
    if (!PLR)
        return 0;

    HeaderRead* hr = new HeaderRead(PLfile);
    CenterDerect* cddata = nullptr;//セントラルディレクトのデータ
    hr->endread(&PLR);//終端コードの読み込み
    hr->freeER();

    DeflateDecode* decShare = new DeflateDecode(&PLR);
    //share セントラル取得
    while (hr->filenum < hr->ER->centralsum) {//cddata mallocなし
        cddata = hr->centeroneread(hr->readpos, hr->ER->size, hr->ER->centralsum, shares, &PLR);
        if (cddata) {
            break;
        }
        hr->freeheader();
    }
    if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
        hr->localread(cddata->localheader, &PLR);//sharesstringsの読み込み
        decShare->dataread(hr->LH->pos, cddata->nonsize);
    }
    hr->freeLH();
    //hr->freeheader();

    shareRandD* sharray = new shareRandD(decShare->ReadV, decShare->readlen);//share string read to array
    sharray->getSicount();//get si count
    sharray->ReadShare();//文字列読み込み デコードデータ削除OK
    delete decShare;

    //------share read---------//
    DeflateDecode* decsheet = new DeflateDecode(&PLR);
    hr->filenum = 0;//レコード数初期化
    hr->readpos = hr->ER->position;
    cddata = nullptr;
    while (hr->filenum < hr->ER->centralsum) {
        cddata = hr->centeroneread(hr->readpos, hr->ER->size, hr->ER->centralsum, sheetname, &PLR);//セントラルディレクトのデータ
        if (cddata) {
            break;
        }
        hr->freeheader();
    }
    if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
        hr->localread(cddata->localheader, &PLR);//sharesstringsの読み込み
        decsheet->dataread(hr->LH->pos, cddata->nonsize);
    }
    hr->freeLH();
    hr->freeheader();

    Ctags* ms = new Ctags(decsheet->ReadV, decsheet->readlen, sharray);

    ms->sheetread();

    delete decsheet;//デコードデータ削除

    shipinfo* sg = new shipinfo(ms->rows);//シートデータ参照　freeなし
    sg->GetItems();//mallocなし　シートとセット

    delete ms;

    PLR.close();
    delete sharray;
    
    //-----------------------//
    //  shareシート読み込み
    //-----------------------//
    std::ifstream Zr(Zfn, std::ios::in | std::ios::binary);
    if (!Zr)
        return 0;

    HeaderRead* hr2 = new HeaderRead(Zfn);

    hr2->endread(&Zr);//終端コードの読み込み
    decShare = new DeflateDecode(&Zr);
    while (hr2->filenum < hr2->ER->centralsum) {
        cddata = hr2->centeroneread(hr2->readpos, hr2->ER->size, hr2->ER->centralnum, shares, &Zr);
        if (cddata)
            break;
        hr2->freeheader();
    }
    if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
        hr2->localread(cddata->localheader, &Zr);//sharesstringsの読み込み
        decShare->dataread(hr2->LH->pos, cddata->nonsize);
        hr2->freeLH();
    }

    hr2->freeheader();
    shareRandD *hattyushare = new shareRandD(decShare->ReadV, decShare->readlen);//share 再初期化 検索用に呼び出し

    hattyushare->getSicount();//get si count
    hattyushare->ReadShare();//文字列読み込み
    
    //--- test txt
    //char inMainstr[] = "mainstr";
    char inSubstr[] = "432";
    char inTwostr[] = "";
    char inThreestr[] = "";
    char inFourstr[] = "";
    hattyushare->searchSi(inSubstr, inTwostr, inThreestr, inFourstr);//マッチした文字列のSi番号取得　なければnullptr

    //シェアー書き込み
    UINT8* sharedata = nullptr;//share書き込み　データ

    //シェアーデータ書き込み
    hattyushare->siwrite();

    //sharedata = hattyushare->writeshare((UINT8*)inMainstr, strlen(inMainstr), inFourstr, inThreestr, inTwostr, inSubstr);//share文字列書き込み share data更新
    UINT64 shrelength = hattyushare->writeleng;
    free(sharedata);//書き込みデータ解放

    delete hr2;
    delete decShare;

   //-----------------------
   //スタイルシート読み込み
   //-----------------------//
    
    HeaderRead *hHinfo = new HeaderRead(Zfn);
    hHinfo->endread(&Zr);//終端コードの読み込み
    DeflateDecode* Sdeco = new DeflateDecode(&Zr);

    while (hHinfo->filenum < hHinfo->ER->centralsum) {
        cddata = hHinfo->centeroneread(hHinfo->readpos, hHinfo->ER->size, hHinfo->ER->centralnum, stylefn, &Zr);
        if (cddata)
            break;
        hHinfo->freeheader();
    }
    if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
        hHinfo->localread(cddata->localheader, &Zr);//sharesstringsの読み込み
        Sdeco->dataread(hHinfo->LH->pos, cddata->nonsize);
        hHinfo->freeLH();
    }
    hHinfo->freeheader();   

    checkstyle* sr = new checkstyle();

    sr->readstyle(Sdeco->ReadV, Sdeco->readlen);

    int stylec = 0;
    //bee 1012
    //magaseek 710
    //smarby 1038
    //zozo 711
    //shoplist 715
    ArrayNumber* changeStr = new ArrayNumber;
    UINT32 styleleng = Sdeco->readlen;//style 解凍データ長
    UINT8 be[] = "bee";    

    sr->configstyle(be);

    char* styleset = nullptr;
    UINT32 memsiz = UINT32(sr->stylelen) + 1;
    styleset = (char*)malloc(memsiz);
    strcpy_s(styleset, memsiz, (const char*)sr->style);//style copy

    sr->styledatawrite(styleleng);

    delete sr;
    delete Sdeco;

    //-----------------------

   //発注到着シート読み込み

   //-----------------------//
    
    DeflateDecode* Hdeco=nullptr;

    char sheet[] = "worksheets/sheet";
    char appfile[] = "docProps/app.xml";
    char workb[] = "xl/workbook.xml";
    char content[] = "[Content_Types]";
    char wbrel[] = "xl/_rels/workbook.xml.rels";
    char draw[] = "xl/drawings/drawing8.xml";

    bool t = false;
    Ctags* mh=nullptr;//発注到着　cell データ読み込み
    CDdataes* slideCDdata = hHinfo->saveCD;//ファイル名検索用
    searchItemNum* sI=nullptr;//品番検索　＆　書き込み

    hHinfo->readpos = hHinfo->ER->position;//読み込み位置初期化
    hHinfo->filenum = 0;//レコード数初期化
    int result = 0;

    //品番、カラーエラー用
    MatchColrs* matchs = nullptr;
    MatchColrs* matchsroot = nullptr;
    char teststyl[] = "200";//test用
    
    while (hHinfo->filenum < hHinfo->ER->centralsum) {
        //ファイル名 sheet 部分一致検索
        cddata = hHinfo->centeroneread(hHinfo->readpos, hHinfo->ER->size, hHinfo->ER->centralnum, draw, &Zr);

        if (cddata) {
            std::cout << "sheet一致ファイルネーム：" << cddata->filename<<" "<< cddata->nonsize << std::endl;
            if (!Zr)
                return 0;
            hHinfo->localread(cddata->localheader, &Zr);//"worksheets/sheet"に一致するファイルの中身検索

            Hdeco = new DeflateDecode(&Zr);//解凍
            Hdeco->dataread(hHinfo->LH->pos, cddata->nonsize);//解凍　データ読み込み

            workb_rels ap(Hdeco->ReadV, Hdeco->readlen);
            ap.readwbrels();
            ap.writewbrel();

            FILE* f = nullptr;
            fopen_s(&f, mfile, "wb");
            if (!f)
                printf("ファイルを開けませんでした");

            for (UINT64 i = 0; i < ap.wl; i++)
                fwrite(&ap.wd[i], sizeof(char), 1, f);
            fclose(f);

    
            //mh = new Ctags(Hdeco->ReadV, Hdeco->readlen, hattyushare);//シートデータ読み込み
            //mh->sheetread();
              /*
            sI = new searchItemNum(mh);
            t = sI->searchitemNumber(hattyushare->uniqstr, hattyushare->inputsinum[3], hattyushare->inputsinum[2], hattyushare->inputsinum[1], hattyushare->inputsinum[0], (char*)styleset, teststyl,sg->its);
            if (t) {
                mh->writesheetdata();//シートデータ書き込み
                crc CRC;
                CRC.rescrc();
                CRC.mcrc(mh->wd, mh->p);//crc 計算

                encoding* enc = new encoding;//圧縮
                enc->compress(mh->wd, mh->p);//データ圧縮
                delete enc;                
            }
            */
            //delete mh;
            delete Hdeco;//デコードデータ　削除
            delete sI;
            
            hHinfo->freeLH();
        }
        hHinfo->freeheader();
    }
    std::cout << "end item search" << std::endl;
    

    delete hHinfo;
    sg->freeits();

    delete hattyushare;
    delete sg;    
    if(Zr)
        Zr.close();
    
    //_CrtDumpMemoryLeaks();//メモリ リーク レポートを表示

    return 0;
}
/*
std::cout << std::endl;
    std::cout << "style 1012 cellXfs 検索" << std::endl;
    std::cout << std::endl;
    if (sr->cellXfsRoot[1012]->numFmtId)
        std::cout << "numFmtId : " << sr->cellXfsRoot[1012]->numFmtId << std::endl;
    if (sr->cellXfsRoot[1012]->fontId)
        std::cout << "fontId : " << sr->cellXfsRoot[1012]->fontId << std::endl;
    if (sr->cellXfsRoot[1012]->fillId)
        std::cout << "fillId : " << sr->cellXfsRoot[1012]->fillId << std::endl;
    if (sr->cellXfsRoot[1012]->borderId)
        std::cout << "borderId : " << sr->cellXfsRoot[1012]->borderId << std::endl;
    if (sr->cellXfsRoot[1012]->xfId)
        std::cout << "xfId : " << sr->cellXfsRoot[1012]->xfId << std::endl;
    if (sr->cellXfsRoot[1012]->applyNumberFormat)
        std::cout << "applyNumberFormat : " << sr->cellXfsRoot[1012]->applyNumberFormat << std::endl;
    if (sr->cellXfsRoot[1012]->applyFont)
        std::cout << "applyFont : " << sr->cellXfsRoot[1012]->applyFont << std::endl;
    if (sr->cellXfsRoot[1012]->applyFill)
        std::cout << "applyFill : " << sr->cellXfsRoot[1012]->applyFill << std::endl;
    if (sr->cellXfsRoot[1012]->applyBorder)
        std::cout << "applyBorder : " << sr->cellXfsRoot[1012]->applyBorder << std::endl;
    if (sr->cellXfsRoot[1012]->applyAlignment)
        std::cout << "applyAlignment : " << sr->cellXfsRoot[1012]->applyAlignment << std::endl;
    if (sr->cellXfsRoot[1012]->Avertical)
        std::cout << "vertical : " << sr->cellXfsRoot[1012]->Avertical << std::endl;
    if (sr->cellXfsRoot[1012]->horizontal)
        std::cout << "horizontal : " << sr->cellXfsRoot[1012]->horizontal << std::endl;
    if (sr->cellXfsRoot[1012]->AwrapText)
        std::cout << "wrapText : " << sr->cellXfsRoot[1012]->AwrapText << std::endl;

    int place = 0;
    //fontid 検索
    while (sr->cellXfsRoot[1012]->fontId[place] != '\0')
        place++;
    UINT32 fontnum = changeStr->RowArraytoNum(sr->cellXfsRoot[1012]->fontId, place);
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

    while (sr->cellXfsRoot[1012]->fillId[place] != '\0')
        place++;
    fontnum = changeStr->RowArraytoNum(sr->cellXfsRoot[1012]->fillId, place);
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
    while (sr->cellXfsRoot[1012]->borderId[place] != '\0')
        place++;
    fontnum = changeStr->RowArraytoNum(sr->cellXfsRoot[1012]->borderId, place);
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


    while (sr->cellXfsRoot[1012]->xfId[place] != '\0')
        place++;
    fontnum = changeStr->RowArraytoNum(sr->cellXfsRoot[1012]->xfId, place);
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


    //xfidでcellstyle検索　sr->cellXfsRoot[1012]->xfId
    for (int nu = 0; nu < sr->cellstyleNum; nu++) {
        res = strcmp((const char*)sr->cellStyleRoot[nu]->xfId, (const char*)sr->cellXfsRoot[1012]->xfId);
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
        */