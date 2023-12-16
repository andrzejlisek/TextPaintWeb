#ifndef SCREEN_H
#define SCREEN_H

#include "xlist.h"
#include "str.h"
#include <string>

class Screen
{
public:
    Screen();
    static inline int CharDoubleTable[18 * 65536];
    static inline int CharDoubleTableInv[18 * 65536];
    static void AddDoubleRange(int X1, int X2);
    static void StaticInit();
    static int CharDouble(int C);
    static int CharDoubleInv(int C);
    static inline int TextNormalBack = 0;
    static inline int TextNormalFore = 7;
    static inline int TerminalCellW = 8;
    static inline int TerminalCellH = 16;

    static inline void (*ScreenClear)( int, int );
    static inline void (*ScreenChar)( int, int, int, int, int, int, int, int );
    static inline void (*ScreenResize_)( int, int );
    static inline void (*ScreenCursorMove_)( int, int );
    static inline void (*ScreenTextMove)( int, int, int, int, int, int );
    static inline void (*ClipboardCopy)( std::string );
    static inline void (*ClipboardPaste)( );
    static inline void (*WorkerSend)( int, std::string );
    static void ScreenResize(int NewW, int NewH);
    static void ScreenCursorMove(int X, int Y);
    static void ScreenRefresh();

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
private:
    static inline XList<int> Range1;
    static inline XList<int> Range2;
    static inline const int CharDoubleTableLength = 18 * 65536;
    static inline const int BlankDoubleChar = (0x10FFFF + 1);
    static inline const int BlankDoubleCharVis = 32;
};

#endif // SCREEN_H
