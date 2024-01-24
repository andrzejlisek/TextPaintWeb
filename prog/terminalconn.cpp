#include "terminalconn.h"

TerminalConn::TerminalConn()
{

}

void TerminalConn::Open(std::string Protocol, std::string AddrPort, int TerminalW, int TerminalH)
{
    ConnRecv = false;
}

void TerminalConn::AfterOpen()
{

}

int TerminalConn::IsConnected()
{
    return 0;
}

void TerminalConn::Send(Raw &Data)
{

}

void TerminalConn::Recv(Raw &Data)
{
    ConnRecv = true;
}

void TerminalConn::Close()
{

}

void TerminalConn::Resize(int NewW, int NewH)
{

}

void TerminalConn::LoopSend(Raw &Data)
{
    Loop.AddRange(Data);
}

void TerminalConn::LoopReceive(Raw &Data)
{
    Data.AddRange(Loop);
    Loop.Clear();
}

void TerminalConn::ScreenNewLine()
{
    Loop.Add(13);
    Loop.Add(10);
}

void TerminalConn::ScreenClear()
{
    Loop.Add(0x1B);
    Loop.Add('[');
    Loop.Add('0');
    Loop.Add('m');
    Loop.Add(0x1B);
    Loop.Add('[');
    Loop.Add('1');
    Loop.Add(';');
    Loop.Add('1');
    Loop.Add('H');
    Loop.Add(0x1B);
    Loop.Add('[');
    Loop.Add('2');
    Loop.Add('J');
}
