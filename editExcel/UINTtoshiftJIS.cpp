#include"SearchItemNUm.h"
#include<Windows.h>

char* searchItemNum::CharChenge(UINT8* cc) {
    char* col = (char*)cc;

    std::string st = std::string(col);
    //stockcolor = splitcolor;
    //�����Q��
    //Unicode�֕ϊ���̕����񒷂𓾂�
    int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, st.c_str(), st.size() + 1, NULL, 0);

    //�K�v�ȕ�����Unicode������̃o�b�t�@���m��
    wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

    //string str((istreambuf_iterator<char>(splitcolor)), istreambuf_iterator<char>());
    MultiByteToWideChar(CP_UTF8, 0, st.c_str(), st.size() + 1, bufUnicode, lenghtUnicode);

    //ShiftJIS�֕ϊ���̕����񒷂𓾂�
    int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

    //�K�v�ȕ�����ShiftJIS������̃o�b�t�@���m��
    char* bufShiftJis = new char[lengthSJis];

    //Unicode����ShiftJIS�֕ϊ�
    WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

    std::string strSJis(bufShiftJis);
    return bufShiftJis;
}