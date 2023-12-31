// picker was blocked due to lack of user activation.


let FileBrwNameStr = "";
let FileBrwNameLen = 0;
let FileBrwListName = [];
let FileBrwListData = [];


function FileBrwUploadProcess(Name, Data)
{
    FileImportFinish(0, 6, FileBrwNameStr + FileGoodName(Name), "", Data, 0);
    ProgEventTick();
}

async function FileBrwUploadProcessZip2(Name, FileList, FileNames)
{
    Name = Name.substr(0, Name.length - 4);

    for (let F in FileList)
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
    let FileUploadField = document.getElementById("FileUpload");
    
    FileImportFinish(0, 6, "NUMBER|" + FileUploadField.files.length, "", "", 2);
    ProgEventTick();
    for (let I = 0; I < FileUploadField.files.length; I++)
    {
        const FileName = FileUploadField.files[I].name;
        const FR = new FileReader();
        
        FR.onload = function(e)
        {
            if (FileName.toLowerCase().endsWith(".zip"))
            {
                FileBrwUploadProcessZip(FileName, e.target.result.substr(e.target.result.indexOf(',') + 1));
            }
            else
            {
                FileBrwUploadProcess(FileName, e.target.result.substr(e.target.result.indexOf(',') + 1));
            }
        };
        
        FR.readAsDataURL(FileUploadField.files[I]);
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

