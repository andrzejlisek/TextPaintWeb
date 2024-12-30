#include "coreansi.h"

void CoreAnsi::AnsiProcessReset(bool AnsiScreenWork_, int SeekMode_, int AnsiOptions, bool UseScrollBufer)
{
    __AnsiLineOccupyUseScrollBuffer = UseScrollBufer;
    if (SeekMode_ <= 1)
    {
        SeekStateSaveLast = -1;
        SeekStateSaveNext = -1;
        SeekState.Clear();
        SeekLineList.Clear();
    }
    UseAnsiCommands = ((AnsiOptions & 1) > 0);
    UseCustomPaletteFont = ((AnsiOptions & 2) > 0);
    SeekMode = SeekMode_;
    AnsiRingBell = AnsiScreenWork_;
    AnsiScreenWork = AnsiScreenWork_;
    AnsiBuffer.Clear();
    AnsiState_.Zero();
    AnsiTerminalReset();
    if (SeekStateSaveLast < 0)
    {
        SeekStateSave(true);
    }
}

void CoreAnsi::AnsiProcessSupply(Str TextFileLine)
{
    if (TextFileLine.Count > 0)
    {
        AnsiBuffer.AddRange(TextFileLine);
    }
}

void CoreAnsi::AnsiTerminalReset()
{
    AnsiState_.Reset(AnsiMaxX, AnsiMaxY, -1, -1, ANSIDOS);
    AnsiScrollClear();

    __AnsiResponse.Clear();
    if (AnsiScreenWork)
    {
        AnsiRepaint(false);
    }
}

bool CoreAnsi::AnsiTerminalResize(int NewW, int NewH, int ScreenStatusBar_)
{
    ScreenOffset = 0;
    ScreenStatusBar = ScreenStatusBar_;
    if (ScreenStatusBar_ > 0)
    {
        NewH--;
        if (ScreenStatusBar_ == 1)
        {
            ScreenOffset = 1;
        }
    }

    NewW = Screen::DefaultW(NewW, ANSIDOS);
    NewH = Screen::DefaultH(NewH, ANSIDOS);

    if ((AnsiMaxX == NewW) && (AnsiMaxY == NewH))
    {
        return false;
    }

    if ((AnsiState_.__AnsiScrollFirst == 0) && (AnsiState_.__AnsiScrollLast == (AnsiMaxY - 1)))
    {
        AnsiState_.__AnsiScrollLast = (NewH - 1);
    }
    if ((AnsiState_.__AnsiMarginLeft == 0) && (AnsiState_.__AnsiMarginRight == (AnsiMaxX - 1)))
    {
        AnsiState_.__AnsiMarginRight = (NewW - 1);
    }

    if (Screen::CurrentX >= NewW)
    {
        Screen::CurrentX = NewW - 1;
    }
    if (Screen::CurrentY >= NewH)
    {
        Screen::CurrentY = NewH - 1;
    }

    AnsiMaxX = NewW;
    AnsiMaxY = NewH;
    AnsiState_.TerminalW = NewW;
    AnsiState_.TerminalH = NewH;
    if (AnsiScreenWork)
    {
        AnsiRepaint(false);
    }
    return true;
}


void CoreAnsi::SeekStateSave(bool StdProc)
{
    if ((SeekMode > 0) && (SeekStateSaveNext <= AnsiState_.__AnsiProcessStep))
    {
        if (((StdProc && AnsiState_.AnsiScrollZeroOffset)) || AnsiState_.AnsiScrollSeekSave)
        {
            AnsiState_.AnsiScrollSeekSave = false;
            SeekState.Add(AnsiState_.Clone(__AnsiLineOccupyUseScrollBuffer));
            SeekStateSaveLast = AnsiState_.__AnsiProcessStep;
            SeekStateSaveNext = SeekStateSaveLast + SeekPeriod;
        }
    }
    AnsiState_.AnsiScrollSeekSave = false;
}

void CoreAnsi::SeekStateSaveForbid()
{
    SeekStateSaveNext = SeekStateSaveLast + SeekPeriod;
}

void CoreAnsi::SeekStateSaveForce()
{
    SeekStateSaveNext = SeekStateSaveLast + 1;
}

bool CoreAnsi::AnsiSeek(int StepCount)
{
    bool ForceSeek = false;

    bool NeedRepaint = false;
    bool AnsiRingBell_ = AnsiRingBell;
    AnsiRingBell = false;
    int RingBellCount = AnsiState_.AnsiRingBellCount;
    long NewProcessStep = AnsiState_.__AnsiProcessStep + StepCount;
    if (ForceSeek || (StepCount < 0) || (StepCount >= SeekPeriod0))
    {
        if (!ForceSeek)
        {
            NeedRepaint = true;
        }
        int SeekIdx = SeekState.Count - 1;
        while (SeekState[SeekIdx].__AnsiProcessStep > NewProcessStep)
        {
            SeekIdx--;
        }
        bool MustResize = false;
        if ((SeekState[SeekIdx].TerminalW != AnsiState_.TerminalW) || (SeekState[SeekIdx].TerminalH != AnsiState_.TerminalH))
        {
            MustResize = true;
        }
        AnsiState::Copy(SeekState[SeekIdx], AnsiState_, __AnsiLineOccupyUseScrollBuffer);
        if (MustResize)
        {
            AnsiResize(AnsiState_.TerminalW, AnsiState_.TerminalH);
        }
        AnsiRepaint(false);
    }
    AnsiProcess((int)(NewProcessStep - AnsiState_.__AnsiProcessStep));

    AnsiRingBell = AnsiRingBell_;
    if (AnsiRingBell)
    {
        if (RingBellCount != AnsiState_.AnsiRingBellCount)
        {
            Screen::Bell();
        }
    }

    return NeedRepaint;
}

void CoreAnsi::AnsiSeekInterval(int Period)
{
    SeekPeriod = Period;
    SeekPeriod0 = Period + Period;
}

void CoreAnsi::AnsiSeekLine()
{
    if (SeekMode == 1)
    {
        SeekLineList.Add(AnsiState_.__AnsiProcessStep);
    }
}

int CoreAnsi::AnsiProcess(int ProcessCount)
{
    int Processed = 0;
    if (ProcessCount < 0)
    {
        ProcessCount = 2147483647;
    }
    bool StdProc = true;
    while (ProcessCount > 0)
    {
        AnsiState_.__AnsiProcessStep++;

        StdProc = true;
        if (AnsiCharPrintRepeater > 0)
        {
            Processed++;
            StdProc = false;
            AnsiCharPrint(AnsiCharPrintLast);
            AnsiCharPrintRepeater--;
            if (AnsiCharPrintRepeater == 0)
            {
                AnsiCharPrintLast = -1;
            }
        }
        if (AnsiState_.__AnsiProcessStep <= AnsiState_.__AnsiProcessDelay)
        {
            Processed++;
            StdProc = false;
        }
        if (StdProc)
        {
            if (AnsiScrollProcess() != 0)
            {
                Processed++;
                StdProc = false;
            }
        }
        if (AnsiState_.AnsiBufferI >= AnsiBuffer.Count)
        {
            if (StdProc)
            {
                StdProc = false;
                if (!AnsiState_.AnsiScrollZeroOffset)
                {
                    Processed++;
                }
                else
                {
                    ProcessCount = 0;
                }
            }
        }
        if (StdProc)
        {
            Processed++;

            int CharToPrint = AnsiBuffer[AnsiState_.AnsiBufferI];
            AnsiState_.AnsiBufferI++;

            bool CharNoCommandStarting = true;

            if (UseAnsiCommands)
            {
                if (CharToPrint == 27)
                {
                    AnsiState_.__AnsiCmd.Clear();
                    AnsiState_.__AnsiCommand = true;
                    CharNoCommandStarting = false;
                }
                else
                {
                    if (ANSI8bit && (CharToPrint >= 0x80) && (CharToPrint <= 0x9F))
                    {
                        AnsiState_.__AnsiCmd.Clear();
                        AnsiState_.__AnsiCmd.Add(CharToPrint - 0x40);
                        AnsiState_.__AnsiCommand = true;
                        CharNoCommandStarting = false;
                    }
                }
            }

            if (AnsiState_.__AnsiCommand)
            {
                if (CharNoCommandStarting)
                {
                    if (CharToPrint >= 32)
                    {
                        AnsiState_.__AnsiCmd.Add(CharToPrint);
                    }
                    else
                    {
                        AnsiCharPrint(CharToPrint);
                    }
                }

                if (AnsiState_.__AnsiCmd.Count > 0)
                {
                    if (AnsiState_.__AnsiVT52)
                    {
                        AnsiProcess_VT52();
                    }
                    else
                    {
                        int ResoX = AnsiMaxX;
                        int ResoY = AnsiMaxY;
                        AnsiProcess_Fixed(CharToPrint);

                        if (AnsiState_.__AnsiCommand && (AnsiState_.__AnsiCmd[0] != ']') && (AnsiState_.__AnsiCmd.Count >= 2) && (CommandEndChar.Contains(CharToPrint)))
                        {
                            AnsiState_.__AnsiCommand = false;
                            std::string AnsiCmd_ = AnsiState_.__AnsiCmd.ToString();
                            if (TextWork::StringStartsWith(AnsiCmd_, "[?"))
                            {
                                AnsiProcess_CSI_Question(AnsiCmd_);
                            }
                            if (TextWork::StringStartsWith(AnsiCmd_, "[") && (!TextWork::StringStartsWith(AnsiCmd_, "[?")))
                            {
                                AnsiProcess_CSI_Fixed(AnsiCmd_);
                            }
                            std::string AnsiCmd_0 = "";
                            for (int i0 = 0; i0 < AnsiCmd_.size(); i0++)
                            {
                                if (AnsiCmd_[i0] >= 32)
                                {
                                    AnsiCmd_0 += TextWork::CharToStr(AnsiCmd_[i0]);
                                }
                                else
                                {
                                    AnsiCmd_0 = AnsiCmd_0 + "{" + Hex::IntToHex(AnsiCmd_[i0]) + "}";
                                }
                            }
                        }
                        if ((ResoX != AnsiMaxX) || (ResoY != AnsiMaxY))
                        {
                            SeekStateSaveForce();
                        }
                    }
                }
            }
            else
            {
                if (CharNoCommandStarting)
                {
                    if (AnsiState_.__AnsiDCS_ || AnsiState_.__AnsiOSC_)
                    {
                        if (AnsiState_.__AnsiDCS_)
                        {
                            AnsiState_.__AnsiDCS = AnsiState_.__AnsiDCS + Str::IntToStr(CharToPrint);
                        }
                        if (AnsiState_.__AnsiOSC_)
                        {
                            if (CharToPrint == 0x07)
                            {
                                AnsiState_.__AnsiCmd.Clear();
                                AnsiState_.__AnsiCmd.Add('\\');
                                AnsiProcess_Fixed(32);
                            }
                            else
                            {
                                AnsiState_.__AnsiOSC = AnsiState_.__AnsiOSC + Str::IntToStr(CharToPrint);
                            }
                        }
                    }
                    else
                    {
                        AnsiCharPrint(CharToPrint);
                    }
                }
            }
        }
        ProcessCount--;
        AnsiState_.__AnsiCounter++;
        SeekStateSave(StdProc);
    }
    if (Processed == 0)
    {
        if (!StdProc)
        {
            Processed = -1;
        }
    }
    return Processed;
}
