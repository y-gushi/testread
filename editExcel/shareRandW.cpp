#include "shareRandW.h"
#include"SearchItemNUm.h"

shareRandD::shareRandD(UINT8* d, UINT64 l) {
    sd = d;
    sdlen = l;
    inputsinum = (char**)malloc(5);
    subcount = 0;
}

shareRandD::~shareRandD() {
    
    for (int i = 0; i < mycount; i++) {//シェアー文字列　メモリ解放
        if (sis[i]) {
            sirPhfree(sis[i]->rp);
            siTvfree(sis[i]->Ts);
            free(sis[i]->phonetic);
            free(sis[i]);
        }
    }
    
    //free(writedata);
    //free(inputsinum);
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
                while (sd[datapos + siunique_place] != '"') {
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
            double po = pow(10, siunique_place - i - 1);
            UINT32 powsize = UINT32(po);
            siunique += (getunique[i] - 48) * powsize;
        }
        std::cout << "get si unique " << siunique << std::endl;
        free(getunique);
    }
}

char* shareRandD::writeSubstr(UINT8* d, char* s) {
    UINT8* sinum = nullptr;
    const char si[] = "<si><t>";
    const char ssi[] = "</t></si>";

    //サブ文字列追加
    if (strlen(s) > 0) {
        for (int i = 0; i < strlen(si); i++) {
            d[writeleng] = si[i];
            writeleng++;
        }
        size_t nmc = 0;
        UINT8* stockstr = nullptr;
        while (s[nmc] != '\0') {
            d[writeleng] = s[nmc];
            nmc++; writeleng++;
        }
        //submath[j] = (UINT8*)malloc(nmc + 1);
        sinum = st.InttoChar(siunique, &siunique_place);//si番号入れる
        siunique++;

        for (int i = 0; i < strlen(ssi); i++) {
            d[writeleng] = ssi[i];
            writeleng++;
        }
    }
    /*
    else {
        sinum = (UINT8*)malloc(1);
        sinum = nullptr;
    }
    */

    return (char*)sinum;
}

/*<si><t>5/21着</t><rPh sb="4" eb="5"><t>チャク</t></rPh><phoneticPr fontId="1"/></si>*/
UINT8* shareRandD::writeshare(UINT8* instr, int instrlen, char* subone, char* subtwo, char* subthree, char* subfour) {
    UINT8 sitag[] = "<si>";
    UINT8 searchsi[5] = { 0 };
    UINT8 siend[] = "</si>";
    UINT8 siendslide[6] = { 0 };

    UINT32 datalen = UINT32(sdlen) + 3000;//データ長

    UINT8* writedata = (unsigned char*)malloc(datalen);

    UINT8 count[] = "count=\"";
    UINT8 uniquecount[] = "uniqueCount=\"";

    unsigned char searchcount[8] = { 0 };//検索スライド用 7文字
    unsigned char searchunique[14] = { 0 };

    int sislide = 0;
    size_t datapos = 0;

    int stocksic = siunique;

    //サブ文字カウント
    if (strlen(subfour) > 0 && !inputsinum[0]) {
        siunique++; sicount++;
    }
    if (strlen(subthree) > 0 && !inputsinum[1]) {
        siunique++; sicount++;
    }
    if (strlen(subtwo) > 0 && !inputsinum[2]) {
        siunique++; sicount++;
    }
    if (strlen(subone) > 0 && !inputsinum[3]) {
        siunique++; sicount++;
    }
    //メイン文字プラス
    siunique++; sicount++;

    countstr = st.InttoChar(sicount, &sicount_place);
    uniqstr = st.InttoChar(siunique, &siunique_place);

    std::cout << "もとのsi 数 :" << stocksic << " " << siunique << std::endl;

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
    uniqstr = st.InttoChar(siunique, &siunique_place);//メイン文字 si番号
    //メイン日付文字列追加
    if (instrlen > 0) {
        for (int i = 0; i < strlen(si); i++) {
            writedata[writeleng] = si[i];
            writeleng++;
        }
        for (int i = 0; i < instrlen; i++) {
            writedata[writeleng] = instr[i];
            writeleng++;
        }
        siunique++;
        for (int i = 0; i < strlen(esi); i++) {
            writedata[writeleng] = esi[i];
            writeleng++;
        }
    }

    //サブ文字列追加 文字がある　si無し
    char* sin = nullptr;
    if (!inputsinum[0] && strlen(subfour) > 0)
        inputsinum[0] = writeSubstr(writedata, subfour);
    if (!inputsinum[1] && strlen(subthree) > 0)
        inputsinum[1] = writeSubstr(writedata, subthree);
    if (!inputsinum[2] && strlen(subtwo) > 0)
        inputsinum[2] = writeSubstr(writedata, subtwo);
    if (!inputsinum[3] && strlen(subone) > 0)
        inputsinum[3] = writeSubstr(writedata, subone);


    //write to end
    while (datapos < sdlen) {
        writedata[writeleng] = sd[datapos];//データコピー
        datapos++;
        writeleng++;
    }

    //std::cout << "uniqstr : " << uniqstr <<std::endl;
    Crc.mcrc(writedata, writeleng);
    buckcrc = Crc.crc32;

    return writedata;//元データ更新
}
//Si 文字列検索
void shareRandD::searchSi(char* ipto, char* iptt, char* iptth, char* iptf) {
    int result = 0;
    int place = 0;
    int searcount = 0;
    UINT8* num = nullptr;

    for (int i = 0; i < 5; i++)
        inputsinum[i] = nullptr;

    for (int i = 0; i < siunique; i++) {
        if (strlen(ipto) > 0) {
            result = strcmp((const char*)sis[i]->Ts->Tv, (const char*)ipto);
            if (result == 0) {//文字列一致
                num = st.InttoChar(i, &place);//intをcharへ
                inputsinum[0] = (char*)num;
            }
        }
        if (strlen(iptt) > 0) {
            result = strcmp((const char*)sis[i]->Ts->Tv, (const char*)iptt);
            if (result == 0) {//文字列一致
                num = st.InttoChar(i, &place);//intをcharへ
                inputsinum[1] = (char*)num;
            }
        }

        if (strlen(iptth) > 0) {
            result = strcmp((const char*)sis[i]->Ts->Tv, (const char*)iptth);
            if (result == 0) {//文字列一致
                num = st.InttoChar(i, &place);//intをcharへ
                inputsinum[2] = (char*)num;
            }
        }

        if (strlen(iptf) > 0) {
            result = strcmp((const char*)sis[i]->Ts->Tv, (const char*)iptf);
            if (result == 0) {//文字列一致
                num = st.InttoChar(i, &place);//intをcharへ
                inputsinum[3] = (char*)num;
            }
        }
    }
}

//share を配列へ入れる
void shareRandD::ReadShare() {
    /*そのセルの書式設定は、RichTextRun (<r>) 要素と RunProperties (<rPr>) 要素を使用して、テキストと一緒に共有文字列項目内に保存されます。 異なる方法で書式設定されているテキスト間の空白文字を保持するために、text (<t>) 要素の space 属性は preserve に設定されています。 */
    //intから変更
    UINT32 i = 0;
    int result = 0;
    int ENDresult = 1;

    UINT8 sip[5] = { 0 };//検索スライド用
    UINT8 Esip[6] = { 0 };///si検索スライド用

    UINT32 newsize = sizeof(Si);
    newsize = newsize * (siunique + 1);
    sis = (Si**)malloc(sizeof(Si) * newsize);//si数分の配列確保

    //テスト用
    //searchItemNum change = searchItemNum(nullptr);
    dp = 0;

    if (sip && sis && Esip) {
        while (dp < sdlen) {
            for (int j = 0; j < 4 - 1; j++) {
                sip[j] = sip[j + 1];
            }
            sip[4 - 1] = sd[dp];//最後に付け加える
            sip[4] = '\0'; dp++;//位置移動

            result = strncmp((char const*)sip, tagSi, 4);
            if (result == 0) {//siタグ一致後<t>検索
                sis[mycount] = getSi();
                mycount++;//si配列数
            }
        }
    }
}

Si* shareRandD::getSi() {
    const char phoneti[] = "<phoneticPr";//11文字
    const char rph[] = "<rPh";//4文字
    const char closerph[] = "</rPh>";//6文字
    const char ttag[] = "<t";//2文字
    const char tclose[] = "</t>";//4文字
    const char closetag[] = "/>";//2文字
    const char siend[] = "</si>";//5文字

    char Pho[12] = { 0 };
    char Rph[5] = { 0 };
    char CRph[7] = { 0 };
    char Tta[4] = { 0 };
    char Clo[3] = { 0 };
    char eSi[6] = { 0 };

    int result = 0;
    int eres = 1;

    UINT8* pho = nullptr;
    Tvalue* Tv = nullptr;
    rPhtag* rPh = nullptr;
    Si* siv = nullptr;

    while (eres != 0) {
        for (int i = 0; i < 10; i++) {
            Pho[i] = Pho[i + 1];
            if (i < 5)
                CRph[i] = CRph[i + 1];
            if (i < 4)
                eSi[i] = eSi[i + 1];
            if (i < 3)
                Rph[i] = Rph[i + 1];
            if (i < 2)
                Tta[i] = Tta[i + 1];
            if (i < 1)
                Clo[i] = Clo[i + 1];
        }
        Pho[10] = eSi[4] = CRph[5] = Rph[3] = Tta[2] = Clo[1] = sd[dp];
        Pho[11] = eSi[5] = CRph[6] = Rph[4] = Tta[3] = Clo[2] = '\0';
        dp++;

        eres = strncmp((const char*)eSi, siend, 5);// </si> 検索

        result = strncmp((const char*)Pho, phoneti, 11);//phoneticPr 検索
        if (result == 0)
            pho = getphoneticPr();

        result = strncmp((const char*)Rph, rph, 4);//rph 検索
        if (result == 0)
            rPh = getrPh(rPh);

        result = strncmp((const char*)Clo, ttag, 2);//t 検索
        if (result == 0) {
            Tv = getTtagValue(Tv);//リスト追加
        }
    }

    siv = (Si*)malloc(sizeof(Si));
    if (!Tv) {
        std::cout << "t null" << std::endl;
    }
    siv->phonetic = pho;
    siv->rp = rPh;
    siv->Ts = Tv;

    return siv;
}

rPhtag* shareRandD::getrPh(rPhtag* rpt) {
    const char closerph[] = "</rPh>";//6文字
    const char sb[] = "sb=\"";//4文字
    const char eb[] = "eb=\"";//4文字
    const char ttag[] = "<t>";//3文字
    const char tclose[] = "</t>";//4文字
    const char closetag[] = "/>";//2文字

    char Rph[5] = { 0 };
    char CRph[7] = { 0 };
    char Tta[4] = { 0 };
    char Clo[3] = { 0 };

    UINT8* Sb = nullptr;
    UINT8* Eb = nullptr;
    UINT8* Tv = nullptr;

    int result = 0;
    int endr = 1;

    while (endr != 0) {
        for (int i = 0; i < 5; i++) {
            CRph[i] = CRph[i + 1];
            if (i < 3)
                Rph[i] = Rph[i + 1];
            if (i < 2)
                Tta[i] = Tta[i + 1];
            if (i < 1)
                Clo[i] = Clo[i + 1];
        }
        CRph[5] = Rph[3] = Tta[2] = Clo[1] = sd[dp];
        CRph[6] = Rph[4] = Tta[3] = Clo[2] = '\0';
        dp++;

        endr = strncmp((const char*)CRph, closerph, 6);// </rPh> 検索

        result = strncmp((const char*)Rph, sb, 4);//sb 検索
        if (result == 0)
            Sb = getValue();

        result = strncmp((const char*)Rph, eb, 4);//eb 検索
        if (result == 0)
            Eb = getValue();

        result = strncmp((const char*)Tta, ttag, 3);//<t> 検索
        if (result == 0)
            Tv = getTValue();
    }

    rpt = addrPh(rpt, Sb, Eb, Tv);

    return rpt;
}

Tvalue* shareRandD::addT(Tvalue* t, UINT8* v, UINT8* x) {
    if (!t) {
        t = (Tvalue*)malloc(sizeof(Tvalue));
        t->Tv = v;
        t->xm = x;
        t->next = nullptr;
    }
    else
        t->next = addT(t->next, v,x);
    return t;
}

rPhtag* shareRandD::addrPh(rPhtag* r, UINT8* s, UINT8* e, UINT8* t) {
    if (!r) {
        r = (rPhtag*)malloc(sizeof(rPhtag));
        r->sb = s;
        r->eb = e;
        r->t = t;
        r->next = nullptr;
    }
    else
        r->next = addrPh(r->next, s, e, t);

    return r;
}

//siの中のtタグ
Tvalue* shareRandD::getTtagValue(Tvalue* tvs) {
    //<t xml:space="preserve"> or <t>
    const char tclose[] = "</t>";//4文字
    const char xmlsp[] = "xml:space=\"";//11文字

    char Rph[5] = { 0 };
    char Xms[12] = { 0 };

    UINT8* Tv = nullptr;
    UINT8* Xm = nullptr;

    int res = 0;
    size_t len = 0;

    while (sd[dp] != '>') {
        for (int i = 0; i < 10; i++)
            Xms[i] = Xms[i + 1];
        Xms[10] = sd[dp];
        Xms[11] = '\0';
        dp++;

        res = strncmp((const char*)Xms, xmlsp, 11);
        if (res == 0) {
            Xm = getValue();
        }
    }

    dp++;// >次から

    while (sd[dp + len] != '<')
        len++;

    Tv = (UINT8*)malloc(len + 1);

    for (size_t j = 0; j < len; j++) {
        Tv[j] = sd[dp];
        dp++;
    }
    Tv[len] = '\0';

    tvs = addT(tvs, Tv, Xm);

    return tvs;
}

// rphの中のtタグ
UINT8* shareRandD::getTValue() {
    //<t xml:space="preserve"> or <t>
    const char tclose[] = "</t>";//4文字
    char Rph[5] = { 0 };
    UINT8* Tv = nullptr;

    size_t len = 0;
    while (sd[dp] != '>')
        dp++;
    dp++;// >次から

    while (sd[dp + len] != '<')
        len++;

    Tv = (UINT8*)malloc(len+1);

    for (size_t j = 0; j < len; j++) {
        Tv[j] = sd[dp];
        dp++;
    }
    Tv[len] = '\0';

    return Tv;
    
}

UINT8* shareRandD::getphoneticPr() {
    //phoneticPrの取得

    const char fontId[] = "fontId=\"";//8文字
    char Fid[9] = { 0 };

    UINT8* fid = nullptr;

    int res = 0;
    while (sd[dp] != '>') {
        for (int i = 0; i < 7; i++)
            Fid[i] = Fid[i + 1];
        Fid[7] = sd[dp];
        Fid[8] = '\0';
        dp++;

        res = strncmp((const char*)Fid, fontId, 8);
        if (res == 0) {
            fid=getValue();
        }
    }

    return fid;
}

UINT8* shareRandD::getValue() {

    size_t vallen = 0;
    size_t taglen = size_t(dp);
    size_t msize = 0;
    UINT8* tagval;
    tagval = nullptr;

    while (sd[dp] != '"') {
        if (sd[dp] != '"')//read data  && d[readp] != '/'
            vallen++;
        dp++;
    }
    //strl = vallen;
    msize = vallen + 1;

    tagval = (UINT8*)malloc(msize);

    for (size_t i = 0; i < vallen; i++)
        tagval[i] = sd[taglen + i];

    tagval[vallen] = '\0';
    return tagval;
}

void shareRandD::sirPhfree(rPhtag* s) {
    rPhtag* q = nullptr;

    freecount++;

    while (s) {
        q = s->next;  /* 次へのポインタを保存 */
        free(s->sb);
        free(s->eb);
        //free(s);        
            //std::cout << "free count : " << freecount << " " << s->Ts << std::endl;        
        s = q;
    }    
}

void shareRandD::siTvfree(Tvalue* s) {
    Tvalue* q = nullptr;

    while (s) {
        q = s->next;  /* 次へのポインタを保存 */
        free(s->Tv); 
        free(s->xm);
        s = q;
    }
}