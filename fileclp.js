function FileClpExists()
{
    if (!navigator.clipboard) return false;
    if (!navigator.clipboard.readText) return false;
    if (!navigator.clipboard.writeText) return false;
    return true;
}


function FileClpGuiRepaint()
{
    const FntSizeW = Math.floor((BrowserW() * 1.0) / 40.0);
    const FntSizeH = Math.floor((BrowserH() * 1.0) / 30.0);
    const FntSize = Math.min(FntSizeW, FntSizeH);

    document.getElementById("clipbrd1").style["font-size"] = Math.floor(FntSize / 1) + "px";
    document.getElementById("clipbrd2").style["font-size"] = Math.floor(FntSize / 1) + "px";

    document.getElementById("clipbrdClos").style["font-size"] = FntSize + "px";
    document.getElementById("clipbrdClea").style["font-size"] = FntSize + "px";
    document.getElementById("clipbrdCopy").style["font-size"] = FntSize + "px";
    document.getElementById("clipbrdSend").style["font-size"] = FntSize + "px";
}

function FileClpGuiVisible()
{
    if (document.getElementById("ClpBrdGui").style["display"] == "block")
    {
        return true;
    }
    else
    {
        return false;
    }
}

function FileClpGuiShow()
{
    document.getElementById("ClpBrdGui").style["display"] = "block";
    FileClpGuiRepaint();
}

function FileClpGuiHide()
{
    document.getElementById("ClpBrdGui").style["display"] = "none";
}

function FileClpGuiGet()
{
    return document.getElementById("clipbrd2").value;
}

function FileClpGuiSet(Txt)
{
    document.getElementById("clipbrd1").value = Txt;
}

function FileClpGuiClea()
{
    document.getElementById("clipbrd2").value = "";
}

function FileClpGuiCopy()
{
    document.getElementById("clipbrd2").value = document.getElementById("clipbrd1").value;
}

function FileClpGuiSend()
{
    // Similar to KeybEventTextText from keyboard.js
    const txt = FileClpGuiGet();
    for (let I = 0; I < txt.length; I++)
    {
        let EvtName = "Character";
        let EvtChar = txt.charCodeAt(I);
        if (EvtChar == 13) EvtName = "Enter";
        if ((EvtChar >= 48) && (EvtChar <= 57)) EvtName = "Digit" + String.fromCharCode(EvtChar);
        if ((EvtChar >= 65) && (EvtChar <= 90)) EvtName = "Key" + String.fromCharCode(EvtChar);
        if ((EvtChar >= 97) && (EvtChar <= 122)) EvtName = "Key" + String.fromCharCode(EvtChar - 32);
        KeybEvent([0, 0, 0, 0, EvtName, "", "", EvtChar]);
    }
    FileClpGuiHide();
}

function FileClpCopy(Id)
{
    let Kind = 0;
    if (FileClpExists())
    {
        navigator.clipboard.readText().then(clipText => ProgEventOther("FileImport", clipText, Id, Kind, 0, 0)).catch(error => ProgEventOther("FileImport", "", Id, Kind, 0, 1));
        return;
    }
    else
    {
        const txt = FileClpGuiGet();
        if (txt.trim().length > 0)
        {
            ProgEventOther("FileImport", txt, Id, Kind, 0, 0)
        }
        else
        {
            ProgEventOther("FileImport", "", Id, Kind, 0, 1);
        }
    }
}

function FileClpPaste(Id, Data)
{
    let Kind = 0;
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
                                Data_ = Data_ + String_fromCharCode(CharCode);

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
    ProgEventOther("FileExport", Data_, Id, Kind, 0, 0);


    if (FileClpExists())
    {
        navigator.clipboard.writeText(Data_).then((_) => {}).catch((_) => {});
    }
    else
    {
        FileClpGuiShow();
        FileClpGuiSet(Data_);
    }
}

