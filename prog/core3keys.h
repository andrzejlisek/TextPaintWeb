#ifndef CORE3KEYS_H
#define CORE3KEYS_H

#include "corecommon.h"

class Core3Keys : public CoreCommon
{
public:
    Core3Keys();
    void Init();
    void EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void EventOther(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4);
private:
    XList<TextCodec> EncodingList;
    int EncodingListI;
    int EncodingListState = 0;
    XList<int> EncodingKey1;
    XList<int> EncodingKey2;
    std::string KeyId0 = "";
    int KeyCounter = 0;
};

#endif // CORE3KEYS_H
