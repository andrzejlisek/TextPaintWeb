#include "core1player.h"

Core1Player::Core1Player()
{

}

void Core1Player::Init()
{
    //LoadFileMeasuring = true;
    //Stopwatch_.MeasureTick();

    Stopwatch_.Reset();

    CoreAnsi_ = std::make_shared<CoreAnsi>(CF);
    DisplayConfig_.get()->CoreAnsi_ = CoreAnsi_;
    DisplayConfig_.get()->PopupBack = PopupBack;
    DisplayConfig_.get()->PopupFore = PopupFore;
    CoreAnsi_.get()->__AnsiProcessDelayFactor = CF.get()->ParamGetI("FileDelayFrame");

    LoadFileTimeChunk = (CF.get()->ParamGetI("TimerPeriod") * LoadFileTimeFactor) / 100;

    ServerPort = CF.get()->ParamGetI("ServerPort");
    ServerTelnet = CF.get()->ParamGetB("ServerTelnet");
    ServerEncoding = std::make_shared<TextCodec>(CF.get()->ParamGetI("ServerEncoding"));

    ANSIBrowseWildcard = CF.get()->ParamGetS("FileBrowseWildcard");
    if (ANSIBrowseWildcard == "")
    {
        ANSIBrowseWildcard = "*";
    }

    FileDelayTime = CF.get()->ParamGetI("FileDelayTime");
    FileDelayStep = CF.get()->ParamGetI("FileDelayStep");
    FileDelayOffset = CF.get()->ParamGetI("FileDelayOffset");

    if (FileDelayTime < 0)
    {
        FileDelayTime = 0;
    }
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
                Screen::ScreenClear(Screen::TextNormalBack, Screen::TextNormalFore);
                Screen::ScreenCursorMove(0, 0);
                Screen_Clear();
                NewFileName.Clear();
                Screen_WriteText("During displaying:");
                Screen_WriteLine();
                Screen_WriteText("Esc - Return to this screen");
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
                Screen_Clear();
                Screen_Refresh();
                WorkStateS = WorkStateSDef::FileOpenFile;
                BinaryFile_.get()->FileImport(-1, false);
            }
            break;
        case WorkStateSDef::FileOpenFile: // Waiting for file contents
            break;
        case WorkStateSDef::FileOpen: // Opening file
            {
                WorkSeekOneChar = false;
                Screen_Clear();
                Screen_Refresh();
                FileCtX.Clear();
                BinaryFile_.get()->Load(FileCtX);
                DispBuffer.Clear();
                AnsiSauce_.Info.Clear();
                AnsiSauce_.LoadRaw(FileCtX_);
                Screen::ScreenClear(Screen::TextNormalBack, Screen::TextNormalFore);
                FilePos = 0;
                CoreAnsi_.get()->AnsiProcessReset(true, false, 1);
                CoreAnsi_.get()->AnsiTerminalResize(Screen::CurrentW, Screen::CurrentH);
                CoreAnsi_.get()->AnsiProcessSupply(FileCtX);


                MoviePos = 0;
                MovieLength = 0;
                /*if (!LoadFileMeasuring)
                {
                    LoadFileMeasuring = true;
                    Stopwatch_.MeasureTick();
                }*/

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
                Screen_Clear();
                Screen_Refresh();

                AnsiSauce_.NonSauceInfo("Index", std::to_string(BinaryFile_.get()->ItemIndex + 1) + "/" + std::to_string(BinaryFile_.get()->ItemCount()));
                AnsiSauce_.NonSauceInfo("File name", BinaryFile_.get()->ItemName(BinaryFile_.get()->ItemIndex).ToString());
                AnsiSauce_.NonSauceInfo("Steps", MovieLength);
                AnsiSauce_.NonSauceInfo("Characters", CoreAnsi_.get()->AnsiState_.AnsiBufferI);
                AnsiSauce_.NonSauceInfo("Overwrites/writes", std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharCounterOver) + "/" + std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharCounter));
                AnsiSauce_.NonSauceInfo("Inserts and deletes", CoreAnsi_.get()->AnsiState_.PrintCharInsDel);
                AnsiSauce_.NonSauceInfo("Scrolls", CoreAnsi_.get()->AnsiState_.PrintCharScroll);

                AnsiSauce_.CreateInfo();
                MoviePos = 0;
                CoreAnsi_.get()->AnsiProcessReset(true, true, 2);
                CoreAnsi_.get()->AnsiProcessSupply(FileCtX);

                /*!!!!if (Server_ != null)
                {
                    Server_.Send(UniConn.HexToRaw(Server_.TerminalResetCommand));
                }*/

                /*!!!!!!!!!!!!if (FileDelayOffset < 0)
                {
                    WorkSeekAdvance = MovieLength + FileDelayOffset;
                    if (WorkSeekAdvance < 0)
                    {
                        WorkSeekAdvance = 0;
                    }
                }
                else
                {
                    WorkSeekAdvance = FileDelayOffset;
                    if (WorkSeekAdvance > MovieLength)
                    {
                        WorkSeekAdvance = MovieLength;
                    }
                }*/
                WorkSeekAdvance = 0;
                Repaint(true);
                WorkStateS = WorkStateSDef::DisplayPause;
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
                    if (CharsToSend <= (DispBufferLen + DispBufferLen))
                    {
                        while (CharsToSend > 0)
                        {
                            DispBuffer.Add(FileCtX[FilePos]);
                            FilePos++;
                            CharsToSend--;
                        }
                        while (DispBuffer.Count > (DispBufferLen + DispBufferLen))
                        {
                            DispBuffer.RemoveRange(0, DispBufferLen);
                        }
                    }
                    else
                    {
                        DispBuffer.Clear();
                        int StartI = CoreAnsi_.get()->AnsiState_.AnsiBufferI - DispBufferLen;
                        if (StartI < 0)
                        {
                            StartI = 0;
                        }
                        for (int i = StartI; i < CoreAnsi_.get()->AnsiState_.AnsiBufferI; i++)
                        {
                            DispBuffer.Add(FileCtX[i]);
                        }
                        FilePos = FilePos + CharsToSend;
                    }
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
                    int BufI = CoreAnsi_.get()->AnsiState_.AnsiBufferI;
                    NeedRepaint = CoreAnsi_.get()->AnsiSeek(0 - WorkSeekAdvance);
                    CharsToSend = CharsToSend - CoreAnsi_.get()->AnsiState_.AnsiBufferI;

                    int BufL = WorkSeekAdvance + DispBufferLen;
                    if (BufL >= BufI)
                    {
                        BufL = BufI - 1;
                    }
                    BufI = BufI - DispBuffer.Count - 1;
                    if (CharsToSend <= (DispBufferLen + DispBufferLen))
                    {
                        while (DispBuffer.Count <= BufL)
                        {
                            DispBuffer.Insert(0, FileCtX[BufI]);
                            BufI--;
                        }
                        while (CharsToSend > 0)
                        {
                            DispBuffer.Remove(DispBuffer.Count - 1);
                            FilePos--;
                            CharsToSend--;
                        }
                    }
                    else
                    {
                        DispBuffer.Clear();
                        int StartI = CoreAnsi_.get()->AnsiState_.AnsiBufferI - DispBufferLen;
                        if (StartI < 0)
                        {
                            StartI = 0;
                        }
                        for (int i = StartI; i < CoreAnsi_.get()->AnsiState_.AnsiBufferI; i++)
                        {
                            DispBuffer.Add(FileCtX[i]);
                        }
                        FilePos = FilePos - CharsToSend;
                    }
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
                    EventOther("Resize", "", DisplayConfig_.get()->ResizeW, DisplayConfig_.get()->ResizeH, 0, 0);
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
                DisplayStatus++;
                if ((DisplayStatus % 3) == 0)
                {
                    DisplayStatus -= 3;
                }
                ForceRepaint = true;
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
                WorkSeekAdvance = MovieLength;
                WorkStateS = WorkStateSDef::DisplayRevStep;
                EventTickX();
            }
            return;
        case _("1_End"):
            {
                WorkSeekAdvance = MovieLength;
                WorkStateS = WorkStateSDef::DisplayFwdStep;
                EventTickX();
            }
            return;
        case _("1_Escape"):
            {
                WorkStateS = WorkStateSDef::InfoScreen;
                EventTickX();
            }
            return;
        case _("1_PageUp"):
            {
                int FType = -1;
                while (FType < 0)
                {
                    if (BinaryFile_.get()->ItemIndex > 0)
                    {
                        BinaryFile_.get()->ItemIndex--;
                    }
                    else
                    {
                        BinaryFile_.get()->ItemIndex = (BinaryFile_.get()->ItemCount() - 1);
                    }
                    FType = BinaryFile_.get()->ItemType(BinaryFile_.get()->ItemIndex);
                }
                WorkStateS = WorkStateSDef::FileOpenFile0;
                EventTickX();
            }
            return;
        case _("1_PageDown"):
            {
                int FType = -1;
                while (FType < 0)
                {
                    if (BinaryFile_.get()->ItemIndex < (BinaryFile_.get()->ItemCount() - 1))
                    {
                        BinaryFile_.get()->ItemIndex++;
                    }
                    else
                    {
                        BinaryFile_.get()->ItemIndex = 0;
                    }
                    FType = BinaryFile_.get()->ItemType(BinaryFile_.get()->ItemIndex);
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
            ForceRepaint = true;
            Repaint(true);
            return;
        case _("1_Minus"):
            return;
        case _("1_Equal"):
            return;
        case _("1_BracketLeft"):
            if (FileDelayStep__ > 1)
            {
                FileDelayStepFactor--;
            }
            CalcFileDelayStep();
            return;
        case _("1_BracketRight"):
            FileDelayStepFactor++;
            if ((FileDelayStep__ * 2) < 1)
            {
                FileDelayStepFactor--;
            }
            CalcFileDelayStep();
            return;
        case _("1_Slash"):
            FileDelayStepFactor = 0;
            CalcFileDelayStep();
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
            CoreAnsi_.get()->AnsiTerminalResize(ScreenW, ScreenH);
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
        CoreAnsi_.get()->AnsiRepaint(false);

        Str CharMsg;
        switch (DisplayStatusDiv)
        {
            case 0:
                {
                    Str CharMsgIdx(std::to_string(MoviePos) + "/" + std::to_string(MovieLength));
                    CharMsgIdx.AddString(" | " + std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharCounterOver) + "/" + std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharCounter) + " " + std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharInsDel) + " " + std::to_string(CoreAnsi_.get()->AnsiState_.PrintCharScroll));
                    CharMsgIdx.AddString(" | " + std::to_string(BinaryFile_.get()->ItemIndex + 1) + "/" + std::to_string(BinaryFile_.get()->ItemCount()) + (AnsiSauce_.Exists ? "= " : ": "));

                    CharMsg.AddRange(BinaryFile_.get()->ItemName(BinaryFile_.get()->ItemIndex));
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
                    CharCounter.AddString(std::to_string(MoviePos) + "/" + std::to_string(MovieLength));
                    CharCounter.AddString(" | " + std::to_string(CoreAnsi_.get()->AnsiState_.AnsiBufferI) + "/" + std::to_string(FileCtX.Count) + ": ");
                    for (int iii = DispBuffer.Count - 1; iii >= 0; iii--)
                    {
                        Str CharMsg_(TextWork::CharCode(DispBuffer[iii], 0) + " ");
                        switch (DisplayStatusDiv)
                        {
                            case 2:
                                if (DispBuffer[iii] >= 33)
                                {
                                    CharMsg_.Clear();
                                    CharMsg_.Add(DispBuffer[iii]);
                                    CharMsg_.Add(' ');
                                    if (Screen::CharDouble(DispBuffer[iii]))
                                    {
                                        CharMsg_.Add(' ');
                                    }
                                }
                                break;
                            case 3:
                                if ((DispBuffer[iii] >= 33) && (DispBuffer[iii] <= 126))
                                {
                                    CharMsg_.Clear();
                                    CharMsg_.Add(DispBuffer[iii]);
                                    CharMsg_.Add(' ');
                                    if (Screen::CharDouble(DispBuffer[iii]))
                                    {
                                        CharMsg_.Add(' ');
                                    }
                                }
                                break;
                        }
                        if ((CharCounter.Count + CharMsg.Count + CharMsg_.Count - 1) < ScreenW)
                        {
                            CharMsg.InsertRange(CharMsg_);
                        }
                    }
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
        for (int i = InfoPosV; i < AnsiSauce_.Info.Count; i++)
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
            Screen_WriteText(T);
            Screen_WriteLine();
        }
        Screen_Refresh();
        DisplayInfoRepaint = false;
    }
}
