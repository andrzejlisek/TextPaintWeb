#ifndef COREANSI_H
#define COREANSI_H

#include <memory>
#include <map>
#include "str.h"
#include "configfile.h"
#include "textwork.h"
#include "ansistate.h"
#include "screen.h"
#include "corestatic.h"

class CoreAnsi
{
public:
    CoreAnsi(std::shared_ptr<ConfigFile> CF);

    void UpdateConfig(std::shared_ptr<ConfigFile> CF);

    int AnsiMaxX = 80;
    int AnsiMaxY = 24;

    bool AnsiRingBell = true;

    XList<std::string> __AnsiResponse;

    int ANSIDOS = 0;
    bool ANSIDOS_ = false;
    bool ANSIPrintBackspace = false;
    bool ANSIPrintTab = false;

    bool ANSI8bit = false;

    // 0 - Do not change
    // 1 - Use as CRLF
    // 2 - Ommit
    int ANSI_CR = 0;
    int ANSI_LF = 0;

    long __AnsiProcessDelayFactor = 0;

    Str AnsiBuffer;

    AnsiState AnsiState_;

    int __ScreenMinX = 0;
    int __ScreenMinY = 0;
    int __ScreenMaxX = 0;
    int __ScreenMaxY = 0;
    void AnsiRepaint(bool AdditionalBuffers);
    void AnsiRepaintLine(int Y);

    AnsiLineOccupyItem AnsiGetLast;
    void AnsiGetF(int X, int Y);
    void AnsiGet(int X, int Y);
    void AnsiCharFUnprotected1(int X, int Y, int Ch);
    void AnsiCharFUnprotected2(int X, int Y, int Ch);
    void AnsiCharF(int X, int Y, int Ch);
    void AnsiCharFI(int X, int Y, int Ch, int ColB, int ColF, int FonA);
    void AnsiCharF(int X, int Y, int Ch, int ColB, int ColF, int FonA);
    void AnsiCharUnprotected1(int X, int Y, int Ch);
    void AnsiCharUnprotected2(int X, int Y, int Ch);
    void AnsiChar(int X, int Y, int Ch);
    void AnsiChar(int X, int Y, int Ch, int ColB, int ColF, int FontW, int FontH, int ColA);
    void AnsiScreenNegative(bool IsNega);

    void AnsiProcessReset(bool __AnsiUseEOF_, bool AnsiScreenWork_, int SeekMode_, int AnsiOptions);
    void AnsiProcessSupply(Str TextFileLine);
    void AnsiTerminalReset();
    bool AnsiTerminalResize(int NewW, int NewH, int ScreenStatusBar_);

    int AnsiProcess(int ProcessCount);
    void AnsiResize(int NewW, int NewH);

    bool AnsiSeek(int StepCount);
    void AnsiRepaintCursor();
private:

    bool UseAnsiCommands = true;
    int ScreenStatusBar = 0;
    int ScreenOffset = 0;

    bool AnsiScreenWork = true;
    Str CommandEndChar;

    // Printable character replacement for standard DOS character from 00h to 1Fh
    int DosControl[33];

    // Color subsitution from 256-color palette - filled in in constructor
    int Color256[257];

    int VT52_SemigraphChars[32];

    bool __AnsiLineOccupy1_Use = false;
    bool __AnsiLineOccupy2_Use = false;

    int ANSIScrollChars = 0;
    int ANSIScrollSmooth = 0;

    int ColorThresholdBlackWhite = 48;
    int ColorThresholdGray = 20;

    std::map<int, int> AnsiColor16_;

    int AnsiColor16(int R, int G, int B);

    XList<int> TempMemo;
    int AnsiGetFontW(int N, int X);
    int AnsiGetFontH(int N);
    int AnsiGetFontSize(int N);
    void AnsiClearFontSize(int FromPos);
    void AnsiSetFontSize(int N, int V, bool Rearrange);
    void AnsiAttributesSave();
    void AnsiAttributesLoad();


    // 0 - No seek
    // 1 - Clear and save state
    // 2 - Save state without clear
    int SeekMode = 0;

    int SeekPeriod = 1023;
    int SeekPeriod0 = 2048;

    XList<AnsiState> SeekState;

    long SeekStateSaveLast = -1;

    bool SeekStateSaveRequest = false;

    void SeekStateSave(bool Instant);

    void SetProcessDelay(long TimeStamp);
    void AnsiProcess_VT52();
    void AnsiProcess_Fixed(int TextFileLine_i);
    void AnsiProcess_CSI_Question(std::string AnsiCmd_);
    void AnsiProcess_CSI_Fixed(std::string AnsiCmd_);
    int AnsiProcess_Int0(std::string Param, std::string AnsiCmd_);
    int AnsiProcess_Int1(std::string Param, std::string AnsiCmd_);
    int AnsiProcess_Int11(std::string Param, std::string AnsiCmd_);
    void AnsiProcess_CSI(std::string AnsiCmd_);
    void AnsiProcess_CSI_m(XList<std::string> AnsiParams);
    int AnsiProcessGetXMin(bool Origin);
    int AnsiProcessGetXMax(bool Origin);
    void AnsiCharPrint(int TextFileLine_i);

    int AnsiCharPrintLast = -1;
    int AnsiCharPrintRepeater = 0;

    XList<int> AnsiScrollPosition;
    XList<int> AnsiScrollOffset;
    void AnsiScrollPrepare();
    void AnsiScrollInit(int Lines, AnsiState::AnsiScrollCommandDef Command);
    void AnsiScrollInit(int Lines, AnsiState::AnsiScrollCommandDef Command, int Param1, int Param2, int Param3, int Param4);
    void AnsiScrollFinish(AnsiState::AnsiScrollCommandDef Command, int Param1, int Param2, int Param3, int Param4);
    void AnsiScrollClear();
    int AnsiScrollProcess();
    bool AnsiScrollFinished = true;
    void AnsiScrollSetOffset(int First, int Last, int Offset);
    void AnsiScrollColumns(int Columns);
    void AnsiScrollLines(int Lines);
    void AnsiDoTab(int TabTimes);
};

#endif // COREANSI_H
