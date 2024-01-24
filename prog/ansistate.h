#ifndef ANSISTATE_H
#define ANSISTATE_H

#include "xlist.h"
#include <string>
#include <sstream>
#include "ansilineoccupy.h"
#include "textwork.h"

class AnsiState
{
public:
    AnsiState();
    int TerminalW = 0;
    int TerminalH = 0;

    long __AnsiProcessStep = 0;
    long __AnsiProcessDelay = 0;
    long __AnsiProcessDelayMin = 1;
    long __AnsiProcessDelayMax = -1;


    bool __AnsiMusic = false;
    bool __AnsiUseEOF = false;
    bool __AnsiBeyondEOF = false;
    bool __AnsiNoWrap = false;

    int __AnsiBack = -1;
    int __AnsiFore = -1;
    int __AnsiAttr = 0;
    int __AnsiBack0 = -1;
    int __AnsiFore0 = -1;
    int __AnsiAttr0 = 0;

    int __AnsiFontSizeW = 0;
    int __AnsiFontSizeH = 0;

    int __AnsiX = 0;
    int __AnsiY = 0;
    int __AnsiX0 = 0;
    int __AnsiY0 = 0;
    bool __AnsiWrapFlag = false;
    bool __AnsiWrapFlag0 = false;

    bool StatusBar = false;
    bool CursorHide = false;

    int __AnsiBack_ = -1;
    int __AnsiFore_ = -1;
    int __AnsiAttr_ = 0;
    int __AnsiX_ = 0;
    int __AnsiY_ = 0;
    int __AnsiBack_0 = -1;
    int __AnsiFore_0 = -1;
    int __AnsiAttr_0 = 0;
    int __AnsiX_0 = 0;
    int __AnsiY_0 = 0;
    bool __AnsiWrapFlag_ = false;
    bool __AnsiWrapFlag_0 = false;

    bool DECCOLMPreserve = false;

    Str __AnsiCmd;

    XList<int> __AnsiTabs;

    std::string __AnsiDCS = "";
    bool __AnsiDCS_ = false;

    bool __AnsiVT52 = false;
    bool VT52_SemigraphDef = false;

    int CharMapNumGL = 0;
    int CharMapNumGR = 2;
    bool CharMapNRCS = false;

    int GetChar(int CharCode);

    int __AnsiScrollFirst = 0;
    int __AnsiScrollLast = 0;
    bool __AnsiOrigin = false;
    bool __AnsiMarginLeftRight = false;
    int __AnsiMarginLeft = 0;
    int __AnsiMarginRight = 0;

    bool __AnsiInsertMode = false;

    int __AnsiCounter = 0;

    bool __AnsiCommand = false;
    bool __AnsiSmoothScroll = false;

    XList<int> __AnsiFontSizeAttr;

    AnsiLineOccupy __AnsiLineOccupy__;
    AnsiLineOccupy __AnsiLineOccupy1__;
    AnsiLineOccupy __AnsiLineOccupy2__;

    AnsiLineOccupy __AnsiLineOccupy__0;
    AnsiLineOccupy __AnsiLineOccupy1__0;
    AnsiLineOccupy __AnsiLineOccupy2__0;

    enum AnsiScrollCommandDef { None, Char, FirstLast, Tab };
    int AnsiScrollProcessBlock = 0;
    bool AnsiScrollZeroOffset = true;
    bool AnsiScrollSeekSave = false;

    XList<int> AnsiScrollQueue_Counter;
    XList<int> AnsiScrollQueue_Lines;
    XList<AnsiScrollCommandDef> AnsiScrollQueue_Command;
    XList<int> AnsiScrollQueue_Param1;
    XList<int> AnsiScrollQueue_Param2;
    XList<int> AnsiScrollQueue_Param3;
    XList<int> AnsiScrollQueue_Param4;
    XList<int> AnsiScrollQueue_First;
    XList<int> AnsiScrollQueue_Last;


    int AnsiBufferI = 0;

    int ScrollLastOffset = 0;
    int ScrollLastOffsetFirst = 2;
    int ScrollLastOffsetLast = 1;
    int AnsiRingBellCount = 0;

    int PrintCharCounter = 0;
    int PrintCharCounterOver = 0;
    int PrintCharInsDel = 0;
    int PrintCharScroll = 0;

    bool ProcessBackgroundChars = false;

    XList<XList<int>> CharProtection1;
    XList<XList<int>> CharProtection2;
    bool CharProtection1Print = false;
    bool CharProtection2Print = false;
    bool CharProtection2Ommit = false;
    void CharProtection1Set(int X, int Y, bool V);
    void CharProtection2Set(int X, int Y, bool V);
    bool CharProtection1Get(int X, int Y);
    bool CharProtection2Get(int X, int Y);

    bool IsScreenAlternate = false;
    void ScreenAlte();
    void ScreenMain();
    void CursorSave();
    void CursorLoad();
    void DecParamSet(int N, int V);
    int DecParamGet(int N);
    void AnsiParamSet(int N, int V);
    int AnsiParamGet(int N);
    void DecParamSet(std::string S, int V);
    int DecParamGet(std::string S);
    void AnsiParamSet(std::string S, int V);
    int AnsiParamGet(std::string S);
    void Zero(bool __AnsiUseEOF_);
    void Reset(int AnsiMaxX, int AnsiMaxY, int NormalB, int NormalF, int ANSIDOS);
    void RefreshCharMaps();
    void SetCharMap(int Num, std::string MapId);
    Str GetScreen(int X1, int Y1, int X2, int Y2);

    int ReportCursorX();
    int ReportCursorY();

    static void Copy(AnsiState &Src, AnsiState &Dst);
    AnsiState Clone();
private:
    void CharMapSwap(int N);

    static void CopyListStr(Str &Src, Str &Dst);
    static void CopyListInt(XList<int> &Src, XList<int> &Dst);
    static void CopyListList(XList<XList<int>> &Src, XList<XList<int>> &Dst);

    std::string CharMapName[4];
    std::string CharMapName0[4];
    std::string CharMapName_[4];
    std::string CharMapName_0[4];
    int CharMap[4][128];
    int CharMap0[4][128];
    int CharMap_[4][128];
    int CharMap_0[4][128];
    int CharMapNumGL0 = 0;
    int CharMapNumGL_ = 0;
    int CharMapNumGL_0 = 0;
    int CharMapNumGR0 = 2;
    int CharMapNumGR_ = 2;
    int CharMapNumGR_0 = 2;

    XList<int> DecParamI;
    XList<int> DecParamV;
};

#endif // ANSISTATE_H
