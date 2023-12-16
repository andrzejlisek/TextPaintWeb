#ifndef TERMINALCONNTEST_H
#define TERMINALCONNTEST_H

#include "terminalconn.h"
#include "stopwatch.h"

class TerminalConnTest : public TerminalConn
{
public:
    TerminalConnTest();
    int IsConnected();
    void Send(Raw &Data);
private:
    static constexpr char SpecialChar = '`';
    Stopwatch SW;
    bool DataSpecial = false;
};

#endif // TERMINALCONNTEST_H
