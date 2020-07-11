#include "ZipFilewrite.h"

zipwrite::zipwrite(unsigned int times)
{
    readdata = 0;
    headreaddata = 0;
    writeposition = 0;
    time = times;
    compressleng = 0;
    //outfile = nullptr;
    filenameleng = 0;
}

bool zipwrite::searchfile(char fn[], char* cdfn, unsigned short cdfnlen)
{
    int i = 0;

    //文字列の数
    while (fn[i] != '\0') {
        i++;
    }
    char* searchfilename = (char*)malloc(50);//検索スライド用
    
    int slidepos = 0;
    int result = 0;

    if (searchfilename) {
        while (slidepos < cdfnlen) {
            if (i > cdfnlen) {//ファイル名が文字列より短い
                return false;
            }
            else {
                if (slidepos == 0) {//
                    for (int j = 0; j < i; j++) {//最初に文字列にfilenameをi分入れる
                        searchfilename[j] = cdfn[slidepos];
                        slidepos++;
                    }
                    if (searchfilename)
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
                result = strncmp(searchfilename, fn, i);
                if (result == 0) {
                    return true;
                }
            }
        }
    }
    
    //std::cout << "filename not match" << std::endl;

    return false;
}

void zipwrite::LoclheadAndDatacopy(unsigned long long position, FILE* wf, char* rfn) {

    std::ifstream infile(rfn, std::ios::in | std::ios::binary);

    if (!infile) {
        std::cout << "ファイル ID.txt が開けません";
    }

    infile.seekg(position, std::ios_base::beg);
    compressleng = 0;
    filenameleng = 0;
    headreaddata = 0;

    // バイナリとして書き込む
    //ローカルシグネチャの書き込み
    for (int i = 0; i < 4; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        writeposition++;
    }

    //バージョンの書き込み
    for (int i = 0; i < 2; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        writeposition++;
    }

    //ビットフラッグ
    for (int i = 0; i < 2; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        writeposition++;
    }
    unsigned short comtype = 0;
    //圧縮方式の書き込み
    for (int i = 0; i < 2; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        headreaddata = readdata & 0xFF;
        headreaddata = ((headreaddata & 0xFF) << (8 * i));
        comtype += headreaddata;
        writeposition++;
    }
    headreaddata = 0;
    //std::cout << "圧縮タイプ　書き込み : " << comtype-'\0' << std::endl;

    //時間と日付の書き込み 更新
    for (int i = 0; i < 4; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        writeposition++;
    }

    //crc 4byte 書き込み（解凍後の値をcrc）
    for (int i = 0; i < 4; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        writeposition++;
    }

    //圧縮サイズ 書き込み
    headreaddata = 0;
    compressleng = 0;
    for (int i = 0; i < 4; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        headreaddata = ((readdata & 0xFF) << (8 * i));
        compressleng += headreaddata;
        writeposition++;
    }
    //std::cout << "書き込み圧縮サイズ : " << compressleng << std::endl;
    headreaddata = 0;

    //圧縮前サイズ 書き込み
    unsigned int nocom = 0;
    for (int i = 0; i < 4; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        writeposition++;
        headreaddata = readdata & 0xFF;
        headreaddata = ((headreaddata & 0xFF) >> (8 * i));
        nocom += headreaddata;
    }
    headreaddata = 0;
    //std::cout << "非圧縮サイズ : " << nocom << std::endl;

    //ファイル名の長さ 書き込み
    for (int i = 0; i < 2; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        headreaddata = readdata & 0xFF;
        headreaddata = ((headreaddata & 0xFF) << (8 * i));
        filenameleng += headreaddata;
        writeposition++;
    }
    headreaddata = 0;

    unsigned int kakutyo = 0;
    unsigned int fieldlen = 0;
    //拡張フィールドの長さ 0書き込み
    for (int i = 0; i < 2; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        kakutyo = readdata & 0xFF;
        kakutyo = ((kakutyo & 0xFF) << (8 * i));
        fieldlen += kakutyo;
        writeposition++;
    }

    if (filenameleng > 0) {
        //ファイル名の書き込み
    //std::cout << "ファイル名: ";
        for (int i = 0; i < filenameleng; i++) {
            infile.read((char*)&readdata, sizeof(char));
            fwrite(&readdata, sizeof(char), 1, wf);
            //std::cout << readdata;
            writeposition++;
        }
        //std::cout << std::endl;
    }
    
    if (fieldlen > 0) {
        //拡張フィールドの 書き込み
    //std::cout << "拡張フィールド";
        for (UINT32 i = 0; i < fieldlen; i++) {
            infile.read((char*)&readdata, sizeof(char));
            fwrite(&readdata, sizeof(char), 1, wf);
            //std::cout << readdata;
            writeposition++;
        }
    }
    
    // datawrite
    for (unsigned int i = 0; i < compressleng; i++) {
            infile.read((char*)&readdata, sizeof(char));
            fwrite(&readdata, sizeof(char), 1, wf);
            writeposition++;
    }

}
