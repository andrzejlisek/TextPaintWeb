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

    void Open(std::string Addr, int Port, std::string TerminalName_, int TerminalW, int TerminalH);
    int IsConnected();
    void Send(Raw &Data);
    void Close();
private:
    std::unique_ptr<TextCodec> B64;
    bool WorkerOpen = false;;
};

#endif // TERMINALCONNWORKER_H
