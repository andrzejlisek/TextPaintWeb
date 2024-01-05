#include "terminalmouse.h"

TerminalMouse::TerminalMouse()
{

}

void TerminalMouse::MouseSet(int Param, bool Value)
{
    switch (Param)
    {
        case 9: Mouse0009 = Value; break;
        case 1000: Mouse1000 = Value; break;
        case 1001: Mouse1001 = Value; break;
        case 1002: Mouse1002 = Value; break;
        case 1003: Mouse1003 = Value; break;
        case 1016: Mouse1016 = Value; break;
        case 1006: Mouse1006 = Value; break;
        case 1015: Mouse1015 = Value; break;
        case 1005: Mouse1005 = Value; break;
    }

    MouseUpdate();
}

void TerminalMouse::MouseOn()
{
    MouseWorks = true;
    MouseUpdate();
}

void TerminalMouse::MouseOff()
{
    MouseWorks = false;
    MouseUpdate();
}

void TerminalMouse::Reset()
{
    Mouse0009 = false;
    Mouse1000 = false;
    Mouse1001 = false;
    Mouse1002 = false;
    Mouse1003 = false;
    Mouse1016 = false;
    Mouse1006 = false;
    Mouse1015 = false;
    Mouse1005 = false;
    MouseUpdate();
}

void TerminalMouse::MouseUpdate()
{
    bool Scr = false;
    if (Mouse0009 || Mouse1000 || Mouse1001 || Mouse1002 || Mouse1003)
    {
        if (MouseWorks)
        {
            Scr = true;
        }
    }
    if (MouseScreen != Scr)
    {
        Screen::MouseActive(Scr);
        MouseScreen = Scr;
    }
}

std::string TerminalMouse::MouseEvent(std::string Name, int X, int Y, int Btn, bool ModShift, bool ModCtrl, bool ModAlt)
{
    int KeyMods = 0;
    if (ModCtrl) { KeyMods += 100; }
    if (ModAlt) { KeyMods += 200; }
    if (ModShift) { KeyMods += 400; }
    switch (_(Name.c_str()))
    {
        case _("Move"):
            {
                if ((Mouse1002 && (Btn > 0)) || (Mouse1003))
                {
                    return MouseEventCode(X, Y, Btn + 20 + KeyMods);
                }
            }
            break;
        case _("Down"):
            {
                bool SendBtn = false;
                if (Mouse0009 && (Btn <= 3)) { SendBtn = true; KeyMods = 0; }
                if (Mouse1000 || Mouse1001 || Mouse1002 || Mouse1003) { SendBtn = true; }
                if (SendBtn)
                {
                    return MouseEventCode(X, Y, Btn + KeyMods);
                }
            }
        case _("Up"):
            {
                bool SendBtn = false;
                if (Mouse1000 || Mouse1001 || Mouse1002 || Mouse1003) { SendBtn = true; }
                if (SendBtn)
                {
                    if (Highlight)
                    {
                        int SelectX1 = (HighlightX);
                        int SelectY1 = (HighlightY);
                        int SelectX2 = (X + 1);
                        int SelectY2 = (Y + 1);

                        if (Mouse1016)
                        {
                            SelectX2 = SelectX2 * Screen::TerminalCellW;
                            SelectY2 = SelectY2 * Screen::TerminalCellH;
                            if (SelectY1 < (HighlightFirst * Screen::TerminalCellH)) { SelectY1 = (HighlightFirst) * Screen::TerminalCellH; }
                            if (SelectY1 > ((HighlightLast - 1) * Screen::TerminalCellH)) { SelectY1 = (HighlightLast - 1) * Screen::TerminalCellH; }
                            if (SelectY2 < (HighlightFirst * Screen::TerminalCellH)) { SelectY2 = (HighlightFirst) * Screen::TerminalCellH; }
                            if (SelectY2 > ((HighlightLast - 1) * Screen::TerminalCellH)) { SelectY2 = (HighlightLast - 1) * Screen::TerminalCellH; }
                        }
                        else
                        {
                            if (SelectY1 < HighlightFirst) { SelectY1 = HighlightFirst; }
                            if (SelectY1 > (HighlightLast - 1)) { SelectY1 = HighlightLast - 1; }
                            if (SelectY2 < HighlightFirst) { SelectY2 = HighlightFirst; }
                            if (SelectY2 > (HighlightLast - 1)) { SelectY2 = HighlightLast - 1; }
                        }

                        Highlight = false;
                        bool Std = true;

                        // SGR-Pixel
                        if (Mouse1016)
                        {
                            Std = false;
                            if ((SelectX1 == SelectX2) && (SelectY1 == SelectY2))
                            {
                                return ("##_[_<" + TelnetReportNumToStr(SelectX1) + "_;" + TelnetReportNumToStr(SelectY1) + "_t");
                            }
                            else
                            {
                                return ("##_[_<" + TelnetReportNumToStr(SelectX1) + "_;" + TelnetReportNumToStr(SelectY1) + "_;" + TelnetReportNumToStr(SelectX2) + "_;" + TelnetReportNumToStr(SelectY2) + "_;" + TelnetReportNumToStr((X + 1) * Screen::TerminalCellW) + "_;" + TelnetReportNumToStr((Y + 1) * Screen::TerminalCellH) + "_T");
                            }
                        }
                        else
                        {
                            // SGR
                            if (Mouse1006)
                            {
                                Std = false;
                                if ((SelectX1 == SelectX2) && (SelectY1 == SelectY2))
                                {
                                    return ("##_[_<" + TelnetReportNumToStr(SelectX1) + "_;" + TelnetReportNumToStr(SelectY1) + "_t");
                                }
                                else
                                {
                                    return ("##_[_<" + TelnetReportNumToStr(SelectX1) + "_;" + TelnetReportNumToStr(SelectY1) + "_;" + TelnetReportNumToStr(SelectX2) + "_;" + TelnetReportNumToStr(SelectY2) + "_;" + TelnetReportNumToStr(X + 1) + "_;" + TelnetReportNumToStr(Y + 1) + "_T");
                                }
                            }
                            else
                            {
                                // URXVT
                                if (Mouse1015)
                                {
                                    Std = false;
                                    if ((SelectX1 == SelectX2) && (SelectY1 == SelectY2))
                                    {
                                        return ("##_[" + TelnetReportNumToStr(SelectX1) + "_;" + TelnetReportNumToStr(SelectY1) + "_t");
                                    }
                                    else
                                    {
                                        return ("##_[" + TelnetReportNumToStr(SelectX1) + "_;" + TelnetReportNumToStr(SelectY1) + "_;" + TelnetReportNumToStr(SelectX2) + "_;" + TelnetReportNumToStr(SelectY2) + "_;" + TelnetReportNumToStr(X + 1) + "_;" + TelnetReportNumToStr(Y + 1) + "_T");
                                    }
                                }
                                else
                                {
                                    // UTF-8
                                    if (Mouse1005 && (X <= 2014) && (Y <= 2014))
                                    {
                                        Std = false;
                                        if ((SelectX1 == SelectX2) && (SelectY1 == SelectY2))
                                        {
                                            return ("##_[_t" + MouseCoordCalc(SelectX1, true) + MouseCoordCalc(SelectY1, true));
                                        }
                                        else
                                        {
                                            return ("##_[_T" + MouseCoordCalc(SelectX1, true) + MouseCoordCalc(SelectY1, true) + MouseCoordCalc(SelectX2, true) + MouseCoordCalc(SelectY2, true) + MouseCoordCalc(X + 1, true) + MouseCoordCalc(Y + 1, true));
                                        }
                                    }
                                    else
                                    {
                                        if (Std && (X <= 222) && (Y <= 222))
                                        {
                                            if ((SelectX1 == SelectX2) && (SelectY1 == SelectY2))
                                            {
                                                return ("##_[_t" + MouseCoordCalc(SelectX1, false) + MouseCoordCalc(SelectY1, false));
                                            }
                                            else
                                            {
                                                return ("##_[_T" + MouseCoordCalc(SelectX1, false) + MouseCoordCalc(SelectY1, false) + MouseCoordCalc(SelectX2, false) + MouseCoordCalc(SelectY2, false) + MouseCoordCalc(X + 1, false) + MouseCoordCalc(Y + 1, false));
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        return MouseEventCode(X, Y, Btn + 10 + KeyMods);
                    }
                }
            }
            break;
    }
    return "";
}

std::string TerminalMouse::TelnetReportNumToStr(int N)
{
    std::string N_ = std::to_string(N);
    std::string S = "";
    for (int i = 0; i < N_.size(); i++)
    {
        S = S + "_" + N_[i];
    }
    return S;
}

std::string TerminalMouse::MouseCoordCalc(int Val, bool UTF8)
{
    if (UTF8)
    {
        if (Val <= 94)
        {
            return TextWork::CharCode(Val + 33, 0);
        }
        else
        {
            return TextWork::CharCode(((Val + 33) / 64) + 192, 0) + TextWork::CharCode(((Val + 33) % 64) + 128, 0);
        }
    }
    else
    {
        return TextWork::CharCode(Val + 33, 0);
    }
}

std::string TerminalMouse::MouseEventCode(int X, int Y, int Evt)
{
    int EvtCode = 0;
    int EvtCode_ = 0;
    int EvtType = 0;
    switch (Evt % 100)
    {
        case 1: EvtCode = 0x20; EvtCode_ = 0x20; EvtType = 0; break;
        case 2: EvtCode = 0x21; EvtCode_ = 0x21; EvtType = 0; break;
        case 3: EvtCode = 0x22; EvtCode_ = 0x22; EvtType = 0; break;
        case 4: EvtCode = 0x60; EvtCode_ = 0x60; EvtType = 0; break;
        case 5: EvtCode = 0x61; EvtCode_ = 0x61; EvtType = 0; break;
        case 11: EvtCode = 0x23; EvtCode_ = 0x20; EvtType = 1; break;
        case 12: EvtCode = 0x23; EvtCode_ = 0x21; EvtType = 1; break;
        case 13: EvtCode = 0x23; EvtCode_ = 0x22; EvtType = 1; break;
        case 14: EvtCode = 0x23; EvtCode_ = 0x60; EvtType = 1; break;
        case 15: EvtCode = 0x23; EvtCode_ = 0x61; EvtType = 1; break;
        case 21: EvtCode = 0x40; EvtCode_ = 0x40; EvtType = 2; break;
        case 22: EvtCode = 0x41; EvtCode_ = 0x41; EvtType = 2; break;
        case 23: EvtCode = 0x42; EvtCode_ = 0x42; EvtType = 2; break;
        case 20: EvtCode = 0x43; EvtCode_ = 0x43; EvtType = 2; break;
    }
    switch (Evt / 100)
    {
        case 0: EvtCode += 0x00; EvtCode_ += 0x00; break;
        case 4: EvtCode += 0x04; EvtCode_ += 0x04; break;
        case 2: EvtCode += 0x08; EvtCode_ += 0x08; break;
        case 6: EvtCode += 0x0C; EvtCode_ += 0x0C; break;
        case 1: EvtCode += 0x10; EvtCode_ += 0x10; break;
        case 5: EvtCode += 0x14; EvtCode_ += 0x14; break;
        case 3: EvtCode += 0x18; EvtCode_ += 0x18; break;
        case 7: EvtCode += 0x1c; EvtCode_ += 0x1c; break;
    }
    if ((X >= 0) && (Y >= 0))
    {
        bool Std = true;

        // SGR-Pixel
        if (Mouse1016)
        {
            Std = false;
            if (EvtType == 1)
            {
                return ("##_[_<" + TelnetReportNumToStr(EvtCode_ - 32) + "_;" + TelnetReportNumToStr(X * Screen::TerminalCellW + Screen::TerminalCellW2) + "_;" + TelnetReportNumToStr(Y * Screen::TerminalCellH + Screen::TerminalCellH2) + "_m");
            }
            else
            {
                return ("##_[_<" + TelnetReportNumToStr(EvtCode_ - 32) + "_;" + TelnetReportNumToStr(X * Screen::TerminalCellW + Screen::TerminalCellW2) + "_;" + TelnetReportNumToStr(Y * Screen::TerminalCellH + Screen::TerminalCellH2) + "_M");
            }
        }
        else
        {
            // SGR
            if (Mouse1006)
            {
                Std = false;
                if (EvtType == 1)
                {
                    return ("##_[_<" + TelnetReportNumToStr(EvtCode_ - 32) + "_;" + TelnetReportNumToStr(X + 1) + "_;" + TelnetReportNumToStr(Y + 1) + "_m");
                }
                else
                {
                    return ("##_[_<" + TelnetReportNumToStr(EvtCode_ - 32) + "_;" + TelnetReportNumToStr(X + 1) + "_;" + TelnetReportNumToStr(Y + 1) + "_M");
                }
            }
            else
            {
                // URXVT
                if (Mouse1015)
                {
                    Std = false;
                    return ("##_[" + TelnetReportNumToStr(EvtCode) + "_;" + TelnetReportNumToStr(X + 1) + "_;" + TelnetReportNumToStr(Y + 1) + "_M");
                }
                else
                {
                    // UTF-8
                    if (Mouse1005 && (X <= 2014) && (Y <= 2014))
                    {
                        Std = false;
                        return ("##_[_M" + TextWork::CharCode(EvtCode, 0) + MouseCoordCalc(X, true) + MouseCoordCalc(Y, true));
                    }
                    else
                    {
                        if (Std && (X <= 222) && (Y <= 222))
                        {
                            return ("##_[_M" + TextWork::CharCode(EvtCode, 0) + MouseCoordCalc(X, false) + MouseCoordCalc(Y, false));
                        }
                    }
                }
            }
        }

    }
    return "";
}
