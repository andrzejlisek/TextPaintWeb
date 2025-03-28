#include "coreansi.h"

CoreAnsi::CoreAnsi(std::shared_ptr<ConfigFile> CF)
{
    CommandEndChar.AddString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz@`{}~|");

    AnsiMaxX = CF.get()->ParamGetI("ANSIWidth");
    AnsiMaxY = CF.get()->ParamGetI("ANSIHeight");
    ScreenStatusBar = 0;

    UpdateConfig(CF);

    if ((ANSIDOS < 0) || (ANSIDOS >= 3)) ANSIDOS = 0;
    ANSIDOS_ = (ANSIDOS == 1);

    if (ScreenStatusBar > 0)
    {
        AnsiTerminalResize(AnsiMaxX, AnsiMaxY + 1, ScreenStatusBar);
    }

    ColorThresholdBlackWhite = CF.get()->ParamGetI("ANSIColorThresholdBlackWhite");
    ColorThresholdGray = CF.get()->ParamGetI("ANSIColorThresholdGray");

    // Create 256-color palette
    int C11 = 63 - 1;
    int C12 = 64 + 1;
    int C21 = 191 - 1;
    int C22 = 192 + 2;
    int Val6[6] = { 0, 51, 102, 153, 204, 255 };
    Color256[0] = AnsiColor16(0, 0, 0);
    Color256[1] = AnsiColor16(255, C11, C11);
    Color256[2] = AnsiColor16(C11, 255, C11);
    Color256[3] = AnsiColor16(C21, C21, 0);
    Color256[4] = AnsiColor16(C11, C11, 255);
    Color256[5] = AnsiColor16(C21, 0, C21);
    Color256[6] = AnsiColor16(0, C21, C21);
    Color256[7] = AnsiColor16(128, 128, 128);
    Color256[8] = AnsiColor16(127, 127, 127);
    Color256[9] = AnsiColor16(255, C12, C12);
    Color256[10] = AnsiColor16(C12, 255, C12);
    Color256[11] = AnsiColor16(C22, C22, 0);
    Color256[12] = AnsiColor16(C12, C12, 255);
    Color256[13] = AnsiColor16(C22, 0, C22);
    Color256[14] = AnsiColor16(0, C22, C22);
    Color256[15] = AnsiColor16(255, 255, 255);
    for (int i_R = 0; i_R < 6; i_R++)
    {
        for (int i_G = 0; i_G < 6; i_G++)
        {
            for (int i_B = 0; i_B < 6; i_B++)
            {
                int i_ = i_R * 36 + i_G * 6 + i_B + 16;
                Color256[i_] = AnsiColor16(Val6[i_R], Val6[i_G], Val6[i_B]);
            }
        }
    }
    Color256[232 + 0] = AnsiColor16(0, 0, 0);
    Color256[232 + 1] = AnsiColor16(11, 11, 11);
    Color256[232 + 2] = AnsiColor16(22, 22, 22);
    Color256[232 + 3] = AnsiColor16(33, 33, 33);
    Color256[232 + 4] = AnsiColor16(44, 44, 44);
    Color256[232 + 5] = AnsiColor16(55, 55, 55);
    Color256[232 + 6] = AnsiColor16(67, 67, 67);
    Color256[232 + 7] = AnsiColor16(78, 78, 78);
    Color256[232 + 8] = AnsiColor16(89, 89, 89);
    Color256[232 + 9] = AnsiColor16(100, 100, 100);
    Color256[232 + 10] = AnsiColor16(111, 111, 111);
    Color256[232 + 11] = AnsiColor16(122, 122, 122);
    Color256[232 + 12] = AnsiColor16(133, 133, 133);
    Color256[232 + 13] = AnsiColor16(144, 144, 144);
    Color256[232 + 14] = AnsiColor16(155, 155, 155);
    Color256[232 + 15] = AnsiColor16(166, 166, 166);
    Color256[232 + 16] = AnsiColor16(177, 177, 177);
    Color256[232 + 17] = AnsiColor16(188, 188, 188);
    Color256[232 + 18] = AnsiColor16(200, 200, 200);
    Color256[232 + 19] = AnsiColor16(211, 211, 211);
    Color256[232 + 20] = AnsiColor16(222, 222, 222);
    Color256[232 + 21] = AnsiColor16(233, 233, 233);
    Color256[232 + 22] = AnsiColor16(244, 244, 244);
    Color256[232 + 23] = AnsiColor16(255, 255, 255);
    Color256[256] = -1;

    ANSIScrollChangeSmooth = CF.get()->ParamGetI("ANSIScrollSmooth");
    ANSIScrollChangeChars = CF.get()->ParamGetI("ANSIScrollChars");
    ANSIScrollChangePrepare = true;
    //AnsiScrollPrepare();



    // Load character maps
    std::string CharDOS[50];
    std::string CharsVT52[50];
    int CharDOS_Length = TextWork::StringSplit(CF.get()->ParamGetS("ANSICharsDOS"), ',', CharDOS);
    int CharsVT52_Length = TextWork::StringSplit(CF.get()->ParamGetS("ANSICharsVT52"), ',', CharsVT52);
    for (int i = 0; i < 33; i++)
    {
        if (CharDOS_Length >= 33)
        {
            int T = TextWork::CodeChar(CharDOS[i]);
            if (T >= 32)
            {
                DosControl[i] = T;
            }
            else
            {
                DosControl[i] = 32;
            }
        }
    }
    for (int i = 0; i < 32; i++)
    {
        if (CharsVT52_Length >= 32)
        {
            int T = TextWork::CodeChar(CharsVT52[i]);
            if (T >= 32)
            {
                VT52_SemigraphChars[i] = T;
            }
            else
            {
                VT52_SemigraphChars[i] = 32;
            }
        }
    }
}

void CoreAnsi::UpdateConfig(std::shared_ptr<ConfigFile> CF)
{
    ANSI_CR = CF.get()->ParamGetI("ANSIReadCR");
    ANSI_LF = CF.get()->ParamGetI("ANSIReadLF");
    ANSIDOS = CF.get()->ParamGetI("ANSIDOS");
    ANSIDOS_ = (ANSIDOS == 1);
    ANSI8bit = CF.get()->ParamGetB("ANSI8bit");
    ANSIPrintBackspace = CF.get()->ParamGetB("ANSIPrintBackspace");
    ANSIPrintTab = CF.get()->ParamGetB("ANSIPrintTab");
    ANSIScrollChangeSmooth = CF.get()->ParamGetI("ANSIScrollSmooth");
    ANSIScrollChangeChars = CF.get()->ParamGetI("ANSIScrollChars");
    if ((ANSIScrollChangeSmooth != ANSIScrollSmooth) || (ANSIScrollChangeChars != ANSIScrollChars))
    {
        ANSIScrollChangePrepare = true;
    }
}

void CoreAnsi::AnsiRepaint(bool AdditionalBuffers)
{
    Screen::ScreenClear(-1, -1);
    __ScreenMinX = 0;
    __ScreenMinY = 0;
    __ScreenMaxX = 0;
    __ScreenMaxY = 0;
    int BufMin = AdditionalBuffers ? 0 : 1;
    int BufMax = AdditionalBuffers ? 2 : 1;
    int Bufoffset = 0;
    for (int BufI = BufMin; BufI <= BufMax; BufI++)
    {
        AnsiLineOccupy __AnsiLineOccupyX;
        if (BufI == 1)
        {
            __AnsiLineOccupyX = AnsiState_.__AnsiLineOccupy__;
        }
        else
        {
            if (BufI == 0)
            {
                __AnsiLineOccupyX = AnsiState_.__AnsiLineOccupy1__;
            }
            else
            {
                __AnsiLineOccupyX = AnsiState_.__AnsiLineOccupy2__;
            }
        }
        for (int Y = 0; Y < __AnsiLineOccupyX.CountLines(); Y++)
        {
            int Y__ = Y + Bufoffset;
            for (int X = 0; X < (__AnsiLineOccupyX.CountItems(Y)); X++)
            {
                if (__ScreenMinX > X) { __ScreenMinX = X; }
                if (__ScreenMinY > Y__) { __ScreenMinY = Y__; }
                if (__ScreenMaxX < X) { __ScreenMaxX = X; }
                if (__ScreenMaxY < Y__) { __ScreenMaxY = Y__; }

                __AnsiLineOccupyX.Get(Y, X);
                Screen::ScreenChar(X, Y__ + ScreenOffset, __AnsiLineOccupyX.Item_Char, __AnsiLineOccupyX.Item_ColorB, __AnsiLineOccupyX.Item_ColorF, __AnsiLineOccupyX.Item_ColorA, __AnsiLineOccupyX.Item_FontW, __AnsiLineOccupyX.Item_FontH);
            }
        }
        if (BufI == 1)
        {
            int T1 = AnsiState_.ScrollLastOffsetFirst;
            int T2 = AnsiState_.ScrollLastOffsetLast;
            AnsiScrollSetOffset(AnsiState_.ScrollLastOffsetFirst + Bufoffset, AnsiState_.ScrollLastOffsetLast + Bufoffset, AnsiState_.ScrollLastOffset);
            AnsiState_.ScrollLastOffsetFirst = T1;
            AnsiState_.ScrollLastOffsetLast = T2;
        }
        Bufoffset = Bufoffset + __AnsiLineOccupyX.CountLines();
    }
}

void CoreAnsi::AnsiRepaintLine(int Y)
{
    if (AnsiScreenWork)
    {
        if (Y < AnsiState_.__AnsiLineOccupy__.CountLines())
        {
            int L = (AnsiState_.__AnsiLineOccupy__.CountItems(Y));
            for (int X = 0; X < AnsiMaxX; X++)
            {
                if (X < L)
                {
                    AnsiState_.__AnsiLineOccupy__.Get(Y, X);
                    Screen::ScreenChar(X, Y + ScreenOffset, AnsiState_.__AnsiLineOccupy__.Item_Char, AnsiState_.__AnsiLineOccupy__.Item_ColorB, AnsiState_.__AnsiLineOccupy__.Item_ColorF, AnsiState_.__AnsiLineOccupy__.Item_ColorA, AnsiState_.__AnsiLineOccupy__.Item_FontW, AnsiState_.__AnsiLineOccupy__.Item_FontH);
                }
                else
                {
                    Screen::ScreenChar(X, Y + ScreenOffset, ' ', -1, -1, 0, 0, 0);
                }
            }
        }
        else
        {
            for (int X = 0; X < AnsiMaxX; X++)
            {
                Screen::ScreenChar(X, Y + ScreenOffset, ' ', -1, -1, 0, 0, 0);
            }
        }
    }
}

void CoreAnsi::AnsiResize(int NewW, int NewH)
{
    if (NewW < 0)
    {
        NewW = AnsiMaxX;
    }
    if (NewH < 0)
    {
        NewH = AnsiMaxY;
    }
    if ((NewW != AnsiMaxX) || (NewH != AnsiMaxY))
    {
        if (Screen::WinAuto)
        {
            if (ScreenStatusBar > 0)
            {
                NewH++;
            }
            AnsiScrollClear();
            if (AnsiScreenWork)
            {
                Screen::ScreenResize(NewW, NewH);
            }
            AnsiTerminalResize(NewW, NewH, ScreenStatusBar);
        }
    }
}
