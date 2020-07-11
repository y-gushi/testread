#pragma once
#include "typechange.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "decord.h"

#define LOCAL_HEADER 0x504b0304
/*local file header�@�V�O�l�`�� 0x04034b50  0x504b0304*/
#define CENTRALSIGNATURE 0x0504b0102
/*central directory header �V�O�l�`�� 0x02014b50 "PK\01\02"  0x0504b0102*/
#define END_OF_CENTRAL 0x06054b50
#define REND 0x504b0506
/*end of central dir signature �V�O�l�`�� 0x06054b50 "PK\05\06"  0x504b0506*/
#define MAKEVERSION 0x2d //64bit�W��
#define VERSION_LH 0x14 //���[�J���w�b�_�@�W�J�ɕK�v�ȃo�[�W����2byte�i���g���G���f�B�A���j
#define BITFLAG_LH 0x06 //���[�J���w�b�_�@�r�b�g�t���b�O2byte�i���g���G���f�B�A���j 0x14
#define DEFLATE_LH_CD 0x08//���k���\�b�h�@�f�t���\�g�@2byte�i���g���G���f�B�A���j
#define NAIBU 0x0
#define GAIBU 0x0

int searchleng(char n[]);

/*�S�Ẵw�b�_���̕����o�C�g�̒l�́A���g���G���f�B�A���Ŋi�[�����B*/
struct CenterDerect {

    UINT64 curpos = 0;
    UINT16 version = 0;    //�쐬���ꂽ�o�[�W���� 2byte
    UINT16 minversion = 0;//�W�J�ɕK�v�ȃo�[�W���� (�ŏ��o�[�W����) 2byte
    UINT16 bitflag = 0;//�ėp�ړI�̃r�b�g�t���O 2byte
    UINT16 method = 0;//���k���\�b�h 2byte
    UINT16 time = 0;//�t�@�C���̍ŏI�ύX���� 2byte
    UINT16 day = 0;//�t�@�C���̍ŏI�ύX���t 2byte
    UINT32 crc = 0;//CRC-32 4byte
    UINT32 size = 0;//���k�T�C�Y 4byte
    UINT32 nonsize = 0;//�񈳏k�T�C�Y 4byte
    UINT16 filenameleng = 0;//�t�@�C�����̒��� (n) 2byte
    UINT16 fieldleng = 0;//�g���t�B�[���h�̒��� (m) 2byte
    UINT16 fielcomment = 0;//�t�@�C���R�����g�̒��� (k) 2byte
    UINT16 discnum = 0;//�t�@�C�����J�n����f�B�X�N�ԍ� 2byte
    UINT16 zokusei = 0;//�����t�@�C������ 2byte
    UINT32 gaibuzokusei = 0;//�O���t�@�C������ 4byte
    UINT32 localheader = 0;//���[�J���t�@�C���w�b�_�̑��΃I�t�Z�b�g 4byte
    //char *filename = nullptr;//�t�@�C���� nbyte
    char* filename = nullptr;
    char* kakutyo = nullptr;//�g���t�B�[���h mbyte
    char* comment = nullptr;//�t�@�C���R�����g kbyte
};

struct ENDrecord {
    UINT32 endsig = 0;

    UINT16 discnum = 0;    //���̃f�B�X�N�̐� 2byte
    UINT16 disccentral = 0;    //�Z���g�����f�B���N�g�����J�n����f�B�X�N 2byte
    UINT16 centralnum = 0; //���̃f�B�X�N��̃Z���g�����f�B���N�g�����R�[�h�̐� 2byte
    UINT16 centralsum = 0; //�Z���g�����f�B���N�g�����R�[�h�̍��v�� 2byte
    UINT32 size = 0;//�Z���g�����f�B���N�g���̃T�C�Y (�o�C�g) 4byte
    UINT32 position = 0;//�Z���g�����f�B���N�g���̊J�n�ʒu�̃I�t�Z�b�g 4byte
    UINT16 commentleng = 0; //�Z���g�����f�B���N�g�����R�[�h�̍��v�� 2byte
    char* comment = nullptr;//ZIP�t�@�C���̃R�����g nbyte
};

struct LocalHeader {
    UINT16 version = 0;    //�W�J�ɕK�v�ȃo�[�W���� (�ŏ��o�[�W����) 2byte
    UINT16 bitflag = 0;//�ėp�ړI�̃r�b�g�t���O 2byte
    UINT16 method = 0;//���k���\�b�h 2byte
    UINT16 time = 0;//�t�@�C���̍ŏI�ύX���� 2byte
    UINT16 day = 0;//�t�@�C���̍ŏI�ύX���t 2byte
    UINT32 crc = 0;//CRC-32 4byte
    UINT32 size = 0;//���k�T�C�Y 4byte
    UINT32 nonsize = 0;//�񈳏k�T�C�Y 4byte
    UINT16 filenameleng = 0;//�t�@�C�����̒��� (n) 2byte
    UINT16 fieldleng = 0;//�g���t�B�[���h�̒��� (m) 2byte
    char* filename = nullptr;//�t�@�C���� nbyte
    char* kakutyo = nullptr;//�g���t�B�[���h mbyte
    UINT64 pos;//�f�[�^�̎n�܂�ʒu
};

struct changefilenames {
    char* cfn = nullptr;
    changefilenames* next = nullptr;
};

struct filelist {
    char* cfn = nullptr;
    int hash = 0;
    filelist* next = nullptr;
};

struct LHDataes {
    LocalHeader* lhdata;
    LHDataes* nextLH;
};

struct CDdataes {
    CenterDerect cdd;
    CDdataes* nextCD;
};

struct inputtxt {
    UINT8* txt;
    UINT8* sinum;
    inputtxt* next;
    inputtxt* parrent;
};

class HeaderRead
{
public:
    UINT32 readdata;//�ꎞ�ǂݍ��݃f�[�^
    char headerReaddata;
    UINT32 sig;//�V�O�l�`������
    char* readfile;  //�ǂݍ��ރt�@�C���̎w��
    //ifstream* f;//file�f�[�^

    //std::vector<CenterDerect> Croot;

    ENDrecord* ER;//end�V�O�l�`���̍\����
    LocalHeader* LH;//local�V�O�l�`���̍\����
    //CenterDerect *CD;//central�V�O�l�`���̍\����
    CenterDerect* CD;
    CenterDerect* scd;
    UINT64 readpos = 0;//���݂̃t�@�C���ʒu

    LocalHeader* rootLH;//�������[�g
    CenterDerect* r;//�����p���[�g

    LHDataes* saveLH;//���[�J���w�b�_�f�[�^�ۑ��p
    CDdataes* saveCD;//�Z���g�����w�b�_�[�ۑ��p

    UINT64 endpos = 0; //�t�@�C���̏I�[�ʒu
    UINT64 curpos = 0; //�t�@�C���̌��݈ʒu

    UINT32 filenum = 0;//�ǂݍ��ݐ�

    int intxtCount = 0;

    filelist* cdfn = nullptr;

    unsigned char* readLHdata = (unsigned char*)malloc(sizeof(unsigned char) * 100);//crc�e�X�g�p
    unsigned char* lhtest = (unsigned char*)malloc(sizeof(unsigned char) * 1l);

    HeaderRead(char* name);
    ~HeaderRead();
    void freeheader();
    void freeLH();
    void endread(std::ifstream* fin);//�G���h�w�b�_�[���ǂݎ��
    void localread(UINT64 pos, std::ifstream* fin);
    void centerread(UINT64 pos, UINT32 size, UINT16 n, std::ifstream* fin);
    CenterDerect* centeroneread(UINT64 pos, UINT32 size, UINT16 n, char* fn, std::ifstream* fin);

    inputtxt* addtxt(inputtxt* intx, char* tx, inputtxt* par);

    void freetxt(inputtxt* p);

    inputtxt* slipInputText(char* ins, inputtxt* it);

    CenterDerect* searchCENTRAL(char* s);//�t�@�C�����ŃZ���g�����f�B���N�g���̌���

    bool searchChara(char fn[], char* cdfn, UINT16 cdfnlen);//�t�@�C�����Ń��[�J���f�B���N�g���̌���
    CenterDerect* talloc();
    LocalHeader* tallocLH();
    LHDataes* tallocLhData();
    LHDataes* addLhData(LHDataes* ld, LocalHeader* lhd);
    CDdataes* tallocCDdataes();
    CDdataes* addCDdata(CDdataes* c, CenterDerect* cddata);
    filelist* addfn(filelist* f, char* fn, int h);
};

/*�w�b�_�[���̍쐬*/
class HeaderWrite {
public:
    char zero=0;
    unsigned int cdsize = 0;
    UINT32 recordsum = 0;
    unsigned int endsize = 0;
    //UINT16 VERSION_LH = 0x14;//delate
    //UINT16 BITFLAG_LH = 0x06;//deflate Super Fast 0x06
    //UINT16 DEFLATE_LH_CD = 0x08;
    /*�������ݗp�֐�*/
    HeaderWrite();
    void centralwrite(FILE* fn, CenterDerect cd);
    void eocdwrite(FILE* fn, UINT16 discnum, UINT16 cdnum, UINT16 disccdsum, UINT16 cdsum, UINT64 pos, UINT32 size);
    changefilenames* addfilename(changefilenames* infn, char* fn);
    int localwrite(FILE* fn, UINT32 t, UINT32 crc32, UINT32 asize, UINT32 bsize, UINT16 nameleng, char n[], UINT16 ver, UINT16 bitf, UINT16 method);
};
