#include <windows.h>
#include <iostream>

int main() {
#ifdef WIN32
    LPWSTR *szArglist;
    int argc;
    szArglist = CommandLineToArgvW(GetCommandLineW(),&argc);
    if(NULL==szArglist) {
       std::cerr << "CommandLineToArgvW failed\n";
    }
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>,wchar_t> convert; // codecvt_utf8 or codecvt<char16_t,char,mbstate_t> should work (will work once the char16_t specialization of codecvt works)
    vector<string> args;
    for(int i=0;i<argc;++i) {
        args.push_back(convert.to_bytes(szArglist[i]));
    }
#endif //ifdef WIN32

}