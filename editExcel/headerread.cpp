#include "Header.h"

HeaderRead::HeaderRead(char* name) {
    readfile = name; //読み込むファイルの指定
    readdata = 0;
    sig = 0;
    endpos = 0;
    curpos = 0;
    headerReaddata = 0;
    lhtest = readLHdata;
    rootLH = nullptr;
    r = nullptr;//セントラルディレクトリのルート
    ER = (ENDrecord*)malloc(sizeof(ENDrecord));
    LH = tallocLH();
    rootLH = LH;
    CD = talloc();
    saveLH = nullptr;
    saveCD = nullptr;
    scd = (CenterDerect*)malloc(sizeof(CenterDerect));
}

HeaderRead::~HeaderRead() {
    //free(root);
    free(r);
    free(ER);
    free(LH);
    free(CD);
    free(cdfn);
    free(scd);
    
}

void HeaderRead::freeER() {
    free(ER->comment);
}

void HeaderRead::freeheader() {
    free(scd->filename);
    free(scd->kakutyo);
    free(scd->comment);
}

void HeaderRead::freeLH() {
    free(LH->filename);
    free(LH->kakutyo);
}

//ファイル名でセントラルディレクトリ　データ検索
//ローカルディレクトリの位置を返す
CenterDerect* HeaderRead::searchCENTRAL(char* s) {
    bool t = false;
    CDdataes* p = saveCD;

    while (p != nullptr) {
        t = searchChara(s, p->cdd.filename, p->cdd.filenameleng);//ファイル名部分一致検索
        if (t) {
            return &p->cdd;
        }
        else {
        }
        p = p->nextCD;
    }
    return nullptr;
}

//ファイル名検索
bool HeaderRead::searchChara(char fn[], char* cdfn, UINT16 cdfnlen)
{
    int i = 0;

    //文字列の数
    while (fn[i] != '\0') {
        i++;
    }
    UINT32 isize = (UINT32)i + 1;
    char* searchfilename = (char*)malloc(isize);//検索スライド用

    int slidepos = 0;

    if (searchfilename) {
        while (slidepos < cdfnlen) {
            if (i > cdfnlen) {//ファイル名が文字列より長い
                return false;
            }
            else {
                if (slidepos == 0) {//
                    for (int j = 0; j < i; j++) {//最初に文字列にfilenameをi分入れる
                        searchfilename[j] = cdfn[slidepos];
                        slidepos++;
                    }
                    searchfilename[i] = '\0';//文字列終端を追加
                }
                else {
                    //配列を一文字づつずらす
                    for (int j = 0; j < i - 1; j++) {
                        searchfilename[j] = searchfilename[j + 1];
                    }
                    searchfilename[i - 1] = cdfn[slidepos];//最後に付け加える
                    searchfilename[i] = '\0';//文字列終端を追加
                    slidepos++;//位置移動
                }
                if (strcmp(searchfilename, fn) == 0)
                {
                    //std::cout << "local filename match" << std::endl;
                    free(searchfilename);
                    return true;
                }
            }
        }
        free(searchfilename);
    }

    return false;
}

CenterDerect* HeaderRead::talloc(void) {
    return (CenterDerect*)malloc(sizeof(CenterDerect));
}

LocalHeader* HeaderRead::tallocLH()
{
    return (LocalHeader*)malloc(sizeof(LocalHeader) * 1);
}

LHDataes* HeaderRead::tallocLhData()
{
    return (LHDataes*)malloc(sizeof(LHDataes));
}


LHDataes* HeaderRead::addLhData(LHDataes* ld, LocalHeader* lhd)
{
    if (!ld) {
        ld = tallocLhData();
        ld->lhdata = lhd;
        ld->nextLH = nullptr;
    }
    else {
        ld->nextLH = addLhData(ld->nextLH, lhd);
    }

    return ld;
}

CDdataes* HeaderRead::tallocCDdataes()
{
    return (CDdataes*)malloc(sizeof(CDdataes));
}

CDdataes* HeaderRead::addCDdata(CDdataes* c, CenterDerect* cddata)
{
    if (c == NULL) {
        c = tallocCDdataes();
        c->cdd = *cddata;
        c->nextCD = nullptr;
    }
    else {
        c->nextCD = addCDdata(c->nextCD, cddata);
    }

    return c;
}

filelist* HeaderRead::addfn(filelist* f, char* fn, int h)
{
    if (!f) {
        f = (filelist*)malloc(sizeof(filelist));
        if (f) {
            UINT32 msize = (UINT32)h + 1;
            f->cfn = (char*)malloc(msize);
            f->cfn = fn;
            f->hash = h;
            f->next = nullptr;
        }
    }
    else {
        f->next = addfn(f, fn, h);
    }
    return f;
}


void HeaderRead::endread(std::ifstream* fin) {
    //std::ifstream fin(readfile, std::ios::in | std::ios::binary);
    if (!fin) {
        std::cout << "ファイル ID.txt が開けません";
    }

    fin->seekg(0, std::ios_base::end);//ファイル終端位置取得
    UINT64 endpos = 0;
    endpos = fin->tellg();

    curpos = endpos;
    curpos -= 1;
    fin->seekg(curpos, std::ios_base::beg);
    //std::cout << "posishion" << curpos << std::endl;

    /*終端コード検索　ファイル終わりから*/
    while (curpos > 0) {

        fin->read((char*)&readdata, sizeof(char));
        sig = sig << 8;
        sig += readdata;
        //std::cout << sig << ',';
        //std::cout << "posishion2:" << curpos << std::endl;

        if (sig == END_OF_CENTRAL) {
            curpos += 4;//シグネチャ分ポインタ戻す
            fin->seekg(curpos);

            //このディスクの数 2byte
            ER->discnum = 0;
            for (int i = 0; i < 2; i++) {
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                ER->discnum += readdata;
            }
            std::cout << "このディスクの数:" << (ER->discnum) << std::endl;

            //セントラルディレクトリが開始するディスク 2byte
            ER->disccentral = 0;
            for (int i = 0; i < 2; i++) {
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                ER->disccentral += readdata;
            }
            std::cout << "開始するディスク" << ER->disccentral << std::endl;

            //このディスク上のセントラルディレクトリレコードの数 2byte
            ER->centralnum = 0;
            for (int i = 0; i < 2; i++) {
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                ER->centralnum += readdata;
            }
            std::cout << "セントラルディレクトリレコードの数" << ER->centralnum << std::endl;

            //セントラルディレクトリレコードの合計数 2byte
            ER->centralsum = 0;
            for (int i = 0; i < 2; i++) {
                readdata = 0;
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                ER->centralsum += readdata;
            }
            std::cout << "セントラルディレクトリレコードの合計数" << ER->centralsum << std::endl;

            //セントラルディレクトリのサイズ (バイト) 4byte
            ER->size = 0;
            for (int i = 0; i < 4; i++) {
                //fin->read((char*)&d, sizeof(char));
                fin->read((char*)&readdata, sizeof(char));
                //std::cout << "before" << readdata << std::endl;
                readdata = (readdata & 0xFF) << (8 * i);
                //std::cout << readdata << std::endl;
                ER->size += readdata;
            }
            std::cout << "セントラルディレクトリのサイズ" << ER->size << std::endl;

            //セントラルディレクトリの開始位置のオフセット 4byte
            ER->position = 0;
            for (int i = 0; i < 4; i++) {
                readdata = 0;
                fin->read((char*)&readdata, sizeof(char));
                readdata = (readdata & 0xFF) << (8 * i);
                ER->position += readdata;
            }
            std::cout << "セントラルディレクトリの開始位置のオフセット:" << std::hex << ER->position << std::endl;
            readpos = ER->position;

            //セントラルディレクトリレコードの合計数 2byte
            ER->commentleng = 0;
            for (int i = 0; i < 2; i++) {
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                ER->commentleng += readdata;
            }
            std::cout << "ZIP file comment length" << std::hex << ER->commentleng << std::endl;

            //std::cout << "ZIPファイルのコメント";
            //ZIPファイルのコメント nbyte
            UINT32 msize = ((UINT32)ER->commentleng) + 1;
            ER->comment = (char*)malloc(msize);
            for (int i = 0; i < ER->commentleng; i++) {
                fin->read((char*)&readdata, sizeof(char));
                ER->comment[i] = (readdata & 0xFF);
                //std::cout << ER->comment[i];
            }
            //std::cout << std::endl;
            //fin->close();
            break;
        }
        curpos--;
        fin->seekg(curpos);
    }

}
//ローカルディレクトリのヘッダー情報
void HeaderRead::localread(UINT64 pos, std::ifstream* fin) {
    //std::ifstream fin(readfile, std::ios::in | std::ios::binary);
    if (!fin) {
        //std::cout << "ファイル ID.txt が開けません";
    }

    fin->seekg(pos, std::ios_base::beg);
    sig = 0;


    /*ローカルヘッダシグネチャを読み込む*/
    while (sig != LOCAL_HEADER) {
        //先頭コードまで読み込む
        fin->read((char*)&readdata, sizeof(char));
        sig = sig << 8;
        sig += readdata & 0xFF;
        //std::cout << sig << ',';
    }

    LH->version = 0;
    for (int i = 0; i < 2; i++) {
        fin->read((char*)&readdata, sizeof(char));
        readdata = ((readdata & 0xFF) << (8 * i));
        LH->version += readdata;
    }
    //std::cout << "展開に必要なバージョン" << LH->version << std::endl;

    //汎用目的のビットフラグ
    LH->bitflag = 0;
    for (int i = 0; i < 2; i++) {
        fin->read((char*)&readdata, sizeof(char));
        readdata = ((readdata & 0xFF) << (8 * i));
        LH->bitflag += readdata;
    }
    //std::cout << "汎用目的のビットフラグ" << LH->version << std::endl;

    //圧縮メソッド
    LH->method = 0;
    for (int i = 0; i < 2; i++) {
        fin->read((char*)&readdata, sizeof(char));
        readdata = ((readdata & 0xFF) << (8 * i));
        LH->method += readdata;
    }
    //std::cout << "圧縮メソッド" << LH->method << std::endl;

    //ファイルの最終変更時間
    LH->time = 0;
    for (int i = 0; i < 2; i++) {
        fin->read((char*)&readdata, sizeof(char));
        readdata = ((readdata & 0xFF) << (8 * i));
        LH->time += readdata;
    }

    //ファイルの最終変更日付
    LH->day = 0;
    for (int i = 0; i < 2; i++) {
        fin->read((char*)&readdata, sizeof(char));
        readdata = ((readdata & 0xFF) << (8 * i));
        LH->day += readdata;
    }
    //std::cout << "ファイルの最終変更日付" << LH->day << std::endl;

    //CRC-32
    LH->crc = 0;
    for (int i = 0; i < 4; i++) {
        fin->read((char*)&readdata, sizeof(char));
        readdata = ((readdata & 0xFF) << (8 * i));
        LH->crc += readdata;
    }
    //std::cout << "ファイルのcrc" << LH->crc << std::endl;

    //圧縮サイズ
    LH->size = 0;
    for (int i = 0; i < 4; i++) {
        fin->read((char*)&readdata, sizeof(char));
        readdata = ((readdata & 0xFF) << (8 * i));
        LH->size += readdata;
    }
    //std::cout << "圧縮サイズ" << LH->size << std::endl;

    //    非圧縮サイズ
    LH->nonsize = 0;
    for (int i = 0; i < 4; i++) {
        fin->read((char*)&readdata, sizeof(char));
        readdata = ((readdata & 0xFF) << (8 * i));
        LH->nonsize += readdata;
    }
    //std::cout << "非圧縮サイズ" << LH->nonsize << std::endl;

    //ファイル名の長さ (n)
    LH->filenameleng = 0;
    for (int i = 0; i < 2; i++) {
        fin->read((char*)&readdata, sizeof(char));
        readdata = ((readdata & 0xFF) << (8 * i));
        LH->filenameleng += readdata;
    }
    //std::cout << "ファイル名の長さ (n)" << LH->filenameleng << std::endl;

    //拡張フィールドの長さ (m)
    LH->fieldleng = 0;
    for (int i = 0; i < 2; i++) {
        fin->read((char*)&readdata, sizeof(char));
        readdata = ((readdata & 0xFF) << (8 * i));
        LH->fieldleng += readdata;
    }
    //std::cout << "拡張フィールドの長さ (m)" << LH->fieldleng << std::endl;

    //ファイル名
    UINT32 msize = ((UINT32)LH->filenameleng) + 1;
    LH->filename = (char*)malloc(msize);
    if (LH->filename) {
        for (UINT16 i = 0; i < LH->filenameleng; i++) {
            fin->read((char*)&readdata, sizeof(char));
            LH->filename[i] = readdata;
        }
        LH->filename[LH->filenameleng] = '\0';
    }

    //拡張フィールド
    msize = ((UINT32)LH->fieldleng) + 1;
    LH->kakutyo = (char*)malloc(msize);
    if (LH->fieldleng > 0) {
        for (UINT16 i = 0; i < LH->fieldleng; i++) {
            fin->read((char*)&readdata, sizeof(char));
            LH->kakutyo[i] = readdata;
        }
    }
    else {
        LH->kakutyo = nullptr;
    }

    LH->pos = fin->tellg();//圧縮データの始まり
}
//セントラルディレクトリのヘッダー情報
void HeaderRead::centerread(UINT64 pos, UINT32 size, UINT16 n, std::ifstream* fin) {
    //std::cout << std::endl;

    //std::ifstream fin(readfile, std::ios::in | std::ios::binary);

    if (!fin) {
        std::cout << "not file open" << std::endl;
    }

    int j = 0;

    fin->seekg(pos, std::ios_base::beg);

    while (j < n) {

        //while (s < size) {
            /*先頭コードまで読み込む*/
        for (int i = 0; i < 4; i++) {
            fin->read((char*)&readdata, sizeof(char));
            sig = sig << 8;
            sig += readdata & 0xFF;
            //std::cout << std::hex << sig << ',';
        }

        if (sig == CENTRALSIGNATURE) { //中央ディレクトリファイルヘッダーの署名= 0x02014b50
            //std::cout << "中央ディレクトリ出力　j:" << j << std::endl;
            //CD->version = 0;
            //mac 62づつ増える？　windows 45づつ増える？
            //属性　GetFileAttributes?
            CD->version = 0;
            for (int i = 0; i < 2; i++) {
                fin->read((char*)&readdata, sizeof(char));//作成者
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->version += readdata;
            }
            std::cout << "作成バージョン：" << CD->version << std::endl;
            //抽出に必要なバージョン（最小）
            CD->minversion = 0;
            for (int i = 0; i < 2; i++) {
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->minversion += readdata;
            }
            //std::cout << "抽出に必要なバージョン（最小：" << CD->minversion << std::endl;
            CD->bitflag = 0;
            for (int i = 0; i < 2; i++) {
                fin->read((char*)&readdata, sizeof(char));//汎用ビットフラグ
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->bitflag += readdata;
            }
            //std::cout << "汎用ビットフラグ：" << CD->bitflag << std::endl;
            CD->method = 0;
            for (int i = 0; i < 2; i++) {//    圧縮方法
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->method += readdata;
            }
            //std::cout << "圧縮方法：" << CD->method << std::endl;
            CD->time = 0;
            for (int i = 0; i < 2; i++) {//    ファイルの最終変更時刻
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->time += readdata;
            }
            //std::cout << "ファイルの最終変更時刻：" << CD->time << std::endl;
            CD->day = 0;
            for (int i = 0; i < 2; i++) {//    ファイルの最終変更日
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->day += readdata;
            }
            //std::cout << "ファイルの最終変更日：" << CD->day << std::endl;
            //crc
            CD->crc = 0;
            for (int i = 0; i < 4; i++) {
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->crc += readdata;
            }
            //std::cout << "CRC-32：" << CD->crc << std::endl;
            //    圧縮サイズ
            CD->size = 0;
            for (int i = 0; i < 4; i++) {
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->size += readdata;
            }
            //std::cout << "圧縮サイズ：" << CD->size << std::endl;
            //    非圧縮サイズ
            CD->nonsize = 0;
            for (int i = 0; i < 4; i++) {
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->nonsize += readdata;
            }
            //std::cout << "非圧縮サイズ：" << CD->nonsize << std::endl;
            //    ファイル名の長さ（n)
            CD->filenameleng = 0;
            for (int i = 0; i < 2; i++) {
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->filenameleng += readdata;
            }

            CD->fieldleng = 0;
            for (int i = 0; i < 2; i++) {//    追加フィールド長（m）
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->fieldleng += readdata;
            }

            CD->fielcomment = 0;
            for (int i = 0; i < 2; i++) {//    ファイルのコメント長（k）
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->fielcomment += readdata;
            }

            CD->discnum = 0;
            for (int i = 0; i < 2; i++) {//    ファイルが始まるディスク番号
                fin->read((char*)&readdata, sizeof(char));
                readdata = (readdata << (8 * i));
                CD->discnum += readdata;
            }
            //std::cout << "ファイルが始まるディスク番号：" << CD->discnum << std::endl;
            CD->zokusei = 0;
            for (int i = 0; i < 2; i++) {//        内部ファイル属性
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->zokusei += readdata;
            }
            //std::cout << "内部ファイル属性：" << CD->zokusei << std::endl;
            CD->gaibuzokusei = 0;
            for (int i = 0; i < 4; i++) {//    外部ファイル属性
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->gaibuzokusei += readdata;
            }
            //std::cout << "外部ファイル属性：" << CD->gaibuzokusei << std::endl;
            CD->localheader = 0;
            for (int i = 0; i < 4; i++) {//    ローカルファイルヘッダの相対オフセット
                fin->read((char*)&readdata, sizeof(char));
                readdata = ((readdata & 0xFF) << (8 * i));
                CD->localheader += readdata;
            }
            //std::cout << "ローカルファイルヘッダの相対オフセット：" << CD->localheader << std::endl;

            //std::cout << "ファイル名：";
            UINT32 msize = ((UINT32)CD->filenameleng) + 1;
            CD->filename = (char*)malloc(msize);
            for (int i = 0; i < CD->filenameleng; i++) {//    ファイル名
                fin->read((char*)&headerReaddata, sizeof(char));
                CD->filename[i] = headerReaddata;
                //std::cout << CD->filename[i];
            }
            CD->filename[CD->filenameleng] = '\0';
            //std::cout << std::endl;

            //std::cout << "拡張フィールド：";
            msize = ((UINT32)CD->fieldleng) + 1;
            if (CD->fieldleng > 0) {
                CD->kakutyo = (char*)malloc(msize);
                for (int i = 0; i < CD->fieldleng; i++) {//    拡張フィールド
                    fin->read((char*)&readdata, sizeof(char));
                    CD->kakutyo[i] = readdata & 0xFF;
                    //std::cout << CD->kakutyo[i];
                }
            }
            //std::cout << std::endl;

            //std::cout << "ファイルコメント：";
            msize = ((UINT32)CD->fielcomment) + 1;
            if (CD->fielcomment > 0) {
                CD->comment = (char*)malloc(msize);
                for (int i = 0; i < CD->fielcomment; i++) {//ファイルコメント
                    fin->read((char*)&readdata, sizeof(char));
                    CD->comment[i] = readdata & 0xFF;
                    //std::cout << CD->comment[i];
                }
            }

            sig = 0;
            //データを保持する
            cdfn = addfn(cdfn, CD->filename, CD->filenameleng);
        }
        j++;
    }
    //fin->close();
}

CenterDerect* HeaderRead::centeroneread(UINT64 pos, UINT32 size, UINT16 n, char* fn, std::ifstream* fin)
{
    //std::ifstream fin(readfile, std::ios::in | std::ios::binary);

    bool flag = false;

    if (!fin) {
        std::cout << "not file open" << std::endl;
    }

    fin->seekg(pos, std::ios_base::beg);

    /*先頭コードまで読み込む*/
    for (int i = 0; i < 4; i++) {
        fin->read((char*)&readdata, sizeof(char));
        sig = sig << 8;
        sig += readdata & 0xFF;
    }

    if (sig == CENTRALSIGNATURE) {
        scd->version = 0;
        for (int i = 0; i < 2; i++) {
            fin->read((char*)&readdata, sizeof(char));//作成者
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->version += readdata;
        }
        //std::cout << "作成バージョン：" << scd->version << std::endl;
        scd->minversion = 0;
        for (int i = 0; i < 2; i++) {
            fin->read((char*)&readdata, sizeof(char));
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->minversion += readdata;
        }
        //std::cout << "抽出に必要なバージョン（最小：" << scd->minversion << std::endl;
        scd->bitflag = 0;
        for (int i = 0; i < 2; i++) {
            fin->read((char*)&readdata, sizeof(char));//汎用ビットフラグ
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->bitflag += readdata;
        }
        //std::cout << "汎用ビットフラグ：" << scd->bitflag << std::endl;
        scd->method = 0;
        for (int i = 0; i < 2; i++) {//    圧縮方法
            fin->read((char*)&readdata, sizeof(char));
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->method += readdata;
        }
        //std::cout << "圧縮方法：" << scd->method << std::endl;
        scd->time = 0;
        for (int i = 0; i < 2; i++) {//    ファイルの最終変更時刻
            fin->read((char*)&readdata, sizeof(char));
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->time += readdata;
        }
        //std::cout << "ファイルの最終変更時刻：" << scd->time << std::endl;
        scd->day = 0;
        for (int i = 0; i < 2; i++) {//    ファイルの最終変更日
            fin->read((char*)&readdata, sizeof(char));
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->day += readdata;
        }
        //std::cout << "ファイルの最終変更日：" << scd->day << std::endl;
        //crc
        scd->crc = 0;
        for (int i = 0; i < 4; i++) {
            fin->read((char*)&readdata, sizeof(char));
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->crc += readdata;
        }
        //std::cout << "CRC-32：" << scd->crc << std::endl;
        //    圧縮サイズ
        scd->size = 0;
        for (int i = 0; i < 4; i++) {
            fin->read((char*)&readdata, sizeof(char));
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->size += readdata;
        }
        //std::cout << "圧縮サイズ：" << scd->size << std::endl;
        //    非圧縮サイズ
        scd->nonsize = 0;
        for (int i = 0; i < 4; i++) {
            fin->read((char*)&readdata, sizeof(char));
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->nonsize += readdata;
        }
        //std::cout << "非圧縮サイズ：" << scd->nonsize << std::endl;
        //    ファイル名の長さ（n)
        scd->filenameleng = 0;
        for (int i = 0; i < 2; i++) {
            fin->read((char*)&readdata, sizeof(char));
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->filenameleng += readdata;
        }

        scd->fieldleng = 0;
        for (int i = 0; i < 2; i++) {//    追加フィールド長（m）
            fin->read((char*)&readdata, sizeof(char));
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->fieldleng += readdata;
        }

        scd->fielcomment = 0;
        for (int i = 0; i < 2; i++) {//ファイルのコメント長（k）
            fin->read((char*)&readdata, sizeof(char));
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->fielcomment += readdata;
        }

        scd->discnum = 0;
        for (int i = 0; i < 2; i++) {//ファイルが始まるディスク番号
            fin->read((char*)&readdata, sizeof(char));
            readdata = (readdata << (8 * i));
            scd->discnum += readdata;
        }

        scd->zokusei = 0;
        for (int i = 0; i < 2; i++) {//内部ファイル属性
            fin->read((char*)&readdata, sizeof(char));
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->zokusei += readdata;
        }
        //std::cout << "内部属性：" << scd->zokusei << std::endl;

        scd->gaibuzokusei = 0;
        for (int i = 0; i < 4; i++) {//    外部ファイル属性
            fin->read((char*)&readdata, sizeof(char));
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->gaibuzokusei += readdata;
        }
        //std::cout << "外部属性：" << scd->gaibuzokusei << std::endl;

        scd->localheader = 0;
        for (int i = 0; i < 4; i++) {//    ローカルファイルヘッダの相対オフセット
            fin->read((char*)&readdata, sizeof(char));
            readdata = ((readdata & 0xFF) << (8 * i));
            scd->localheader += readdata;
        }

        UINT32 msize = ((UINT32)scd->filenameleng) + 1;
        scd->filename = (char*)malloc(msize);
        for (int i = 0; i < scd->filenameleng; i++) {//    ファイル名
            fin->read((char*)&headerReaddata, sizeof(char));
            scd->filename[i] = headerReaddata;
        }
        scd->filename[scd->filenameleng] = '\0';
        flag = searchChara(fn, scd->filename, scd->filenameleng);

        if (scd->fieldleng > 0) {
            msize = ((UINT32)scd->fieldleng) + 1;
            scd->kakutyo = (char*)malloc(msize);
            for (int i = 0; i < scd->fieldleng; i++) {//    拡張フィールド
                fin->read((char*)&readdata, sizeof(char));
                scd->kakutyo[i] = readdata & 0xFF;
            }
        }
        else {
            scd->kakutyo = nullptr;
        }
        
        if (scd->fielcomment>0) {
            msize = ((UINT32)scd->fielcomment) + 1;
            scd->comment = (char*)malloc(msize);
            for (int i = 0; i < scd->fielcomment; i++) {//ファイルコメント
                fin->read((char*)&readdata, sizeof(char));
                scd->comment[i] = readdata & 0xFF;
            }
        }
        else {
            scd->comment = nullptr;
        }
        
        sig = 0;

        readpos = fin->tellg();
    }
    filenum++;

    //fin->close();

    if (flag)
        return scd;

    return nullptr;
}

inputtxt* HeaderRead::addtxt(inputtxt* intx, char* tx, inputtxt* par) {

    if (!intx) {
        intx = (inputtxt*)malloc(sizeof(inputtxt));
        intx->parrent = par;
        intx->txt = (UINT8*)tx;
        intx->sinum = (UINT8*)malloc(1);
        intx->sinum = nullptr;
        intx->next = nullptr;
    }
    else {
        intx->next = addtxt(intx->next, tx, intx);
    }
        

    return intx;
}

void HeaderRead::freetxt(inputtxt* p) {
    inputtxt* q;
    while (p) {
        q = p->next;
        free(p);
        p = q;
    }
}

inputtxt* HeaderRead::slipInputText(char* ins,inputtxt* it) {
    size_t i = 0;

    while (ins[i] != '\0') {
        if (ins[i] == ',')//改行数 コメント数　改行で行分ける
            intxtCount++;
        else
            std::cout << ins[i] << ",";
        i++;
    }

    intxtCount++;//最終行プラス

    char** instrs = (char**)malloc(intxtCount);
    //char instrs[5][255] = { 0 };

    int j = 0;
    i = 0;
    int stockpos = 0;
    while (j < intxtCount) {
        while (ins[i] != ',' && ins[i] != '\0')
            i++;
        
        size_t strleng = i - stockpos;
        size_t msize = strleng + 1;

        char *inst = (char*)malloc(msize);
        std::cout << "文字数：" << i << "," << msize << std::endl;
        if (!inst)
            return nullptr;

        for (int t = 0; t < strleng; t++)
            inst[t] = ins[stockpos + t];
        inst[strleng] = '\0';
        it = addtxt(it, inst,nullptr);
        //instrs[j] = inst;
        //std::cout << "入力テキスト：" << instrs[j] << " " << strleng << std::endl;
        i++;//改行　スキップ
        stockpos = i;//スタート位置更新

        j++;
    }

    return it;
}

