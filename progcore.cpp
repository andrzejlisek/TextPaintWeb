#include "progcommon.cpp"

#include "prog/xlist.h"
#include "prog/configfile.h"
#include "prog/core0editor.h"
#include "prog/core1player.h"
#include "prog/core2terminal.h"
#include "prog/core3keys.h"
#include "prog/core4command.h"
#include "prog/core9menu.h"
#include "prog/binaryfile.h"

void RespondClear()
{
    BufClear();
    BufTxt("_ProgCallback([");
}

void RespondFinish()
{
    BufTxt("0]);");
    BufChr(0);
    emscripten_worker_respond(IOBuf, IOBufPtr);
}

void RespondPartial()
{
    if (BufCheck())
    {
        BufTxt("0]);");
        BufChr(0);
        emscripten_worker_respond_provisionally(IOBuf, IOBufPtr);
        RespondClear();
    }
}

std::shared_ptr<ConfigFile> CF;

void FileConfig(std::string Param)
{
    BufNum(113);
    BufTxt("\"");
    BufStr(Param);
    BufTxt("\",\"");
    BufStr(CF.get()->ParamGetS(Param));
    BufTxt("\",");
    RespondPartial();
}

void FileConfigVal(std::string Param, std::string Value)
{
    BufNum(113);
    BufTxt("\"");
    BufStr(Param);
    BufTxt("\",\"");
    BufStr(Value);
    BufTxt("\",");
    RespondPartial();
}

void ScreenChar(int X, int Y, int Chr, int Back, int Fore, int Attr, int FontW, int FontH)
{
    BufNum(101);
    BufNum(X);
    BufNum(Y);
    BufNum(Chr);
    BufNum(Back);
    BufNum(Fore);
    BufNum(Attr);
    BufNum(FontW);
    BufNum(FontH);
    RespondPartial();
}

void ScreenClear(int ColorBack, int ColorFore)
{
    BufNum(102);
    BufNum(ColorBack);
    BufNum(ColorFore);
    RespondPartial();
}

void ScreenResize(int NewW, int NewH)
{
    BufNum(103);
    BufNum(NewW);
    BufNum(NewH);
    RespondPartial();
}

void ScreenCursorMove(int X, int Y, int Opt)
{
    BufNum(104);
    BufNum(X);
    BufNum(Y);
    BufNum(Opt);
    RespondPartial();
}

void ScreenTextMove(int X1, int Y1, int X2, int Y2, int W, int H)
{
    BufNum(105);
    BufNum(X1);
    BufNum(Y1);
    BufNum(X2);
    BufNum(Y2);
    BufNum(W);
    BufNum(H);
    RespondPartial();
}

void ScreenLineOffset(int Y, int Offset, int Blank, int ColorBack, int ColorFore, int ColorAttr)
{
    BufNum(106);
    BufNum(Y);
    BufNum(Offset);
    BufNum(Blank);
    BufNum(ColorBack);
    BufNum(ColorFore);
    BufNum(ColorAttr);
    RespondPartial();
}

void ScreenOther(int Param)
{
    BufNum(107);
    BufNum(Param);
    RespondPartial();
}

void ScreenOtherString(int Param, std::string ParamStr)
{
    BufNum(108);
    BufNum(Param);
    BufTxt("\"");
    BufStr(ParamStr);
    BufTxt("\",");
    RespondPartial();
}

void MouseHighlight(int Work, int X, int Y, int F, int L)
{
    BufNum(203);
    BufNum(Work);
    BufNum(X);
    BufNum(Y);
    BufNum(F);
    BufNum(L);
    RespondPartial();
}

void ScreenSetConfig()
{
    FileConfig("DisplayBlink");
    FileConfig("DisplayAttrib");
    FileConfig("DisplayInterpolate");

    FileConfig("ANSIColors");
    FileConfig("ANSIReverseMode");
    FileConfig("ANSIColorBlink");
    FileConfig("ANSIColorBold");
    FileConfig("ANSIIgnoreConcealed");

    FileConfig("ANSIDOS");
    FileConfig("ANSI8bit");
    FileConfig("ANSIPrintBackspace");
    FileConfig("ANSIPrintTab");
    FileConfig("ANSIReadCR");
    FileConfig("ANSIReadLF");

    FileConfig("ColorBlending");
    FileConfig("ColorBlendingGamma");

    FileConfig("BellVolume");
    FileConfig("BellFrequency");
    FileConfig("BellDuration");
    
    FileConfig("FontSelect");
    FileConfig("PaletteSelect");

    FileConfig("TimerStep");
    FileConfig("TimerStepText");
    FileConfig("TimerStepChange");
    FileConfig("ANSIScrollChars");
    FileConfig("ANSIScrollCharsChange");
    FileConfig("ANSIScrollSmooth");

    FileConfig("DisplayCursor");
    FileConfig("WinAuto");
}

void WorkerSend(int T, std::string X)
{
    BufNum(200 + T);
    BufTxt("\"");
    BufTxt(X.c_str());
    BufTxt("\",");
    RespondPartial();
}

void FileImport(int Id, int Kind, std::string Name, std::string Attrib)
{
    BufNum(111);
    BufNum(Id);
    BufNum(Kind);
    BufTxt("\"");
    BufStr(Name);
    BufTxt("\",\"");
    BufStr(Attrib);
    BufTxt("\",");
    RespondPartial();
}

void FileExport(int Id, int Kind, std::string Name, std::string Attrib, std::string Data)
{
    BufNum(112);
    BufNum(Id);
    BufNum(Kind);
    BufTxt("\"");
    BufStr(Name);
    BufTxt("\",\"");
    BufStr(Attrib);
    BufTxt("\",\"");
    BufStr(Data);
    BufTxt("\",");
    RespondPartial();
}

std::unique_ptr<CoreCommon> Core;
std::shared_ptr<BinaryFile> BinaryFile_;

extern "C"
{
    // 1 - First init
    // 2 - Waiting for "config.txt"
    // 3 - Waiting for "system.txt"
    // 4 - Program run
    int InitState = 0;


    bool ProgMenu = true;

    void Init0()
    {
        // Creating environment
        ProgMenu = false;
        switch (CF.get()->ParamGetI("WorkMode"))
        {
            case 0:
                Core = std::make_unique<Core0Editor>();
                break;
            case 1:
                Core = std::make_unique<Core1Player>();
                break;
            case 2:
                Core = std::make_unique<Core2Terminal>();
                break;
            case 3:
                Core = std::make_unique<Core3Keys>();
                break;
            case 4:
                Core = std::make_unique<Core4Command>();
                break;
            case 9:
                Core = std::make_unique<Core9Menu>();
                ProgMenu = true;
                break;
        }
        Core.get()->BinaryFile_ = BinaryFile_;
        Core.get()->CF = CF;
        Core.get()->InitCommon();
        Core.get()->Init();
        Core.get()->EventOther("Resize", "", CF.get()->ParamGetI("WinW"), CF.get()->ParamGetI("WinH"), 0, 0);
    }

    EMSCRIPTEN_KEEPALIVE
    void Init(char * data, int size)
    {
        int InitMode = data[0];

        int Temp = 0;
        switch (InitMode)
        {
            case '1': // Create structures and execute "config.txt" import
                Screen::ScreenChar_ = ScreenChar;
                Screen::ScreenClear_ = ScreenClear;
                Screen::ScreenResize_ = ScreenResize;
                Screen::ScreenCursorMove_ = ScreenCursorMove;
                Screen::ScreenTextMove_ = ScreenTextMove;
                Screen::ScreenLineOffset_ = ScreenLineOffset;
                Screen::ScreenSetConfig = ScreenSetConfig;
                Screen::WorkerSend = WorkerSend;
                Screen::FileImport_ = FileImport;
                Screen::FileExport_ = FileExport;
                Screen::ScreenOther = ScreenOther;
                Screen::ScreenOtherString = ScreenOtherString;
                Screen::MouseHighlight = MouseHighlight;

                BufInit();
                RespondClear();

                CF = std::make_shared<ConfigFile>();
                BinaryFile_ = std::make_shared<BinaryFile>();

                BinaryFile_.get()->FileImportSys();
                BufNum(98);
                RespondFinish();
                break;
            case '2': // After importing "config.txt", prepare configuration structure for use in JavaScript
                if (BinaryFile_.get()->FileImportWaiting)
                {
                    RespondClear();
                    BufNum(98);
                    RespondFinish();
                }
                else
                {
                    CF.get()->ParamClear();
                    CF.get()->FileLoad(0, BinaryFile_.get()->LoadToStringConfig());
                    BinaryFile_.get()->Init(CF);

                    int TempWinW = Screen::DefaultW(CF.get()->ParamGetI("WinW"), CF.get()->ParamGetI("ANSIDOS"));
                    int TempWinH = Screen::DefaultH(CF.get()->ParamGetI("WinH"), CF.get()->ParamGetI("ANSIDOS"));
                    
                    CF.get()->ParamSet("WinW", TempWinW);
                    CF.get()->ParamSet("WinH", TempWinH);

                    RespondClear();
                    FileConfig("WebSocket");
                    FileConfig("WinTouchScreen");
                    FileConfig("ColorKeyboard");
                    FileConfig("FontName1");
                    FileConfig("FontName2");
                    FileConfig("FontChars");
                    FileConfig("FontMode");
                    FileConfig("TimerFrames");
                    FileConfig("TimerCursor");
                    FileConfig("TimerBlink");
                    FileConfig("TimerTick");
                    FileConfig("WinScreenSize");
                    FileConfig("WinKeyboardSize");
                    {
                        int I_ = 1;
                        while (CF.get()->ParamExists("ColorBlending_" + std::to_string(I_)))
                        {
                            FileConfig("ColorBlending_" + std::to_string(I_));
                            I_++;
                        }
                        

                        I_ = 1;
                        std::string XItem = "File1";
                        while (CF.get()->ParamExists(XItem))
                        {
                            FileConfig(XItem);
                            I_++;
                            XItem = "File" + std::to_string(I_);
                        }
                        FileConfigVal("FileN", std::to_string(I_));

                        I_ = 1;
                        XItem = "Font1";
                        while (CF.get()->ParamExists(XItem + "Name"))
                        {
                            FileConfig(XItem + "File1");
                            FileConfig(XItem + "File2");
                            FileConfig(XItem + "File3");
                            I_++;
                            XItem = "Font" + std::to_string(I_);
                        }
                        FileConfigVal("FontN", std::to_string(I_));

                        I_ = 1;
                        XItem = "Palette1Colors";
                        while (CF.get()->ParamExists(XItem))
                        {
                            FileConfig(XItem);
                            I_++;
                            XItem = "Palette" + std::to_string(I_) + "Colors";
                        }
                        FileConfigVal("PaletteN", std::to_string(I_));
                    }
                    FileConfig("PaletteR");
                    FileConfig("PaletteG");
                    FileConfig("PaletteB");

                    ScreenSetConfig();
                    BufNum(99);
                    RespondFinish();
                }
                break;
                
            case '3': // Execute program

                CF.get()->ParamSet("WorkMode", "9");

                RespondClear();

                // Create double font table
                Screen::StaticInit(CF);

                // Execute program
                Init0();

                BufNum(100);
                RespondFinish();
                break;
        }
    }

    EMSCRIPTEN_KEEPALIVE
    void EventTick(char * data, int size)
    {
        RespondClear();
        Core.get()->EventTick();
        BufNum(999);
        RespondFinish();
    }

    EMSCRIPTEN_KEEPALIVE
    void EventKey(char * data, int size)
    {
        BufSet(data);
    
        std::string KeyName = BufGetStr();
        int KeyChar = BufGetNum();
        bool ModShift = (BufGetNum() == 1);
        bool ModCtrl = (BufGetNum() == 1);
        bool ModAlt = (BufGetNum() == 1);

        RespondClear();
    
        Core.get()->EventKey(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
        if (Core.get()->AppExit)
        {
            CF.get()->ParamSet("WorkMode", 9);
            Init0();
        }
        else
        {
            if (ProgMenu)
            {
                int Opt = ((Core9Menu*)Core.get())->Opt;
                CF.get()->ParamSet("WorkMode", Opt);
                Init0();
            }
        }

        RespondFinish();
    }

    EMSCRIPTEN_KEEPALIVE
    void EventOther(char * data, int size)
    {
        BufSet(data);
        std::string EvtName = BufGetStr();
        std::string EvtParam0 = BufGetStr();
        int EvtParam1 = BufGetNum();
        int EvtParam2 = BufGetNum();
        int EvtParam3 = BufGetNum();
        int EvtParam4 = BufGetNum();

        RespondClear();
        Core.get()->EventOther(EvtName, EvtParam0, EvtParam1, EvtParam2, EvtParam3, EvtParam4);
        RespondFinish();
    }

    EMSCRIPTEN_KEEPALIVE
    void EventOtherFile(char * data, int size)
    {
        BufSet(data);
        std::string EvtName = BufGetStr();
        std::string EvtParam0 = BufGetStr();
        int EvtParam1 = BufGetNum();
        int EvtParam2 = BufGetNum();
        int EvtParam3 = BufGetNum();
        int EvtParam4 = BufGetNum();

        RespondClear();
        bool Fire = true;
        if ((EvtName == "FileImport") || (EvtName == "FileExport"))
        {
            Fire = BinaryFile_.get()->EventFile(EvtName, EvtParam0, EvtParam1, EvtParam2, EvtParam3, EvtParam4);
        }
        if (EvtName == "ConfigSet")
        {
            std::string _Param = EvtParam0.substr(0, EvtParam1);
            std::string _Value = EvtParam0.substr(EvtParam1 + 1);
            CF.get()->ParamSet(_Param, _Value);
            if (EvtParam2 == 1)
            {
                BinaryFile_.get()->SaveFromStringConfig(CF.get()->FileSave(0));
                BinaryFile_.get()->SysSaveConfig();
            }
            if (Core)
            {
                Core.get()->UpdateConfig(CF);
                Core.get()->CoreAnsi_.get()->UpdateConfig(CF);
            }
            Fire = false;
        }
        if (EvtName == "ScreenFont")
        {
            Screen::UpdateFontParams(CF);
            Fire = false;
        }
        if (EvtName == "Resize")
        {
            if (EvtParam3 == 1)
            {
                CF.get()->ParamSet("WinW", EvtParam1);
                CF.get()->ParamSet("WinH", EvtParam2);
                BinaryFile_.get()->SaveFromStringConfig(CF.get()->FileSave(0));
                BinaryFile_.get()->SysSaveConfig();
            }
            Fire = true;
        }
        if (Fire)
        {
            Core.get()->EventOther(EvtName, EvtParam0, EvtParam1, EvtParam2, EvtParam3, EvtParam4);
        }
        RespondFinish();
   }
}
