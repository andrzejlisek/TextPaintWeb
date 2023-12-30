#include "screen.h"

Screen::Screen()
{

}

void Screen::AddDoubleRange(int X1, int X2)
{
    if (X1 < 0)
    {
        Range1.Clear();
        Range2.Clear();
    }
}

void Screen::StaticInit()
{
    for (int i = 0; i < CharDoubleTableLength; i++)
    {
        CharDoubleTable[i] = 0;
        CharDoubleTableInv[i] = 0;
    }

    // !!!!!!!! Dopisac tworzenie podwojnych znakow

    // !!!!!! Sprawdzic !!!
    /*CharDoubleTableInv[BlankDoubleChar] = BlankDoubleChar;
    for (int i = 0; i < CharDoubleTableLength; i++)
    {
        if ((CharDoubleTable[i] >= 0) && (CharDoubleTable[i] != BlankDoubleChar))
        {
            CharDoubleTableInv[CharDoubleTable[i]] = i;
        }
    }*/

}

int Screen::CharDouble(int C)
{
    return CharDoubleTable[C];
}

int Screen::CharDoubleInv(int C)
{
    if (C != BlankDoubleChar)
    {
        return CharDoubleTableInv[C];
    }
    else
    {
        return BlankDoubleCharVis;
    }
}

void Screen::ScreenClear(int Back, int Fore)
{
    int Area = ScreenMemo1.Count;
    for (int I = 0; I < Area; I++)
    {
        ScreenMemo1[I] = -1;
        ScreenMemo2[I] = -1;
    }
    ScreenClear_(Back, Fore);
}

void Screen::ScreenChar(int X, int Y, int C, int ColorBack, int ColorFore, int ColorAttr, int FontW, int FontH)
{
    if (X < 0) return;
    if (Y < 0) return;
    if (X >= CurrentW) return;
    if (Y >= CurrentH) return;
    int Ptr = Y * CurrentW + X;
    int Memo1 = (C << 8) + (ColorAttr);
    int Memo2 = ((ColorBack >= 0 ? ColorBack : 32) << 6) + (ColorFore >= 0 ? ColorFore : 32) + (FontW << 12) + (FontH << 21);
    if ((ScreenMemo1[Ptr] != Memo1) || (ScreenMemo2[Ptr] != Memo2))
    {
        ScreenMemo1[Ptr] = Memo1;
        ScreenMemo2[Ptr] = Memo2;
        ScreenChar_(X, Y, C, ColorBack, ColorFore, ColorAttr, FontW, FontH);
    }
}

void Screen::ScreenTextMove(int X1, int Y1, int X2, int Y2, int W, int H)
{
    int YStart = Y2;
    int YStop = Y2 + H;
    int YAdv = 1;
    int YDelta = Y1 - Y2;

    if (Y1 < Y2)
    {
        YStart = Y1 + H;
        YStop = Y1;
        YAdv = -1;
        YDelta = Y1 - Y2;
    }

    int XStart = X2;
    int XStop = X2 + W;
    int XAdv = 1;
    int XDelta = X1 - X2;

    if (X1 < X2)
    {
        XStart = X1 + W;
        XStop = X1;
        XAdv = -1;
        XDelta = X1 - X2;
    }

    for (int Y = YStart; Y != YStop; Y += YAdv)
    {
        for (int X = XStart; X != XStop; X += XAdv)
        {
            int ScrIdx0 = (Y) * CurrentW + (X);
            int ScrIdx1 = (Y + YDelta) * CurrentW + (X + XDelta);
            ScreenMemo1[ScrIdx0] = ScreenMemo1[ScrIdx1];
            ScreenMemo2[ScrIdx0] = ScreenMemo2[ScrIdx1];
        }
    }
    ScreenTextMove_(X1, Y1, X2, Y2, W, H);
}

void Screen::ScreenResize(int NewW, int NewH)
{
    ScreenMemo1.Clear();
    ScreenMemo2.Clear();
    int Area = NewW * NewH;
    while (Area > 0)
    {
        ScreenMemo1.Add(-1);
        ScreenMemo2.Add(-1);
        Area--;
    }
    CurrentW = NewW;
    CurrentH = NewH;
    ScreenResize_(NewW, NewH);
}

void Screen::ScreenCursorMove(int X, int Y)
{
    if (X < 0) X = 0;
    if (Y < 0) Y = 0;
    if (X >= CurrentW) X = CurrentW - 1;
    if (Y >= CurrentH) Y = CurrentH - 1;
    CurrentX = X;
    CurrentY = Y;
}

void Screen::ScreenRefresh()
{
    ScreenCursorMove_(CurrentX, CurrentY);
}

void Screen::ScreenWriteChar(int Chr)
{
    Screen::ScreenWriteChar(Chr, TextNormalBack, TextNormalFore);
}

void Screen::ScreenWriteChar(int Chr, int Back, int Fore)
{
    ScreenChar(CurrentX, CurrentY, Chr, Back, Fore, 0, 0, 0);
    CurrentX++;
    if (CurrentX >= CurrentW)
    {
        ScreenWriteLine(Back, Fore);
    }
}

void Screen::ScreenWriteChar0(int Chr)
{
    Screen::ScreenWriteChar0(Chr, TextNormalBack, TextNormalFore);
}

void Screen::ScreenWriteChar0(int Chr, int Back, int Fore)
{
    ScreenChar(CurrentX, CurrentY, Chr, Back, Fore, 0, 0, 0);
    CurrentX++;
    if (CurrentX >= CurrentW)
    {
        CurrentX = CurrentW - 1;
    }
}

void Screen::ScreenWriteText(Str Txt)
{
    Screen::ScreenWriteText(Txt, TextNormalBack, TextNormalFore);
}

void Screen::ScreenWriteText(Str Txt, int Back, int Fore)
{
    for (int I = 0; I < Txt.Count; I++)
    {
        ScreenWriteChar(Txt[I], Back, Fore);
    }
}

void Screen::ScreenWriteText0(Str Txt)
{
    Screen::ScreenWriteText0(Txt, TextNormalBack, TextNormalFore);
}

void Screen::ScreenWriteText0(Str Txt, int Back, int Fore)
{
    for (int I = 0; I < Txt.Count; I++)
    {
        ScreenWriteChar0(Txt[I], Back, Fore);
    }
}

void Screen::ScreenWriteLine()
{
    Screen::ScreenWriteLine(TextNormalBack, TextNormalFore);
}

void Screen::ScreenWriteLine(int Back, int Fore)
{
    CurrentX = 0;
    CurrentY++;
    if (CurrentY >= CurrentH)
    {
        ScreenTextMove(0, 1, 0, 0, CurrentW, CurrentH - 1);
        CurrentY = CurrentH - 1;
        for (int I = 0; I < CurrentW; I++)
        {
            ScreenChar(I, CurrentY, 32, Back, Fore, 0, 0, 0);
        }
    }
}

void Screen::MouseActive(bool X)
{

}

int Screen::FileImport(int Kind, std::string Name, std::string Attrib)
{
    FilePortId++;
    FileImport_(FilePortId, Kind, Name, Attrib);
    return FilePortId;
}

int Screen::FileExport(int Kind, std::string Name, std::string Attrib, std::string Data)
{
    FilePortId++;
    FileExport_(FilePortId, Kind, Name, Attrib, Data);
    return FilePortId;
}

int Screen::DefaultW(int ScrW, int ANSIDOS)
{
    if (ScrW <= 0)
    {
        return 80;
    }
    else
    {
        return ScrW;
    }
}

int Screen::DefaultH(int ScrH, int ANSIDOS)
{
    if (ScrH <= 0)
    {
        if (ANSIDOS == 1)
        {
            return 25;
        }
        else
        {
            return 24;
        }
    }
    else
    {
        return ScrH;
    }
}
