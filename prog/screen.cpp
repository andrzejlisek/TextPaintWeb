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

void Screen::ScreenResize(int NewW, int NewH)
{
    CurrentW = NewW;
    CurrentH = NewH;
    ScreenResize_(NewW, NewH);
}

void Screen::ScreenCursorMove(int X, int Y)
{
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
