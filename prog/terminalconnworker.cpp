#include "terminalconnworker.h"

TerminalConnWorker::TerminalConnWorker()
{
    B64 = std::make_unique<TextCodec>(TextCodec::BASE64);
}

TerminalConnWorker::~TerminalConnWorker()
{
    Close();
}

void TerminalConnWorker::Open(std::string Addr, int Port, std::string TerminalName_, int TerminalW, int TerminalH)
{
    if (!WorkerOpen)
    {
        Screen::WorkerSend(1, "");
    }
    WorkerOpen = true;
}

int TerminalConnWorker::IsConnected()
{
    if (WorkerOpen)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void TerminalConnWorker::Send(Raw &Data)
{
    if (WorkerOpen)
    {
        Str S;
        B64.get()->Reset();
        B64.get()->Decode(Data, S);
        Screen::WorkerSend(0, S.ToString());
    }
}

void TerminalConnWorker::Close()
{
    if (WorkerOpen)
    {
        Screen::WorkerSend(2, "");
    }
    WorkerOpen = false;
}
