#include "editorpixelpaint.h"

EditorPixelPaint::EditorPixelPaint()
{

}

bool EditorPixelPaint::IsCharPaint()
{
    if (PPS.PaintModeN == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void EditorPixelPaint::SelectPaintMode()
{
    PPS.CanvasX = PPS.CanvasX / PPS.CharW;
    PPS.CanvasY = PPS.CanvasY / PPS.CharH;
    PPS.SizeX = PPS.SizeX / PPS.CharW;
    PPS.SizeY = PPS.SizeY / PPS.CharH;
    PPS.CharW = PaintMode_[PPS.PaintModeN].CharW;
    PPS.CharH = PaintMode_[PPS.PaintModeN].CharH;
    PPS.CharX = 0;
    PPS.CharY = 0;
    PPS.CanvasX = PPS.CanvasX * PPS.CharW;
    PPS.CanvasY = PPS.CanvasY * PPS.CharH;
    PPS.SizeX = PPS.SizeX * PPS.CharW;
    PPS.SizeY = PPS.SizeY * PPS.CharH;
}

void EditorPixelPaint::Init(std::shared_ptr<ConfigFile> CF)
{
    PaintMode PaintModeNew;
    PaintModeNew.Name = "Char";
    PaintModeNew.CharW = 1;
    PaintModeNew.CharH = 1;

    std::string PixelCharDefault[1000];
    int PixelCharDefaultL = TextWork::StringSplit(CF.get()->ParamGetS("PixelChar"), ',', PixelCharDefault);

    PaintModeNew.IntToChar[0] = TextWork::CodeChar(PixelCharDefault[0]);
    PaintModeNew.IntToChar[1] = TextWork::CodeChar(PixelCharDefault[1]);
    PaintModeNew.CharToInt[TextWork::CodeChar(PixelCharDefault[0])] = 0;
    PaintModeNew.CharToInt[TextWork::CodeChar(PixelCharDefault[1])] = 1;
    PaintMode_.Add(PaintModeNew);

    int I = 1;
    std::string PaintDef = CF.get()->ParamGetS("Pixel_" + std::to_string(I));
    while (PaintDef != "")
    {
        std::string PaintDef_[3];
        TextWork::StringSplit(PaintDef, ',', PaintDef_);

        PaintModeNew.Name = PaintDef_[0];
        PaintModeNew.CharW = stoi(PaintDef_[1]);
        PaintModeNew.CharH = stoi(PaintDef_[2]);
        PaintModeNew.CharToInt.clear();
        PaintModeNew.IntToChar.clear();
        int II = 0;
        std::string CharVals[256];
        int CharValsL = TextWork::StringSplit(CF.get()->ParamGetS("Pixel_" + std::to_string(I) + "_" + std::to_string(II)), ',', CharVals);
        int CharValN = 0;
        while ((CharValsL > 0) && (CharVals[0] != ""))
        {
            for (int III = 0; III < CharValsL; III++)
            {
                int TempCharCode = TextWork::CodeChar(CharVals[III]);
                PaintModeNew.IntToChar[CharValN] = TempCharCode;
                if (PaintModeNew.CharToInt.count(TempCharCode) == 0)
                {
                    PaintModeNew.CharToInt[TempCharCode] = CharValN;
                }
                CharValN++;
            }
            II++;
            CharValsL = TextWork::StringSplit(CF.get()->ParamGetS("Pixel_" + std::to_string(I) + "_" + std::to_string(II)), ',', CharVals);
        }
        I++;
        PaintDef = CF.get()->ParamGetS("Pixel_" + std::to_string(I));
        PaintMode_.Add(PaintModeNew);

    }
    PPS.PaintModeN = 0;
    PaintModeCount = PaintMode_.Count;
    PPS.PaintPencil = false;
    PPS.PaintColor = 0;
    SelectPaintMode();
}

void EditorPixelPaint::PaintStart()
{
    PPS.CanvasXBase = EditorData_.get()->CursorXBase();
    PPS.CanvasYBase = EditorData_.get()->CursorYBase();
    PPS.FontW = EditorData_.get()->CursorFontW;
    PPS.FontH = EditorData_.get()->CursorFontH;
    PPS.CanvasX = ((EditorData_.get()->CursorX - PPS.CanvasXBase) * PPS.CharW) / PPS.FontW;
    PPS.CanvasY = ((EditorData_.get()->CursorY - PPS.CanvasYBase) * PPS.CharH) / PPS.FontH;
    PPS.PaintPencil = false;
    SelectPaintMode();
}

int EditorPixelPaint::GetCursorPosXSize()
{
    return PPS.CanvasXBase + ((PPS.CanvasX + PPS.SizeX) / PPS.CharW) * PPS.FontW;
}

int EditorPixelPaint::GetCursorPosYSize()
{
    return PPS.CanvasYBase + ((PPS.CanvasY + PPS.SizeY) / PPS.CharH) * PPS.FontH;
}

int EditorPixelPaint::GetCursorPosX()
{
    return PPS.CanvasXBase + ((PPS.CanvasX - PPS.CharX) / PPS.CharW) * PPS.FontW;
}

int EditorPixelPaint::GetCursorPosY()
{
    return PPS.CanvasYBase + ((PPS.CanvasY - PPS.CharY) / PPS.CharH) * PPS.FontH;
}

int EditorPixelPaint::CharGet(int X, int Y, bool Space)
{
    return EditorData_.get()->ElementGetVal((X / PPS.CharW) * PPS.FontW + PPS.CanvasXBase, (Y / PPS.CharH) * PPS.FontH + PPS.CanvasYBase, Space, false, 0);
}

int EditorPixelPaint::ColoBGet(int X, int Y, bool Space)
{
    return EditorData_.get()->ElementGetVal((X / PPS.CharW) * PPS.FontW + PPS.CanvasXBase, (Y / PPS.CharH) * PPS.FontH + PPS.CanvasYBase, Space, false, 1);
}

int EditorPixelPaint::ColoFGet(int X, int Y, bool Space)
{
    return EditorData_.get()->ElementGetVal((X / PPS.CharW) * PPS.FontW + PPS.CanvasXBase, (Y / PPS.CharH) * PPS.FontH + PPS.CanvasYBase, Space, false, 2);
}

int EditorPixelPaint::ColoAGet(int X, int Y, bool Space)
{
    return EditorData_.get()->ElementGetVal((X / PPS.CharW) * PPS.FontW + PPS.CanvasXBase, (Y / PPS.CharH) * PPS.FontH + PPS.CanvasYBase, Space, false, 3);
}

void EditorPixelPaint::CharPut(int X, int Y, int Ch, int ColB, int ColF, int ColA)
{
    AnsiLineOccupyItem Item;
    Item.Item_Char = Ch;
    Item.Item_ColorB = ColB;
    Item.Item_ColorF = ColF;
    Item.Item_ColorA = ColA;
    Item.Item_FontW = PPS.FontW;
    Item.Item_FontH = PPS.FontH;
    EditorData_.get()->CharPut((X / PPS.CharW) * PPS.FontW + PPS.CanvasXBase, (Y / PPS.CharH) * PPS.FontH + PPS.CanvasYBase, Item, false);
}

void EditorPixelPaint::MoveCursor(int Direction)
{
    switch (Direction)
    {
        case 0: // Up
        case 10: // Up
            if (PPS.CanvasY > 0)
            {
                PPS.CanvasY--;
                if (PPS.CharY > 0)
                {
                    PPS.CharY--;
                }
                else
                {
                    PPS.CharY = PPS.CharH - 1;
                }
            }
            break;
        case 1: // Down
        case 11: // Down
            PPS.CanvasY++;
            if (PPS.CharY < (PPS.CharH - 1))
            {
                PPS.CharY++;
            }
            else
            {
                PPS.CharY = 0;
            }
            break;
        case 2: // Left
        case 12: // Left
            if (PPS.CanvasX > 0)
            {
                PPS.CanvasX--;
                if (PPS.CharX > 0)
                {
                    PPS.CharX--;
                }
                else
                {
                    PPS.CharX = PPS.CharW - 1;
                }
            }
            break;
        case 3: // Right
        case 13: // Right
            PPS.CanvasX++;
            if (PPS.CharX < (PPS.CharW - 1))
            {
                PPS.CharX++;
            }
            else
            {
                PPS.CharX = 0;
            }
            break;
        case 4: // Up right
            MoveCursor(10);
            MoveCursor(13);
            break;
        case 5: // Down left
            MoveCursor(11);
            MoveCursor(12);
            break;
        case 6: // Up left
            MoveCursor(10);
            MoveCursor(12);
            break;
        case 7: // Down right
            MoveCursor(11);
            MoveCursor(13);
            break;
    }
}

void EditorPixelPaint::SwapCursorsX()
{
    PPS.CanvasX = PPS.CanvasX + PPS.SizeX;
    PPS.CharX = PPS.CanvasX % PPS.CharW;
    PPS.SizeX = 0 - PPS.SizeX;
    if (PPS.CanvasX < 0)
    {
        PPS.CanvasX = 0;
    }
}

void EditorPixelPaint::SwapCursorsY()
{
    PPS.CanvasY = PPS.CanvasY + PPS.SizeY;
    PPS.CharY = PPS.CanvasY % PPS.CharH;
    PPS.SizeY = 0 - PPS.SizeY;
    if (PPS.CanvasY < 0)
    {
        PPS.CanvasY = 0;
    }
}

void EditorPixelPaint::SwapCursors(int RotMode)
{
    if ((PPS.SizeX == 0) && (PPS.SizeY != 0))
    {
        SwapCursorsY();
        return;
    }
    if ((PPS.SizeX != 0) && (PPS.SizeY == 0))
    {
        SwapCursorsX();
        return;
    }
    if ((PPS.SizeX != 0) && (PPS.SizeY != 0))
    {
        if (((PPS.SizeX < 0) && (PPS.SizeY < 0)) || ((PPS.SizeX > 0) && (PPS.SizeY > 0)))
        {
            if (RotMode > 0)
            {
                SwapCursorsX();
            }
            else
            {
                SwapCursorsY();
            }
            return;
        }
        if (((PPS.SizeX < 0) && (PPS.SizeY > 0)) || ((PPS.SizeX > 0) && (PPS.SizeY < 0)))
        {
            if (RotMode > 0)
            {
                SwapCursorsY();
            }
            else
            {
                SwapCursorsX();
            }
            return;
        }
    }
}

int EditorPixelPaint::CustomCharColor()
{
    if ((PPS.PaintColor == 0) || (PPS.PaintColor == 4))
    {
        return 1;
    }
    if ((PPS.PaintColor == 1) || (PPS.PaintColor == 5))
    {
        return 0;
    }
    if ((PPS.PaintColor == 2) || (PPS.PaintColor == 6))
    {
        if (GetPixel0(PPS.CanvasX, PPS.CanvasY) == 1)
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
    if ((PPS.PaintColor == 3) || (PPS.PaintColor == 7))
    {
        if (GetPixel0(PPS.CanvasX, PPS.CanvasY) == 1)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
    return 0;
}

void EditorPixelPaint::CustomCharSet(int C)
{
    PaintMode_[0].IntToChar[CustomCharColor()] = C;
    PaintMode_[0].CharToInt.clear();
    PaintMode_[0].CharToInt[PaintMode_[0].IntToChar[0]] = 0;
    PaintMode_[0].CharToInt[PaintMode_[0].IntToChar[1]] = 1;
}

int EditorPixelPaint::CustomCharGet()
{
    return PaintMode_[0].IntToChar[CustomCharColor()];
}

void EditorPixelPaint::Paint()
{
    SetPixel(PPS.CanvasX, PPS.CanvasY);
}

Str EditorPixelPaint::GetStatusInfo()
{
    Str Sb;
    Sb.AddString("  ");
    std::string ModeName = PaintMode_[PPS.PaintModeN].Name;
    if (IsCharPaint())
    {
        ModeName = TextWork::CharCode(PaintMode_[0].IntToChar[0], 1) + "/" + TextWork::CharCode(PaintMode_[0].IntToChar[1], 1);
    }
    Sb.AddString(PPS.DefaultColor ? "Pxl-F" : "Pxl-B");
    if (PPS.PaintPencil)
    {
        Sb.AddString("* ");
    }
    else
    {
        Sb.AddString("  ");
    }
    Sb.AddString(ModeName + " ");
    switch (PPS.PaintColor)
    {
        case 0:
            Sb.AddString("Fore-H");
            break;
        case 1:
            Sb.AddString("Back-H");
            break;
        case 2:
            Sb.AddString("Nega-H");
            break;
        case 3:
            Sb.AddString("Posi-H");
            break;
        case 4:
            Sb.AddString("Fore-F");
            break;
        case 5:
            Sb.AddString("Back-F");
            break;
        case 6:
            Sb.AddString("Nega-F");
            break;
        case 7:
            Sb.AddString("Posi-F");
            break;
    }
    switch (PPS.PaintMoveRoll)
    {
        case 0:
            Sb.AddString(" Repeat");
            break;
        case 1:
            Sb.AddString(" Roll");
            break;
        case 2:
            Sb.AddString(" Back");
            break;
        case 3:
            Sb.AddString(" Fore");
            break;
        case 4:
            Sb.AddString(" FlipRot");
            break;
    }
    return Sb;
}

int EditorPixelPaint::GetPxlCoB0(int X, int Y)
{
    int PxlC = GetPxlCoB(X, Y);
    if (PxlC < 0)
    {
        return 0;
    }
    return PxlC;
}

int EditorPixelPaint::GetPxlCoF0(int X, int Y)
{
    int PxlC = GetPxlCoF(X, Y);
    if (PxlC < 0)
    {
        return 0;
    }
    return PxlC;
}

int EditorPixelPaint::GetPxlCoA0(int X, int Y)
{
    int PxlC = GetPxlCoA(X, Y);
    if (PxlC < 0)
    {
        return 0;
    }
    return PxlC;
}

int EditorPixelPaint::GetPixel0(int X, int Y)
{
    int PxlC = GetPixel(X, Y);
    if (PxlC < 0)
    {
        return 0;
    }
    return PxlC;
}

int EditorPixelPaint::GetPxlCoB(int X, int Y)
{
    return ColoBGet(X, Y, true);
}

int EditorPixelPaint::GetPxlCoF(int X, int Y)
{
    return ColoFGet(X, Y, true);
}

int EditorPixelPaint::GetPxlCoA(int X, int Y)
{
    return ColoAGet(X, Y, true);
}

void EditorPixelPaint::PixelStackPush()
{
    PixelStackN.Add(PxlNum);
    PixelStackB.Add(PxlBit);
}

void EditorPixelPaint::PixelStackPop()
{
    PxlNum = PixelStackN.PopLast();
    PxlBit = PixelStackB.PopLast();
}

int EditorPixelPaint::GetPixel(int X, int Y)
{
    PxlNum = 0;
    PxlBit = 1 << (((Y % PPS.CharH) * PPS.CharW) + (X % PPS.CharW));
    int CharNum = CharGet(X, Y, false);
    if ((CharNum < 0) || (X < 0) || (Y < 0))
    {
        return -1;
    }
    if (PPS.DefaultColor)
    {
        PxlNum = (1 << (PPS.CharH * PPS.CharW)) - 1;
    }
    if (PaintMode_[PPS.PaintModeN].CharToInt.count(CharNum) > 0)
    {
        PxlNum = PaintMode_[PPS.PaintModeN].CharToInt[CharNum];
    }
    if ((PxlNum & PxlBit) > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void EditorPixelPaint::SetPixel(int X, int Y)
{
    if ((X < 0) || (Y < 0))
    {
        return;
    }
    int G = GetPixel0(X, Y);
    if ((PPS.PaintColor == 1) || (PPS.PaintColor == 5) || (((PPS.PaintColor == 2) || (PPS.PaintColor == 6)) && (G == 1)))
    {
        PxlNum = PxlNum & (0xFFFFFF - PxlBit);
    }
    if ((PPS.PaintColor == 0) || (PPS.PaintColor == 4) || (((PPS.PaintColor == 2) || (PPS.PaintColor == 6)) && (G == 0)))
    {
        PxlNum = PxlNum | PxlBit;
    }
    int NewChar = PaintMode_[PPS.PaintModeN].IntToChar[PxlNum];
    int NewColoB = EditorData_.get()->DrawColoBI;
    int NewColoF = EditorData_.get()->DrawColoFI;
    int NewColoA = EditorData_.get()->DrawColoAI;
    if (!EditorData_.get()->ToggleDrawText)
    {
        NewChar = CharGet(X, Y, true);
    }
    if (!EditorData_.get()->ToggleDrawColo)
    {
        NewColoB = ColoBGet(X, Y, true);
        NewColoF = ColoFGet(X, Y, true);
        NewColoA = ColoAGet(X, Y, true);
    }
    CharPut(X, Y, NewChar, NewColoB, NewColoF, NewColoA);
}

void EditorPixelPaint::SetPixel(int X, int Y, int PxlColor, int AnsiCB, int AnsiCF, int AnsiCA)
{
    if ((X < 0) || (Y < 0))
    {
        return;
    }
    if (PxlColor < 0)
    {
        PxlColor = 0;
    }
    if (PxlColor == 1)
    {
        PxlNum = PxlNum | PxlBit;
    }
    if (PxlColor == 0)
    {
        PxlNum = PxlNum & (0xFFFFFF - PxlBit);
    }
    int NewChar = PaintMode_[PPS.PaintModeN].IntToChar[PxlNum];
    int NewColoB = AnsiCB;
    int NewColoF = AnsiCF;
    int NewColoA = AnsiCA;
    if (!EditorData_.get()->ToggleDrawText)
    {
        NewChar = CharGet(X, Y, true);
    }
    if (!EditorData_.get()->ToggleDrawColo)
    {
        NewColoB = ColoBGet(X, Y, true);
        NewColoF = ColoFGet(X, Y, true);
        NewColoA = ColoAGet(X, Y, true);
    }
    CharPut(X, Y, NewChar, NewColoB, NewColoF, NewColoA);
}

void EditorPixelPaint::SetPixelBuf(int X, int Y)
{
    for (int i = 0; i < PixBufX.Count; i++)
    {
        if ((PixBufX[i] == X) && (PixBufY[i] == Y))
        {
            return;
        }
    }
    PixBufX.Add(X);
    PixBufY.Add(Y);
    SetPixel(X, Y);
}

void EditorPixelPaint::PixBufClear()
{
    PixBufX.Clear();
    PixBufY.Clear();
}

void EditorPixelPaint::PaintLine()
{
    int X0 = PPS.CanvasX;
    int Y0 = PPS.CanvasY;
    int X1 = PPS.CanvasX + PPS.SizeX;
    int Y1 = PPS.CanvasY + PPS.SizeY;
    int DX = X1 - X0;
    int DY = Y1 - Y0;
    int IncX = (DX > 0 ? 1 : -1);
    int IncY = (DY > 0 ? 1 : -1);
    int D = 0;
    int DeltaA = 0;
    int DeltaB = 0;
    int X = 0;
    int Y = 0;
    int I;

    if (DX < 0) { DX = 0 - DX; }
    if (DY < 0) { DY = 0 - DY; }

    if (DX > DY)
    {
        D = 2 * DY - DX;
        DeltaA = 2 * DY;
        DeltaB = 2 * DY - 2 * DX;
        X = 0;
        Y = 0;
        for (I = 0; I <= DX; I++)
        {
            SetPixel(X0 + X, Y0 + Y);
            if (D > 0)
            {
                D = D + DeltaB;
                X = X + IncX;
                Y = Y + IncY;
            }
            else
            {
                D = D + DeltaA;
                X = X + IncX;
            }
        }
    }
    else
    {
        D = 2 * DX - DY;
        DeltaA = 2 * DX;
        DeltaB = 2 * DX - 2 * DY;
        X = 0;
        Y = 0;
        for (I = 0; I <= DY; I++)
        {
            SetPixel(X0 + X, Y0 + Y);
            if (D > 0)
            {
                D = D + DeltaB;
                X = X + IncX;
                Y = Y + IncY;
            }
            else
            {
                D = D + DeltaA;
                Y = Y + IncY;
            }
        }
    }
}

void EditorPixelPaint::PaintRect()
{
    int X0 = PPS.CanvasX;
    int Y0 = PPS.CanvasY;
    int X1 = PPS.CanvasX + PPS.SizeX;
    int Y1 = PPS.CanvasY + PPS.SizeY;
    if (X1 < X0) { int _ = X0; X0 = X1; X1 = _; }
    if (Y1 < Y0) { int _ = Y0; Y0 = Y1; Y1 = _; }
    if (PPS.PaintColor >= 4)
    {
        for (int Y = Y0; Y <= Y1; Y++)
        {
            for (int X = X0; X <= X1; X++)
            {
                SetPixel(X, Y);
            }
        }
    }
    else
    {
        for (int Y = Y0; Y <= Y1; Y++)
        {
            SetPixel(X0, Y);
            if (X0 != X1)
            {
                SetPixel(X1, Y);
            }
        }
        for (int X = (X0 + 1); X < X1; X++)
        {
            SetPixel(X, Y0);
            if (Y0 != Y1)
            {
                SetPixel(X, Y1);
            }
        }
    }
}

void EditorPixelPaint::PaintEllipseWork(int X0, int Y0, int RX, int RY, int PX, int PY, bool M1, bool M2)
{
    bool Fill = (PPS.PaintColor >= 4);
    int RX2 = RX * RX;
    int RY2 = RY * RY;
    int D = 4 * RY2 - 4 * RY * RX2 + RX2;
    int DeltaA = 4 * 3 * RY2;
    int DeltaB = 4 * (3 * RY2 - 2 * RY * RX2 + 2 * RX2);
    int Limit = (RX2 * RX2) / (RX2 + RY2);
    int X = 0;
    int Y = RY;
    bool Working = true;
    while (Working)
    {
        if (M1)
        {
            if (Fill)
            {
                for (int X_ = (0 - X); X_ <= (X + PX); X_++)
                {
                    SetPixelBuf(X0 + X_, Y0 - Y);
                    SetPixelBuf(X0 + X_, Y0 + Y + PY);
                }
            }
            else
            {
                SetPixelBuf(X0 + X + PX, Y0 + Y + PY);
                SetPixelBuf(X0 - X, Y0 + Y + PY);
                SetPixelBuf(X0 + X + PX, Y0 - Y);
                SetPixelBuf(X0 - X, Y0 - Y);
            }
        }
        if (M2)
        {
            if (Fill)
            {
                for (int Y_ = (0 - Y); Y_ <= (Y + PY); Y_++)
                {
                    SetPixelBuf(X0 + Y_, Y0 - X);
                    SetPixelBuf(X0 + Y_, Y0 + X + PX);
                }
            }
            else
            {
                SetPixelBuf(X0 + Y + PY, Y0 + X + PX);
                SetPixelBuf(X0 - Y, Y0 + X + PX);
                SetPixelBuf(X0 + Y + PY, Y0 - X);
                SetPixelBuf(X0 - Y, Y0 - X);
            }
        }
        if ((X * X) >= Limit) { Working = false; }
        if (D > 0)
        {
            D = D + DeltaB;
            DeltaA = DeltaA + 4 * 2 * RY2;
            DeltaB = DeltaB + 4 * (2 * RY2 + 2 * RX2);
            X = X + 1;
            Y = Y - 1;
        }
        else
        {
            D = D + DeltaA;
            DeltaA = DeltaA + 4 * 2 * RY2;
            DeltaB = DeltaB + 4 * 2 * RY2;
            X = X + 1;
        }
    }
}

void EditorPixelPaint::PaintEllipse()
{
    int X0 = PPS.CanvasX;
    int Y0 = PPS.CanvasY;
    int X1 = PPS.CanvasX + PPS.SizeX;
    int Y1 = PPS.CanvasY + PPS.SizeY;
    if (X1 < X0) { int _ = X0; X0 = X1; X1 = _; }
    if (Y1 < Y0) { int _ = Y0; Y0 = Y1; Y1 = _; }
    int PX = 0;
    int PY = 0;
    if (((X1 - X0) % 2) == 1) { X1--; PX = 1; }
    if (((Y1 - Y0) % 2) == 1) { Y1--; PY = 1; }
    int X = (X1 + X0) / 2;
    int Y = (Y1 + Y0) / 2;
    int RX = (X1 - X0) / 2;
    int RY = (Y1 - Y0) / 2;
    PixBufClear();
    PaintEllipseWork(X, Y, RX, RY, PX, PY, true, false);
    PaintEllipseWork(X, Y, RY, RX, PY, PX, false, true);
    PixBufClear();
}

void EditorPixelPaint::PaintFill()
{
    int X0 = PPS.CanvasX;
    int Y0 = PPS.CanvasY;

    int FillColor = GetPixel0(X0, Y0);

    if (FillColor == 0)
    {
        if ((PPS.PaintColor == 1) || (PPS.PaintColor == 5))
        {
            return;
        }
    }
    if (FillColor == 1)
    {
        if ((PPS.PaintColor == 0) || (PPS.PaintColor == 4))
        {
            return;
        }
    }

    XList<int> QX;
    XList<int> QY;
    int QN = 0;


    QX.Add(X0);
    QY.Add(Y0);
    QN++;

    int X;
    int Y;
    while (QN > 0)
    {
        X = QX.PopFirst();
        Y = QY.PopFirst();
        QN--;

        if (GetPixel(X, Y) == FillColor)
        {
            SetPixel(X, Y);
            if (GetPixel(X - 1, Y) == FillColor)
            {
                QX.Add(X - 1);
                QY.Add(Y);
                QN++;
            }
            if (GetPixel(X, Y - 1) == FillColor)
            {
                QX.Add(X);
                QY.Add(Y - 1);
                QN++;
            }
            if (GetPixel(X + 1, Y) == FillColor)
            {
                QX.Add(X + 1);
                QY.Add(Y);
                QN++;
            }
            if (GetPixel(X, Y + 1) == FillColor)
            {
                QX.Add(X);
                QY.Add(Y + 1);
                QN++;
            }
        }
    }

}

void EditorPixelPaint::PaintFlipRot(int Direction)
{
    int X0 = PPS.CanvasX;
    int Y0 = PPS.CanvasY;
    int X1 = PPS.CanvasX + PPS.SizeX;
    int Y1 = PPS.CanvasY + PPS.SizeY;
    if (X1 < X0) { int _ = X0; X0 = X1; X1 = _; }
    if (Y1 < Y0) { int _ = Y0; Y0 = Y1; Y1 = _; }
    int Temp1C, Temp1B, Temp1F, Temp1A;
    int Temp2C, Temp2B, Temp2F, Temp2A;
    int Temp3C, Temp3B, Temp3F, Temp3A;
    int Temp4C, Temp4B, Temp4F, Temp4A;
    int W = (X1 - X0 + 1);
    if ((W & 1) != 0)
    {
        if (Direction < 2)
        {
            W--;
        }
        else
        {
            W++;
        }
    }
    W = W >> 1;
    int H = (Y1 - Y0 + 1);
    if ((H & 1) != 0)
    {
        H--;
    }
    H = H >> 1;


    switch (Direction)
    {
        case 0: // Flip V
            if (IsCharPaint())
            {
                for (int i_X = X0; i_X <= X1; i_X++)
                {
                    for (int i_Y = 0; i_Y < H; i_Y++)
                    {
                        Temp1C = CharGet(i_X, Y0 + i_Y, true);
                        Temp1B = ColoBGet(i_X, Y0 + i_Y, true);
                        Temp1F = ColoFGet(i_X, Y0 + i_Y, true);
                        Temp1A = ColoAGet(i_X, Y0 + i_Y, true);
                        Temp2C = CharGet(i_X, Y1 - i_Y, true);
                        Temp2B = ColoBGet(i_X, Y1 - i_Y, true);
                        Temp2F = ColoFGet(i_X, Y1 - i_Y, true);
                        Temp2A = ColoAGet(i_X, Y1 - i_Y, true);
                        CharPut(i_X, Y1 - i_Y, Temp1C, Temp1B, Temp1F, Temp1A);
                        CharPut(i_X, Y0 + i_Y, Temp2C, Temp2B, Temp2F, Temp2A);
                    }
                }
            }
            else
            {
                for (int i_X = X0; i_X <= X1; i_X++)
                {
                    for (int i_Y = 0; i_Y < H; i_Y++)
                    {
                        Temp1C = GetPixel0(i_X, Y0 + i_Y);
                        Temp1B = GetPxlCoB0(i_X, Y0 + i_Y);
                        Temp1F = GetPxlCoF0(i_X, Y0 + i_Y);
                        Temp1A = GetPxlCoA0(i_X, Y0 + i_Y);
                        PixelStackPush();
                        Temp2C = GetPixel0(i_X, Y1 - i_Y);
                        Temp2B = GetPxlCoB0(i_X, Y1 - i_Y);
                        Temp2F = GetPxlCoF0(i_X, Y1 - i_Y);
                        Temp2A = GetPxlCoA0(i_X, Y1 - i_Y);
                        SetPixel(i_X, Y1 - i_Y, Temp1C, Temp1B, Temp1F, Temp1A);
                        PixelStackPop();
                        SetPixel(i_X, Y0 + i_Y, Temp2C, Temp2B, Temp2F, Temp2A);
                    }
                }
            }
            break;
        case 1: // Flip H
            if (IsCharPaint())
            {
                for (int i_Y = Y0; i_Y <= Y1; i_Y++)
                {
                    for (int i_X = 0; i_X < W; i_X++)
                    {
                        Temp1C = CharGet(X0 + i_X, i_Y, true);
                        Temp1B = ColoBGet(X0 + i_X, i_Y, true);
                        Temp1F = ColoFGet(X0 + i_X, i_Y, true);
                        Temp1A = ColoAGet(X0 + i_X, i_Y, true);
                        Temp2C = CharGet(X1 - i_X, i_Y, true);
                        Temp2B = ColoBGet(X1 - i_X, i_Y, true);
                        Temp2F = ColoFGet(X1 - i_X, i_Y, true);
                        Temp2A = ColoAGet(X1 - i_X, i_Y, true);
                        CharPut(X1 - i_X, i_Y, Temp1C, Temp1B, Temp1F, Temp1A);
                        CharPut(X0 + i_X, i_Y, Temp2C, Temp2B, Temp2F, Temp2A);
                    }
                }
            }
            else
            {
                for (int i_Y = Y0; i_Y <= Y1; i_Y++)
                {
                    for (int i_X = 0; i_X < W; i_X++)
                    {
                        Temp1C = GetPixel0(X0 + i_X, i_Y);
                        Temp1B = GetPxlCoB0(X0 + i_X, i_Y);
                        Temp1F = GetPxlCoF0(X0 + i_X, i_Y);
                        Temp1A = GetPxlCoA0(X0 + i_X, i_Y);
                        PixelStackPush();
                        Temp2C = GetPixel0(X1 - i_X, i_Y);
                        Temp2B = GetPxlCoB0(X1 - i_X, i_Y);
                        Temp2F = GetPxlCoF0(X1 - i_X, i_Y);
                        Temp2A = GetPxlCoA0(X1 - i_X, i_Y);
                        SetPixel(X1 - i_X, i_Y, Temp1C, Temp1B, Temp1F, Temp1A);
                        PixelStackPop();
                        SetPixel(X0 + i_X, i_Y, Temp2C, Temp2B, Temp2F, Temp2A);
                    }
                }
            }
            break;
        case 2: // Rotate L
            if ((X1 - X0) == (Y1 - Y0))
            {
                if (IsCharPaint())
                {
                    for (int i_Y = 0; i_Y < H; i_Y++)
                    {
                        for (int i_X = 0; i_X < W; i_X++)
                        {
                            Temp1C = CharGet(X0 + i_X, Y0 + i_Y, true);
                            Temp1B = ColoBGet(X0 + i_X, Y0 + i_Y, true);
                            Temp1F = ColoFGet(X0 + i_X, Y0 + i_Y, true);
                            Temp1A = ColoAGet(X0 + i_X, Y0 + i_Y, true);
                            Temp2C = CharGet(X1 - i_Y, Y0 + i_X, true);
                            Temp2B = ColoBGet(X1 - i_Y, Y0 + i_X, true);
                            Temp2F = ColoFGet(X1 - i_Y, Y0 + i_X, true);
                            Temp2A = ColoAGet(X1 - i_Y, Y0 + i_X, true);
                            Temp3C = CharGet(X1 - i_X, Y1 - i_Y, true);
                            Temp3B = ColoBGet(X1 - i_X, Y1 - i_Y, true);
                            Temp3F = ColoFGet(X1 - i_X, Y1 - i_Y, true);
                            Temp3A = ColoAGet(X1 - i_X, Y1 - i_Y, true);
                            Temp4C = CharGet(X0 + i_Y, Y1 - i_X, true);
                            Temp4B = ColoBGet(X0 + i_Y, Y1 - i_X, true);
                            Temp4F = ColoFGet(X0 + i_Y, Y1 - i_X, true);
                            Temp4A = ColoAGet(X0 + i_Y, Y1 - i_X, true);
                            CharPut(X0 + i_Y, Y1 - i_X, Temp1C, Temp1B, Temp1F, Temp1A);
                            CharPut(X1 - i_X, Y1 - i_Y, Temp4C, Temp4B, Temp4F, Temp4A);
                            CharPut(X1 - i_Y, Y0 + i_X, Temp3C, Temp3B, Temp3F, Temp3A);
                            CharPut(X0 + i_X, Y0 + i_Y, Temp2C, Temp2B, Temp2F, Temp2A);
                        }
                    }
                }
                else
                {
                    for (int i_Y = 0; i_Y < H; i_Y++)
                    {
                        for (int i_X = 0; i_X < W; i_X++)
                        {
                            Temp1C = GetPixel0(X0 + i_X, Y0 + i_Y);
                            Temp1B = GetPxlCoB0(X0 + i_X, Y0 + i_Y);
                            Temp1F = GetPxlCoF0(X0 + i_X, Y0 + i_Y);
                            Temp1A = GetPxlCoA0(X0 + i_X, Y0 + i_Y);
                            PixelStackPush();
                            Temp2C = GetPixel0(X1 - i_Y, Y0 + i_X);
                            Temp2B = GetPxlCoB0(X1 - i_Y, Y0 + i_X);
                            Temp2F = GetPxlCoF0(X1 - i_Y, Y0 + i_X);
                            Temp2A = GetPxlCoA0(X1 - i_Y, Y0 + i_X);
                            PixelStackPush();
                            Temp3C = GetPixel0(X1 - i_X, Y1 - i_Y);
                            Temp3B = GetPxlCoB0(X1 - i_X, Y1 - i_Y);
                            Temp3F = GetPxlCoF0(X1 - i_X, Y1 - i_Y);
                            Temp3A = GetPxlCoA0(X1 - i_X, Y1 - i_Y);
                            PixelStackPush();
                            Temp4C = GetPixel0(X0 + i_Y, Y1 - i_X);
                            Temp4B = GetPxlCoB0(X0 + i_Y, Y1 - i_X);
                            Temp4F = GetPxlCoF0(X0 + i_Y, Y1 - i_X);
                            Temp4A = GetPxlCoA0(X0 + i_Y, Y1 - i_X);
                            SetPixel(X0 + i_Y, Y1 - i_X, Temp1C, Temp1B, Temp1F, Temp1A);
                            PixelStackPop();
                            SetPixel(X1 - i_X, Y1 - i_Y, Temp4C, Temp4B, Temp4F, Temp4A);
                            PixelStackPop();
                            SetPixel(X1 - i_Y, Y0 + i_X, Temp3C, Temp3B, Temp3F, Temp3A);
                            PixelStackPop();
                            SetPixel(X0 + i_X, Y0 + i_Y, Temp2C, Temp2B, Temp2F, Temp2A);
                        }
                    }
                }
            }
            break;
        case 3: // Rotate R
            if ((X1 - X0) == (Y1 - Y0))
            {
                if (IsCharPaint())
                {
                    for (int i_Y = 0; i_Y < H; i_Y++)
                    {
                        for (int i_X = 0; i_X < W; i_X++)
                        {
                            Temp1C = CharGet(X0 + i_X, Y0 + i_Y, true);
                            Temp1B = ColoBGet(X0 + i_X, Y0 + i_Y, true);
                            Temp1F = ColoFGet(X0 + i_X, Y0 + i_Y, true);
                            Temp1A = ColoAGet(X0 + i_X, Y0 + i_Y, true);
                            Temp2C = CharGet(X1 - i_Y, Y0 + i_X, true);
                            Temp2B = ColoBGet(X1 - i_Y, Y0 + i_X, true);
                            Temp2F = ColoFGet(X1 - i_Y, Y0 + i_X, true);
                            Temp2A = ColoAGet(X1 - i_Y, Y0 + i_X, true);
                            Temp3C = CharGet(X1 - i_X, Y1 - i_Y, true);
                            Temp3B = ColoBGet(X1 - i_X, Y1 - i_Y, true);
                            Temp3F = ColoFGet(X1 - i_X, Y1 - i_Y, true);
                            Temp3A = ColoAGet(X1 - i_X, Y1 - i_Y, true);
                            Temp4C = CharGet(X0 + i_Y, Y1 - i_X, true);
                            Temp4B = ColoBGet(X0 + i_Y, Y1 - i_X, true);
                            Temp4F = ColoFGet(X0 + i_Y, Y1 - i_X, true);
                            Temp4A = ColoAGet(X0 + i_Y, Y1 - i_X, true);
                            CharPut(X0 + i_Y, Y1 - i_X, Temp3C, Temp3B, Temp3F, Temp3A);
                            CharPut(X1 - i_X, Y1 - i_Y, Temp2C, Temp2B, Temp2F, Temp2A);
                            CharPut(X1 - i_Y, Y0 + i_X, Temp1C, Temp1B, Temp1F, Temp1A);
                            CharPut(X0 + i_X, Y0 + i_Y, Temp4C, Temp4B, Temp4F, Temp4A);
                        }
                    }
                }
                else
                {
                    for (int i_Y = 0; i_Y < H; i_Y++)
                    {
                        for (int i_X = 0; i_X < W; i_X++)
                        {
                            Temp1C = GetPixel0(X0 + i_X, Y0 + i_Y);
                            Temp1B = GetPxlCoB0(X0 + i_X, Y0 + i_Y);
                            Temp1F = GetPxlCoF0(X0 + i_X, Y0 + i_Y);
                            Temp1A = GetPxlCoA0(X0 + i_X, Y0 + i_Y);
                            PixelStackPush();
                            Temp2C = GetPixel0(X1 - i_Y, Y0 + i_X);
                            Temp2B = GetPxlCoB0(X1 - i_Y, Y0 + i_X);
                            Temp2F = GetPxlCoF0(X1 - i_Y, Y0 + i_X);
                            Temp2A = GetPxlCoA0(X1 - i_Y, Y0 + i_X);
                            PixelStackPush();
                            Temp3C = GetPixel0(X1 - i_X, Y1 - i_Y);
                            Temp3B = GetPxlCoB0(X1 - i_X, Y1 - i_Y);
                            Temp3F = GetPxlCoF0(X1 - i_X, Y1 - i_Y);
                            Temp3A = GetPxlCoA0(X1 - i_X, Y1 - i_Y);
                            PixelStackPush();
                            Temp4C = GetPixel0(X0 + i_Y, Y1 - i_X);
                            Temp4B = GetPxlCoB0(X0 + i_Y, Y1 - i_X);
                            Temp4F = GetPxlCoF0(X0 + i_Y, Y1 - i_X);
                            Temp4A = GetPxlCoA0(X0 + i_Y, Y1 - i_X);
                            SetPixel(X0 + i_Y, Y1 - i_X, Temp3C, Temp3B, Temp3F, Temp3A);
                            PixelStackPop();
                            SetPixel(X1 - i_X, Y1 - i_Y, Temp2C, Temp2B, Temp2F, Temp2A);
                            PixelStackPop();
                            SetPixel(X1 - i_Y, Y0 + i_X, Temp1C, Temp1B, Temp1F, Temp1A);
                            PixelStackPop();
                            SetPixel(X0 + i_X, Y0 + i_Y, Temp4C, Temp4B, Temp4F, Temp4A);
                        }
                    }
                }
            }
            break;
    }
}

void EditorPixelPaint::PaintMove(int Direction)
{
    int X0 = PPS.CanvasX;
    int Y0 = PPS.CanvasY;
    int X1 = PPS.CanvasX + PPS.SizeX;
    int Y1 = PPS.CanvasY + PPS.SizeY;
    if (X1 < X0) { int _ = X0; X0 = X1; X1 = _; }
    if (Y1 < Y0) { int _ = Y0; Y0 = Y1; Y1 = _; }
    if (PPS.PaintMoveRoll == 4)
    {
        PaintFlipRot(Direction);
        return;
    }
    int X0_ = X0;
    int Y0_ = Y0;
    int X1_ = X1;
    int Y1_ = Y1;
    if (IsCharPaint())
    {
        X0 = X0 * PPS.FontW + PPS.CanvasXBase;
        Y0 = Y0 * PPS.FontH + PPS.CanvasYBase;
        X1 = X1 * PPS.FontW + PPS.CanvasXBase;
        Y1 = Y1 * PPS.FontH + PPS.CanvasYBase;
        if ((Direction == 0) || (Direction == 1))
        {
            X1 = X1 + (PPS.FontW - 1);
        }
        if ((Direction == 2) || (Direction == 3))
        {
            Y1 = Y1 + (PPS.FontH - 1);
        }
    }
    std::vector<std::vector<int>> PixelEdgeC;
    std::vector<std::vector<int>> PixelEdgeB;
    std::vector<std::vector<int>> PixelEdgeF;
    std::vector<std::vector<int>> PixelEdgeA;
    int TempC, TempB, TempF, TempA;
    std::vector<std::vector<AnsiLineOccupyItem>> PixelEdge;

    switch (Direction)
    {
        case 0: // Up
            if (IsCharPaint())
            {
                CoreStatic::Array2(PixelEdge, PPS.FontH, X1 - X0 + 1);

                for (int i = X0; i <= X1; i++)
                {
                    for (int i_ = 0; i_ < PPS.FontH; i_++)
                    {
                        PixelEdge[i_][i - X0] = EditorData_.get()->ElementGetObj(i, Y0 + i_, true, true);
                    }
                }
                for (int ii = Y0; ii < Y1; ii += PPS.FontH)
                {
                    for (int i = X0; i <= X1; i++)
                    {
                        for (int i_ = 0; i_ < PPS.FontH; i_++)
                        {
                            EditorData_.get()->CharPut(i, ii + i_, EditorData_.get()->ElementGetObj(i, ii + i_ + PPS.FontH, true, true), true);
                        }
                    }
                }
                if (PPS.PaintMoveRoll == 1)
                {
                    for (int i = X0; i <= X1; i++)
                    {
                        for (int i_ = 0; i_ < PPS.FontH; i_++)
                        {
                            EditorData_.get()->CharPut(i, Y1 + i_, PixelEdge[i_][i - X0], true);
                        }
                    }
                }
            }
            else
            {
                CoreStatic::Array2(PixelEdgeC, PPS.FontH, X1 - X0 + 1);
                CoreStatic::Array2(PixelEdgeB, PPS.FontH, X1 - X0 + 1);
                CoreStatic::Array2(PixelEdgeF, PPS.FontH, X1 - X0 + 1);
                CoreStatic::Array2(PixelEdgeA, PPS.FontH, X1 - X0 + 1);

                for (int i = X0; i <= X1; i++)
                {
                    PixelEdgeC[0][i - X0] = GetPixel0(i, Y0);
                    PixelEdgeB[0][i - X0] = GetPxlCoB(i, Y0);
                    PixelEdgeF[0][i - X0] = GetPxlCoF(i, Y0);
                    PixelEdgeA[0][i - X0] = GetPxlCoA0(i, Y0);
                }
                for (int ii = Y0; ii < Y1; ii++)
                {
                    for (int i = X0; i <= X1; i++)
                    {
                        TempC = GetPixel0(i, ii + 1);
                        TempB = GetPxlCoB(i, ii + 1);
                        TempF = GetPxlCoF(i, ii + 1);
                        TempA = GetPxlCoA0(i, ii + 1);
                        GetPixel(i, ii);
                        SetPixel(i, ii, TempC, TempB, TempF, TempA);
                    }
                }
                if (PPS.PaintMoveRoll == 1)
                {
                    for (int i = X0; i <= X1; i++)
                    {
                        GetPixel(i, Y1);
                        SetPixel(i, Y1, PixelEdgeC[0][i - X0], PixelEdgeB[0][i - X0], PixelEdgeF[0][i - X0], PixelEdgeA[0][i - X0]);
                    }
                }
            }
            if (PPS.PaintMoveRoll == 2)
            {
                for (int i = X0_; i <= X1_; i++)
                {
                    GetPixel(i, Y1_);
                    SetPixel(i, Y1_, 0, EditorData_.get()->DrawColoBI, EditorData_.get()->DrawColoFI, EditorData_.get()->DrawColoAI);
                }
            }
            if (PPS.PaintMoveRoll == 3)
            {
                for (int i = X0_; i <= X1_; i++)
                {
                    GetPixel(i, Y1_);
                    SetPixel(i, Y1_, 1, EditorData_.get()->DrawColoBI, EditorData_.get()->DrawColoFI, EditorData_.get()->DrawColoAI);
                }
            }
            break;
        case 1: // Down
            if (IsCharPaint())
            {
                CoreStatic::Array2(PixelEdge, PPS.FontH, X1 - X0 + 1);

                for (int i = X0; i <= X1; i++)
                {
                    for (int i_ = 0; i_ < PPS.FontH; i_++)
                    {
                        PixelEdge[i_][i - X0] = EditorData_.get()->ElementGetObj(i, Y1 + i_, true, true);
                    }
                }
                for (int ii = Y1; ii > Y0; ii -= PPS.FontH)
                {
                    for (int i = X0; i <= X1; i++)
                    {
                        for (int i_ = 0; i_ < PPS.FontH; i_++)
                        {
                            EditorData_.get()->CharPut(i, ii + i_, EditorData_.get()->ElementGetObj(i, ii + i_ - PPS.FontH, true, true), true);
                        }
                    }
                }
                if (PPS.PaintMoveRoll == 1)
                {
                    for (int i = X0; i <= X1; i++)
                    {
                        for (int i_ = 0; i_ < PPS.FontH; i_++)
                        {
                            EditorData_.get()->CharPut(i, Y0 + i_, PixelEdge[i_][i - X0], true);
                        }
                    }
                }
            }
            else
            {
                CoreStatic::Array2(PixelEdgeC, PPS.FontH, X1 - X0 + 1);
                CoreStatic::Array2(PixelEdgeB, PPS.FontH, X1 - X0 + 1);
                CoreStatic::Array2(PixelEdgeF, PPS.FontH, X1 - X0 + 1);
                CoreStatic::Array2(PixelEdgeA, PPS.FontH, X1 - X0 + 1);

                for (int i = X0; i <= X1; i++)
                {
                    PixelEdgeC[0][i - X0] = GetPixel0(i, Y1);
                    PixelEdgeB[0][i - X0] = GetPxlCoB(i, Y1);
                    PixelEdgeF[0][i - X0] = GetPxlCoF(i, Y1);
                    PixelEdgeA[0][i - X0] = GetPxlCoA0(i, Y1);
                }
                for (int ii = Y1; ii > Y0; ii--)
                {
                    for (int i = X0; i <= X1; i++)
                    {
                        TempC = GetPixel0(i, ii - 1);
                        TempB = GetPxlCoB(i, ii - 1);
                        TempF = GetPxlCoF(i, ii - 1);
                        TempA = GetPxlCoA0(i, ii - 1);
                        GetPixel(i, ii);
                        SetPixel(i, ii, TempC, TempB, TempF, TempA);
                    }
                }
                if (PPS.PaintMoveRoll == 1)
                {
                    for (int i = X0; i <= X1; i++)
                    {
                        GetPixel(i, Y0);
                        SetPixel(i, Y0, PixelEdgeC[0][i - X0], PixelEdgeB[0][i - X0], PixelEdgeF[0][i - X0], PixelEdgeA[0][i - X0]);
                    }
                }
            }
            if (PPS.PaintMoveRoll == 2)
            {
                for (int i = X0_; i <= X1_; i++)
                {
                    GetPixel(i, Y0_);
                    SetPixel(i, Y0_, 0, EditorData_.get()->DrawColoBI, EditorData_.get()->DrawColoFI, EditorData_.get()->DrawColoAI);
                }
            }
            if (PPS.PaintMoveRoll == 3)
            {
                for (int i = X0_; i <= X1_; i++)
                {
                    GetPixel(i, Y0_);
                    SetPixel(i, Y0_, 1, EditorData_.get()->DrawColoBI, EditorData_.get()->DrawColoFI, EditorData_.get()->DrawColoAI);
                }
            }
            break;
        case 2: // Left
            if (IsCharPaint())
            {
                CoreStatic::Array2(PixelEdge, PPS.FontW, Y1 - Y0 + 1);

                for (int i = Y0; i <= Y1; i++)
                {
                    for (int i_ = 0; i_ < PPS.FontW; i_++)
                    {
                        PixelEdge[i_][i - Y0] = EditorData_.get()->ElementGetObj(X0 + i_, i, true, true);
                    }
                }
                for (int ii = X0; ii < X1; ii += PPS.FontW)
                {
                    for (int i = Y0; i <= Y1; i++)
                    {
                        for (int i_ = 0; i_ < PPS.FontW; i_++)
                        {
                            EditorData_.get()->CharPut(ii + i_, i, EditorData_.get()->ElementGetObj(ii + i_ + PPS.FontW, i, true, true), true);
                        }
                    }
                }
                if (PPS.PaintMoveRoll == 1)
                {
                    for (int i = Y0; i <= Y1; i++)
                    {
                        for (int i_ = 0; i_ < PPS.FontW; i_++)
                        {
                            EditorData_.get()->CharPut(X1 + i_, i, PixelEdge[i_][i - Y0], true);
                        }
                    }
                }
            }
            else
            {
                CoreStatic::Array2(PixelEdgeC, PPS.FontW, Y1 - Y0 + 1);
                CoreStatic::Array2(PixelEdgeB, PPS.FontW, Y1 - Y0 + 1);
                CoreStatic::Array2(PixelEdgeF, PPS.FontW, Y1 - Y0 + 1);
                CoreStatic::Array2(PixelEdgeA, PPS.FontW, Y1 - Y0 + 1);

                for (int i = Y0; i <= Y1; i++)
                {
                    PixelEdgeC[0][i - Y0] = GetPixel0(X0, i);
                    PixelEdgeB[0][i - Y0] = GetPxlCoB(X0, i);
                    PixelEdgeF[0][i - Y0] = GetPxlCoF(X0, i);
                    PixelEdgeA[0][i - Y0] = GetPxlCoA0(X0, i);
                }
                for (int ii = X0; ii < X1; ii++)
                {
                    for (int i = Y0; i <= Y1; i++)
                    {
                        TempC = GetPixel0(ii + 1, i);
                        TempB = GetPxlCoB(ii + 1, i);
                        TempF = GetPxlCoF(ii + 1, i);
                        TempA = GetPxlCoA0(ii + 1, i);
                        GetPixel(ii, i);
                        SetPixel(ii, i, TempC, TempB, TempF, TempA);
                    }
                }
                if (PPS.PaintMoveRoll == 1)
                {
                    for (int i = Y0; i <= Y1; i++)
                    {
                        GetPixel(X1, i);
                        SetPixel(X1, i, PixelEdgeC[0][i - Y0], PixelEdgeB[0][i - Y0], PixelEdgeF[0][i - Y0], PixelEdgeA[0][i - Y0]);
                    }
                }
            }
            if (PPS.PaintMoveRoll == 2)
            {
                for (int i = Y0_; i <= Y1_; i++)
                {
                    GetPixel(X1_, i);
                    SetPixel(X1_, i, 0, EditorData_.get()->DrawColoBI, EditorData_.get()->DrawColoFI, EditorData_.get()->DrawColoAI);
                }
            }
            if (PPS.PaintMoveRoll == 3)
            {
                for (int i = Y0_; i <= Y1_; i++)
                {
                    GetPixel(X1_, i);
                    SetPixel(X1_, i, 1, EditorData_.get()->DrawColoBI, EditorData_.get()->DrawColoFI, EditorData_.get()->DrawColoAI);
                }
            }
            break;
        case 3: // Right
            if (IsCharPaint())
            {
                CoreStatic::Array2(PixelEdge, PPS.FontW, Y1 - Y0 + 1);

                for (int i = Y0; i <= Y1; i++)
                {
                    for (int i_ = 0; i_ < PPS.FontW; i_++)
                    {
                        PixelEdge[i_][i - Y0] = EditorData_.get()->ElementGetObj(X1 + i_, i, true, true);
                    }
                }
                for (int ii = X1; ii > X0; ii -= PPS.FontW)
                {
                    for (int i = Y0; i <= Y1; i++)
                    {
                        for (int i_ = 0; i_ < PPS.FontW; i_++)
                        {
                            EditorData_.get()->CharPut(ii + i_, i, EditorData_.get()->ElementGetObj(ii + i_ - PPS.FontW, i, true, true), true);
                        }
                    }
                }
                if (PPS.PaintMoveRoll == 1)
                {
                    for (int i = Y0; i <= Y1; i++)
                    {
                        for (int i_ = 0; i_ < PPS.FontW; i_++)
                        {
                            EditorData_.get()->CharPut(X0 + i_, i, PixelEdge[i_][i - Y0], true);
                        }
                    }
                }
            }
            else
            {
                CoreStatic::Array2(PixelEdgeC, PPS.FontW, Y1 - Y0 + 1);
                CoreStatic::Array2(PixelEdgeB, PPS.FontW, Y1 - Y0 + 1);
                CoreStatic::Array2(PixelEdgeF, PPS.FontW, Y1 - Y0 + 1);
                CoreStatic::Array2(PixelEdgeA, PPS.FontW, Y1 - Y0 + 1);

                for (int i = Y0; i <= Y1; i++)
                {
                    PixelEdgeC[0][i - Y0] = GetPixel0(X1, i);
                    PixelEdgeB[0][i - Y0] = GetPxlCoB(X1, i);
                    PixelEdgeF[0][i - Y0] = GetPxlCoF(X1, i);
                    PixelEdgeA[0][i - Y0] = GetPxlCoA0(X1, i);
                }
                for (int ii = X1; ii > X0; ii--)
                {
                    for (int i = Y0; i <= Y1; i++)
                    {
                        TempC = GetPixel0(ii - 1, i);
                        TempB = GetPxlCoB(ii - 1, i);
                        TempF = GetPxlCoF(ii - 1, i);
                        TempA = GetPxlCoA0(ii - 1, i);
                        SetPixel(ii, i);
                        SetPixel(ii, i, TempC, TempB, TempF, TempA);
                    }
                }
                if (PPS.PaintMoveRoll == 1)
                {
                    for (int i = Y0; i <= Y1; i++)
                    {
                        GetPixel(X0, i);
                        SetPixel(X0, i, PixelEdgeC[0][i - Y0], PixelEdgeB[0][i - Y0], PixelEdgeF[0][i - Y0], PixelEdgeA[0][i - Y0]);
                    }
                }
            }
            if (PPS.PaintMoveRoll == 2)
            {
                for (int i = Y0_; i <= Y1_; i++)
                {
                    GetPixel(X0_, i);
                    SetPixel(X0_, i, 0, EditorData_.get()->DrawColoBI, EditorData_.get()->DrawColoFI, EditorData_.get()->DrawColoAI);
                }
            }
            if (PPS.PaintMoveRoll == 3)
            {
                for (int i = Y0_; i <= Y1_; i++)
                {
                    GetPixel(X0_, i);
                    SetPixel(X0_, i, 1, EditorData_.get()->DrawColoBI, EditorData_.get()->DrawColoFI, EditorData_.get()->DrawColoAI);
                }
            }
            break;
    }
}

void EditorPixelPaint::PaintInvert()
{
    int X0 = PPS.CanvasX;
    int Y0 = PPS.CanvasY;
    int X1 = PPS.CanvasX + PPS.SizeX;
    int Y1 = PPS.CanvasY + PPS.SizeY;
    if (X1 < X0) { int _ = X0; X0 = X1; X1 = _; }
    if (Y1 < Y0) { int _ = Y0; Y0 = Y1; Y1 = _; }

    for (int i_Y = Y0; i_Y <= Y1; i_Y++)
    {
        for (int i_X = X0; i_X <= X1; i_X++)
        {
            if (GetPixel(i_X, i_Y) == 1)
            {
                SetPixel(i_X, i_Y, 0, EditorData_.get()->DrawColoBI, EditorData_.get()->DrawColoFI, EditorData_.get()->DrawColoAI);
            }
            else
            {
                SetPixel(i_X, i_Y, 1, EditorData_.get()->DrawColoBI, EditorData_.get()->DrawColoFI, EditorData_.get()->DrawColoAI);
            }
        }
    }
}

void EditorPixelPaint::ClipboardCopy()
{
    int X0 = PPS.CanvasX;
    int Y0 = PPS.CanvasY;
    int X1 = PPS.CanvasX + PPS.SizeX;
    int Y1 = PPS.CanvasY + PPS.SizeY;
    if (X1 < X0) { int _ = X0; X0 = X1; X1 = _; }
    if (Y1 < Y0) { int _ = Y0; Y0 = Y1; Y1 = _; }

    EditorClipboard_.get()->TextClipboard.Clear();
    for (int i_Y = Y0; i_Y <= Y1; i_Y++)
    {
        for (int i_X = X0; i_X <= X1; i_X++)
        {
            EditorClipboard_.get()->TextClipboard.BlankChar();
            EditorClipboard_.get()->TextClipboard.Item_ColorB = GetPxlCoB(i_X, i_Y);
            EditorClipboard_.get()->TextClipboard.Item_ColorF = GetPxlCoF(i_X, i_Y);
            if (GetPixel(i_X, i_Y) == 1)
            {
                EditorClipboard_.get()->TextClipboard.Item_Char = PaintMode_[0].IntToChar[1];
            }
            else
            {
                EditorClipboard_.get()->TextClipboard.Item_Char = PaintMode_[0].IntToChar[0];
            }
            EditorClipboard_.get()->TextClipboard.Set_(i_Y - Y0, i_X - X0);
        }
    }
    EditorClipboard_.get()->SysClipboardSet();
}

void EditorPixelPaint::ClipboardPaste()
{
    int X0 = PPS.CanvasX;
    int Y0 = PPS.CanvasY;
    int X1 = PPS.CanvasX + PPS.SizeX;
    int Y1 = PPS.CanvasY + PPS.SizeY;
    if (X1 < X0) { int _ = X0; X0 = X1; X1 = _; }
    if (Y1 < Y0) { int _ = Y0; Y0 = Y1; Y1 = _; }

    if (EditorClipboard_.get()->SysClipboardGet())
    {
        for (int i_Y = Y0; i_Y <= Y1; i_Y++)
        {
            for (int i_X = X0; i_X <= X1; i_X++)
            {
                if (EditorClipboard_.get()->TextClipboard.Get_(i_Y - Y0, i_X - X0))
                {
                    if (EditorClipboard_.get()->TextClipboard.Item_Char == PaintMode_[0].IntToChar[0])
                    {
                        GetPixel(i_X, i_Y);
                        SetPixel(i_X, i_Y, 0, EditorClipboard_.get()->TextClipboard.Item_ColorB, EditorClipboard_.get()->TextClipboard.Item_ColorF, EditorClipboard_.get()->TextClipboard.Item_ColorA);
                    }
                    if (EditorClipboard_.get()->TextClipboard.Item_Char == PaintMode_[0].IntToChar[1])
                    {
                        GetPixel(i_X, i_Y);
                        SetPixel(i_X, i_Y, 1, EditorClipboard_.get()->TextClipboard.Item_ColorB, EditorClipboard_.get()->TextClipboard.Item_ColorF, EditorClipboard_.get()->TextClipboard.Item_ColorA);
                    }
                }
            }
        }
    }
}

void EditorPixelPaint::UndoRedoBegin()
{
    if (UndoRedoItemOld.Count >= UndoRedoItemIndex)
    {
        UndoRedoItemOld.RemoveRange(UndoRedoItemIndex);
        UndoRedoItemNew.RemoveRange(UndoRedoItemIndex);
    }
    UndoRedoItemOld.Add(PPS.GetState());
}

void EditorPixelPaint::UndoRedoEnd()
{
    UndoRedoItemNew.Add(PPS.GetState());
    UndoRedoItemIndex++;
}

void EditorPixelPaint::UndoRedoUndo()
{
    if (UndoRedoItemIndex > 0)
    {
        UndoRedoItemIndex--;
        PPS.SetState(UndoRedoItemOld[UndoRedoItemIndex]);
    }
}

void EditorPixelPaint::UndoRedoRedo()
{
    if (UndoRedoItemIndex < UndoRedoItemNew.Count)
    {
        PPS.SetState(UndoRedoItemNew[UndoRedoItemIndex]);
        UndoRedoItemIndex++;
    }
}
