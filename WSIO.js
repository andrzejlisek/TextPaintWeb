class WebSocketInputOutput
{
    constructor(Addr_, RetryTime_, ResultFunc_)
    {
        this.IsOpen = true;
        this.RetryTime = RetryTime_;
        this.ResultFunc = ResultFunc_;
        this.MessageI = new WebSocketInputOutputKeyValue();
        this.MessageO = new WebSocketInputOutputKeyValue();
        this.Addr = Addr_;
        this.MsgCounter = 1;
        this.PromisePool1 = [];
        this.PromisePool0 = [];
        this.MessageQueue = [];
        this.SrvInvoke();
    }
    
    Close()
    {
        this.IsOpen = false;
        this.WS.close();
    }
    
    SrvInvoke()
    {
        if (this.IsOpen)
        {
            try
            {
                this.WS = new WebSocket(this.Addr);
                this.WS.onopen = (event) => this.SrvOpen();
                this.WS.onmessage = (message) => this.SrvMessage(message);
                this.WS.onerror = (message) => this.SrvError(message);
            }
            catch (error)
            {
                this.SrvError(error);
            }
        }
    }

    SrvOpen()
    {
        this.ResultFunc({Type:"WebSocket",Error:""});
        for (let I = 0; I < this.MessageQueue.length; I++)
        {
            this.WS.send(this.MessageQueue[I]);
        }
        this.MessageQueue = [];
    }

    SrvMessage(Msg)
    {
        this.MessageI.StringLoad(Msg.data);
        let Id = this.MessageI.ParamGetS("Id");
        if (Id > 0)
        {
            let PromiseParams = this.ProcessMessage(this.MessageI, this.MessageO);
            let ErrMsg = this.MessageI.ParamGetS("Error");
            if (ErrMsg)
            {
                this.PromisePool0[Id](ErrMsg);
            }
            else
            {
                this.PromisePool1[Id](PromiseParams);
            }
            delete this.PromisePool1[Id];
            delete this.PromisePool0[Id];
        }
        else
        {
            let MsgType = this.MessageI.ParamGetS("Type");
            switch (MsgType)
            {
                case "Connection":
                    this.ResultFunc({Type:MsgType, Error:this.MessageI.ParamGetS("Error"), Data:this.MessageI.ParamGetS("Data"), ConnId:this.MessageI.ParamGetS("ConnId")});
                    break;
            }
        }
    }

    SrvError(Msg)
    {
        if (this.IsOpen)
        {
            this.ResultFunc({Type:"WebSocket",Error:"Connection error",ErrorMessage:Msg});
            if (this.RetryTime > 0)
            {
                setTimeout(() => { this.SrvInvoke(); }, this.RetryTime);
            }
        }
    }
    
    SendMessage(Func1, Func0)
    {
        let Id = this.MsgCounter;
        this.PromisePool1[Id] = Func1;
        this.PromisePool0[Id] = Func0;
        this.MessageO.ParamSet("Id", Id);
        let ToQueue = true;
        if (this.WS)
        {
            if (this.WS.readyState === WebSocket.OPEN)
            {
                ToQueue = false;
                this.WS.send(this.MessageO.StringSave());
            }
        }
        if (ToQueue)
        {
            this.SrvInvoke();
            this.MessageQueue.push(this.MessageO.StringSave());
        }
        this.MsgCounter = this.MsgCounter + 1;
    }
    
    
    
    
    
    Test(TimeOut, ErrorMsg)
    {
        this.MessageO.ParamClear();
        this.MessageO.ParamSet("Command", "Test");
        this.MessageO.ParamSet("Time", TimeOut);
        this.MessageO.ParamSet("Error", ErrorMsg);
        return new Promise((resolve, reject) => {
            this.SendMessage(resolve, reject);
        });
    }

    GetFiles(Path, Filter)
    {
        this.MessageO.ParamClear();
        this.MessageO.ParamSet("Command", "DirectoryList");
        this.MessageO.ParamSet("Path", Path);
        this.MessageO.ParamSet("Filter", Filter);
        return new Promise((resolve, reject) => {
            this.SendMessage(resolve, reject);
        });
    }

    FileDelete(Path)
    {
        this.MessageO.ParamClear();
        this.MessageO.ParamSet("Command", "FileDelete");
        this.MessageO.ParamSet("Path", Path);
        return new Promise((resolve, reject) => {
            this.SendMessage(resolve, reject);
        });
    }
    
    FileOpen(Path, Read, Write, Append, Truncate)
    {
        this.MessageO.ParamClear();
        this.MessageO.ParamSet("Command", "FileOpen");
        this.MessageO.ParamSet("Path", Path);
        this.MessageO.ParamSet("Read", Read);
        this.MessageO.ParamSet("Write", Write);
        this.MessageO.ParamSet("Append", Append);
        this.MessageO.ParamSet("Truncate", Truncate);
        return new Promise((resolve, reject) => {
            this.SendMessage(resolve, reject);
        });
    }
    
    FileRead(FileId, Count)
    {
        this.MessageO.ParamClear();
        this.MessageO.ParamSet("Command", "FileRead");
        this.MessageO.ParamSet("FileId", FileId);
        this.MessageO.ParamSet("Count", Count);
        return new Promise((resolve, reject) => {
            this.SendMessage(resolve, reject);
        });
    }

    FileWrite(FileId, Data)
    {
        this.MessageO.ParamClear();
        this.MessageO.ParamSet("Command", "FileWrite");
        this.MessageO.ParamSet("FileId", FileId);
        this.MessageO.ParamSet("Data", Data);
        return new Promise((resolve, reject) => {
            this.SendMessage(resolve, reject);
        });
    }

    FileClose(FileId)
    {
        this.MessageO.ParamClear();
        this.MessageO.ParamSet("Command", "FileClose");
        this.MessageO.ParamSet("FileId", FileId);
        return new Promise((resolve, reject) => {
            this.SendMessage(resolve, reject);
        });
    }

    ConnOpen(Address, Type, Push)
    {
        this.MessageO.ParamClear();
        this.MessageO.ParamSet("Command", "ConnOpen");
        this.MessageO.ParamSet("Address", Address);
        this.MessageO.ParamSet("Type", Type);
        this.MessageO.ParamSet("Push", Push);
        return new Promise((resolve, reject) => {
            this.SendMessage(resolve, reject);
        });
    }

    ConnInfo(ConnId)
    {
        this.MessageO.ParamClear();
        this.MessageO.ParamSet("Command", "ConnInfo");
        this.MessageO.ParamSet("ConnId", ConnId);
        return new Promise((resolve, reject) => {
            this.SendMessage(resolve, reject);
        });
    }

    ConnClose(ConnId)
    {
        this.MessageO.ParamClear();
        this.MessageO.ParamSet("Command", "ConnClose");
        this.MessageO.ParamSet("ConnId", ConnId);
        return new Promise((resolve, reject) => {
            this.SendMessage(resolve, reject);
        });
    }

    ConnSend(ConnId, Data)
    {
        this.MessageO.ParamClear();
        this.MessageO.ParamSet("Command", "ConnSend");
        this.MessageO.ParamSet("ConnId", ConnId);
        this.MessageO.ParamSet("Data", Data);
        return new Promise((resolve, reject) => {
            this.SendMessage(resolve, reject);
        });
    }

    ConnRecv(ConnId)
    {
        this.MessageO.ParamClear();
        this.MessageO.ParamSet("Command", "ConnRecv");
        this.MessageO.ParamSet("ConnId", ConnId);
        return new Promise((resolve, reject) => {
            this.SendMessage(resolve, reject);
        });
    }

    ProcessMessage(MessageI, MessageO)
    {
        let CmdInfo = MessageI.ParamGetS("Command");
        //Info("[" + MessageI.Print() + "]");
        //Info("[" + CmdInfo + "]");
        let Par0;
        let Par1;
        let Par2;
        
        switch (MessageI.ParamGetS("Command"))
        {
            case "Test":
                return {};
            case "DirectoryList":
                Par0 = MessageI.ParamGetS("Path");
                Par1 = MessageI.ParamGetS("Parent");
                Par2 = [];
                {
                    let L = MessageI.ParamGetI("ItemCount");
                    for (let I = 0; I < L; I++)
                    {
                        let Item = {
                            Name:MessageI.ParamGetS("Item" + I + "Name"),
                            Dir:MessageI.ParamGetB("Item" + I + "Dir"),
                            File:MessageI.ParamGetB("Item" + I + "File"),
                            Size:MessageI.ParamGetI("Item" + I + "Size"),
                            Date:MessageI.ParamGetI("Item" + I + "Date")
                        };
                        Par2.push(Item);
                    }
                }
                return { Path:Par0, Parent:Par1, Files:Par2 };
            case "FileOpen":
                Par0 = MessageI.ParamGetS("FileId");
                Par1 = MessageI.ParamGetS("Size");
                Par2 = MessageI.ParamGetS("Position");
                return { FileId:Par0, Size:Par1, Position:Par2 };
            case "FileRead":
                if (MessageI.ParamGetS("Data"))
                {
                    return MessageI.ParamGetS("Data");
                }
                else
                {
                    return "";
                }
            case "FileWrite":
                return {};
            case "FileClose":
                return {};
            case "ConnOpen":
                return { ConnId:MessageI.ParamGetS("ConnId") };
            case "ConnInfo":
                return { Status:MessageI.ParamGetS("Status"), Push:MessageI.ParamGetS("Push") };
            case "ConnClose":
                return {};
            case "ConnSend":
                return {};
            case "ConnRecv":
                if (MessageI.ParamGetS("Data"))
                {
                    return MessageI.ParamGetS("Data");
                }
                else
                {
                    return "";
                }
            default:
                return {};
        }
    }

}

