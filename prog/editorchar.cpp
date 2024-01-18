#include "editorchar.h"

EditorChar::EditorChar()
{
    for (int I = 0; I < 256; I++)
    {
        FavChar[I] = 32;
    }
}

void EditorChar::Init(std::shared_ptr<ConfigFile> CF_)
{
    CF = CF_;

    for (int I = 0; I < 256; I++)
    {
        if (CF.get()->ParamExists("FavChar" + std::to_string(I)))
        {
            FavChar[I] = CF.get()->ParamGetI("FavChar" + std::to_string(I));
        }
        else
        {
            FavChar[I] = 32;
        }
    }
}

void EditorChar::EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    switch (_(KeyName.c_str()))
    {
        default:
            RepaintDepth = 99;
            if (SelectorState == 1)
            {
                if (((int)KeyChar >= 32) && ((int)KeyChar <= 255))
                {
                    int C = SelectChar;
                    if (SelectChar >= 0)
                    {
                        FavChar[KeyChar] = SelectChar;
                        CF.get()->ParamSet("FavChar" + std::to_string(KeyChar), std::to_string(SelectChar));
                        BinaryFile_.get()->SaveFromStringConfig(CF.get()->FileSave(0));
                        BinaryFile_.get()->SysSaveConfig();

                        RepaintDepth = 3;
                    }
                    else
                    {
                        SelectCharFavSet(KeyChar);
                        RepaintDepth = 1;
                    }
                }
            }
            if (SelectorState == 2)
            {
                switch (_(KeyName.c_str()))
                {
                    case _("Digit1"):
                        SelectColoA = CoreStatic::SetAttribBit(SelectColoA, 0, !CoreStatic::GetAttribBit(SelectColoA, 0));
                        RepaintDepth = 3;
                        return;
                    case _("Digit2"):
                        SelectColoA = CoreStatic::SetAttribBit(SelectColoA, 1, !CoreStatic::GetAttribBit(SelectColoA, 1));
                        RepaintDepth = 3;
                        return;
                    case _("Digit3"):
                        SelectColoA = CoreStatic::SetAttribBit(SelectColoA, 2, !CoreStatic::GetAttribBit(SelectColoA, 2));
                        RepaintDepth = 3;
                        return;
                    case _("Digit4"):
                        SelectColoA = CoreStatic::SetAttribBit(SelectColoA, 6, !CoreStatic::GetAttribBit(SelectColoA, 6));
                        RepaintDepth = 3;
                        return;
                    case _("Digit5"):
                        SelectColoA = CoreStatic::SetAttribBit(SelectColoA, 3, !CoreStatic::GetAttribBit(SelectColoA, 3));
                        RepaintDepth = 3;
                        return;
                    case _("Digit6"):
                        SelectColoA = CoreStatic::SetAttribBit(SelectColoA, 4, !CoreStatic::GetAttribBit(SelectColoA, 4));
                        RepaintDepth = 3;
                        return;
                    case _("Digit7"):
                        SelectColoA = CoreStatic::SetAttribBit(SelectColoA, 5, !CoreStatic::GetAttribBit(SelectColoA, 5));
                        RepaintDepth = 3;
                        return;
                }
            }
            break;
        case _("Resize"):
            /*SelectCharClose(2);
            Core_.ScreenRefresh(false);
            SelectCharInit();*/
            break;
        case _("ArrowUp"):
            SelectCharChange(-2);
            break;
        case _("ArrowDown"):
            SelectCharChange(2);
            break;
        case _("ArrowLeft"):
            SelectCharChange(-1);
            break;
        case _("ArrowRight"):
            SelectCharChange(1);
            break;
        case _("PageUp"):
            SelectCharChange(-3);
            break;
        case _("PageDown"):
            SelectCharChange(3);
            break;
        case _("Home"):
            SelectCharChange(-4);
            break;
        case _("End"):
            SelectCharChange(4);
            break;
        case _("F1"):
            SelectCharChange(-5);
            break;
        case _("F2"):
            SelectCharChange(5);
            break;
        case _("F3"):
            RepaintDepth = 1;
            if (SelectorState < 2)
            {
                SelectorState++;
            }
            else
            {
                SelectorState = 1;
            }
            break;
        case _("F4"):
            if (SelectorState == 1)
            {
                RepaintDepth = 3;
                WinBitmapEnabled = !WinBitmapEnabled;
            }
            if (SelectorState == 2)
            {
                //DisplayConfigOpen();
            }
            break;
        case _("Backspace"):
            if ((SelectChar >= 0) || (SelectToFav < 0))
            {
                SelectToFav = SelectChar;
            }
            else
            {
                if ((SelectChar < 0) && (SelectToFav >= 0))
                {
                    FavChar[SelectCharFavGet()] = SelectToFav;
                    CF.get()->ParamSet("FavChar" + std::to_string(SelectCharFavGet()), std::to_string(SelectToFav));
                    BinaryFile_.get()->SaveFromStringConfig(CF.get()->FileSave(0));
                    BinaryFile_.get()->SysSaveConfig();
                    SelectToFav = -1;
                }
            }
            RepaintDepth = 1;
            return;
        case _("Escape"):
            RepaintDepth = 101;
            return;
        case _("Enter"):
            RepaintDepth = 102;
            return;
        /*case "WindowClose":
            SelectCharClose(3);
            Core_.PixelCharGet();
            Core_.ScreenRefresh(true);
            return;*/
        case _("Tab"):
            RepaintDepth = 0;
            CharPosMode++;
            if (CharPosMode == 4)
            {
                CharPosMode = 0;
            }
            return;
        case _("Insert"):
            {
                if (SelectorState == 1)
                {
                    if (SelectChar >= 0)
                    {
                        FavPage = true;
                        int C = SelectChar;
                        SelectCharFavSet(0);
                        for (int i = 0; i < 256; i++)
                        {
                            if (FavChar[i] == C)
                            {
                                SelectCharFavSet(i);
                                break;
                            }
                        }
                    }
                    else
                    {
                        FavPage = false;
                        SelectChar = FavChar[SelectCharFavGet()];
                    }
                }
                if (SelectorState == 2)
                {
                    if (EditorData_.get()->ToggleDrawText && EditorData_.get()->ToggleDrawColo)
                    {
                        EditorData_.get()->ToggleDrawColo = false;
                    }
                    else
                    {
                        if (EditorData_.get()->ToggleDrawText)
                        {
                            EditorData_.get()->ToggleDrawText = false;
                            EditorData_.get()->ToggleDrawColo = true;
                        }
                        else
                        {
                            EditorData_.get()->ToggleDrawText = true;
                            EditorData_.get()->ToggleDrawColo = true;
                        }
                    }
                }
                RepaintDepth = 1;
            }
            return;
        case _("Delete"):
            {
                if (SelectorState == 1)
                {
                    if ((EditorData_.get()->CursorChar >= 0) && (EditorData_.get()->CursorChar < MaxCharCode))
                    {
                        SelectChar = EditorData_.get()->CursorChar;
                    }
                }
                if (SelectorState == 2)
                {
                    SelectColoB = EditorData_.get()->CursorColoB;
                    SelectColoF = EditorData_.get()->CursorColoF;
                    SelectColoA = EditorData_.get()->CursorColoA;
                }
                RepaintDepth = 1;
            }
            return;
    }
}

int EditorChar::WinBitmapNearest(int CharCode, bool Backward)
{
    if (WinBitmapEnabled)
    {
        int CharPage = CharCode >> 8;
        if (Screen::BitmapPage.Count > 0)
        {
            int PageMin = Screen::BitmapPage[0];
            int PageMax = Screen::BitmapPage[Screen::BitmapPage.Count - 1];
            if ((CharPage > PageMin) && (CharPage < PageMax))
            {
                int PagePos = Screen::BitmapPage.IndexOfBin(CharPage);
                if (Backward)
                {
                    if (PagePos < 0)
                    {
                        CharPage = Screen::BitmapPage[(0 - PagePos) - 1];
                    }
                }
                else
                {
                    if (PagePos < 0)
                    {
                        CharPage = Screen::BitmapPage[(0 - PagePos)];
                    }
                }
            }
            if (CharPage < PageMin)
            {
                if (Backward)
                {
                    CharPage = PageMax;
                }
                else
                {
                    CharPage = PageMin;
                }
            }
            if (CharPage > PageMax)
            {
                if (Backward)
                {
                    CharPage = PageMax;
                }
                else
                {
                    CharPage = PageMin;
                }
            }
        }
        else
        {
            if ((CharPage >= 0xD8) && (CharPage <= 0xDF))
            {
                if (Backward)
                {
                    CharPage = 0xD7;
                }
                else
                {
                    CharPage = 0xE0;
                }
            }
        }
        CharCode = (CharPage << 8) + (CharCode & 255);
    }
    return CharCode;
}

int EditorChar::SelectCharCode()
{
    if (FavPage)
    {
        return FavChar[SelectCharFavGet()];
    }
    else
    {
        return SelectChar;
    }
}

int EditorChar::SelectCharPart(int N)
{
    if (FavPage)
    {
        switch (N)
        {
            case 0:
                return SelectCharFavGet() & 255;
            case 1:
                return SelectCharFavGet() >> 8;
            case 2:
                return SelectCharFavGet() & 15;
            case 3:
                return (SelectCharFavGet() >> 4) & 15;
        }
    }
    else
    {
        switch (N)
        {
            case 0:
                return SelectChar & 255;
            case 1:
                return SelectChar >> 8;
            case 2:
                return SelectChar & 15;
            case 3:
                return (SelectChar >> 4) & 15;
        }
    }
    return SelectChar;
}

void EditorChar::SelectCharFavSet(int N)
{
    SelectChar = 0 - (N + 256);
}

int EditorChar::SelectCharFavGet()
{
    return (0 - SelectChar) - 256;
}

void EditorChar::SelectCharChange(int T)
{
    if (SelectorState == 1)
    {
        int Char0X = SelectCharPart(2);
        int Char0Y = SelectCharPart(3);
        int Page0 = SelectCharPart(1);
        int SelectChar0 = SelectChar;
        switch (T)
        {
            case -1:
                if (FavPage)
                {
                    SelectChar += 1;
                }
                else
                {
                    SelectChar -= 1;
                }
                break;
            case 1:
                if (FavPage)
                {
                    SelectChar -= 1;
                }
                else
                {
                    SelectChar += 1;
                }
                break;
            case -2:
                if (FavPage)
                {
                    SelectChar += 16;
                }
                else
                {
                    SelectChar -= 16;
                }
                break;
            case 2:
                if (FavPage)
                {
                    SelectChar -= 16;
                }
                else
                {
                    SelectChar += 16;
                }
                break;
            case -3:
                if (!FavPage)
                {
                    SelectChar -= 256;
                }
                break;
            case 3:
                if (!FavPage)
                {
                    SelectChar += 256;
                }
                break;
            case -4:
                if (!FavPage)
                {
                    SelectChar -= 4096;
                }
                break;
            case 4:
                if (!FavPage)
                {
                    SelectChar += 4096;
                }
                break;
            case -5:
                if (!FavPage)
                {
                    SelectChar -= 65536;
                }
                break;
            case 5:
                if (!FavPage)
                {
                    SelectChar += 65536;
                }
                break;
        }
        if (FavPage)
        {
            while (SelectChar < (-511))
            {
                SelectChar += 256;
            }
            while (SelectChar > (-256))
            {
                SelectChar -= 256;
            }
        }
        else
        {
            while (SelectChar < 0)
            {
                SelectChar += MaxCharCode;
            }
            while (SelectChar >= MaxCharCode)
            {
                SelectChar -= MaxCharCode;
            }
            if (T != 0)
            {
                if ((SelectChar0 >> 8) != (SelectChar >> 8))
                {
                    SelectChar = WinBitmapNearest(SelectChar, (T < 0));
                }
            }
        }
        if (Page0 != SelectCharPart(1))
        {
            RepaintDepth = 1;
        }
        else
        {
            RepaintDepth = 3;

            int CharX = SelectCharPart(2);
            int CharY = SelectCharPart(3);
            if ((Char0X == 0) && (CharX == 15))
            {
                RepaintDepth = 1;
            }
            if ((Char0X == 15) && (CharX == 0))
            {
                RepaintDepth = 1;
            }
            if ((Char0Y == 0) && (CharY == 15))
            {
                RepaintDepth = 1;
            }
            if ((Char0Y == 15) && (CharY == 0))
            {
                RepaintDepth = 1;
            }
        }
    }
    if (SelectorState == 2)
    {
        switch (T)
        {
            case -1:
                SelectColoF--;
                if (SelectColoF == -2) { SelectColoF = 15; }
                break;
            case 1:
                SelectColoF++;
                if (SelectColoF == 16) { SelectColoF = -1; }
                break;
            case -2:
                SelectColoB--;
                if (SelectColoB == -2) { SelectColoB = 15; }
                break;
            case 2:
                SelectColoB++;
                if (SelectColoB == 16) { SelectColoB = -1; }
                break;
            case 3:
                CoreAnsi_.get()->AnsiMaxX++;
                break;
            case -3:
                if (CoreAnsi_.get()->AnsiMaxX > 0) { CoreAnsi_.get()->AnsiMaxX--; }
                break;
            case 4:
                CoreAnsi_.get()->AnsiMaxY++;
                break;
            case -4:
                if (CoreAnsi_.get()->AnsiMaxY > 0) { CoreAnsi_.get()->AnsiMaxY--; }
                break;
            case 5:
                CoreAnsi_.get()->ANSI_LF++;
                if (CoreAnsi_.get()->ANSI_LF == 3)
                {
                    CoreAnsi_.get()->ANSI_LF = 0;
                }
                break;
            case -5:
                CoreAnsi_.get()->ANSI_CR++;
                if (CoreAnsi_.get()->ANSI_CR == 3)
                {
                    CoreAnsi_.get()->ANSI_CR = 0;
                }
                break;
        }
        RepaintDepth = 3;
    }
}
