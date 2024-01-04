#include "coreansi.h"

void CoreAnsi::AnsiGetF(int X, int Y)
{
    if (AnsiGetFontSize(Y) > 0)
    {
        AnsiGet(X * 2, Y);
    }
    else
    {
        AnsiGet(X, Y);
    }
}

void CoreAnsi::AnsiGet(int X, int Y)
{
    AnsiGetLast.BlankChar();
    if (AnsiState_.__AnsiLineOccupy__.CountLines() > Y)
    {
        if ((AnsiState_.__AnsiLineOccupy__.CountItems(Y)) > X)
        {
            AnsiState_.__AnsiLineOccupy__.Get(Y, X);
            AnsiGetLast.Item_Char = AnsiState_.__AnsiLineOccupy__.Item_Char;
            AnsiGetLast.Item_ColorB = AnsiState_.__AnsiLineOccupy__.Item_ColorB;
            AnsiGetLast.Item_ColorF = AnsiState_.__AnsiLineOccupy__.Item_ColorF;
            AnsiGetLast.Item_ColorA = AnsiState_.__AnsiLineOccupy__.Item_ColorA;
            AnsiGetLast.Item_FontW = AnsiState_.__AnsiLineOccupy__.Item_FontW;
            AnsiGetLast.Item_FontH = AnsiState_.__AnsiLineOccupy__.Item_FontH;
        }
    }
}

void CoreAnsi::AnsiCharFUnprotected1(int X, int Y, int Ch)
{
    int S = AnsiGetFontSize(Y);
    int T = 1;
    if (S > 0)
    {
        T = 2;
    }

    if (!AnsiState_.CharProtection1Get(X * T, Y))
    {
        AnsiCharF(X, Y, Ch);
    }
}

void CoreAnsi::AnsiCharFUnprotected2(int X, int Y, int Ch)
{
    int S = AnsiGetFontSize(Y);
    int T = 1;
    if (S > 0)
    {
        T = 2;
    }

    if (!AnsiState_.CharProtection2Get(X * T, Y))
    {
        AnsiCharF(X, Y, Ch);
    }
}

void CoreAnsi::AnsiCharF(int X, int Y, int Ch)
{
    AnsiCharF(X, Y, Ch, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
}

void CoreAnsi::AnsiCharFI(int X, int Y, int Ch, int ColB, int ColF, int FonA)
{
    if (AnsiState_.__AnsiInsertMode)
    {
        if (AnsiState_.__AnsiLineOccupy__.CountLines() > Y)
        {
            if (AnsiGetFontSize(Y) > 0)
            {
                if (AnsiState_.__AnsiLineOccupy__.CountItems(Y) >= X)
                {
                    AnsiState_.__AnsiLineOccupy__.BlankChar();
                    AnsiState_.__AnsiLineOccupy__.Insert(Y, X);
                    AnsiState_.__AnsiLineOccupy__.Insert(Y, X);
                }
                if (AnsiState_.__AnsiLineOccupy__.CountItems(Y) > AnsiMaxX)
                {
                    AnsiState_.__AnsiLineOccupy__.Delete(Y, AnsiMaxX);
                    AnsiState_.__AnsiLineOccupy__.Delete(Y, AnsiMaxX);
                }
            }
            else
            {
                if (AnsiState_.__AnsiLineOccupy__.CountItems(Y) >= X)
                {
                    AnsiState_.__AnsiLineOccupy__.BlankChar();
                    AnsiState_.__AnsiLineOccupy__.Insert(Y, X);
                }
                if (AnsiState_.__AnsiLineOccupy__.CountItems(Y) > AnsiMaxX)
                {
                    AnsiState_.__AnsiLineOccupy__.Delete(Y, AnsiMaxX);
                }
            }
            AnsiState_.PrintCharInsDel++;
            AnsiRepaintLine(Y);
        }
    }
    AnsiCharF(X, Y, Ch, ColB, ColF, FonA);
}

void CoreAnsi::AnsiCharF(int X, int Y, int Ch, int ColB, int ColF, int FonA)
{
    int S = AnsiGetFontSize(Y);
    if (S > 0)
    {
        AnsiChar(X * 2 + 0, Y, Ch, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, 1, S - 1, AnsiState_.__AnsiAttr);
        AnsiChar(X * 2 + 1, Y, Ch, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, 2, S - 1, AnsiState_.__AnsiAttr);
    }
    else
    {
        AnsiChar(X, Y, Ch, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiFontSizeW, AnsiState_.__AnsiFontSizeH, AnsiState_.__AnsiAttr);
        AnsiState_.__AnsiFontSizeW = CoreStatic::FontCounter(AnsiState_.__AnsiFontSizeW);
    }
}

void CoreAnsi::AnsiCharUnprotected1(int X, int Y, int Ch)
{
    if (!AnsiState_.CharProtection1Get(X, Y))
    {
        AnsiChar(X, Y, Ch);
    }
}

void CoreAnsi::AnsiCharUnprotected2(int X, int Y, int Ch)
{
    if (!AnsiState_.CharProtection2Get(X, Y))
    {
        AnsiChar(X, Y, Ch);
    }
}

void CoreAnsi::AnsiChar(int X, int Y, int Ch)
{
    AnsiChar(X, Y, Ch, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, 0, 0, AnsiState_.__AnsiAttr);
}

void CoreAnsi::AnsiChar(int X, int Y, int Ch, int ColB, int ColF, int FontW, int FontH, int ColA)
{
    if (X < 0)
    {
        return;
    }
    if (Y < 0)
    {
        return;
    }

    if (AnsiScreenWork)
    {
        Screen::ScreenChar(X, Y + ScreenOffset, Ch, ColB, ColF, ColA, FontW, FontH);
    }
    while (AnsiState_.__AnsiLineOccupy__.CountLines() <= Y)
    {
        AnsiState_.__AnsiLineOccupy__.AppendLine();
    }
    while ((AnsiState_.__AnsiLineOccupy__.CountItems(Y)) <= X)
    {
        AnsiState_.__AnsiLineOccupy__.BlankChar();
        AnsiState_.__AnsiLineOccupy__.Append(Y);
    }
    AnsiState_.__AnsiLineOccupy__.Get(Y, X);

    int __Ch = AnsiState_.__AnsiLineOccupy__.Item_Char;
    int __ColB = AnsiState_.__AnsiLineOccupy__.Item_ColorB;
    int __ColF = AnsiState_.__AnsiLineOccupy__.Item_ColorF;
    int __ColA = AnsiState_.__AnsiLineOccupy__.Item_ColorA & 127;

    AnsiState_.__AnsiLineOccupy__.Item_Char = Ch;
    AnsiState_.__AnsiLineOccupy__.Item_ColorB = ColB;
    AnsiState_.__AnsiLineOccupy__.Item_ColorF = ColF;
    AnsiState_.__AnsiLineOccupy__.Item_ColorA = ColA;
    AnsiState_.__AnsiLineOccupy__.Item_FontW = FontW;
    AnsiState_.__AnsiLineOccupy__.Item_FontH = FontH;
    AnsiState_.__AnsiLineOccupy__.Set(Y, X);
    ColA = ColA & 127;
    bool IsCharOver = false;
    if ((__ColB != ColB) && (__ColB >= 0) && (__ColB != Screen::TextNormalBack))
    {
        IsCharOver = true;
    }
    if (!Str::SpaceChars.Contains(__Ch))
    {
        if ((__Ch != Ch))
        {
            IsCharOver = true;
        }
        if ((__ColF != ColF) && (__ColF >= 0) && (__ColF != Screen::TextNormalFore))
        {
            IsCharOver = true;
        }
        if ((__ColA != ColA) && (__ColA > 0))
        {
            IsCharOver = true;
        }
    }
    if (IsCharOver)
    {
        AnsiState_.PrintCharCounterOver++;
    }
    AnsiState_.PrintCharCounter++;
    AnsiState_.CharProtection1Set(X, Y, AnsiState_.CharProtection1Print);
    AnsiState_.CharProtection2Set(X, Y, AnsiState_.CharProtection2Print);
}

void CoreAnsi::AnsiScreenNegative(bool IsNega)
{
    if (IsNega)
    {
        AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr | 0x80;
        AnsiState_.__AnsiAttr_ = AnsiState_.__AnsiAttr_ | 0x80;
    }
    else
    {
        AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr & 0x7F;
        AnsiState_.__AnsiAttr_ = AnsiState_.__AnsiAttr_ & 0x7F;
    }
    while (AnsiState_.__AnsiLineOccupy__.CountLines() < AnsiMaxY)
    {
        AnsiState_.__AnsiLineOccupy__.AppendLine();
    }
    for (int Y = 0; Y < AnsiMaxY; Y++)
    {
        while (AnsiState_.__AnsiLineOccupy__.CountItems(Y) < AnsiMaxX)
        {
            AnsiState_.__AnsiLineOccupy__.BlankChar();
            AnsiState_.__AnsiLineOccupy__.Append(Y);
        }
        for (int X = 0; X < AnsiMaxX; X++)
        {
            AnsiState_.__AnsiLineOccupy__.Get(Y, X);
            if (IsNega)
            {
                AnsiState_.__AnsiLineOccupy__.Item_ColorA = AnsiState_.__AnsiLineOccupy__.Item_ColorA | 0x80;
            }
            else
            {
                AnsiState_.__AnsiLineOccupy__.Item_ColorA = AnsiState_.__AnsiLineOccupy__.Item_ColorA & 0x7F;
            }
            AnsiState_.__AnsiLineOccupy__.Set(Y, X);
            if (AnsiScreenWork)
            {
                Screen::ScreenChar(X, Y + ScreenOffset, AnsiState_.__AnsiLineOccupy__.Item_Char, AnsiState_.__AnsiLineOccupy__.Item_ColorB, AnsiState_.__AnsiLineOccupy__.Item_ColorF, AnsiState_.__AnsiLineOccupy__.Item_ColorA, AnsiState_.__AnsiLineOccupy__.Item_FontW, AnsiState_.__AnsiLineOccupy__.Item_FontH);
            }
        }
    }
}
