#ifndef XBIN_H
#define XBIN_H

#include "raw.h"
#include "str.h"
#include "textcodec.h"
#include <iostream>

class XBIN
{
public:
    XBIN();
    void SetRaw(Raw &Raw_, int RawL, bool IsXBIN);
    void GetStr(Str &Str_, int CodecN);

    int FileW = 65536;
    int FileH = 65536;
    bool DataCompression = false;
    int FontN = 0;
    int FontSize = 0;
    int DataRawLength = 0;
    Str DataPal;
private:
    Raw DataRaw;
    Raw DataFont1;
    Raw DataFont2;
};

#endif // XBIN_H
