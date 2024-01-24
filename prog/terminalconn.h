#ifndef TERMINALCONN_H
#define TERMINALCONN_H

#include <memory>
#include "textcodec.h"
#include <iostream>

class TerminalConn
{
public:
    TerminalConn();
    virtual void Open(std::string Protocol, std::string AddrPort, int TerminalW, int TerminalH);
    virtual void AfterOpen();
    virtual int IsConnected();
    virtual void Send(Raw &Data);
    virtual void Recv(Raw &Data);
    virtual void Close();
    virtual void Resize(int NewW, int NewH);
    XList<Raw> Msgs;
protected:
    bool ConnRecv = false;
    Raw Loop;
    void LoopSend(Raw &Data);
    void LoopReceive(Raw &Data);
    void ScreenNewLine();
    void ScreenClear();
};

#endif // TERMINALCONN_H
