#include "screen.h"

Screen::Screen()
{

}

void Screen::UpdateFontParams(std::shared_ptr<ConfigFile> CF)
{
    std::string PagesS = CF.get()->ParamGetS("_WinBitmapFontPageList");
    std::string Double1 = CF.get()->ParamGetS("_WinBitmapFontDouble1");
    std::string Double2 = CF.get()->ParamGetS("_WinBitmapFontDouble2");

    XList<std::string> PagesArr;
    TextWork::StringSplit(PagesS, '|', PagesArr);
    BitmapPage.Clear();
    for (int I = 0; I < PagesArr.Count; I++)
    {
        if (PagesArr[I].size() > 0)
        {
            BitmapPage.AddSort(std::stoi(PagesArr[I]));
        }
    }
    FontSinglePage = (BitmapPage.Count == 1);

    Range1.Clear();
    Range2.Clear();
    XList<std::string> DoubleRange1;
    XList<std::string> DoubleRange2;
    TextWork::StringSplit(Double1, '|', DoubleRange1);
    TextWork::StringSplit(Double2, '|', DoubleRange2);

    if (DoubleRange1.Count == DoubleRange2.Count)
    {
        for (int I = 0; I < DoubleRange1.Count; I++)
        {
            if ((DoubleRange1[I].size() > 0) && (DoubleRange2[I].size() > 0))
            {
                Range1.Add(std::stoi(DoubleRange1[I]));
                Range2.Add(std::stoi(DoubleRange2[I]));
            }
        }

        for (int I = 0; I < Range1.Count; I++)
        {
            for (int J = 0; J < Range1.Count; J++)
            {
                if (Range1[I] < Range1[J])
                {
                    int T1 = Range1[I];
                    int T2 = Range2[I];
                    Range1[I] = Range1[J];
                    Range2[I] = Range2[J];
                    Range1[J] = T1;
                    Range2[J] = T2;
                }
            }
        }
    }
}

void Screen::StaticInit(std::shared_ptr<ConfigFile> CF)
{
    FontListMode = CF.get()->ParamGetI("FontMode");

    // Font list
    int ItemN = 1;
    std::string ItemName = CF.get()->ParamGetS("Font1Name");
    while (ItemName != "")
    {
        FontListName.Add(ItemName);
        FontListFile1.Add(CF.get()->ParamGetS("Font" + std::to_string(ItemN) + "File1"));
        FontListFile2.Add(CF.get()->ParamGetS("Font" + std::to_string(ItemN) + "File2"));
        FontListFile3.Add(CF.get()->ParamGetS("Font" + std::to_string(ItemN) + "File3"));

        ItemN++;
        ItemName = CF.get()->ParamGetS("Font" + std::to_string(ItemN) + "Name");
    }
    FontListSelect = CF.get()->ParamGetI("FontSelect") - 1;
    if (FontListSelect < 0)
    {
        FontListSelect = 0;
    }

    // Palette list
    ItemN = 1;
    ItemName = CF.get()->ParamGetS("Palette1Name");
    while (ItemName != "")
    {
        PaletteListName.Add(ItemName);
        PaletteListColors.Add(CF.get()->ParamGetS("Palette" + std::to_string(ItemN) + "Colors"));

        ItemN++;
        ItemName = CF.get()->ParamGetS("Palette" + std::to_string(ItemN) + "Name");
    }
    PaletteListSelect = CF.get()->ParamGetI("PaletteSelect") - 1;
    if (PaletteListSelect < 0)
    {
        PaletteListSelect = 0;
    }
}

int Screen::CharDouble(int C)
{
    int T = Range2.IndexOfBinRange(C);
    if (T >= 0)
    {
        if (Range1[T] <= C)
        {
            return C + UnicodeCount;
        }
    }
    return 0;
}

int Screen::CharDoubleInv(int C)
{
    if (C >= UnicodeCount)
    {
        int T = Range2.IndexOfBinRange(C - UnicodeCount);
        if (T >= 0)
        {
            if (Range1[T] <= (C - UnicodeCount))
            {
                return C - UnicodeCount;
            }
        }
    }
    return -1;
}

void Screen::ScreenClear(int Back, int Fore)
{
    ScreenLineOffsetArray.Clear();
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
    if (FontSinglePage)
    {
        if (FontSingleChar.count(C) > 0)
        {
            C = FontSingleChar[C];
        }
    }
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
    ScreenLineOffsetArray.Clear();
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
    for (int I = 0; I < ScreenLineOffsetArray.Count; I++)
    {
        if (ScreenLineOffsetArray[I].NeedSet)
        {
            ScreenLineOffsetDef _ = ScreenLineOffsetArray[I];
            ScreenLineOffset_(I, _.Offset, _.Blank, _.ColorBack, _.ColorFore, _.ColorAttr);
            ScreenLineOffsetArray[I].NeedSet = false;
        }
    }
    ScreenCursorMove_(CurrentX, CurrentY);
}

void Screen::ScreenLineOffset(int Y, int Offset, int Blank, int ColorBack, int ColorFore, int ColorAttr)
{
    ScreenLineOffsetDef _;
    while (ScreenLineOffsetArray.Count <= Y)
    {
        _.NeedSet = false;
        _.Offset = 0;
        ScreenLineOffsetArray.Add(_);
    }
    _.NeedSet = true;
    _.Offset = Offset;
    _.Blank = Blank;
    _.ColorBack = ColorBack;
    _.ColorFore = ColorFore;
    _.ColorAttr = ColorAttr;
    ScreenLineOffsetArray[Y] = _;
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
    if (X)
    {
        ScreenOther(1);
    }
    else
    {
        ScreenOther(2);
    }
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

void Screen::Bell()
{
    ScreenOther(0);
}

void Screen::CursorHide(bool Hide)
{
    if (CursorHideState != Hide)
    {
        if (Hide)
        {
            ScreenOther(3);
        }
        else
        {
            ScreenOther(4);
        }
        CursorHideState = Hide;
    }
}

void Screen::SetPalette(std::string PaletteColors)
{
    PaletteCustom = true;
    Screen::ScreenOtherString(1, PaletteColors);
}

void Screen::SetFont(std::string FontFile1, std::string FontFile2, std::string FontFile3, int FontMode)
{
    FontCustom = true;
    Screen::ScreenOtherString(2, FontFile1 + "|" + FontFile2 + "|" + FontFile3 + "|" + std::to_string(FontMode));
}

void Screen::SetCustomFont(int Size)
{
    for (int I = 0; I < 256; I++)
    {
        FontCustomMap[I] = I;
    }
    FontCustomMap[256] = 0;
    FontCustom = true;
    Screen::ScreenOtherString(3, std::to_string(Size));
}

void Screen::SetCustomChar(int CharCode, int CharDisp, std::string Data)
{
    FontCustomMap[CharCode] = CharDisp;
    Screen::ScreenOtherString(4, Data);
}

void Screen::SetPalette()
{
    SetPalette(PaletteListColors[PaletteListSelect]);
    PaletteCustom = false;
}

void Screen::SetFont()
{
    std::string F1 = FontListFile1[FontListSelect];
    std::string F2 = FontListFile2[FontListSelect];
    std::string F3 = FontListFile3[FontListSelect];
    SetFont(F1, F2, F3, FontListMode);
    FontCustom = false;
}

void Screen::ResetCustomPaletteFont()
{
    SetPalette();
    SetFont();
}
