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
		q = t->next;  /* ���ւ̃|�C���^��ۑ� */
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
	CenterDerect* cddata = nullptr;//�Z���g�����f�B���N�g�̃f�[�^
	hr->endread(&inf);//�I�[�R�[�h�̓ǂݍ���
	hr->freeER();

	//-----------------------//
	//�V�F�A�[������ǂݍ���
	//-----------------------//

	DeflateDecode* decShare = new DeflateDecode(&inf);//sharestring �t�@�C���̕ۑ��p
	//share �Z���g�����擾
	while (hr->filenum < hr->ER->centralsum) {//cddata malloc�Ȃ�
		cddata = hr->centeroneread(hr->readpos, hr->ER->size, hr->ER->centralsum, (char*)shares, &inf);
		if (cddata) {
			break;
		}
		hr->freeheader();
	}
	if (cddata) {//�t�@�C�����������΃��[�J���w�b�_�[�ǂݍ���
		hr->localread(cddata->localheader, &inf);//sharesstrings�̓ǂݍ���
		decShare->dataread(hr->LH->pos, cddata->nonsize);
		hr->freeLH();
	}

	shareRandD* sharray = new shareRandD(decShare->ReadV, decShare->readlen);//share string read to array

	sharray->getSicount();//get si count
	sharray->ReadShare();//������ǂݍ��� �f�R�[�h�f�[�^�폜OK

	delete decShare;

	//-------------------------------//
	//�p�b�L���O���X�g�V�[�g�ǂݍ���
	//-------------------------------//

	char sheetname[] = "worksheets/sheet1.xml";//�t�@�C���ʒu�@�t�@�C����������v����
	DeflateDecode* decsheet = new DeflateDecode(&inf);

	hr->filenum = 0;//���R�[�h��������
	hr->readpos = hr->ER->position;
	cddata = nullptr;
	while (hr->filenum < hr->ER->centralsum) {
		cddata = hr->centeroneread(hr->readpos, hr->ER->size, hr->ER->centralsum, sheetname, &inf);//�Z���g�����f�B���N�g�̃f�[�^
		if (cddata) {
			break;
		}
		hr->freeheader();
	}
	if (cddata) {//�t�@�C�����������΃��[�J���w�b�_�[�ǂݍ���
		hr->localread(cddata->localheader, &inf);//sharesstrings�̓ǂݍ���
		decsheet->dataread(hr->LH->pos, cddata->nonsize);
		hr->freeLH();
	}

	Ctags* ms = new Ctags(decsheet->ReadV, decsheet->readlen, sharray);

	ms->sheetread();

	delete decsheet;//�f�R�[�h�f�[�^�폜

	shipinfo* sg = new shipinfo(ms->rows);//�V�[�g�f�[�^�Q�Ɓ@free�Ȃ�

	sg->GetItems();//malloc�Ȃ��@�V�[�g�ƃZ�b�g

	delete ms;
	delete hr;

	inf.close();

	return sg->its;//get item list
}
