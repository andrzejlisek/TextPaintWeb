#ifndef SCREEN_H
#define SCREEN_H

#include "xlist.h"
#include "str.h"
#include <string>
#include <iostream>
#include "textwork.h"

class Screen
{
public:
    Screen();
    static inline bool WinAuto = true;
    static void StaticInit(std::string Double1, std::string Double2);
    static int CharDouble(int C);
    static int CharDoubleInv(int C);
    static inline int TextNormalBack = 0;
    static inline int TextNormalFore = 7;
    static inline int TerminalCellW = 8;
    static inline int TerminalCellH = 16;

    static inline void (*ScreenSetConfig)( );
    static inline void (*ScreenClear_)( int, int );
    static inline void (*ScreenChar_)( int, int, int, int, int, int, int, int );
    static inline void (*ScreenResize_)( int, int );
    static inline void (*ScreenCursorMove_)( int, int );
    static inline void (*ScreenTextMove_)( int, int, int, int, int, int );
    static inline void (*ScreenLineOffset)( int, int, int, int, int, int );
    static inline void (*WorkerSend)( int, std::string );
    static void ScreenClear(int Back, int Fore);
    static void ScreenChar(int X, int Y, int C, int ColorBack, int ColorFore, int ColorAttr, int FontW, int FontH);
    static void ScreenTextMove(int X1, int Y1, int X2, int Y2, int W, int H);
    static void ScreenResize(int NewW, int NewH);
    static void ScreenCursorMove(int X, int Y);
    static void ScreenRefresh();

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
private:
    static inline XList<int> Range1;
    static inline XList<int> Range2;
};

#endif // SCREEN_H
