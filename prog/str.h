#ifndef STR_H
#define STR_H

#include "xlist.h"
#include <string>
#include <sstream>
#include "hex.h"

class Str : public XList<unsigned int>
{
public:
    inline static int SpaceChar0;
    inline static XList<int> SpaceChars;

    Str();
    Str(std::string T);

    Str Copy();
    int TrimEndLenCol();
    int TrimEndLength();
    Str TrimEnC();
    Str TrimEnd();
    Str Substring(int Pos, int Count_);

    void AddString(int T);
    void AddString(std::string T);

    std::string ToString();

    static Str FromString(std::string T);
    static Str FromString(int I);
    static Str CodeToInt(std::string STR);
    static std::string IntToStr(int I);
    static Str BlkCol(int I);
    static Str Spaces(int I);
};

#endif // STR_H
