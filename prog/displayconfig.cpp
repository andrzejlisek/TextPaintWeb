#include "displayconfig.h"

DisplayConfig::DisplayConfig()
{

}

std::string DisplayConfig::IncDecDisplay(int BaseValue, int ParamValue, int Direction)
{
    if (Direction < 0)
    {
        switch (ParamValue)
        {
            case -9: return std::to_string(BaseValue) + " / 10";
            case -8: return std::to_string(BaseValue) + " / 9";
            case -7: return std::to_string(BaseValue) + " / 8";
            case -6: return std::to_string(BaseValue) + " / 7";
            case -5: return std::to_string(BaseValue) + " / 6";
            case -4: return std::to_string(BaseValue) + " / 5";
            case -3: return std::to_string(BaseValue) + " / 4";
            case -2: return std::to_string(BaseValue) + " / 3";
            case -1: return std::to_string(BaseValue) + " / 2";
            case 0: return std::to_string(BaseValue) + " - 1";
            case 1: return std::to_string(BaseValue) + " - 10";
            case 2: return std::to_string(BaseValue) + " - 100";
            case 3: return std::to_string(BaseValue) + " - 1000";
            case 4: return std::to_string(BaseValue) + " - 10000";
            case 5: return std::to_string(BaseValue) + " - 100000";
            case 6: return std::to_string(BaseValue) + " - 1000000";
        }
    }
    else
    {
        switch (ParamValue)
        {
            case -9: return std::to_string(BaseValue) + " * 10";
            case -8: return std::to_string(BaseValue) + " * 9";
            case -7: return std::to_string(BaseValue) + " * 8";
            case -6: return std::to_string(BaseValue) + " * 7";
            case -5: return std::to_string(BaseValue) + " * 6";
            case -4: return std::to_string(BaseValue) + " * 5";
            case -3: return std::to_string(BaseValue) + " * 4";
            case -2: return std::to_string(BaseValue) + " * 3";
            case -1: return std::to_string(BaseValue) + " * 2";
            case 0: return std::to_string(BaseValue) + " + 1";
            case 1: return std::to_string(BaseValue) + " + 10";
            case 2: return std::to_string(BaseValue) + " + 100";
            case 3: return std::to_string(BaseValue) + " + 1000";
            case 4: return std::to_string(BaseValue) + " + 10000";
            case 5: return std::to_string(BaseValue) + " + 100000";
            case 6: return std::to_string(BaseValue) + " + 1000000";
        }
    }
    return "???";
}

int DisplayConfig::IncDecChange(int BaseValue, int ParamValue, int Direction)
{
    int Val = BaseValue;
    if (Direction < 0)
    {
        switch (ParamValue)
        {
            case -9: Val = BaseValue / 10; break;
            case -8: Val = BaseValue / 9; break;
            case -7: Val = BaseValue / 8; break;
            case -6: Val = BaseValue / 7; break;
            case -5: Val = BaseValue / 6; break;
            case -4: Val = BaseValue / 5; break;
            case -3: Val = BaseValue / 4; break;
            case -2: Val = BaseValue / 3; break;
            case -1: Val = BaseValue / 2; break;
            case 0: Val = BaseValue - 1; break;
            case 1: Val = BaseValue - 10; break;
            case 2: Val = BaseValue - 100; break;
            case 3: Val = BaseValue - 1000; break;
            case 4: Val = BaseValue - 10000; break;
            case 5: Val = BaseValue - 100000; break;
            case 6: Val = BaseValue - 1000000; break;
        }
        if (Val < 1) Val = 1;
        return Val;
    }
    else
    {
        switch (ParamValue)
        {
            case -9: Val = BaseValue * 10; break;
            case -8: Val = BaseValue * 9; break;
            case -7: Val = BaseValue * 8; break;
            case -6: Val = BaseValue * 7; break;
            case -5: Val = BaseValue * 6; break;
            case -4: Val = BaseValue * 5; break;
            case -3: Val = BaseValue * 4; break;
            case -2: Val = BaseValue * 3; break;
            case -1: Val = BaseValue * 2; break;
            case 0: Val = BaseValue + 1; break;
            case 1: Val = BaseValue + 10; break;
            case 2: Val = BaseValue + 100; break;
            case 3: Val = BaseValue + 1000; break;
            case 4: Val = BaseValue + 10000; break;
            case 5: Val = BaseValue + 100000; break;
            case 6: Val = BaseValue + 1000000; break;
        }
        if (Val > 1000000000) Val = 1000000000;
        if (Val < 1) Val = 1000000000;
        return Val;
    }
    return Val;
}

void DisplayConfig::Repaint()
{
    int OffsetX = 0;
    int OffsetY = 0;
    InfoW = 32;
    InfoH = 21;

    XList<Str> MenuInfo;

    switch (PopupState)
    {
        case 0:
            {
                MenuInfo.Add(Str(" Screen size: " + std::to_string(Screen::CurrentW) + "x" + std::to_string(Screen::CurrentH) + (Screen::WinAuto ? " auto" : "     ")));
                MenuInfo.Add(Str(""));
                switch (CoreAnsi_.get()->ANSIDOS)
                {
                    case 0: MenuInfo.Add(Str(" K. Process mode: VTx/ISO")); break;
                    case 1: MenuInfo.Add(Str(" K. Process mode: DOS")); break;
                    case 2: MenuInfo.Add(Str(" K. Process mode: VTx/DEC")); break;
                }
                MenuInfo.Add(Str(" L. 8-bit controls: " + (CoreAnsi_.get()->ANSI8bit ? std::string("Yes") : std::string("No"))));
                MenuInfo.Add(Str(" U. Backspace: " + (CoreAnsi_.get()->ANSIPrintBackspace ? std::string("Character") : std::string("Movement"))));
                MenuInfo.Add(Str(" I. Tab: " + (CoreAnsi_.get()->ANSIPrintTab ? std::string("Character") : std::string("Movement"))));
                switch (CoreAnsi_.get()->ANSI_CR)
                {
                    case 0: MenuInfo.Add(Str(" O. CR: CR")); break;
                    case 1: MenuInfo.Add(Str(" O. CR: CR+LF")); break;
                    case 2: MenuInfo.Add(Str(" O. CR: Ignore")); break;
                }
                switch (CoreAnsi_.get()->ANSI_LF)
                {
                    case 0: MenuInfo.Add(Str(" P. LF: LF")); break;
                    case 1: MenuInfo.Add(Str(" P. LF: CR+LF")); break;
                    case 2: MenuInfo.Add(Str(" P. LF: Ignore")); break;
                }
                MenuInfo.Add(Str(""));
                MenuInfo.Add(Str(" Q. Use colors: " + (CF.get()->ParamGetI("ANSIColors") ? std::string("Yes") : std::string("No "))));
                switch (CF.get()->ParamGetI("ANSIReverseMode"))
                {
                    case 0: MenuInfo.Add(Str(" W. Reverse mode: None  ")); break;
                    case 1: MenuInfo.Add(Str(" W. Reverse mode: Before")); break;
                    case 2: MenuInfo.Add(Str(" W. Reverse mode: After ")); break;
                }
                MenuInfo.Add(Str(" E. Bold as color: " + (CF.get()->ParamGetB("ANSIColorBold") ? std::string("Yes") : std::string("No "))));
                MenuInfo.Add(Str(" R. Blink as color: " + (CF.get()->ParamGetB("ANSIColorBlink") ? std::string("Yes") : std::string("No "))));
                MenuInfo.Add(Str(" T. Ignore concealed: " + (CF.get()->ParamGetB("ANSIIgnoreConcealed") ? std::string("Yes") : std::string("No "))));
                MenuInfo.Add(Str(" "));
                switch (CF.get()->ParamGetI("DisplayBlink"))
                {
                    case 0: MenuInfo.Add(Str(" S. Blink: None")); break;
                    case 1: MenuInfo.Add(Str(" S. Blink: VTx ")); break;
                    case 2: MenuInfo.Add(Str(" S. Blink: DOS ")); break;
                }
                MenuInfo.Add(Str(" D. Bold: " + (((CF.get()->ParamGetI("DisplayAttrib") & 1) > 0) ? std::string("Yes") : std::string("No "))));
                MenuInfo.Add(Str(" F. Italic: " + (((CF.get()->ParamGetI("DisplayAttrib") & 2) > 0) ? std::string("Yes") : std::string("No "))));
                MenuInfo.Add(Str(" G. Underline: " + (((CF.get()->ParamGetI("DisplayAttrib") & 4) > 0) ? std::string("Yes") : std::string("No "))));
                MenuInfo.Add(Str(" H. Strikethrough: " + (((CF.get()->ParamGetI("DisplayAttrib") & 8) > 0) ? std::string("Yes") : std::string("No "))));
                MenuInfo.Add(Str(" J. Color blending: " + (CF.get()->ParamGetB("ColorBlending") ? std::string("Yes") : std::string("No "))));
            }
            break;
        case 1:
        case 2:
            {
                int ItemPos = 1;
                XList<Str> ItemList;
                if (PopupState == 1)
                {
                    MenuInfo.Add(Str("Palette select"));
                    MenuInfo.Add(Str(""));
                    ItemPos = Screen::PaletteListSelect;
                    for (int I = 0; I < Screen::PaletteListName.Count; I++)
                    {
                        ItemList.Add(Str(Screen::PaletteListName[I]));
                    }
                }
                if (PopupState == 2)
                {
                    MenuInfo.Add(Str("Font select"));
                    switch (Screen::FontListMode)
                    {
                        case 0:
                            MenuInfo.Add(Str(" [Monospace]  Duospace   Dbl-W "));
                            break;
                        case 1:
                            MenuInfo.Add(Str("  Monospace  [Duospace]  Dbl-W "));
                            break;
                        case 2:
                            MenuInfo.Add(Str("  Monospace   Duospace  [Dbl-W]"));
                            break;
                    }
                    ItemPos = Screen::FontListSelect;
                    for (int I = 0; I < Screen::FontListName.Count; I++)
                    {
                        ItemList.Add(Str(Screen::FontListName[I]));
                    }
                }
                int HalfPos = (InfoH / 2) - 1;
                for (int I = 0; I < (InfoH - 2); I++)
                {
                    Str VisibleItem;

                    int ItemListI = ItemPos - HalfPos + I;
                    if ((ItemListI >= 0) && (ItemListI < ItemList.Count))
                    {
                        VisibleItem.AddRange(ItemList[ItemListI]);
                    }
                    else
                    {
                        VisibleItem.AddString("");
                    }

                    if (I == HalfPos)
                    {
                        VisibleItem.InsertString("> ");
                    }
                    else
                    {
                        VisibleItem.InsertString("  ");
                    }
                    MenuInfo.Add(VisibleItem);
                }
            }
            break;
        case 3:
            {
                MenuInfo.Add(Str(""));

                switch (CF.get()->ParamGetI("DisplayCursor"))
                {
                    case 0: MenuInfo.Add(Str(" Q. Cursor: Hidden")); break;
                    case 1: MenuInfo.Add(Str(" Q. Cursor: Blinking underline")); break;
                    case 2: MenuInfo.Add(Str(" Q. Cursor: Steady underline")); break;
                    case 3: MenuInfo.Add(Str(" Q. Cursor: Blinking vert. bar")); break;
                    case 4: MenuInfo.Add(Str(" Q. Cursor: Steady vert. bar")); break;
                    case 5: MenuInfo.Add(Str(" Q. Cursor: Blinking box")); break;
                    case 6: MenuInfo.Add(Str(" Q. Cursor: Steady box")); break;
                }

                MenuInfo.Add(Str(""));
                switch (CF.get()->ParamGetI("ANSIScrollSmooth"))
                {
                    case 0: MenuInfo.Add(Str(" W. Scroll: None")); break;
                    case 1: MenuInfo.Add(Str(" W. Scroll: 1 step")); break;
                    case 2: MenuInfo.Add(Str(" W. Scroll: 2 steps")); break;
                    case 3: MenuInfo.Add(Str(" W. Scroll: 4 steps")); break;
                    case 4: MenuInfo.Add(Str(" W. Scroll: 8 steps")); break;
                    case 5: MenuInfo.Add(Str(" W. Scroll: 16 steps")); break;
                }

                MenuInfo.Add(Str(""));
                MenuInfo.Add(Str(" E. Set step decrease"));
                MenuInfo.Add(Str(" R. Set step increase"));

                MenuInfo.Add(Str(""));
                MenuInfo.Add(Str(" S. Set ANI step: " + IncDecDisplay(CF.get()->ParamGetI("TimerStep"), CF.get()->ParamGetI("TimerStepChange"),  1)));
                MenuInfo.Add(Str(" X. Set ANI step: " + IncDecDisplay(CF.get()->ParamGetI("TimerStep"), CF.get()->ParamGetI("TimerStepChange"), -1)));

                MenuInfo.Add(Str(" D. Set TXT step: " + IncDecDisplay(CF.get()->ParamGetI("TimerStepText"), CF.get()->ParamGetI("TimerStepChange"),  1)));
                MenuInfo.Add(Str(" C. Set TXT step: " + IncDecDisplay(CF.get()->ParamGetI("TimerStepText"), CF.get()->ParamGetI("TimerStepChange"), -1)));

                MenuInfo.Add(Str(" F. Set scroll: " + IncDecDisplay(CF.get()->ParamGetI("ANSIScrollChars"), CF.get()->ParamGetI("TimerStepChange"),  1)));
                MenuInfo.Add(Str(" V. Set scroll: " + IncDecDisplay(CF.get()->ParamGetI("ANSIScrollChars"), CF.get()->ParamGetI("TimerStepChange"), -1)));
            }
            break;
    }

    if ((MenuPos == 1) || (MenuPos == 3))
    {
        OffsetX = Screen::CurrentW - InfoW;
    }
    if ((MenuPos == 2) || (MenuPos == 3))
    {
        OffsetY = Screen::CurrentH - InfoH;
    }

    int CB_ = PopupBack;
    int CF_ = PopupFore;

    for (int Y = 0; Y < InfoH; Y++)
    {
        for (int X = 0; X < InfoW; X++)
        {
            Screen::ScreenChar(OffsetX + X, OffsetY + Y, ' ', CB_, CF_, 0, 0, 0);
            if (MenuInfo.Count > Y)
            {
                if (MenuInfo[Y].Count > X)
                {
                    Screen::ScreenChar(OffsetX + X, OffsetY + Y, MenuInfo[Y][X], CB_, CF_, 0, 0, 0);
                }
            }
        }
    }

    if (PopupState == 1)
    {
        for (int I = 0; I < 16; I++)
        {
            Screen::ScreenChar(OffsetX + (I * 2 + 0), OffsetY + 1, '#', I + 16, I + 16, 0, 0, 0);
            Screen::ScreenChar(OffsetX + (I * 2 + 1), OffsetY + 1, '#', I + 16, I + 16, 0, 0, 0);
        }
    }

    if ((PopupState == 2) && (Screen::FontCustom || Screen::FontSinglePage))
    {
        int HalfPos = (InfoH / 2) - 1;
        for (int YY = 0; YY < (InfoH - 2); YY++)
        {
            if (YY != HalfPos)
            {
                for (int XX = 0; XX < InfoW; XX++)
                {
                    Screen::ScreenChar(OffsetX + XX, OffsetY + YY + 2, ' ', CB_, CF_, 0, 0, 0);
                }
            }
        }
        int Ptr = 0;
        for (int YY = 0; YY < 17; YY++)
        {
            if (YY == (HalfPos - 1))
            {
                YY++;
            }
            if (Screen::FontCustom)
            {
                for (int XX = 0; XX < 16; XX++)
                {
                    Screen::ScreenChar(OffsetX + (XX * 2), OffsetY + YY + 3, Screen::FontCustomMap[Ptr], CB_, CF_, 0, 0, 0);
                    Ptr++;
                }
            }
            else
            {
                for (int XX = 0; XX < 16; XX++)
                {
                    Screen::ScreenChar(OffsetX + (XX * 2), OffsetY + YY + 3, Ptr, CB_, CF_, 0, 0, 0);
                    Ptr++;
                }
            }
        }
    }

    for (int i = -1; i < InfoW + 1; i++)
    {
        Screen::ScreenChar(OffsetX + i, OffsetY + InfoH, ' ', CF_, CB_, 0, 0, 0);
        Screen::ScreenChar(OffsetX + i, OffsetY - 1, ' ', CF_, CB_, 0, 0, 0);
    }
    for (int i = 0; i < InfoH; i++)
    {
        Screen::ScreenChar(OffsetX + InfoW, OffsetY + i, ' ', CF_, CB_, 0, 0, 0);
        Screen::ScreenChar(OffsetX - 1, OffsetY + i, ' ', CF_, CB_, 0, 0, 0);
    }

    switch (MenuPos)
    {
        case 0:
            Screen::ScreenCursorMove(InfoW, InfoH);
            break;
        case 1:
            Screen::ScreenCursorMove(OffsetX - 1, InfoH);
            break;
        case 2:
            Screen::ScreenCursorMove(InfoW, OffsetY - 1);
            break;
        case 3:
            Screen::ScreenCursorMove(OffsetX - 1, OffsetY - 1);
            break;
    }
    Screen::ScreenRefresh();
}

void DisplayConfig::EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    RequestResize = false;
    RequestRepaint = false;
    RequestClose = false;
    RequestReload = false;
    RequestSave = false;
    int Temp;

    switch (_(KeyName.c_str()))
    {
        case _("Escape"):
        case _("Enter"):
        case _("NumpadEnter"):
            RequestClose = true;
            break;
        case _("Tab"):
            MenuPos++;
            if (MenuPos == 4)
            {
                MenuPos = 0;
            }
            RequestRepaint = true;
            break;
        case _("Insert"):
        case _("Delete"):
            {
                PopupState++;
                if (PopupState == 4)
                {
                    PopupState = 0;
                }
            }
            break;
    }

    switch (PopupState)
    {
        case 0:
            switch (_(KeyName.c_str()))
            {
                case _("ArrowUp"):
                    ScreenResize(0, -1);
                    break;
                case _("ArrowDown"):
                    ScreenResize(0, +1);
                    break;
                case _("ArrowLeft"):
                    ScreenResize(-1, 0);
                    break;
                case _("ArrowRight"):
                    ScreenResize(+1, 0);
                    break;
                case _("PageUp"):
                    ScreenResize(0, -10);
                    break;
                case _("PageDown"):
                    ScreenResize(0, +10);
                    break;
                case _("Home"):
                    ScreenResize(-10, 0);
                    break;
                case _("End"):
                    ScreenResize(+10, 0);
                    break;
                case _("Space"):
                    Screen::WinAuto = !Screen::WinAuto;
                    CF.get()->ParamSet("WinAuto", Screen::WinAuto);
                    Screen::ScreenSetConfig();
                    RequestSave = true;
                    break;

                case _("KeyQ"):
                    CF.get()->ParamSet("ANSIColors", !CF.get()->ParamGetB("ANSIColors"));
                    Screen::ScreenSetConfig();
                    RequestSave = true;
                    break;
                case _("KeyW"):
                    Temp = CF.get()->ParamGetI("ANSIReverseMode");
                    Temp++;
                    if (Temp == 3) Temp = 0;
                    CF.get()->ParamSet("ANSIReverseMode", Temp);
                    Screen::ScreenSetConfig();
                    RequestSave = true;
                    break;
                case _("KeyE"):
                    CF.get()->ParamSet("ANSIColorBold", !CF.get()->ParamGetB("ANSIColorBold"));
                    Screen::ScreenSetConfig();
                    RequestSave = true;
                    break;
                case _("KeyR"):
                    CF.get()->ParamSet("ANSIColorBlink", !CF.get()->ParamGetB("ANSIColorBlink"));
                    Screen::ScreenSetConfig();
                    RequestSave = true;
                    break;
                case _("KeyT"):
                    CF.get()->ParamSet("ANSIIgnoreConcealed", !CF.get()->ParamGetB("ANSIIgnoreConcealed"));
                    Screen::ScreenSetConfig();
                    RequestSave = true;
                    break;

                case _("KeyK"):
                    CoreAnsi_.get()->ANSIDOS = ((CoreAnsi_.get()->ANSIDOS + 1) % 3);
                    CoreAnsi_.get()->ANSIDOS_ = (CoreAnsi_.get()->ANSIDOS == 1);
                    CF.get()->ParamSet("ANSIDOS", CoreAnsi_.get()->ANSIDOS);
                    Screen::ScreenSetConfig();
                    RequestReload = true;
                    RequestSave = true;
                    break;
                case _("KeyL"):
                    CoreAnsi_.get()->ANSI8bit = !CoreAnsi_.get()->ANSI8bit;
                    CF.get()->ParamSet("ANSI8bit", CoreAnsi_.get()->ANSI8bit ? 1 : 0);
                    Screen::ScreenSetConfig();
                    RequestReload = true;
                    RequestSave = true;
                    break;
                case _("KeyU"):
                    CoreAnsi_.get()->ANSIPrintBackspace = !CoreAnsi_.get()->ANSIPrintBackspace;
                    CF.get()->ParamSet("ANSIPrintBackspace", CoreAnsi_.get()->ANSIPrintBackspace ? 1 : 0);
                    Screen::ScreenSetConfig();
                    RequestReload = true;
                    RequestSave = true;
                    break;
                case _("KeyI"):
                    CoreAnsi_.get()->ANSIPrintTab = !CoreAnsi_.get()->ANSIPrintTab;
                    CF.get()->ParamSet("ANSIPrintTab", CoreAnsi_.get()->ANSIPrintTab ? 1 : 0);
                    Screen::ScreenSetConfig();
                    RequestReload = true;
                    RequestSave = true;
                    break;
                case _("KeyO"):
                    CoreAnsi_.get()->ANSI_CR = ((CoreAnsi_.get()->ANSI_CR + 1) % 3);
                    CF.get()->ParamSet("ANSIReadCR", CoreAnsi_.get()->ANSI_CR);
                    Screen::ScreenSetConfig();
                    RequestReload = true;
                    RequestSave = true;
                    break;
                case _("KeyP"):
                    CoreAnsi_.get()->ANSI_LF = ((CoreAnsi_.get()->ANSI_LF + 1) % 3);
                    CF.get()->ParamSet("ANSIReadLF", CoreAnsi_.get()->ANSI_LF);
                    Screen::ScreenSetConfig();
                    RequestReload = true;
                    RequestSave = true;
                    break;

                case _("KeyS"):
                    Temp = CF.get()->ParamGetI("DisplayBlink");
                    Temp++;
                    if (Temp == 3) Temp = 0;
                    CF.get()->ParamSet("DisplayBlink", Temp);
                    Screen::ScreenSetConfig();
                    RequestSave = true;
                    break;
                case _("KeyD"):
                    Temp = CF.get()->ParamGetI("DisplayAttrib");
                    if (CoreStatic::GetAttribBit(Temp, 0))
                    {
                        Temp = CoreStatic::SetAttribBit(Temp, 0, 0);
                    }
                    else
                    {
                        Temp = CoreStatic::SetAttribBit(Temp, 0, 1);
                    }
                    CF.get()->ParamSet("DisplayAttrib", Temp);
                    Screen::ScreenSetConfig();
                    RequestSave = true;
                    break;
                case _("KeyF"):
                    Temp = CF.get()->ParamGetI("DisplayAttrib");
                    if (CoreStatic::GetAttribBit(Temp, 1))
                    {
                        Temp = CoreStatic::SetAttribBit(Temp, 1, 0);
                    }
                    else
                    {
                        Temp = CoreStatic::SetAttribBit(Temp, 1, 1);
                    }
                    CF.get()->ParamSet("DisplayAttrib", Temp);
                    Screen::ScreenSetConfig();
                    RequestSave = true;
                    break;
                case _("KeyG"):
                    Temp = CF.get()->ParamGetI("DisplayAttrib");
                    if (CoreStatic::GetAttribBit(Temp, 2))
                    {
                        Temp = CoreStatic::SetAttribBit(Temp, 2, 0);
                    }
                    else
                    {
                        Temp = CoreStatic::SetAttribBit(Temp, 2, 1);
                    }
                    CF.get()->ParamSet("DisplayAttrib", Temp);
                    Screen::ScreenSetConfig();
                    RequestSave = true;
                    break;
                case _("KeyH"):
                    Temp = CF.get()->ParamGetI("DisplayAttrib");
                    if (CoreStatic::GetAttribBit(Temp, 3))
                    {
                        Temp = CoreStatic::SetAttribBit(Temp, 3, 0);
                    }
                    else
                    {
                        Temp = CoreStatic::SetAttribBit(Temp, 3, 1);
                    }
                    CF.get()->ParamSet("DisplayAttrib", Temp);
                    Screen::ScreenSetConfig();
                    RequestSave = true;
                    break;
                case _("KeyJ"):
                    CF.get()->ParamSet("ColorBlending", !CF.get()->ParamGetB("ColorBlending"));
                    Screen::ScreenSetConfig();
                    RequestSave = true;
                    break;
            }
            break;
        case 1:
            switch (_(KeyName.c_str()))
            {
                case _("ArrowUp"):
                    if ((Screen::PaletteListSelect > 0) || Screen::PaletteCustom)
                    {
                        if (!Screen::PaletteCustom)
                        {
                            Screen::PaletteListSelect--;
                        }
                        CF.get()->ParamSet("PaletteSelect", Screen::PaletteListSelect + 1);
                        Screen::SetPalette();
                        RequestSave = true;
                    }
                    break;
                case _("ArrowDown"):
                    if ((Screen::PaletteListSelect < (Screen::PaletteListName.Count - 1)) || Screen::PaletteCustom)
                    {
                        if (!Screen::PaletteCustom)
                        {
                            Screen::PaletteListSelect++;
                        }
                        CF.get()->ParamSet("PaletteSelect", Screen::PaletteListSelect + 1);
                        Screen::SetPalette();
                        RequestSave = true;
                    }
                    break;
            }
            break;
        case 2:
            switch (_(KeyName.c_str()))
            {
                case _("ArrowUp"):
                    if ((Screen::FontListSelect > 0) || Screen::FontCustom)
                    {
                        if (!Screen::FontCustom)
                        {
                            Screen::FontListSelect--;
                        }
                        CF.get()->ParamSet("FontSelect", Screen::FontListSelect + 1);
                        Screen::SetFont();
                        RequestSave = true;
                    }
                    break;
                case _("ArrowDown"):
                    if ((Screen::FontListSelect < (Screen::FontListName.Count - 1)) || Screen::FontCustom)
                    {
                        if (!Screen::FontCustom)
                        {
                            Screen::FontListSelect++;
                        }
                        CF.get()->ParamSet("FontSelect", Screen::FontListSelect + 1);
                        Screen::SetFont();
                        RequestSave = true;
                    }
                    break;
                case _("ArrowLeft"):
                    if (Screen::FontListMode > 0)
                    {
                        Screen::FontListMode--;
                        CF.get()->ParamSet("FontMode", Screen::FontListMode);
                        Screen::SetFont();
                        RequestSave = true;
                    }
                    break;
                case _("ArrowRight"):
                    if (Screen::FontListMode < 2)
                    {
                        Screen::FontListMode++;
                        CF.get()->ParamSet("FontMode", Screen::FontListMode);
                        Screen::SetFont();
                        RequestSave = true;
                    }
                    break;
            }
            break;
        case 3:
            switch (_(KeyName.c_str()))
            {
                case _("KeyQ"):
                    {
                        CF.get()->ParamSet("DisplayCursor", (CF.get()->ParamGetI("DisplayCursor") + 1) % 7);
                        Screen::ScreenSetConfig();
                        RequestSave = true;
                    }
                    break;
                case _("KeyW"):
                    {
                        CF.get()->ParamSet("ANSIScrollSmooth", (CF.get()->ParamGetI("ANSIScrollSmooth") + 1) % 6);
                        Screen::ScreenSetConfig();
                        RequestSave = true;
                    }
                    break;
                case _("KeyE"):
                    {
                        int Temp = CF.get()->ParamGetI("TimerStepChange");
                        if (Temp > (-9)) Temp--;
                        CF.get()->ParamSet("TimerStepChange", Temp);
                        Screen::ScreenSetConfig();
                        RequestSave = true;
                    }
                    break;
                case _("KeyR"):
                    {
                        int Temp = CF.get()->ParamGetI("TimerStepChange");
                        if (Temp < 6) Temp++;
                        CF.get()->ParamSet("TimerStepChange", Temp);
                        Screen::ScreenSetConfig();
                        RequestSave = true;
                    }
                    break;

                case _("KeyS"):
                    {
                        CF.get()->ParamSet("TimerStep", IncDecChange(CF.get()->ParamGetI("TimerStep"), CF.get()->ParamGetI("TimerStepChange"),  1));
                        Screen::ScreenSetConfig();
                        RequestSave = true;
                    }
                    break;
                case _("KeyX"):
                    {
                        CF.get()->ParamSet("TimerStep", IncDecChange(CF.get()->ParamGetI("TimerStep"), CF.get()->ParamGetI("TimerStepChange"), -1));
                        Screen::ScreenSetConfig();
                        RequestSave = true;
                    }
                    break;
                case _("KeyD"):
                    {
                        CF.get()->ParamSet("TimerStepText", IncDecChange(CF.get()->ParamGetI("TimerStepText"), CF.get()->ParamGetI("TimerStepChange"),  1));
                        Screen::ScreenSetConfig();
                        RequestSave = true;
                    }
                    break;
                case _("KeyC"):
                    {
                        CF.get()->ParamSet("TimerStepText", IncDecChange(CF.get()->ParamGetI("TimerStepText"), CF.get()->ParamGetI("TimerStepChange"), -1));
                        Screen::ScreenSetConfig();
                        RequestSave = true;
                    }
                    break;
                case _("KeyF"):
                    {
                        CF.get()->ParamSet("ANSIScrollChars", IncDecChange(CF.get()->ParamGetI("ANSIScrollChars"), CF.get()->ParamGetI("TimerStepChange"),  1));
                        Screen::ScreenSetConfig();
                        RequestSave = true;
                        RequestReload = true;
                    }
                    break;
                case _("KeyV"):
                    {
                        CF.get()->ParamSet("ANSIScrollChars", IncDecChange(CF.get()->ParamGetI("ANSIScrollChars"), CF.get()->ParamGetI("TimerStepChange"), -1));
                        Screen::ScreenSetConfig();
                        RequestSave = true;
                        RequestReload = true;
                    }
                    break;
            }
            break;
    }
}

void DisplayConfig::Open()
{
    Repaint();
}

void DisplayConfig::ScreenResize(int DeltaX, int DeltaY)
{
    ResizeW = Screen::CurrentW + DeltaX;
    ResizeH = Screen::CurrentH + DeltaY;
    if (ResizeW < 1) ResizeW = 1;
    if (ResizeH < 1) ResizeH = 1;
    RequestResize = true;
}
