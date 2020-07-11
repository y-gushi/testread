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

    //������̐�
    while (fn[i] != '\0') {
        i++;
    }
    char* searchfilename = (char*)malloc(50);//�����X���C�h�p
    
    int slidepos = 0;
    int result = 0;

    if (searchfilename) {
        while (slidepos < cdfnlen) {
            if (i > cdfnlen) {//�t�@�C��������������Z��
                return false;
            }
            else {
                if (slidepos == 0) {//
                    for (int j = 0; j < i; j++) {//�ŏ��ɕ������filename��i�������
                        searchfilename[j] = cdfn[slidepos];
                        slidepos++;
                    }
                    if (searchfilename)
                        searchfilename[i] = '\0';//������I�[��ǉ�
                }
                else {
                    //�z����ꕶ���Â��炷
                    for (int j = 0; j < i - 1; j++) {
                        searchfilename[j] = searchfilename[j + 1];
                    }
                    searchfilename[i - 1] = cdfn[slidepos];//�Ō�ɕt��������
                    searchfilename[i] = '\0';//������I�[��ǉ�
                    slidepos++;//�ʒu�ړ�
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
        std::cout << "�t�@�C�� ID.txt ���J���܂���";
    }

    infile.seekg(position, std::ios_base::beg);
    compressleng = 0;
    filenameleng = 0;
    headreaddata = 0;

    // �o�C�i���Ƃ��ď�������
    //���[�J���V�O�l�`���̏�������
    for (int i = 0; i < 4; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        writeposition++;
    }

    //�o�[�W�����̏�������
    for (int i = 0; i < 2; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        writeposition++;
    }

    //�r�b�g�t���b�O
    for (int i = 0; i < 2; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        writeposition++;
    }
    unsigned short comtype = 0;
    //���k�����̏�������
    for (int i = 0; i < 2; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        headreaddata = readdata & 0xFF;
        headreaddata = ((headreaddata & 0xFF) << (8 * i));
        comtype += headreaddata;
        writeposition++;
    }
    headreaddata = 0;
    //std::cout << "���k�^�C�v�@�������� : " << comtype-'\0' << std::endl;

    //���ԂƓ��t�̏������� �X�V
    for (int i = 0; i < 4; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        writeposition++;
    }

    //crc 4byte �������݁i�𓀌�̒l��crc�j
    for (int i = 0; i < 4; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        writeposition++;
    }

    //���k�T�C�Y ��������
    headreaddata = 0;
    compressleng = 0;
    for (int i = 0; i < 4; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        headreaddata = ((readdata & 0xFF) << (8 * i));
        compressleng += headreaddata;
        writeposition++;
    }
    //std::cout << "�������݈��k�T�C�Y : " << compressleng << std::endl;
    headreaddata = 0;

    //���k�O�T�C�Y ��������
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
    //std::cout << "�񈳏k�T�C�Y : " << nocom << std::endl;

    //�t�@�C�����̒��� ��������
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
    //�g���t�B�[���h�̒��� 0��������
    for (int i = 0; i < 2; i++) {
        infile.read((char*)&readdata, sizeof(char));
        fwrite(&readdata, sizeof(char), 1, wf);
        kakutyo = readdata & 0xFF;
        kakutyo = ((kakutyo & 0xFF) << (8 * i));
        fieldlen += kakutyo;
        writeposition++;
    }

    if (filenameleng > 0) {
        //�t�@�C�����̏�������
    //std::cout << "�t�@�C����: ";
        for (int i = 0; i < filenameleng; i++) {
            infile.read((char*)&readdata, sizeof(char));
            fwrite(&readdata, sizeof(char), 1, wf);
            //std::cout << readdata;
            writeposition++;
        }
        //std::cout << std::endl;
    }
    
    if (fieldlen > 0) {
        //�g���t�B�[���h�� ��������
    //std::cout << "�g���t�B�[���h";
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
