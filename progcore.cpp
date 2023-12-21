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
    BufTxt("_TestOK2([");
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

void ScreenCursorMove(int X, int Y)
{
    BufNum(104);
    BufNum(X);
    BufNum(Y);
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

void WorkerSend(int T, std::string X)
{
    BufNum(200 + T);
    BufTxt("\"");
    BufTxt(X.c_str());
    BufTxt("\",");
    RespondPartial();
}

void FileImport(int Id, int Kind, std::string Name)
{
    BufNum(111);
    BufNum(Id);
    BufNum(Kind);
    BufTxt("\"");
    BufStr(Name);
    BufTxt("\",");
    RespondPartial();
}

void FileExport(int Id, int Kind, std::string Name, std::string Data)
{
    BufNum(112);
    BufNum(Id);
    BufNum(Kind);
    BufTxt("\"");
    BufStr(Name);
    BufTxt("\",\"");
    BufStr(Data);
    BufTxt("\",");
    RespondPartial();
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

std::unique_ptr<CoreCommon> Core;
std::shared_ptr<BinaryFile> BinaryFile_;

extern "C"
{
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
                Screen::ScreenChar = ScreenChar;
                Screen::ScreenClear = ScreenClear;
                Screen::ScreenResize_ = ScreenResize;
                Screen::ScreenCursorMove_ = ScreenCursorMove;
                Screen::ScreenTextMove = ScreenTextMove;
                Screen::ScreenLineOffset = ScreenLineOffset;
                Screen::WorkerSend = WorkerSend;
                Screen::FileImport_ = FileImport;
                Screen::FileExport_ = FileExport;

                BufInit();
                RespondClear();
               
                BinaryFile_ = std::make_shared<BinaryFile>();
                CF = std::make_shared<ConfigFile>();

                BinaryFile_.get()->FileImportSys(0);
                BufNum(97);
                RespondFinish();
                break;
            case '2': // After importing "config.txt", execute "system.txt" import
                CF.get()->ParamClear();
                CF.get()->FileLoad(0, BinaryFile_.get()->LoadToString());
            
                RespondClear();
                BinaryFile_.get()->FileImportSys(1);
                BufNum(98);
                RespondFinish();
                break;
            case '3': // After importing both system files, prepare configuration structure for use in JavaScript
                CF.get()->FileLoad(1, BinaryFile_.get()->LoadToString());

                RespondClear();
                FileConfig("WinTouchScreen");
                FileConfig("ColorKeyboard");
                FileConfig("WinFontName");
                FileConfig("DuospaceFontName");
                FileConfig("DuospaceMode");
                FileConfig("DuospaceDoubleChars");
                FileConfig("WinTimer");
                FileConfig("WinScreenSize");
                FileConfig("WinKeyboardSize");
                FileConfig("WinSteadyCursor");
                FileConfig("WinColorBlending");
                {
                    int I_ = 1;
                    while (CF.get()->ParamExists("WinColorBlending_" + std::to_string(I_)))
                    {
                        FileConfig("WinColorBlending_" + std::to_string(I_));
                        I_++;
                    }
                }
                FileConfig("WinPaletteR");
                FileConfig("WinPaletteG");
                FileConfig("WinPaletteB");
                FileConfig("WinPaletteBlinkR");
                FileConfig("WinPaletteBlinkG");
                FileConfig("WinPaletteBlinkB");

                FileConfig("DisplayBlink");
                FileConfig("DisplayAttrib");
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


        // Default configuration
/*ANSIWidth=0
ANSIHeight=0
ANSIDOS=0
*/

        CF.get()->ParamSet("WinW", "80");
        CF.get()->ParamSet("WinH", "24");





                BufNum(99);
                RespondFinish();
                break;
                
            case '4': // Execute program

                CF.get()->ParamSet("WorkMode", "9");

                RespondClear();

                // Create double font table
                Screen::AddDoubleRange(-1, -1);
                Screen::StaticInit();

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

        Core.get()->EventOther(EvtName, EvtParam0, EvtParam1, EvtParam2, EvtParam3, EvtParam4);

        RespondClear();
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

        bool Fire = true;
        if (EvtName == "FileImport")
        {
            Fire = BinaryFile_.get()->EventFileImport(EvtParam1, EvtParam2, EvtParam0);
        }
        if (EvtName == "FileExport")
        {
            Fire = BinaryFile_.get()->EventFileExport(EvtParam1, EvtParam2);
        }
        RespondClear();
        if (EvtName == "ConfigSet")
        {
            std::string _Param = EvtParam0.substr(0, EvtParam1);
            std::string _Value = EvtParam0.substr(EvtParam1 + 1);
            CF.get()->ParamSet(_Param, _Value);
            if (EvtParam2 == 1)
            {
                BinaryFile_.get()->SaveFromString(CF.get()->FileSave(0));
                BinaryFile_.get()->SysSaveConfig();
            }
            Core.get()->CoreAnsi_.get()->UpdateConfig(CF);
            Fire = false;
        }
        if (EvtName == "Resize")
        {
            //ScreenResize(EvtParam1, EvtParam2);
            Fire = true;
        }
        if (Fire)
        {
            Core.get()->EventOther(EvtName, EvtParam0, EvtParam1, EvtParam2, EvtParam3, EvtParam4);
        }
        RespondFinish();
   }
}
