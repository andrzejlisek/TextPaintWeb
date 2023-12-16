#ifndef TERMINALKEYBOARD_H
#define TERMINALKEYBOARD_H

#include <memory>
#include "coreansi.h"
#include <map>
#include <string>
#include "xlist.h"
#include "textwork.h"

class TerminalKeyboard
{
public:
    std::shared_ptr<CoreAnsi> CoreAnsi_;
    TerminalKeyboard();
    std::string GetTelnetKeyboardConf(int N);
    void SetTelnetKeyboardConf(int N, int V);
    std::string KeyCAS(bool ModShift, bool ModCtrl, bool ModAlt);
    std::string KeyCode(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    std::string TerminalAnswerBack;
    int TelnetFuncKeyOther = 0;
    int TelnetKeyboardConfI = 0;
    int TelnetKeyboardModsI = 0;
    std::string TelnetKeyboardMods = "000";
    std::string TelnetKeyboardConf = "0000000";
    std::string TelnetKeyboardConfMax = "1112311";

    Str EchoChar;

    void TelnetKeyboardConfMove();
    void TelnetKeyboardConfStep();
    void TelnetKeyboardModsMove();
    void TelnetKeyboardModsStep();
    std::map<std::string, std::string> TerminalKeys;
private:
    XList<std::string> TerminalKeys0;
    XList<std::string> TerminalKeys1;
    XList<std::string> TerminalKeys2;
    XList<std::string> TerminalKeys3;
    XList<std::string> TerminalKeys4;
};

#endif // TERMINALKEYBOARD_H
