#ifndef CORECOMMON_H
#define CORECOMMON_H

#include <memory>
#include "configfile.h"
#include "str.h"
#include "textwork.h"
#include <iostream>
#include "textcodec.h"
#include "corestatic.h"
#include <algorithm>
#include "coreansi.h"
#include "clipboard.h"
#include "binaryfile.h"
#include "filemanager.h"
#include "displayconfig.h"
#include "stopwatch.h"

class CoreCommon
{
public:
    CoreCommon();
    std::shared_ptr<ConfigFile> CF;
    std::shared_ptr<CoreAnsi> CoreAnsi_;
    std::shared_ptr<DisplayConfig> DisplayConfig_;

    void ScreenChar0(int X, int Y, int Chr, int Back, int Fore, int Attr, int FontW, int FontH);
    void ScreenChar0(int X, int Y, int Chr, int Back, int Fore);
    virtual void Init();
    void InitCommon();
    virtual void EventTick();
    virtual void EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    virtual void EventOther(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4);
    int CharDoubleInv(int C);
    int CharDouble(int C);
    bool AppExit = false;
    std::shared_ptr<BinaryFile> BinaryFile_;
    FileManager FileManager_;
    void SaveConfig();
    void ScreenStatusBarSet(int ScreenStatusBar_);
protected:
    int ScreenStatusBar = 0;
    void ReadColor(std::string SettingValue, int &ColorB, int &ColorF);
    int ScreenW = 80;
    int ScreenH = 24;
    XList<int> TempMemoI;
    XList<bool> TempMemoB;
    int StatusBack = 15;
    int StatusFore = 0;
    int PopupBack = 0;
    int PopupFore = 15;

    XList<Str> ScreenWelcomeBuf;
    void Screen_WriteText(std::string Text);
    void Screen_WriteLine();
    void Screen_BackText();
    void Screen_Clear();
    void Screen_Refresh();

private:
    int BlankDoubleChar = (0x10FFFF + 1);
    int BlankDoubleCharVis = 32;
};

#endif // CORECOMMON_H
