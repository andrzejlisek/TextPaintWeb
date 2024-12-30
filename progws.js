const WebSocketFileRoot = "!!socket";

function WebSocketResult(Params)
{
    switch (Params.Type)
    {
        case "WebSocket":
            if (Params.Error)
            {
                console.log("WebSocket error: " + Params.ErrorMessage);
                console.log(Params.ErrorMessage);
                WebSocketObjectReady = true;
                WebSocketObjectConnected = false;
            }
            else
            {
                WebSocketObjectReady = true;
                WebSocketObjectConnected = true;
            }
            break;
        case "Connection":
            {
                let Data_ = Params.Data;
                while (Data_.length > FileBufSize)
                {
                    let DataPart = Data_.substring(0, FileBufSize);

                    ProgEventOther("Received", DataPart, 0, 0, 0, 0);

                    Data_ = Data_.substring(FileBufSize);
                }
                ProgEventOther("Received", Data_, 0, 0, 0, 0);
            }
            break;
    }
}

function WebSocketPathInsideSystem(Name)
{
    if (Name == WebSocketFileRoot)
    {
        return true;
    }
    if (Name.startsWith(WebSocketFileRoot + "/"))
    {
        return true;
    }
    return false;
}

function WebSocketPathToServer(Name)
{
    if (Name == WebSocketFileRoot)
    {
        return "";
    }
    else
    {
        return Name.substr(WebSocketFileRoot.length + 1);
    }
}

function WebSocketPathFromServer(Name)
{
    if (Name == "")
    {
        return WebSocketFileRoot;
    }
    else
    {
        return WebSocketFileRoot + "/" + Name;
    }
}

function WebSocketFileSystemDir(Name)
{
    FileImportFinish(0, 2, Name, "~DIR", "", 0);
    Name = WebSocketPathToServer(Name);
    WebSocketObject.GetFiles(Name, "^.*$").then(
    (Params) => {
        let Params_Path = Params.Path;
        let Params_Parent = Params.Parent;
        let FileList = Params.Files;
        let DirListStr = "";

        Name = WebSocketPathFromServer(Name);

        for (let I = 0; I < FileList.length; I++)
        {
            if (FileList[I].Dir)
            {
                DirListStr = DirListStr + Name + "/" + FileList[I].Name + "/\n";
            }
            else
            {
                DirListStr = DirListStr + Name + "/" + FileList[I].Name + "\n";
            }
        }
        
        FileImportFinish(0, 2, Name, "~DIR", btoa(DirListStr), 0);
    }
    ).catch((ErrMsg) => {
        FileImportFinish(0, 2, Name, "~DIR", "", 0);
    });
}

function WebSocketFileSystemGet(Name, Attrib)
{
    Name = WebSocketPathToServer(Name);
    WebSocketObject.FileOpen(Name, true, false, false, false).then(
    (Params1) => {
        if (Params1.FileId > 0)
        {
            WebSocketObject.FileRead(Params1.FileId, Params1.Size).then(
            (Data) => {

                WebSocketObject.FileClose(Params1.FileId).then(
                (Params2) => {
                    Name = WebSocketPathFromServer(Name);
                    FileImportFinish(0, 1, Name, Attrib, Data, 0);
                }).catch((ErrMsg) => {
                    FileImportFinish(0, 1, Name, Attrib, Data, 0);
                });
            }).catch((ErrMsg) => {
                FileImportFinish(0, 1, Name, Attrib, "", 1);
            });
        }
    }).catch((ErrMsg) => {
        FileImportFinish(0, 1, Name, Attrib, "", 1);
    });
}

function WebSocketFileSystemSet(Name, Attrib, Data)
{
    Name = WebSocketPathToServer(Name);
    WebSocketObject.FileOpen(Name, false, true, false, true).then(
    (Params1) => {
        if (Params1.FileId > 0)
        {
            WebSocketObject.FileWrite(Params1.FileId, Data).then(
            (Data) => {
                WebSocketObject.FileClose(Params1.FileId).then(
                (Params2) => {
                    Name = WebSocketPathFromServer(Name);
                    FileExportFinish(0, 1, Name, Attrib, Data, 0);
                }).catch((ErrMsg) => {
                    Name = WebSocketPathFromServer(Name);
                    FileExportFinish(0, 1, Name, Attrib, Data, 0);
                });
            }).catch((ErrMsg) => {
                Name = WebSocketPathFromServer(Name);
                FileExportFinish(0, 1, Name, Attrib, Data, 1);
            });
        }
    }).catch((ErrMsg) => {
        Name = WebSocketPathFromServer(Name);
        FileExportFinish(0, 1, Name, Attrib, Data, 1);
    });
}

function WebSocketFileSystemDelete(Name, Attrib)
{
    Name = WebSocketPathToServer(Name);
    WebSocketObject.FileDelete(Name).then(
    (Params) => {
        Name = WebSocketPathFromServer(Name);
        if (Attrib != "~~")
        {
            FileExportFinish(0, 3, Name, Attrib, "", 0);
        }
    }
    ).catch((ErrMsg) => {
        Name = WebSocketPathFromServer(Name);
        if (Attrib != "~~")
        {
            FileExportFinish(0, 3, Name, Attrib, "", 0);
        }
    });
}

function WebSocketConnOpen(Param)
{
    ConnId = 0;
    const ParamI = Param.indexOf(':');
    
    WebSocketObject.ConnOpen(Param.substr(ParamI + 1), Param.substr(0, ParamI), true).then(
    (Params) => {
        ConnId = Params.ConnId;
        ProgEventOther("Received", "", 0, 0, 0, 0);
    }
    ).catch((ErrMsg) => {
        console.log("ConnOpen error: " + ErrMsg);
        ProgEventOther("Received", btoaASCII(ErrMsg), 1, 0, 0, 0);
    });

}

function WebSocketConnClose()
{
    WebSocketObject.ConnClose(ConnId).then(
    (Params) => {
        ProgEventOther("Received", "", 1, 0, 0, 0);
    }
    ).catch((ErrMsg) => {
        console.log("ConnClose error: " + ErrMsg);
        ProgEventOther("Received", btoaASCII(ErrMsg), 1, 0, 0, 0);
    });
}

function WebSocketConnSend(Data)
{
    WebSocketObject.ConnSend(ConnId, Data).then(
    (Params) => {
    }
    ).catch((ErrMsg) => {
        console.log("ConnSend error: " + ErrMsg);
    });
}

