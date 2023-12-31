#include "coreansi.h"

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
    AnsiState_.AnsiScrollCommand = Command;
    AnsiState_.AnsiScrollParam1 = Param1;
    AnsiState_.AnsiScrollParam2 = Param2;
    AnsiState_.AnsiScrollParam3 = Param3;
    AnsiState_.AnsiScrollParam4 = Param4;
    AnsiState_.AnsiScrollRev = (Lines < 0);
    AnsiState_.AnsiScrollLinesI = (Lines >= 0) ? Lines : (0 - Lines);

    if (AnsiState_.__AnsiSmoothScroll && (ANSIScrollSmooth > 0))
    {
        AnsiState_.AnsiScrollCounter = ANSIScrollChars + 1;
        AnsiState_.ProcessBackgroundChars = true;

        if (AnsiScrollPosition.Count == 0)
        {
            int ScrollTime = (AnsiState_.AnsiScrollCounter - 1);

            switch (ANSIScrollSmooth)
            {
                case 1:
                    AnsiScrollPosition.Add(ScrollTime / 2);
                    AnsiScrollOffset.Add(8);
                    break;
                case 2:
                    AnsiScrollPosition.Add(ScrollTime - (ScrollTime / 4));
                    AnsiScrollPosition.Add((ScrollTime / 4));
                    AnsiScrollOffset.Add(4);
                    AnsiScrollOffset.Add(8);
                    break;
                case 3:
                    AnsiScrollPosition.Add(ScrollTime - (ScrollTime / 8));
                    AnsiScrollPosition.Add(ScrollTime - ((3 * ScrollTime) / 8));
                    AnsiScrollPosition.Add(((3 * ScrollTime) / 8));
                    AnsiScrollPosition.Add((ScrollTime / 8));
                    AnsiScrollOffset.Add(2);
                    AnsiScrollOffset.Add(4);
                    AnsiScrollOffset.Add(6);
                    AnsiScrollOffset.Add(8);
                    break;
                case 4:
                    AnsiScrollPosition.Add(ScrollTime - (ScrollTime / 16));
                    AnsiScrollPosition.Add(ScrollTime - ((3 * ScrollTime) / 16));
                    AnsiScrollPosition.Add(ScrollTime - ((5 * ScrollTime) / 16));
                    AnsiScrollPosition.Add(ScrollTime - ((7 * ScrollTime) / 16));
                    AnsiScrollPosition.Add(((7 * ScrollTime) / 16));
                    AnsiScrollPosition.Add(((5 * ScrollTime) / 16));
                    AnsiScrollPosition.Add(((3 * ScrollTime) / 16));
                    AnsiScrollPosition.Add((ScrollTime / 16));
                    AnsiScrollOffset.Add(1);
                    AnsiScrollOffset.Add(2);
                    AnsiScrollOffset.Add(3);
                    AnsiScrollOffset.Add(4);
                    AnsiScrollOffset.Add(5);
                    AnsiScrollOffset.Add(6);
                    AnsiScrollOffset.Add(7);
                    AnsiScrollOffset.Add(8);
                    break;
            }
        }

        AnsiScrollProcess();
    }
    else
    {
        AnsiState_.AnsiScrollCounter = 0;
        AnsiScrollLines(AnsiState_.AnsiScrollRev ? 0 - AnsiState_.AnsiScrollLinesI : AnsiState_.AnsiScrollLinesI);
        AnsiScrollFinish(false);
    }
}

bool CoreAnsi::AnsiScrollFinish(bool ScrollDisp)
{
    AnsiScrollSetOffset(0);
    AnsiState_.PrintCharScroll++;
    switch (AnsiState_.AnsiScrollCommand)
    {
        case AnsiState::AnsiScrollCommandDef::None:
            {

            }
            break;
        case AnsiState::AnsiScrollCommandDef::Char:
            {
                int CharDbl = Screen::CharDouble(AnsiState_.AnsiScrollParam1);
                AnsiCharFI(AnsiState_.__AnsiX, AnsiState_.__AnsiY, AnsiState_.AnsiScrollParam1, AnsiState_.AnsiScrollParam2, AnsiState_.AnsiScrollParam3, AnsiState_.AnsiScrollParam4);
                AnsiState_.__AnsiX++;
                if (CharDbl != 0)
                {
                    AnsiCharFI(AnsiState_.__AnsiX, AnsiState_.__AnsiY, CharDbl, AnsiState_.AnsiScrollParam2, AnsiState_.AnsiScrollParam3, AnsiState_.AnsiScrollParam4);
                    AnsiState_.__AnsiX++;
                }
            }
            ScrollDisp = true;
            break;
        case AnsiState::AnsiScrollCommandDef::FirstLast:
            AnsiState_.__AnsiScrollFirst = AnsiState_.AnsiScrollParam1;
            AnsiState_.__AnsiScrollLast = AnsiState_.AnsiScrollParam2;
            break;
        case AnsiState::AnsiScrollCommandDef::Tab:
            AnsiDoTab(AnsiState_.AnsiScrollParam1);
            break;
    }
    AnsiState_.AnsiScrollCommand = AnsiState::AnsiScrollCommandDef::None;
    SeekStateSave(true);
    return ScrollDisp;
}

bool CoreAnsi::AnsiScrollProcess()
{
    bool ScrollDisp = false;
    bool ScrollMove = false;

    for (int i = 0; i < AnsiScrollPosition.Count; i++)
    {
        if (AnsiState_.AnsiScrollCounter == AnsiScrollPosition[i])
        {
            if (AnsiScrollOffset[i] == 8)
            {
                ScrollMove = true;
                AnsiScrollSetOffset(0);
            }
            else
            {
                if (AnsiState_.AnsiScrollRev)
                {
                    AnsiScrollSetOffset(0 - AnsiScrollOffset[i]);
                }
                else
                {
                    AnsiScrollSetOffset(AnsiScrollOffset[i]);
                }
            }
            ScrollDisp = true;
        }
    }

    //if (AnsiScrollCounter == ((ANSIScrollChars2) + 1))
    if (ScrollMove)
    {
        AnsiScrollLines(AnsiState_.AnsiScrollRev ? -1 : 1);
        ScrollDisp = true;
    }
    if (AnsiState_.AnsiScrollCounter == 1)
    {
        AnsiState_.AnsiScrollLinesI--;
        if (AnsiState_.AnsiScrollLinesI > 0)
        {
            AnsiState_.AnsiScrollCounter = ANSIScrollChars + 1;
        }
        else
        {
            ScrollDisp = AnsiScrollFinish(ScrollDisp);
        }
    }
    AnsiState_.AnsiScrollCounter--;
    return ScrollDisp;
}

void CoreAnsi::AnsiScrollSetOffset(int Offset)
{
    AnsiState_.ScrollLastOffset = Offset;
    if (AnsiScreenWork)
    {
        if (Offset < 0)
        {
            Screen::ScreenLineOffset(AnsiState_.__AnsiScrollLast + ScreenOffset, Offset, 0, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
            for (int Y = AnsiState_.__AnsiScrollLast - 1; Y > AnsiState_.__AnsiScrollFirst; Y--)
            {
                Screen::ScreenLineOffset(Y + ScreenOffset, Offset, 0, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
            }
            Screen::ScreenLineOffset(AnsiState_.__AnsiScrollFirst + ScreenOffset, Offset, 1, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
        }
        else
        {
            Screen::ScreenLineOffset(AnsiState_.__AnsiScrollFirst + ScreenOffset, Offset, 0, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
            for (int Y = AnsiState_.__AnsiScrollFirst + 1; Y < AnsiState_.__AnsiScrollLast; Y++)
            {
                Screen::ScreenLineOffset(Y + ScreenOffset, Offset, 0, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
            }
            Screen::ScreenLineOffset(AnsiState_.__AnsiScrollLast + ScreenOffset, Offset, 1, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
        }
    }
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
