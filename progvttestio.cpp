#include <iostream>
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

worker_handle Wrk;

void cback0(char * data, int size, void * arg)
{
}

void cback(char * data, int size, void * arg)
{
    if (size > 5)
    {
        emscripten_run_script(data);
    }
}

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void EventData(char * data)
    {
        int data_l = strlen(data) + 1;
        emscripten_call_worker(Wrk, "EventData", data, data_l, cback, (void*)0);
    }

    EMSCRIPTEN_KEEPALIVE
    void EventReport(char * data)
    {
        int data_l = strlen(data) + 1;
        emscripten_call_worker(Wrk, "EventReport", data, data_l, cback, (void*)0);
    }

    EMSCRIPTEN_KEEPALIVE
    void EventStart(char * arg, int DebugParams_)
    {
        char MsgX[2];
        MsgX[0] = DebugParams_ + 48;
        MsgX[1] = 0;
        emscripten_call_worker(Wrk, "EventStart", MsgX, 2, cback0, (void*)0);
    }

    EMSCRIPTEN_KEEPALIVE
    void EventStop()
    {
        char MsgX[2];
        MsgX[0] = '0';
        MsgX[1] = 0;
        emscripten_call_worker(Wrk, "EventStop", MsgX, 2, cback0, (void*)0);
    }
}

int main()
{
    Wrk = emscripten_create_worker("compiled/progvttest.js");
    return 0;
}

