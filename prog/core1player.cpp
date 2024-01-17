#include "core1player.h"

Core1Player::Core1Player()
{

}

void Core1Player::Init()
{
    CoreAnsi_ = std::make_shared<CoreAnsi>(CF);
    DisplayConfig_.get()->CoreAnsi_ = CoreAnsi_;
    DisplayConfig_.get()->PopupBack = PopupBack;
    DisplayConfig_.get()->PopupFore = PopupFore;
    CoreAnsi_.get()->__AnsiProcessDelayFactor = CF.get()->ParamGetI("PlayerDelayFrame");

    FileOpenAtEnd = CF.get()->ParamGetB("PlayerEnd");

    LoadFileTimeChunk = (CF.get()->ParamGetI("TimerPeriod") * LoadFileTimeFactor) / 100;

    ServerPort = CF.get()->ParamGetI("ServerPort");
    ServerTelnet = CF.get()->ParamGetB("ServerTelnet");
    ServerEncoding = std::make_shared<TextCodec>(CF.get()->ParamGetI("ServerEncoding"));

    ANSIBrowseWildcard = CF.get()->ParamGetS("FileBrowseWildcard");
    if (ANSIBrowseWildcard == "")
    {
        ANSIBrowseWildcard = "*";
    }

    FileDelayStep = CF.get()->ParamGetI("PlayerStep");

    if (FileDelayStep <= 0)
    {
        FileDelayStep = 1;
    }
    CalcFileDelayStep();
}

void Core1Player::CalcFileDelayStep()
{
    FileDelayStep__ = FileDelayStep;
    int T = FileDelayStepFactor;
    while (T > 0)
    {
        FileDelayStep__ = FileDelayStep__ * 2;
        T--;
    }
    while (T < 0)
    {
        FileDelayStep__ = FileDelayStep__ / 2;
        T++;
    }
    if (FileDelayStep__ < 1)
    {
        FileDelayStep__ = 1;
    }
    if ((WorkStateS == WorkStateSDef::DisplayFwdPlay) || (WorkStateS == WorkStateSDef::DisplayRevPlay))
    {
        WorkSeekAdvance = FileDelayStep__;
    }
}

void Core1Player::EventTick()
{
    switch (WorkStateS)
    {
        case WorkStateSDef::None:
            break;
        case WorkStateSDef::InfoScreen: // Information screen before file opening
            {
                Screen::CursorHide(false);
                Screen::ScreenClear(Screen::TextNormalBack, Screen::TextNormalFore);
                Screen::ScreenCursorMove(0, 0);
                Screen_Clear();
                NewFileName.Clear();
                Screen_WriteText("During displaying:");
                Screen_WriteLine();
                Screen_WriteText("Esc or = - Return to this screen");
                Screen_WriteLine();
                Screen_WriteText("Tab - Show/hide status");
                Screen_WriteLine();
                Screen_WriteText("` - Change status information");
                Screen_WriteLine();
                Screen_WriteText("Space - File information");
                Screen_WriteLine();
                Screen_WriteText("Enter - Play/stop forward");
                Screen_WriteLine();
                Screen_WriteText("Backspace - Play/stop backward");
                Screen_WriteLine();
                Screen_WriteText("Up/Down arrow - Move 1 step");
                Screen_WriteLine();
                Screen_WriteText("Left/Right arrow - Move " + std::to_string(FileDelayStep__) + " steps");
                Screen_WriteLine();
                Screen_WriteText("Home - Move to begin of animation");
                Screen_WriteLine();
                Screen_WriteText("End - Move to end of animation");
                Screen_WriteLine();
                Screen_WriteText("] - Increase playing speed");
                Screen_WriteLine();
                Screen_WriteText("[ - Decrease playing speed");
                Screen_WriteLine();
                Screen_WriteText("/ - Reset playing speed");
                Screen_WriteLine();
                Screen_WriteText("Page Up/Page Down - Previous/Next file");
                Screen_WriteLine();
                Screen_WriteText(", - Copy screen to clipboard");
                Screen_WriteLine();
                Screen_WriteText(". - File manager");
                Screen_WriteLine();
                Screen_WriteText("\\ - Display configuration");
                Screen_WriteLine();
                Screen_WriteLine();
                Screen_WriteLine();
                Screen_WriteText("Press Enter to start displaying or Tab to quit application.");
                Screen_WriteLine();
                Screen_Refresh();
                WorkStateS = WorkStateSDef::InfoScreenWaitForKey;
            }
            break;
        case WorkStateSDef::InfoScreenWaitForKey: // Waiting for user key press before opening file
            break;
        case WorkStateSDef::FileOpenFile0: // Waiting for file contents
            {
                Screen::CursorHide(false);
                Screen_Clear();
                Screen_Refresh();
                WorkStateS = WorkStateSDef::FileOpenFile;
                BinaryFile_.get()->FileImport(BinaryFile_.get()->CurrentFileName);
            }
            break;
        case WorkStateSDef::FileOpenFile: // Waiting for file contents
            break;
        case WorkStateSDef::FileOpen: // Opening file
            {
                Screen::CursorHide(false);
                WorkSeekOneChar = false;
                Screen_Clear();
                Screen_Refresh();
                FileCtX.Clear();
                FileCtX_.Clear();
                BinaryFile_.get()->LoadRaw(BinaryFile_.get()->CurrentFileName, FileCtX_);
                AnsiSauce_.Info.Clear();
                AnsiSauce_.LoadRaw(FileCtX_);
                int FileType = BinaryFile_.get()->GetFileType(BinaryFile_.get()->CurrentFileName);
                switch (FileType)
                {
                    case 0: // TXT
                        BinaryFile_.get()->Load(BinaryFile_.get()->CurrentFileName, FileCtX, 1);
                        break;
                    case 1: // ANSI
                        BinaryFile_.get()->Load(BinaryFile_.get()->CurrentFileName, FileCtX, 0);
                        break;
                    case 2: // BIN
                        AnsiSauce_.CreateInfo();
                        XBIN_.SetRaw(FileCtX_, AnsiSauce_.DataIdx, false);
                        XBIN_.GetStr(FileCtX, BinaryFile_.get()->CurrentFileAttrGet(0));
                        break;
                    case 3: // XBIN
                        AnsiSauce_.CreateInfo();
                        XBIN_.SetRaw(FileCtX_, AnsiSauce_.DataIdx, true);
                        XBIN_.GetStr(FileCtX, BinaryFile_.get()->CurrentFileAttrGet(0));
                        break;
                }
                Screen::ScreenClear(Screen::TextNormalBack, Screen::TextNormalFore);
                FilePos = 0;
                switch (FileType)
                {
                    case 0: // TXT
                        CoreAnsi_.get()->AnsiProcessReset(true, false, 1, 0);
                        break;
                    case 1: // ANSI
                        CoreAnsi_.get()->AnsiProcessReset(true, false, 1, 1);
                        break;
                    case 2: // BIN
                    case 3: // XBIN
                        CoreAnsi_.get()->AnsiProcessReset(true, false, 1, 3);
                        break;
                }
                CoreAnsi_.get()->AnsiTerminalResize(Screen::CurrentW, Screen::CurrentH, ScreenStatusBar);
                CoreAnsi_.get()->AnsiProcessSupply(FileCtX);


                MoviePos = 0;
                MovieLength = 0;

                WorkStateS = WorkStateSDef::FileOpenWait;
                LoadFileChunk = 10;
            }
            break;
        case WorkStateSDef::FileOpenWait:
            {
                FileTimeMeasure.Reset();
                while (FileTimeMeasure.Elapsed() < LoadFileTimeChunk)
                {
                    int MovieLength0 = CoreAnsi_.get()->AnsiProcess(LoadFileChunk);
                    if (MovieLength0 > 0)
                    {
                        MovieLength = MovieLength + MovieLength0;
                    }
                    else
                    {
                        WorkStateS = WorkStateSDef::FileOpenPrepare;
                        break;
                    }
                }
                Screen_Clear();
                Screen_WriteText(std::to_string(CoreAnsi_.get()->AnsiState_.AnsiBufferI) + "/" + std::to_string(FileCtX.Count));
                Screen_Refresh();
            }
            break;
        case WorkStateSDef::FileOpenPrepare:
            {
                Screen::CursorHide(false);
                Screen_Clear();
                Screen_Refresh();
                int FileType = BinaryFile_.get()->GetFileType(BinaryFile_.get()->CurrentFileName);


                AnsiSauce_.NonSauceInfo("Index", std::to_string(BinaryFile_.get()->DirItemIdx + 1) + "/" + std::to_string(BinaryFile_.get()->DirItemName.Count));
                AnsiSauce_.NonSauceInfo("File name", BinaryFile_.get()->CurrentFileName.ToString());
                switch (FileType)
                {
                    case 0: // TXT
                        AnsiSauce_.NonSauceInfo("File type", "TEXT");
                        break;
                    case 1: // ANSI
                        AnsiSauce_.NonSauceInfo("File type", "ANSI");
                        break;
                    case 2: // BIN
                        AnsiSauce_.NonSauceInfo("File type", "BIN");
                        AnsiSauce_.NonSauceInfo("Binary data length", (XBIN_.DataRawLength / 2));
                        break;
                    case 3: // XBIN
                        AnsiSauce_.NonSauceInfo("File type", "XBIN");
                        AnsiSauce_.NonSauceInfo("Binary data length", (XBIN_.DataRawLength / 2));
                        break;
                }
                AnsiSauce_.NonSauceInfo("Steps", MovieLength);
                AnsiSauce_.NonSauceInfo("Characters", CoreAnsi_.get()->AnsiState_.AnsiBufferI);
                AnsiSauce_.NonSauceInfo("Overwrites/writes", std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharCounterOver) + "/" + std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharCounter));
                AnsiSauce_.NonSauceInfo("Inserts and deletes", CoreAnsi_.get()->AnsiState_.PrintCharInsDel);
                AnsiSauce_.NonSauceInfo("Scrolls", CoreAnsi_.get()->AnsiState_.PrintCharScroll);

                if (FileType == 3)
                {
                    AnsiSauce_.NonSauceInfo("XBIN size", std::to_string(XBIN_.FileW) + "x" + std::to_string(XBIN_.FileH));
                    AnsiSauce_.NonSauceInfo("XBIN palette", (XBIN_.DataPal.Count > 0) ? "Yes" : "No");
                    if (XBIN_.FontSize > 0)
                    {
                        AnsiSauce_.NonSauceInfo("XBIN fonts", std::to_string(XBIN_.FontN) + ", " + std::to_string(XBIN_.FontSize) + "px");
                    }
                    else
                    {
                        AnsiSauce_.NonSauceInfo("XBIN fonts", XBIN_.FontN);
                    }
                    AnsiSauce_.NonSauceInfo("XBIN compression", (XBIN_.DataCompression) ? "Yes" : "No");
                }

                AnsiSauce_.CreateInfo();
                MoviePos = 0;
                switch (FileType)
                {
                    case 0: // TXT
                        CoreAnsi_.get()->AnsiProcessReset(true, true, 2, 0);
                        break;
                    case 1: // ANSI
                        CoreAnsi_.get()->AnsiProcessReset(true, true, 2, 1);
                        break;
                    case 2: // BIN
                    case 3: // XBIN
                        CoreAnsi_.get()->AnsiProcessReset(true, true, 2, 3);
                        break;
                }
                CoreAnsi_.get()->AnsiProcessSupply(FileCtX);

                /*!!!!if (Server_ != null)
                {
                    Server_.Send(UniConn.HexToRaw(Server_.TerminalResetCommand));
                }*/



                WorkSeekAdvance = FileOpenAtEnd ? MovieLength : 0;
                Repaint(true);
                if (WorkSeekAdvance > 0)
                {
                    WorkStateS = WorkStateSDef::DisplayFwdStep;
                }
                else
                {
                    WorkStateS = WorkStateSDef::DisplayPause;
                }
            }
            break;
        case WorkStateSDef::DisplayFwdStep: // Play forward
        case WorkStateSDef::DisplayFwdPlay: // Play forward
            {
                if (WorkSeekAdvance > (MovieLength - MoviePos))
                {
                    WorkSeekAdvance = (MovieLength - MoviePos);
                }

                bool NeedRepaint = false;
                if (WorkSeekAdvance > 0)
                {
                    int CharsToSend = CoreAnsi_.get()->AnsiState_.AnsiBufferI;
                    NeedRepaint = CoreAnsi_.get()->AnsiSeek(WorkSeekAdvance);
                    CharsToSend = CoreAnsi_.get()->AnsiState_.AnsiBufferI - CharsToSend;

                    /*!!!!!!if (Server_ != null)
                    {
                        Server_.Send(ServerEncoding.GetBytes(TextWork.IntToStr(FileCtX.GetRange(FilePos, CharsToSend))));
                    }*/
                    FilePos = FilePos + CharsToSend;
                    MoviePos += WorkSeekAdvance;
                }
                else
                {
                    WorkStateS = WorkStateSDef::DisplayPause;
                }

                if (WorkStateS == WorkStateSDef::DisplayFwdStep)
                {
                    WorkStateS = WorkStateSDef::DisplayPause;
                }
                Repaint(false);
            }
            break;
        case WorkStateSDef::DisplayRevStep: // Play backward
        case WorkStateSDef::DisplayRevPlay: // Play backward
            {
                if (WorkSeekAdvance > MoviePos)
                {
                    WorkSeekAdvance = MoviePos;
                }

                bool NeedRepaint = false;
                if (WorkSeekAdvance > 0)
                {
                    int CharsToSend = CoreAnsi_.get()->AnsiState_.AnsiBufferI;
                    NeedRepaint = CoreAnsi_.get()->AnsiSeek(0 - WorkSeekAdvance);
                    CharsToSend = CharsToSend - CoreAnsi_.get()->AnsiState_.AnsiBufferI;

                    FilePos = FilePos - CharsToSend;
                    /*if (Server_ != null)
                    {
                        Server_.Send(UniConn.HexToRaw(Server_.TerminalResetCommand));
                        Server_.Send(ServerEncoding.GetBytes(TextWork.IntToStr(FileCtX.GetRange(0, FilePos))));
                    }*/
                    MoviePos -= WorkSeekAdvance;
                }
                else
                {
                    WorkStateS = WorkStateSDef::DisplayPause;
                }

                if (WorkStateS == WorkStateSDef::DisplayRevStep)
                {
                    WorkStateS = WorkStateSDef::DisplayPause;
                }
                Repaint(false);
            }
            break;
        case WorkStateSDef::DisplayPause: // Display paused
            if (FileOpenFile0Info)
            {
                FileOpenFile0Info = false;
                InfoPosH = 0;
                InfoPosV = 0;
                WorkStateS = WorkStateSDef::DisplayInfo;
                DisplayInfoText(true);
            }
            break;
        case WorkStateSDef::DisplayInfo: // Sauce info
            {
                DisplayInfoText(false);
            }
            break;
        case WorkStateSDef::FileMan: // File manager
            FileManager_.EventTick();
            break;
        case WorkStateSDef::DispConf: // Display configuration
            break;
    }
}

void Core1Player::EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    std::string WorkDisp = "__";

    switch (WorkStateS)
    {
        default:
            break;
        case WorkStateSDef::InfoScreenWaitForKey:
            WorkDisp = "0_";
            break;
        case WorkStateSDef::DisplayFwdStep:
        case WorkStateSDef::DisplayRevStep:
        case WorkStateSDef::DisplayFwdPlay:
        case WorkStateSDef::DisplayRevPlay:
        case WorkStateSDef::DisplayPause:
            WorkDisp = "1_";
            break;
        case WorkStateSDef::DisplayInfo:
            WorkDisp = "2_";
            break;
        case WorkStateSDef::FileOpenWait:
            WorkDisp = "3_";
            break;
        case WorkStateSDef::FileMan:
            FileManager_.EventKey(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            if (FileManager_.RequestRepaint)
            {
                Repaint(true);
                FileManager_.Repaint();
            }
            if (FileManager_.RequestCloseOld)
            {
                Repaint(true);
                WorkStateS = WorkStateSDef::DisplayPause;
                EventTickX();
            }
            if (FileManager_.RequestCloseNew)
            {
                Repaint(true);
                WorkStateS = WorkStateSDef::FileOpenFile0;
                EventTickX();
            }
            return;
        case WorkStateSDef::DispConf:
            DisplayConfig_.get()->EventKey(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            DisplayConfig_.get()->Repaint();
            if (DisplayConfig_.get()->RequestSave)
            {
                SaveConfig();
            }
            if (DisplayConfig_.get()->RequestRepaint)
            {
                Repaint(true);
                DisplayConfig_.get()->Repaint();
            }
            if (DisplayConfig_.get()->RequestClose)
            {
                WorkStateS = WorkStateSDef::DisplayPause;
                Repaint(true);
            }
            else
            {
                if (DisplayConfig_.get()->RequestResize)
                {
                    EventOther("Resize", "", DisplayConfig_.get()->ResizeW, DisplayConfig_.get()->ResizeH, 1, 0);
                }
                DisplayConfig_.get()->Repaint();
            }
            EventTickX();
            return;
    }


    switch (_((WorkDisp + KeyName).c_str()))
    {
        case _("0_Enter"):
        case _("0_NumpadEnter"):
            {
                WorkStateS = WorkStateSDef::FileOpenFile0;
                EventTickX();
            }
            return;
        case _("0_Tab"):
            {
                ScreenStatusBarSet(0);
                Screen_Clear();
                AppExit = true;
            }
            return;
        case _("1_Space"):
            InfoPosH = 0;
            InfoPosV = 0;
            WorkStateS = WorkStateSDef::DisplayInfo;
            DisplayInfoText(true);
            return;
        case _("1_Backspace"):
            {
                if ((WorkStateS == WorkStateSDef::DisplayFwdPlay) || (WorkStateS == WorkStateSDef::DisplayRevPlay))
                {
                    WorkStateS = WorkStateSDef::DisplayPause;
                }
                else
                {
                    WorkSeekAdvance = FileDelayStep__;
                    WorkStateS = WorkStateSDef::DisplayRevPlay;
                }
            }
            return;
        case _("1_Enter"):
        case _("1_NumpadEnter"):
            {
                if ((WorkStateS == WorkStateSDef::DisplayFwdPlay) || (WorkStateS == WorkStateSDef::DisplayRevPlay))
                {
                    WorkStateS = WorkStateSDef::DisplayPause;
                }
                else
                {
                    WorkSeekAdvance = FileDelayStep__;
                    WorkStateS = WorkStateSDef::DisplayFwdPlay;
                }
            }
            return;
        case _("1_Tab"):
            {
                int DisplayStatus_ = (DisplayStatus % 3);
                DisplayStatus++;
                if ((DisplayStatus % 3) == 0)
                {
                    DisplayStatus -= 3;
                }
                if ((DisplayStatus_ == 0) && ((DisplayStatus % 3) > 0)) ScreenH++;
                if ((DisplayStatus_ > 0) && ((DisplayStatus % 3) == 0)) ScreenH--;
                ScreenStatusBarSet(DisplayStatus % 3);
                Repaint(true);
            }
            return;
        case _("1_ArrowUp"):
            {
                WorkSeekAdvance = 1;
                WorkStateS = WorkStateSDef::DisplayRevStep;
                EventTickX();
            }
            return;
        case _("1_ArrowDown"):
            {
                WorkSeekAdvance = 1;
                WorkStateS = WorkStateSDef::DisplayFwdStep;
                EventTickX();
            }
            return;
        case _("1_ArrowLeft"):
            {
                WorkSeekAdvance = FileDelayStep__;
                WorkStateS = WorkStateSDef::DisplayRevStep;
                EventTickX();
            }
            return;
        case _("1_ArrowRight"):
            {
                WorkSeekAdvance = FileDelayStep__;
                WorkStateS = WorkStateSDef::DisplayFwdStep;
                EventTickX();
            }
            return;
        case _("1_Home"):
            {
                if (MoviePos == 0)
                {
                    FileOpenAtEnd = false;
                    CF.get()->ParamSet("PlayerEnd", 0);
                    SaveConfig();
                }
                WorkSeekAdvance = MovieLength;
                WorkStateS = WorkStateSDef::DisplayRevStep;
                EventTickX();
            }
            return;
        case _("1_End"):
            {
                if (MoviePos == MovieLength)
                {
                    FileOpenAtEnd = true;
                    CF.get()->ParamSet("PlayerEnd", 1);
                    SaveConfig();
                }
                WorkSeekAdvance = MovieLength;
                WorkStateS = WorkStateSDef::DisplayFwdStep;
                EventTickX();
            }
            return;
        case _("1_Escape"):
        case _("3_Escape"):
        case _("1_Equal"):
        case _("3_Equal"):
            {
                WorkStateS = WorkStateSDef::InfoScreen;
                EventTickX();
            }
            return;
        case _("1_PageUp"):
        case _("2_PageUp"):
        case _("3_PageUp"):
            {
                int FType = -1;
                while (FType < 0)
                {
                    if (BinaryFile_.get()->DirItemIdx > 0)
                    {
                        BinaryFile_.get()->DirItemIdx--;
                    }
                    else
                    {
                        BinaryFile_.get()->DirItemIdx = (BinaryFile_.get()->DirItemName.Count - 1);
                    }
                    FType = BinaryFile_.get()->GetFileType(BinaryFile_.get()->DirItemIdx);
                }
                BinaryFile_.get()->CurrentFileName = BinaryFile_.get()->DirItemName[BinaryFile_.get()->DirItemIdx];
                if (WorkDisp == "2_")
                {
                    FileOpenFile0Info = true;
                }
                WorkStateS = WorkStateSDef::FileOpenFile0;
                EventTickX();
            }
            return;
        case _("1_PageDown"):
        case _("2_PageDown"):
        case _("3_PageDown"):
            {
                int FType = -1;
                while (FType < 0)
                {
                    if (BinaryFile_.get()->DirItemIdx < (BinaryFile_.get()->DirItemName.Count - 1))
                    {
                        BinaryFile_.get()->DirItemIdx++;
                    }
                    else
                    {
                        BinaryFile_.get()->DirItemIdx = 0;
                    }
                    FType = BinaryFile_.get()->GetFileType(BinaryFile_.get()->DirItemIdx);
                }
                BinaryFile_.get()->CurrentFileName = BinaryFile_.get()->DirItemName[BinaryFile_.get()->DirItemIdx];
                if (WorkDisp == "2_")
                {
                    FileOpenFile0Info = true;
                }
                WorkStateS = WorkStateSDef::FileOpenFile0;
                EventTickX();
            }
            return;
        case _("1_Backquote"):
            if ((DisplayStatus < 12))
            {
                DisplayStatus += 3;
            }
            else
            {
                DisplayStatus -= 12;
            }
            Repaint(false);
            return;
        case _("1_BracketLeft"):
            if (FileDelayStep__ > 1)
            {
                FileDelayStepFactor--;
            }
            CalcFileDelayStep();
            Repaint(false);
            return;
        case _("1_BracketRight"):
            FileDelayStepFactor++;
            if ((FileDelayStep__ * 2) < 1)
            {
                FileDelayStepFactor--;
            }
            CalcFileDelayStep();
            Repaint(false);
            return;
        case _("1_Slash"):
            FileDelayStepFactor = 0;
            CalcFileDelayStep();
            Repaint(false);
            return;
        case _("1_Comma"):
            Clipboard_.SetText(CoreAnsi_.get()->AnsiState_.GetScreen(0, 0, ScreenW - 1, ScreenH - 1));
            Screen::FileExport(0, "", "", Clipboard_.SystemText);
            return;
        case _("1_Period"):
            WorkStateS = WorkStateSDef::FileMan;
            FileManager_.Open();
            return;
        case _("1_Backslash"):
            WorkStateS = WorkStateSDef::DispConf;
            DisplayConfig_.get()->Open();
            EventTickX();
            return;
        case _("2_ArrowUp"):
            InfoPosV--;
            DisplayInfoRepaint = true;
            EventTickX();
            return;
        case _("2_ArrowDown"):
            InfoPosV++;
            DisplayInfoRepaint = true;
            EventTickX();
            return;
        case _("2_ArrowLeft"):
            InfoPosH--;
            DisplayInfoRepaint = true;
            EventTickX();
            return;
        case _("2_ArrowRight"):
            InfoPosH++;
            DisplayInfoRepaint = true;
            EventTickX();
            return;
        case _("2_Space"):
        case _("2_Backspace"):
        case _("2_Enter"):
        case _("2_NumpadEnter"):
        case _("2_Escape"):
            Repaint(true);
            WorkStateS = WorkStateSDef::DisplayPause;
            EventTickX();
            return;
    }
}

void Core1Player::EventOther(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4)
{
    switch (_(EvtName.c_str()))
    {
        case _("Resize"):
            ScreenW = EvtParam1;
            ScreenH = EvtParam2;
            Screen::ScreenResize(ScreenW, ScreenH);
            CoreAnsi_.get()->AnsiTerminalResize(ScreenW, ScreenH, ScreenStatusBar);
            switch (WorkStateS)
            {
                case WorkStateSDef::InfoScreenWaitForKey:
                case WorkStateSDef::DisplayInfo:
                    Screen_Refresh();
                    break;
                default:
                    break;
            }
            break;
        case _("FileImport"):
            {
                if ((EvtParam2 == 2) && (WorkStateS == WorkStateSDef::FileMan))
                {
                    FileManager_.Repaint();
                }
                if (EvtParam2 == 1)
                {
                    bool ImportAllowed = false;
                    if (WorkStateS == WorkStateSDef::FileOpenFile)
                    {
                        ImportAllowed = true;
                    }
                    if ((WorkStateS == WorkStateSDef::FileMan) && (FileManager_.RequestCloseOld || FileManager_.RequestCloseNew))
                    {
                        ImportAllowed = true;
                    }
                    if (ImportAllowed)
                    {
                        FileManager_.RequestCloseOld = false;
                        FileManager_.RequestCloseNew = false;
                        if (EvtParam2 != 0)
                        {
                            WorkStateS = WorkStateSDef::FileOpen;
                        }
                    }
                }
            }
            break;
    }
}

void Core1Player::EventTickX()
{
    TimerTick = false;
    EventTick();
    TimerTick = true;
}

void Core1Player::Repaint(bool Force)
{
    int DisplayStatusMod = (DisplayStatus % 3);
    int DisplayStatusDiv = (DisplayStatus / 3);

    if (DisplayStatusMod != 0)
    {
        if (Force)
        {
            CoreAnsi_.get()->AnsiRepaint(false);
        }

        Str CharMsg;
        int MoviePosLead = std::to_string(MovieLength).size() - std::to_string(MoviePos).size();
        switch (DisplayStatusDiv)
        {
            case 0:
                {
                    Str CharMsgIdx;
                    while (MoviePosLead > 0)
                    {
                        CharMsgIdx.Add(' ');
                        MoviePosLead--;
                    }
                    CharMsgIdx.AddString(std::to_string(MoviePos) + "/" + std::to_string(MovieLength));
                    CharMsgIdx.AddString(" | " + std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharCounterOver) + "/" + std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharCounter) + " " + std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharInsDel) + " " + std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharScroll));
                    CharMsgIdx.AddString(" | " + std::to_string(BinaryFile_.get()->DirItemIdx + 1) + "/" + std::to_string(BinaryFile_.get()->DirItemName.Count) + ((AnsiSauce_.DataIdx >= 0) ? "= " : ": "));

                    CharMsg.AddRange(BinaryFile_.get()->CurrentFileName);
                    int MaxS = (ScreenW - CharMsgIdx.Count);
                    if (CharMsg.Count > MaxS)
                    {
                        CharMsg.RemoveRange(CharMsg.Count - MaxS + 3);
                        CharMsg.InsertRange(0, '.');
                        CharMsg.InsertRange(0, '.');
                        CharMsg.InsertRange(0, '.');
                    }
                    CharMsg.InsertRange(0, CharMsgIdx);
                    CharMsg.Add(' ');
                }
                break;
            case 1:
                {
                    while (MoviePosLead > 0)
                    {
                        CharMsg.Add(' ');
                        MoviePosLead--;
                    }
                    CharMsg.AddString(std::to_string(MoviePos) + "/" + std::to_string(MovieLength));
                    CharMsg.AddString(" | " + std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharCounterOver) + "/" + std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharCounter) + " " + std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharInsDel) + " " + std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharScroll));
                    CharMsg.AddString(" | " + TextWork::NumPlusMinus(FileDelayStepFactor));
                    CharMsg.AddString(" | " + std::to_string(FileDelayStep__));

                    if (CoreAnsi_.get()->AnsiState_.__AnsiProcessDelayMin > CoreAnsi_.get()->AnsiState_.__AnsiProcessDelayMax)
                    {
                        CharMsg.AddString(" | ? ?  ");
                    }
                    else
                    {
                        CharMsg.AddString(" | " + std::to_string(CoreAnsi_.get()->AnsiState_.__AnsiProcessDelayMin) + " " + std::to_string(CoreAnsi_.get()->AnsiState_.__AnsiProcessDelayMax) + "  ");
                    }
                }
                break;
            case 2:
            case 3:
            case 4:
                {
                    Str CharCounter;
                    while (MoviePosLead > 0)
                    {
                        CharCounter.Add(' ');
                        MoviePosLead--;
                    }
                    CharCounter.AddString(std::to_string(MoviePos) + "/" + std::to_string(MovieLength));
                    int FileCtXI = CoreAnsi_.get()->AnsiState_.AnsiBufferI;
                    CharCounter.AddString(" | ");
                    MoviePosLead = std::to_string(FileCtX.Count).size() - std::to_string(FileCtXI).size();
                    while (MoviePosLead > 0)
                    {
                        CharCounter.Add(' ');
                        MoviePosLead--;
                    }
                    CharCounter.AddString(std::to_string(FileCtXI) + "/" + std::to_string(FileCtX.Count) + ": ");

                    int PrevewLength = (ScreenW - CharCounter.Count - 2) / 6;

                    for (int iii = (0 - PrevewLength); iii < PrevewLength; iii++)
                    {
                        if (iii == 0)
                        {
                            CharMsg.AddString(" | ");
                        }
                        else
                        {
                            CharMsg.Add(' ');
                        }
                        if (((FileCtXI + iii) >= 0) && ((FileCtXI + iii) < FileCtX.Count))
                        {
                            bool DispChar = false;
                            int DispChrN = FileCtX[FileCtXI + iii];
                            switch (DisplayStatusDiv)
                            {
                                case 2:
                                    {
                                        DispChar = (DispChrN >= 33);
                                    }
                                    break;
                                case 3:
                                    {
                                        DispChar = ((DispChrN >= 33) && (DispChrN <= 126));
                                    }
                                    break;
                            }
                            if (DispChar)
                            {
                                if (Screen::CharDouble(DispChrN))
                                {
                                    CharMsg.Add(DispChrN);
                                    CharMsg.Add('_');
                                }
                                else
                                {
                                    CharMsg.Add('_');
                                    CharMsg.Add(DispChrN);
                                }
                            }
                            else
                            {
                                if (DispChrN > 255)
                                {
                                    if (iii < (PrevewLength - 1))
                                    {
                                        CharMsg.AddString(TextWork::CharCode(DispChrN, 2));
                                    }
                                    else
                                    {
                                        CharMsg.AddString("  ");
                                    }
                                    if (iii < 0)
                                    {
                                        CharMsg.PopFirst();
                                        CharMsg.PopFirst();
                                        CharMsg.PopFirst();
                                    }
                                    else
                                    {
                                        PrevewLength--;
                                    }
                                }
                                else
                                {
                                    CharMsg.AddString(TextWork::CharCode(DispChrN, 0));
                                }
                            }
                        }
                        else
                        {
                            CharMsg.AddString("  ");
                        }
                    }
                    if (CharMsg[0] != ' ')
                    {
                        CharMsg.PopFirst();
                        CharMsg.PopFirst();
                        CharMsg.PopFirst();
                        CharMsg.Insert(' ');
                        CharMsg.Insert(' ');
                        CharMsg.Insert(' ');
                    }

                    CharMsg.Add(' ');
                    CharMsg.InsertPad(ScreenW - CharCounter.Count, ' ');
                    CharMsg.InsertRange(CharCounter);
                }
                break;
        }
        if (CharMsg.Count > ScreenW)
        {
            CharMsg.RemoveRange(ScreenW);
        }
        CharMsg.AddPad(ScreenW, ' ');

        if (DisplayStatusMod == 1)
        {
            Screen::ScreenCursorMove(0, 0);
            Screen::ScreenWriteText0(CharMsg, StatusBack, StatusFore);
        }
        if (DisplayStatusMod == 2)
        {
            Screen::ScreenCursorMove(0, ScreenH - 1);
            Screen::ScreenWriteText0(CharMsg, StatusBack, StatusFore);
        }
    }
    else
    {
        if (Force)
        {
            CoreAnsi_.get()->AnsiRepaint(false);
        }
    }
    Screen::CursorHide(CoreAnsi_.get()->AnsiState_.CursorHide);
    CoreAnsi_.get()->AnsiRepaintCursor();
}

void Core1Player::DisplayInfoText(bool Forced)
{
    if (Forced)
    {
        DisplayInfoRepaint = true;
    }

    if (DisplayInfoRepaint)
    {
        if (InfoPosH < 0) { InfoPosH = 0; }
        if (InfoPosH > 80) { InfoPosH = 80; }
        if (InfoPosV < 0) { InfoPosV = 0; }
        if (InfoPosV >= AnsiSauce_.Info.Count) { InfoPosV = AnsiSauce_.Info.Count - 1; }
        Screen_Clear();
        int CountX = AnsiSauce_.Info.Count;
        if (ScreenH < AnsiSauce_.Info.Count - InfoPosV)
        {
            CountX = ScreenH + InfoPosV;
        }
        for (int i = InfoPosV; i < CountX; i++)
        {
            std::string T = AnsiSauce_.Info[i];
            if (InfoPosH > 0)
            {
                if (T.size() >= InfoPosH)
                {
                    T = T.substr(InfoPosH);
                }
                else
                {
                    T = "";
                }
            }
            if (i < (ScreenH + InfoPosV - 1))
            {
                if (T.size() > ScreenW)
                {
                    T = T.substr(0, ScreenW);
                }
                Screen_WriteText(T);
                if (T.size() < ScreenW)
                {
                    Screen_WriteLine();
                }
            }
            else
            {
                if (T.size() >= ScreenW)
                {
                    T = T.substr(0, ScreenW - 1);
                }
                Screen_WriteText(T);
            }
        }
        Screen_Refresh();
        DisplayInfoRepaint = false;
    }
}
