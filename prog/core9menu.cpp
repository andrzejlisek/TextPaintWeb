#include "core9menu.h"

Core9Menu::Core9Menu()
{

}

void Core9Menu::EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    switch (_(KeyName.c_str()))
    {
        case _("Digit0"):
        case _("Numpad0"):
            Opt = 0;
            break;
        case _("Digit1"):
        case _("Numpad1"):
            Opt = 1;
            break;
        case _("Digit2"):
        case _("Numpad2"):
            Opt = 2;
            break;
        case _("Digit3"):
        case _("Numpad3"):
            Opt = 3;
            break;
    }
}

void Core9Menu::EventOther(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4)
{
    switch (_(EvtName.c_str()))
    {
        case _("Resize"):
            ScreenW = EvtParam1;
            ScreenH = EvtParam2;
            Screen::ScreenResize(ScreenW, ScreenH);
            Screen::ScreenClear(Screen::TextNormalBack, Screen::TextNormalFore);
            Screen::ScreenCursorMove(0, 0);
            Screen::ScreenWriteText(Str("Select mode by press digit key:"));
            Screen::ScreenWriteLine();
            Screen::ScreenWriteText(Str("0 - Text editor"));
            Screen::ScreenWriteLine();
            Screen::ScreenWriteText(Str("1 - Animation player"));
            Screen::ScreenWriteLine();
            Screen::ScreenWriteText(Str("2 - Terminal"));
            Screen::ScreenWriteLine();
            Screen::ScreenWriteText(Str("3 - Keyboard test"));
            Screen::ScreenWriteLine();
            Screen::ScreenRefresh();
            break;
    }
}
