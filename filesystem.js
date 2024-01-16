let FetchFilesN = ["!!config.txt"];

function FileSystemInit()
{
    FetchFilesN = ["!!config.txt"];

    let N = ConfigFileI("FileN");
    for (let I = 1; I < N; I++)
    {
        FetchFilesN.push(ConfigFileS("File" + I));
    }
}


let FileDb;
let FileDbName = "Files";
let FileLastWord = String.fromCharCode(255);

function FileDbOpen()
{
    const Req = window.indexedDB.open('TextPaint', 1);

    Req.addEventListener('error', () => console.error('FileDb: Database failed to open'));

    Req.addEventListener('success', () => {
        FileDb = Req.result;
        FileDbOpenComplete();
    });

    Req.addEventListener('upgradeneeded', e => {
        FileDb = e.target.result;

        const ObjStore = FileDb.createObjectStore(FileDbName, { keyPath: 'id' });

        FileDbOpenComplete();
    });
}

function FileDbOpenComplete(GetIndex)
{
    FileDbReady = true;
    ProgInit();
}


function FileDirFromName(FileName)
{
    const I = FileName.lastIndexOf("/");
    if (I >= 0)
    {
        return FileName.substr(0, I + 1);
    }
    else
    {
        return "";
    }
}


function FileDirItemName(RootDir, FileName, Recur)
{
    if (RootDir == FileDirFromName(FileName))
    {
        return FileName;
    }
    else
    {
        if (FileName.startsWith(RootDir))
        {
            const I1 = FileName.indexOf("/", RootDir.length + 1);
            if ((I1 >= 0) && (!Recur))
            {
                return FileName.substr(0, I1 + 1);
            }
            else
            {
                return FileName;
            }
        }
    }
    return "";
}


function FileSystemDir(Name, Recursive)
{
    const ObjStore = FileDb.transaction(FileDbName).objectStore(FileDbName);

    let DirList = [];
    let DirListStr = "";

    ObjStore.openCursor(IDBKeyRange.bound(Name, Name + FileLastWord, false, true)).addEventListener('success', e => {

        const cursor = e.target.result;
        let FoundName = "";
        if(cursor)
        {
            FoundName = FileDirItemName(Name, cursor.value.name, Recursive);
            if ((FoundName != "") && (DirList.indexOf(FoundName) < 0))
            {
                DirList.push(FoundName);
                DirListStr = DirListStr + FoundName + "\n";
            }
            cursor.continue();
        }
        else
        {
            for (let I = 0; I < FetchFilesN.length; I++)
            {
                FoundName = FileDirItemName(Name, FetchFilesN[I], Recursive);
                if ((FoundName != "") && (DirList.indexOf(FoundName) < 0))
                {
                    DirList.push(FoundName);
                    DirListStr = DirListStr + FoundName + "\n";
                }
            }
            
            if (Recursive)
            {
                FileImportFinish(0, 3, Name, "~RECURS", btoa(DirListStr), 0);
            }
            else
            {
                FileImportFinish(0, 2, Name, "~DIR", btoa(DirListStr), 0);
            }
        }
    });
}


function FileSystemGet(Name, Attrib)
{
    const ObjStore = FileDb.transaction(FileDbName).objectStore(FileDbName);
    let NoDataRead = true;
    ObjStore.openCursor(IDBKeyRange.bound(Name, Name)).addEventListener('success', e => {

        const cursor = e.target.result;
        if(cursor)
        {
            NoDataRead = false;
            FileImportFinish(0, 1, Name, Attrib, cursor.value.data, 0);
            cursor.continue();
        }
        else
        {
            if (NoDataRead)
            {
                for (let I = 0; I < FetchFilesN.length; I++)
                {
                    if (Name == FetchFilesN[I])
                    {
                        let file = "files/" + Name;
                        fetch(file)
                        .then(_1 => _1.blob())
                        .then(_2 => FileFetchBlobBase64(_2))
                        .then(_3 => FileImportFinish(0, 1, Name, Attrib, FileFetchTruncateBase64(_3), 0));
                    }
                }
            }
        }
    });
}


function FileSystemSet(Name, Attrib, Data)
{
    let Preview = FileCreatePreview(Data);
    
    const Item = { id: Name, name: Name, data: Data, preview: Preview };
    const DbTrans = FileDb.transaction([FileDbName], 'readwrite');
    const ObjStore = DbTrans.objectStore(FileDbName);
    const Req = ObjStore.put(Item);

    Req.addEventListener('success', () => {
    });

    DbTrans.addEventListener('complete', () => {
        FileExportFinish(0, 1, Name, Attrib, Data, 0);
    });

    DbTrans.addEventListener('error', () => {
        FileExportFinish(0, 1, Name, Attrib, Data, 1);
    });
}


function FileSystemDelete(Name, Attrib)
{
    const DbTrans = FileDb.transaction([FileDbName], 'readwrite');
    const ObjStore = DbTrans.objectStore(FileDbName);
    if (Name.endsWith("/"))
    {
        const Req = ObjStore.delete(IDBKeyRange.bound(Name, Name + FileLastWord, false, true));
    }
    else
    {
        const Req = ObjStore.delete(IDBKeyRange.bound(Name, Name));
    }

    DbTrans.addEventListener('complete', () => {
        FileExportFinish(0, 3, Name, Attrib, "", 0);
    });
}





