#include "coreansi.h"

void CoreAnsi::AnsiProcessReset(bool __AnsiUseEOF_, bool AnsiScreenWork_, int SeekMode_)
{
    if (SeekMode_ <= 1)
    {
        SeekStateSaveLast = -1;
        SeekState.Clear();
    }
    SeekMode = SeekMode_;
    AnsiRingBell = AnsiScreenWork_;
    AnsiScreenWork = AnsiScreenWork_;
    AnsiBuffer.Clear();
    AnsiState_.Zero(__AnsiUseEOF_);
    AnsiTerminalReset();
    SeekStateSaveRequest = true;
    SeekStateSave(true);
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

    __AnsiResponse.Clear();
    if (AnsiScreenWork)
    {
        Screen::MouseActive(false);
        AnsiRepaint(false);
    }
}

bool CoreAnsi::AnsiTerminalResize(int NewW, int NewH)
{
    if (NewW <= 0)
    {
        NewW = 80;
    }
    if (NewH <= 0)
    {
        if (ANSIDOS_)
        {
            NewH = 25;
        }
        else
        {
            NewH = 24;
        }
    }

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

    /*!!!!!!!!!!!!if (Core_.CursorX >= NewW)
    {
        Core_.CursorX = NewW - 1;
    }
    if (Core_.CursorY >= NewH)
    {
        Core_.CursorY = NewH - 1;
    }*/

    if (SeekState.Count > 1)
    {
        //SeekState.RemoveRange(1, SeekState.Count - 1);
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


void CoreAnsi::SeekStateSave(bool Instant)
{
    if ((SeekMode > 0) && ((SeekStateSaveLast < AnsiState_.__AnsiProcessStep)))
    {
        if (Instant)
        {
            if (SeekStateSaveRequest || ((AnsiState_.__AnsiProcessStep & SeekPeriod) == 0))
            {
                {
                    SeekState.Add(AnsiState_.Clone());
                    SeekStateSaveLast = AnsiState_.__AnsiProcessStep + SeekPeriod;
                }
                SeekStateSaveRequest = false;
            }
        }
        else
        {
            if (((AnsiState_.__AnsiProcessStep & SeekPeriod) == 0))
            {
                SeekStateSaveRequest = true;
            }
        }
    }
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
        AnsiState::Copy(SeekState[SeekIdx], AnsiState_);
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
            ScreenBell();
        }
    }

    return NeedRepaint;
}


int CoreAnsi::AnsiProcess(int ProcessCount)
{
    int Processed = 0;
    if (ProcessCount < 0)
    {
        ProcessCount = 2147483647;
    }
    if ((ProcessCount == 0) || (AnsiState_.__AnsiBeyondEOF))
    {
        return 0;
    }
    bool StdProc = true;
    bool ProcAdditionalChars = false;
    bool SeekStateSaveInstant = false;
    while (ProcessCount > 0)
    {
        AnsiState_.__AnsiProcessStep++;

        if (AnsiState_.__AnsiBeyondEOF)
        {
            break;
        }
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
            ProcAdditionalChars = true;
        }
        if (AnsiState_.__AnsiProcessStep <= AnsiState_.__AnsiProcessDelay)
        {
            Processed++;
            StdProc = false;
        }
        if (AnsiState_.AnsiScrollCounter > 0)
        {
            Processed++;
            StdProc = false;
            if (AnsiScrollProcess())
            {
            }
            ProcAdditionalChars = true;
        }
        SeekStateSaveInstant = StdProc;
        if (StdProc)
        {
            if (AnsiState_.AnsiBufferI >= AnsiBuffer.Count)
            {
                AnsiState_.__AnsiAdditionalChars = 0;
                return Processed;
            }
            Processed++;

            int CharToPrint = AnsiBuffer[AnsiState_.AnsiBufferI];
            AnsiState_.AnsiBufferI++;
            if (AnsiState_.__AnsiCommand)
            {
                if (CharToPrint < 32)
                {
                    if (CharToPrint == 27)
                    {
                        AnsiState_.__AnsiCmd.Clear();
                    }
                    else
                    {
                        if (AnsiCharNotCmd(CharToPrint))
                        {
                            AnsiCharPrint(CharToPrint);
                        }
                    }
                }
                else
                {
                    AnsiState_.__AnsiCmd.Add(CharToPrint);
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
                            SeekStateSaveLast = AnsiState_.__AnsiProcessStep - 1;
                            SeekStateSaveRequest = true;
                        }
                    }
                }
            }
            else
            {
                if (CharToPrint == 27)
                {
                    AnsiState_.__AnsiCmd.Clear();
                    AnsiState_.__AnsiCommand = true;
                }
                else
                {
                    if (AnsiCharNotCmd(CharToPrint))
                    {
                        if (AnsiState_.__AnsiDCS_)
                        {
                            AnsiState_.__AnsiDCS = AnsiState_.__AnsiDCS + Str::IntToStr(CharToPrint);
                        }
                        else
                        {
                            AnsiCharPrint(CharToPrint);
                        }
                    }
                }
            }
            if (AnsiState_.__AnsiAdditionalChars == 0)
            {
                ProcessCount--;
                AnsiState_.__AnsiCounter++;
            }
            else
            {
                AnsiState_.__AnsiAdditionalChars--;
                Processed--;
                AnsiState_.__AnsiProcessStep--;
                SeekStateSaveInstant = false;
            }
        }
        else
        {
            if (ProcAdditionalChars && (AnsiState_.__AnsiAdditionalChars < ANSIScrollBuffer))
            {
                AnsiState_.__AnsiAdditionalChars++;
            }
            ProcessCount--;
            AnsiState_.__AnsiCounter++;
        }
        SeekStateSave(SeekStateSaveInstant);
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
