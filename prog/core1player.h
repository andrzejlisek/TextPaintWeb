#ifndef CORE1PLAYER_H
#define CORE1PLAYER_H

#include "corecommon.h"
#include "ansisauce.h"
#include "stopwatch.h"

class Core1Player : public CoreCommon
{
public:
    Core1Player();
    void Init();
    void EventTick();
    void EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void EventOther(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4);
private:
    bool FileOpenAtEnd = false;

    void CalcFileDelayStep();

    void EventTickX();
    void Repaint(bool Force);
    bool TimerTick = true;
    bool LoadFileMeasuring = false;
    int LoadFileTimeFactor = 80;
    int LoadFileTimeChunk = 10;
    int LoadFileChunk = 10;
    int FilePos = 0;

    int DisplayTextMode = 0;

    Stopwatch FileTimeMeasure;

    int Time0;

    typedef std::chrono::time_point<std::chrono::high_resolution_clock> time_pt;
    time_pt Time1;
    time_pt Time2;
    time_pt Time3;
    time_pt Time4;

    enum WorkStateSDef { None, InfoScreen, InfoScreenWaitForKey, FileMan, FileMan0,
                         FileOpenFile0, FileOpenFile, FileOpen, FileOpenWait, FileOpenPrepare,
                         DisplayFwdStep, DisplayFwdPlay, DisplayRevStep, DisplayRevPlay, DisplayPause,
                         DisplayInfo, DispConf };
    WorkStateSDef WorkStateS = WorkStateSDef::InfoScreen;

    int ServerPort = 0;
    bool ServerTelnet = false;
    std::shared_ptr<TextCodec> ServerEncoding;

    int FileDelaySeek = 1000;

    Clipboard Clipboard_;
    Str FileCtX;

    std::string ANSIBrowseWildcard;

    long FileTimerOffset = 0;
    int ActionRequestParam = 0;
    int ActionRequest = 0;
    int WorkSeekAdvance = 1;
    bool WorkSeekOneChar = false;

    int MoviePos = 0;
    int MovieLength = 0;

    int InfoPosH = 0;
    int InfoPosV = 0;

    int DisplayStatus_ = 0;
    bool DisplayInfoRepaint = false;

    XList<int> NewFileName;

    int DisplayStatus = 0;
    bool DisplayPaused = false;

    bool DisplayConfigReload = false;

    bool DisplayConfig_RequestReapint = false;
    bool DisplayConfig_RequestMenu = false;
    bool DisplayConfig_RequestClose = false;

    void DisplayInfoText(bool Forced);

    bool FileOpenFile0Info = false;

    XList<int> LineList;
    int WorkSeekAdvanceCalc(int Dir);
};

#endif // CORE1PLAYER_H
