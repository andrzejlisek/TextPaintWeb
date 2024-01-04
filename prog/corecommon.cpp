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
    if (BinaryFile_.get()->PreInit)
    {
        TextCodec::CodecListCreate();

        int I = 1;
        int XCodec = 0;
        bool XAnsi = false;

        while (CF.get()->ParamExists("File" + std::to_string(I) + "Name"))
        {
            std::string XFile = CF.get()->ParamGetS("File" + std::to_string(I) + "Name");
            if (CF.get()->ParamExists("File" + std::to_string(I) + "Codec"))
            {
                XCodec = CF.get()->ParamGetI("File" + std::to_string(I) + "Codec");
            }
            if (CF.get()->ParamExists("File" + std::to_string(I) + "Ansi"))
            {
                XAnsi = CF.get()->ParamGetB("File" + std::to_string(I) + "Ansi");
            }
            BinaryFile_.get()->ItemAdd(BinaryFileItem(XFile, 5, 0, XCodec, XAnsi), false);
            I++;
        }
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
    BinaryFile_.get()->SaveFromString(CF.get()->FileSave(0));
    BinaryFile_.get()->SysSaveConfig();
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
