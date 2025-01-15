function FileClpCopy(Id)
{
    let Kind = 0;
    if (navigator.clipboard)
    {
        if (navigator.clipboard.readText)
        {
            navigator.clipboard.readText().then(clipText => ProgEventOther("FileImport", clipText, Id, Kind, 0, 0)).catch(error => ProgEventOther("FileImport", "", Id, Kind, 0, 1));
            return;
        }
    }
    ProgEventOther("FileImport", "", Id, Kind, 0, 1);
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
    
    if (navigator.clipboard)
    {
        if (navigator.clipboard.writeText)
        {
            navigator.clipboard.writeText(Data_).then((_) => {}).catch((_) => {});
        }
        else
        {
            console.log(Data_);
        }
    }
    else
    {
        console.log(Data_);
    }
}

