#ifndef CORE2TERMINAL_H
#define CORE2TERMINAL_H

#include "corecommon.h"
#include "map"
#include "terminalkeyboard.h"
#include "terminalconn.h"
#include "terminalconntest.h"
#include "terminalconnworker.h"
#include "textcodec.h"

class Core2Terminal : public CoreCommon
{
public:
    Core2Terminal();
    void Init();
    void EventTick();
    void EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void EventOther(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4);
    void ConnOpen();
    void ConnClose(bool StopApp);
private:
    std::unique_ptr<TextCodec> B64;
    int ServerPort = 0;
    bool ServerTelnet = false;
    std::shared_ptr<TextCodec> ServerCodec;

    bool LocalEcho = false;
    bool Command_8bit = false;
    bool UseCtrlKeys = false;
    int DisplayStatus = 0;

    void DisplayStatusRepaint();
    void DisplayStatusPrepare();

    std::string WindowTitleStatus;
    int WindowTitleStatusSize;
    std::string WindowTitle;
    std::string WindowIcon;
    XList<std::string> WindowTitleSt;
    XList<std::string> WindowIconSt;

    std::string TerminalConnection;
    std::string TerminalName;

    // 0 - VT100
    // 1 - VT102
    // 2 - VT220
    // 3 - VT320
    // 4 - VT420
    // 5 - VT520
    int TerminalType = 1;

    std::shared_ptr<TextCodec> TerminalCodecS;
    std::shared_ptr<TextCodec> TerminalCodecR;

    TerminalKeyboard TerminalKeyboard_;

    int TerminalStep;
    Clipboard Clipboard_;

    enum WorkStateCDef { InfoScreen, Session, Toolbox, Toolbox_, EscapeKey, DispConf };
    WorkStateCDef WorkStateC = WorkStateCDef::InfoScreen;

    std::unique_ptr<TerminalConn> Conn;
    bool ConnInternal = false;

    std::string EscapeKey_;
    std::string EscapeKeyId(std::string KeyName, int KeyChar);

    void SendHex(std::string STR);

    void ProcessReceived(Raw &Data);
    std::string TelnetReportStrToStr(std::string N);
    std::string TelnetReportNumToStr(int N);
    void TelnetReport(std::string ReportRequest);

    int TelnetInfoPos = 0;
    void TelnetDisplayInfo(bool NeedRepaint);
    bool TelnetDisplayInfoRepaint = false;
};

#endif // CORE2TERMINAL_H
