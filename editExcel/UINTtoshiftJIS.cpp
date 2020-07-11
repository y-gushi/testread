#include"SearchItemNUm.h"
#include<Windows.h>

char* searchItemNum::CharChenge(UINT8* cc) {
    char* col = (char*)cc;

    std::string st = std::string(col);
    //stockcolor = splitcolor;
    //次を参照
    //Unicodeへ変換後の文字列長を得る
    int lenghtUnicode = MultiByteToWideChar(CP_UTF8, 0, st.c_str(), st.size() + 1, NULL, 0);

    //必要な分だけUnicode文字列のバッファを確保
    wchar_t* bufUnicode = new wchar_t[lenghtUnicode];

    //string str((istreambuf_iterator<char>(splitcolor)), istreambuf_iterator<char>());
    MultiByteToWideChar(CP_UTF8, 0, st.c_str(), st.size() + 1, bufUnicode, lenghtUnicode);

    //ShiftJISへ変換後の文字列長を得る
    int lengthSJis = WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, -1, NULL, 0, NULL, NULL);

    //必要な分だけShiftJIS文字列のバッファを確保
    char* bufShiftJis = new char[lengthSJis];

    //UnicodeからShiftJISへ変換
    WideCharToMultiByte(CP_THREAD_ACP, 0, bufUnicode, lenghtUnicode + 1, bufShiftJis, lengthSJis, NULL, NULL);

    std::string strSJis(bufShiftJis);
    return bufShiftJis;
}