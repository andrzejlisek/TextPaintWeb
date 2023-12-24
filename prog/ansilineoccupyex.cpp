#include "ansilineoccupyex.h"

AnsiLineOccupyEx::AnsiLineOccupyEx()
{

}

int AnsiLineOccupyEx::GetDataChar(int Y, int X)
{
    Get(Y, X);
    return Item_Char;
}

int AnsiLineOccupyEx::GetDataColo(int Y, int X)
{
    Get(Y, X);
    return Item_ColorF;
}

int AnsiLineOccupyEx::GetDataAttr(int Y, int X)
{
    Get(Y, X);
    return Item_ColorA;
}

int AnsiLineOccupyEx::GetDataFont(int Y, int X)
{
    return 0;
}

void AnsiLineOccupyEx::Set_(int Y, int X)
{
    while(Data.Count <= Y)
    {
        Str _;
        Data.Add(_);
    }
    PadRightSpace(Y, X + 1);
    Set(Y, X);
}

bool AnsiLineOccupyEx::Get_(int Y, int X)
{
    BlankChar();
    if ((Data.Count > Y) && (Y >= 0))
    {
        if ((Data[Y].Count > (X * Factor)) && (X >= 0))
        {
            Get(Y, X);
            return true;
        }
    }
    return false;
}

int AnsiLineOccupyEx::CountItemsTrim(int Y)
{
    ItemTempSave();
    int I = CountItems(Y);
    bool Work = (I > 0);
    if (Work)
    {
        Get(Y, I - 1);
        Work = ((Str::SpaceChars.Contains(Item_Char)) && (Item_ColorB < 0) && (Item_ColorF < 0) && (Item_ColorA == 0));
    }
    while (Work)
    {
        I--;
        Work = (I > 0);
        if (Work)
        {
            Get(Y, I - 1);
            Work = ((Str::SpaceChars.Contains(Item_Char)) && (Item_ColorB < 0) && (Item_ColorF < 0) && (Item_ColorA == 0));
        }
    }
    ItemTempLoad();
    return I;
}

void AnsiLineOccupyEx::Trim(int Y)
{
    ItemTempSave();
    int X = CountItems(Y) - 1;
    if (X >= 0)
    {
        Get(Y, X);
        while ((X >= 0) && (Str::SpaceChars.Contains(Item_Char)) && (Item_ColorB < 0) && (Item_ColorF < 0) && (Item_ColorA == 0))
        {
            Delete(Y, X);
            X--;
            if (X < 0)
            {
                break;
            }
            Get(Y, X);
        }
    }
    ItemTempLoad();
}

void AnsiLineOccupyEx::TrimLines()
{
    int L = CountLines();
    int Y = 0;
    for (Y = 0; Y < L; Y++)
    {
        Trim(Y);
    }
    Y = L - 1;
    while (Y >= 0)
    {
        if (CountItems(Y) == 0)
        {
            DeleteLine(Y);
            Y--;
        }
        else
        {
            break;
        }
    }
}

void AnsiLineOccupyEx::DeleteAll()
{

}

Str AnsiLineOccupyEx::GetLineString(int Y)
{
    Str Temp;
    for (int X = 0; X < CountItems(Y); X++)
    {
        Get(Y, X);
        Temp.Add(Item_Char);
    }
    return Temp;
}

void AnsiLineOccupyEx::SetLineString(int Y, Str Text)
{
    Data[Y].Clear();
    BlankChar();
    for (int i = 0; i < Text.Count; i++)
    {
        Item_Char = Text[i];
        Append(Y);
    }
}

void AnsiLineOccupyEx::SetLineString(int Y, std::string Text)
{
    Data[Y].Clear();
    Str Text_ = Str::FromString(Text);
    BlankChar();
    for (int i = 0; i < Text_.Count; i++)
    {
        Item_Char = Text_[i];
        Append(Y);
    }
}

void AnsiLineOccupyEx::ClearLine(int Y)
{
    Data[Y].Clear();
}

void AnsiLineOccupyEx::ItemTempSave()
{
    ItemTemp.Add(Item_Char);
    ItemTemp.Add(Item_ColorB);
    ItemTemp.Add(Item_ColorF);
    ItemTemp.Add(Item_ColorA);
    ItemTemp.Add(Item_FontW);
    ItemTemp.Add(Item_FontH);
}

void AnsiLineOccupyEx::ItemTempLoad()
{
    Item_FontH = ItemTemp.PopLast();
    Item_FontW = ItemTemp.PopLast();
    Item_ColorA = ItemTemp.PopLast();
    Item_ColorF = ItemTemp.PopLast();
    Item_ColorB = ItemTemp.PopLast();
    Item_Char = ItemTemp.PopLast();
}

void AnsiLineOccupyEx::PadRightTab(int Y, int Size)
{
    ItemTempSave();
    BlankChar();
    Item_Char = '\t';
    int CountF = CountItems(Y);
    while (CountF < Size)
    {
        Append(Y);
        CountF++;
    }
    ItemTempLoad();
}

void AnsiLineOccupyEx::PadRightSpace(int Y, int Size)
{
    ItemTempSave();
    BlankChar();
    int CountF = CountItems(Y);
    while (CountF < Size)
    {
        Append(Y);
        CountF++;
    }
    ItemTempLoad();
}

void AnsiLineOccupyEx::PadRight(int Y, int Size)
{
    int CountF = CountItems(Y);
    while (CountF < Size)
    {
        Append(Y);
        CountF++;
    }
}

void AnsiLineOccupyEx::DeleteLeft(int Y, int Size)
{
    Size = Size * Factor;
    if (Data[Y].Count > (Size))
    {
        Data[Y].RemoveRange(0, Size);
    }
    else
    {
        Data[Y].Clear();
    }
}

void AnsiLineOccupyEx::Crop(int Y, int Start, int Size)
{
    Start = Start * Factor;
    Size = Size * Factor;

    if (Data[Y].Count > (Start + Size))
    {
        Data[Y].RemoveRange(Start + Size, Data[Y].Count - (Start + Size));
    }
    if (Start > 0)
    {
        if (Data[Y].Count > Start)
        {
            Data[Y].RemoveRange(0, Start);
        }
        else
        {
            Data[Y].Clear();
        }
    }
}

void AnsiLineOccupyEx::LineCopy(AnsiLineOccupyEx Src, int SrcY, int DstY)
{
    Data[DstY] = Src.Data[SrcY];
    //Str Temp;
    //Temp.AddRange();
    /*Data[DstY].Clear();
    for (int i = 0; i < Src.Data[SrcY].Count; i++)
    {
        Data[DstY].Add(Src.Data[SrcY][i]);
    }*/
}

void AnsiLineOccupyEx::AppendLineCopy(AnsiLineOccupyEx Src, int SrcY)
{
    Str Temp;
    for (int i = 0; i < Src.Data[SrcY].Count; i++)
    {
        Temp.Add(Src.Data[SrcY][i]);
    }
    Data.Add(Temp);
}

void AnsiLineOccupyEx::Insert(int Y, int X, int L)
{
    Str DataX;
    DataX.Add(Item_Char);
    DataX.Add(Item_ColorB);
    DataX.Add(Item_ColorF);
    DataX.Add(Item_ColorA);
    DataX.Add(Item_FontW);
    DataX.Add(Item_FontH);
    while (L > 0)
    {
        Data[Y].InsertRange(X * Factor, DataX);
        L--;
    }
}

void AnsiLineOccupyEx::Insert(int Y, int X, AnsiLineOccupyEx Obj, int ObjY)
{
    Data[Y].InsertRange(X * Factor, Obj.Data[ObjY]);
}

void AnsiLineOccupyEx::Append(int Y, int L)
{
    Str DataX;
    DataX.Add(Item_Char);
    DataX.Add(Item_ColorB);
    DataX.Add(Item_ColorF);
    DataX.Add(Item_ColorA);
    DataX.Add(Item_FontW);
    DataX.Add(Item_FontH);
    while (L > 0)
    {
        Data[Y].AddRange(DataX);
        L--;
    }
}

void AnsiLineOccupyEx::Append(int Y, AnsiLineOccupyEx Obj, int ObjY)
{
    Data[Y].AddRange(Obj.Data[ObjY]);
}

void AnsiLineOccupyEx::Delete(int Y, int X, int L)
{
    Data[Y].RemoveRange(X * Factor, L * Factor);
}

AnsiLineOccupyEx AnsiLineOccupyEx::CloneData()
{
    AnsiLineOccupyEx Obj;
    for (int i = 0; i < Data.Count; i++)
    {
        Str DataItem;
        for (int ii = 0; ii < Data[i].Count; ii++)
        {
            DataItem.Add(Data[i][ii]);
        }
        Obj.Data.Add(DataItem);
    }
    Obj.CopyItem(*this);
    return Obj;
}

void AnsiLineOccupyEx::MergeColor(AnsiLineOccupyEx ColorObj)
{
    ColorObj.TrimLines();
}
