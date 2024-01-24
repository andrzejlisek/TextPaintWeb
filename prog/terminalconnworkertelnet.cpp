#include "terminalconnworkertelnet.h"

TerminalConnWorkerTelnet::TerminalConnWorkerTelnet()
{
}

void TerminalConnWorkerTelnet::AfterOpen()
{
    ProcessState = 0;
    TelnetCommand = "";

    std::string TerminalNameHex = "";
    for (int i = 0; i < TerminalName.size(); i++)
    {
        if ((((int)TerminalName[i]) >= 32) && (((int)TerminalName[i]) <= 126))
        {
            TerminalNameHex = TerminalNameHex + TextWork::CharCode(TerminalName[i], 0);
        }
    }

    // FA SUB BEGIN
    // F0 SUB SE
    // FB WILL
    // FC WON'T
    // FD DO
    // FE DON'T
    CommandMsgKey.Clear(); CommandMsgVal.Clear();
    CommandMsgKey.Add("FFFD18"); CommandMsgVal.Add("FFFB18");
    CommandMsgKey.Add("FFFD03"); CommandMsgVal.Add("FFFB03");
    CommandMsgKey.Add("FFFA1801FFF0"); CommandMsgVal.Add("FFFA1800" + TerminalNameHex + "FFF0");
    CommandMsgKey.Add("FFFA2701FFF0"); CommandMsgVal.Add("FFFA2700FFF0");
    CommandMsgKey.Add("FFFA2001FFF0"); CommandMsgVal.Add("FFFA200033383430302C3338343030FFF0");
    CommandMsgKey.Add("FFFB03"); CommandMsgVal.Add("FFFD03");
    CommandMsgKey.Add("FFFD01"); CommandMsgVal.Add("FFFB01");
    CommandMsgKey.Add("FFFD1F"); CommandMsgVal.Add("FFFB1F");
    CommandMsgKey.Add("FFFB01"); CommandMsgVal.Add("FFFD01");
    CommandMsgKey.Add("FFFE03"); CommandMsgVal.Add("FFFC03");

    CommandMsgKey.Add("FFFB??"); CommandMsgVal.Add("FFFE");
    CommandMsgKey.Add("FFFC??"); CommandMsgVal.Add("FFFE");
    CommandMsgKey.Add("FFFD??"); CommandMsgVal.Add("FFFC");
    CommandMsgKey.Add("FFFE??"); CommandMsgVal.Add("FFFC");
}

void TerminalConnWorkerTelnet::Send(Raw &Data)
{
    if (WorkerOpen)
    {
        for (int I = (Data.Count - 1); I >= 0; I--)
        {
            if (Data[I] == 255)
            {
                Data.Insert(I, 255);
            }
        }

        SendRaw(Data);
    }
}

void TerminalConnWorkerTelnet::SendRaw(Raw &Data)
{
    Str S;
    B64.get()->Reset();
    B64.get()->Decode(Data, S);
    Screen::WorkerSend(WorkerNumSend, S.ToString());
}

void TerminalConnWorkerTelnet::Recv(Raw &Data)
{
    Raw Data_ = Data.Copy();
    Data.Clear();

    for (int DataI = 0; DataI < Data_.Count; DataI++)
    {
        unsigned char Chr = Data_[DataI];
        switch (ProcessState)
        {
            case 0:
                {
                    if (Chr == 255)
                    {
                        ProcessState = 1;
                        TelnetCommand = "FF";
                    }
                    else
                    {
                        Data.Add(Chr);
                    }
                }
                break;
            case 1:
                {
                    TelnetCommand = TelnetCommand + Hex::IntToHex8(Chr);

                    bool StdCmd = true;

                    if (TelnetCommand == "FFFF")
                    {
                        Data.Add(255);
                        ProcessState = 0;
                        StdCmd = false;
                    }
                    if (TelnetCommand == "FFF9")
                    {
                        ProcessState = 0;
                        StdCmd = false;
                    }
                    if (StdCmd)
                    {
                        std::string CommandAnswer = "";
                        if (TelnetCommand.size() >= 6)
                        {
                            for (int i = 0; i < CommandMsgKey.Count; i++)
                            {
                                if (TelnetCommand.size() >= CommandMsgKey[i].size())
                                {
                                    if (TextWork::StringStartsWith(TelnetCommand, CommandMsgKey[i]))
                                    {
                                        CommandAnswer = CommandMsgVal[i];
                                        break;
                                    }
                                }
                            }

                            if (CommandAnswer == "")
                            {
                                for (int i = 0; i < CommandMsgKey.Count; i++)
                                {
                                    std::string CommandMsgKey0 = TextWork::StringReplace(CommandMsgKey[i], "??", "");
                                    if (TelnetCommand.size() >= (CommandMsgKey0.size()))
                                    {
                                        if (TextWork::StringStartsWith(TelnetCommand, CommandMsgKey0) && TextWork::StringEndsWith(CommandMsgKey[i], "??"))
                                        {
                                            CommandAnswer = CommandMsgVal[i] + TelnetCommand.substr(4);
                                            break;
                                        }
                                    }
                                }
                            }

                            if (CommandAnswer != "")
                            {
                                Str CommandAnswer_S = Str::FromString(TextWork::StringReplace(CommandAnswer, "|", ""));
                                Raw CommandAnswer_R;
                                std::cout << "TELNET: " << CommandDesc(TelnetCommand) << " -> " << CommandDesc(CommandAnswer) << std::endl;
                                TextCodec Hex_(TextCodec::HEX);
                                Hex_.EnqueueStr(CommandAnswer_S);
                                Hex_.DequeueRaw(CommandAnswer_R);
                                SendRaw(CommandAnswer_R);
                                ProcessState = 0;

                                //switch (TelnetCommand + "|" + CommandAnswer)
                                //{
                                //    case "FFFB01|FFFD01":
                                //        LocalEcho = false;
                                //        break;
                                //    case "FFFC01|FFFE01":
                                //        LocalEcho = true;
                                //        break;
                                //}

                            }
                            else
                            {
                                bool NeedAnswer = false;
                                switch (_(TelnetCommand.substr(2, 2).c_str()))
                                {
                                    case _("FB"):
                                    case _("FC"):
                                    case _("FD"):
                                    case _("FE"):
                                        if (TelnetCommand.size() == 6)
                                        {
                                            NeedAnswer = true;
                                            ProcessState = 0;
                                        }
                                        break;
                                    case _("FF"):
                                        ProcessState = 0;
                                        break;
                                    case _("FA"):
                                        if (TextWork::StringEndsWith(TelnetCommand, "FFF0"))
                                        {
                                            NeedAnswer = true;
                                            ProcessState = 0;
                                        }
                                        break;
                                }
                                if (NeedAnswer)
                                {
                                    std::cout << "TELNET: Command without answer " << CommandDesc(TelnetCommand) << std::endl;
                                }
                            }
                        }
                    }
                }
                break;
        }
    }
    ConnRecv = true;
}

std::string TerminalConnWorkerTelnet::CommandDesc(std::string Cmd)
{
    std::string CmdDesc = "";

    if (Str::FromString(Cmd).Contains('|'))
    {
        XList<std::string> Cmd_;
        TextWork::StringSplit(Cmd, '|', Cmd_);
        for (int i = 0; i < Cmd_.Count; i++)
        {
            if (i > 0)
            {
                CmdDesc = CmdDesc + "|";
            }
            CmdDesc = CmdDesc + Cmd_[i];
        }
        return CmdDesc;
    }

    switch (_(Cmd.substr(4, 2).c_str()))
    {
        case _("00"): CmdDesc = "Binary Transmission"; break;
        case _("01"): CmdDesc = "Echo"; break;
        case _("02"): CmdDesc = "Reconnection"; break;
        case _("03"): CmdDesc = "Suppress Go Ahead"; break;
        case _("04"): CmdDesc = "Approx Message Size Negotiation"; break;
        case _("05"): CmdDesc = "Status"; break;
        case _("06"): CmdDesc = "Timing Mark"; break;
        case _("07"): CmdDesc = "Remote Controlled Trans and Echo"; break;
        case _("08"): CmdDesc = "Output Line Width"; break;
        case _("09"): CmdDesc = "Output Page Size"; break;
        case _("0A"): CmdDesc = "Output Carriage-Return Disposition"; break;
        case _("0B"): CmdDesc = "Output Horizontal Tab Stops"; break;
        case _("0C"): CmdDesc = "Output Horizontal Tab Disposition"; break;
        case _("0D"): CmdDesc = "Output Formfeed Disposition"; break;
        case _("0E"): CmdDesc = "Output Vertical Tabstops"; break;
        case _("0F"): CmdDesc = "Output Vertical Tab Disposition"; break;
        case _("10"): CmdDesc = "Output Linefeed Disposition"; break;
        case _("11"): CmdDesc = "Extended ASCII"; break;
        case _("12"): CmdDesc = "Logout"; break;
        case _("13"): CmdDesc = "Byte Macro"; break;
        case _("14"): CmdDesc = "Data Entry Terminal"; break;
        case _("15"): CmdDesc = "SUPDUP"; break;
        case _("16"): CmdDesc = "SUPDUP Output"; break;
        case _("17"): CmdDesc = "Send Location"; break;
        case _("18"): CmdDesc = "Terminal Type"; break;
        case _("19"): CmdDesc = "End of Record"; break;
        case _("1A"): CmdDesc = "TACACS User Identification"; break;
        case _("1B"): CmdDesc = "Output Marking"; break;
        case _("1C"): CmdDesc = "Terminal Location Number"; break;
        case _("1D"): CmdDesc = "Telnet 3270 Regime"; break;
        case _("1E"): CmdDesc = "X.3 PAD"; break;
        case _("1F"): CmdDesc = "Negotiate About Window Size"; break;
        case _("20"): CmdDesc = "Terminal Speed"; break;
        case _("21"): CmdDesc = "Remote Flow Control"; break;
        case _("22"): CmdDesc = "Linemode"; break;
        case _("23"): CmdDesc = "X Display Location"; break;
        case _("24"): CmdDesc = "Environment Variables"; break;
        case _("27"): CmdDesc = "Telnet Environment Option"; break;
        default:
            {
                CmdDesc = "UNKNOWN " + Cmd.substr(4, 2);
            }
            break;
    }
    switch (_(Cmd.substr(2, 2).c_str()))
    {
        case _("FB"): CmdDesc = "WILL " + CmdDesc; break;
        case _("FC"): CmdDesc = "WON'T " + CmdDesc; break;
        case _("FD"): CmdDesc = "DO " + CmdDesc; break;
        case _("FE"): CmdDesc = "DON'T " + CmdDesc; break;
        case _("FA"): CmdDesc = "SUB " + CmdDesc + " " + Cmd.substr(6, Cmd.size() - 10); break;
        default:
            {
                CmdDesc = "UNKNOWN " + Cmd.substr(2, 2);
            }
            break;
    }
    return CmdDesc + " (" + Cmd + ")";
}

void TerminalConnWorkerTelnet::Resize(int NewW, int NewH)
{
    Str CommandAnswer_S = Str::FromString("FFFA1F" + Hex::IntToHex16(NewW) + Hex::IntToHex16(NewH) + "FFF0");
    Raw CommandAnswer_R;
    std::cout << "TELNET: Screen size " << NewW << "x" << NewH << std::endl;
    TextCodec Hex_(TextCodec::HEX);
    Hex_.EnqueueStr(CommandAnswer_S);
    Hex_.DequeueRaw(CommandAnswer_R);
    SendRaw(CommandAnswer_R);
}
