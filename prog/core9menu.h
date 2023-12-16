#ifndef CORE9MENU_H
#define CORE9MENU_H

#include "corecommon.h"

class Core9Menu : public CoreCommon
{
public:
    Core9Menu();
    void EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void EventOther(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4);
    int Opt = 9;
};

#endif // CORE9MENU_H
