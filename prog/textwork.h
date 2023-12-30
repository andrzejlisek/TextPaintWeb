#ifndef TEXTWORK_H
#define TEXTWORK_H

#include "xlist.h"
#include <string>
#include <iostream>
#include <sstream>
#include "hex.h"
#include "textcodec.h"
#include "str.h"

class TextWork
{
public:
    static std::string NumPlusMinus(long Num);
    /*static int CodeChar(std::string C);*/
    static std::string CharPrint(int C);
    static std::string CharToStr(int C);

    static std::string CharCode(int C, int Mode);
    TextWork();
    static std::string LTrim(std::string S);
    static std::string RTrim(std::string S);
    static std::string Trim(std::string S);
    static std::string LPad(std::string S, int N, std::string Chr);
    static std::string RPad(std::string S, int N, std::string Chr);

    static int StringSplit(std::string Str, char Chr, std::string * Array);
    static int StringSplit(std::string Str, char Chr, XList<std::string> &Array);
    static std::string StringReplace(std::string Str, std::string From, std::string To);
    static int CodeChar(std::string C);

    static void StringToLines(std::string S, XList<std::string> &Array);
    static void StringToLines(Str S, XList<Str> &Array);

    static bool StringStartsWith(std::string S, std::string X);
    static bool StringEndsWith(std::string S, std::string X);
    static bool StringStartsEndsWith(std::string S, std::string X1, std::string X2);

    static int StrToInt(std::string S, int Default);
    static long long StrToLong(std::string S, long long Default);
};

#endif // TEXTWORK_H
