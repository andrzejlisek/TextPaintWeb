#ifndef TERMINALCONN_H
#define TERMINALCONN_H

#include <memory>
#include "textcodec.h"
#include <iostream>

class TerminalConn
{
public:
    TerminalConn();
    virtual void Open(std::string Addr, int Port, std::string TerminalName_, int TerminalW, int TerminalH);
    virtual int IsConnected();
    virtual void Send(Raw &Data);
    virtual void Close();
    virtual void Resize(int NewW, int NewH);
    XList<Raw> Msgs;
protected:
    Raw Loop;
    void LoopSend(Raw &Data);
    void LoopReceive(Raw &Data);
    void ScreenNewLine();
    void ScreenClear();
};

#endif // TERMINALCONN_H
