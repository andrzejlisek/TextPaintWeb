#include "coreansi.h"

void CoreAnsi::SetProcessDelay(long TimeStamp)
{
    AnsiState_.__AnsiProcessDelay = TimeStamp * __AnsiProcessDelayFactor;

    long DelayDiff = AnsiState_.__AnsiProcessDelay - AnsiState_.__AnsiProcessStep;

    if (AnsiState_.__AnsiProcessDelayMin > AnsiState_.__AnsiProcessDelayMax)
    {
        AnsiState_.__AnsiProcessDelayMin = DelayDiff;
        AnsiState_.__AnsiProcessDelayMax = DelayDiff;
    }
    else
    {
        AnsiState_.__AnsiProcessDelayMin = std::min(AnsiState_.__AnsiProcessDelayMin, DelayDiff);
        AnsiState_.__AnsiProcessDelayMax = std::max(AnsiState_.__AnsiProcessDelayMax, DelayDiff);
    }
}

void CoreAnsi::AnsiProcess_VT52()
{
    switch (AnsiState_.__AnsiCmd[0])
    {
        case '<':
            AnsiState_.__AnsiWrapFlag = false;
            AnsiState_.__AnsiVT52 = false;
            AnsiState_.__AnsiCommand = false;
            break;
        case '=':
            __AnsiResponse.Add("NumpadKey_1");
            AnsiState_.__AnsiCommand = false;
            break;
        case '>':
            __AnsiResponse.Add("NumpadKey_0");
            AnsiState_.__AnsiCommand = false;
            break;
        case 'A':
            AnsiState_.__AnsiWrapFlag = false;
            AnsiState_.__AnsiY -= 1;
            if (AnsiState_.__AnsiY < 0)
            {
                AnsiState_.__AnsiY = 0;
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case 'B':
            AnsiState_.__AnsiWrapFlag = false;
            AnsiState_.__AnsiY += 1;
            AnsiState_.__AnsiCommand = false;
            break;
        case 'C':
            AnsiState_.__AnsiWrapFlag = false;
            AnsiState_.__AnsiX += 1;
            if ((AnsiMaxX > 0) && (AnsiState_.__AnsiX >= AnsiMaxX))
            {
                if (ANSIDOS_)
                {
                    AnsiState_.__AnsiY++;
                    AnsiState_.__AnsiX = AnsiState_.__AnsiX - AnsiMaxX;
                }
                else
                {
                    AnsiState_.__AnsiX = AnsiMaxX - 1;
                }
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case 'D':
            AnsiState_.__AnsiWrapFlag = false;
            if (AnsiMaxX > 0)
            {
                if (AnsiState_.__AnsiX >= AnsiMaxX)
                {
                    AnsiState_.__AnsiX = AnsiMaxX - 1;
                }
            }
            AnsiState_.__AnsiX -= 1;
            if (AnsiState_.__AnsiX < 0)
            {
                AnsiState_.__AnsiX = 0;
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case 'E':
        case 'J':
            AnsiState_.__AnsiWrapFlag = false;
            if (AnsiState_.__AnsiCmd[0] == 'E')
            {
                AnsiState_.__AnsiX = 0;
                AnsiState_.__AnsiY = 0;
            }
            for (int i_ = AnsiState_.__AnsiY + 1; i_ < AnsiMaxY; i_++)
            {
                for (int ii_ = 0; ii_ < AnsiMaxX; ii_++)
                {
                    AnsiChar(ii_, i_, 32);
                }
            }
            if (AnsiMaxY > AnsiState_.__AnsiY)
            {
                for (int ii_ = AnsiState_.__AnsiX; ii_ < AnsiMaxX; ii_++)
                {
                    AnsiChar(ii_, AnsiState_.__AnsiY, 32);
                }
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case 'F':
            AnsiState_.VT52_SemigraphDef = true;
            AnsiState_.__AnsiCommand = false;
            break;
        case 'G':
            AnsiState_.VT52_SemigraphDef = false;
            AnsiState_.__AnsiCommand = false;
            break;
        case 'H':
            AnsiState_.__AnsiWrapFlag = false;
            AnsiState_.__AnsiX = 0;
            AnsiState_.__AnsiY = 0;
            AnsiState_.__AnsiCommand = false;
            break;
        case 'I':
            AnsiState_.__AnsiWrapFlag = false;
            AnsiState_.__AnsiY -= 1;
            AnsiSeekLine();
            if (AnsiState_.__AnsiY < AnsiState_.__AnsiScrollFirst)
            {
                AnsiScrollInit(AnsiState_.__AnsiY - AnsiState_.__AnsiScrollFirst, AnsiState::AnsiScrollCommandDef::None);
                AnsiState_.__AnsiY = AnsiState_.__AnsiScrollFirst;
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case 'K':
            AnsiState_.__AnsiWrapFlag = false;
            for (int ii_ = AnsiState_.__AnsiX; ii_ < AnsiMaxX; ii_++)
            {
                AnsiChar(ii_, AnsiState_.__AnsiY, 32);
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case 'Y':
            if (AnsiState_.__AnsiCmd.Count == 3)
            {
                AnsiState_.__AnsiWrapFlag = false;
                AnsiState_.__AnsiY = AnsiState_.__AnsiCmd[1] - 32;
                AnsiState_.__AnsiX = AnsiState_.__AnsiCmd[2] - 32;
                AnsiState_.__AnsiCommand = false;
            }
            break;
        case 'Z':
            __AnsiResponse.Add("VT52:Z");
            AnsiState_.__AnsiCommand = false;
            break;
        case '[':
            {
                switch (AnsiState_.__AnsiCmd[AnsiState_.__AnsiCmd.Count - 1])
                {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    case ';':
                    case '[':
                        break;
                    case 'V':
                        {
                            std::string AnsiCmd_ = AnsiState_.__AnsiCmd.ToString();
                            XList<std::string> AnsiParams;
                            TextWork::StringSplit(AnsiCmd_.substr(1, AnsiCmd_.size() - 2), ';', AnsiParams);
                            if (AnsiCmd_.size() >= 2)
                            {
                                switch (_(AnsiParams[0].c_str()))
                                {
                                    case _("1"):
                                        {
                                            SetProcessDelay(AnsiProcess_Int0(AnsiParams[1], AnsiCmd_));
                                        }
                                        break;
                                }
                            }
                        }
                        AnsiState_.__AnsiCommand = false;
                        break;
                    default:
                        AnsiState_.__AnsiCommand = false;
                        break;
                }
            }
            break;
        case 'b':
        case 'c':
            if (AnsiState_.__AnsiCmd.Count == 2)
            {
                AnsiState_.__AnsiCommand = false;
            }
            break;
        case 'd':
            AnsiState_.__AnsiWrapFlag = false;
            for (int i_ = 0; i_ < AnsiState_.__AnsiY; i_++)
            {
                if (AnsiMaxY > AnsiState_.__AnsiY)
                {
                    for (int ii_ = 0; ii_ < AnsiMaxX; ii_++)
                    {
                        AnsiChar(ii_, i_, 32);
                    }
                }
                else
                {
                    break;
                }
            }
            for (int ii_ = 0; ii_ <= AnsiState_.__AnsiX; ii_++)
            {
                AnsiChar(ii_, AnsiState_.__AnsiY, 32);
            }
            AnsiState_.__AnsiCommand = false;
            break;
        default:
            AnsiState_.__AnsiCommand = false;
            break;
    }
}

void CoreAnsi::AnsiProcess_Fixed(int TextFileLine_i)
{
    if (AnsiState_.__AnsiCmd[0] == '[')
    {
        return;
    }
    switch (AnsiState_.__AnsiCmd[0])
    {
        case ' ':
            if (AnsiState_.__AnsiCmd.Count == 2)
            {
                switch (AnsiState_.__AnsiCmd[1])
                {
                    case 'F': // S7C1T
                        __AnsiResponse.Add("Control8bit_0");
                        break;
                    case 'G': // S8C1T
                        __AnsiResponse.Add("Control8bit_1");
                        break;
                }
                AnsiState_.__AnsiCommand = false;
            }
            break;
        case '#':
            if (AnsiState_.__AnsiCmd.Count == 2)
            {
                switch (AnsiState_.__AnsiCmd[1])
                {
                    case '3': // DECDHL
                        {
                            AnsiState_.__AnsiWrapFlag = false;
                            AnsiSetFontSize(AnsiState_.__AnsiY, 2, true);
                        }
                        break;
                    case '4': // DECDHL
                        {
                            AnsiState_.__AnsiWrapFlag = false;
                            AnsiSetFontSize(AnsiState_.__AnsiY, 3, true);
                        }
                        break;
                    case '5': // DECSWL
                        {
                            AnsiState_.__AnsiWrapFlag = false;
                            AnsiSetFontSize(AnsiState_.__AnsiY, 0, true);
                        }
                        break;
                    case '6': // DECDWL
                        {
                            AnsiState_.__AnsiWrapFlag = false;
                            AnsiSetFontSize(AnsiState_.__AnsiY, 1, true);
                        }
                        break;
                    case '8':
                        {
                            for (int YY = 0; YY < AnsiMaxY; YY++)
                            {
                                for (int XX = 0; XX < AnsiMaxX; XX++)
                                {
                                    AnsiChar(XX, YY, 'E', Screen::TextNormalBack, Screen::TextNormalFore, 0, 0, 0);
                                }
                            }
                        }
                        break;
                }
                AnsiState_.__AnsiCommand = false;
            }
            break;
        case '(':
        case ')':
        case '*':
        case '+':
            if (AnsiState_.__AnsiCmd.Count >= 2)
            {
                int CharNum = 0;
                switch (AnsiState_.__AnsiCmd[0])
                {
                    case '(':
                        CharNum = 0;
                        break;
                    case ')':
                        CharNum = 1;
                        break;
                    case '*':
                        CharNum = 2;
                        break;
                    case '+':
                        CharNum = 3;
                        break;
                }

                std::string CharMap = TextWork::CharToStr(AnsiState_.__AnsiCmd[1]);

                if ((AnsiState_.__AnsiCmd[1] == '\"') || (AnsiState_.__AnsiCmd[1] == '%') || (AnsiState_.__AnsiCmd[1] == '&'))
                {
                    if (AnsiState_.__AnsiCmd.Count == 3)
                    {
                        CharMap = CharMap + TextWork::CharToStr(AnsiState_.__AnsiCmd[2]);
                    }
                    else
                    {
                        CharMap = "";
                    }
                }

                if (CharMap != "")
                {
                    if (!ANSIDOS_)
                    {
                        AnsiState_.SetCharMap(CharNum, "1" + CharMap);
                    }
                    AnsiState_.__AnsiCommand = false;
                }
            }
            break;
        case '-':
        case '.':
        case '/':
            if (AnsiState_.__AnsiCmd.Count == 2)
            {
                int CharNum = 0;
                switch (AnsiState_.__AnsiCmd[0])
                {
                    case '-':
                        CharNum = 1;
                        break;
                    case '.':
                        CharNum = 2;
                        break;
                    case '/':
                        CharNum = 3;
                        break;
                }

                std::string CharMap = TextWork::CharToStr(AnsiState_.__AnsiCmd[1]);

                if (CharMap != "")
                {
                    if (!ANSIDOS_)
                    {
                        AnsiState_.SetCharMap(CharNum, "2" + CharMap);
                    }
                    AnsiState_.__AnsiCommand = false;
                }
            }
            break;
        case '6': // DECBI
            AnsiState_.__AnsiWrapFlag = false;
            for (int __AnsiY = AnsiState_.__AnsiScrollFirst; __AnsiY <= AnsiState_.__AnsiScrollLast; __AnsiY++)
            {
                if (AnsiState_.__AnsiLineOccupy__.CountLines() > __AnsiY)
                {
                    int FontSize = AnsiGetFontSize(__AnsiY);
                    if (FontSize > 0)
                    {
                        AnsiGetF(AnsiState_.__AnsiX, __AnsiY);
                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) >= (AnsiProcessGetXMax(false)))
                        {
                            AnsiState_.__AnsiLineOccupy__.Delete(__AnsiY, (AnsiProcessGetXMax(false) - 1) * 2);
                            AnsiState_.__AnsiLineOccupy__.Delete(__AnsiY, (AnsiProcessGetXMax(false) - 1) * 2);
                        }
                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) > (AnsiState_.__AnsiX * 2))
                        {
                            AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                            AnsiState_.__AnsiLineOccupy__.Item_FontW = 2;
                            AnsiState_.__AnsiLineOccupy__.Item_FontH = FontSize - 1;
                            AnsiState_.__AnsiLineOccupy__.Insert(__AnsiY, AnsiProcessGetXMin(false) * 2);
                            AnsiState_.__AnsiLineOccupy__.Item_FontW = 1;
                            AnsiState_.__AnsiLineOccupy__.Insert(__AnsiY, AnsiProcessGetXMin(false) * 2);
                        }
                    }
                    else
                    {
                        AnsiGetF(AnsiProcessGetXMin(false), __AnsiY);
                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) >= (AnsiProcessGetXMax(false)))
                        {
                            AnsiState_.__AnsiLineOccupy__.Delete(__AnsiY, (AnsiProcessGetXMax(false) - 1));
                        }
                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) > (AnsiProcessGetXMin(false)))
                        {
                            AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);

                            AnsiState_.__AnsiLineOccupy__.Insert(__AnsiY, AnsiProcessGetXMin(false));
                        }
                    }
                    AnsiRepaintLine(__AnsiY);
                }
                AnsiState_.PrintCharInsDel++;
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case '7': // DECSC
            AnsiState_.CursorSave();
            AnsiState_.__AnsiCommand = false;
            break;
        case '8': // DECRC
            AnsiState_.CursorLoad();
            AnsiState_.__AnsiCommand = false;
            break;
        case '9': // DECFI
            AnsiState_.__AnsiWrapFlag = false;
            for (int __AnsiY = AnsiState_.__AnsiScrollFirst; __AnsiY <= AnsiState_.__AnsiScrollLast; __AnsiY++)
            {
                if (AnsiState_.__AnsiLineOccupy__.CountLines() > __AnsiY)
                {
                    if (AnsiGetFontSize(__AnsiY) > 0)
                    {
                        AnsiGetF((AnsiProcessGetXMax(false) / 2) - 1, __AnsiY);
                        int TempC = AnsiGetLast.Item_Char;
                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) >= ((AnsiProcessGetXMax(false) / 2)))
                        {
                            AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                            AnsiState_.__AnsiLineOccupy__.Insert(__AnsiY, ((AnsiProcessGetXMax(false) / 2)));
                        }
                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) > (AnsiProcessGetXMin(false)))
                        {
                            AnsiState_.__AnsiLineOccupy__.Delete(__AnsiY, AnsiProcessGetXMin(false));
                            AnsiState_.__AnsiLineOccupy__.Delete(__AnsiY, AnsiProcessGetXMin(false));
                        }
                        if (TempC > 0)
                        {
                            AnsiCharF((AnsiProcessGetXMax(false) / 2) - 1, __AnsiY, 32);
                        }
                    }
                    else
                    {
                        AnsiGetF(AnsiProcessGetXMax(false) - 1, __AnsiY);
                        int TempC = AnsiGetLast.Item_Char;
                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) >= (AnsiProcessGetXMax(false)))
                        {
                            AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                            AnsiState_.__AnsiLineOccupy__.Insert(__AnsiY, AnsiProcessGetXMax(false));
                        }
                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) > (AnsiProcessGetXMin(false)))
                        {
                            AnsiState_.__AnsiLineOccupy__.Delete(__AnsiY, AnsiProcessGetXMin(false));
                        }
                        if (TempC > 0)
                        {
                            AnsiCharF(AnsiProcessGetXMax(false) - 1, __AnsiY, 32);
                        }
                    }
                    AnsiRepaintLine(__AnsiY);
                }
                AnsiState_.PrintCharInsDel++;
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case '=': // DECKPAM
            __AnsiResponse.Add("NumpadKey_1");
            AnsiState_.__AnsiCommand = false;
            break;
        case '>': // DECKPNM
            __AnsiResponse.Add("NumpadKey_0");
            AnsiState_.__AnsiCommand = false;
            break;
        case 'D': // IND
            if (ANSIDOS_)
            {

            }
            else
            {
                AnsiState_.__AnsiWrapFlag = false;
                AnsiState_.__AnsiY += 1;
                AnsiSeekLine();
                if (AnsiState_.__AnsiY > AnsiState_.__AnsiScrollLast)
                {
                    AnsiScrollInit(AnsiState_.__AnsiY - AnsiState_.__AnsiScrollLast, AnsiState::AnsiScrollCommandDef::None);
                    AnsiState_.__AnsiY = AnsiState_.__AnsiScrollLast;
                }
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case 'E': // NEL
            AnsiState_.__AnsiWrapFlag = false;
            AnsiState_.__AnsiY += 1;
            AnsiState_.__AnsiX = AnsiProcessGetXMin(false);
            AnsiSeekLine();
            if (AnsiState_.__AnsiY >= AnsiMaxY)
            {
                AnsiScrollInit(1, AnsiState::AnsiScrollCommandDef::None);
                AnsiState_.__AnsiY--;
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case 'H': // HTS
            if (!AnsiState_.__AnsiTabs.Contains(AnsiState_.__AnsiX))
            {
                AnsiState_.__AnsiTabs.Add(AnsiState_.__AnsiX);
                AnsiState_.__AnsiTabs.Sort();
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case 'M': // RI
            if (ANSIDOS_)
            {
                AnsiState_.__AnsiMusic = true;
            }
            else
            {
                AnsiState_.__AnsiWrapFlag = false;
                AnsiState_.__AnsiY -= 1;
                AnsiSeekLine();
                if (AnsiState_.__AnsiY < AnsiState_.__AnsiScrollFirst)
                {
                    AnsiScrollInit(AnsiState_.__AnsiY - AnsiState_.__AnsiScrollFirst, AnsiState::AnsiScrollCommandDef::None);
                    AnsiState_.__AnsiY = AnsiState_.__AnsiScrollFirst;
                }
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case 'N': // SS2
            if (!ANSIDOS_)
            {
                AnsiState_.CharMapNumGL = AnsiState_.CharMapNumGL % 10;
                AnsiState_.CharMapNumGL += 20;
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case 'O': // SS3
            if (!ANSIDOS_)
            {
                AnsiState_.CharMapNumGL = AnsiState_.CharMapNumGL % 10;
                AnsiState_.CharMapNumGL += 30;
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case 'P': // DCS
            AnsiState_.__AnsiDCS = "";
            AnsiState_.__AnsiDCS_ = true;
            AnsiState_.__AnsiCommand = false;
            break;
        case 'V':
            AnsiState_.CharProtection2Print = true;
            break;
        case 'W':
            AnsiState_.CharProtection2Print = false;
            break;
        case '\\': // ST
            if (AnsiState_.__AnsiDCS_)
            {
                __AnsiResponse.Add(AnsiState_.__AnsiDCS);
                AnsiState_.__AnsiDCS = "";
                AnsiState_.__AnsiDCS_ = false;
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case ']': // OSC
            if (TextFileLine_i == 0x07)
            {
                std::string AnsiCmd_ = AnsiState_.__AnsiCmd.ToString();
                int Sep = AnsiCmd_.find(';');
                if (Sep > 0)
                {
                    std::string Opt = AnsiCmd_.substr(0, Sep);
                    if (Opt == "]0")
                    {
                        __AnsiResponse.Add("WindowIcon" + AnsiCmd_.substr(Sep + 1));
                        __AnsiResponse.Add("WindowTitle" + AnsiCmd_.substr(Sep + 1));
                    }
                    if (Opt == "]1")
                    {
                        __AnsiResponse.Add("WindowIcon" + AnsiCmd_.substr(Sep + 1));
                    }
                    if (Opt == "]2")
                    {
                        __AnsiResponse.Add("WindowTitle" + AnsiCmd_.substr(Sep + 1));
                    }
                }
                AnsiState_.__AnsiCommand = false;
            }
            break;
        case 'c': // RIS
            AnsiTerminalReset();
            break;
        case 'n': // LS2
            if (!ANSIDOS_)
            {
                AnsiState_.CharMapNumGL = 2;
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case 'o': // LS3
            if (!ANSIDOS_)
            {
                AnsiState_.CharMapNumGL = 3;
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case '~': // LS1R
            if (!ANSIDOS_)
            {
                AnsiState_.CharMapNumGR = 1;
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case '}': // LS2R
            if (!ANSIDOS_)
            {
                AnsiState_.CharMapNumGR = 2;
            }
            AnsiState_.__AnsiCommand = false;
            break;
        case '|': // LS3R
            if (!ANSIDOS_)
            {
                AnsiState_.CharMapNumGR = 3;
            }
            AnsiState_.__AnsiCommand = false;
            break;
        default:
            {
                switch (AnsiState_.__AnsiCmd[0])
                {
                    case '%':
                    case '-':
                    case '.':
                    case '/':
                        if (AnsiState_.__AnsiCmd.Count == 2)
                        {
                            AnsiState_.__AnsiCommand = false;
                        }
                        break;
                    default:
                        AnsiState_.__AnsiCommand = false;
                        break;
                }
            }
            break;
    }
}

void CoreAnsi::AnsiProcess_CSI_Question(std::string AnsiCmd_)
{
    XList<std::string> AnsiParams;
    TextWork::StringSplit(AnsiCmd_.substr(2, AnsiCmd_.size() - 3), ';', AnsiParams);
    switch (AnsiCmd_[AnsiCmd_.size() - 1])
    {
        case 'h': // DECSET
            {
                for (int i = 0; i < AnsiParams.Count; i++)
                {
                    AnsiState_.DecParamSet(AnsiParams[i], 1);
                    switch (_(AnsiParams[i].c_str()))
                    {
                        case _("1"): // DECSET / DECCKM
                            __AnsiResponse.Add("CursorKey_1");
                            break;
                        case _("3"): // DECSET / DECCOLM
                            {
                                AnsiState_.__AnsiWrapFlag = false;
                                for (int i_ = 0; i_ < AnsiMaxY; i_++)
                                {
                                    for (int ii_ = AnsiMaxX; ii_ < 132; ii_++)
                                    {
                                        AnsiChar(ii_, i_, 32);
                                    }
                                }
                                AnsiResize(132, -1);
                                AnsiClearFontSize(-1);
                                if (!AnsiState_.DECCOLMPreserve)
                                {
                                    for (int i_ = 0; i_ < AnsiMaxY; i_++)
                                    {
                                        for (int ii_ = 0; ii_ < AnsiMaxX; ii_++)
                                        {
                                            AnsiChar(ii_, i_, 32);
                                        }
                                    }
                                }
                                AnsiState_.__AnsiX = 0;
                                if (AnsiMaxY > 0)
                                {
                                    AnsiState_.__AnsiY = AnsiState_.__AnsiScrollFirst;
                                }
                                else
                                {
                                    AnsiState_.__AnsiY = 0;
                                }
                            }
                            break;
                        case _("4"): // DECSET / DECSCLM
                            AnsiState_.__AnsiSmoothScroll = true;
                            break;
                        case _("5"): // DECSET / DECSCNM
                            AnsiScreenNegative(true);
                            break;
                        case _("6"): // DECSET / DECOM
                            AnsiState_.__AnsiWrapFlag = false;
                            AnsiState_.__AnsiOrigin = true;
                            AnsiState_.__AnsiX = 0;
                            AnsiState_.__AnsiY = AnsiState_.__AnsiScrollFirst;
                            break;
                        case _("7"): // DECSET / DECAWM
                            AnsiState_.__AnsiWrapFlag = false;
                            AnsiState_.__AnsiNoWrap = false;
                            break;
                        case _("9"):
                            __AnsiResponse.Add("Mouse;1;" + AnsiParams[i]);
                            break;
                        case _("25"): // DECSET / DECTCEM
                            AnsiState_.CursorHide = false;
                            break;
                        case _("42"): // DECSET / DECNRCM
                            AnsiState_.CharMapNRCS = true;
                            AnsiState_.RefreshCharMaps();
                            break;
                        case _("47"):
                            AnsiState_.ScreenAlte();
                            for (int Y = 0; Y < AnsiMaxY; Y++)
                            {
                                AnsiRepaintLine(Y);
                            }
                            break;
                        case _("66"): // DECSET / DECNKM
                            __AnsiResponse.Add("NumpadKey_1");
                            break;
                        case _("67"): // DECSET / DECBKM
                            __AnsiResponse.Add("BackspaceKey_1");
                            break;
                        case _("69"): // DECSET / DECLRMM
                            AnsiState_.__AnsiMarginLeftRight = true;
                            break;
                        case _("95"): // DECSET / DECNCSM
                            AnsiState_.DECCOLMPreserve = true;
                            break;
                        case _("1047"):
                            AnsiState_.ScreenAlte();
                            for (int Y = 0; Y < AnsiMaxY; Y++)
                            {
                                AnsiRepaintLine(Y);
                            }
                            break;
                        case _("1048"):
                            AnsiState_.CursorSave();
                            break;
                        case _("1049"):
                            AnsiState_.CursorSave();
                            AnsiState_.ScreenAlte();
                            for (int Y = 0; Y < AnsiMaxY; Y++)
                            {
                                AnsiRepaintLine(Y);
                            }
                            break;
                        case _("1000"):
                        case _("1001"):
                        case _("1002"):
                        case _("1003"):
                        case _("1004"):
                        case _("1005"):
                        case _("1006"):
                        case _("1015"):
                        case _("1016"):
                            __AnsiResponse.Add("Mouse;1;" + AnsiParams[i]);
                            break;
                    }
                }
            }
            break;
        case 'l': // DECRST
            {
                for (int i = 0; i < AnsiParams.Count; i++)
                {
                    AnsiState_.DecParamSet(AnsiParams[i], 2);
                    switch (_(AnsiParams[i].c_str()))
                    {
                        case _("1"): // DECRST / DECCKM
                            __AnsiResponse.Add("CursorKey_0");
                            break;
                        case _("2"): // DECRST / DECANM
                            AnsiState_.__AnsiVT52 = true;
                            AnsiState_.__AnsiCommand = false;
                            break;
                        case _("3"): // DECRST / DECCOLM
                            {
                                AnsiState_.__AnsiWrapFlag = false;
                                for (int i_ = 0; i_ < AnsiMaxY; i_++)
                                {
                                    for (int ii_ = AnsiMaxX; ii_ < 80; ii_++)
                                    {
                                        AnsiChar(ii_, i_, 32);
                                    }
                                }
                                AnsiResize(80, -1);
                                AnsiClearFontSize(-1);
                                if (!AnsiState_.DECCOLMPreserve)
                                {
                                    for (int i_ = 0; i_ < AnsiMaxY; i_++)
                                    {
                                        for (int ii_ = 0; ii_ < AnsiMaxX; ii_++)
                                        {
                                            AnsiChar(ii_, i_, 32);
                                        }
                                    }
                                }
                                AnsiState_.__AnsiX = 0;
                                if (AnsiMaxY > 0)
                                {
                                    AnsiState_.__AnsiY = AnsiState_.__AnsiScrollFirst;
                                }
                                else
                                {
                                    AnsiState_.__AnsiY = 0;
                                }
                            }
                            break;
                        case _("4"): // DECRST / DECSCLM
                            AnsiState_.__AnsiSmoothScroll = false;
                            break;
                        case _("5"): // DECRST / DECSCNM
                            AnsiScreenNegative(false);
                            break;
                        case _("6"): // DECRST / DECOM
                            AnsiState_.__AnsiWrapFlag = false;
                            AnsiState_.__AnsiOrigin = false;
                            AnsiState_.__AnsiX = 0;
                            AnsiState_.__AnsiY = 0;
                            break;
                        case _("7"): // DECRST / DECAWM
                            AnsiState_.__AnsiWrapFlag = false;
                            AnsiState_.__AnsiNoWrap = true;
                            break;
                        case _("9"):
                            __AnsiResponse.Add("Mouse;0;" + AnsiParams[i]);
                            break;
                        case _("25"): // DECRST / DECTCEM
                            AnsiState_.CursorHide = true;
                            break;
                        case _("42"): // DECRST / DECNRCM
                            AnsiState_.CharMapNRCS = false;
                            AnsiState_.RefreshCharMaps();
                            break;
                        case _("47"):
                            AnsiState_.ScreenMain();
                            for (int Y = 0; Y < AnsiMaxY; Y++)
                            {
                                AnsiRepaintLine(Y);
                            }
                            break;
                        case _("66"): // DECRST / DECNKM
                            __AnsiResponse.Add("NumpadKey_0");
                            break;
                        case _("67"): // DECRST / DECBKM
                            __AnsiResponse.Add("BackspaceKey_0");
                            break;
                        case _("69"): // DECRST / DECLRMM
                            AnsiState_.__AnsiMarginLeftRight = false;
                            break;
                        case _("95"): // DECRST / DECNCSM
                            AnsiState_.DECCOLMPreserve = false;
                            break;
                        case _("1047"):
                            if (AnsiState_.IsScreenAlternate)
                            {
                                for (int i_ = 0; i_ < AnsiMaxY; i_++)
                                {
                                    for (int ii_ = 0; ii_ < AnsiMaxX; ii_++)
                                    {
                                        AnsiChar(ii_, i_, 32);
                                    }
                                }
                            }
                            AnsiState_.ScreenMain();
                            for (int Y = 0; Y < AnsiMaxY; Y++)
                            {
                                AnsiRepaintLine(Y);
                            }
                            break;
                        case _("1048"):
                            AnsiState_.CursorLoad();
                            break;
                        case _("1049"):
                            if (AnsiState_.IsScreenAlternate)
                            {
                                for (int i_ = 0; i_ < AnsiMaxY; i_++)
                                {
                                    for (int ii_ = 0; ii_ < AnsiMaxX; ii_++)
                                    {
                                        AnsiChar(ii_, i_, 32);
                                    }
                                }
                            }
                            AnsiState_.ScreenMain();
                            AnsiState_.CursorLoad();
                            for (int Y = 0; Y < AnsiMaxY; Y++)
                            {
                                AnsiRepaintLine(Y);
                            }
                            break;
                        case _("1000"):
                        case _("1001"):
                        case _("1002"):
                        case _("1003"):
                        case _("1004"):
                        case _("1005"):
                        case _("1006"):
                        case _("1015"):
                        case _("1016"):
                            __AnsiResponse.Add("Mouse;0;" + AnsiParams[i]);
                            break;
                    }
                }
            }
            break;
        case 'n': // DSR
            {
                __AnsiResponse.Add(AnsiCmd_);
            }
            break;
        case 'p':
            {
                // DECRQM
                if (TextWork::StringEndsWith(AnsiCmd_, "$p"))
                {
                    __AnsiResponse.Add(AnsiCmd_);
                }
            }
            break;
    }
    switch (_(AnsiCmd_.c_str()))
    {
        case _("[?J"): // DECSED
        case _("[?0J"): // DECSED
            AnsiState_.__AnsiWrapFlag = false;
            AnsiClearFontSize(AnsiState_.__AnsiY + 1);
            for (int i_ = AnsiState_.__AnsiY + 1; i_ < AnsiMaxY; i_++)
            {
                for (int ii_ = 0; ii_ < AnsiMaxX; ii_++)
                {
                    AnsiCharUnprotected1(ii_, i_, 32);
                }
            }
            if (AnsiMaxY > AnsiState_.__AnsiY)
            {
                if (AnsiGetFontSize(AnsiState_.__AnsiY) > 0)
                {
                    for (int ii_ = (AnsiState_.__AnsiX << 1); ii_ < AnsiMaxX; ii_++)
                    {
                        AnsiCharUnprotected1(ii_, AnsiState_.__AnsiY, 32);
                    }
                }
                else
                {
                    for (int ii_ = AnsiState_.__AnsiX; ii_ < AnsiMaxX; ii_++)
                    {
                        AnsiCharUnprotected1(ii_, AnsiState_.__AnsiY, 32);
                    }
                }
            }
            break;
        case _("[?1J"): // DECSED
            AnsiState_.__AnsiWrapFlag = false;
            for (int i_ = 0; i_ < AnsiState_.__AnsiY; i_++)
            {
                AnsiSetFontSize(i_, 0, true);
                if (AnsiMaxY > AnsiState_.__AnsiY)
                {
                    for (int ii_ = 0; ii_ < AnsiMaxX; ii_++)
                    {
                        AnsiCharUnprotected1(ii_, i_, 32);
                    }
                }
                else
                {
                    break;
                }
            }
            if (AnsiGetFontSize(AnsiState_.__AnsiY) > 0)
            {
                for (int ii_ = 0; ii_ <= (AnsiState_.__AnsiX << 1); ii_++)
                {
                    AnsiCharUnprotected1(ii_, AnsiState_.__AnsiY, 32);
                }
            }
            else
            {
                for (int ii_ = 0; ii_ <= AnsiState_.__AnsiX; ii_++)
                {
                    AnsiCharUnprotected1(ii_, AnsiState_.__AnsiY, 32);
                }
            }
            break;
        case _("[?2J"): // DECSED
            AnsiState_.__AnsiWrapFlag = false;
            AnsiClearFontSize(-1);
            for (int i_ = 0; i_ < AnsiMaxY; i_++)
            {
                for (int ii_ = 0; ii_ < AnsiMaxX; ii_++)
                {
                    AnsiCharUnprotected1(ii_, i_, 32);
                }
            }
            if (ANSIDOS_)
            {
                AnsiState_.__AnsiX = 0;
                AnsiState_.__AnsiY = 0;
            }
            break;
        case _("[?K"): // DECSEL
        case _("[?0K"): // DECSEL
            AnsiState_.__AnsiWrapFlag = false;
            if (AnsiMaxY > AnsiState_.__AnsiY)
            {
                if (AnsiGetFontSize(AnsiState_.__AnsiY) > 0)
                {
                    for (int ii_ = (AnsiState_.__AnsiX << 1); ii_ < AnsiMaxX; ii_++)
                    {
                        AnsiCharUnprotected1(ii_, AnsiState_.__AnsiY, 32);
                    }
                }
                else
                {
                    for (int ii_ = AnsiState_.__AnsiX; ii_ < AnsiMaxX; ii_++)
                    {
                        AnsiCharUnprotected1(ii_, AnsiState_.__AnsiY, 32);
                    }
                }
            }
            break;
        case _("[?1K"): // DECSEL
            AnsiState_.__AnsiWrapFlag = false;
            if (AnsiMaxY > AnsiState_.__AnsiY)
            {
                if (AnsiGetFontSize(AnsiState_.__AnsiY) > 0)
                {
                    for (int ii_ = 0; ii_ <= (AnsiState_.__AnsiX << 1); ii_++)
                    {
                        AnsiCharUnprotected1(ii_, AnsiState_.__AnsiY, 32);
                    }
                }
                else
                {
                    for (int ii_ = 0; ii_ <= AnsiState_.__AnsiX; ii_++)
                    {
                        AnsiCharUnprotected1(ii_, AnsiState_.__AnsiY, 32);
                    }
                }
            }
            break;
        case _("[?2K"): // DECSEL
            AnsiState_.__AnsiWrapFlag = false;
            if (AnsiMaxY > AnsiState_.__AnsiY)
            {
                for (int ii_ = 0; ii_ < AnsiMaxX; ii_++)
                {
                    AnsiCharUnprotected1(ii_, AnsiState_.__AnsiY, 32);
                }
            }
            break;
    }
}

void CoreAnsi::AnsiProcess_CSI_Fixed(std::string AnsiCmd_)
{
    switch (_(AnsiCmd_.c_str()))
    {
        case _("[J"): // ED
        case _("[0J"): // ED
            AnsiState_.__AnsiWrapFlag = false;
            AnsiClearFontSize(AnsiState_.__AnsiY + 1);
            for (int i_ = AnsiState_.__AnsiY + 1; i_ < AnsiMaxY; i_++)
            {
                for (int ii_ = 0; ii_ < AnsiMaxX; ii_++)
                {
                    AnsiCharUnprotected2(ii_, i_, 32);
                }
            }
            if (AnsiMaxY > AnsiState_.__AnsiY)
            {
                if (AnsiGetFontSize(AnsiState_.__AnsiY) > 0)
                {
                    for (int ii_ = (AnsiState_.__AnsiX << 1); ii_ < AnsiMaxX; ii_++)
                    {
                        AnsiCharUnprotected2(ii_, AnsiState_.__AnsiY, 32);
                    }
                }
                else
                {
                    for (int ii_ = AnsiState_.__AnsiX; ii_ < AnsiMaxX; ii_++)
                    {
                        AnsiCharUnprotected2(ii_, AnsiState_.__AnsiY, 32);
                    }
                }
            }
            break;
        case _("[1J"): // ED
            AnsiState_.__AnsiWrapFlag = false;
            for (int i_ = 0; i_ < AnsiState_.__AnsiY; i_++)
            {
                AnsiSetFontSize(i_, 0, true);
                if (AnsiMaxY > AnsiState_.__AnsiY)
                {
                    for (int ii_ = 0; ii_ < AnsiMaxX; ii_++)
                    {
                        AnsiCharUnprotected2(ii_, i_, 32);
                    }
                }
                else
                {
                    break;
                }
            }
            if (AnsiGetFontSize(AnsiState_.__AnsiY) > 0)
            {
                for (int ii_ = 0; ii_ <= (AnsiState_.__AnsiX << 1); ii_++)
                {
                    AnsiCharUnprotected2(ii_, AnsiState_.__AnsiY, 32);
                }
            }
            else
            {
                for (int ii_ = 0; ii_ <= AnsiState_.__AnsiX; ii_++)
                {
                    AnsiCharUnprotected2(ii_, AnsiState_.__AnsiY, 32);
                }
            }
            break;
        case _("[2J"): // ED
            AnsiState_.__AnsiWrapFlag = false;
            AnsiClearFontSize(-1);
            for (int i_ = 0; i_ < AnsiMaxY; i_++)
            {
                for (int ii_ = 0; ii_ < AnsiMaxX; ii_++)
                {
                    AnsiCharUnprotected2(ii_, i_, 32);
                }
            }
            if (ANSIDOS_)
            {
                AnsiState_.__AnsiX = 0;
                AnsiState_.__AnsiY = 0;
            }
            break;
        case _("[K"): // EL
        case _("[0K"): // EL
            AnsiState_.__AnsiWrapFlag = false;
            if (AnsiMaxY > AnsiState_.__AnsiY)
            {
                if (AnsiGetFontSize(AnsiState_.__AnsiY) > 0)
                {
                    for (int ii_ = (AnsiState_.__AnsiX << 1); ii_ < AnsiMaxX; ii_++)
                    {
                        AnsiCharUnprotected2(ii_, AnsiState_.__AnsiY, 32);
                    }
                }
                else
                {
                    for (int ii_ = AnsiState_.__AnsiX; ii_ < AnsiMaxX; ii_++)
                    {
                        AnsiCharUnprotected2(ii_, AnsiState_.__AnsiY, 32);
                    }
                }
            }
            break;
        case _("[1K"): // EL
            AnsiState_.__AnsiWrapFlag = false;
            if (AnsiMaxY > AnsiState_.__AnsiY)
            {
                if (AnsiGetFontSize(AnsiState_.__AnsiY) > 0)
                {
                    for (int ii_ = 0; ii_ <= (AnsiState_.__AnsiX << 1); ii_++)
                    {
                        AnsiCharUnprotected2(ii_, AnsiState_.__AnsiY, 32);
                    }
                }
                else
                {
                    for (int ii_ = 0; ii_ <= AnsiState_.__AnsiX; ii_++)
                    {
                        AnsiCharUnprotected2(ii_, AnsiState_.__AnsiY, 32);
                    }
                }
            }
            break;
        case _("[2K"): // EL
            AnsiState_.__AnsiWrapFlag = false;
            if (AnsiMaxY > AnsiState_.__AnsiY)
            {
                for (int ii_ = 0; ii_ < AnsiMaxX; ii_++)
                {
                    AnsiCharUnprotected2(ii_, AnsiState_.__AnsiY, 32);
                }
            }
            break;
        case _("[c"): // Primary DA
        case _("[0c"): // Primary DA
            __AnsiResponse.Add("[0c");
            break;
        case _("[>c"): // Secondary DA
        case _("[>0c"): // Secondary DA
        case _("[=c"): // Tertiary DA
        case _("[=0c"): // Tertiary DA
        case _("[5n"): // DSR
        case _("[6n"): // DSR / CPR
        case _("[0x"): // DECREQTPARM
        case _("[1x"): // DECREQTPARM
            __AnsiResponse.Add(AnsiCmd_);
            break;
        case _("[!p"): // DECSTR
            AnsiTerminalReset();
            break;
        case _("[s"): // SCOSC
            if (!AnsiState_.__AnsiMarginLeftRight)
            {
                AnsiState_.CursorSave();
            }
            else
            {
                AnsiState_.__AnsiMarginLeft = 0;
                AnsiState_.__AnsiMarginRight = AnsiMaxX;
            }
            break;
        case _("[u"): // SCORC
            AnsiState_.CursorLoad();
            break;
        case _("[$}"): // DECSASD
        case _("[0$}"): // DECSASD
            AnsiState_.StatusBar = false;
            break;
        case _("[1$}"): // DECSASD
            AnsiState_.StatusBar = true;
            break;
        default:
            AnsiProcess_CSI(AnsiCmd_);
            break;
    }
}

int CoreAnsi::AnsiProcess_Int0(std::string Param, std::string AnsiCmd_)
{
    return TextWork::StrToInt(Param, 0);
}

int CoreAnsi::AnsiProcess_Int1(std::string Param, std::string AnsiCmd_)
{
    return TextWork::StrToInt(Param, 1);
}

int CoreAnsi::AnsiProcess_Int11(std::string Param, std::string AnsiCmd_)
{
    if (Param == "0") return 1;
    return TextWork::StrToInt(Param, 1);
}


void CoreAnsi::AnsiProcess_CSI(std::string AnsiCmd_)
{
    XList<std::string> AnsiParams;
    TextWork::StringSplit(AnsiCmd_.substr(1, AnsiCmd_.size() - 2), ';', AnsiParams);
    switch (AnsiCmd_[AnsiCmd_.size() - 1])
    {
        case '@': // ICH // SL
            if (AnsiCmd_[AnsiCmd_.size() - 2] == ' ')
            {
                // SL
                AnsiScrollColumns(AnsiProcess_Int11(AnsiParams[0].substr(0, AnsiParams[0].size() - 1), AnsiCmd_));
            }
            else
            {
                // ICH
                AnsiState_.__AnsiWrapFlag = false;
                int InsCycle = AnsiProcess_Int1(AnsiParams[0], AnsiCmd_);
                if (AnsiState_.__AnsiLineOccupy__.CountLines() > AnsiState_.__AnsiY)
                {
                    int FontSize = AnsiGetFontSize(AnsiState_.__AnsiY);
                    while (InsCycle > 0)
                    {
                        if (FontSize > 0)
                        {
                            AnsiGetF(AnsiState_.__AnsiX, AnsiState_.__AnsiY);
                            if (AnsiState_.__AnsiLineOccupy__.CountItems(AnsiState_.__AnsiY) > (AnsiState_.__AnsiX * 2))
                            {
                                AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                                AnsiState_.__AnsiLineOccupy__.Item_FontW = 2;
                                AnsiState_.__AnsiLineOccupy__.Item_FontH = FontSize - 1;
                                AnsiState_.__AnsiLineOccupy__.Insert(AnsiState_.__AnsiY, AnsiState_.__AnsiX * 2);
                                AnsiState_.__AnsiLineOccupy__.Item_FontW = 1;
                                AnsiState_.__AnsiLineOccupy__.Insert(AnsiState_.__AnsiY, AnsiState_.__AnsiX * 2);
                            }
                            if (AnsiState_.__AnsiLineOccupy__.CountItems(AnsiState_.__AnsiY) > (AnsiProcessGetXMax(false)))
                            {
                                AnsiState_.__AnsiLineOccupy__.Delete(AnsiState_.__AnsiY, AnsiProcessGetXMax(false));
                                AnsiState_.__AnsiLineOccupy__.Delete(AnsiState_.__AnsiY, AnsiProcessGetXMax(false));
                            }
                        }
                        else
                        {
                            AnsiGetF(AnsiState_.__AnsiX, AnsiState_.__AnsiY);
                            if (AnsiState_.__AnsiLineOccupy__.CountItems(AnsiState_.__AnsiY) > (AnsiState_.__AnsiX))
                            {
                                AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);

                                AnsiState_.__AnsiLineOccupy__.Insert(AnsiState_.__AnsiY, AnsiState_.__AnsiX);
                            }
                            if (AnsiState_.__AnsiLineOccupy__.CountItems(AnsiState_.__AnsiY) > (AnsiProcessGetXMax(false)))
                            {
                                AnsiState_.__AnsiLineOccupy__.Delete(AnsiState_.__AnsiY, AnsiProcessGetXMax(false));
                            }
                        }
                        InsCycle--;
                    }
                    AnsiRepaintLine(AnsiState_.__AnsiY);
                }
                AnsiState_.PrintCharInsDel++;
            }
            break;
        case 'A': // CUU // SR
            AnsiState_.__AnsiWrapFlag = false;
            if (AnsiCmd_[AnsiCmd_.size() - 2] == ' ')
            {
                // SR
                AnsiScrollColumns(0 - AnsiProcess_Int11(AnsiParams[0].substr(0, AnsiParams[0].size() - 1), AnsiCmd_));
            }
            else
            {
                // CUU
                AnsiState_.__AnsiY -= std::max(AnsiProcess_Int1(AnsiParams[0], AnsiCmd_), 1);
                if (AnsiState_.__AnsiY < AnsiState_.__AnsiScrollFirst)
                {
                    AnsiState_.__AnsiY = AnsiState_.__AnsiScrollFirst;
                }
            }
            break;
        case 'B': // CUD
            AnsiState_.__AnsiWrapFlag = false;
            AnsiState_.__AnsiY += std::max(AnsiProcess_Int1(AnsiParams[0], AnsiCmd_), 1);
            if (AnsiState_.__AnsiY > AnsiState_.__AnsiScrollLast)
            {
                AnsiState_.__AnsiY = AnsiState_.__AnsiScrollLast;
            }
            break;
        case 'C': // CUF
            AnsiState_.__AnsiWrapFlag = false;
            AnsiState_.__AnsiX += std::max(AnsiProcess_Int1(AnsiParams[0], AnsiCmd_), 1);
            if ((AnsiMaxX > 0) && (AnsiState_.__AnsiX >= AnsiProcessGetXMax(false)))
            {
                if (ANSIDOS_)
                {
                    AnsiState_.__AnsiY++;
                    AnsiState_.__AnsiX = AnsiState_.__AnsiX - AnsiMaxX;
                }
                else
                {
                    AnsiState_.__AnsiX = AnsiProcessGetXMax(false) - 1;
                }
            }
            break;
        case 'D': // CUB
            AnsiState_.__AnsiWrapFlag = false;
            if (AnsiMaxX > 0)
            {
                if (AnsiState_.__AnsiX >= AnsiMaxX)
                {
                    AnsiState_.__AnsiX = AnsiMaxX - 1;
                }
            }
            AnsiState_.__AnsiX -= std::max(AnsiProcess_Int1(AnsiParams[0], AnsiCmd_), 1);
            if (AnsiState_.__AnsiX < AnsiProcessGetXMin(false))
            {
                AnsiState_.__AnsiX = AnsiProcessGetXMin(false);
            }
            break;
        case 'E': // CNL
            AnsiState_.__AnsiWrapFlag = false;
            AnsiState_.__AnsiX = AnsiProcessGetXMin(false);
            AnsiState_.__AnsiY += AnsiProcess_Int11(AnsiParams[0], AnsiCmd_);
            if (AnsiState_.__AnsiY > AnsiState_.__AnsiScrollLast)
            {
                AnsiState_.__AnsiY = AnsiState_.__AnsiScrollLast;
            }
            break;
        case 'F': // CPL
            AnsiState_.__AnsiWrapFlag = false;
            AnsiState_.__AnsiX = AnsiProcessGetXMin(false);
            AnsiState_.__AnsiY -= AnsiProcess_Int11(AnsiParams[0], AnsiCmd_);
            if (AnsiState_.__AnsiY < AnsiState_.__AnsiScrollFirst)
            {
                AnsiState_.__AnsiY = AnsiState_.__AnsiScrollFirst;
            }
            break;
        case 'G': // CHA
            AnsiState_.__AnsiWrapFlag = false;
            AnsiState_.__AnsiX = AnsiProcess_Int11(AnsiParams[0], AnsiCmd_) - 1;
            if (AnsiState_.__AnsiOrigin && AnsiState_.__AnsiMarginLeftRight)
            {
                AnsiState_.__AnsiX += AnsiState_.__AnsiMarginLeft;
            }
            break;
        case 'H': // CUP
        case 'f': // HVP
            if (!AnsiState_.StatusBar)
            {
                AnsiState_.__AnsiWrapFlag = false;
                if (AnsiParams.Count == 1)
                {
                    AnsiParams.Add("1");
                }
                AnsiState_.__AnsiY = AnsiProcess_Int1(AnsiParams[0], AnsiCmd_) - 1;
                AnsiState_.__AnsiX = AnsiProcess_Int1(AnsiParams[1], AnsiCmd_) - 1;
                if (AnsiState_.__AnsiY < 0)
                {
                    AnsiState_.__AnsiY = 0;
                }
                if (AnsiState_.__AnsiX < 0)
                {
                    AnsiState_.__AnsiX = 0;
                }
                if (AnsiState_.__AnsiOrigin)
                {
                    AnsiState_.__AnsiY += AnsiState_.__AnsiScrollFirst;
                }
                AnsiState_.__AnsiX += AnsiProcessGetXMin(true);
                if (AnsiState_.__AnsiX >= AnsiMaxX)
                {
                    AnsiState_.__AnsiX = AnsiMaxX - 1;
                }
                if (AnsiState_.__AnsiY >= AnsiMaxY)
                {
                    AnsiState_.__AnsiY = AnsiMaxY - 1;
                }
                if (AnsiState_.__AnsiOrigin)
                {
                    if (AnsiState_.__AnsiMarginLeftRight && (AnsiState_.__AnsiX >= AnsiState_.__AnsiMarginRight))
                    {
                        AnsiState_.__AnsiX = AnsiState_.__AnsiMarginRight - 1;
                    }
                    if (AnsiState_.__AnsiY > AnsiState_.__AnsiScrollLast)
                    {
                        AnsiState_.__AnsiY = AnsiState_.__AnsiScrollLast;
                    }
                }
            }
            break;
        case 'I': // CHT
            {
                AnsiDoTab(AnsiProcess_Int11(AnsiParams[0], AnsiCmd_));
            }
            break;
        case 'L': // IL
            if (ANSIDOS_)
            {

            }
            else
            {
                if ((AnsiState_.__AnsiY >= AnsiState_.__AnsiScrollFirst) && (AnsiState_.__AnsiY <= AnsiState_.__AnsiScrollLast))
                {
                    int T1 = AnsiState_.__AnsiScrollFirst;
                    int T2 = AnsiState_.__AnsiScrollLast;
                    AnsiState_.__AnsiScrollFirst = AnsiState_.__AnsiY;
                    AnsiSeekLine();
                    AnsiScrollInit(0 - AnsiProcess_Int1(AnsiParams[0], AnsiCmd_), AnsiState::AnsiScrollCommandDef::FirstLast, T1, T2, 0, 0);
                }
            }
            break;
        case 'M': // DL
            if (ANSIDOS_)
            {
                AnsiState_.__AnsiMusic = true;
            }
            else
            {
                if ((AnsiState_.__AnsiY >= AnsiState_.__AnsiScrollFirst) && (AnsiState_.__AnsiY <= AnsiState_.__AnsiScrollLast))
                {
                    int T1 = AnsiState_.__AnsiScrollFirst;
                    int T2 = AnsiState_.__AnsiScrollLast;
                    AnsiState_.__AnsiScrollFirst = AnsiState_.__AnsiY;
                    AnsiSeekLine();
                    AnsiScrollInit(AnsiProcess_Int1(AnsiParams[0], AnsiCmd_), AnsiState::AnsiScrollCommandDef::FirstLast, T1, T2, 0, 0);
                }
            }
            break;
        case 'P': // DCH
            {
                AnsiState_.__AnsiWrapFlag = false;
                int DelCycle = AnsiProcess_Int1(AnsiParams[0], AnsiCmd_);
                if (AnsiState_.__AnsiLineOccupy__.CountLines() > AnsiState_.__AnsiY)
                {
                    while (DelCycle > 0)
                    {
                        if (AnsiGetFontSize(AnsiState_.__AnsiY) > 0)
                        {
                            AnsiGetF((AnsiProcessGetXMax(false)) - 1, AnsiState_.__AnsiY);
                            int TempC = AnsiGetLast.Item_Char;
                            if (AnsiState_.__AnsiLineOccupy__.CountItems(AnsiState_.__AnsiY) >= ((AnsiProcessGetXMax(false))))
                            {
                                AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);

                                AnsiState_.__AnsiLineOccupy__.Insert(AnsiState_.__AnsiY, (AnsiProcessGetXMax(false)));
                                AnsiState_.__AnsiLineOccupy__.Insert(AnsiState_.__AnsiY, (AnsiProcessGetXMax(false)));
                            }
                            if (AnsiState_.__AnsiLineOccupy__.CountItems(AnsiState_.__AnsiY) > (AnsiState_.__AnsiX))
                            {
                                AnsiState_.__AnsiLineOccupy__.Delete(AnsiState_.__AnsiY, AnsiState_.__AnsiX);
                                AnsiState_.__AnsiLineOccupy__.Delete(AnsiState_.__AnsiY, AnsiState_.__AnsiX);
                            }
                            if (TempC > 0)
                            {
                                AnsiCharF((AnsiProcessGetXMax(false) / 2) - 1, AnsiState_.__AnsiY, 32);
                            }
                        }
                        else
                        {
                            AnsiGetF(AnsiProcessGetXMax(false) - 1, AnsiState_.__AnsiY);
                            int TempC = AnsiGetLast.Item_Char;
                            if (AnsiState_.__AnsiLineOccupy__.CountItems(AnsiState_.__AnsiY) >= ((AnsiProcessGetXMax(false))))
                            {
                                AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);

                                AnsiState_.__AnsiLineOccupy__.Insert(AnsiState_.__AnsiY, (AnsiProcessGetXMax(false)));
                            }
                            if (AnsiState_.__AnsiLineOccupy__.CountItems(AnsiState_.__AnsiY) > (AnsiState_.__AnsiX))
                            {
                                AnsiState_.__AnsiLineOccupy__.Delete(AnsiState_.__AnsiY, AnsiState_.__AnsiX);
                            }
                            if (TempC > 0)
                            {
                                AnsiCharF(AnsiProcessGetXMax(false) - 1, AnsiState_.__AnsiY, 32);
                            }
                        }
                        DelCycle--;
                    }
                    AnsiRepaintLine(AnsiState_.__AnsiY);
                }
                AnsiState_.PrintCharInsDel++;
            }
            break;
        case 'S': // SU
            if (AnsiParams.Count == 1)
            {
                AnsiSeekLine();
                AnsiScrollInit(AnsiProcess_Int11(AnsiParams[0], AnsiCmd_), AnsiState::AnsiScrollCommandDef::None);
            }
            break;
        case 'T': // SD, XTHIMOUSE
            if (AnsiParams.Count == 1)
            {
                AnsiSeekLine();
                AnsiScrollInit(0 - AnsiProcess_Int11(AnsiParams[0], AnsiCmd_), AnsiState::AnsiScrollCommandDef::None);
            }
            if (AnsiParams.Count >= 5)
            {
                __AnsiResponse.Add("Mouse;2;" + AnsiCmd_.substr(1, AnsiCmd_.size() - 2));
            }
            break;
        case 'V':
            if (AnsiParams.Count == 2)
            {
                AnsiParams.Add("0");
            }
            if (AnsiParams.Count == 1)
            {
                AnsiParams.Add("0");
                AnsiParams.Add("0");
            }
            if (AnsiParams[0] == "") { AnsiParams[0] = "0"; }
            if (AnsiParams[1] == "") { AnsiParams[1] = "0"; }
            if (AnsiParams[2] == "") { AnsiParams[2] = "0"; }
            switch (_(AnsiParams[0].c_str()))
            {
                case _("0"): // Arbitrary font size and character part
                    AnsiState_.__AnsiFontSizeW = AnsiProcess_Int0(AnsiParams[1], AnsiCmd_);
                    AnsiState_.__AnsiFontSizeH = AnsiProcess_Int0(AnsiParams[2], AnsiCmd_);
                    if (AnsiState_.__AnsiFontSizeW > CoreStatic::FontMaxSizeCode)
                    {
                        AnsiState_.__AnsiFontSizeW = 0;
                    }
                    if (AnsiState_.__AnsiFontSizeH > CoreStatic::FontMaxSizeCode)
                    {
                        AnsiState_.__AnsiFontSizeH = 0;
                    }
                    break;
                case _("1"): // Screen display delay
                    {
                        SetProcessDelay(AnsiProcess_Int0(AnsiParams[1], AnsiCmd_));
                    }
                    break;
                case _("2"): // XBIN - Custom palette
                    if (UseCustomPaletteFont)
                    {
                        if (AnsiParams.Count > 16 * 3)
                        {
                            std::string PalDef = "";
                            for (int I = 1; I < AnsiParams.Count; I++)
                            {
                                PalDef = PalDef + Hex::IntToHex8(AnsiProcess_Int0(AnsiParams[I], AnsiCmd_));
                            }
                            Screen::SetPalette(PalDef + PalDef);
                        }
                    }
                    break;
                case _("3"): // XBIN - Custom font
                    if (UseCustomPaletteFont)
                    {
                        switch (AnsiProcess_Int0(AnsiParams[1], AnsiCmd_))
                        {
                            case 1:
                                SeekStateSaveForbid();
                                Screen::SetCustomFont(AnsiProcess_Int0(AnsiParams[2], AnsiCmd_));
                                break;
                            case 2:
                                SeekStateSaveForbid();
                                {
                                    std::string PalDef = AnsiParams[2];
                                    for (int I = 3; I < AnsiParams.Count; I++)
                                    {
                                        PalDef = PalDef + "|" + AnsiParams[I];
                                    }
                                    Screen::SetCustomChar(AnsiProcess_Int0(AnsiParams[2], AnsiCmd_), AnsiProcess_Int0(AnsiParams[3], AnsiCmd_), PalDef);
                                }
                                break;
                            case 3:
                                SeekStateSaveForce();
                                Screen::SetCustomChar(256, 0, "");
                                break;
                        }
                    }
                    break;
            }
            break;
        case 'X': // ECH
            {
                AnsiState_.__AnsiWrapFlag = false;
                int DelCycle = AnsiProcess_Int1(AnsiParams[0], AnsiCmd_);
                while (DelCycle > 0)
                {
                    AnsiCharFUnprotected2(AnsiState_.__AnsiX + DelCycle - 1, AnsiState_.__AnsiY, 32);
                    DelCycle--;
                }
            }
            break;
        case 'Z': // CBT
            {
                AnsiDoTab(0 - AnsiProcess_Int11(AnsiParams[0], AnsiCmd_));
            }
            break;
        case '`': // HPA
            if (!AnsiState_.StatusBar)
            {
                AnsiState_.__AnsiWrapFlag = false;
                AnsiState_.__AnsiX = AnsiProcess_Int1(AnsiParams[0], AnsiCmd_) - 1 + AnsiProcessGetXMin(true);
            }
            break;
        case 'a': // HPR
            if (!AnsiState_.StatusBar)
            {
                AnsiState_.__AnsiWrapFlag = false;
                AnsiState_.__AnsiX += AnsiProcess_Int1(AnsiParams[0], AnsiCmd_);
            }
            break;
        case 'b': // REP
            {
                if (AnsiCharPrintLast >= 0)
                {
                    AnsiCharPrintRepeater = AnsiProcess_Int11(AnsiParams[0], AnsiCmd_);
                    AnsiState_.ProcessBackgroundChars = true;
                }
            }
            break;
        case 'd': // VPA
            AnsiState_.__AnsiWrapFlag = false;
            AnsiState_.__AnsiY = AnsiProcess_Int1(AnsiParams[0], AnsiCmd_) - 1;
            if (AnsiState_.__AnsiOrigin)
            {
                AnsiState_.__AnsiY += AnsiState_.__AnsiScrollFirst;
            }
            break;
        case 'e': // VPR
            AnsiState_.__AnsiWrapFlag = false;
            AnsiState_.__AnsiY += AnsiProcess_Int1(AnsiParams[0], AnsiCmd_);
            break;
        case 'g': // TBC
            {
                switch (_(AnsiParams[0].c_str()))
                {
                    case _(""):
                    case _("0"):
                        if (AnsiState_.__AnsiTabs.Contains(AnsiState_.__AnsiX) && (AnsiState_.__AnsiX >= 0))
                        {
                            AnsiState_.__AnsiTabs.Remove(AnsiState_.__AnsiTabs.IndexOf(AnsiState_.__AnsiX));
                        }
                        break;
                    case _("3"):
                        AnsiState_.__AnsiTabs.Clear();
                        AnsiState_.__AnsiTabs.Add(-1);
                        break;
                }
            }
            break;
        case 'h': // SM
            AnsiState_.AnsiParamSet(AnsiParams[0], 1);
            switch (_(AnsiParams[0].c_str()))
            {
                case _("4"): // SM / IRM
                    AnsiState_.__AnsiInsertMode = true;
                    break;
                case _("6"): // SM / ???
                    AnsiState_.CharProtection2Ommit = true;
                    break;
                case _("12"): // SM / SRM
                    __AnsiResponse.Add("LocalEcho_0");
                    break;
                case _("20"): // SM / LNM
                    __AnsiResponse.Add("EnterKey_1");
                    break;
            }
            break;
        case 'l': // RM
            AnsiState_.AnsiParamSet(AnsiParams[0], 2);
            switch (_(AnsiParams[0].c_str()))
            {
                case _("4"): // RM / IRM
                    AnsiState_.__AnsiInsertMode = false;
                    break;
                case _("6"): // RM / ???
                    AnsiState_.CharProtection2Ommit = false;
                    break;
                case _("12"): // RM / SRM
                    __AnsiResponse.Add("LocalEcho_1");
                    break;
                case _("20"): // RM / LNM
                    __AnsiResponse.Add("EnterKey_0");
                    break;
            }
            break;
        case 'm': // SGR
            {
                AnsiProcess_CSI_m(AnsiParams);
            }
            break;
        case 'p':
            {
                // DECRQM
                if (TextWork::StringEndsWith(AnsiCmd_, "$p"))
                {
                    __AnsiResponse.Add(AnsiCmd_);
                }

                // DECSCL
                if (TextWork::StringEndsWith(AnsiCmd_, "\"p"))
                {
                    if (AnsiParams.Count >= 2)
                    {
                        if (AnsiProcess_Int0(AnsiParams[1].substr(0, AnsiParams[1].size() - 1), AnsiCmd_) == 1)
                        {
                            __AnsiResponse.Add("Control8bit_0");
                        }
                        else
                        {
                            __AnsiResponse.Add("Control8bit_1");
                        }
                    }
                }
            }
            break;
        case 'q':
            {
                // DECSCA
                if (TextWork::StringEndsWith(AnsiCmd_, "\"q"))
                {
                    if (AnsiParams.Count >= 1)
                    {
                        AnsiParams[AnsiParams.Count - 1] = AnsiParams[AnsiParams.Count - 1].substr(0, AnsiParams[AnsiParams.Count - 1].size() - 1);
                        if (AnsiProcess_Int0(AnsiParams[0], AnsiCmd_) == 1)
                        {
                            AnsiState_.CharProtection1Print = true;
                        }
                        else
                        {
                            AnsiState_.CharProtection1Print = false;
                        }
                    }
                }

                // DECSCUSR
                if (TextWork::StringEndsWith(AnsiCmd_, " q"))
                {
                    if (AnsiParams.Count >= 1)
                    {
                        AnsiParams[AnsiParams.Count - 1] = AnsiParams[AnsiParams.Count - 1].substr(0, AnsiParams[AnsiParams.Count - 1].size() - 1);
                        switch (AnsiProcess_Int0(AnsiParams[0], AnsiCmd_))
                        {
                            case 0: AnsiState_.CursorTerm = 5; break;
                            case 1: AnsiState_.CursorTerm = 5; break;
                            case 2: AnsiState_.CursorTerm = 6; break;
                            case 3: AnsiState_.CursorTerm = 1; break;
                            case 4: AnsiState_.CursorTerm = 2; break;
                            case 5: AnsiState_.CursorTerm = 3; break;
                            case 6: AnsiState_.CursorTerm = 4; break;
                            default: AnsiState_.CursorTerm = 0; break;
                        }
                    }
                }
            }
            break;
        case 'r': // DECSTBM
            // DECCARA
            if (TextWork::StringEndsWith(AnsiCmd_, "$r"))
            {
                if (AnsiParams.Count >= 5)
                {
                    AnsiParams[AnsiParams.Count - 1] = AnsiParams[AnsiParams.Count - 1].substr(0, AnsiParams[AnsiParams.Count - 1].size() - 1);
                    AnsiAttributesSave();
                    int RecY1 = AnsiProcess_Int0(AnsiParams[0], AnsiCmd_) - 1;
                    int RecX1 = AnsiProcess_Int0(AnsiParams[1], AnsiCmd_) - 1;
                    int RecY2 = AnsiProcess_Int0(AnsiParams[2], AnsiCmd_) - 1;
                    int RecX2 = AnsiProcess_Int0(AnsiParams[3], AnsiCmd_) - 1;
                    int Attr = AnsiProcess_Int0(AnsiParams[4], AnsiCmd_);
                    if (AnsiState_.__AnsiOrigin)
                    {
                        RecY1 += AnsiState_.__AnsiScrollFirst;
                        RecY2 += AnsiState_.__AnsiScrollFirst;
                        if (AnsiState_.__AnsiMarginLeftRight)
                        {
                            RecX1 += AnsiState_.__AnsiMarginLeft;
                            RecX2 += AnsiState_.__AnsiMarginLeft;
                        }
                    }

                    for (int Y = RecY1; Y <= RecY2; Y++)
                    {
                        for (int X = RecX1; X <= RecX2; X++)
                        {
                            AnsiGetF(X, Y);
                            int TempC = AnsiGetLast.Item_Char;
                            int TempB = AnsiGetLast.Item_ColorB;
                            int TempF = AnsiGetLast.Item_ColorF;
                            int TempA = AnsiGetLast.Item_ColorA;
                            switch (Attr)
                            {
                                case 0:
                                    TempA = TempA & 0x20;
                                    break;
                                case 1: // Bold
                                    TempA = TempA | 0x01;
                                    break;
                                case 3: // Italic
                                    TempA = TempA | 0x02;
                                    break;
                                case 4: // Underline
                                    TempA = TempA | 0x04;
                                    break;
                                case 5: // Blink
                                    TempA = TempA | 0x08;
                                    break;
                                case 7: // Inverse
                                    TempA = TempA | 0x10;
                                    break;
                                case 8: // Invisible
                                    TempA = TempA | 0x20;
                                    break;
                                case 9: // Strikethrough
                                    TempA = TempA | 0x40;
                                    break;
                            }
                            AnsiState_.__AnsiBack = TempB;
                            AnsiState_.__AnsiFore = TempF;
                            AnsiState_.__AnsiAttr = TempA;
                            AnsiCharF(X, Y, TempC);
                        }
                        AnsiRepaintLine(Y);
                    }
                    AnsiAttributesLoad();
                }
            }
            else
            {
                AnsiState_.__AnsiWrapFlag = false;
                if (AnsiParams.Count == 1)
                {
                    AnsiParams.Add(std::to_string(AnsiMaxY + 1));
                }
                AnsiState_.__AnsiScrollFirst = AnsiProcess_Int0(AnsiParams[0], AnsiCmd_) - 1;
                AnsiState_.__AnsiScrollLast = AnsiProcess_Int0(AnsiParams[1], AnsiCmd_) - 1;
                if (AnsiState_.__AnsiScrollFirst > AnsiState_.__AnsiScrollLast)
                {
                    AnsiState_.__AnsiScrollFirst = 0;
                    AnsiState_.__AnsiScrollLast = AnsiMaxY - 1;
                }
                if (AnsiState_.__AnsiScrollFirst < 0)
                {
                    AnsiState_.__AnsiScrollFirst = 0;
                }
                if (AnsiState_.__AnsiScrollLast >= AnsiMaxY)
                {
                    AnsiState_.__AnsiScrollLast = AnsiMaxY - 1;
                }
                AnsiState_.__AnsiX = 0;
                if (AnsiState_.__AnsiOrigin)
                {
                    AnsiState_.__AnsiY = AnsiState_.__AnsiScrollFirst;
                }
                else
                {
                    AnsiState_.__AnsiY = 0;
                }
            }
            break;
        case 's': // DECSLRM
            if (AnsiParams.Count == 1)
            {
                AnsiParams.Add(std::to_string(AnsiMaxX + 1));
            }
            if (AnsiState_.__AnsiMarginLeftRight)
            {
                AnsiState_.__AnsiMarginLeft = AnsiProcess_Int0(AnsiParams[0], AnsiCmd_) - 1;
                AnsiState_.__AnsiMarginRight = AnsiProcess_Int0(AnsiParams[1], AnsiCmd_);
                if ((AnsiState_.__AnsiMarginLeft == -1) && (AnsiState_.__AnsiMarginRight == 0))
                {
                    AnsiState_.__AnsiMarginLeft = 0;
                    AnsiState_.__AnsiMarginRight = AnsiMaxX;
                }
                if (AnsiState_.__AnsiMarginLeft >= AnsiState_.__AnsiMarginRight)
                {
                    AnsiState_.__AnsiMarginLeft = 0;
                    AnsiState_.__AnsiMarginRight = AnsiMaxX;
                }
                if (AnsiState_.__AnsiMarginLeft < 0)
                {
                    AnsiState_.__AnsiMarginLeft = 0;
                }
                if (AnsiState_.__AnsiMarginRight > AnsiMaxX)
                {
                    AnsiState_.__AnsiMarginRight = AnsiMaxX;
                }
            }
            break;
        case 't': // XTWINOPS
            // DECRARA
            if (TextWork::StringEndsWith(AnsiCmd_, "$t"))
            {
                if (AnsiParams.Count >= 5)
                {
                    AnsiParams[AnsiParams.Count - 1] = AnsiParams[AnsiParams.Count - 1].substr(0, AnsiParams[AnsiParams.Count - 1].size() - 1);
                    AnsiAttributesSave();
                    int RecY1 = AnsiProcess_Int0(AnsiParams[0], AnsiCmd_) - 1;
                    int RecX1 = AnsiProcess_Int0(AnsiParams[1], AnsiCmd_) - 1;
                    int RecY2 = AnsiProcess_Int0(AnsiParams[2], AnsiCmd_) - 1;
                    int RecX2 = AnsiProcess_Int0(AnsiParams[3], AnsiCmd_) - 1;
                    int Attr = AnsiProcess_Int0(AnsiParams[4], AnsiCmd_);
                    if (AnsiState_.__AnsiOrigin)
                    {
                        RecY1 += AnsiState_.__AnsiScrollFirst;
                        RecY2 += AnsiState_.__AnsiScrollFirst;
                        if (AnsiState_.__AnsiMarginLeftRight)
                        {
                            RecX1 += AnsiState_.__AnsiMarginLeft;
                            RecX2 += AnsiState_.__AnsiMarginLeft;
                        }
                    }

                    for (int Y = RecY1; Y <= RecY2; Y++)
                    {
                        for (int X = RecX1; X <= RecX2; X++)
                        {
                            AnsiGetF(X, Y);
                            int TempC = AnsiGetLast.Item_Char;
                            int TempB = AnsiGetLast.Item_ColorB;
                            int TempF = AnsiGetLast.Item_ColorF;
                            int TempA = AnsiGetLast.Item_ColorA;
                            switch (Attr)
                            {
                                case 1: // Bold
                                    if ((TempA & 1) > 0)
                                    {
                                        TempA = TempA & 0xFE;
                                    }
                                    else
                                    {
                                        TempA = TempA | 0x01;
                                    }
                                    break;
                                case 3: // Italic
                                    if ((TempA & 2) > 0)
                                    {
                                        TempA = TempA & 0xFD;
                                    }
                                    else
                                    {
                                        TempA = TempA | 0x02;
                                    }
                                    break;
                                case 4: // Underline
                                    if ((TempA & 4) > 0)
                                    {
                                        TempA = TempA & 0xFB;
                                    }
                                    else
                                    {
                                        TempA = TempA | 0x04;
                                    }
                                    break;
                                case 5: // Blink
                                    if ((TempA & 8) > 0)
                                    {
                                        TempA = TempA & 0xF7;
                                    }
                                    else
                                    {
                                        TempA = TempA | 0x08;
                                    }
                                    break;
                                case 7: // Inverse
                                    if ((TempA & 16) > 0)
                                    {
                                        TempA = TempA & 0xEF;
                                    }
                                    else
                                    {
                                        TempA = TempA | 0x10;
                                    }
                                    break;
                                case 8: // Invisible
                                    if ((TempA & 32) > 0)
                                    {
                                        TempA = TempA & 0xDF;
                                    }
                                    else
                                    {
                                        TempA = TempA | 0x20;
                                    }
                                    break;
                                case 9: // Strikethrough
                                    if ((TempA & 64) > 0)
                                    {
                                        TempA = TempA & 0xBF;
                                    }
                                    else
                                    {
                                        TempA = TempA | 0x40;
                                    }
                                    break;
                            }
                            AnsiState_.__AnsiBack = TempB;
                            AnsiState_.__AnsiFore = TempF;
                            AnsiState_.__AnsiAttr = TempA;
                            AnsiCharF(X, Y, TempC);
                        }
                        AnsiRepaintLine(Y);
                    }
                    AnsiAttributesLoad();
                }
            }
            else
            {
                {
                    switch (AnsiProcess_Int0(AnsiParams[0], AnsiCmd_))
                    {
                        case 11:
                        case 13:
                        case 14:
                        case 15:
                        case 16:
                        case 18:
                        case 19:
                        case 20:
                        case 21:
                        case 22:
                        case 23:
                            __AnsiResponse.Add(AnsiCmd_);
                            break;
                        default: // DECSLPP
                            {
                                int S = AnsiProcess_Int0(AnsiParams[0], AnsiCmd_);
                                if (S >= 24)
                                {
                                    AnsiResize(-1, S);
                                }
                            }
                            break;
                    }
                }
                if (AnsiParams.Count == 3)
                {
                    switch (AnsiProcess_Int0(AnsiParams[0], AnsiCmd_))
                    {
                        case 4:
                            {
                                int CmdW = AnsiProcess_Int0(AnsiParams[2], AnsiCmd_) / Screen::TerminalCellW;
                                int CmdH = AnsiProcess_Int0(AnsiParams[1], AnsiCmd_) / Screen::TerminalCellH;
                                if ((CmdW > 0) && (CmdH > 0))
                                {
                                    AnsiResize(CmdW, CmdH);
                                }
                                else
                                {
                                    if (CmdW > 0)
                                    {
                                        AnsiResize(CmdW, -1);
                                    }
                                    if (CmdH > 0)
                                    {
                                        AnsiResize(-1, CmdH);
                                    }
                                }
                            }
                            break;
                        case 8:
                            {
                                int CmdW = AnsiProcess_Int0(AnsiParams[2], AnsiCmd_);
                                int CmdH = AnsiProcess_Int0(AnsiParams[1], AnsiCmd_);
                                if ((CmdW > 0) && (CmdH > 0))
                                {
                                    AnsiResize(CmdW, CmdH);
                                }
                                else
                                {
                                    if (CmdW > 0)
                                    {
                                        AnsiResize(CmdW, -1);
                                    }
                                    if (CmdH > 0)
                                    {
                                        AnsiResize(-1, CmdH);
                                    }
                                }
                            }
                            break;
                    }
                }
            }
            break;
        case 'v':
            // DECCRA
            if (TextWork::StringEndsWith(AnsiCmd_, "$v"))
            {
                if (AnsiParams.Count >= 8)
                {
                    AnsiParams[AnsiParams.Count - 1] = AnsiParams[AnsiParams.Count - 1].substr(0, AnsiParams[AnsiParams.Count - 1].size() - 1);
                    AnsiAttributesSave();
                    int RecY1 = AnsiProcess_Int0(AnsiParams[0], AnsiCmd_) - 1;
                    int RecX1 = AnsiProcess_Int0(AnsiParams[1], AnsiCmd_) - 1;
                    int RecY2 = AnsiProcess_Int0(AnsiParams[2], AnsiCmd_) - 1;
                    int RecX2 = AnsiProcess_Int0(AnsiParams[3], AnsiCmd_) - 1;
                    int TargetY = AnsiProcess_Int0(AnsiParams[5], AnsiCmd_) - 1;
                    int TargetX = AnsiProcess_Int0(AnsiParams[6], AnsiCmd_) - 1;
                    TargetX = TargetX - RecX1;
                    TargetY = TargetY - RecY1;
                    if (AnsiState_.__AnsiOrigin)
                    {
                        RecY1 += AnsiState_.__AnsiScrollFirst;
                        RecY2 += AnsiState_.__AnsiScrollFirst;
                        if (AnsiState_.__AnsiMarginLeftRight)
                        {
                            RecX1 += AnsiState_.__AnsiMarginLeft;
                            RecX2 += AnsiState_.__AnsiMarginLeft;
                        }
                    }

                    if (TargetY >= 0)
                    {
                        for (int Y = RecY2; Y >= RecY1; Y--)
                        {
                            if (TargetX >= 0)
                            {
                                for (int X = RecX2; X >= RecX1; X--)
                                {
                                    AnsiGetF(X, Y);
                                    int TempC = AnsiGetLast.Item_Char;
                                    int TempB = AnsiGetLast.Item_ColorB;
                                    int TempF = AnsiGetLast.Item_ColorF;
                                    int TempA = AnsiGetLast.Item_ColorA;
                                    AnsiState_.__AnsiBack = TempB;
                                    AnsiState_.__AnsiFore = TempF;
                                    AnsiState_.__AnsiAttr = TempA;
                                    AnsiCharF(X + TargetX, Y + TargetY, TempC);
                                }
                            }
                            else
                            {
                                for (int X = RecX1; X <= RecX2; X++)
                                {
                                    AnsiGetF(X, Y);
                                    int TempC = AnsiGetLast.Item_Char;
                                    int TempB = AnsiGetLast.Item_ColorB;
                                    int TempF = AnsiGetLast.Item_ColorF;
                                    int TempA = AnsiGetLast.Item_ColorA;
                                    AnsiState_.__AnsiBack = TempB;
                                    AnsiState_.__AnsiFore = TempF;
                                    AnsiState_.__AnsiAttr = TempA;
                                    AnsiCharF(X + TargetX, Y + TargetY, TempC);
                                }
                            }
                            AnsiRepaintLine(Y + TargetY);
                        }
                    }
                    else
                    {
                        for (int Y = RecY1; Y <= RecY2; Y++)
                        {
                            if (TargetX >= 0)
                            {
                                for (int X = RecX2; X >= RecX1; X--)
                                {
                                    AnsiGetF(X, Y);
                                    int TempC = AnsiGetLast.Item_Char;
                                    int TempB = AnsiGetLast.Item_ColorB;
                                    int TempF = AnsiGetLast.Item_ColorF;
                                    int TempA = AnsiGetLast.Item_ColorA;
                                    AnsiState_.__AnsiBack = TempB;
                                    AnsiState_.__AnsiFore = TempF;
                                    AnsiState_.__AnsiAttr = TempA;
                                    AnsiCharF(X + TargetX, Y + TargetY, TempC);
                                }
                            }
                            else
                            {
                                for (int X = RecX1; X <= RecX2; X++)
                                {
                                    AnsiGetF(X, Y);
                                    int TempC = AnsiGetLast.Item_Char;
                                    int TempB = AnsiGetLast.Item_ColorB;
                                    int TempF = AnsiGetLast.Item_ColorF;
                                    int TempA = AnsiGetLast.Item_ColorA;
                                    AnsiState_.__AnsiBack = TempB;
                                    AnsiState_.__AnsiFore = TempF;
                                    AnsiState_.__AnsiAttr = TempA;
                                    AnsiCharF(X + TargetX, Y + TargetY, TempC);
                                }
                            }
                            AnsiRepaintLine(Y + TargetY);
                        }
                    }
                    AnsiAttributesLoad();
                }
            }
            break;
        case 'w':
            // DECEFR
            if (TextWork::StringEndsWith(AnsiCmd_, "\'w"))
            {
                __AnsiResponse.Add("Mouse;13;" + AnsiCmd_.substr(1, AnsiCmd_.size() - 3));
            }
            break;
        case 'x':
            // DECFRA
            if (TextWork::StringEndsWith(AnsiCmd_, "$x"))
            {
                if (AnsiParams.Count >= 5)
                {
                    AnsiParams[AnsiParams.Count - 1] = AnsiParams[AnsiParams.Count - 1].substr(0, AnsiParams[AnsiParams.Count - 1].size() - 1);
                    AnsiAttributesSave();
                    int FillC = AnsiProcess_Int0(AnsiParams[0], AnsiCmd_);
                    int RecY1 = AnsiProcess_Int0(AnsiParams[1], AnsiCmd_) - 1;
                    int RecX1 = AnsiProcess_Int0(AnsiParams[2], AnsiCmd_) - 1;
                    int RecY2 = AnsiProcess_Int0(AnsiParams[3], AnsiCmd_) - 1;
                    int RecX2 = AnsiProcess_Int0(AnsiParams[4], AnsiCmd_) - 1;
                    if (AnsiState_.__AnsiOrigin)
                    {
                        RecY1 += AnsiState_.__AnsiScrollFirst;
                        RecY2 += AnsiState_.__AnsiScrollFirst;
                        if (AnsiState_.__AnsiMarginLeftRight)
                        {
                            RecX1 += AnsiState_.__AnsiMarginLeft;
                            RecX2 += AnsiState_.__AnsiMarginLeft;
                        }
                    }

                    FillC = AnsiState_.GetChar(FillC);

                    int FillCDbl = Screen::CharDouble(FillC);
                    if (FillCDbl != 0)
                    {
                        for (int Y = RecY1; Y <= RecY2; Y++)
                        {
                            for (int X = RecX1; X < RecX2; X += 2)
                            {
                                AnsiCharF(X + 0, Y, FillC);
                                AnsiCharF(X + 1, Y, FillCDbl);
                            }
                            AnsiRepaintLine(Y);
                        }
                    }
                    else
                    {
                        for (int Y = RecY1; Y <= RecY2; Y++)
                        {
                            for (int X = RecX1; X <= RecX2; X++)
                            {
                                AnsiCharF(X, Y, FillC);
                            }
                            AnsiRepaintLine(Y);
                        }
                    }
                    AnsiAttributesLoad();
                }
            }
            break;
        case 'y':
            // DECRQCRA
            if (TextWork::StringEndsWith(AnsiCmd_, "*y"))
            {
                __AnsiResponse.Add(AnsiCmd_);
            }
            break;
        case 'z':
            // DECERA
            if (TextWork::StringEndsWith(AnsiCmd_, "$z"))
            {
                if (AnsiParams.Count >= 4)
                {
                    AnsiParams[AnsiParams.Count - 1] = AnsiParams[AnsiParams.Count - 1].substr(0, AnsiParams[AnsiParams.Count - 1].size() - 1);
                    AnsiAttributesSave();
                    int RecY1 = AnsiProcess_Int0(AnsiParams[0], AnsiCmd_) - 1;
                    int RecX1 = AnsiProcess_Int0(AnsiParams[1], AnsiCmd_) - 1;
                    int RecY2 = AnsiProcess_Int0(AnsiParams[2], AnsiCmd_) - 1;
                    int RecX2 = AnsiProcess_Int0(AnsiParams[3], AnsiCmd_) - 1;
                    if (AnsiState_.__AnsiOrigin)
                    {
                        RecY1 += AnsiState_.__AnsiScrollFirst;
                        RecY2 += AnsiState_.__AnsiScrollFirst;
                        if (AnsiState_.__AnsiMarginLeftRight)
                        {
                            RecX1 += AnsiState_.__AnsiMarginLeft;
                            RecX2 += AnsiState_.__AnsiMarginLeft;
                        }
                    }

                    for (int Y = RecY1; Y <= RecY2; Y++)
                    {
                        for (int X = RecX1; X <= RecX2; X++)
                        {
                            AnsiCharF(X, Y, ' ');
                        }
                        AnsiRepaintLine(Y);
                    }
                    AnsiAttributesLoad();
                }
            }

            // DECELR
            if (TextWork::StringEndsWith(AnsiCmd_, "\'z"))
            {
                __AnsiResponse.Add("Mouse;10;" + AnsiCmd_.substr(1, AnsiCmd_.size() - 3));
            }
            break;
        case '{':
            // DECSERA
            if (TextWork::StringEndsWith(AnsiCmd_, "${"))
            {
                if (AnsiParams.Count >= 4)
                {
                    AnsiParams[AnsiParams.Count - 1] = AnsiParams[AnsiParams.Count - 1].substr(0, AnsiParams[AnsiParams.Count - 1].size() - 1);
                    AnsiAttributesSave();
                    int RecY1 = AnsiProcess_Int0(AnsiParams[0], AnsiCmd_) - 1;
                    int RecX1 = AnsiProcess_Int0(AnsiParams[1], AnsiCmd_) - 1;
                    int RecY2 = AnsiProcess_Int0(AnsiParams[2], AnsiCmd_) - 1;
                    int RecX2 = AnsiProcess_Int0(AnsiParams[3], AnsiCmd_) - 1;
                    if (AnsiState_.__AnsiOrigin)
                    {
                        RecY1 += AnsiState_.__AnsiScrollFirst;
                        RecY2 += AnsiState_.__AnsiScrollFirst;
                        if (AnsiState_.__AnsiMarginLeftRight)
                        {
                            RecX1 += AnsiState_.__AnsiMarginLeft;
                            RecX2 += AnsiState_.__AnsiMarginLeft;
                        }
                    }

                    for (int Y = RecY1; Y <= RecY2; Y++)
                    {
                        for (int X = RecX1; X <= RecX2; X++)
                        {
                            AnsiGetF(X, Y);
                            int TempB = AnsiGetLast.Item_ColorB;
                            int TempF = AnsiGetLast.Item_ColorF;
                            int TempA = AnsiGetLast.Item_ColorA;
                            AnsiState_.__AnsiBack = TempB;
                            AnsiState_.__AnsiFore = TempF;
                            AnsiState_.__AnsiAttr = TempA;
                            AnsiCharFUnprotected1(X, Y, ' ');
                        }
                        AnsiRepaintLine(Y);
                    }
                    AnsiAttributesLoad();
                }
            }

            // DECSLE
            if (TextWork::StringEndsWith(AnsiCmd_, "\'{"))
            {
                __AnsiResponse.Add("Mouse;11;" + AnsiCmd_.substr(1, AnsiCmd_.size() - 3));
            }
            break;
        case '|':
            {
                // DECSNLS
                if ((AnsiCmd_.size() > 2) && (TextWork::StringEndsWith(AnsiCmd_, "*|")))
                {
                    AnsiResize(-1, AnsiProcess_Int1(AnsiParams[0].substr(0, AnsiParams[0].size() - 1), AnsiCmd_));
                }

                // DECSCPP
                if ((AnsiCmd_.size() > 2) && (TextWork::StringEndsWith(AnsiCmd_, "$|")))
                {
                    int Dim = AnsiProcess_Int0(AnsiParams[0].substr(0, AnsiParams[0].size() - 1), AnsiCmd_);
                    if (Dim == 0) Dim = 80;
                    AnsiResize(Dim, -1);
                }
            }

            // DECRQLP
            if (TextWork::StringEndsWith(AnsiCmd_, "\'|"))
            {
                __AnsiResponse.Add("Mouse;12;" + AnsiCmd_.substr(1, AnsiCmd_.size() - 3));
            }
            break;
        case '}':
            // DECIC
            if (TextWork::StringEndsWith(AnsiCmd_, "'}"))
            {
                AnsiState_.__AnsiWrapFlag = false;
                if (AnsiParams[0].size() > 1)
                {
                    AnsiParams[0] = AnsiParams[0].substr(0, AnsiParams[0].size() - 1);
                }
                else
                {
                    AnsiParams[0] = "1";
                }
                int __AnsiX = AnsiState_.__AnsiX;
                if ((!AnsiState_.__AnsiMarginLeftRight) || ((__AnsiX >= AnsiState_.__AnsiMarginLeft) && (__AnsiX < AnsiState_.__AnsiMarginRight)))
                {
                    if (AnsiState_.__AnsiOrigin || ((AnsiState_.__AnsiY >= AnsiState_.__AnsiScrollFirst) && (AnsiState_.__AnsiY <= AnsiState_.__AnsiScrollLast)))
                    {
                        for (int __AnsiY = AnsiState_.__AnsiScrollFirst; __AnsiY <= AnsiState_.__AnsiScrollLast; __AnsiY++)
                        {
                            int InsCycle = AnsiProcess_Int1(AnsiParams[0], AnsiCmd_);
                            if (AnsiState_.__AnsiLineOccupy__.CountLines() > __AnsiY)
                            {
                                int FontSize = AnsiGetFontSize(__AnsiY);
                                while (InsCycle > 0)
                                {
                                    if (FontSize > 0)
                                    {
                                        AnsiGetF(__AnsiX, __AnsiY);
                                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) > (__AnsiX * 2))
                                        {
                                            AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                                            AnsiState_.__AnsiLineOccupy__.Item_FontW = 2;
                                            AnsiState_.__AnsiLineOccupy__.Item_FontH = FontSize - 1;
                                            AnsiState_.__AnsiLineOccupy__.Insert(__AnsiY, __AnsiX * 2);
                                            AnsiState_.__AnsiLineOccupy__.Item_FontW = 1;
                                            AnsiState_.__AnsiLineOccupy__.Insert(__AnsiY, __AnsiX * 2);
                                        }
                                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) > (AnsiProcessGetXMax(false)))
                                        {
                                            AnsiState_.__AnsiLineOccupy__.Delete(__AnsiY, AnsiProcessGetXMax(false));
                                            AnsiState_.__AnsiLineOccupy__.Delete(__AnsiY, AnsiProcessGetXMax(false));
                                        }
                                    }
                                    else
                                    {
                                        AnsiGetF(__AnsiX, __AnsiY);
                                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) > (__AnsiX))
                                        {
                                            AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);

                                            AnsiState_.__AnsiLineOccupy__.Insert(__AnsiY, __AnsiX);
                                        }
                                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) > (AnsiProcessGetXMax(false)))
                                        {
                                            AnsiState_.__AnsiLineOccupy__.Delete(__AnsiY, AnsiProcessGetXMax(false));
                                        }
                                    }
                                    InsCycle--;
                                }
                                AnsiRepaintLine(__AnsiY);
                            }
                            AnsiState_.PrintCharInsDel++;
                        }
                    }
                }
            }
            break;
        case '~':
            // DECDC
            if (TextWork::StringEndsWith(AnsiCmd_, "'~"))
            {
                AnsiState_.__AnsiWrapFlag = false;
                if (AnsiParams[0].size() > 1)
                {
                    AnsiParams[0] = AnsiParams[0].substr(0, AnsiParams[0].size() - 1);
                }
                else
                {
                    AnsiParams[0] = "1";
                }
                int __AnsiX = AnsiState_.__AnsiX;
                if ((!AnsiState_.__AnsiMarginLeftRight) || ((__AnsiX >= AnsiState_.__AnsiMarginLeft) && (__AnsiX < AnsiState_.__AnsiMarginRight)))
                {
                    if (AnsiState_.__AnsiOrigin || ((AnsiState_.__AnsiY >= AnsiState_.__AnsiScrollFirst) && (AnsiState_.__AnsiY <= AnsiState_.__AnsiScrollLast)))
                    {
                        for (int __AnsiY = AnsiState_.__AnsiScrollFirst; __AnsiY <= AnsiState_.__AnsiScrollLast; __AnsiY++)
                        {
                            int DelCycle = AnsiProcess_Int1(AnsiParams[0], AnsiCmd_);
                            if (AnsiState_.__AnsiLineOccupy__.CountLines() > __AnsiY)
                            {
                                while (DelCycle > 0)
                                {
                                    if (AnsiGetFontSize(__AnsiY) > 0)
                                    {
                                        AnsiGetF((AnsiProcessGetXMax(false)) - 1, __AnsiY);
                                        int TempC = AnsiGetLast.Item_Char;
                                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) >= ((AnsiProcessGetXMax(false))))
                                        {
                                            AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);

                                            AnsiState_.__AnsiLineOccupy__.Insert(__AnsiY, (AnsiProcessGetXMax(false)));
                                            AnsiState_.__AnsiLineOccupy__.Insert(__AnsiY, (AnsiProcessGetXMax(false)));
                                        }
                                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) > (__AnsiX))
                                        {
                                            AnsiState_.__AnsiLineOccupy__.Delete(__AnsiY, __AnsiX);
                                            AnsiState_.__AnsiLineOccupy__.Delete(__AnsiY, __AnsiX);
                                        }
                                        if (TempC > 0)
                                        {
                                            AnsiCharF((AnsiProcessGetXMax(false) / 2) - 1, __AnsiY, 32);
                                        }
                                    }
                                    else
                                    {
                                        AnsiGetF(AnsiProcessGetXMax(false) - 1, __AnsiY);
                                        int TempC = AnsiGetLast.Item_Char;
                                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) >= ((AnsiProcessGetXMax(false))))
                                        {
                                            AnsiState_.__AnsiLineOccupy__.BlankChar(AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);

                                            AnsiState_.__AnsiLineOccupy__.Insert(__AnsiY, (AnsiProcessGetXMax(false)));
                                        }
                                        if (AnsiState_.__AnsiLineOccupy__.CountItems(__AnsiY) > (__AnsiX))
                                        {
                                            AnsiState_.__AnsiLineOccupy__.Delete(__AnsiY, __AnsiX);
                                        }
                                        if (TempC > 0)
                                        {
                                            AnsiCharF(AnsiProcessGetXMax(false) - 1, __AnsiY, 32);
                                        }
                                    }
                                    DelCycle--;
                                }
                                AnsiRepaintLine(__AnsiY);
                            }
                            AnsiState_.PrintCharInsDel++;
                        }
                    }
                }
            }
            break;
    }
}

void CoreAnsi::AnsiProcess_CSI_m(XList<std::string> AnsiParams)
{
    if (AnsiState_.StatusBar)
    {
        return;
    }

    for (int i_ = 0; i_ < AnsiParams.Count; i_++)
    {
        switch (_(AnsiParams[i_].c_str()))
        {
            case _("0"):
            case _("00"):
            case _(""):
                AnsiState_.__AnsiFore = -1;
                AnsiState_.__AnsiBack = -1;
                AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr & 0x80;
                break;

            // Bold
            case _("1"): AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr | 0x01; break;
            case _("22"): AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr & 0xFE; break;

            // Italic
            case _("3"): AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr | 0x02; break;
            case _("23"): AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr & 0xFD; break;

            // Underline
            case _("4"): AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr | 0x04; break;
            case _("24"): AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr & 0xFB; break;

            // Blink
            case _("5"): AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr | 0x08; break;
            case _("25"): AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr & 0xF7; break;

            // Reverse
            case _("7"): AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr | 0x10; break;
            case _("27"): AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr & 0xEF; break;

            // Conceale
            case _("8"): AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr | 0x20; break;
            case _("28"): AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr & 0xDF; break;

            // Strikethrough
            case _("9"): AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr | 0x40; break;
            case _("29"): AnsiState_.__AnsiAttr = AnsiState_.__AnsiAttr & 0xBF; break;

            // Default color
            case _("39"): AnsiState_.__AnsiFore = -1; break;
            case _("49"): AnsiState_.__AnsiBack = -1; break;

            // Foreground color
            case _("30"): AnsiState_.__AnsiFore = 0; break;
            case _("31"): AnsiState_.__AnsiFore = 1; break;
            case _("32"): AnsiState_.__AnsiFore = 2; break;
            case _("33"): AnsiState_.__AnsiFore = 3; break;
            case _("34"): AnsiState_.__AnsiFore = 4; break;
            case _("35"): AnsiState_.__AnsiFore = 5; break;
            case _("36"): AnsiState_.__AnsiFore = 6; break;
            case _("37"): AnsiState_.__AnsiFore = 7; break;
            case _("90"): AnsiState_.__AnsiFore = 8; break;
            case _("91"): AnsiState_.__AnsiFore = 9; break;
            case _("92"): AnsiState_.__AnsiFore = 10; break;
            case _("93"): AnsiState_.__AnsiFore = 11; break;
            case _("94"): AnsiState_.__AnsiFore = 12; break;
            case _("95"): AnsiState_.__AnsiFore = 13; break;
            case _("96"): AnsiState_.__AnsiFore = 14; break;
            case _("97"): AnsiState_.__AnsiFore = 15; break;

            // Background color
            case _("40"): AnsiState_.__AnsiBack = 0; break;
            case _("41"): AnsiState_.__AnsiBack = 1; break;
            case _("42"): AnsiState_.__AnsiBack = 2; break;
            case _("43"): AnsiState_.__AnsiBack = 3; break;
            case _("44"): AnsiState_.__AnsiBack = 4; break;
            case _("45"): AnsiState_.__AnsiBack = 5; break;
            case _("46"): AnsiState_.__AnsiBack = 6; break;
            case _("47"): AnsiState_.__AnsiBack = 7; break;
            case _("100"): AnsiState_.__AnsiBack = 8; break;
            case _("101"): AnsiState_.__AnsiBack = 9; break;
            case _("102"): AnsiState_.__AnsiBack = 10; break;
            case _("103"): AnsiState_.__AnsiBack = 11; break;
            case _("104"): AnsiState_.__AnsiBack = 12; break;
            case _("105"): AnsiState_.__AnsiBack = 13; break;
            case _("106"): AnsiState_.__AnsiBack = 14; break;
            case _("107"): AnsiState_.__AnsiBack = 15; break;

            case _("38"):
                {
                    if (AnsiParams.Count > i_ + 2)
                    {
                        if (AnsiParams[i_ + 1] == "5")
                        {
                            AnsiState_.__AnsiFore = Color256[TextWork::StrToInt(AnsiParams[i_ + 2], 256)];
                            i_ += 2;
                        }
                    }
                    if (AnsiParams.Count > i_ + 4)
                    {
                        if (AnsiParams[i_ + 1] == "2")
                        {
                            int TempR = TextWork::StrToInt(AnsiParams[i_ + 2], -1);
                            int TempG = TextWork::StrToInt(AnsiParams[i_ + 3], -1);
                            int TempB = TextWork::StrToInt(AnsiParams[i_ + 4], -1);
                            AnsiState_.__AnsiFore = AnsiColor16(TempR, TempG, TempB);
                            i_ += 4;
                        }
                    }
                }
                break;

            case _("48"):
                {
                    if (AnsiParams.Count > i_ + 2)
                    {
                        if (AnsiParams[i_ + 1] == "5")
                        {
                            AnsiState_.__AnsiBack = Color256[TextWork::StrToInt(AnsiParams[i_ + 2], 256)];
                            i_ += 2;
                        }
                    }
                    if (AnsiParams.Count > i_ + 4)
                    {
                        if (AnsiParams[i_ + 1] == "2")
                        {
                            int TempR = TextWork::StrToInt(AnsiParams[i_ + 2], -1);
                            int TempG = TextWork::StrToInt(AnsiParams[i_ + 3], -1);
                            int TempB = TextWork::StrToInt(AnsiParams[i_ + 4], -1);
                            AnsiState_.__AnsiBack = AnsiColor16(TempR, TempG, TempB);
                            i_ += 4;
                        }
                    }
                }
                break;

            default:
                if (TextWork::StringStartsWith(AnsiParams[i_], "38:2:"))
                {
                    XList<std::string> AnsiParamsX;
                    TextWork::StringSplit(AnsiParams[i_], ':', AnsiParamsX);
                    if (AnsiParamsX.Count >= 5)
                    {
                        int TempR = TextWork::StrToInt(AnsiParamsX[2], -1);
                        int TempG = TextWork::StrToInt(AnsiParamsX[3], -1);
                        int TempB = TextWork::StrToInt(AnsiParamsX[4], -1);
                        AnsiState_.__AnsiFore = AnsiColor16(TempR, TempG, TempB);
                    }
                }
                if (TextWork::StringStartsWith(AnsiParams[i_], "48:2:"))
                {
                    XList<std::string> AnsiParamsX;
                    TextWork::StringSplit(AnsiParams[i_], ':', AnsiParamsX);
                    if (AnsiParamsX.Count >= 5)
                    {
                        int TempR = TextWork::StrToInt(AnsiParamsX[2], -1);
                        int TempG = TextWork::StrToInt(AnsiParamsX[3], -1);
                        int TempB = TextWork::StrToInt(AnsiParamsX[4], -1);
                        AnsiState_.__AnsiBack = AnsiColor16(TempR, TempG, TempB);
                    }
                }
                if (TextWork::StringStartsWith(AnsiParams[i_], "38:5:"))
                {
                    XList<std::string> AnsiParamsX;
                    TextWork::StringSplit(AnsiParams[i_], ':', AnsiParamsX);
                    if (AnsiParamsX.Count >= 3)
                    {
                        AnsiState_.__AnsiFore = Color256[TextWork::StrToInt(AnsiParamsX[2], 256)];
                    }
                }
                if (TextWork::StringStartsWith(AnsiParams[i_], "48:5:"))
                {
                    XList<std::string> AnsiParamsX;
                    TextWork::StringSplit(AnsiParams[i_], ':', AnsiParamsX);
                    if (AnsiParamsX.Count >= 3)
                    {
                        AnsiState_.__AnsiBack = Color256[TextWork::StrToInt(AnsiParamsX[2], 256)];
                    }
                }
                break;
        }
    }
}

int CoreAnsi::AnsiProcessGetXMin(bool Origin)
{
    if (((!Origin) || AnsiState_.__AnsiOrigin) && AnsiState_.__AnsiMarginLeftRight)
    {
        return AnsiState_.__AnsiMarginLeft;
    }
    else
    {
        return 0;
    }
}

int CoreAnsi::AnsiProcessGetXMax(bool Origin)
{
    if (((!Origin) || AnsiState_.__AnsiOrigin) && AnsiState_.__AnsiMarginLeftRight)
    {
        return AnsiState_.__AnsiMarginRight;
    }
    else
    {
        return AnsiMaxX;
    }
}

void CoreAnsi::AnsiRepaintCursor()
{
    if (AnsiGetFontSize(AnsiState_.__AnsiY) > 0)
    {
        Screen::ScreenCursorMove(AnsiState_.__AnsiX << 1, AnsiState_.__AnsiY + ScreenOffset, 1);
    }
    else
    {
        Screen::ScreenCursorMove(AnsiState_.__AnsiX, AnsiState_.__AnsiY + ScreenOffset, 0);
    }
    Screen::ScreenRefresh();
}

void CoreAnsi::AnsiCharPrint(int TextFileLine_i)
{
    if (AnsiState_.StatusBar)
    {
        return;
    }

    int TextFileLine_i_GetChar = TextFileLine_i;
    if ((!ANSIDOS_) && (UseAnsiCommands))
    {
        if (AnsiState_.__AnsiVT52)
        {
            if (AnsiState_.VT52_SemigraphDef)
            {
                if ((TextFileLine_i >= 95) && (TextFileLine_i <= 126))
                {
                    TextFileLine_i_GetChar = VT52_SemigraphChars[TextFileLine_i - 95];
                }
            }
        }
        else
        {
            TextFileLine_i_GetChar = AnsiState_.GetChar(TextFileLine_i);
        }

        if (TextFileLine_i_GetChar == 127)
        {
            return;
        }
    }

    if ((ANSIDOS_) && (!AnsiState_.__AnsiMusic) && ((TextFileLine_i < 32) || (TextFileLine_i == 127)))
    {
        switch (TextFileLine_i)
        {
            case 13: // CR
            case 10: // LF
                break;
            case 26: // SUB - End of file
                if (!UseAnsiCommands)
                {
                    TextFileLine_i_GetChar = DosControl[TextFileLine_i];
                }
                break;
            case 8: // BS
                if (ANSIPrintBackspace)
                {
                    TextFileLine_i_GetChar = DosControl[TextFileLine_i];
                }
                break;
            case 9: // HT
                if (ANSIPrintTab)
                {
                    TextFileLine_i_GetChar = DosControl[TextFileLine_i];
                }
                break;
            case 127:
                TextFileLine_i_GetChar = DosControl[32];
                break;
            default:
                TextFileLine_i_GetChar = DosControl[TextFileLine_i];
                break;
        }
    }
    if ((TextFileLine_i_GetChar >= 32))
    {
        TextFileLine_i = TextFileLine_i_GetChar;
        if (!AnsiState_.__AnsiMusic)
        {
            int TextFileLine_i_dbl = Screen::CharDouble(TextFileLine_i);
            if (ANSIDOS_)
            {
                AnsiCharPrintLast = TextFileLine_i;
                AnsiCharFI(AnsiState_.__AnsiX, AnsiState_.__AnsiY, TextFileLine_i, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                AnsiState_.__AnsiX++;
                if ((TextFileLine_i_dbl != 0) && (AnsiState_.__AnsiX < AnsiProcessGetXMax(false)))
                {
                    AnsiCharFI(AnsiState_.__AnsiX, AnsiState_.__AnsiY, TextFileLine_i_dbl, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                    AnsiState_.__AnsiX++;
                }
                if ((AnsiMaxX > 0) && (AnsiState_.__AnsiX >= AnsiProcessGetXMax(false)))
                {
                    if (AnsiState_.__AnsiNoWrap)
                    {
                        AnsiState_.__AnsiX--;
                    }
                    else
                    {
                        AnsiState_.__AnsiX = AnsiProcessGetXMin(false);
                        AnsiState_.__AnsiY++;
                        AnsiSeekLine();
                        if ((AnsiMaxY > 0) && (AnsiState_.__AnsiY > AnsiState_.__AnsiScrollLast))
                        {
                            int L = AnsiState_.__AnsiY - AnsiState_.__AnsiScrollLast;
                            AnsiState_.__AnsiY = AnsiState_.__AnsiScrollLast;
                            AnsiScrollInit(L, AnsiState::AnsiScrollCommandDef::None);
                        }
                    }
                }
            }
            else
            {
                bool CharNoScroll = true;
                if (AnsiState_.__AnsiWrapFlag)
                {
                    if (!AnsiState_.__AnsiNoWrap)
                    {
                        AnsiState_.__AnsiX = AnsiProcessGetXMin(false);
                        AnsiState_.__AnsiY++;
                        AnsiSeekLine();
                        if ((AnsiMaxY > 0) && (AnsiState_.__AnsiY > AnsiState_.__AnsiScrollLast))
                        {
                            int L = AnsiState_.__AnsiY - AnsiState_.__AnsiScrollLast;
                            AnsiState_.__AnsiY = AnsiState_.__AnsiScrollLast;
                            AnsiScrollInit(L, AnsiState::AnsiScrollCommandDef::Char, TextFileLine_i, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                            CharNoScroll = false;
                        }
                    }
                    AnsiState_.__AnsiWrapFlag = false;
                }
                if (CharNoScroll)
                {
                    AnsiCharPrintLast = TextFileLine_i;
                    AnsiCharFI(AnsiState_.__AnsiX, AnsiState_.__AnsiY, TextFileLine_i, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                    AnsiState_.__AnsiX++;
                    if ((TextFileLine_i_dbl != 0) && (AnsiState_.__AnsiX < AnsiProcessGetXMax(false)))
                    {
                        AnsiCharFI(AnsiState_.__AnsiX, AnsiState_.__AnsiY, TextFileLine_i_dbl, AnsiState_.__AnsiBack, AnsiState_.__AnsiFore, AnsiState_.__AnsiAttr);
                        AnsiState_.__AnsiX++;
                    }
                    if ((AnsiMaxX > 0) && (AnsiState_.__AnsiX >= AnsiProcessGetXMax(false)))
                    {
                        if (!AnsiState_.__AnsiNoWrap)
                        {
                            AnsiState_.__AnsiWrapFlag = true;
                        }
                        AnsiState_.__AnsiX--;
                    }
                }
            }
        }
    }
    else
    {
        if (AnsiState_.__AnsiMusic)
        {
            switch (TextFileLine_i)
            {
                case 14: // SO - Exit from ANSI music
                    {
                        if (AnsiState_.__AnsiMusic)
                        {
                            AnsiState_.__AnsiMusic = false;
                        }
                    }
                    break;
            }
        }
        else
        {
            switch (TextFileLine_i)
            {
                case 5: // ENQ
                    if (!ANSIDOS_)
                    {
                        __AnsiResponse.Add("AnswerBack");
                    }
                    break;
                case 7: // BEL
                    if (!AnsiState_.__AnsiCommand)
                    {
                        AnsiState_.AnsiRingBellCount++;
                        if (AnsiRingBell)
                        {
                            Screen::Bell();
                        }
                    }
                    break;
                case 8: // BS
                    {
                        if (!ANSIPrintBackspace)
                        {
                            AnsiState_.__AnsiWrapFlag = false;
                            if (AnsiState_.__AnsiX >= AnsiProcessGetXMax(false))
                            {
                                AnsiState_.__AnsiX = AnsiProcessGetXMax(false) - 1;
                            }
                            if (AnsiState_.__AnsiX > AnsiProcessGetXMin(false))
                            {
                                AnsiState_.__AnsiX--;
                            }
                        }
                    }
                    break;
                case 9: // TAB
                    {
                        if (!ANSIPrintTab)
                        {
                            AnsiDoTab(1);
                        }
                    }
                    break;
                case 13: // CR
                    {
                        switch (ANSI_CR)
                        {
                            case 0:
                                AnsiState_.__AnsiWrapFlag = false;
                                AnsiState_.__AnsiX = AnsiProcessGetXMin(false);
                                break;
                            case 1:
                                AnsiState_.__AnsiWrapFlag = false;
                                AnsiState_.__AnsiX = AnsiProcessGetXMin(false);
                                AnsiSeekLine();
                                if (AnsiState_.__AnsiY == AnsiState_.__AnsiScrollLast)
                                {
                                    AnsiScrollInit(1, AnsiState::AnsiScrollCommandDef::None);
                                }
                                else
                                {
                                    AnsiState_.__AnsiY++;
                                }
                                break;
                        }
                    }
                    break;
                case 12: // FF - same as LF
                    if (!ANSIDOS_)
                    {
                        switch (ANSI_LF)
                        {
                            case 0:
                                AnsiState_.__AnsiWrapFlag = false;
                                AnsiSeekLine();
                                if (AnsiState_.__AnsiY == AnsiState_.__AnsiScrollLast)
                                {
                                    AnsiScrollInit(1, AnsiState::AnsiScrollCommandDef::None);
                                }
                                else
                                {
                                    AnsiState_.__AnsiY++;
                                }
                                break;
                            case 1:
                                AnsiState_.__AnsiWrapFlag = false;
                                AnsiState_.__AnsiX = AnsiProcessGetXMin(false);
                                AnsiSeekLine();
                                if (AnsiState_.__AnsiY == AnsiState_.__AnsiScrollLast)
                                {
                                    AnsiScrollInit(1, AnsiState::AnsiScrollCommandDef::None);
                                }
                                else
                                {
                                    AnsiState_.__AnsiY++;
                                }
                                break;
                        }
                    }
                    break;
                case 10: // LF
                    {
                        switch (ANSI_LF)
                        {
                            case 0:
                                AnsiState_.__AnsiWrapFlag = false;
                                AnsiSeekLine();
                                if (AnsiState_.__AnsiY == AnsiState_.__AnsiScrollLast)
                                {
                                    AnsiScrollInit(1, AnsiState::AnsiScrollCommandDef::None);
                                }
                                else
                                {
                                    AnsiState_.__AnsiY++;
                                }
                                break;
                            case 1:
                                AnsiState_.__AnsiWrapFlag = false;
                                AnsiState_.__AnsiX = AnsiProcessGetXMin(false);
                                AnsiSeekLine();
                                if (AnsiState_.__AnsiY == AnsiState_.__AnsiScrollLast)
                                {
                                    AnsiScrollInit(1, AnsiState::AnsiScrollCommandDef::None);
                                }
                                else
                                {
                                    AnsiState_.__AnsiY++;
                                }
                                break;
                        }
                    }
                    break;
                case 11: // VT - same as LF
                    if (!ANSIDOS_)
                    {
                        AnsiState_.__AnsiWrapFlag = false;
                        AnsiSeekLine();
                        if (AnsiState_.__AnsiY == AnsiState_.__AnsiScrollLast)
                        {
                            AnsiScrollInit(1, AnsiState::AnsiScrollCommandDef::None);
                        }
                        else
                        {
                            AnsiState_.__AnsiY++;
                        }
                    }
                    break;
                case 14: // SO, LS1
                    if (!ANSIDOS_)
                    {
                        AnsiState_.CharMapNumGL = 1;
                    }
                    break;
                case 15: // SI, LS0
                    if (!ANSIDOS_)
                    {
                        AnsiState_.CharMapNumGL = 0;
                    }
                    break;
                case 26: // SUB - End of file
                    break;
            }
        }
    }
}
