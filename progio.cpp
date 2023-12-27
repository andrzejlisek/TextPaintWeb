// https://stackoverflow.com/questions/32291084/minimal-working-example-for-emscripten-webworker
// https://emscripten.org/docs/api_reference/emscripten.h.html

#include "progcommon.cpp"

void Callback(char * data, int size, void * arg)
{
    switch(data[0])
    {
        case '_':
            emscripten_run_script(data);
            return;
    }
}

worker_handle worker;
bool VTRun = false;

int main()
{
    BufInit();
    worker = emscripten_create_worker("compiled/progcore.js");
    return 0;
}

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void Init(const char * ConfData)
    {
        BufClear();
        BufTxt(ConfData);
        BufChr(0);
        emscripten_call_worker(worker, "Init", IOBuf, IOBufPtr, Callback, (void*)0);
    }
    
    EMSCRIPTEN_KEEPALIVE
    void EventTick()
    {
        char MsgX[2];
        MsgX[0] = '_';
        MsgX[1] = 0;
        emscripten_call_worker(worker, "EventTick", MsgX, 2, Callback, (void*)0);
    }

    EMSCRIPTEN_KEEPALIVE
    void EventKey(char * KeyName, int KeyChr, int KeyS, int KeyC, int KeyA)
    {
        BufClear();
        BufTxt(KeyName);
        BufChr(',');
        BufNum(KeyChr);
        BufNum(KeyS);
        BufNum(KeyC);
        BufNum(KeyA);
        BufChr(0);
        emscripten_call_worker(worker, "EventKey", IOBuf, IOBufPtr, Callback, (void*)0);
    }
    
    EMSCRIPTEN_KEEPALIVE
    void EventOther(const char * EvtName, const char * EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4)
    {
        BufClear();
        BufTxt(EvtName);
        BufChr(',');
        BufStr(EvtParam0);
        BufChr(',');
        BufNum(EvtParam1);
        BufNum(EvtParam2);
        BufNum(EvtParam3);
        BufNum(EvtParam4);
        BufChr(0);
        emscripten_call_worker(worker, "EventOther", IOBuf, IOBufPtr, Callback, (void*)0);
    }

    EMSCRIPTEN_KEEPALIVE
    void EventOtherFile(const char * EvtName, const char * EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4)
    {
        BufClear();
        BufTxt(EvtName);
        BufChr(',');
        BufStr(EvtParam0);
        BufChr(',');
        BufNum(EvtParam1);
        BufNum(EvtParam2);
        BufNum(EvtParam3);
        BufNum(EvtParam4);
        BufChr(0);
        emscripten_call_worker(worker, "EventOtherFile", IOBuf, IOBufPtr, Callback, (void*)0);
    }
}

