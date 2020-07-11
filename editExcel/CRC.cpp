#include "CRC.h"

crc::crc() {//png�p�����@int len, int w, int h, int bit, int col
    crc32 = UINT32(0xFFFFFFFF);
    magic = UINT32(0xEDB88320);
    tablev = 0;
    /* �e�[�u�����쐬���� */
    for (int i = 0; i < 256; i++) {      /* ����8�r�b�g���ꂼ��ɂ��Čv�Z���� */
        tablev = i;      /* ����8�r�b�g��Y�����ɁA���24�r�b�g��0�ɏ��������� */
        for (int j = 0; j < 8; j++) {
            tablev = (tablev & 1) ? (magic ^ (tablev >> 1)) : (tablev >> 1); /* 1�����ӂꂽ��}�W�b�N�i���o�[��XOR���� */
        }
        table[i] = tablev;        /* �v�Z�����l���e�[�u���Ɋi�[���� */
    }

}

unsigned int crc::mcrc(unsigned char* a, UINT64 len) {
    crc32 = 0xFFFFFFFF;
    /* �e�[�u����p����CRC32���v�Z���� */
    for (UINT64 i = 0; i < len; i++) {
        crc32 = table[(crc32 ^ a[i]) & 0xff] ^ (crc32 >> 8); /* 1�o�C�g�������čX�V���� */
    }
    //printf("CRC:%08" PRIX32"\n", ~crc32);
    crc32 = crc32 ^ 0xFFFFFFFF;
    //std::cout << "crcHEX�\��" << std::hex << crc32 << std::endl;
    return crc32;
}

void crc::rescrc() {//crc���Z�b�g�p
    crc32 = UINT32(0xFFFFFFFF);
}