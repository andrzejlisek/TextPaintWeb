#include "coreansi.h"

void CoreAnsi::ScreenPutChar(int X, int Y, int C, int ColorBack, int ColorFore, int FontW, int FontH, int ColorAttr)
{
    Screen::ScreenChar(X, Y, C, ColorBack, ColorFore, ColorAttr, FontW, FontH);
}

void CoreAnsi::ScreenBell()
{

}

void CoreAnsi::ScreenSetLineOffset(int Y, int Offset, bool Blank, int ColorBack, int ColorFore, int ColorAttr)
{
    Screen::ScreenLineOffset(Y, Offset, Blank ? 1 : 0, ColorBack, ColorFore, ColorAttr);
}

void CoreAnsi::ScreenMove(int SrcX, int SrcY, int DstX, int DstY, int W, int H)
{
    Screen::ScreenTextMove(SrcX, SrcY, DstX, DstY, W, H);
}

void CoreAnsi::ScreenClear(int ColorB, int ColorF)
{
    Screen::ScreenClear(ColorB, ColorF);
}

void CoreAnsi::ScreenSetCursorPosition(int X, int Y)
{
    Screen::ScreenCursorMove(X, Y);
    Screen::ScreenRefresh();
}

void CoreAnsi::ScreenAppResize(int NewW, int NewH, bool Force)
{
    Screen::ScreenResize(NewW, NewH);
}
