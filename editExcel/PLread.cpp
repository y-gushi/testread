#include "PLread.h"

PLRead::PLRead()
{
	erroritem = nullptr;
	fnlen = 0;
}

PLRead::~PLRead()
{
	freeItem(erroritem);
}

void PLRead::freeItem(Items* t) {
	struct Items* q;
	while (t != NULL) {
		q = t->next;  /* 次へのポインタを保存 */
		free(t->itn);
		free(t->col);
		free(t->s100);
		free(t->s110);
		free(t->s120);
		free(t->s130);
		free(t->s140);
		free(t->s150);
		free(t->s160);
		free(t->sF);
		free(t->s90);
		free(t);
		t = q;
	}
}

Items* PLRead::packingread(char* fn)
{
	std::ifstream inf(fn, std::ios::in | std::ios::binary);

	erroritem = (Items*)malloc(sizeof(Items));
	erroritem->itn = (UINT8*)malloc(16);
	while (fn[fnlen] != '\0')
		fnlen++;
	fnlen++;
	erroritem->col = (UINT8*)malloc(fnlen);
	strcpy((char*)erroritem->itn, ef);
	strcpy((char*)erroritem->col, fn);
	erroritem->s100 = nullptr;
	erroritem->s110 = nullptr;
	erroritem->s120 = nullptr;
	erroritem->s130 = nullptr;
	erroritem->s140 = nullptr;
	erroritem->s150 = nullptr;
	erroritem->s160 = nullptr;
	erroritem->s90 = nullptr;
	erroritem->sF = nullptr;
	erroritem->next = nullptr;

	if (!inf)
		return nullptr;

	HeaderRead* hr = new HeaderRead(fn);
	CenterDerect* cddata = nullptr;//セントラルディレクトのデータ
	hr->endread(&inf);//終端コードの読み込み
	hr->freeER();

	//-----------------------//
	//シェアー文字列読み込み
	//-----------------------//

	DeflateDecode* decShare = new DeflateDecode(&inf);//sharestring ファイルの保存用
	//share セントラル取得
	while (hr->filenum < hr->ER->centralsum) {//cddata mallocなし
		cddata = hr->centeroneread(hr->readpos, hr->ER->size, hr->ER->centralsum, (char*)shares, &inf);
		if (cddata) {
			break;
		}
		hr->freeheader();
	}
	if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
		hr->localread(cddata->localheader, &inf);//sharesstringsの読み込み
		decShare->dataread(hr->LH->pos, cddata->nonsize);
		hr->freeLH();
	}

	shareRandD* sharray = new shareRandD(decShare->ReadV, decShare->readlen);//share string read to array

	sharray->getSicount();//get si count
	sharray->ReadShare();//文字列読み込み デコードデータ削除OK

	delete decShare;

	//-------------------------------//
	//パッキングリストシート読み込み
	//-------------------------------//

	char sheetname[] = "worksheets/sheet1.xml";//ファイル位置　ファイル名部分一致検索
	DeflateDecode* decsheet = new DeflateDecode(&inf);

	hr->filenum = 0;//レコード数初期化
	hr->readpos = hr->ER->position;
	cddata = nullptr;
	while (hr->filenum < hr->ER->centralsum) {
		cddata = hr->centeroneread(hr->readpos, hr->ER->size, hr->ER->centralsum, sheetname, &inf);//セントラルディレクトのデータ
		if (cddata) {
			break;
		}
		hr->freeheader();
	}
	if (cddata) {//ファイル名が合えばローカルヘッダー読み込み
		hr->localread(cddata->localheader, &inf);//sharesstringsの読み込み
		decsheet->dataread(hr->LH->pos, cddata->nonsize);
		hr->freeLH();
	}

	Ctags* ms = new Ctags(decsheet->ReadV, decsheet->readlen, sharray);

	ms->sheetread();

	delete decsheet;//デコードデータ削除

	shipinfo* sg = new shipinfo(ms->rows);//シートデータ参照　freeなし

	sg->GetItems();//mallocなし　シートとセット

	delete ms;
	delete hr;

	inf.close();

	return sg->its;//get item list
}
