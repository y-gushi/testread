#include "unitchange.h"
#include<iostream>
unitC::unitC()
{
	Num= (NumberFlag*)malloc(sizeof(NumberFlag));
	Num = nullptr;
}

unitC::~unitC()
{
}

//���p�J�i��S�p��
UINT8* unitC::changeKana(UINT8* s) {
	size_t m = 0;
	UINT32 zen = 0;
	UINT32 index = 0;
	UINT32 sharp = 0xEFBC83;
	UINT32 beforstock = 0;

	//�������m�F
	while (s[m] != '\0')
		m++;

	int i = 0;
	int j = 0;

	UINT8* after = (UINT8*)malloc(m + 1);

	//�S�p�p���T�[�`
	if (m > 2) {
		while (s[i] != '\0') {
			zen = 0;
			if (i > 1) {
				zen += (s[i - 2]) << 16;
				zen += (s[i - 1]) << 8;
				zen += s[i];
				zen = zen & (0xFFFFFF);
			}
			//std::cout << "UI32 :" << zen << std::endl;

			if (zen >= Hkana[0][0] && zen <= Hkana[55][0]) {
				//index = zen - Hkana[0][0];
				for (int k = 0; k < 58; k++) {
					if (Hkana[k][0] == zen) {
						index = k;
						break;
					}
				}
				//std::cout << "���� :" << index << std::endl;
				//2�O�߂� ��� 3�o�C�g
				for (int h = 0; h < 3; h++) {
					after[j - 2 + h] = (Hkana[index][1] >> (16 - (8 * h))) & 0xFF;
				}
				beforstock = index;
			}
			else if (zen == Hkana[56][0]) {
				//���_�V
				//index = beforstock - Hkana[0][0];//���_�O�̕���
				
				for (int h = 0; h < 3; h++) {//6�o�C�g��3�o�C�g��
					after[j - 5 + h] = ((Hkana[beforstock][1] + 1) >> (16 - (8 * h))) & 0xFF;
				}
				j -= 3;
			}
			else if (zen == Hkana[57][0]) {
				//���_�Z
				index = beforstock - Hkana[0][0];//���_�O�̕���
				for (int h = 0; h < 3; h++) {
					after[j - 5 + h] = ((Hkana[index][1] + 2) >> (16 - (8 * h))) & 0xFF;
				}
				j -= 3;
			}
			else {//
				after[j] = s[i];
			}
			
			j++; i++;
		}
		after[j] = '\0';
	}
	free(s);
	return after;
}

UINT8* unitC::changenumber(UINT8* c)
{
	size_t m = 0;
	UINT32 zen = 0;
	UINT32 index = 0;
	UINT32 sharp = 0xEFBC83;
	//UINT32 beforstock = 0;

	//�������m�F
	while (c[m] != '\0')
		m++;

	int i = 0;
	int j = 0;

	UINT8* after = (UINT8*)malloc(m + 1);

	//�S�p�p���T�[�`
	if (m > 2) {
		while (c[i] != '\0') {
			zen = 0;
			if (i > 2) {
				zen += (c[i - 2]) << 16;
				zen += (c[i - 1]) << 8;
				zen += c[i];
				zen = zen & (0xFFFFFF);
			}

			if (zen >= zenkakuAlpaK[0] && zen <= zenkakuAlpaK[25]) {//�S�p������
				index = zen - zenkakuAlpaK[0];
				//2�O�߂� ���
				after[j - 2] = utfAlphaK[index];//���p �p������
				j-=3;
			}
			else if (zen >= zenkakuAlpaO[0] && zen <= zenkakuAlpaO[25]) {//�S�p�啶��
				index = zen - zenkakuAlpaO[0];
				//2�O�߂� ���
				after[j - 2] = utfAlphaO[index];//���p �p�啶��
				j-=3;
			}
			else if (zen >= zenkakuNum[0] && zen <= zenkakuNum[9]) {//�S�p����
				index = zen - zenkakuNum[0];
				//2�O�߂� ���
				after[j-2] = utfNum[index];//���p����
				j-=3;
			}
			else if (zen == sharp) {//�S�p���Ƃ΂�
				j -= 3;
			}
			else {//���p#�Ƃ΂�
				//if (c[i] != '#' && c[i] != '\n' && c[i] != ' ' && c[i] != '�@')
					after[j] = c[i];
			}
			//beforstock = zen;
			j++; i++;
		}
		after[j] = '\0';
	}

	free(c);
	
	return after;
}

UINT8* unitC::slipNum(UINT8* s)
{
	//�����J�E���g�@3�ȏ�  �폜
	//#�@�폜
	int i = 0;
	bool flg = false;
	int count = 0;//������
	int Sc = 0;//�����J�n�ʒu
	int Ec = 0;//�I���ʒu
	int j = 0;
	bool f = false;

	while (s[i] != '\0')
		i++;
	const UINT32 msize = (UINT32)i + 1;
	UINT8* after=(UINT8*)malloc(msize);
	UINT8* more = (UINT8*)malloc(msize);

	Num = (NumberFlag*)malloc(sizeof(NumberFlag));
	Num = nullptr;

	i = 0;

	if (more && after) {
		while (s[i] != '\0') {
			if (s[i] >= 0x30 && s[i] <= 0x39)
			{
				Sc = i;
				if (s[i + 1] >= 0x30 && s[i + 1] <= 0x39 && s[i + 1] != '\0') {
					while (s[i] >= 0x30 && s[i] <= 0x39 && s[i] != '\0')
					{
						i++;
					}
					if (i - Sc > 2) {
						Num = addnum(Sc, i, Num);//����3�ȏ�@�\���̒ǉ�
					}
				}
				else {
					i++;
				}
			}
			else {
				i++;
			}
		}
		//size_t a = (size_t)i + 1;
		//after = (UINT8*)malloc(i+1);

		i = 0;
		j = 0;

		if (Num) {
			while (Num) {
				while (Num->start > i && s[i] != '\0') {//�����A���X�^�[�g�ʒu�܂�
					if (s[i] != '\n') {
						more[j] = s[i];
						i++; j++;
					}
					else {
						i++;
					}
				}
				i = (Num->End);//�I���ʒu�܂łƂ΂�			

				Num = Num->next;//��
			}

			while (s[j] != '\0') {//�r���ŏI���p
				if (s[i] != 0xd && s[i] != 0xa && s[i] != '&') {
					more[j] = s[i];
					i++; j++;
				}
				else {
					i++;
				}
			}
			if (more)
				more[j] = '\0';
			int morelen = j;

			//&#10; ����
			i = 0; j = 0;
			while (more[i] != '\0') {
				if (more[i] == '&' && (morelen - i) > 4) {
					if (more[i + 1] == '#' && more[i + 2] == '1' && more[i + 3] == '0' && more[i + 4] == ';')
						i += 5;
					else if (more[i] != '#' && more[i] != ' ' && more[i] != '�@') {
						after[j] = more[i];
						i++; j++;
					}
					else {
						i++;
					}
				}
				else if (more[i] != '#' && more[i] != ' ' && more[i] != '�@') {
					after[j] = more[i];
					i++; j++;
				}
				else {
					i++;
				}
			}
		}
		else {
			return s;
		}
		
		
		after[j] = '\0';
	}
	free(Num);
	free(more);

	return after;
}

NumberFlag* unitC::addnum(int s, int e, NumberFlag* n)
{
	if (!n) {
		n = (NumberFlag*)malloc(sizeof(NumberFlag));
		if (n) {
			n->start = s;
			n->End = e;
			n->next = nullptr;
		}
	}
	else {
		n->next = addnum(s, e, n->next);
	}

	return n;
}

