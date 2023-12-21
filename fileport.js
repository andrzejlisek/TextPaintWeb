let FileNameSystem = ["!!config.txt", "!!system.txt"];
let FileBufSize = 512;

let FileIndexLS = {};
let FileIndexLSNum = 1;
let FileIndexName = "TextPaintFileIndex";
let FileIndexTempl = "TextPaintFile";
let FileIndexSepa = "\n";

let ConfigFileJS = {};


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

function IndexImportLS()
{
    FileIndexLS = {};
    if (DataExists(FileIndexName))
    {
        let IndexRaw = DataGet(FileIndexName).split(FileIndexSepa);
        for (let I = 0; I < IndexRaw.length; I += 2)
        {
            if (IndexRaw.length > (I + 1))
            {
                if (IndexRaw[I] && (IndexRaw[I] != ""))
                {
                    let N = parseInt(IndexRaw[I + 1]);
                    if (DataExists(FileIndexTempl + N))
                    {
                        if (FileIndexLSNum <= N)
                        {
                            FileIndexLSNum = N + 1;
                        }
                        FileIndexLS[IndexRaw[I]] = N;
                    }
                }
            }
        }
    }
}

function IndexExportLS()
{
    let IndexRaw = "";
    for (let k in FileIndexLS)
    {
        IndexRaw = IndexRaw + k + FileIndexSepa + FileIndexLS[k] + FileIndexSepa;
    }
    DataSet(FileIndexName, IndexRaw);
}

IndexImportLS();

function FileImportFinish(Id, Kind, Name, Data, Err)
{
    if (Err > 0)
    {
        ProgEventOtherFile("FileImport", Data, Id, Kind, 0, 1);
    }
    else
    {
        ProgEventOtherFile("FileImport", "", Id, Kind + 20, 0, 0);
        
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

function FileFetch(Id, Kind, Name)
{
    let file = "files/" + Name;
    //fetch(file)
    //.then(x => x.text())
    //.then(y => FileImportFinish(Id, Kind, Name, btoa(y), 0));


    fetch(file)
    .then(_1 => _1.blob())
    .then(_2 => FileFetchBlobBase64(_2))
    .then(_3 => FileImportFinish(Id, Kind, Name, FileFetchTruncateBase64(_3), 0));
}    




function FileImport(Id, Kind, Name)
{
    switch (Kind)
    {
        case 0: // System clipboard
            if (navigator.clipboard.readText)
            {
                navigator.clipboard.readText().then(clipText => ProgEventOther("FileImport", clipText, Id, Kind, 0, 0)).catch(error => ProgEventOther("FileImport", "", Id, Kind, 0, 1));
            }
            else
            {
                ProgEventOther("FileImport", "", Id, Kind, 0, 1);
            }
            break;
        case 1: // Local storage
        case 4: // Local storage - if not exists, then fetch
            if (FileIndexLS[Name])
            {
                FileIdx = FileIndexLS[Name];
                let Data = DataGet(FileIndexTempl + FileIdx);
                FileImportFinish(Id, Kind, Name, Data, 0);
            }
            else
            {
                if (Kind == 1)
                {
                    FileImportFinish(Id, Kind, Name, "", 1);
                }
                if (Kind == 4)
                {
                    FileFetch(Id, Kind, Name);
                }
            }
            break;
        case 2: // Database
        case 5: // Database - if not exists, then fetch
            if (Kind == 5)
            {
                FileFetch(Id, Kind, Name);
            }
            break;
        case 3: // Fetch - read only
            {
                FileFetch(Id, Kind, Name);
            }
            break;
        case 9: // System file, behavior depends on file name
            if (Name == FileNameSystem[0])
            {
                if (FileIndexLS[Name])
                {
                    FileIdx = FileIndexLS[Name];
                    let Data = DataGet(FileIndexTempl + FileIdx);
                    FileSystemFetch(Id, Name, Data, Kind);
                }
                else
                {
                    let file = "files/" + Name;
                    fetch(file)
                    .then(x => x.text())
                    .then(y => FileSystemFetch(Id, Name, btoa(y), Kind));
                }
            }
            if (Name == FileNameSystem[1])
            {
                let file = "files/" + Name;
                fetch(file)
                .then(x => x.text())
                .then(y => FileSystemFetch(Id, Name, btoa(y), Kind));
            }
            break;
    }
}

let FileSystem0 = false;
let FileSystem1 = false;

function FileSystemFetch(Id, Name, Data, Kind)
{
    if (Name == FileNameSystem[0])
    {
        FileImportFinish(Id, 9, Name, Data, 0);
        FileSystem0 = true;
    }
    if (Name == FileNameSystem[1])
    {
        FileImportFinish(Id, 9, Name, Data, 0);
        FileSystem1 = true;
    }
}

function FileExport(Id, Kind, Name, Data)
{
    if (Kind >= 100)
    {
        FileDelete(Id, Kind - 100, Name);
        return;
    }
    
    switch (Kind)
    {
        case 0: // System clipboard
            if (navigator.clipboard.writeText)
            {
                let Data_ = "";
                let DataNum = false;
                let RawDataNum = 0;
                let Temp = [0, 0, 0, 0, 0, 0, 0, 0];
                let DequeueTextState = 0;
                for (let I = 0; I < Data.length; I++)
                {
                    if (DataNum)
                    {
                        if (Data[I] == ',')
                        {
                            DataNum = false;
                            switch (DequeueTextState)
                            {
                                case 0:
                                    if (RawDataNum < 128)
                                    {
                                        Data_ = Data_ + String.fromCharCode(RawDataNum);
                                    }
                                    else
                                    {
                                        // 110_____
                                        if ((RawDataNum >= 0xC0) && (RawDataNum <= 0xDF))
                                        {
                                            DequeueTextState = 1;
                                            Temp[1] = RawDataNum & 0x1F;
                                        }

                                        // 1110____
                                        if ((RawDataNum >= 0xE0) && (RawDataNum <= 0xEF))
                                        {
                                            DequeueTextState = 2;
                                            Temp[2] = RawDataNum & 0x0F;
                                        }

                                        // 11110___
                                        if ((RawDataNum >= 0xF0) && (RawDataNum <= 0xF7))
                                        {
                                            DequeueTextState = 3;
                                            Temp[3] = RawDataNum & 0x07;
                                        }

                                        // 111110__
                                        if ((RawDataNum >= 0xF8) && (RawDataNum <= 0xFB))
                                        {
                                            DequeueTextState = 4;
                                            Temp[4] = RawDataNum & 0x03;
                                        }

                                        // 1111110_
                                        if ((RawDataNum >= 0xFC) && (RawDataNum <= 0xFD))
                                        {
                                            DequeueTextState = 5;
                                            Temp[5] = RawDataNum & 0x01;
                                        }
                                    }
                                    break;
                                case 1:
                                case 2:
                                case 3:
                                case 4:
                                case 5:
                                    if ((RawDataNum >= 0x80) && (RawDataNum <= 0xBF))
                                    {
                                        Temp[5] = Temp[5] << 6;
                                        Temp[4] = Temp[4] << 6;
                                        Temp[3] = Temp[3] << 6;
                                        Temp[2] = Temp[2] << 6;
                                        Temp[1] = Temp[1] << 6;
                                        Temp[0] = Temp[0] << 6;

                                        DequeueTextState--;
                                        Temp[DequeueTextState] = (RawDataNum & 0x3F);
                                        if (DequeueTextState == 0)
                                        {
                                            let CharCode = Temp[5] + Temp[4] + Temp[3] + Temp[2] + Temp[1] + Temp[0];
                                            if (CharCode <= 65535)
                                            {
                                                Data_ = Data_ + String.fromCharCode(CharCode);
                                            }

                                            Temp[0] = 0;
                                            Temp[1] = 0;
                                            Temp[2] = 0;
                                            Temp[3] = 0;
                                            Temp[4] = 0;
                                            Temp[5] = 0;
                                        }
                                    }
                                    else
                                    {
                                        DequeueTextState = 0;
                                        Temp[0] = 0;
                                        Temp[1] = 0;
                                        Temp[2] = 0;
                                        Temp[3] = 0;
                                        Temp[4] = 0;
                                        Temp[5] = 0;
                                        I--;
                                    }
                                    break;
                            }
                        }
                        else
                        {
                            RawDataNum = RawDataNum * 10;
                            RawDataNum = RawDataNum + (Data.charCodeAt(I) - 48);
                        }
                    }
                    else
                    {
                        if (Data[I] == '_')
                        {
                            DataNum = true;
                            RawDataNum = 0;
                        }
                        else
                        {
                            Data_ = Data_ + Data[I];
                        }
                    }
                }
                navigator.clipboard.writeText(Data_);
                ProgEventOther("FileExport", Data_, Id, Kind, 0, 0);
            }
            break;
        case 1: // Local storage
        case 4: // Local storage - if not exists, then fetch
            {
                let FileIdx = 0;
                if (FileIndexLS[Name])
                {
                    FileIdx = FileIndexLS[Name];
                }
                else
                {
                    FileIdx = FileIndexLSNum;
                    FileIndexLS[Name] = FileIdx;
                    FileIndexLSNum++;
                    IndexExportLS();
                }
                DataSet(FileIndexTempl + FileIdx, Data);
                if (Kind == 1)
                {
                    ProgEventOther("FileExport", "", Id, Kind, 0, 0);
                }
            }
            break;
        case 2: // Database
        case 5: // Database - if not exists, then fetch
            break;
        case 3: // Fetch - not possible
            break;
    }
}

function FileDelete(Id, Kind, Name)
{
    switch (Kind)
    {
        case 0: // System clipboard - not possible
            break;
        case 1: // Local storage
        case 4: // Local storage - if not exists, then fetch
        case 9: // System file, depends on file name
            {
                let FileIdx = 0;
                if ((Kind != 9) || (Name == FileNameSystem[0]))
                {
                    if (FileIndexLS[Name])
                    {
                        FileIdx = FileIndexLS[Name];
                        FileIdx = FileIndexLS[Name];
                        DataDelete(FileIndexTempl + FileIdx);
                        delete FileIndexLS[Name];
                        IndexExportLS();
                    }
                }
                if (Kind != 9)
                {
                    ProgEventOther("FileExport", "", Id, Kind, 0, 0);
                }
            }
            break;
        case 2: // Database
        case 5: // Database - if not exists, then fetch
            break;
        case 3: // Fetch - not possible
            break;
    }
}

