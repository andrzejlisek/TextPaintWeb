#ifndef SCREEN_H
#define SCREEN_H

#include "xlist.h"
#include "str.h"
#include <string>
#include <iostream>
#include "textwork.h"
#include "configfile.h"
#include <memory>

class Screen
{
public:
    Screen();
    static inline bool WinAuto = true;
    static void StaticInit(std::shared_ptr<ConfigFile> CF);
    static void UpdateFontParams(std::shared_ptr<ConfigFile> CF);
    static inline XList<int> BitmapPage;
    static int CharDouble(int C);
    static int CharDoubleInv(int C);
    static inline int TextNormalBack = 0;
    static inline int TextNormalFore = 7;
    static inline int TerminalCellW = 8;
    static inline int TerminalCellH = 16;
    static inline int TerminalCellW2 = 4;
    static inline int TerminalCellH2 = 8;

    static inline void (*ScreenSetConfig)( );
    static inline void (*ScreenClear_)( int, int );
    static inline void (*ScreenChar_)( int, int, int, int, int, int, int, int );
    static inline void (*ScreenResize_)( int, int );
    static inline void (*ScreenCursorMove_)( int, int );
    static inline void (*ScreenTextMove_)( int, int, int, int, int, int );
    static inline void (*ScreenLineOffset_)( int, int, int, int, int, int );
    static inline void (*WorkerSend)( int, std::string );
    static inline void (*ScreenOther)( int );
    static inline void (*ScreenOtherString)( int, std::string );
    static inline void (*MouseHighlight)( int, int, int, int, int );
    static void ScreenClear(int Back, int Fore);
    static void ScreenChar(int X, int Y, int C, int ColorBack, int ColorFore, int ColorAttr, int FontW, int FontH);
    static void ScreenTextMove(int X1, int Y1, int X2, int Y2, int W, int H);
    static void ScreenResize(int NewW, int NewH);
    static void ScreenCursorMove(int X, int Y);
    static void ScreenRefresh();
    static void ScreenLineOffset(int Y, int Offset, int Blank, int ColorBack, int ColorFore, int ColorAttr);

    static inline XList<int> ScreenMemo1;
    static inline XList<int> ScreenMemo2;

    static void ScreenWriteChar(int Chr, int Back, int Fore);
    static void ScreenWriteChar0(int Chr, int Back, int Fore);
    static void ScreenWriteText(Str Txt, int Back, int Fore);
    static void ScreenWriteText0(Str Txt, int Back, int Fore);
    static void ScreenWriteLine(int Back, int Fore);

    static void ScreenWriteChar(int Chr);
    static void ScreenWriteChar0(int Chr);
    static void ScreenWriteText(Str Txt);
    static void ScreenWriteText0(Str Txt);
    static void ScreenWriteLine();

    static inline int CurrentX = 0;
    static inline int CurrentY = 0;
    static inline int CurrentW = 1;
    static inline int CurrentH = 1;

    static void MouseActive(bool X);

    static inline int FilePortId = 0;
    static inline void (*FileImport_)( int, int, std::string, std::string );
    static inline void (*FileExport_)( int, int, std::string, std::string, std::string );
    static int FileImport(int Kind, std::string Name, std::string Attrib);
    static int FileExport(int Kind, std::string Name, std::string Attrib, std::string Data);

    static int DefaultW(int ScrW, int ANSIDOS);
    static int DefaultH(int ScrH, int ANSIDOS);

    static inline constexpr int UnicodeCount = 0x110000;

    static void Bell();
    static void CursorHide(bool Hide);

    static inline int FontListSelect = 0;
    static inline int FontListMode = 0;
    static inline XList<std::string> FontListName;
    static inline XList<std::string> FontListFile1;
    static inline XList<std::string> FontListFile2;
    static inline XList<std::string> FontListFile3;

    static inline int PaletteListSelect = 0;
    static inline XList<std::string> PaletteListName;
    static inline XList<std::string> PaletteListColors;

    static void ResetCustomPaletteFont();
    static void SetPalette(std::string PaletteColors);
    static void SetFont(std::string FontFile1, std::string FontFile2, std::string FontFile3, int FontMode);
    static void SetCustomFont(int Size);
    static void SetCustomChar(std::string Data);
    static void SetPalette();
    static void SetFont();

    static inline std::map<int, int> FontSingleChar;
private:
    static inline XList<int> Range1;
    static inline XList<int> Range2;
    static inline bool CursorHideState = false;
    static inline bool FontSinglePage = false;

    struct ScreenLineOffsetDef
    {
        bool NeedSet = false;
        int Offset = 0;
        int Blank;
        int ColorBack;
        int ColorFore;
        int ColorAttr;
    };

    static inline XList<ScreenLineOffsetDef> ScreenLineOffsetArray;
};

#endif // SCREEN_H
