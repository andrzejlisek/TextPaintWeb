#include "ansilineoccupy.h"

AnsiLineOccupy::AnsiLineOccupy()
{
    Data.Clear();
}

void AnsiLineOccupy::Clear()
{
    Data.Clear();
}

void AnsiLineOccupy::Append(int Y)
{
    Data[Y].Add(Item_Char);
    Data[Y].Add(Item_ColorB);
    Data[Y].Add(Item_ColorF);
    Data[Y].Add(Item_ColorA + (Item_Type << 8));
    Data[Y].Add(Item_FontW);
    Data[Y].Add(Item_FontH);
}

void AnsiLineOccupy::Insert(int Y, int X)
{
    Data[Y].Insert(X * Factor, Item_FontH);
    Data[Y].Insert(X * Factor, Item_FontW);
    Data[Y].Insert(X * Factor, Item_ColorA + (Item_Type << 8));
    Data[Y].Insert(X * Factor, Item_ColorF);
    Data[Y].Insert(X * Factor, Item_ColorB);
    Data[Y].Insert(X * Factor, Item_Char);
}

void AnsiLineOccupy::Delete(int Y, int X)
{
    Data[Y].RemoveRange(X * Factor, Factor);
}

void AnsiLineOccupy::AppendLine()
{
    Str X;
    Data.Add(X);
}

void AnsiLineOccupy::InsertLine(int I)
{
    Str X;
    Data.Insert(I, X);
}

void AnsiLineOccupy::AppendLine(AnsiLineOccupy X, int I)
{
    Data.Add(X.Data[I]);
}

void AnsiLineOccupy::InsertLine(AnsiLineOccupy X, int I, int II)
{
    Data.Insert(II, X.Data[I]);
}

void AnsiLineOccupy::DeleteLine(int I)
{
    Data.Remove(I);
}

void AnsiLineOccupy::Get(int Y, int X)
{
    Item_Char = Data[Y][X * Factor + 0];
    Item_ColorB = Data[Y][X * Factor + 1];
    Item_ColorF = Data[Y][X * Factor + 2];
    Item_ColorA = Data[Y][X * Factor + 3] & 255;
    Item_Type = Data[Y][X * Factor + 3] >> 8;
    Item_FontW = Data[Y][X * Factor + 4];
    Item_FontH = Data[Y][X * Factor + 5];
}

void AnsiLineOccupy::Set(int Y, int X)
{
    Data[Y][X * Factor + 0] = Item_Char;
    Data[Y][X * Factor + 1] = Item_ColorB;
    Data[Y][X * Factor + 2] = Item_ColorF;
    Data[Y][X * Factor + 3] = Item_ColorA + (Item_Type << 8);
    Data[Y][X * Factor + 4] = Item_FontW;
    Data[Y][X * Factor + 5] = Item_FontH;
}

int AnsiLineOccupy::CountLines()
{
    return Data.Count;
}

int AnsiLineOccupy::CountItems(int I)
{
    return Data[I].Count / Factor;
}

void AnsiLineOccupy::Copy(AnsiLineOccupy &Src, AnsiLineOccupy &Dst)
{
    Dst.Data.Clear();
    for (int i = 0; i < Src.Data.Count; i++)
    {
        Str Temp;
        for (int ii = 0; ii < Src.Data[i].Count; ii++)
        {
            Temp.Add(Src.Data[i][ii]);
        }
        Dst.Data.Add(Temp);
    }
}

void AnsiLineOccupy::Swap(AnsiLineOccupy &Src, AnsiLineOccupy &Dst)
{
    AnsiLineOccupy Temp;
    Copy(Dst, Temp);
    Copy(Src, Dst);
    Copy(Temp, Src);
}
