#ifndef ANSIFILE_H
#define ANSIFILE_H

#include "str.h"
#include "ansilineoccupyex.h"
#include <string>
#include "screen.h"
#include "corestatic.h"

class AnsiFile
{
public:
    AnsiFile();
    void Reset();
    Str Process(AnsiLineOccupyEx TextBuffer, int TextBufferI, int AnsiMaxX);
private:
    int LastB = -1;
    int LastF = -1;
    int LastA = -1;
    int LastFontW = 0;
    int LastFontH = 0;

};

#endif // ANSIFILE_H
