function FileBrwUpload()
{
    let Fld = document.getElementById("FileUpload");
    Fld.click();
}

function FileBrwUploadEvent()
{
    let Fld = document.getElementById("FileUpload");
    
    let FR = new FileReader();
    FR.X = Fld.files[0].name;
    FR.onload = function(e)
    {
        let Idx = e.target.result.indexOf(',');
        let FileName = e.target.X
        let FileData = e.target.result.substr(Idx + 1);

        console.log("Ladowanie pliku - nazwa  " + FileName);
        console.log("Ladowanie pliku - tresc  " + FileData);
    };
    FR.readAsDataURL(Fld.files[0]);
}

function FileBrwDownload(Name, Data)
{
    const DataStr = atob(Data);
    const DataNum = new Array(DataStr.length);
    for (let i = 0; i < DataStr.length; i++)
    {
        DataNum[i] = DataStr.charCodeAt(i);
    }
    const DataBlob = new Blob([new Uint8Array(DataNum)], {type: "application/octet-stream"});

    var ALink = document.getElementById("FileDownload");
    ALink.download = Name;
    ALink.href = URL.createObjectURL(DataBlob);
    ALink.click();
}

