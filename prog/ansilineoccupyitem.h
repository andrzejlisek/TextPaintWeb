#ifndef ANSILINEOCCUPYITEM_H
#define ANSILINEOCCUPYITEM_H

#include "str.h"

class AnsiLineOccupyItem
{
    // Attribute bits:
    // 1. Bold             01  FE
    // 2. Italic           02  FD
    // 4. Underline        04  FB
    // 8. Blink            08  F7
    // 16. Reverse         10  EF
    // 32. Concealed       20  DF
    // 64. Strikethrough   40  BF
    // 128. ScreenNegate   80  7F
public:
    AnsiLineOccupyItem();
    int Item_Char;
    int Item_ColorB;
    int Item_ColorF;
    int Item_ColorA;
    int Item_FontW;
    int Item_FontH;
    int Item_Type;
    void BlankChar();
    void BlankChar(int ColorB_, int ColorF_, int ColorA_);
    void CopyItem(AnsiLineOccupyItem &Src);
    AnsiLineOccupyItem CopyItemObj();
};

#endif // ANSILINEOCCUPYITEM_H
