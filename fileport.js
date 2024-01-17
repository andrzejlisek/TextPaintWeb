let FileSystemName = "!!config.txt";

let FileBufSize = 10000;

let FileIndexLS = {};
let FileIndexLSAttr = {};
let FileIndexLSNum = 1;

let FileIndexDB = {};
let FileIndexDBAttr = {};
let FileIndexDBNum = 1;

let FileIndexName = "TextPaintFileIndex";
let FileIndexTempl = "TextPaintFile";
let FileIndexSepa = "\n";
let FileFreeSpaceLS = 0;
let FileFreeSpaceDB = 0;
let FileFreeSpaceSafe = 100;

let ConfigFileJS = {};

let FileIndexToProgram = true;

function ConfigFileS(Param)
{
    if (ConfigFileJS[Param])
    {
        return ConfigFileJS[Param];
    }
    else
    {
        return "";
    }
}

function ConfigFileI(Param)
{
    if (ConfigFileJS[Param])
    {
        return parseInt(ConfigFileJS[Param]);
    }
    else
    {
        return 0;
    }
}

function ConfigFileB(Param)
{
    if (ConfigFileI(Param) != 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

function ConfigFileGui(Param, Value)
{
    if ((typeof Value) == "boolean")
    {
        Value = (Value) ? "1" : "0";
    }
    ConfigFileGet(Param, Value);
    ProgEventOtherFile("ConfigSet", Param + "=" + Value, Param.length, 1, 0, 0);
}


function ConfigFileGet(Param, Value)
{
    if ((typeof Value) == "boolean")
    {
        Value = (Value) ? 1 : 0;
    }
    ConfigFileJS[Param] = Value;
}

function ConfigFileSet(Param, Value)
{
    if ((typeof Value) == "boolean")
    {
        Value = (Value) ? "1" : "0";
    }
    ProgEventOtherFile("ConfigSet", Param + "=" + Value, Param.length, 0, 0, 0);
}



function ConfigFileReset()
{
    FileSystemDelete(FileSystemName, "~~");
}

function IndexClear()
{
    FileSystemDir("", 2);
}

function IndexImport(IndexRaw0, AttribOffset)
{
    let FileIndex__ = {};
    let IndexRaw = IndexRaw0.split(FileIndexSepa);
    for (let I = 0; I < IndexRaw.length; I += 3)
    {
        if (IndexRaw.length > (I + 2))
        {
            if (IndexRaw[I] && (IndexRaw[I] != ""))
            {
                if (AttribOffset == 0)
                {
                    let N = parseInt(IndexRaw[I + 1]);
                    if (FileIndexDBNum <= N)
                    {
                        FileIndexDBNum = N + 1;
                    }
                    FileIndex__[IndexRaw[I]] = N;
                }
                if (AttribOffset == 1)
                {
                    FileIndex__[IndexRaw[I]] = IndexRaw[I + 2];
                }
            }
        }
    }
    return FileIndex__;
}

function IndexExport(FileIndex__, FileIndex__Attr)
{
    let IndexRaw = "";
    for (let k in FileIndex__)
    {
        IndexRaw = IndexRaw + k + FileIndexSepa + FileIndex__[k] + FileIndexSepa + FileIndex__Attr[k] + FileIndexSepa;
    }
    return IndexRaw;
}



function IndexExportLS()
{
    let IndexRaw = IndexExport(FileIndexLS, FileIndexLSAttr);
    DataSet(FileIndexName, IndexRaw);
}

function IndexExportDB()
{
    let IndexRaw = IndexExport(FileIndexDB, FileIndexDBAttr);
    FileDbSet(0, 0, 0, "", "", IndexRaw);
}

function IndexAttribText()
{
    let Attrib1 = [];
    let Attrib2 = [];

    for (let k in FileIndexLS)
    {
        if (Attrib1.indexOf(FileIndexLS[k]) < 0)
        {
            Attrib1.push(k);
            Attrib2.push(FileIndexLSAttr[k]);
        }
    }
    for (let k in FileIndexDB)
    {
        if (Attrib1.indexOf(FileIndexDB[k]) < 0)
        {
            Attrib1.push(k);
            Attrib2.push(FileIndexDBAttr[k]);
        }
    }

    let IndexTxt = "";
    for (var I = 0; I < Attrib1.length; I++)
    {
        IndexTxt = IndexTxt + "~" + Attrib1[I] + "\n"
        IndexTxt = IndexTxt + "~" + Attrib2[I] + "\n"
    }
    IndexTxt = IndexTxt + "\n";
    
    FileIndexToProgram = false;
    return btoa(IndexTxt);
}


function FileGoodName(Str)
{
    return StringASCII(Str, "_", 0);
}


function FileCreatePreview(Data)
{
    let Base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    let Preview = "";
    for (var I = 0; I < Data.length; I += 4)
    {
        let Byte0 = Base64Chars.indexOf(Data.charAt(I + 0));
        let Byte1 = Base64Chars.indexOf(Data.charAt(I + 1));
        let Byte2 = Base64Chars.indexOf(Data.charAt(I + 2));
        let Byte3 = Base64Chars.indexOf(Data.charAt(I + 3));
        
        if ((Byte0 >= 0) && (Byte1 >= 0))
        {
            let CharVal = ASCII(((Byte0 << 2) + (Byte1 >> 4)) & 255, 95);
            Preview = Preview + String.fromCharCode(CharVal);
        }
        if ((Byte1 >= 0) && (Byte2 >= 0))
        {
            let CharVal = ASCII(((Byte1 << 4) + (Byte2 >> 2)) & 255, 95);
            Preview = Preview + String.fromCharCode(CharVal);
        }
        if ((Byte2 >= 0) && (Byte3 >= 0))
        {
            let CharVal = ASCII(((Byte2 << 6) + (Byte3)) & 255, 95);
            Preview = Preview + String.fromCharCode(CharVal);
        }
    }
    return Preview;
}


function FileImportFinish(Id, Kind, Name, Attrib, Data, Err)
{
    if (Err > 0)
    {
        ProgEventOtherFile("FileImport", Name, Id, Kind, 0, Err);
    }
    else
    {
        if (FileIndexToProgram && (Name == FileSystemName))
        {
            Data = IndexAttribText() + Data;
        }
    
        ProgEventOtherFile("FileImport", Name, Id, Kind + 20, 0, Err);
        
        let Data_ = Data;
        while (Data_.length > FileBufSize)
        {
            let DataPart = Data_.substring(0, FileBufSize);

            ProgEventOtherFile("FileImport", DataPart, Id, Kind + 10, 0, Err);

            Data_ = Data_.substring(FileBufSize);
        }
        ProgEventOtherFile("FileImport", Data_, Id, Kind, 0, Err);
    }
}

function FileExportFinish(Id, Kind, Name, Attrib, Data, Err)
{
    ProgEventOther("FileExport", Name, Id, Kind, 0, Err);
}

function FileFetchBlobBase64(blob)
{
    return new Promise((resolve, reject) => {
        const reader = new FileReader;
        reader.onerror = reject;
        reader.onload = () => {
            resolve(reader.result);
        };
        reader.readAsDataURL(blob);
    });
}

function FileFetchTruncateBase64(blob)
{
    let Idx = blob.indexOf(',');
    return blob.substr(Idx + 1);
}

function FileImport(Id, Kind, Name, Attrib)
{
    switch (Kind)
    {
        case 0: // System clipboard
            FileClpCopy(Id);
            break;
        case 1: // File system
            FileSystemGet(Name, Attrib);
            break;
        case 2: // Directory
            if (Attrib == "~RECUR")
            {
                FileSystemDir(Name, 1);
            }
            else
            {
                FileSystemDir(Name, 0);
            }
            break;
        case 6: // File upload
            FileBrwUpload(Name, Attrib);
            break;
    }
}

function FileExport(Id, Kind, Name, Attrib, Data)
{
    switch (Kind)
    {
        case 0: // System clipboard
            FileClpPaste(Id, Data);
            break;
        case 1: // File system
            FileSystemSet(Name, Attrib, Data)
            break;
        case 3: // Delete file or directory
            FileSystemDelete(Name, Attrib);
            break;
        case 6: // File download
            FileBrwDownload(Name, Attrib, Data);
            break;
    }
}

