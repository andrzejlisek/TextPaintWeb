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
    worker = emscripten_create_worker("progcore.js");
    return 0;
}

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void Init()
    {
        BufClear();
        BufChr('_');
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
    void TestString(char * Str1, wchar_t * Str2)
    {

        EventOther("TextPaste", ",Na_pis,", 1, 2, 0, 0);

        std::cout << "test znaku start" << std::endl;


        std::string xString(Str1);
        std::wstring xWString(Str2);
        /*std::u16string xString16(Str1);
        std::u32string xString32(Str1);*/

        std::cout << "1. " << xString.size() << "  ";
        for (int I = 0; I < xString.size(); I++)
        {
            int TT = (int)(xString[I]);
            if (TT < 0) TT += 256;
            std::cout << "  " << TT;
        }
        std::cout << std::endl;

        std::cout << "2. " << xWString.size() << "  ";
        for (int I = 0; I < xWString.size(); I++)
        {
            std::cout << "  " << (int)(xWString[I]);
        }
        std::cout << std::endl;

        /*std::cout << "3. " << xString16.size() << "  ";
        for (int I = 0; I < xString16.size(); I++)
        {
            std::cout << "  " << (int)(xString16[I]);
        }
        std::cout << std::endl;

        std::cout << "4. " << xString32.size() << "  ";
        for (int I = 0; I < xString32.size(); I++)
        {
            std::cout << "  " << (int)(xString32[I]);
        }
        std::cout << std::endl;*/
    
        std::cout << "test znaku stop" << std::endl;
    }
}

