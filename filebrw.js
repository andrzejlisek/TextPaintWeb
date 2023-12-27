// picker was blocked due to lack of user activation.


let FileBrwNameStr = "";
let FileBrwNameLen = 0;
let FileBrwListName = [];
let FileBrwListData = [];


function FileBrwIsZip(Name)
{
    if (Name.length > 4)
    {
        if (Name.substr(Name.length - 4).toLowerCase() == ".zip")
        {
            return true;
        }
    }
    return false;
}




function FileBrwUploadProcess(Name, Data)
{
    FileImportFinish(0, 6, FileBrwNameStr + FileGoodName(Name), "", Data, 0);
    ProgEventTick();
}

async function FileBrwUploadProcessZip2(Name, FileList, FileNames)
{
    Name = Name.substr(0, Name.length - 4);

    for (F in FileList)
    {
        if (F.length > 0)
        {
            if (F.charAt(F.length - 1) != '/')
            {
                await FileList[F].async("base64").then(function (XData)
                {
                    FileBrwUploadProcess(Name + "/" + F, XData);
                });
            }
        }
    }
}

function FileBrwUploadProcessZip(Name, Data)
{
    let zip = new JSZip();
    let FileNames = [];
    zip.loadAsync(DataTextToBlob(Data)).then(function (zip)
    {
        for (F in zip.files)
        {
            if (F.length > 0)
            {
                if (F.charAt(F.length - 1) != '/') { FileNames.push(F); }
            }
        }
        
        let NumAdd = FileNames.length;
        NumAdd = NumAdd - 1;
        FileImportFinish(0, 6, "NUMADD|" + NumAdd, "", "", 2);
        ProgEventTick();
        
        FileBrwUploadProcessZip2(Name, zip.files, FileNames);
    });
}

function FileBrwUpload(Name, Attrib)
{
    FileBrwNameStr = Name;
    let Fld = document.getElementById("FileUpload");
    Fld.click();
}

function FileBrwUploadEvent()
{
    FileBrwListName = [];
    FileBrwListData = [];
    let Fld = document.getElementById("FileUpload");
    
    FileImportFinish(0, 6, "NUMBER|" + Fld.files.length, "", "", 2);
    ProgEventTick();
    for (var I = 0; I < Fld.files.length; I++)
    {
        let FR = new FileReader();
        
        FR.X = Fld.files[I].name;
        
        FR.onload = function(e)
        {
            if (FileBrwIsZip(e.target.X))
            {
                FileBrwUploadProcessZip(e.target.X, e.target.result.substr(e.target.result.indexOf(',') + 1));
            }
            else
            {
                FileBrwUploadProcess(e.target.X, e.target.result.substr(e.target.result.indexOf(',') + 1));
            }
        };
        
        FR.readAsDataURL(Fld.files[I]);
    }
}












function FileBrwDownload(Name, Attrib, Data)
{

    if (Attrib == "~BEGIN")
    {
        FileBrwNameLen = Name.lastIndexOf('/') + 1;
        if (FileBrwNameLen > 0)
        {
            FileBrwNameStr = Name.substr(FileBrwNameLen);
        }
        else
        {
            FileBrwNameStr = Name;
        }
        FileBrwListName = [];
        FileBrwListData = [];
    }
    if (Attrib == "~END")
    {
        if (FileBrwListName.length == 1)
        {
            let ALink = document.getElementById("FileDownload");
            ALink.download = FileBrwNameStr;
            ALink.href = URL.createObjectURL(DataTextToBlob(FileBrwListData[0]));
            ALink.click();
        }
        if (FileBrwListName.length > 1)
        {
            var ZIP = new JSZip();
        
        
            for (var I = 0; I < FileBrwListData.length; I++)
            {
                ZIP.file(FileBrwListName[I].substr(FileBrwNameStr.length + 1), DataTextToBlob(FileBrwListData[I]));
            }
            FileBrwNameStr = FileBrwNameStr + ".zip";
        
            ZIP.generateAsync({type : "base64"}).then((ZipData) => {
                let ALink = document.getElementById("FileDownload");
                ALink.download = FileBrwNameStr;
                ALink.href = URL.createObjectURL(DataTextToBlob(ZipData));
                ALink.click();
            });
        }
    }
    if ((Attrib != "~BEGIN") && (Attrib != "~END"))
    {
        if (FileBrwNameLen > 0)
        {
            Name = Name.substr(FileBrwNameLen);
        }
        FileBrwListName.push(Name);
        FileBrwListData.push(Data);
    }
}

