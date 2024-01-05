#ifndef TERMINALMOUSE_H
#define TERMINALMOUSE_H

#include <iostream>
#include "screen.h"

class TerminalMouse
{
public:
    TerminalMouse();
    void MouseSet(int Param, bool Value);
    void MouseOn();
    void MouseOff();
    void Reset();
    bool Highlight = false;
    int HighlightX = 0;
    int HighlightY = 0;
    int HighlightFirst = 0;
    int HighlightLast = 0;
    std::string MouseEvent(std::string Name, int X, int Y, int Btn, bool ModShift, bool ModCtrl, bool ModAlt);
    bool MouseScreen = false;
private:
    bool MouseWorks = false;


    bool Mouse0009 = false; // Work mode: X10
    bool Mouse1000 = false; // Work mode: Normal
    bool Mouse1001 = false; // Work mode: Highlight
    bool Mouse1002 = false; // Work mode: Button-Event
    bool Mouse1003 = false; // Work mode: Any-event
    bool Mouse1016 = false; // Coordinates: SGR/pixels
    bool Mouse1006 = false; // Coordinates: SGR
    bool Mouse1015 = false; // Coordinates: URXVT
    bool Mouse1005 = false; // Coordinates: UTF-8
    void MouseUpdate();
    std::string TelnetReportNumToStr(int N);
    std::string MouseCoordCalc(int Val, bool UTF8);
    std::string MouseEventCode(int X, int Y, int Evt);
};

#endif // TERMINALMOUSE_H
