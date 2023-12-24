#include "ansilineoccupyitem.h"

AnsiLineOccupyItem::AnsiLineOccupyItem()
{
    BlankChar();
}

void AnsiLineOccupyItem::BlankChar()
{
    Item_Char = Str::SpaceChar0;
    Item_ColorB = -1;
    Item_ColorF = -1;
    Item_ColorA = 0;
    Item_FontW = 0;
    Item_FontH = 0;
}

void AnsiLineOccupyItem::BlankChar(int ColorB_, int ColorF_, int ColorA_)
{
    Item_Char = Str::SpaceChar0;
    Item_ColorB = ColorB_;
    Item_ColorF = ColorF_;
    Item_ColorA = ColorA_;
    Item_FontW = 0;
    Item_FontH = 0;
}

void AnsiLineOccupyItem::CopyItem(AnsiLineOccupyItem &Src)
{
    Item_Char = Src.Item_Char;
    Item_ColorB = Src.Item_ColorB;
    Item_ColorF = Src.Item_ColorF;
    Item_ColorA = Src.Item_ColorA;
    Item_FontW = Src.Item_FontW;
    Item_FontH = Src.Item_FontH;
}

AnsiLineOccupyItem AnsiLineOccupyItem::CopyItemObj()
{
    AnsiLineOccupyItem Obj;
    Obj.CopyItem(*this);
    return Obj;
}
