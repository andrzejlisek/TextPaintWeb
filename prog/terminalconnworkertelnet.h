#ifndef TERMINALCONNWORKERTELNET_H
#define TERMINALCONNWORKERTELNET_H

#include "terminalconnworker.h"

class TerminalConnWorkerTelnet : public TerminalConnWorker
{
public:
    TerminalConnWorkerTelnet();
    void AfterOpen();
    void Send(Raw &Data);
    void Recv(Raw &Data);
    void Resize(int NewW, int NewH);
private:
    void SendRaw(Raw &Data);
    XList<std::string> CommandMsgKey;
    XList<std::string> CommandMsgVal;

    // 0 - Normal
    // 1 - InsideCommand
    int ProcessState = 0;
    std::string TelnetCommand;
    std::string CommandDesc(std::string Cmd);
};

#endif // TERMINALCONNWORKERTELNET_H
