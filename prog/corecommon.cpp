#include "corecommon.h"

CoreCommon::CoreCommon()
{

}

void CoreCommon::EventTick()
{

}

void CoreCommon::EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{

}

void CoreCommon::EventOther(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4)
{

}

void CoreCommon::Init()
{

}

void CoreCommon::UpdateConfig(std::shared_ptr<ConfigFile> CF)
{
    ProcessTimerStep = CF.get()->ParamGetI("TimerStep");
    if (ProcessTimerStep <= 0)
    {
        ProcessTimerStep = 1;
    }
    ProcessTimerStepText = CF.get()->ParamGetI("TimerStepText");
    if (ProcessTimerStepText <= 0)
    {
        ProcessTimerStepText = 1;
    }
    Screen::WinAuto = CF.get()->ParamGetB("WinAuto");
}

void CoreCommon::ReadColor(std::string SettingValue, int &ColorB, int &ColorF)
{
    if (SettingValue.length() == 2)
    {
        int V = Hex::HexToInt(SettingValue);
        ColorB = V / 16;
        ColorF = V % 16;
    }
}

void CoreCommon::InitCommon()
{
    Screen::CursorHideTerm(false, 0);
    if (BinaryFile_.get()->PreInit)
    {
        TextCodec::CodecListCreate(CF.get()->ParamGetS("ANSICharsDOS"));
        Screen::FontSingleChar.clear();
        for (int I = 0; I < 32; I++)
        {
            Screen::FontSingleChar[TextCodec::CharDOS[I]] = I;
        }
        Screen::FontSingleChar[TextCodec::CharDOS[32]] = 127;
        BinaryFile_.get()->SetDir(Str("/"));
    }
    BinaryFile_.get()->PreInit = false;

    DisplayConfig_ = std::make_shared<DisplayConfig>();
    DisplayConfig_.get()->CF = CF;

    ReadColor(CF.get()->ParamGetS("ColorNormal"), Screen::TextNormalBack, Screen::TextNormalFore);
    ReadColor(CF.get()->ParamGetS("ColorStatus"), StatusBack, StatusFore);
    ReadColor(CF.get()->ParamGetS("ColorPopup"), PopupBack, PopupFore);
    StatusBack += 16;
    StatusFore += 16;
    PopupBack += 16;
    PopupFore += 16;
    //ScreenStatusBar = CF.get()->ParamGetI("DisplayStatusBar");
    ScreenStatusBar = 0;

    FileManager_.BinaryFile_ = BinaryFile_;
    FileManager_.PopupBack = PopupBack;
    FileManager_.PopupFore = PopupFore;

    std::string Space[256];
    int L = TextWork::StringSplit(CF.get()->ParamGetS("Space"), ',', Space);
    Str::SpaceChars.Add(32);
    for (int I = 0; I < L; I++)
    {
        int Item = Hex::HexToInt(Space[I]);
        if (Item > 32)
        {
            Str::SpaceChars.Add(Item);
        }
    }
    Str::SpaceChar0 = Str::SpaceChars[0];
}

void CoreCommon::ScreenChar0(int X, int Y, int Chr, int Back, int Fore)
{
    ScreenChar0(X, Y, Chr, Back, Fore, 0, 0, 0);
}

void CoreCommon::ScreenChar0(int X, int Y, int Chr, int Back, int Fore, int Attr, int FontW, int FontH)
{
    if (X < 0) return;
    if (Y < 0) return;
    if (X >= ScreenW) return;
    if (Y >= ScreenW) return;
    Screen::ScreenChar(X, Y, Chr, Back, Fore, Attr, FontW, FontH);
}

void CoreCommon::Screen_WriteText(std::string Text)
{
    ScreenWelcomeBuf[ScreenWelcomeBuf.Count - 1].AddString(Text);
    //Screen_.WriteText(Text, Core_.TextNormalBack, Core_.TextNormalFore);
}

void CoreCommon::Screen_WriteLine()
{
    ScreenWelcomeBuf.Add(Str());
}

void CoreCommon::Screen_BackText()
{
    if (ScreenWelcomeBuf[ScreenWelcomeBuf.Count - 1].Count > 0)
    {
        ScreenWelcomeBuf[ScreenWelcomeBuf.Count - 1].RemoveRange(ScreenWelcomeBuf[ScreenWelcomeBuf.Count - 1].Count - 1, 1);
    }
}

void CoreCommon::Screen_Clear()
{
    ScreenWelcomeBuf.Clear();
    ScreenWelcomeBuf.Add(Str());
}

void CoreCommon::Screen_Refresh()
{
    Screen::ScreenClear(Screen::TextNormalBack, Screen::TextNormalFore);
    Screen::ScreenCursorMove(0, 0);
    for (int i = 0; i < ScreenWelcomeBuf.Count; i++)
    {
        if (i > 0) Screen::ScreenWriteLine(Screen::TextNormalBack, Screen::TextNormalFore);
        Screen::ScreenWriteText(ScreenWelcomeBuf[i], Screen::TextNormalBack, Screen::TextNormalFore);
    }
    Screen::ScreenRefresh();
}

void CoreCommon::SaveConfig()
{
    BinaryFile_.get()->SaveFromStringConfig(CF.get()->FileSave(0));
    BinaryFile_.get()->SysSaveConfig();
    UpdateConfig(CF);
    CoreAnsi_.get()->UpdateConfig(CF);
}

void CoreCommon::ScreenStatusBarSet(int ScreenStatusBar_)
{
    int TermW = Screen::CurrentW;
    int TermH = Screen::CurrentH;
    if ((ScreenStatusBar == 0) && (ScreenStatusBar_ > 0))
    {
        TermH++;
        Screen::ScreenResize(TermW, TermH);
    }
    if ((ScreenStatusBar > 0) && (ScreenStatusBar_ == 0))
    {
        TermH--;
        Screen::ScreenResize(TermW, TermH);
    }
    CoreAnsi_.get()->AnsiTerminalResize(TermW, TermH, ScreenStatusBar_);
    ScreenStatusBar = ScreenStatusBar_;
}

int CoreCommon::OptionTextKeyNumber(std::string KeyName)
{
    switch (_(KeyName.c_str()))
    {
        case _("Digit0"): return 0;
        case _("Digit1"): return 1;
        case _("Digit2"): return 2;
        case _("Digit3"): return 3;
        case _("Digit4"): return 4;
        case _("Digit5"): return 5;
        case _("Digit6"): return 6;
        case _("Digit7"): return 7;
        case _("Digit8"): return 8;
        case _("Digit9"): return 9;
        case _("Numpad0"): return 0;
        case _("Numpad1"): return 1;
        case _("Numpad2"): return 2;
        case _("Numpad3"): return 3;
        case _("Numpad4"): return 4;
        case _("Numpad5"): return 5;
        case _("Numpad6"): return 6;
        case _("Numpad7"): return 7;
        case _("Numpad8"): return 8;
        case _("Numpad9"): return 9;
        case _("KeyA"): return 10;
        case _("KeyB"): return 11;
        case _("KeyC"): return 12;
        case _("KeyD"): return 13;
        case _("KeyE"): return 14;
        case _("KeyF"): return 15;
        case _("KeyG"): return 16;
        case _("KeyH"): return 17;
        case _("KeyI"): return 18;
        case _("KeyJ"): return 19;
        case _("KeyK"): return 20;
        case _("KeyL"): return 21;
        case _("KeyM"): return 22;
        case _("KeyN"): return 23;
        case _("KeyO"): return 24;
        case _("KeyP"): return 25;
        case _("KeyQ"): return 26;
        case _("KeyR"): return 27;
        case _("KeyS"): return 28;
        case _("KeyT"): return 29;
        case _("KeyU"): return 30;
        case _("KeyV"): return 31;
        case _("KeyW"): return 32;
        case _("KeyX"): return 33;
        case _("KeyY"): return 34;
        case _("KeyZ"): return 35;
    }
    return -1;
}

void CoreCommon::OptionTextInfoAddLine(std::string Info)
{
    OptionTextInfo = OptionTextInfo + Info + "\n";
}

bool CoreCommon::OptionTextKeyWrite(std::string KeyName, int KeyChar)
{
    switch (_(KeyName.c_str()))
    {
        case _("NumpadEnter"):
        case _("Enter"):
            if (OptionTextData.size() > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        case _("Backspace"):
            if (OptionTextData.size() > 0)
            {
                if (OptionTextData.size() > 1)
                {
                    OptionTextData = OptionTextData.substr(0, OptionTextData.size() - 1);
                }
                else
                {
                    OptionTextData = "";
                }
            }
            break;
        case _("Escape"):
            OptionTextData = "";
            break;
        default:
            if (KeyChar >= 32)
            {
                OptionTextData = OptionTextData + TextWork::CharToStr(KeyChar);
            }
    }
    return false;
}

void CoreCommon::OptionTextDisplayRefresh()
{
    XList<std::string> OptTxt;
    TextWork::StringSplit(OptionTextInfo + "\n" + OptionTextData, '\n', OptTxt);
    Screen::ScreenClear(Screen::TextNormalBack, Screen::TextNormalFore);
    Screen::ScreenCursorMove(0, 0);
    for (int I = 0; I < OptTxt.Count; I++)
    {
        if (I > 0)
        {
            Screen::ScreenWriteLine(Screen::TextNormalBack, Screen::TextNormalFore);
        }
        Screen::ScreenWriteText(Str::FromString(OptTxt[I]), Screen::TextNormalBack, Screen::TextNormalFore);
    }
    Screen::ScreenRefresh();
}
