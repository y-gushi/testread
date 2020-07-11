#include "shareRandW.h"
#include"SearchItemNUm.h"

shareRandD::shareRandD(UINT8* d, UINT64 l) {
    sd = d;
    sdlen = l;
}

shareRandD::~shareRandD() {
    for (int i = 0; i < mycount; i++)//シェアー文字列　メモリ解放
        Sitablefree(sis[i]);
    //free(writedata);
}

void shareRandD::getSicount() {
    UINT8 count[] = "count=\"";//rの数
    UINT8 uniquecount[] = "uniqueCount=\"";//siの数

    unsigned char searchcount[8] = { 0 };//検索スライド用 7文字
    unsigned char searchunique[14] = { 0 };

    UINT8* getcount = nullptr;
    UINT8* getunique = nullptr;

    int datapos = 0;

    if (searchcount) {
        while (datapos < sdlen)
        {
            for (int i = 0; i < 6; i++) {
                searchcount[i] = searchcount[i + 1];
            }
            searchcount[6] = sd[datapos];
            searchcount[7] = '\0';
            datapos++;
            int result = strcmp((const char*)searchcount, (const char*)count);
            if (result == 0)
            {
                siunique_place = 0;
                while (sd[datapos + sicount_place] != '"') {
                    sicount_place++;
                }
                UINT32 msize = (UINT32)sicount_place + 1;
                getcount = (unsigned char*)malloc(msize);
                if (getcount) {
                    for (int i = 0; i < sicount_place; i++) {
                        getcount[i] = sd[datapos];
                        datapos++;
                    }
                }
                break;
            }
        }
    }

    if (searchunique) {
        while (datapos < sdlen)
        {
            for (int i = 0; i < 12; i++) {
                searchunique[i] = searchunique[i + 1];
            }
            searchunique[12] = sd[datapos];
            searchunique[13] = '\0';
            datapos++;
            if (strcmp((const char*)searchunique, (const char*)uniquecount) == 0)
            {
                siunique_place = 0;
                while (sd[datapos+ siunique_place] != '"') {
                    siunique_place++;
                }
                UINT32 msize = (UINT32)siunique_place + 1;
                getunique = (unsigned char*)malloc(msize);
                if (getunique) {
                    for (int i = 0; i < siunique_place; i++) {
                        getunique[i] = sd[datapos];
                        datapos++;
                    }
                }
                break;
            }
        }
    }

    if (getcount) {
        for (int i = 0; i < sicount_place; i++) {//share count数　数字に変換
            double po = pow(10, siunique_place - i - 1);
            UINT32 powsize = UINT32(po);
            sicount += (getcount[i] - 48) * powsize;
        }
        free(getcount);
    }

    if (getunique) {
        for (int i = 0; i < siunique_place; i++) {//share unique数　数字に変換
            double po= pow(10, siunique_place - i - 1);
            UINT32 powsize = UINT32(po);
            siunique += (getunique[i] - 48) * powsize;
        }
        std::cout << "get si unique " << siunique << std::endl;
        free(getunique);
    }
}
/*<si><t>5/21着</t><rPh sb="4" eb="5"><t>チャク</t></rPh><phoneticPr fontId="1"/></si>*/
UINT8* shareRandD::writeshare(UINT8* instr, int instrlen,inputtxt *substr, int INstrCount) {
    UINT8 sitag[] = "<si>";
    UINT8 searchsi[5] = { 0 };
    UINT8 siend[] = "</si>";
    UINT8 siendslide[6] = { 0 };

    UINT32 datalen = UINT32(sdlen) + 2000;//データ長

    UINT8* writedata = (unsigned char*)malloc(datalen);

    UINT8 count[] = "count=\"";
    UINT8 uniquecount[] = "uniqueCount=\"";

    unsigned char searchcount[8] = { 0 };//検索スライド用 7文字
    unsigned char searchunique[14] = { 0 };

    int sislide = 0;
    size_t datapos = 0;

    int stocksic = siunique;

    inputtxt* subm = substr;

    while (subm) {
        if (!subm->sinum) {
            sicount++;//1タグ分増やす
            siunique++;
        }
        subm = subm->next;
    }

    countstr = st.InttoChar(sicount, &sicount_place);
    uniqstr = st.InttoChar(siunique, &siunique_place);

    //count write and copy
    int coucount = 1;
    while (coucount != 0)
    {
        for (int i = 0; i < 6; i++) {
            searchcount[i] = searchcount[i + 1];
        }
        searchcount[6] = sd[datapos];
        writedata[writeleng] = sd[datapos];//データコピー
        searchcount[7] = '\0';
        coucount = strcmp((const char*)searchcount, (const char*)count);
        datapos++;
        writeleng++;
    }
    //data write
    for (int i = 0; i < sicount_place; i++) {
        writedata[writeleng] = countstr[i];
        writeleng++;
        datapos++;
    }

    //si unique write and copy
    coucount = 1;
    while (coucount != 0)
    {
        for (int i = 0; i < 12; i++) {
            searchunique[i] = searchunique[i + 1];
        }
        searchunique[12] = sd[datapos];
        writedata[writeleng] = sd[datapos];//データコピー
        searchunique[13] = '\0';
        coucount = strcmp((const char*)searchunique, (const char*)uniquecount);
        datapos++;
        writeleng++;
    }

    //data write
    for (int i = 0; i < siunique_place; i++) {
        writedata[writeleng] = uniqstr[i];
        writeleng++;
        datapos++;
    }

    //si count and copy
    while (sislide < stocksic) {
        for (int i = 0; i < 3; i++) {
            searchsi[i] = searchsi[i + 1];
        }
        searchsi[3] = sd[datapos];
        writedata[writeleng] = sd[datapos];//データコピー
        searchsi[4] = '\0';
        datapos++;
        writeleng++;
        //std::cout << "searchsi:" << searchsi << " , ";
        int result = strcmp((const char*)sitag, (const char*)searchsi);
        if (result == 0)
            sislide++;
    }
    //std::cout<<"si 数 : "<<sislide<<std::endl;

    //si end and copy
    coucount = 1;
    while (coucount != 0)
    {
        for (int i = 0; i < 4; i++) {
            siendslide[i] = siendslide[i + 1];
        }
        siendslide[4] = sd[datapos];
        writedata[writeleng] = sd[datapos];//データコピー
        siendslide[5] = '\0';
        coucount = strcmp((const char*)siend, (const char*)siendslide);
        datapos++;
        writeleng++;
    }

    //in string data write
    const char si[] = "<si><t>";
    const char esi[] = "</t><phoneticPr fontId=\"366\"/></si>";
    const char ssi[] = "</t></si>";
    size_t nmc = 0;

    siunique = stocksic;//sicount書き込み前の数に

    //メイン日付文字列追加
    for (size_t i = 0; i < strlen(si); i++) {
        writedata[writeleng] = si[i];
        writeleng++;
    }
    for (size_t i = 0; i < instrlen; i++) {
        writedata[writeleng] = instr[i];
        writeleng++;
    }
    siunique++;
    for (size_t i = 0; i < strlen(esi); i++) {
        writedata[writeleng] = esi[i];
        writeleng++;
    }

    subm = substr;
    //サブ文字列追加
    while (subm) {
        if (!subm->sinum) {//siになければ書き込み
            for (int i = 0; i < strlen(si); i++) {
                writedata[writeleng] = si[i];
                writeleng++;
            }
            nmc = 0;
            UINT8* stockstr=nullptr;
            while (subm->txt[nmc] != '\0') {
                writedata[writeleng] = subm->txt[nmc];
                nmc++; writeleng++;
            }
            //submath[j] = (UINT8*)malloc(nmc + 1);
            subm->sinum = st.InttoChar(siunique, &siunique_place);//si番号入れる

            std::cout << "si number : " << subm->sinum<<" "<< writeleng<<" "<<datapos << " " << datalen << std::endl;
            siunique++;

            for (size_t i = 0; i < strlen(ssi); i++) {
                writedata[writeleng] = ssi[i];
                writeleng++;
            }
        }
        subm = subm->next;
    }
    std::cout << "追加データ書き込み終わり" << std::endl;
    //write to end
    while (datapos < sdlen) {
        writedata[writeleng] = sd[datapos];//データコピー
        datapos++;
        writeleng++;
        std::cout << "sdlen : " << writeleng << " " << datapos << " " << sdlen << std::endl;
    }
    uniqstr = st.InttoChar(siunique - 1, &siunique_place);
    //std::cout << "uniqstr : " << uniqstr <<std::endl;
    Crc.mcrc(writedata, writeleng);
    buckcrc = Crc.crc32;

    std::cout << "si書き込み終わり" << std::endl;

    return writedata;//元データ更新
}
//Si 文字列検索
inputtxt* shareRandD::searchSi(inputtxt* ipt) {
    int result = 0;
    int place = 0;
    int searcount = 0;
    UINT8* num = nullptr;
    inputtxt* root = ipt;

    while (ipt) {
        for (int i = 0; i < siunique; i++) {
            result = strcmp((const char*)sis[i]->Ts, (const char*)ipt->txt);
            if (result == 0) {//文字列一致
                num = st.InttoChar(i, &place);//intをcharへ
                std::cout << "si 一致" << num << std::endl;
                ipt->sinum = num;
                break;
            }
         }
        ipt = ipt->next;
    }
    
    return ipt;
}

//share を配列へ入れる
void shareRandD::ReadShare() {
    /*そのセルの書式設定は、RichTextRun (<r>) 要素と RunProperties (<rPr>) 要素を使用して、テキストと一緒に共有文字列項目内に保存されます。 異なる方法で書式設定されているテキスト間の空白文字を保持するために、text (<t>) 要素の space 属性は preserve に設定されています。 */
    UINT64 dp = 0;//intから変更
    UINT32 i = 0;
    int result = 0;
    int ENDresult = 1;

    UINT8 sip[5] = { 0 };//検索スライド用
    UINT8 Esip[6] = { 0 };///si検索スライド用

    UINT32 newsize = sizeof(Si);
    newsize = newsize * (siunique+1);
    sis = (Si**)malloc(sizeof(Si) * siunique);//si数分の配列確保

    //テスト用
    searchItemNum change=searchItemNum(nullptr,nullptr);

    if (sip && sis && Esip) {
        while (dp < sdlen) {
            for (int j = 0; j < 4 - 1; j++) {
                sip[j] = sip[j + 1];
            }
            sip[4 - 1] = sd[dp];//最後に付け加える
            sip[4] = '\0'; dp++;//位置移動

            result = strncmp((char const*)sip, tagSi, 4);
            if (result == 0) {//siタグ一致後<t>検索
                Si* Sts = nullptr;
                ENDresult = 1;
                sis[mycount] = nullptr;
                while (ENDresult != 0) {// </si>まで
                    for (int j = 0; j < 5 - 1; j++) {
                        Esip[j] = Esip[j + 1];
                        if (j < 3 - 1)
                            sip[j] = sip[j + 1];
                    }
                    Esip[5 - 1] = sip[3 - 1] = sd[dp];//最後に付け加える
                    Esip[5] = sip[3] = '\0'; dp++;//位置移動

                    ENDresult = strncmp((char const*)Esip, esi, 5);
                    result = strncmp((char const*)sip, tagT, 2);
                    if (result == 0) {// <t が一致したら<までsi get
                        i = 0;
                        while (sd[dp - 1] != '>'&& dp < sdlen)//t tag 閉じまで
                            dp++;
                        while (sd[dp + i] != '<'&& dp < sdlen)
                            i++;//文字数カウント

                        UINT32 msize = i + 1;
                        UINT8* sistr = (UINT8*)malloc(msize);//tagの値
                        if (sistr) {
                            for (UINT32 j = 0; j < i; j++) {
                                sistr[j] = sd[dp];//si 文字取得
                                dp++;
                            }
                            sistr[i] = '\0';
                            //std::cout << "get si str" << sistr << std::endl;
                            sis[mycount] = addSitable(sis[mycount], sistr);

                            Tcount++;//t配列数
                        }
                        else {
                            std::cout << "cant get si str" << std::endl;
                        }
                    }
                }
                Tcount = 0;
                mycount++;//si配列数
            }
        }
    }
    else {
        std::cout << "malloc si get null" << std::endl;
    }
}

Si* shareRandD::addSitable(Si* s, UINT8* str) {
    if (!s) {
        s = (Si*)malloc(sizeof(Si));
        if (s) {
            s->Ts = str;
            s->next = nullptr;
        }
    }
    else {
        s->next = addSitable(s->next, str);
    }
    return s;
}

void shareRandD::Sitablefree(Si* s) {
    Si* q = nullptr;

    while (s) {
        q = s->next;  /* 次へのポインタを保存 */
        free(s);
        s = q;
    }
}