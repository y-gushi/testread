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


int main(char* fname[], int i) {

    char infile[] = "C:/Users/Bee/Desktop/【2020年6月6日更新】2020-発注到着【システムで入力】.xlsx"; //読み込むファイルの指定 【2020年1月11日更新】2020-発注到着
    char infile2[] = "C:/Users/Bee/Desktop/LY200605-A BEEだけ.xlsx"; //読み込むファイルの指定 /LY20191127-SHIP BEE だけ.xlsx
    char mfile[] = "C:/Users/Bee/Desktop/test[システム].xlsx"; //テスト書き出し
    char recd[] = "C:/Users/Bee/Desktop/Centraldata"; //テスト書き出し

    HeaderRead* hr=new HeaderRead(infile2);
    CenterDerect* cddata = nullptr;//セントラルディレクトのデータ
    hr->endread();//終端コードの読み込み

    /*-----------------------
     シェアー文字列読み込み
     -----------------------*/

    char shares[] = "sharedStrings.xml";//ファイル位置　ファイル名部分一致検索　sharedStrings.xml
    bool fla = false;
    DeflateDecode* decShare = new DeflateDecode;//sharestring ファイルの保存用

    //cddata = hr->searchCENTRAL(shares);//セントラルディレクトのデータ
    
    while (hr->filenum < hr->ER->centralsum) {
        cddata = hr->centeroneread(hr->readpos, hr->ER->size, hr->ER->centralsum, shares);
        if (cddata) {
            break;
        }
    }

    if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
        hr->localread(cddata->localheader);//sharesstringsの読み込み
        decShare->dataread(infile2, hr->LH->pos, cddata->nonsize);
    }

    shareRandD sharray(decShare->ReadV, decShare->readlen);//share string read to array
    sharray.getSicount();//get si count
    sharray.ReadShare();//文字列読み込み
    

    /*-----------------------
    パッキングリストシート読み込み
    -----------------------*/
    
    char sheetname[] = "worksheets/sheet1.xml";//ファイル位置　ファイル名部分一致検索
    DeflateDecode* decsheet = new DeflateDecode;

    hr->filenum = 0;//レコード数初期化
    hr->readpos = hr->ER->position;
    cddata = nullptr;
    while (hr->filenum < hr->ER->centralsum) {
        cddata = hr->centeroneread(hr->readpos, hr->ER->size, hr->ER->centralsum, sheetname);//セントラルディレクトのデータ
        if (cddata) {
            break;
        }
    }

    if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
        hr->localread(cddata->localheader);//sharesstringsの読み込み
        decsheet->dataread(infile2, hr->LH->pos, cddata->nonsize);
    }

    Ctags ms(decsheet->ReadV, decsheet->readlen, &sharray);
    ms.sheetread();
    delete decsheet;//デコードデータ削除
    shipinfo sg(ms.rows);
    sg.GetItems();
    delete ms.rows;//cell 消去
    delete hr;
    
    /*-----------------------
    shareシート読み込み
    -----------------------*/
    
    HeaderRead* hr2=new HeaderRead(infile);//ファイル変更
    hr2->endread();//終端コードの読み込み
    //hr2->centerread(hr2->ER->position, hr2->ER->size, hr2->ER->centralnum);//セントラルディレクトリの読み込み
    
    decShare = new DeflateDecode;

    while (hr2->filenum < hr2->ER->centralsum) {
        cddata = hr2->centeroneread(hr2->readpos, hr2->ER->size, hr2->ER->centralnum, shares);
        if (cddata)
            break;
    }
    
    if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
        hr2->localread(cddata->localheader);//sharesstringsの読み込み
        decShare->dataread(infile, hr2->LH->pos, cddata->nonsize);
    }
    sharray = shareRandD(decShare->ReadV, decShare->readlen);//share 再初期化
    
    sharray.getSicount();//get si count
    
    //if (sg.day)
    //    std::cout << "day str : " << sg.day << std::endl;
    decShare->ReadV = sharray.writeshare(sg.day, sg.daylen);//share文字列書き込み share data更新
    

    sharray.ReadShare();//文字列読み込み
    
    /*--------------------------
    share data書き込み　圧縮
    ---------------------------*/
    
    //ファイル書き出し用
    _Post_ _Notnull_ FILE* cdf;
    fopen_s(&cdf, recd, "wb");
    if (!cdf)
        printf("ファイルを開けませんでした");
    
    _Post_ _Notnull_ FILE* fpr;
    fopen_s(&fpr, mfile, "wb");
    if (!fpr)
        printf("ファイルを開けませんでした");


    LHmake dd;
    dd.getday();//時間の取得
    zipwrite zip(dd.times);//zipデータを作る
    HeaderWrite hw;//ローカルヘッダの書き込み
    //changefilenames* stockfn = (changefilenames*)malloc(sizeof(changefilenames));//圧縮ファイル名ストック
    //stockfn = nullptr;
    UINT32 localSize = 0;
    UINT32 CDp = 0;

    
    encoding* shenc = new encoding;//sharestring 圧縮
    shenc->compress(decShare->ReadV, sharray.writeleng);
    delete decShare;//シェアー解凍データ削除

    if (cddata) {
        localSize = hw.localwrite(fpr, dd.times, sharray.buckcrc, shenc->datalen, sharray.writeleng, cddata->filenameleng, cddata->filename, cddata->version, cddata->bitflag, cddata->method);
        CDp = (shenc->datalen) + localSize;//ローカルデータサイズ

        //セントラル情報の書き換え//
        cddata->crc = sharray.Crc.crc32;
        cddata->localheader = zip.writeposition;
        cddata->day = (dd.times) & 0xFFFF;
        cddata->time = (dd.times >> 16) & 0xFFFF;
        cddata->size = shenc->datalen;
        cddata->nonsize = sharray.writeleng;//内容変更したら　更新必要

        hw.centralwrite(cdf, *cddata);
    }   
    
    zip.writeposition += CDp;//データ書き込み位置更新

    shenc->write(fpr);//圧縮データの書き込み
    free(shenc->rvp);//share デコードデータ削除
    delete shenc;//share圧縮データ削除
    
    //if (cddata) {
    //    stockfn = hw.addfilename(stockfn, cddata->filename);//変更データのファイル名ストック
    //}
    
     /*-----------------------
    発注到着シート読み込み
    -----------------------*/
    
    DeflateDecode* Hdeco;
    char sheet[] = "worksheets/sheet362";
    const char sharefn[] = "xl/sharedStrings.xml";
    bool t = false;
    Ctags* mh;//発注到着　cell データ読み込み
    searchItemNum* sI;//品番検索　＆　書き込み
    CDdataes* slideCDdata = hr2->saveCD;//ファイル名検索用
    
    hr2->readpos = hr2->ER->position;//読み込み位置初期化
    hr2->filenum = 0;//レコード数初期化
    int result = 0;

    //品番、カラーエラー用
    MatchColrs* matchs = (MatchColrs*)malloc(sizeof(MatchColrs));
    matchs = nullptr;
    MatchColrs* matchsroot = (MatchColrs*)malloc(sizeof(MatchColrs));
    matchsroot = nullptr;

    while (hr2->filenum < hr2->ER->centralsum) {
        //ファイル名 sheet 部分一致検索
        cddata = hr2->centeroneread(hr2->readpos, hr2->ER->size, hr2->ER->centralnum, sheet);
        if (cddata) {
            std::cout << "sheet一致ファイルネーム：" << cddata->filename << std::endl;
            hr2->localread(cddata->localheader);//"worksheets/sheet"に一致するファイルの中身検索

            Hdeco = new DeflateDecode;//解凍
            Hdeco->dataread(infile, hr2->LH->pos, cddata->nonsize);//解凍　データ読み込み

            mh = new Ctags(Hdeco->ReadV, Hdeco->readlen, &sharray);//シートデータ読み込み
            mh->sheetread();
            free(Hdeco->ReadV);
            delete Hdeco;//デコードデータ　削除
            sI = new searchItemNum(sg.its, mh);
            //std::cout << "sharray.uniqstr : " << sharray.uniqstr << std::endl;
            t = sI->searchitemNumber(sharray.uniqstr);//品番検索　＆　セルデータ追加　シェアー消去（入れる場合は引数に）

            if (t)
            {//一致品番あった場合
                if (sI->rootMat) {//一致アイテムの保存
                    while (sI->rootMat) {
                        matchs = sI->addmatches(matchs, sI->rootMat->itemnum, sI->rootMat->color);
                        sI->rootMat = sI->rootMat->next;
                    }
                }

                mh->writesheetdata();//シートデータ書き込み
                crc CRC;
                CRC.rescrc();
                CRC.mcrc(mh->data, mh->p);//crc 計算
                encoding* enc = new encoding;//圧縮
                enc->compress(mh->data, mh->p);//データ圧縮

                //if (mh->data) { free(mh->data); }//エラー

                cddata->zokusei = 0x00;
                cddata->gaibuzokusei = 0x00;
                cddata->bitflag = 0x06;
                localSize = hw.localwrite(fpr, dd.times, CRC.crc32, enc->datalen & 0xFFFFFFFF, mh->p & 0xFFFFFFFF, cddata->filenameleng, cddata->filename,cddata->version, cddata->bitflag, cddata->method);//ローカルヘッダー書き込み
                CDp = (enc->datalen) + localSize;//ローカルデータサイズ
                cddata->crc = CRC.crc32;//セントラル情報の書き換え
                cddata->localheader = zip.writeposition;
                cddata->time = (dd.times) & 0xFFFF;
                cddata->day = (dd.times >> 16) & 0xFFFF; 
                cddata->size = enc->datalen;
                cddata->nonsize = mh->p & 0xFFFFFFFF;//内容変更したら　更新必要
                //cddata->bitflag = 0x07;//0B00000111 超高速 高速 cddata->bitflag
                zip.writeposition += CDp;//データ書き込み位置更新
                enc->write(fpr);//圧縮データの書き込み
                free(enc->rvp);//エンコードデータメモリ開放
                //cddata一旦書き込み
                hw.centralwrite(cdf, *cddata);
                
                delete enc;
            }
            else {
                //cddata一旦書き込み
                UINT32 LHposstock = zip.writeposition;//ローカルヘッダーの位置更新用
                zip.LoclheadAndDatacopy(cddata->localheader, fpr, infile);//ローカルヘッダー検索＆書き込み
                cddata->localheader = LHposstock;//ローカルヘッダー相対位置のみ変更
                hw.centralwrite(cdf, *cddata);
            }
            delete mh;
        }
        else {
            result = strcmp(hr2->scd->filename, sharefn);
            if (result != 0) {
                //cddata一旦書き込み
                UINT32 LHposstock = zip.writeposition;//ローカルヘッダーの位置更新用
                zip.LoclheadAndDatacopy(hr2->scd->localheader, fpr, infile);//ローカルヘッダー検索＆書き込み
                hr2->scd->localheader = LHposstock;//ローカルヘッダー相対位置のみ変更
                hw.centralwrite(cdf, *hr2->scd);
            }
        }
    }
    free(cddata);

    int matchor = 1;
    int matchcol = 1;

    Items* errorItem = (Items*)malloc(sizeof(Items));
    errorItem = nullptr;

    sI = new searchItemNum(nullptr, nullptr);

    matchsroot = matchs;
    unitC un;

    //シートがない品番、カラーを比較
    if (matchs) {
        while (sg.its) {
            sg.its->col = un.slipNum(sg.its->col);
            while (matchs) {
                matchor = strcmp((const char*)sg.its->itn, (const char*)matchs->itemnum);
                if (matchor == 0) {//品番、いろ一致入力
                    matchcol = strcmp((const char*)sg.its->col, (const char*)matchs->color);
                    if (matchcol == 0) {
                        //std::cout << "保存　一致データあり" << std::endl;
                        break;
                    }
                }
                else {
                    matchcol = 1;
                }
                matchs = matchs->next;
            }
            if (matchor == 0 && matchcol == 0) {//品番、いろ一致入力
            }
            else {
                //エラー　シートなし
                errorItem = sI->addItems(errorItem, sg.its);
                char* shiftj = sI->CharChenge(sg.its->col);
                std::cout << "シートなし" << sg.its->itn << std::endl;
            }
            matchs = matchsroot;//初期化
            sg.its = sg.its->next;
        }
    }else{
        std::cout << "sheet no error" << std::endl;
    }
    free(errorItem);
    free(matchsroot);

    hw.eocdwrite(cdf, hr2->ER->discnum, hr2->ER->disccentral, hr2->ER->centralnum, hr2->ER->centralsum, zip.writeposition, hw.cdsize);
    
    if (cdf)
        fclose(cdf);

    std::ifstream fin(recd, std::ios::in | std::ios::binary);
    if (!fin) {
        std::cout << "not file open" << std::endl;
    }
    //std::cout << hw.cdsize << " zip " <<zip.writeposition << std::endl;
    char rd = 0;

    if (fpr) {
        while (!fin.eof()) {
            fin.read((char*)&rd, sizeof(char));
            if(!fin.eof())
                fwrite(&rd, sizeof(char), 1, fpr);
        }
    }

    fin.close();

    remove(recd);

    if (fpr)
        fclose(fpr);
        
    /*for (int i=0;i<ms.p;i++) {
        fwrite(&ms.data[i], sizeof(char), 1, fpr);
    }*/

    return 0;
}