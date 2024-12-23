#include "terminalconnworker.h"

TerminalConnWorker::TerminalConnWorker()
{
    B64 = std::make_unique<TextCodec>(TextCodec::BASE64);
}

TerminalConnWorker::~TerminalConnWorker()
{
    Close();
}

void TerminalConnWorker::Open(std::string Protocol, std::string AddrPort, int TerminalW, int TerminalH)
{
    ConnRecv = false;
    if (!WorkerOpen)
    {
        std::string WorkerAddr = "";
        int Idx = Str(Protocol).IndexOf(':');
        TerminalName = Protocol.substr(Idx + 1);
        switch (_(Protocol.substr(0, Idx).c_str()))
        {
            case _("CMD"):
                WorkerAddr = "CMD:" + AddrPort;
                WorkerNumSend = 10;
                WorkerNumOpen = 11;
                WorkerNumClose = 12;
                WorkerNumReport = 99;
                break;
            case _("TELNET"):
                WorkerAddr = "NET:" + AddrPort;
                WorkerNumSend = 10;
                WorkerNumOpen = 11;
                WorkerNumClose = 12;
                WorkerNumReport = 99;
                break;
            case _("RAWNET"):
                WorkerAddr = "NET:" + AddrPort;
                WorkerNumSend = 10;
                WorkerNumOpen = 11;
                WorkerNumClose = 12;
                WorkerNumReport = 99;
                break;
            default:
                break;
        }
        Screen::WorkerSend(WorkerNumOpen, WorkerAddr);
    }
    WorkerOpen = true;
}

int TerminalConnWorker::IsConnected()
{
    if (WorkerOpen)
    {
        if (ConnRecv)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
    else
    {
        return 3;
    }
}

void TerminalConnWorker::ReportBegin()
{
    if (WorkerOpen && (WorkerNumReport < 99))
    {
        Screen::WorkerSend(WorkerNumReport, "1");
    }
}

void TerminalConnWorker::ReportEnd()
{
    if (WorkerOpen && (WorkerNumReport < 99))
    {
        Screen::WorkerSend(WorkerNumReport, "0");
    }
}

void TerminalConnWorker::Send(Raw &Data)
{
    if (WorkerOpen)
    {
        Str S;
        B64.get()->Reset();
        B64.get()->Decode(Data, S);
        Screen::WorkerSend(WorkerNumSend, S.ToString());
    }
}

void TerminalConnWorker::Close()
{
    if (WorkerOpen)
    {
        Screen::WorkerSend(WorkerNumClose, "");
    }
    WorkerOpen = false;
}
