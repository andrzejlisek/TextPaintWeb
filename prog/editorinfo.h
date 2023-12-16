#ifndef EDITORINFO_H
#define EDITORINFO_H

#include <string>
#include "xlist.h"

class EditorInfo
{
public:
    EditorInfo();
    int InfoX = 0;
    int InfoY = 0;
    int InfoW = 0;
    int InfoH = 0;
    XList<std::string> Info;
    void CreateInfo(int N);
    int Move(int Dir);
private:
    XList<std::string> Info0;
    XList<std::string> Info1;
    XList<std::string> Info2;
    XList<std::string> Info3;
    XList<std::string> Info4;
    XList<std::string> Info9;
};

#endif // EDITORINFO_H
