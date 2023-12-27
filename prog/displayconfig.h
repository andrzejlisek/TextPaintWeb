#ifndef DISPLAYCONFIG_H
#define DISPLAYCONFIG_H

#include "screen.h"
#include "str.h"
#include <memory>
#include "coreansi.h"

class DisplayConfig
{
public:
    DisplayConfig();
    std::shared_ptr<CoreAnsi> CoreAnsi_;
    std::shared_ptr<ConfigFile> CF;
    void Repaint();
    void EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void Open();
    int PopupFore = 0;
    int PopupBack = 0;
    bool RequestRepaint = false;
    bool RequestClose = false;
    bool RequestResize = false;
    bool RequestReload = false;
    bool RequestSave = false;
    int ResizeW = 1;
    int ResizeH = 1;
private:
    int MenuPos = 0;
    void ScreenResize(int DeltaX, int DeltaY);
};

#endif // DISPLAYCONFIG_H
