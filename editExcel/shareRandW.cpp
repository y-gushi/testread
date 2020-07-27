#include "shareRandW.h"
#include"SearchItemNUm.h"

shareRandD::shareRandD(UINT8* d, UINT64 l) {
    sd = d;
    sdlen = l;
    inputsinum = (char**)calloc(5,sizeof(char));
    subcount = 0;
    wd = nullptr;
    wlen = 0;
    sis = nullptr;
}

shareRandD::~shareRandD() {
    for (int ht = 0; ht < mycount; ht++) {//�V�F�A�[������@���������
        if (sis[ht]) {
            if(sis[ht]->rp)
                sirPhfree(sis[ht]->rp);
            if (sis[ht]->Ts)
                siTvfree(sis[ht]->Ts);
            if (sis[ht]->rPr)
                sirPrfree(sis[ht]->rPr);
            free(sis[ht]->phonetic);
            free(sis[ht]);
        }
    }
    free(wd);
    //free(writedata);
    //for(int i=0;i<5;i++)
    free(inputsinum);
}

void shareRandD::getSicount() {
    UINT8 count[] = "count=\"";//r�̐�
    UINT8 uniquecount[] = "uniqueCount=\"";//si�̐�

    unsigned char searchcount[8] = { 0 };//�����X���C�h�p 7����
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
        for (int i = 0; i < sicount_place; i++) {//share count���@�����ɕϊ�
            double po = pow(10, siunique_place - i - 1);
            UINT32 powsize = UINT32(po);
            sicount += (getcount[i] - 48) * powsize;
        }
        free(getcount);
    }

    if (getunique) {
        for (int i = 0; i < siunique_place; i++) {//share unique���@�����ɕϊ�
            double po = pow(10, siunique_place - i - 1);
            UINT32 powsize = UINT32(po);
            siunique += (getunique[i] - 48) * powsize;
        }
        free(getunique);
    }
}

char* shareRandD::writeSubstr(UINT8* d, char* s) {
    UINT8* sinum = nullptr;
    const char si[] = "<si><t>";
    const char ssi[] = "</t></si>";

    //�T�u������ǉ�
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
        sinum = st.InttoChar(siunique, &siunique_place);//si�ԍ������
        siunique++;

        for (int i = 0; i < strlen(ssi); i++) {
            d[writeleng] = ssi[i];
            writeleng++;
        }
    }
    return (char*)sinum;
}

UINT8* shareRandD::addsistr(UINT8* sistr) {
    size_t Sisiz = siunique + 1;
    Si** resis = (Si**)realloc(sis, sizeof(Si)* Sisiz);

    Tvalue* tv=(Tvalue*)malloc(sizeof(Tvalue));
    tv->Tv = sistr;
    tv->xm = nullptr;
    tv->next = nullptr;

    resis[siunique] = (Si*)malloc(sizeof(Si));
    resis[siunique]->Ts = tv;
    resis[siunique]->phonetic = nullptr;
    resis[siunique]->rp = nullptr;
    resis[siunique]->rPr = nullptr;

    sis = resis;//���̎Q�ƕύX

    siunique++;

    return nullptr;
}

/*<si><t>5/21��</t><rPh sb="4" eb="5"><t>�`���N</t></rPh><phoneticPr fontId="1"/></si>*/
UINT8* shareRandD::writeshare(UINT8* instr, int instrlen, char* subone, char* subtwo, char* subthree, char* subfour) {
    UINT8 sitag[] = "<si>";
    UINT8 searchsi[5] = { 0 };
    UINT8 siend[] = "</si>";
    UINT8 siendslide[6] = { 0 };

    UINT32 datalen = UINT32(sdlen) + 4000;//�f�[�^��

    UINT8* writedata = (unsigned char*)malloc(datalen);

    UINT8 count[] = "count=\"";
    UINT8 uniquecount[] = "uniqueCount=\"";

    unsigned char searchcount[8] = { 0 };//�����X���C�h�p 7����
    unsigned char searchunique[14] = { 0 };

    int sislide = 0;
    size_t datapos = 0;

    int stocksic = siunique;

    //�T�u�����J�E���g
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
    //���C�������v���X
    siunique++; sicount++;

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
        writedata[writeleng] = sd[datapos];//�f�[�^�R�s�[
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
        writedata[writeleng] = sd[datapos];//�f�[�^�R�s�[
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
        writedata[writeleng] = sd[datapos];//�f�[�^�R�s�[
        searchsi[4] = '\0';
        datapos++;
        writeleng++;
        int result = strcmp((const char*)sitag, (const char*)searchsi);
        if (result == 0)
            sislide++;
    }

    //si end and copy
    coucount = 1;
    while (coucount != 0)
    {
        for (int i = 0; i < 4; i++) {
            siendslide[i] = siendslide[i + 1];
        }
        siendslide[4] = sd[datapos];
        writedata[writeleng] = sd[datapos];//�f�[�^�R�s�[
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

    siunique = stocksic;//sicount�������ݑO�̐���
    uniqstr = st.InttoChar(siunique, &siunique_place);//���C������ si�ԍ�
    //���C�����t������ǉ�
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

    //�T�u������ǉ� ����������@si����
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
        writedata[writeleng] = sd[datapos];//�f�[�^�R�s�[
        datapos++;
        writeleng++;
    }

    //std::cout << "uniqstr : " << uniqstr <<std::endl;
    //Crc.mcrc(writedata, writeleng);
    //buckcrc = Crc.crc32;

    return writedata;//���f�[�^�X�V
}
//Si �����񌟍�
void shareRandD::searchSi(char* ipto, char* iptt, char* iptth, char* iptf) {
    int result = 0;
    int place = 0;
    int searcount = 0;
    UINT8* num = nullptr;
    size_t strsize = 0;
    size_t o = strlen(ipto);
    size_t t = strlen(iptt);
    size_t th = strlen(iptth);
    size_t fo = strlen(iptf);

    for (UINT32 i = 0; i < siunique; i++) {
        if (sis[i]->Ts) {
            if (sis[i]->Ts->Tv) {
                strsize = strlen((const char*)sis[i]->Ts->Tv);
                if (o > 0&& strsize>0) {
                    result = strncmp((const char*)sis[i]->Ts->Tv, (const char*)ipto,strsize);
                    if (result == 0) {//�������v
                        num = st.InttoChar(i, &place);//int��char��
                        inputsinum[0] = (char*)num;
                    }
                }
                if (t > 0&& strsize>0) {
                    result = strncmp((const char*)sis[i]->Ts->Tv, (const char*)iptt, strsize);
                    if (result == 0) {//�������v
                        num = st.InttoChar(i, &place);//int��char��
                        inputsinum[1] = (char*)num;
                    }
                }

                if (th > 0&& strsize>0) {
                    result = strncmp((const char*)sis[i]->Ts->Tv, (const char*)iptth, strsize);
                    if (result == 0) {//�������v
                        num = st.InttoChar(i, &place);//int��char��
                        inputsinum[2] = (char*)num;
                    }
                }

                if (fo > 0&& strsize>0) {
                    result = strncmp((const char*)sis[i]->Ts->Tv, (const char*)iptf, strsize);
                    if (result == 0) {//�������v
                        num = st.InttoChar(i, &place);//int��char��
                        inputsinum[3] = (char*)num;
                    }
                }
            }            
        }
    }

    if (!inputsinum[0] && o > 0) {
        //����si�ǉ�
        addsistr((UINT8*)ipto);
    }
}

//share ��z��֓����
void shareRandD::ReadShare() {
    /*���̃Z���̏����ݒ�́ARichTextRun (<r>) �v�f�� RunProperties (<rPr>) �v�f���g�p���āA�e�L�X�g�ƈꏏ�ɋ��L�����񍀖ړ��ɕۑ�����܂��B �قȂ���@�ŏ����ݒ肳��Ă���e�L�X�g�Ԃ̋󔒕�����ێ����邽�߂ɁAtext (<t>) �v�f�� space ������ preserve �ɐݒ肳��Ă��܂��B */
    //int����ύX
    UINT32 i = 0;
    int result = 0;
    int ENDresult = 1;

    UINT8 sip[5] = { 0 };//�����X���C�h�p
    UINT8 Esip[6] = { 0 };///si�����X���C�h�p

    UINT32 newsize = sizeof(Si);
    newsize = newsize * (siunique);
    sis = (Si**)calloc(newsize, sizeof(Si));//si�����̔z��m��

    //�e�X�g�p
    //searchItemNum change = searchItemNum(nullptr);
    dp = 0;

    if (sip && sis && Esip) {
        while (dp < sdlen) {
            for (int j = 0; j < 4 - 1; j++) {
                sip[j] = sip[j + 1];
            }
            sip[4 - 1] = sd[dp];//�Ō�ɕt��������
            sip[4] = '\0'; dp++;//�ʒu�ړ�

            result = strncmp((char const*)sip, tagSi, 4);
            if (result == 0) {//si�^�O��v��<t>����
                sis[mycount] = getSi(sis[mycount]);
                mycount++;//si�z��
            }
        }
        //std::cout << "������ : " << sicount << std::endl;
    }
}

Si* shareRandD::getSi(Si *h) {
    const char phoneti[] = "<phoneticPr";//11����
    const char rph[] = "<rPh";//4����
    const char closerph[] = "</rPh>";//6����
    const char ttag[] = "<t";//2����
    const char tclose[] = "</t>";//4����
    const char closetag[] = "/>";//2����
    const char siend[] = "</si>";//5����
    const char rpr[] = "<rPr>";//5����

    char Pho[12] = { 0 };
    char Rph[5] = { 0 };
    char Tta[4] = { 0 };
    char Clo[3] = { 0 };
    char eSi[6] = { 0 };

    int result = 0;
    int eres = 1;

    UINT8* pho = nullptr;
    Tvalue* Tv = nullptr;
    rPhtag* rPh = nullptr;
    rPrtag* rr = nullptr;
    h = nullptr;

    while (eres != 0) {
        for (int i = 0; i < 10; i++) {
            Pho[i] = Pho[i + 1];
            if (i < 5)
                Rph[i] = Rph[i + 1];
            if (i < 4)
                eSi[i] = eSi[i + 1];
            if (i < 1)
                Clo[i] = Clo[i + 1];
        }
        Pho[10] = eSi[4] = Rph[3] = Clo[1] = sd[dp];
        Pho[11] = eSi[5] = Rph[4] = Clo[2] = '\0';
        dp++;

        eres = strncmp((const char*)eSi, siend, 5);// </si> ����

        result = strncmp((const char*)eSi, rpr, 5);//rPr ����
        if (result == 0)
            rr = getrPr(rr);

        result = strncmp((const char*)Pho, phoneti, 11);//phoneticPr ����
        if (result == 0)
            pho = getphoneticPr();

        result = strncmp((const char*)Rph, rph, 4);//rph ����
        if (result == 0)
            rPh = getrPh(rPh);

        result = strncmp((const char*)Clo, ttag, 2);//t ����
        if (result == 0) {
            Tv = getTtagValue(Tv);//���X�g�ǉ�
        }
    }    
    h = (Si*)malloc(sizeof(Si));
    h->phonetic = pho;
    h->rPr = rr;
    h->rp = rPh;
    h->Ts = Tv;

    return h;
}

rPrtag* shareRandD::getrPr(rPrtag* rpt) {

    const char closerph[] = "</rPr>";//6����
    const char closetag[] = "/>";//2����
    const char color[] = "color theme=\"";//13����
    const char rgb[] = "color rgb=\"";//11����
    const char rfont[] = "rFont val=\"";//11����
    const char famil[] = "family val=\"";//12
    const char charset[] = "charset val=\"";//13
    const char scheme[] = "scheme val=\"";//12
    const char sz[] = "sz val=\"";//8

    char Rph[5] = { 0 };
    char CRph[7] = { 0 };
    char Tta[4] = { 0 };
    char Clo[3] = { 0 };
    char Col[14] = { 0 };
    char rFo[12] = { 0 };
    char Fami[13] = { 0 };
    char s[9] = { 0 };

    UINT8* c = nullptr;
    UINT8* rf = nullptr;
    UINT8* f = nullptr;
    UINT8* ch = nullptr;
    UINT8* sc = nullptr;
    UINT8* sZ = nullptr;
    UINT8* rg = nullptr;

    int result = 0;
    int endr = 1;

    while (endr != 0) {
        for (int i = 0; i < 12; i++) {
            Col[i] = Col[i + 1];
            if (i < 11)
                Fami[i] = Fami[i + 1];
            if (i < 10)
                rFo[i] = rFo[i + 1];
            if (i < 7)
                s[i] = s[i + 1];
            if (i < 5)
                CRph[i] = CRph[i + 1];
        }
        s[7] = rFo[10] = Fami[11] = Col[12] = CRph[5] = sd[dp];
        s[8] = rFo[11] = Fami[12] = Col[13] = CRph[6] = '\0';
        dp++;

        endr = strncmp((const char*)CRph, closerph, 6);// </rPh> ����

        result = strncmp((const char*)s, sz, 8);//rfont ����
        if (result == 0)
            sZ = getValue();

        result = strncmp((const char*)rFo, rgb, 11);//rgb ����
        if (result == 0)
            rg = getValue();

        result = strncmp((const char*)rFo, rfont, 11);//rfont ����
        if (result == 0)
            rf = getValue();

        result = strncmp((const char*)Fami, scheme, 12);//scheme ����
        if (result == 0)
            sc = getValue();

        result = strncmp((const char*)Fami, famil, 12);//family ����
        if (result == 0)
            f = getValue();

        result = strncmp((const char*)Col, charset, 13);//charset ����
        if (result == 0)
            ch = getValue();

        result = strncmp((const char*)Col, color, 13);//color ����
        if (result == 0)
            c = getValue();
    }
    rpt = addrPr(rpt, sZ, c, rf, f, ch, sc, rg);

    return rpt;
}

rPhtag* shareRandD::getrPh(rPhtag* rpt) {

    const char closerph[] = "</rPh>";//6����
    const char sb[] = "sb=\"";//4����
    const char eb[] = "eb=\"";//4����
    const char ttag[] = "<t>";//3����
    const char tclose[] = "</t>";//4����
    const char closetag[] = "/>";//2����

    char Rph[5] = { 0 };
    char CRph[7] = { 0 };
    char Tta[4] = { 0 };
    char Clo[3] = { 0 };
    char Col[14] = { 0 };
    char rFo[12] = { 0 };
    char Fami[13] = { 0 };
    char s[9] = { 0 };

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
        }
        CRph[5] = Rph[3] = Tta[2] = sd[dp];
        CRph[6] = Rph[4] = Tta[3] = '\0';
        dp++;

        endr = strncmp((const char*)CRph, closerph, 6);// </rPh> ����

        result = strncmp((const char*)Rph, sb, 4);//sb ����
        if (result == 0)
            Sb = getValue();

        result = strncmp((const char*)Rph, eb, 4);//eb ����
        if (result == 0)
            Eb = getValue();

        result = strncmp((const char*)Tta, ttag, 3);//<t> ����
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
        t->next = addT(t->next, v, x);
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

rPrtag* shareRandD::addrPr(rPrtag* r, UINT8* Sz, UINT8* co, UINT8* rf, UINT8* fa, UINT8* ch, UINT8* sc, UINT8* rgb) {
    if (!r) {
        r = (rPrtag*)malloc(sizeof(rPrtag));
        r->sz = Sz;
        r->theme = co;
        r->rgb = rgb;
        r->rFont = rf;
        r->family = fa;
        r->charset = ch;
        r->scheme = sc;
        r->next = nullptr;
    }
    else
        r->next = addrPr(r->next, Sz, co, rf, fa, ch, sc, rgb);

    return r;
}

//si�̒���t�^�O
Tvalue* shareRandD::getTtagValue(Tvalue* tvs) {
    //<t xml:space="preserve"> or <t>
    const char tclose[] = "</t>";//4����
    const char xmlsp[] = "xml:space=\"";//11����

    char Rph[5] = { 0 };
    char Xms[12] = { 0 };

    UINT8* Tv = nullptr;
    UINT8* Xm = nullptr;

    int res = 0;
    size_t len = 0;
    //tvs = nullptr;

    while (sd[dp] != '>') {
        if (sd[dp] == '/')
            return nullptr;

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

    dp++;// >������

    while (sd[dp + len] != '<')
        len++;

    if (len > 0) {
        Tv = (UINT8*)malloc(len + 1);

        for (size_t j = 0; j < len; j++) {
            Tv[j] = sd[dp];
            dp++;
        }
        Tv[len] = '\0';
    }    

    tvs = addT(tvs, Tv, Xm);

    return tvs;
}

// rph�̒���t�^�O
UINT8* shareRandD::getTValue() {
    //<t xml:space="preserve"> or <t>
    const char tclose[] = "</t>";//4����
    char Rph[5] = { 0 };
    UINT8* Tv = nullptr;

    size_t len = 0;
    
    while (sd[dp + len] != '<')
        len++;

    if (len > 0) {
        Tv = (UINT8*)malloc(len + 1);

        for (size_t j = 0; j < len; j++) {
            Tv[j] = sd[dp];
            dp++;
        }
        Tv[len] = '\0';
    }

    return Tv;

}

UINT8* shareRandD::getphoneticPr() {
    //phoneticPr�̎擾

    const char fontId[] = "fontId=\"";//8����
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
            fid = getValue();
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

    tagval = (UINT8*)calloc(msize,sizeof(UINT8));

    for (size_t i = 0; i < vallen; i++) {
        if (sd[taglen + i] != '"')
            tagval[i] = sd[taglen + i];
    }

    tagval[vallen] = '\0';
    return tagval;
}

void shareRandD::sirPrfree(rPrtag* s) {
    rPrtag* q = nullptr;

    while (s) {
        q = s->next;  /* ���ւ̃|�C���^��ۑ� */
        free(s->charset);
        free(s->family);
        free(s->rFont);
        free(s->rgb);
        free(s->scheme);
        free(s->sz);
        free(s->theme);

        free(s);
        s = q;
    }
}

void shareRandD::sirPhfree(rPhtag* s) {
    rPhtag* q = nullptr;

    freecount++;

    while (s) {
        q = s->next;  /* ���ւ̃|�C���^��ۑ� */
        free(s->sb);
        free(s->eb);
        free(s->t);

        free(s);
        s = q;
    }
}

void shareRandD::siTvfree(Tvalue* s) {
    Tvalue* q = nullptr;
    int j = 0;
    while (s) {
        q = s->next;  /* ���ւ̃|�C���^��ۑ� */
        //std::cout << "free count : " << s->Tv << " " << j << std::endl;
        free(s->Tv);
        free(s->xm);
        s = q;
    }
}