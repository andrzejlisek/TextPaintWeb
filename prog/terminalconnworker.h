#ifndef TERMINALCONNWORKER_H
#define TERMINALCONNWORKER_H

#include "terminalconn.h"
#include "textcodec.h"
#include "screen.h"
#include "memory"

class TerminalConnWorker : public TerminalConn
{
public:
    TerminalConnWorker();
    ~TerminalConnWorker();

    void Open(std::string Protocol, std::string AddrPort, int TerminalW, int TerminalH);
    int IsConnected();
    void Send(Raw &Data);
    void Close();
protected:
    std::string TerminalName;
    std::unique_ptr<TextCodec> B64;
    bool WorkerOpen = false;
    int WorkerNumSend = 0;
    int WorkerNumOpen = 1;
    int WorkerNumClose = 2;
};

#endif // TERMINALCONNWORKER_H
