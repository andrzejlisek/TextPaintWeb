#include "core2terminal.h"

Core2Terminal::Core2Terminal()
{

}

void Core2Terminal::Init()
{
    B64 = std::make_unique<TextCodec>(TextCodec::BASE64);

    CoreAnsi_ = std::make_shared<CoreAnsi>(CF);
    DisplayConfig_.get()->CoreAnsi_ = CoreAnsi_;
    DisplayConfig_.get()->PopupBack = PopupBack;
    DisplayConfig_.get()->PopupFore = PopupFore;
    CoreAnsi_.get()->__AnsiProcessDelayFactor = CF.get()->ParamGetI("FileDelayFrame");
    TerminalKeyboard_.CoreAnsi_ = CoreAnsi_;

    ServerPort = CF.get()->ParamGetI("ServerPort");
    ServerTelnet = CF.get()->ParamGetB("ServerTelnet");
    ServerCodec = std::make_shared<TextCodec>(CF.get()->ParamGetI("ServerEncoding"));

    LocalEcho = false;
    Command_8bit = false;
    UseCtrlKeys = false;


    WindowTitle = "";
    WindowIcon = "";
    DisplayStatusPrepare();

    TerminalConnection = CF.get()->ParamGetS("TerminalConnection");

    TerminalName = CF.get()->ParamGetS("TerminalName");
    TerminalType = CF.get()->ParamGetI("TerminalType");
    if (TerminalType < 0)
    {
        TerminalType = 1;
    }
    std::string TerminalAnswerBack0 = CF.get()->ParamGetS("TerminalAnswerBack");
    TerminalKeyboard_.TerminalAnswerBack = "";
    for (int i = 0; i < TerminalAnswerBack0.size(); i++)
    {
        TerminalKeyboard_.TerminalAnswerBack = TerminalKeyboard_.TerminalAnswerBack + "_" + TextWork::CharToStr(TerminalAnswerBack0[i]);
    }

    TerminalCodecS = std::make_shared<TextCodec>(0);
    TerminalCodecR = std::make_shared<TextCodec>(0);

    std::string TelnetKeyboardConf_ = std::to_string(CF.get()->ParamGetI("TerminalKeys"));
    TerminalKeyboard_.TelnetKeyboardConf = "";
    while (TelnetKeyboardConf_.size() < TerminalKeyboard_.TelnetKeyboardConfMax.size())
    {
        TelnetKeyboardConf_ = TelnetKeyboardConf_ + "0";
    }
    for (int i = 0; i < TerminalKeyboard_.TelnetKeyboardConfMax.size(); i++)
    {
        Str Allowed;
        Allowed.Clear();
        Allowed.Add('0');
        if (TerminalKeyboard_.TelnetKeyboardConfMax[i] == '1') { Allowed.AddString("1"); }
        if (TerminalKeyboard_.TelnetKeyboardConfMax[i] == '2') { Allowed.AddString("12"); }
        if (TerminalKeyboard_.TelnetKeyboardConfMax[i] == '3') { Allowed.AddString("123"); }
        if (TerminalKeyboard_.TelnetKeyboardConfMax[i] == '4') { Allowed.AddString("1234"); }
        if (Allowed.Contains(TelnetKeyboardConf_[i]))
        {
            TerminalKeyboard_.TelnetKeyboardConf = TerminalKeyboard_.TelnetKeyboardConf + TextWork::CharToStr(TelnetKeyboardConf_[i]);
        }
        else
        {
            TerminalKeyboard_.TelnetKeyboardConf = TerminalKeyboard_.TelnetKeyboardConf + "0";
        }
    }

    TerminalStep = CF.get()->ParamGetI("TerminalStep");
    if (TerminalStep <= 0)
    {
        TerminalStep = 2000000000;
    }

    Conn = std::make_unique<TerminalConn>();

    ConnListName.Clear();
    ConnListAddr.Clear();
    ConnListType.Clear();
    ConnListCodec.Clear();

    ConnListName.Add("VTTEST");
    ConnListType.Add("VTTEST");
    ConnListAddr.Add("VTTEST");
    ConnListCodec.Add(0);

    int ConnN = 1;
    while (CF.get()->ParamExists("Terminal" + std::to_string(ConnN) + "Name"))
    {
        std::string _ConnName = CF.get()->ParamGetS("Terminal" + std::to_string(ConnN) + "Name");
        std::string _ConnType = CF.get()->ParamGetS("Terminal" + std::to_string(ConnN) + "Type");
        std::string _ConnAddr = CF.get()->ParamGetS("Terminal" + std::to_string(ConnN) + "Addr");
        int _ConnCodec = CF.get()->ParamGetI("Terminal" + std::to_string(ConnN) + "Codec");
        if ((_ConnType.size() > 0) && (_ConnAddr.size() > 0) && (_ConnName.size() > 0))
        {
            ConnListName.Add(_ConnName);
            ConnListType.Add(_ConnType);
            ConnListAddr.Add(_ConnAddr);
            ConnListCodec.Add(_ConnCodec);
        }
        ConnN++;
    }

    //ConnListName.Add("Other");
    //ConnListType.Add("CUSTOM");
    //ConnListAddr.Add("~");
    //ConnListCodec.Add(0);

    ConnListPrint();
    WorkStateC = WorkStateCDef::ConnListSelect;
}

void Core2Terminal::ConnListPrint()
{
    Screen_Clear();
    int L = std::min(ConnListName.Count, (int)(ConnListKeys.size()));
    for (int I = 0; I < L; I++)
    {
        Screen_WriteText(ConnListKeys.substr(I, 1));
        Screen_WriteText(" - ");
        Screen_WriteText(ConnListName[I]);
        Screen_WriteLine();
    }
    Screen_Refresh();
}

void Core2Terminal::EventTick()
{
    if (ConnResizeCounter > 0)
    {
        if (Conn.get()->IsConnected() == 1)
        {
            ConnResizeCounter--;
            if (ConnResizeCounter == 0)
            {
                Conn.get()->Resize(ScreenW, ScreenH);
            }
        }
    }

    if (ConnInternal)
    {
        while (Conn.get()->Msgs.Count > 0)
        {
            ProcessReceived(Conn.get()->Msgs[0]);
            Conn.get()->Msgs.Remove(0);
        }
    }

    int Processed = CoreAnsi_.get()->AnsiProcess(TerminalStep);
    if (Processed > 0)
    {
        TelnetDisplayInfoRepaint = true;
        Screen::CursorHide(CoreAnsi_.get()->AnsiState_.CursorHide);
        CoreAnsi_.get()->AnsiRepaintCursor();
        bool NeedStatusRepaint = false;
        if (ScreenW != Screen::CurrentW)
        {
            NeedStatusRepaint = true;
            ScreenW = Screen::CurrentW;
        }
        if (ScreenH != Screen::CurrentH)
        {
            NeedStatusRepaint = true;
            ScreenH = Screen::CurrentH;
        }
        if (NeedStatusRepaint)
        {
            DisplayStatusRepaint();
        }
    }
    for (int i = 0; i < CoreAnsi_.get()->__AnsiResponse.Count; i++)
    {
        TelnetReport(CoreAnsi_.get()->__AnsiResponse[i]);
    }
    CoreAnsi_.get()->__AnsiResponse.Clear();
    switch (WorkStateC)
    {
        case WorkStateCDef::Toolbox:
        case WorkStateCDef::EscapeKey:
            {
                TelnetDisplayInfo(false);
            }
            break;
        case WorkStateCDef::DispConf:
            DisplayConfig_.get()->Repaint();
            break;
        default:
            break;
    }
}

void Core2Terminal::EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    switch (WorkStateC)
    {
        case WorkStateCDef::ConnListSelect:
            {
                int ConnSelectedItem = -1;
                switch (_(KeyName.c_str()))
                {
                    case _("Digit0"): ConnSelectedItem = 0; break;
                    case _("Digit1"): ConnSelectedItem = 1; break;
                    case _("Digit2"): ConnSelectedItem = 2; break;
                    case _("Digit3"): ConnSelectedItem = 3; break;
                    case _("Digit4"): ConnSelectedItem = 4; break;
                    case _("Digit5"): ConnSelectedItem = 5; break;
                    case _("Digit6"): ConnSelectedItem = 6; break;
                    case _("Digit7"): ConnSelectedItem = 7; break;
                    case _("Digit8"): ConnSelectedItem = 8; break;
                    case _("Digit9"): ConnSelectedItem = 9; break;
                    case _("Numpad0"): ConnSelectedItem = 0; break;
                    case _("Numpad1"): ConnSelectedItem = 1; break;
                    case _("Numpad2"): ConnSelectedItem = 2; break;
                    case _("Numpad3"): ConnSelectedItem = 3; break;
                    case _("Numpad4"): ConnSelectedItem = 4; break;
                    case _("Numpad5"): ConnSelectedItem = 5; break;
                    case _("Numpad6"): ConnSelectedItem = 6; break;
                    case _("Numpad7"): ConnSelectedItem = 7; break;
                    case _("Numpad8"): ConnSelectedItem = 8; break;
                    case _("Numpad9"): ConnSelectedItem = 9; break;
                    case _("KeyA"): ConnSelectedItem = 10; break;
                    case _("KeyB"): ConnSelectedItem = 11; break;
                    case _("KeyC"): ConnSelectedItem = 12; break;
                    case _("KeyD"): ConnSelectedItem = 13; break;
                    case _("KeyE"): ConnSelectedItem = 14; break;
                    case _("KeyF"): ConnSelectedItem = 15; break;
                    case _("KeyG"): ConnSelectedItem = 16; break;
                    case _("KeyH"): ConnSelectedItem = 17; break;
                    case _("KeyI"): ConnSelectedItem = 18; break;
                    case _("KeyJ"): ConnSelectedItem = 19; break;
                    case _("KeyK"): ConnSelectedItem = 20; break;
                    case _("KeyL"): ConnSelectedItem = 21; break;
                    case _("KeyM"): ConnSelectedItem = 22; break;
                    case _("KeyN"): ConnSelectedItem = 23; break;
                    case _("KeyO"): ConnSelectedItem = 24; break;
                    case _("KeyP"): ConnSelectedItem = 25; break;
                    case _("KeyQ"): ConnSelectedItem = 26; break;
                    case _("KeyR"): ConnSelectedItem = 27; break;
                    case _("KeyS"): ConnSelectedItem = 28; break;
                    case _("KeyT"): ConnSelectedItem = 29; break;
                    case _("KeyU"): ConnSelectedItem = 30; break;
                    case _("KeyV"): ConnSelectedItem = 31; break;
                    case _("KeyW"): ConnSelectedItem = 32; break;
                    case _("KeyX"): ConnSelectedItem = 33; break;
                    case _("KeyY"): ConnSelectedItem = 34; break;
                    case _("KeyZ"): ConnSelectedItem = 35; break;
                }
                if (ConnSelectedItem >= 0)
                {
                    ConnSelectedAddr = ConnListAddr[ConnSelectedItem];
                    ConnSelectedType = ConnListType[ConnSelectedItem];
                    ConnSelectedCodec = ConnListCodec[ConnSelectedItem];
                    WorkStateC = WorkStateCDef::InfoScreen;
                    Screen_Clear();
                    Screen_WriteText("Press key, which will be used as escape key...");
                    Screen_Refresh();
                }
            }
            break;
        case WorkStateCDef::InfoScreen:
            EscapeKey_ = EscapeKeyId(KeyName, KeyChar);
            WorkStateC = WorkStateCDef::Session;
            ConnOpen();
            TerminalMouse_.MouseOn();
            break;
        case WorkStateCDef::Session:
            if (EscapeKey_ == EscapeKeyId(KeyName, KeyChar))
            {
                TerminalMouse_.MouseOff();
                WorkStateC = WorkStateCDef::Toolbox;
            }
            else
            {
                SendHex(TerminalKeyboard_.KeyCode(KeyName, KeyChar, ModShift, ModCtrl, ModAlt));
                if (TerminalKeyboard_.EchoChar.Count)
                {
                    if (LocalEcho)
                    {
                        CoreAnsi_.get()->AnsiProcessSupply(TerminalKeyboard_.EchoChar);
                    }
                    TerminalKeyboard_.EchoChar.Clear();
                }
            }
            break;
        case WorkStateCDef::Toolbox:
            {
                std::string CAS = TerminalKeyboard_.KeyCAS(ModShift, ModCtrl, ModAlt);
                switch (_(KeyName.c_str()))
                {
                    case _("Backspace"):
                        ScreenStatusBarSet(0);
                        ConnClose(true);
                        return;

                    case _("Tab"):
                        TelnetInfoPos++;
                        if (TelnetInfoPos == 4)
                        {
                            TelnetInfoPos = 0;
                        }
                        break;
                    case _("Enter"):
                    case _("NumpadEnter"):
                        {
                            WorkStateC = WorkStateCDef::EscapeKey;
                        }
                        break;
                    case _("Escape"):
                        {
                            WorkStateC = WorkStateCDef::Session;
                            TerminalMouse_.MouseOn();
                        }
                        break;
                    case _("KeyA"):
                    case _("KeyB"):
                    case _("KeyC"):
                    case _("KeyD"):
                    case _("KeyE"):
                    case _("KeyF"):
                    case _("KeyG"):
                    case _("KeyH"):
                    case _("KeyI"):
                    case _("KeyJ"):
                    case _("KeyK"):
                    case _("KeyL"):
                    case _("KeyM"):
                    case _("KeyN"):
                    case _("KeyO"):
                    case _("KeyP"):
                    case _("KeyQ"):
                    case _("KeyR"):
                    case _("KeyS"):
                    case _("KeyT"):
                    case _("KeyU"):
                    case _("KeyV"):
                    case _("KeyW"):
                    case _("KeyX"):
                    case _("KeyY"):
                    case _("KeyZ"):
                        SendHex(TerminalKeyboard_.KeyCode(KeyName, KeyChar, false, true, false));
                        WorkStateC = WorkStateCDef::Session;
                        TerminalMouse_.MouseOn();
                        break;
                    case _("Digit1"):
                    case _("Digit2"):
                    case _("Digit3"):
                    case _("Digit4"):
                    case _("Digit5"):
                    case _("Digit6"):
                    case _("Digit7"):
                    case _("Digit8"):
                    case _("Digit9"):
                        SendHex(TerminalKeyboard_.KeyCode("F" + KeyName.substr(5), 0, ModShift, ModCtrl, ModAlt));
                        WorkStateC = WorkStateCDef::Session;
                        TerminalMouse_.MouseOn();
                        break;
                    case _("Digit0"):
                        SendHex(TerminalKeyboard_.KeyCode("F10", 0, ModShift, ModCtrl, ModAlt));
                        WorkStateC = WorkStateCDef::Session;
                        TerminalMouse_.MouseOn();
                        break;

                    default:
                        {
                            switch (KeyChar)
                            {
                                case '=':
                                    {
                                        WorkStateC = WorkStateCDef::Session;
                                        TerminalMouse_.MouseOn();
                                    }
                                    break;
                                case ';':
                                case ':':
                                    TerminalKeyboard_.TelnetFuncKeyOther++;
                                    if (TerminalKeyboard_.TelnetFuncKeyOther == 3)
                                    {
                                        UseCtrlKeys = !UseCtrlKeys;
                                        TerminalKeyboard_.TelnetFuncKeyOther = 0;
                                    }
                                    break;
                                case ',':
                                    {
                                        WorkStateC = WorkStateCDef::Session;
                                        Clipboard_.SetText(CoreAnsi_.get()->AnsiState_.GetScreen(0, 0, ScreenW - 1, ScreenH - 1));
                                        Screen::FileExport(0, "", "", Clipboard_.SystemText);
                                        TerminalMouse_.MouseOn();
                                    }
                                    break;
                                case '.':
                                    {
                                        WorkStateC = WorkStateCDef::Session;
                                        Screen::FileImport(0, "", "");
                                        TerminalMouse_.MouseOn();
                                    }
                                    break;
                                default:
                                    if (UseCtrlKeys)
                                    {
                                        switch (KeyChar)
                                        {
                                            case ' ':
                                            case '@':
                                                WorkStateC = WorkStateCDef::Session;
                                                SendHex("00");
                                                TerminalMouse_.MouseOn();
                                                break;
                                            case '[':
                                            case '{':
                                                WorkStateC = WorkStateCDef::Session;
                                                SendHex("1B");
                                                TerminalMouse_.MouseOn();
                                                break;
                                            case '\\':
                                            case '|':
                                                WorkStateC = WorkStateCDef::Session;
                                                SendHex("1C");
                                                TerminalMouse_.MouseOn();
                                                break;
                                            case ']':
                                            case '}':
                                                WorkStateC = WorkStateCDef::Session;
                                                SendHex("1D");
                                                TerminalMouse_.MouseOn();
                                                break;
                                            case '~':
                                            case '^':
                                                WorkStateC = WorkStateCDef::Session;
                                                SendHex("1E");
                                                TerminalMouse_.MouseOn();
                                                break;
                                            case '/':
                                            case '_':
                                                WorkStateC = WorkStateCDef::Session;
                                                SendHex("1F");
                                                TerminalMouse_.MouseOn();
                                                break;
                                            case '?':
                                                WorkStateC = WorkStateCDef::Session;
                                                SendHex("7F");
                                                TerminalMouse_.MouseOn();
                                                break;
                                            case '<':
                                                WorkStateC = WorkStateCDef::Session;
                                                SendHex(TerminalKeyboard_.KeyCode("F11", 0, false, ModCtrl, ModAlt));
                                                TerminalMouse_.MouseOn();
                                                break;
                                            case '>':
                                                WorkStateC = WorkStateCDef::Session;
                                                SendHex(TerminalKeyboard_.KeyCode("F12", 0, false, ModCtrl, ModAlt));
                                                TerminalMouse_.MouseOn();
                                                break;
                                        }
                                    }
                                    else
                                    {
                                        switch (KeyChar)
                                        {
                                            case '`':
                                                {
                                                    int DisplayStatus_ = DisplayStatus;
                                                    DisplayStatus++;
                                                    if (DisplayStatus == 3)
                                                    {
                                                        DisplayStatus = 0;
                                                    }
                                                    if ((DisplayStatus_ == 0) && (DisplayStatus > 0)) ScreenH++;
                                                    if ((DisplayStatus_ > 0) && (DisplayStatus == 0)) ScreenH--;
                                                    ScreenStatusBarSet(DisplayStatus);
                                                    DisplayStatusRepaint();
                                                }
                                                break;
                                            case '~':
                                                LocalEcho = !LocalEcho;
                                                break;
                                            case '|':
                                                Command_8bit = !Command_8bit;
                                                break;
                                            case '{':
                                                TerminalKeyboard_.TelnetKeyboardConfMove();
                                                break;
                                            case '}':
                                                TerminalKeyboard_.TelnetKeyboardConfStep();
                                                break;
                                            case '[':
                                                TerminalKeyboard_.TelnetKeyboardModsMove();
                                                break;
                                            case ']':
                                                TerminalKeyboard_.TelnetKeyboardModsStep();
                                                break;
                                            case '<':
                                                WorkStateC = WorkStateCDef::Session;
                                                SendHex(TerminalKeyboard_.KeyCode("F11", 0, false, ModCtrl, ModAlt));
                                                TerminalMouse_.MouseOn();
                                                break;
                                            case '>':
                                                WorkStateC = WorkStateCDef::Session;
                                                SendHex(TerminalKeyboard_.KeyCode("F12", 0, false, ModCtrl, ModAlt));
                                                TerminalMouse_.MouseOn();
                                                break;
                                            case '/':
                                                if (Conn.get()->IsConnected() == 0)
                                                {
                                                    ConnOpen();
                                                }
                                                else
                                                {
                                                    if (Conn.get()->IsConnected() == 1)
                                                    {
                                                        ConnClose(false);
                                                    }
                                                }
                                                break;
                                            case '?':
                                                {
                                                    WorkStateC = WorkStateCDef::Session;
                                                    if (Conn.get()->IsConnected() == 1)
                                                    {
                                                        Conn.get()->Resize(ScreenW, ScreenH);
                                                    }
                                                    TerminalMouse_.MouseOn();
                                                }
                                                break;
                                            case '\\':
                                                {
                                                    WorkStateC = WorkStateCDef::DispConf;
                                                    DisplayConfig_.get()->Open();
                                                }
                                                break;
                                        }
                                    }
                                    break;
                            }
                        }
                        break;
                }
                CoreAnsi_.get()->AnsiRepaint(false);
                DisplayStatusRepaint();
                CoreAnsi_.get()->AnsiRepaintCursor();
            }
            break;
        case WorkStateCDef::EscapeKey:
            EscapeKey_ = EscapeKeyId(KeyName, KeyChar);
            WorkStateC = WorkStateCDef::Toolbox;
            CoreAnsi_.get()->AnsiRepaint(false);
            DisplayStatusRepaint();
            CoreAnsi_.get()->AnsiRepaintCursor();
            break;
        case WorkStateCDef::Toolbox_:
            break;
        case WorkStateCDef::DispConf:
            DisplayConfig_.get()->EventKey(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            DisplayConfig_.get()->Repaint();
            if (DisplayConfig_.get()->RequestSave)
            {
                SaveConfig();
            }
            if (DisplayConfig_.get()->RequestRepaint)
            {
                CoreAnsi_.get()->AnsiRepaint(false);
                DisplayStatusRepaint();
                DisplayConfig_.get()->Repaint();
            }
            if (DisplayConfig_.get()->RequestClose)
            {
                WorkStateC = WorkStateCDef::Toolbox;
                CoreAnsi_.get()->AnsiRepaint(false);
                DisplayStatusRepaint();
            }
            else
            {
                if (DisplayConfig_.get()->RequestResize)
                {
                    EventOther("Resize", "", DisplayConfig_.get()->ResizeW, DisplayConfig_.get()->ResizeH, 1, 0);
                }
                CoreAnsi_.get()->AnsiRepaint(false);
                DisplayStatusRepaint();
            }
            break;
    }
}

void Core2Terminal::EventOther(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4)
{
    switch (_(EvtName.c_str()))
    {
        case _("Resize"):
            ScreenW = EvtParam1;
            ScreenH = EvtParam2;
            Screen::ScreenResize(ScreenW, ScreenH);
            CoreAnsi_.get()->AnsiTerminalResize(ScreenW, ScreenH, ScreenStatusBar);
            DisplayStatusRepaint();
            switch (WorkStateC)
            {
                case WorkStateCDef::ConnListSelect:
                case WorkStateCDef::InfoScreen:
                    Screen_Refresh();
                    break;
                case WorkStateCDef::Toolbox:
                case WorkStateCDef::EscapeKey:
                    TelnetDisplayInfo(true);
                    break;
                case WorkStateCDef::DispConf:
                    DisplayConfig_.get()->Repaint();
                    break;
                default:
                    break;
            }
            break;
        case _("FileImport"):
            {
                if ((EvtParam2 == 0) && (EvtParam4 == 0))
                {
                    Clipboard_.SystemText = EvtParam0;
                    Str Text = Clipboard_.GetText();
                    Raw Data;
                    TerminalCodecS.get()->Encode(Text, Data);
                    Conn.get()->Send(Data);
                }
            }
        case _("Received"):
            {
                Str Text(EvtParam0);
                Raw Data;
                B64.get()->Encode(Text, Data);
                ProcessReceived(Data);
                if (EvtParam1 == 1)
                {
                    Conn = std::make_unique<TerminalConn>();
                }
            }
            break;
        case _("Mouse"):
            {
                EventMouse(EvtParam0, EvtParam1, EvtParam2, EvtParam3);
            }
            return;
    }
}

void Core2Terminal::EventMouse(std::string Name, int X, int Y, int Btn)
{
    if ((WorkStateC == WorkStateCDef::Session) || (WorkStateC == WorkStateCDef::Toolbox))
    {
        if ((X >= 0) && (Y >= 0))
        {
            if ((WorkStateC == WorkStateCDef::Session) && TerminalMouse_.MouseScreen)
            {
                SendHex(TerminalMouse_.MouseEvent(Name, X, Y, Btn, false, false, false));
            }
        }
    }
}

std::string Core2Terminal::EscapeKeyId(std::string KeyName, int KeyChar)
{
    return KeyName + "_" + TextWork::CharCode(KeyChar, 0);
}

void Core2Terminal::SendHex(std::string STR)
{
    if (STR.size() == 0)
    {
        return;
    }

    Str STR0;
    Raw STR_;
    if (STR[0] != '~')
    {
        if (Command_8bit && (!CoreAnsi_.get()->AnsiState_.__AnsiVT52))
        {
            for (int i = 0x40; i < 0x60; i++)
            {
                STR = TextWork::StringReplace(STR, "##_" + TextWork::CharToStr(i), Hex::IntToHex8(i + 0x40));
                STR = TextWork::StringReplace(STR, "##" + Hex::IntToHex8(i), Hex::IntToHex8(i + 0x40));
            }
        }
        else
        {
            STR = TextWork::StringReplace(STR, "##", "1B");
        }
        STR0.AddString(STR);
        TextCodec::Transcode(STR0, TextCodec::HEX, 0);
    }
    else
    {
        int StrChr = Hex::HexToInt(STR.substr(2));
        STR0.Add(StrChr);
    }
    TerminalCodecS.get()->Encode(STR0, STR_);
    Conn.get()->Send(STR_);
}

void Core2Terminal::ConnOpen()
{
    TerminalMouse_.Reset();
    Screen::CursorHide(false);
    Screen::ScreenClear(Screen::TextNormalBack, Screen::TextNormalFore);
    DisplayStatusRepaint();
    Screen::ScreenCursorMove(0, 0);
    Screen::ScreenRefresh();

    TerminalCodecS.get()->Reset();
    TerminalCodecR.get()->Reset();
    ServerCodec.get()->Reset();

    std::string ConnSelectedType_ = "";
    std::string ConnResizeCounter_ = "";
    for (int I = 0; I < ConnSelectedType.size(); I++)
    {
        if ((ConnSelectedType[I] >= '0') && (ConnSelectedType[I] <= '9'))
        {
            ConnResizeCounter_ = ConnResizeCounter_ + TextWork::CharToStr(ConnSelectedType[I]);
        }
        else
        {
            ConnSelectedType_ = ConnSelectedType_ + TextWork::CharToStr(ConnSelectedType[I]);
        }
    }
    if (ConnResizeCounter_.size() > 0)
    {
        ConnResizeCounter = std::stoi(ConnResizeCounter_);
    }
    else
    {
        ConnResizeCounter = 0;
    }

    switch (_(ConnSelectedType_.c_str()))
    {
        case _("TELNET"):
            Conn = std::make_unique<TerminalConnWorkerTelnet>();
            break;
        default:
            Conn = std::make_unique<TerminalConnWorker>();
            break;
    }
    ConnInternal = true;

    CoreAnsi_.get()->AnsiProcessReset(false, true, 0, 1);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(1, 4);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(2, 2);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(3, 2);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(4, 2);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(5, 4);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(7, 4);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(10, 4);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(11, 4);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(12, 1);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(13, 4);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(14, 4);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(15, 4);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(16, 4);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(17, 4);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(18, 4);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(19, 4);
    CoreAnsi_.get()->AnsiState_.AnsiParamSet(20, 2);
    CoreAnsi_.get()->AnsiState_.DecParamSet(9, 2);
    CoreAnsi_.get()->AnsiState_.DecParamSet(1000, 2);
    CoreAnsi_.get()->AnsiState_.DecParamSet(1001, 2);
    CoreAnsi_.get()->AnsiState_.DecParamSet(1002, 2);
    CoreAnsi_.get()->AnsiState_.DecParamSet(1003, 2);
    CoreAnsi_.get()->AnsiState_.DecParamSet(1004, 2);
    CoreAnsi_.get()->AnsiState_.DecParamSet(1005, 2);
    CoreAnsi_.get()->AnsiState_.DecParamSet(1006, 2);
    CoreAnsi_.get()->AnsiState_.DecParamSet(1015, 2);
    CoreAnsi_.get()->AnsiState_.DecParamSet(1016, 2);

    TerminalCodecS = std::make_shared<TextCodec>(ConnSelectedCodec);
    TerminalCodecR = std::make_shared<TextCodec>(ConnSelectedCodec);

    Conn.get()->Open(ConnSelectedType_ + ":" + TerminalName, ConnSelectedAddr, ScreenW, ScreenH);
    Conn.get()->AfterOpen();
}

void Core2Terminal::ConnClose(bool StopApp)
{
    Conn.get()->Close();
    if (StopApp)
    {
        Conn = std::make_unique<TerminalConn>();
        AppExit = true;
    }
}

void Core2Terminal::ProcessReceived(Raw &Data)
{
    Conn.get()->Recv(Data);
    if (Data.Count > 0)
    {
        Str Data_;
        TerminalCodecR.get()->Decode(Data, Data_);
        if (Data_.Count > 0)
        {
            CoreAnsi_.get()->AnsiProcessSupply(Data_);
        }
    }
}

std::string Core2Terminal::TelnetReportStrToStr(std::string N)
{
    Str N_ = Str::FromString(N);
    std::string S = "";
    for (int i = 0; i < N_.Count; i++)
    {
        std::string Chr = TextWork::CharCode(N_[i], 0);
        if (TextWork::StringEndsWith(Chr, "??"))
        {
            Chr = "_ ";
        }
        S = S + Chr;
    }
    return S;
}

std::string Core2Terminal::TelnetReportNumToStr(int N)
{
    std::string N_ = std::to_string(N);
    std::string S = "";
    for (int i = 0; i < N_.size(); i++)
    {
        S = S + "_" + N_[i];
    }
    return S;
}

void Core2Terminal::TelnetReport(std::string ReportRequest)
{
    bool SendAnswer = (TerminalType < 10);
    switch (_(ReportRequest.c_str()))
    {
        default:
            {
                if (TextWork::StringStartsWith(ReportRequest, "Mouse;"))
                {
                    SendHex(TerminalMouse_.Command(ReportRequest));
                }
                if (TextWork::StringStartsWith(ReportRequest, "WindowTitle"))
                {
                    WindowTitle = ReportRequest.substr(11);
                    DisplayStatusPrepare();
                }
                if (TextWork::StringStartsWith(ReportRequest, "WindowIcon"))
                {
                    WindowIcon = ReportRequest.substr(10);
                    DisplayStatusPrepare();
                }
                if (TextWork::StringStartsEndsWith(ReportRequest, "[", "*y"))
                {
                    XList<std::string> ParamStr;
                    TextWork::StringSplit(ReportRequest.substr(1, ReportRequest.size() - 3), ';', ParamStr);
                    int N = 0;
                    std::string Resp = "##_P" + TelnetReportStrToStr(ParamStr[0]) + "_!_~";
                    Resp = Resp + TelnetReportStrToStr(TextWork::CharCode(N, 1));
                    Resp = Resp + "##_\\";
                    if (SendAnswer)
                    {
                        SendHex(Resp);
                    }
                }
                if (TextWork::StringStartsWith(ReportRequest, "$q"))
                {
                    if (SendAnswer)
                    {
                        SendHex("##_P_0_$_r##_\\");
                    }
                }
                if (TextWork::StringStartsEndsWith(ReportRequest, "[?", "$p"))
                {
                    int N = TextWork::StrToInt(ReportRequest.substr(2, ReportRequest.size() - 4), 0);
                    int V = CoreAnsi_.get()->AnsiState_.DecParamGet(N);
                    switch (N)
                    {
                        case 1:
                            V = (TerminalKeyboard_.GetTelnetKeyboardConf(0) == "1") ? 1 : 2;
                            break;
                        case 3:
                            {
                                V = 0;
                                if (ScreenW == 80) V = 2;
                                if (ScreenW == 132) V = 1;
                            }
                            break;
                        case 4:
                            V = CoreAnsi_.get()->AnsiState_.__AnsiSmoothScroll ? 1 : 2;
                            break;
                        case 5:
                            {
                                V = 2;
                                if (CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy__.CountLines() > 0)
                                {
                                    if (CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy__.CountItems(0) > 0)
                                    {
                                        CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy__.Get(0, 0);
                                        if (CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy__.Item_ColorA >= 128)
                                        {
                                            V = 1;
                                        }
                                    }
                                }
                            }
                            break;
                        case 6:
                            V = CoreAnsi_.get()->AnsiState_.__AnsiOrigin ? 1 : 2;
                            break;
                        case 7:
                            V = CoreAnsi_.get()->AnsiState_.__AnsiNoWrap ? 2 : 1;
                            break;
                        case 47:
                        case 1047:
                        case 1049:
                            V = CoreAnsi_.get()->AnsiState_.IsScreenAlternate ? 1 : 2;
                            break;
                        case 66:
                            V = (TerminalKeyboard_.GetTelnetKeyboardConf(6) == "1") ? 1 : 2;
                            break;
                        case 67:
                            V = (TerminalKeyboard_.GetTelnetKeyboardConf(5) == "1") ? 1 : 2;
                            break;
                        case 69:
                            V = CoreAnsi_.get()->AnsiState_.__AnsiMarginLeftRight ? 1 : 2;
                            break;
                        case 95:
                            V = CoreAnsi_.get()->AnsiState_.DECCOLMPreserve ? 1 : 2;
                            break;
                    }
                    if (SendAnswer)
                    {
                        SendHex("##_[_?" + TelnetReportNumToStr(N) + "_;_" + std::to_string(V) + "_$_y");
                    }
                }
                else
                {
                    if (TextWork::StringStartsEndsWith(ReportRequest, "[", "$p"))
                    {
                        int N = TextWork::StrToInt(ReportRequest.substr(1, ReportRequest.size() - 3), 0);
                        int V = CoreAnsi_.get()->AnsiState_.AnsiParamGet(N);
                        switch (N)
                        {
                            case 12:
                                V = LocalEcho ? 2 : 1;
                                break;
                            case 20:
                                V = (TerminalKeyboard_.GetTelnetKeyboardConf(4) == "1") ? 1 : 2;
                                break;
                        }
                        if (SendAnswer)
                        {
                            SendHex("##_[" + TelnetReportNumToStr(N) + "_;_" + std::to_string(V) + "_$_y");
                        }
                    }
                }
            }
            break;
        case _("[11t"):
            if (SendAnswer)
            {
                SendHex("##_[_1_t");
            }
            break;
        case _("[13t"):
        case _("[13;2t"):
            if (SendAnswer)
            {
                SendHex("##_[_3_;_1_;_1_t");
            }
            break;
        case _("[14t"):
        case _("[14;2t"):
            if (SendAnswer)
            {
                SendHex("##_[_4_;" + TelnetReportNumToStr(ScreenH * Screen::TerminalCellH) + "_;" + TelnetReportNumToStr(ScreenW * Screen::TerminalCellW) + "_t");
            }
            break;
        case _("[15t"):
            if (SendAnswer)
            {
                SendHex("##_[_5_;" + TelnetReportNumToStr(ScreenH * Screen::TerminalCellH) + "_;" + TelnetReportNumToStr(ScreenW * Screen::TerminalCellW) + "_t");
            }
            break;
        case _("[16t"):
            if (SendAnswer)
            {
                SendHex("##_[_6_;" + TelnetReportNumToStr(Screen::TerminalCellH) + "_;" + TelnetReportNumToStr(Screen::TerminalCellW) + "_t");
            }
            break;
        case _("[18t"):
            if (SendAnswer)
            {
                SendHex("##_[_8_;" + TelnetReportNumToStr(ScreenH) + "_;" + TelnetReportNumToStr(ScreenW) + "_t");
            }
            break;
        case _("[19t"):
            if (SendAnswer)
            {
                SendHex("##_[_9_;" + TelnetReportNumToStr(ScreenH) + "_;" + TelnetReportNumToStr(ScreenW) + "_t");
            }
            break;
        case _("[20t"):
            if (SendAnswer)
            {
                SendHex("##_[_L" + TelnetReportStrToStr(WindowIcon) + "##_\\");
            }
            break;
        case _("[21t"):
            if (SendAnswer)
            {
                SendHex("##_[_l" + TelnetReportStrToStr(WindowTitle) + "##_\\");
            }
            break;
        case _("[22;0t"):
            {
                WindowTitleSt.Add(WindowTitle);
                WindowIconSt.Add(WindowIcon);
            }
            break;
        case _("[22;1t"):
            {
                WindowIconSt.Add(WindowIcon);
            }
            break;
        case _("[22;2t"):
            {
                WindowTitleSt.Add(WindowTitle);
            }
            break;
        case _("[23;0t"):
            {
                if (WindowTitleSt.Count > 0)
                {
                    WindowTitle = WindowTitleSt.PopLast();
                }
                else
                {
                    WindowTitle = "";
                }
                if (WindowIconSt.Count > 0)
                {
                    WindowIcon = WindowIconSt.PopLast();
                }
                else
                {
                    WindowIcon = "";
                }
                DisplayStatusPrepare();
            }
            break;
        case _("[23;1t"):
            {
                if (WindowIconSt.Count > 0)
                {
                    WindowIcon = WindowIconSt.PopLast();
                }
                else
                {
                    WindowIcon = "";
                }
                DisplayStatusPrepare();
            }
            break;
        case _("[23;2t"):
            {
                if (WindowTitleSt.Count > 0)
                {
                    WindowTitle = WindowTitleSt.PopLast();
                }
                else
                {
                    WindowTitle = "";
                }
                DisplayStatusPrepare();
            }
            break;
        case _("[?6n"): // DECXCPR
            if (SendAnswer)
            {
                SendHex("##_[_?" + TelnetReportNumToStr(CoreAnsi_.get()->AnsiState_.ReportCursorY()) + "_;" + TelnetReportNumToStr(CoreAnsi_.get()->AnsiState_.ReportCursorX()) + "_;_1_R");
            }
            break;
        case _("[?15n"): // Printer
            if (SendAnswer)
            {
                SendHex("##_[_?_1_3_n");
            }
            break;
        case _("[?25n"): // UDK
            if (SendAnswer)
            {
                SendHex("##_[_?_2_0_n");
            }
            break;
        case _("[?26n"): // Keyboard
            if (SendAnswer)
            {
                SendHex("##_[_?_2_7_;_1_;_0_;_0_n");
            }
            break;
        case _("[?53n"): // Locator
            if (SendAnswer)
            {
                SendHex("##_[_?_5_3_n");
            }
            break;
        case _("[?62n"): // DECMSR
            if (SendAnswer)
            {
                SendHex("##_[_0_0_0_0_*_{");
            }
            break;
        case _("[?63;1n"): // DECCKSR
            if (SendAnswer)
            {
                SendHex("##_P_1_!_~_0_0_0_0##_\\");
            }
            break;
        case _("[?75n"): // Data integrity
            if (SendAnswer)
            {
                SendHex("##_[_?_7_0_n");
            }
            break;
        case _("[?85n"): // Multi-session
            if (SendAnswer)
            {
                SendHex("##_[_?_8_3_n");
            }
            break;
        case _("$q\"p"): // DCS / DECSCL
            {
                std::string Bit = Command_8bit ? "_0" : "_1";
                switch (TerminalType)
                {
                    case 0:
                        SendHex("##_P_1_$_r_6_1_;" + Bit + "_\"_p##_\\");
                        break;
                    case 1:
                        SendHex("##_P_1_$_r_6_1_;" + Bit + "_\"_p##_\\");
                        break;
                    case 2:
                        SendHex("##_P_1_$_r_6_2_;" + Bit + "_\"_p##_\\");
                        break;
                    case 3:
                        SendHex("##_P_1_$_r_6_3_;" + Bit + "_\"_p##_\\");
                        break;
                    case 4:
                        SendHex("##_P_1_$_r_6_4_;" + Bit + "_\"_p##_\\");
                        break;
                    case 5:
                        SendHex("##_P_1_$_r_6_5_;" + Bit + "_\"_p##_\\");
                        break;
                }
            }
            break;
        case _("$q*|"): // DCS / DECSNLS
            if (SendAnswer)
            {
                SendHex("##_P_1_$_r" + TelnetReportNumToStr(CoreAnsi_.get()->AnsiMaxY) + "_*_|##_\\");
            }
            break;
        case _("$qr"): // DCS / DECSTBM
            if (SendAnswer)
            {
                SendHex("##_P_1_$_r" + TelnetReportNumToStr(CoreAnsi_.get()->AnsiState_.__AnsiScrollFirst + 1) + "_;" + TelnetReportNumToStr(CoreAnsi_.get()->AnsiState_.__AnsiScrollLast + 1) + "_r##_\\");
            }
            break;
        case _("$qs"): // DCS / DECSLRM
            if (SendAnswer)
            {
                SendHex("##_P_1_$_r" + TelnetReportNumToStr(CoreAnsi_.get()->AnsiState_.__AnsiMarginLeft + 1) + "_;" + TelnetReportNumToStr(CoreAnsi_.get()->AnsiState_.__AnsiMarginRight) + "_s##_\\");
            }
            break;
        case _("[0c"): // Primary DA
            {
                switch (TerminalType)
                {
                    case 0:
                        SendHex("##_[_?_1_;_2_c");
                        break;
                    case 1:
                        SendHex("##_[_?_6_c");
                        break;
                    case 2:
                        SendHex("##_[_?_6_2_;_1_;_2_;_6_;_7_;_8_;_9_c");
                        break;
                    case 3:
                        SendHex("##_[_?_6_3_;_1_;_2_;_6_;_7_;_8_;_9_c");
                        break;
                    case 4:
                        SendHex("##_[_?_6_4_;_1_;_2_;_6_;_7_;_8_;_9_;_1_5_;_1_8_;_2_1_c");
                        break;
                    case 5:
                        SendHex("##_[_?_6_5_;_1_;_2_;_6_;_7_;_8_;_9_;_1_5_;_1_8_;_2_1_c");
                        break;
                }
            }
            break;
        case _("[>c"): // Secondary DA
        case _("[>0c"): // Secondary DA
            {
                switch (TerminalType)
                {
                    case 0:
                        SendHex("##_[_>_0_;_1_0_;_0_c");
                        break;
                    case 1:
                        SendHex("##_[_>_0_;_1_0_;_0_c");
                        break;
                    case 2:
                        SendHex("##_[_>_1_;_1_0_;_0_c");
                        break;
                    case 3:
                        SendHex("##_[_>_2_4_;_1_0_;_0_c");
                        break;
                    case 4:
                        SendHex("##_[_>_4_1_;_1_0_;_0_c");
                        break;
                    case 5:
                        SendHex("##_[_>_6_4_;_1_0_;_0_c");
                        break;
                }
            }
            break;
        case _("[=c"): // Tertiary DA
        case _("[=0c"): // Tertiary DA
            switch (TerminalType)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    SendHex("##_P_!_|_0_0_0_0_0_0_0_0##_\\");
                    break;
            }
            break;
        case _("[6n"): // DSR / CPR
            if (SendAnswer)
            {
                SendHex("##_[" + TelnetReportNumToStr(CoreAnsi_.get()->AnsiState_.ReportCursorY()) + "_;" + TelnetReportNumToStr(CoreAnsi_.get()->AnsiState_.ReportCursorX()) + "_R");
            }
            break;
        case _("VT52:Z"):
            if (SendAnswer)
            {
                SendHex("1B_/_Z");
            }
            break;
        case _("[5n"): // DSR
            if (SendAnswer)
            {
                SendHex("##_[_0_n");
            }
            break;
        case _("[0x"): // DECREQTPARM
            if (SendAnswer)
            {
                SendHex("##_[_2_;_1_;_1_;_1_1_2_;_1_1_2_;_1_;_0_x");
            }
            break;
        case _("[1x"): // DECREQTPARM
            if (SendAnswer)
            {
                SendHex("##_[_3_;_1_;_1_;_1_1_2_;_1_1_2_;_1_;_0_x");
            }
            break;

        case _("AnswerBack"):
            if (SendAnswer)
            {
                SendHex(TerminalKeyboard_.TerminalAnswerBack);
            }
            break;

        case _("Control8bit_1"):
            Command_8bit = true;
            break;
        case _("Control8bit_0"):
            Command_8bit = false;
            break;
        case _("LocalEcho_1"):
            LocalEcho = true;
            break;
        case _("LocalEcho_0"):
            LocalEcho = false;
            break;
        case _("CursorKey_1"):
            TerminalKeyboard_.SetTelnetKeyboardConf(0, 1);
            break;
        case _("CursorKey_0"):
            TerminalKeyboard_.SetTelnetKeyboardConf(0, 0);
            break;
        case _("EnterKey_1"):
            if (TerminalKeyboard_.GetTelnetKeyboardConf(4) == "0") TerminalKeyboard_.SetTelnetKeyboardConf(4, 1);
            if (TerminalKeyboard_.GetTelnetKeyboardConf(4) == "2") TerminalKeyboard_.SetTelnetKeyboardConf(4, 3);
            break;
        case _("EnterKey_0"):
            if (TerminalKeyboard_.GetTelnetKeyboardConf(4) == "1") TerminalKeyboard_.SetTelnetKeyboardConf(4, 0);
            if (TerminalKeyboard_.GetTelnetKeyboardConf(4) == "3") TerminalKeyboard_.SetTelnetKeyboardConf(4, 2);
            break;
        case _("NumpadKey_1"):
            TerminalKeyboard_.SetTelnetKeyboardConf(6, 1);
            break;
        case _("NumpadKey_0"):
            TerminalKeyboard_.SetTelnetKeyboardConf(6, 0);
            break;
        case _("BackspaceKey_1"):
            TerminalKeyboard_.SetTelnetKeyboardConf(5, 1);
            break;
        case _("BackspaceKey_0"):
            TerminalKeyboard_.SetTelnetKeyboardConf(5, 0);
            break;
    }
}

void Core2Terminal::TelnetDisplayInfo(bool NeedRepaint)
{
    if (TelnetDisplayInfoRepaint)
    {
        TelnetDisplayInfoRepaint = false;
        NeedRepaint = false;
    }
    if (NeedRepaint)
    {
        CoreAnsi_.get()->AnsiRepaint(false);
    }
    int CB_ = PopupBack;
    int CF_ = PopupFore;

    DisplayStatusRepaint();
    XList<std::string> InfoMsg;
    int IsConn = Conn.get()->IsConnected();
    if (WorkStateC == WorkStateCDef::Toolbox)
    {
        std::string StatusInfo = "Unknown";
        switch (IsConn)
        {
            case 0: StatusInfo = "Disconnected"; break;
            case 1: StatusInfo = "Connected"; break;
            case 2: StatusInfo = "Connecting"; break;
            case 3: StatusInfo = "Disconnecting"; break;
        }
        InfoMsg.Add(" Status: " + StatusInfo + " ");
        InfoMsg.Add(" Screen size: " + std::to_string(CoreAnsi_.get()->AnsiMaxX) + "x" + std::to_string(CoreAnsi_.get()->AnsiMaxY) + " ");
        InfoMsg.Add(" Escape key: " + EscapeKey_ + " ");
        InfoMsg.Add(" Esc = - Return to terminal ");
        InfoMsg.Add(" Enter - Change escape key ");
        InfoMsg.Add(" Tab - Move info ");
        InfoMsg.Add(" Backspace - Quit ");
        switch (TerminalKeyboard_.TelnetFuncKeyOther)
        {
            case 0:
                InfoMsg.Add(" 1-0 - Send F1-F10                  ");
                InfoMsg.Add(" < > - Send F11, F12                ");
                break;
            case 1:
                InfoMsg.Add(" 1-0 - Send F11-F20                 ");
                InfoMsg.Add(" < > - Send F11, F12                ");
                break;
            case 2:
                InfoMsg.Add(" 1-7 - Send NUL,ESC,FS,GS,RS,US,DEL ");
                InfoMsg.Add(" < > - Send AnswerBack              ");
                break;
        }
        InfoMsg.Add(" ; : - Change control code ");
        InfoMsg.Add(" Letter - Send CTRL+letter ");
        std::string TelnetKeyboardConf_ = "";
        switch (_((std::to_string(TerminalKeyboard_.TelnetKeyboardConfI) + TextWork::CharToStr(TerminalKeyboard_.TelnetKeyboardConf[TerminalKeyboard_.TelnetKeyboardConfI])).c_str()))
        {
            default: TelnetKeyboardConf_ = (std::to_string(TerminalKeyboard_.TelnetKeyboardConfI) + std::to_string(TerminalKeyboard_.TelnetKeyboardConf[TerminalKeyboard_.TelnetKeyboardConfI])); break;
            case _("00"): TelnetKeyboardConf_ = "Cursor - Normal"; break;
            case _("01"): TelnetKeyboardConf_ = "Cursor - Appli"; break;
            case _("10"): TelnetKeyboardConf_ = "F1-F4 - Normal"; break;
            case _("11"): TelnetKeyboardConf_ = "F1-F4 - Alter"; break;
            case _("20"): TelnetKeyboardConf_ = "F5-F12 - Normal"; break;
            case _("21"): TelnetKeyboardConf_ = "F5-F12 - Alter"; break;
            case _("30"): TelnetKeyboardConf_ = "Editing - DEC"; break;
            case _("31"): TelnetKeyboardConf_ = "Editing - IBM 1"; break;
            case _("32"): TelnetKeyboardConf_ = "Editing - IBM 2"; break;
            case _("40"): TelnetKeyboardConf_ = "Enter - CR"; break;
            case _("41"): TelnetKeyboardConf_ = "Enter - CR+lf"; break;
            case _("42"): TelnetKeyboardConf_ = "Enter - LF"; break;
            case _("43"): TelnetKeyboardConf_ = "Enter - cr+LF"; break;
            case _("50"): TelnetKeyboardConf_ = "Backspace - DEL"; break;
            case _("51"): TelnetKeyboardConf_ = "Backspace - BS"; break;
            case _("60"): TelnetKeyboardConf_ = "Numpad - Normal"; break;
            case _("61"): TelnetKeyboardConf_ = "Numpad - Appli"; break;
        }


        std::string TelnetKeyboardMods_ = "";
        switch (_((std::to_string(TerminalKeyboard_.TelnetKeyboardModsI) + TextWork::CharToStr(TerminalKeyboard_.TelnetKeyboardMods[TerminalKeyboard_.TelnetKeyboardModsI])).c_str()))
        {
            default: TelnetKeyboardMods_ = (std::to_string(TerminalKeyboard_.TelnetKeyboardModsI) + std::to_string(TerminalKeyboard_.TelnetKeyboardMods[TerminalKeyboard_.TelnetKeyboardModsI])); break;
            case _("00"): TelnetKeyboardMods_ = "Ctrl - None"; break;
            case _("01"): TelnetKeyboardMods_ = "Ctrl - Force"; break;
            case _("10"): TelnetKeyboardMods_ = "Alt - None"; break;
            case _("11"): TelnetKeyboardMods_ = "Alt - Force"; break;
            case _("20"): TelnetKeyboardMods_ = "Shift - None"; break;
            case _("21"): TelnetKeyboardMods_ = "Shift - Force"; break;
        }

        if (UseCtrlKeys)
        {
            InfoMsg.Add(" @ - Send NUL");
            InfoMsg.Add(" [ { - Send ESC");
            InfoMsg.Add(" \\ | - Send FS");
            InfoMsg.Add(" ] } - Send GS");
            InfoMsg.Add(" ~ ^ - Send RS");
            InfoMsg.Add(" / _ - Send US");
            InfoMsg.Add(" ? - Send DEL");
        }
        else
        {
            InfoMsg.Add(" { } - Key codes: " + TelnetKeyboardConf_ + " ");
            InfoMsg.Add(" [ ] - Modifiers: " + TelnetKeyboardMods_ + " ");
            InfoMsg.Add(" / - Connect/disconnect ");
            InfoMsg.Add(" ? - Send screen size ");
            InfoMsg.Add(" ~ - Local echo: " + (LocalEcho ? std::string("on") : std::string("off")));
            InfoMsg.Add(" | - Input commands: " + (Command_8bit ? std::string("8-bit") : std::string("7-bit")));
            InfoMsg.Add(" \\ - Display configuration");
        }
        InfoMsg.Add(" ` - Title bar");
        InfoMsg.Add(" , - Copy screen as text");
        InfoMsg.Add(" . - Paste text as keystrokes");
    }
    if (WorkStateC == WorkStateCDef::EscapeKey)
    {
        InfoMsg.Add(" Press key, which will be ");
        InfoMsg.Add(" used as new escape key   ");
    }

    int InfoW = 0;
    int InfoH = InfoMsg.Count;

    XList<Str> InfoMsg_;
    for (int i = 0; i < InfoH; i++)
    {
        InfoMsg_.Add(Str::FromString(InfoMsg[i]));
        InfoW = std::max(InfoW, InfoMsg_[i].Count);
    }
    for (int i = 0; i < InfoH; i++)
    {
        while (InfoMsg_[i].Count < InfoW)
        {
            InfoMsg_[i].Add(32);
        }
    }



    int OffsetX = 0;
    int OffsetY = 0;
    int InfoCX = InfoW;
    int InfoCY = InfoH;
    if ((TelnetInfoPos == 1) || (TelnetInfoPos == 3))
    {
        OffsetX = ScreenW - InfoW;
        InfoCX = OffsetX - 1;
    }
    if ((TelnetInfoPos == 2) || (TelnetInfoPos == 3))
    {
        OffsetY = ScreenH - InfoH;
        InfoCY = OffsetY - 1;
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

    for (int I = 0; I < InfoH; I++)
    {
        for (int II = 0; II < InfoW; II++)
        {
            Screen::ScreenChar(OffsetX + II, OffsetY + I, InfoMsg_[I][II], CB_, CF_, 0, 0, 0);
        }
    }
    Screen::ScreenCursorMove(InfoCX, InfoCY);
    Screen::ScreenRefresh();
}

void Core2Terminal::DisplayStatusRepaint()
{
    if (ScreenStatusBar > 0)
    {
        int ScrPos = (ScreenStatusBar == 1) ? 0 : (ScreenH - 1);
        if (WindowTitleStatusSize > ScreenW)
        {
            for (int I = 0; I < ScreenW; I++)
            {
                Screen::ScreenChar(I, ScrPos, WindowTitleStatus[I], StatusBack, StatusFore, 0, 0, 0);
            }
        }
        else
        {
            int StrOffset = (ScreenW - WindowTitleStatusSize) >> 1;
            for (int I = 0; I < ScreenW; I++)
            {
                Screen::ScreenChar(I, ScrPos, ' ', StatusBack, StatusFore, 0, 0, 0);
            }
            for (int I = 0; I < WindowTitleStatusSize; I++)
            {
                Screen::ScreenChar(I + StrOffset, ScrPos, WindowTitleStatus[I], StatusBack, StatusFore, 0, 0, 0);
            }
        }
    }
}

void Core2Terminal::DisplayStatusPrepare()
{
    if ((WindowTitle != "") || (WindowIcon != ""))
    {
        if (WindowTitle == WindowIcon)
        {
            WindowTitleStatus = WindowTitle;
        }
        else
        {
            if ((WindowTitle == "") || (WindowIcon == ""))
            {
                WindowTitleStatus = (WindowIcon + WindowTitle);
            }
            else
            {
                WindowTitleStatus = (WindowIcon + " " + WindowTitle);
            }
        }
    }
    else
    {
        WindowTitleStatus = "";
    }
    WindowTitleStatusSize = WindowTitleStatus.size();
    DisplayStatusRepaint();
}
