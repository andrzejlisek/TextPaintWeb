#include <iostream>
#include <emscripten/emscripten.h>
#include <string>
#include <memory>

#define BufSizeStep 500
#define BufSizeLimit 500000

char * IOBuf;
int IOBufSize = 0;
int IOBufPtr = 0;
bool IOBufInit = true;
bool IOBufLimit = false;

char BufDigits[10];

void BufInit()
{
    IOBufSize = (BufSizeStep + BufSizeStep);
    IOBuf = (char*)malloc(IOBufSize);
}

void BufExpand()
{
    char * IOBuf_ = IOBuf;
    IOBuf = (char*)malloc(IOBufSize + (BufSizeStep + BufSizeStep));
    memcpy(IOBuf, IOBuf_, IOBufSize);
    free(IOBuf_);
    IOBufSize += (BufSizeStep + BufSizeStep);
    if (IOBufSize >= BufSizeLimit)
    {
        IOBufLimit = true;
    }
}

void BufClear()
{
    IOBufPtr = 0;
    if (IOBufLimit)
    {
        IOBufLimit = false;
        free(IOBuf);
        IOBufSize = (BufSizeStep + BufSizeStep);
        //IOBufSize -= (BufSizeStep + BufSizeStep);
        IOBuf = (char*)malloc(IOBufSize);
    }
}

bool BufCheck()
{
    if (IOBufPtr > (IOBufSize - BufSizeStep))
    {
        BufExpand();
    }
    return IOBufLimit;
}

void BufChr(int Chr)
{
    IOBuf[IOBufPtr] = Chr;
    IOBufPtr++;
}

void BufNum(int Num)
{
    if (Num < 0)
    {
        IOBuf[IOBufPtr] = '-';
        IOBufPtr++;
        Num = 0 - Num;
    }
    if (Num == 0)
    {
        IOBuf[IOBufPtr] = '0';
        IOBufPtr++;
    }
    int Digits = 0;
    int Num0 = Num;
    while (Num0 > 0)
    {
        BufDigits[Digits] = (Num0 % 10);
        Num0 = Num0 / 10;
        Digits++;
    }
    while (Digits > 0)
    {
        Digits--;
        IOBuf[IOBufPtr] = BufDigits[Digits] + 48;
        IOBufPtr++;
    }
    IOBuf[IOBufPtr] = ',';
    IOBufPtr++;
}

void BufTxt(const char * Txt)
{
    int TxtL = strlen(Txt);
    while (IOBufSize <= (IOBufPtr + TxtL))
    {
        BufExpand();
    }
    memcpy(IOBuf + IOBufPtr, Txt, TxtL);
    IOBufPtr += TxtL;
}

void BufStr(std::string Str)
{
    int StrL = Str.size();
    while (IOBufSize <= (IOBufPtr + StrL))
    {
        BufExpand();
    }
    for (int I = 0; I < StrL; I++)
    {
        char Chr = Str[I];
        if ((Chr == '_') || (Chr == ',') || (Chr > 126) || (Chr < 32))
        {
            IOBuf[IOBufPtr] = '_';
            IOBufPtr++;
            if (Chr >= 0)
            {
                BufNum(Chr);
            }
            else
            {
                BufNum(Chr + 256);
            }
        }
        else
        {
            IOBuf[IOBufPtr] = Chr;
            IOBufPtr++;
        }
    }
}

int BufGetNum()
{
    int N = 0;
    while (IOBuf[IOBufPtr] != ',')
    {
        N = N * 10;
        N = N + (IOBuf[IOBufPtr] - 48);
        IOBufPtr++;
    }
    IOBufPtr++;
    return N;
}

std::string BufGetStr()
{
    std::string S = "";
    while (IOBuf[IOBufPtr] != ',')
    {
        char Chr = IOBuf[IOBufPtr];
        if (Chr == '_')
        {
            IOBufPtr++;
            int ChrN = BufGetNum();
            std::cout << "{" << ChrN << "}" << std::endl;
            S.push_back((char)ChrN);
        }
        else
        {
            S.push_back(Chr);
            IOBufPtr++;
        }
    }
    IOBufPtr++;
    return S;
}

void BufSet(char * data)
{
    strcpy(IOBuf, data);
    IOBufPtr = 0;
}

