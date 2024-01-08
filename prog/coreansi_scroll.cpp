#include "coreansi.h"

void CoreAnsi::AnsiScrollPrepare()
{
    AnsiScrollPosition.Clear();
    AnsiScrollOffset.Clear();
    switch (ANSIScrollSmooth)
    {
        case 1:
            AnsiScrollPosition.Add(ANSIScrollChars / 2);
            AnsiScrollOffset.Add(0);
            break;
        case 2:
            AnsiScrollPosition.Add(ANSIScrollChars - (ANSIScrollChars / 4));
            AnsiScrollPosition.Add((ANSIScrollChars / 4));
            AnsiScrollOffset.Add(8);
            AnsiScrollOffset.Add(0);
            break;
        case 3:
            AnsiScrollPosition.Add(ANSIScrollChars - (ANSIScrollChars / 8));
            AnsiScrollPosition.Add(ANSIScrollChars - ((3 * ANSIScrollChars) / 8));
            AnsiScrollPosition.Add(((3 * ANSIScrollChars) / 8));
            AnsiScrollPosition.Add((ANSIScrollChars / 8));
            AnsiScrollOffset.Add(12);
            AnsiScrollOffset.Add(8);
            AnsiScrollOffset.Add(4);
            AnsiScrollOffset.Add(0);
            break;
        case 4:
            AnsiScrollPosition.Add(ANSIScrollChars - (ANSIScrollChars / 16));
            AnsiScrollPosition.Add(ANSIScrollChars - ((3 * ANSIScrollChars) / 16));
            AnsiScrollPosition.Add(ANSIScrollChars - ((5 * ANSIScrollChars) / 16));
            AnsiScrollPosition.Add(ANSIScrollChars - ((7 * ANSIScrollChars) / 16));
            AnsiScrollPosition.Add(((7 * ANSIScrollChars) / 16));
            AnsiScrollPosition.Add(((5 * ANSIScrollChars) / 16));
            AnsiScrollPosition.Add(((3 * ANSIScrollChars) / 16));
            AnsiScrollPosition.Add((ANSIScrollChars / 16));
            AnsiScrollOffset.Add(14);
            AnsiScrollOffset.Add(12);
            AnsiScrollOffset.Add(10);
            AnsiScrollOffset.Add(8);
            AnsiScrollOffset.Add(6);
            AnsiScrollOffset.Add(4);
            AnsiScrollOffset.Add(2);
            AnsiScrollOffset.Add(0);
            break;
        case 5:
            AnsiScrollPosition.Add(ANSIScrollChars - (ANSIScrollChars / 32));
            AnsiScrollPosition.Add(ANSIScrollChars - ((3 * ANSIScrollChars) / 32));
            AnsiScrollPosition.Add(ANSIScrollChars - ((5 * ANSIScrollChars) / 32));
            AnsiScrollPosition.Add(ANSIScrollChars - ((7 * ANSIScrollChars) / 32));
            AnsiScrollPosition.Add(ANSIScrollChars - ((9 * ANSIScrollChars) / 32));
            AnsiScrollPosition.Add(ANSIScrollChars - ((11 * ANSIScrollChars) / 32));
            AnsiScrollPosition.Add(ANSIScrollChars - ((13 * ANSIScrollChars) / 32));
            AnsiScrollPosition.Add(ANSIScrollChars - ((15 * ANSIScrollChars) / 32));
            AnsiScrollPosition.Add(((15 * ANSIScrollChars) / 32));
            AnsiScrollPosition.Add(((13 * ANSIScrollChars) / 32));
            AnsiScrollPosition.Add(((11 * ANSIScrollChars) / 32));
            AnsiScrollPosition.Add(((9 * ANSIScrollChars) / 32));
            AnsiScrollPosition.Add(((7 * ANSIScrollChars) / 32));
            AnsiScrollPosition.Add(((5 * ANSIScrollChars) / 32));
            AnsiScrollPosition.Add(((3 * ANSIScrollChars) / 32));
            AnsiScrollPosition.Add((ANSIScrollChars / 32));
            AnsiScrollOffset.Add(15);
            AnsiScrollOffset.Add(14);
            AnsiScrollOffset.Add(13);
            AnsiScrollOffset.Add(12);
            AnsiScrollOffset.Add(11);
            AnsiScrollOffset.Add(10);
            AnsiScrollOffset.Add(9);
            AnsiScrollOffset.Add(8);
            AnsiScrollOffset.Add(7);
            AnsiScrollOffset.Add(6);
            AnsiScrollOffset.Add(5);
            AnsiScrollOffset.Add(4);
            AnsiScrollOffset.Add(3);
            AnsiScrollOffset.Add(2);
            AnsiScrollOffset.Add(1);
            AnsiScrollOffset.Add(0);
            break;
    }
}

void CoreAnsi::AnsiScrollInit(int Lines, AnsiState::AnsiScrollCommandDef Command)
{
    AnsiScrollInit(Lines, Command, 0, 0, 0, 0);
}

void CoreAnsi::AnsiScrollInit(int Lines, AnsiState::AnsiScrollCommandDef Command, int Param1, int Param2, int Param3, int Param4)
{
    if (Lines == 0)
    {
        return;
    }

    if (AnsiState_.__AnsiSmoothScroll && (ANSIScrollSmooth > 0))
    {
        while ((Lines > 1) || (Lines < (-1)))
        {
            AnsiState_.AnsiScrollQueue_Lines.Add(Lines > 0 ? 2 : (-2));
            AnsiState_.AnsiScrollQueue_Command.Add(Command);
            AnsiState_.AnsiScrollQueue_Param1.Add(Param1);
            AnsiState_.AnsiScrollQueue_Param2.Add(Param2);
            AnsiState_.AnsiScrollQueue_Param3.Add(Param3);
            AnsiState_.AnsiScrollQueue_Param4.Add(Param4);
            AnsiState_.AnsiScrollQueue_First.Add(AnsiState_.__AnsiScrollFirst);
            AnsiState_.AnsiScrollQueue_Last.Add(AnsiState_.__AnsiScrollLast);

            if (AnsiState_.AnsiScrollQueue_Counter > 0)
            {
                AnsiState_.AnsiScrollQueue_Counter.Add(ANSIScrollChars);
                AnsiState_.AnsiScrollProcessBlock = 1;
            }
            else
            {
                AnsiState_.AnsiScrollQueue_Counter.Add(ANSIScrollChars - 1);
                AnsiScrollLines(AnsiState_.AnsiScrollQueue_Lines[0]);
            }

            if (Lines > 0)
            {
                Lines--;
            }
            else
            {
                Lines++;
            }
        }

        AnsiState_.AnsiScrollQueue_Lines.Add(Lines);
        AnsiState_.AnsiScrollQueue_Command.Add(Command);
        AnsiState_.AnsiScrollQueue_Param1.Add(Param1);
        AnsiState_.AnsiScrollQueue_Param2.Add(Param2);
        AnsiState_.AnsiScrollQueue_Param3.Add(Param3);
        AnsiState_.AnsiScrollQueue_Param4.Add(Param4);
        AnsiState_.AnsiScrollQueue_First.Add(AnsiState_.__AnsiScrollFirst);
        AnsiState_.AnsiScrollQueue_Last.Add(AnsiState_.__AnsiScrollLast);

        if (AnsiState_.AnsiScrollQueue_Counter > 0)
        {
            AnsiState_.AnsiScrollQueue_Counter.Add(ANSIScrollChars);
            AnsiState_.AnsiScrollProcessBlock = 1;
        }
        else
        {
            AnsiState_.AnsiScrollQueue_Counter.Add(ANSIScrollChars - 1);
            AnsiScrollLines(AnsiState_.AnsiScrollQueue_Lines[0]);
            if (Lines > 0)
            {
                AnsiScrollSetOffset(AnsiState_.AnsiScrollQueue_First[0], AnsiState_.AnsiScrollQueue_Last[0], -16);
            }
            else
            {
                AnsiScrollSetOffset(AnsiState_.AnsiScrollQueue_First[0], AnsiState_.AnsiScrollQueue_Last[0], 16);
            }
        }
    }
    else
    {
        AnsiScrollLines(Lines);
        AnsiScrollFinish(Command, Param1, Param2, Param3, Param4);
    }
}

void CoreAnsi::AnsiScrollFinish(AnsiState::AnsiScrollCommandDef Command, int Param1, int Param2, int Param3, int Param4)
{
    AnsiState_.PrintCharScroll++;
    switch (Command)
    {
        case AnsiState::AnsiScrollCommandDef::None:
            {

            }
            break;
        case AnsiState::AnsiScrollCommandDef::Char:
            {
                int CharDbl = Screen::CharDouble(Param1);
                AnsiCharFI(AnsiState_.__AnsiX, AnsiState_.__AnsiY, Param1, Param2, Param3, Param4);
                AnsiState_.__AnsiX++;
                if (CharDbl != 0)
                {
                    AnsiCharFI(AnsiState_.__AnsiX, AnsiState_.__AnsiY, CharDbl, Param2, Param3, Param4);
                    AnsiState_.__AnsiX++;
                }
            }
            break;
        case AnsiState::AnsiScrollCommandDef::FirstLast:
            AnsiState_.__AnsiScrollFirst = Param1;
            AnsiState_.__AnsiScrollLast = Param2;
            break;
        case AnsiState::AnsiScrollCommandDef::Tab:
            AnsiDoTab(Param1);
            break;
    }
}

void CoreAnsi::AnsiScrollClear()
{
    CoreAnsi::AnsiScrollSetOffset(AnsiState_.ScrollLastOffsetFirst, AnsiState_.ScrollLastOffsetLast,  0);
    AnsiState_.AnsiScrollZeroOffset = true;
    AnsiState_.AnsiScrollQueue_Counter.Clear();
    AnsiState_.AnsiScrollQueue_Lines.Clear();
    AnsiState_.AnsiScrollQueue_Command.Clear();
    AnsiState_.AnsiScrollQueue_Param1.Clear();
    AnsiState_.AnsiScrollQueue_Param2.Clear();
    AnsiState_.AnsiScrollQueue_Param3.Clear();
    AnsiState_.AnsiScrollQueue_Param4.Clear();
    AnsiState_.AnsiScrollQueue_First.Clear();
    AnsiState_.AnsiScrollQueue_Last.Clear();
    AnsiState_.AnsiScrollProcessBlock = false;
    AnsiScrollFinished = true;
}

int CoreAnsi::AnsiScrollProcess()
{
    if (AnsiState_.AnsiScrollProcessBlock == 2)
    {
        AnsiState_.AnsiScrollProcessBlock = 0;
    }

    if (AnsiState_.AnsiScrollQueue_Counter.Count == 0)
    {
        return 0;
    }

    int QueueCounter = AnsiState_.AnsiScrollQueue_Counter[0];
    int QueueLines = AnsiState_.AnsiScrollQueue_Lines[0];
    if (AnsiState_.AnsiScrollQueue_Counter[0] == ANSIScrollChars)
    {
        if (QueueLines > 0)
        {
            AnsiScrollLines(1);
            if (QueueLines == 1)
            {
                AnsiState_.AnsiScrollProcessBlock = 2;
                AnsiScrollFinish(AnsiState_.AnsiScrollQueue_Command[0], AnsiState_.AnsiScrollQueue_Param1[0], AnsiState_.AnsiScrollQueue_Param2[0], AnsiState_.AnsiScrollQueue_Param3[0], AnsiState_.AnsiScrollQueue_Param4[0]);
            }
            AnsiScrollSetOffset(AnsiState_.AnsiScrollQueue_First[0], AnsiState_.AnsiScrollQueue_Last[0], -16);
        }
        else
        {
            AnsiScrollLines(-1);
            if (QueueLines == (-1))
            {
                AnsiState_.AnsiScrollProcessBlock = 2;
                AnsiScrollFinish(AnsiState_.AnsiScrollQueue_Command[0], AnsiState_.AnsiScrollQueue_Param1[0], AnsiState_.AnsiScrollQueue_Param2[0], AnsiState_.AnsiScrollQueue_Param3[0], AnsiState_.AnsiScrollQueue_Param4[0]);
            }
            AnsiScrollSetOffset(AnsiState_.AnsiScrollQueue_First[0], AnsiState_.AnsiScrollQueue_Last[0], 16);
        }
        AnsiState_.AnsiScrollZeroOffset = false;
    }
    else
    {
        if (AnsiState_.AnsiScrollQueue_Counter[0] == 1)
        {
            AnsiScrollSetOffset(AnsiState_.AnsiScrollQueue_First[0], AnsiState_.AnsiScrollQueue_Last[0], 0);
            AnsiState_.AnsiScrollZeroOffset = true;
            AnsiState_.AnsiScrollQueue_Counter.PopFirst();
            AnsiState_.AnsiScrollQueue_Lines.PopFirst();
            AnsiState_.AnsiScrollQueue_Command.PopFirst();
            AnsiState_.AnsiScrollQueue_Param1.PopFirst();
            AnsiState_.AnsiScrollQueue_Param2.PopFirst();
            AnsiState_.AnsiScrollQueue_Param3.PopFirst();
            AnsiState_.AnsiScrollQueue_Param4.PopFirst();
            AnsiState_.AnsiScrollQueue_First.PopFirst();
            AnsiState_.AnsiScrollQueue_Last.PopFirst();
            if (AnsiState_.AnsiScrollQueue_Counter.Count == 0)
            {
                AnsiState_.AnsiScrollProcessBlock = 2;
                AnsiScrollFinished = true;
                return 2;
            }
            else
            {
                AnsiScrollFinished = false;
                return AnsiState_.AnsiScrollProcessBlock;
            }
        }
        else
        {
            for (int i = 0; i < AnsiScrollPosition.Count; i++)
            {
                if (QueueCounter == AnsiScrollPosition[i])
                {
                    if (QueueLines > 0)
                    {
                        AnsiScrollSetOffset(AnsiState_.AnsiScrollQueue_First[0], AnsiState_.AnsiScrollQueue_Last[0], 0 - AnsiScrollOffset[i]);
                    }
                    else
                    {
                        AnsiScrollSetOffset(AnsiState_.AnsiScrollQueue_First[0], AnsiState_.AnsiScrollQueue_Last[0], AnsiScrollOffset[i]);
                    }
                }
            }
        }
    }

    AnsiState_.AnsiScrollQueue_Counter[0]--;
    AnsiScrollFinished = false;
    return AnsiState_.AnsiScrollProcessBlock;
}

void CoreAnsi::AnsiScrollSetOffset(int First, int Last, int Offset)
{
    if (AnsiScreenWork & (First < Last))
    {
        if (Offset < 0)
        {
            Screen::ScreenLineOffset(Last + ScreenOffset, Offset, 0, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
            for (int Y = Last - 1; Y > First; Y--)
            {
                Screen::ScreenLineOffset(Y + ScreenOffset, Offset, 0, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
            }
            Screen::ScreenLineOffset(First + ScreenOffset, Offset, 1, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
        }
        else
        {
            Screen::ScreenLineOffset(First + ScreenOffset, Offset, 0, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
            for (int Y = First + 1; Y < Last; Y++)
            {
                Screen::ScreenLineOffset(Y + ScreenOffset, Offset, 0, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
            }
            Screen::ScreenLineOffset(Last + ScreenOffset, Offset, 1, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
        }
    }
    if (Offset == 0)
    {
        First = 2;
        Last = 1;
    }
    AnsiState_.ScrollLastOffset = Offset;
    AnsiState_.ScrollLastOffsetFirst = First;
    AnsiState_.ScrollLastOffsetLast = Last;
}

void CoreAnsi::AnsiScrollColumns(int Columns)
{
    int MaxPos0 = (AnsiProcessGetXMax(true));
    int MaxPos1 = ((AnsiProcessGetXMax(true) - 1));
    int MaxPos2 = ((AnsiProcessGetXMax(true) - 2));
    for (int i = AnsiState_.__AnsiScrollFirst; i <= AnsiState_.__AnsiScrollLast; i++)
    {
        if (AnsiState_.__AnsiLineOccupy__.CountLines() > i)
        {
            int Columns_ = Columns;
            while (Columns_ < 0)
            {
                if (AnsiGetFontSize(i) > 0)
                {
                    if (AnsiState_.__AnsiLineOccupy__.CountItems(i) >= (MaxPos0))
                    {
                        AnsiState_.__AnsiLineOccupy__.Delete(i, (AnsiProcessGetXMax(true) - 2));
                    }
                    AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                    AnsiState_.__AnsiLineOccupy__.Item_FontW = 2;
                    AnsiState_.__AnsiLineOccupy__.Item_FontH = AnsiGetFontH(i);
                    AnsiState_.__AnsiLineOccupy__.Insert(i, 0);
                    AnsiState_.__AnsiLineOccupy__.Item_FontW = 1;
                    AnsiState_.__AnsiLineOccupy__.Insert(i, 0);
                }
                else
                {
                    if (AnsiState_.__AnsiLineOccupy__.CountItems(i) >= (MaxPos0))
                    {
                        AnsiState_.__AnsiLineOccupy__.Delete(i, (AnsiProcessGetXMax(true) - 1));
                    }
                    AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                    AnsiState_.__AnsiLineOccupy__.Insert(i, 0);
                }
                Columns_++;
            }
            while (Columns_ > 0)
            {
                if (AnsiGetFontSize(i) > 0)
                {
                    if (AnsiState_.__AnsiLineOccupy__.CountItems(i) >= (MaxPos0))
                    {
                        AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                        AnsiState_.__AnsiLineOccupy__.Item_FontW = 2;
                        AnsiState_.__AnsiLineOccupy__.Item_FontH = AnsiGetFontH(i);
                        AnsiState_.__AnsiLineOccupy__.Insert(i, MaxPos2);
                        AnsiState_.__AnsiLineOccupy__.Item_FontW = 1;
                        AnsiState_.__AnsiLineOccupy__.Insert(i, MaxPos2);
                    }
                    if (AnsiState_.__AnsiLineOccupy__.CountItems(i) >= (2))
                    {
                        AnsiState_.__AnsiLineOccupy__.Delete(i, 0);
                        AnsiState_.__AnsiLineOccupy__.Delete(i, 0);
                    }
                }
                else
                {
                    if (AnsiState_.__AnsiLineOccupy__.CountItems(i) >= (MaxPos0))
                    {
                        AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                        AnsiState_.__AnsiLineOccupy__.Insert(i, MaxPos1);
                    }
                    if (AnsiState_.__AnsiLineOccupy__.CountItems(i) >= (1))
                    {
                        AnsiState_.__AnsiLineOccupy__.Delete(i, 0);
                    }
                }
                Columns_--;
            }
            AnsiRepaintLine(i);
        }
        AnsiState_.PrintCharInsDel++;
        AnsiState_.PrintCharInsDel++;
    }
}

void CoreAnsi::AnsiScrollLines(int Lines)
{
    while (Lines < 0)
    {
        int ScrollMarginL = AnsiProcessGetXMin(false);
        int ScrollMarginR = AnsiProcessGetXMax(false);
        if ((ScrollMarginL == 0) && (ScrollMarginR == AnsiMaxX))
        {
            if (__AnsiLineOccupy2_Use)
            {
                if (AnsiState_.__AnsiScrollLast == (AnsiMaxY - 1))
                {
                    while (AnsiState_.__AnsiLineOccupy__.CountLines() <= (AnsiMaxY - 1))
                    {
                        AnsiState_.__AnsiLineOccupy__.AppendLine();
                    }
                    AnsiState_.__AnsiLineOccupy2__.AppendLine(AnsiState_.__AnsiLineOccupy__, AnsiMaxY - 1);
                }
            }

            if (AnsiState_.__AnsiLineOccupy__.CountLines() > AnsiState_.__AnsiScrollLast)
            {
                AnsiState_.__AnsiLineOccupy__.DeleteLine(AnsiState_.__AnsiScrollLast);
            }
            if (AnsiState_.__AnsiLineOccupy__.CountLines() > AnsiState_.__AnsiScrollFirst)
            {
                AnsiState_.__AnsiLineOccupy__.InsertLine(AnsiState_.__AnsiScrollFirst);
            }
        }
        else
        {
            while (AnsiState_.__AnsiLineOccupy__.CountLines() <= AnsiState_.__AnsiScrollLast)
            {
                AnsiState_.__AnsiLineOccupy__.AppendLine();
            }
            for (int YY = AnsiState_.__AnsiScrollLast; YY > AnsiState_.__AnsiScrollFirst; YY--)
            {
                AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                while (AnsiState_.__AnsiLineOccupy__.CountItems(YY) <= (ScrollMarginR))
                {
                    AnsiState_.__AnsiLineOccupy__.Append(YY);
                }
                while (AnsiState_.__AnsiLineOccupy__.CountItems(YY - 1) <= (ScrollMarginR))
                {
                    AnsiState_.__AnsiLineOccupy__.Append(YY - 1);
                }
                for (int XX = ScrollMarginL; XX < ScrollMarginR; XX++)
                {
                    AnsiState_.__AnsiLineOccupy__.Get(YY - 1, XX);
                    AnsiState_.__AnsiLineOccupy__.Set(YY, XX);
                }
            }
        }
        if (AnsiScreenWork && (AnsiState_.__AnsiScrollLast > AnsiState_.__AnsiScrollFirst))
        {
            Screen::ScreenTextMove(ScrollMarginL, AnsiState_.__AnsiScrollFirst + ScreenOffset, ScrollMarginL, AnsiState_.__AnsiScrollFirst + 1 + ScreenOffset, ScrollMarginR - ScrollMarginL, AnsiState_.__AnsiScrollLast - AnsiState_.__AnsiScrollFirst);
        }

        for (int i = AnsiState_.__AnsiScrollLast; i > AnsiState_.__AnsiScrollFirst; i--)
        {
            AnsiSetFontSize(i, AnsiGetFontSize(i - 1), false);
        }
        AnsiSetFontSize(AnsiState_.__AnsiScrollFirst, 0, true);

        for (int i = ScrollMarginL; i < ScrollMarginR; i++)
        {
            AnsiChar(i, AnsiState_.__AnsiScrollFirst, 32, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, 0, 0, 0);
        }

        for (int i = (AnsiMaxY - 1); i <= AnsiState_.__AnsiScrollLast; i++)
        {
            AnsiRepaintLine(i);
        }

        Lines++;
    }
    while (Lines > 0)
    {
        int ScrollMarginL = AnsiProcessGetXMin(false);
        int ScrollMarginR = AnsiProcessGetXMax(false);
        if ((ScrollMarginL == 0) && (ScrollMarginR == AnsiMaxX))
        {
            if (__AnsiLineOccupy1_Use)
            {
                if (AnsiState_.__AnsiScrollFirst == 0)
                {
                    while (AnsiState_.__AnsiLineOccupy__.CountLines() <= 0)
                    {
                        AnsiState_.__AnsiLineOccupy__.AppendLine();
                    }
                    AnsiState_.__AnsiLineOccupy1__.AppendLine(AnsiState_.__AnsiLineOccupy__, 0);
                }
            }

            if (AnsiState_.__AnsiLineOccupy__.CountLines() > AnsiState_.__AnsiScrollFirst)
            {
                AnsiState_.__AnsiLineOccupy__.DeleteLine(AnsiState_.__AnsiScrollFirst);
            }
            if (AnsiState_.__AnsiLineOccupy__.CountLines() > AnsiState_.__AnsiScrollLast)
            {
                AnsiState_.__AnsiLineOccupy__.InsertLine(AnsiState_.__AnsiScrollLast);
            }
        }
        else
        {
            while (AnsiState_.__AnsiLineOccupy__.CountLines() <= AnsiState_.__AnsiScrollLast)
            {
                AnsiState_.__AnsiLineOccupy__.AppendLine();
            }
            AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
            for (int YY = AnsiState_.__AnsiScrollFirst; YY < AnsiState_.__AnsiScrollLast; YY++)
            {
                while (AnsiState_.__AnsiLineOccupy__.CountItems(YY) <= (ScrollMarginR))
                {
                    AnsiState_.__AnsiLineOccupy__.Append(YY);
                }
                while (AnsiState_.__AnsiLineOccupy__.CountItems(YY + 1) <= (ScrollMarginR))
                {
                    AnsiState_.__AnsiLineOccupy__.Append(YY + 1);
                }
                for (int XX = ScrollMarginL; XX < ScrollMarginR; XX++)
                {
                    AnsiState_.__AnsiLineOccupy__.Get(YY + 1, XX);
                    AnsiState_.__AnsiLineOccupy__.Set(YY, XX);
                }
            }
        }
        if (AnsiScreenWork && (AnsiState_.__AnsiScrollLast > AnsiState_.__AnsiScrollFirst))
        {
            Screen::ScreenTextMove(ScrollMarginL, AnsiState_.__AnsiScrollFirst + 1 + ScreenOffset, ScrollMarginL, AnsiState_.__AnsiScrollFirst + ScreenOffset, ScrollMarginR - ScrollMarginL, AnsiState_.__AnsiScrollLast - AnsiState_.__AnsiScrollFirst);
        }

        for (int i = AnsiState_.__AnsiScrollFirst; i < AnsiState_.__AnsiScrollLast; i++)
        {
            AnsiSetFontSize(i, AnsiGetFontSize(i + 1), false);
        }
        AnsiSetFontSize(AnsiState_.__AnsiScrollLast, 0, true);

        for (int i = ScrollMarginL; i < ScrollMarginR; i++)
        {
            AnsiChar(i, AnsiState_.__AnsiScrollLast, 32, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, 0, 0, 0);
        }

        for (int i = (AnsiMaxY - 1); i <= AnsiState_.__AnsiScrollLast; i++)
        {
            AnsiRepaintLine(i);
        }

        Lines--;
    }


    /*for (int i = (WinH - 1); i >= 0; i--)
    {
        AnsiRepaintLine(i);
    }*/
}



void CoreAnsi::AnsiDoTab(int TabTimes)
{
    int TabLimitL = AnsiProcessGetXMin(false);
    int TabLimitR = AnsiProcessGetXMax(false);

    if (TabTimes > 0)
    {
        while (TabTimes > 0)
        {
            if (AnsiState_.__AnsiWrapFlag)
            {
                AnsiState_.__AnsiWrapFlag = false;
                AnsiState_.__AnsiX = TabLimitL;
                AnsiState_.__AnsiY++;
                if ((AnsiMaxY > 0) && (AnsiState_.__AnsiY > AnsiState_.__AnsiScrollLast))
                {
                    AnsiScrollInit(AnsiState_.__AnsiY - AnsiState_.__AnsiScrollLast, AnsiState::AnsiScrollCommandDef::Tab, TabTimes - 1, 0, 0, 0);
                    AnsiState_.__AnsiY = AnsiState_.__AnsiScrollLast;
                    return;
                }
            }
            else
            {
                AnsiState_.__AnsiX++;
                if (AnsiState_.__AnsiTabs[AnsiState_.__AnsiTabs.Count - 1] > AnsiState_.__AnsiX)
                {
                    while (!AnsiState_.__AnsiTabs.Contains(AnsiState_.__AnsiX))
                    {
                        AnsiState_.__AnsiX++;
                    }
                }
                else
                {
                    while ((AnsiState_.__AnsiX % 8) > 0)
                    {
                        AnsiState_.__AnsiX++;
                    }
                }
                if (AnsiGetFontSize(AnsiState_.__AnsiY) > 0)
                {
                    if (AnsiState_.__AnsiX >= (TabLimitR / 2))
                    {
                        AnsiState_.__AnsiX = (TabLimitR / 2) - 1;
                    }
                }
                else
                {
                    if (AnsiState_.__AnsiX >= TabLimitR)
                    {
                        AnsiState_.__AnsiX = TabLimitR - 1;
                    }
                }
            }
            TabTimes--;
        }
    }
    else
    {
        AnsiState_.__AnsiWrapFlag = false;
        while (TabTimes < 0)
        {
            AnsiState_.__AnsiX--;
            if (AnsiState_.__AnsiTabs[AnsiState_.__AnsiTabs.Count - 1] > AnsiState_.__AnsiX)
            {
                while ((!AnsiState_.__AnsiTabs.Contains(AnsiState_.__AnsiX)) && (AnsiState_.__AnsiX > TabLimitL))
                {
                    AnsiState_.__AnsiX--;
                }
            }
            else
            {
                while (((AnsiState_.__AnsiX % 8) > 0) && (AnsiState_.__AnsiX > TabLimitL))
                {
                    AnsiState_.__AnsiX--;
                }
            }
            if (AnsiState_.__AnsiX < TabLimitL)
            {
                AnsiState_.__AnsiX = TabLimitL;
            }
            TabTimes++;
        }
    }
}
