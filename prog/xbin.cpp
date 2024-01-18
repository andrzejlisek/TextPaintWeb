#include "xbin.h"

XBIN::XBIN()
{

}

void XBIN::SetRaw(Raw &Raw_, int RawL, bool IsXBIN)
{
    if (RawL < 0)
    {
        RawL = Raw_.Count;
    }

    DataRaw.Clear();
    DataPal.Clear();
    DataFont1.Clear();
    DataFont2.Clear();
    FileW = -1;
    FileH = -1;
    FontN = 0;
    FontSize = 0;

    DataCompression = false;
    int DataPtr = 0;
    if (IsXBIN)
    {
        // File header
        FileW = (((int)Raw_[6]) << 8) + ((int)Raw_[5]);
        FileH = (((int)Raw_[8]) << 8) + ((int)Raw_[7]);
        FontSize = (int)Raw_[9];
        int FileFlag = (int)Raw_[10];

        DataCompression = ((FileFlag & 4) != 0);
        DataPtr = 11;

        // Color palette
        if ((FileFlag & 1) != 0)
        {
            int PalIdx[16] = { 0x0, 0x4, 0x2, 0x6, 0x1, 0x5, 0x3, 0x7, 0x8, 0xC, 0xA, 0xE, 0x9, 0xD, 0xB, 0xF };
            for (int i = 0; i < 16; i++)
            {
                int DataR = ((int)Raw_[DataPtr + (PalIdx[i] * 3 + 0)]) << 2;
                int DataG = ((int)Raw_[DataPtr + (PalIdx[i] * 3 + 1)]) << 2;
                int DataB = ((int)Raw_[DataPtr + (PalIdx[i] * 3 + 2)]) << 2;

                DataPal.Add(DataR);
                DataPal.Add(DataG);
                DataPal.Add(DataB);
            }
            DataPtr += 48;
        }

        // Embedded font
        if ((FileFlag & 2) != 0)
        {
            FontN = 1;
            int FileFontSize = 256 * FontSize;
            DataFont1.AddRange(Raw_.GetRange(DataPtr, FileFontSize));
            DataPtr += FileFontSize;
            if (((FileFlag & 16) != 0))
            {
                FontN = 2;
                DataFont2.AddRange(Raw_.GetRange(DataPtr, FileFontSize));
                DataPtr += FileFontSize;
            }
        }
    }

    // Image data
    if (DataCompression)
    {
        int FileWH = FileW * FileH * 2;
        while ((DataPtr < RawL) && (DataRaw.Count < FileWH))
        {
            int Rep = (((int)(Raw_[DataPtr])) & 0x3F);
            switch (((int)(Raw_[DataPtr])) & 0xC0)
            {
                case 0x00:
                    {
                        DataPtr++;
                        while (Rep >= 0)
                        {
                            DataRaw.Add(Raw_[DataPtr]);
                            DataPtr++;
                            DataRaw.Add(Raw_[DataPtr]);
                            DataPtr++;
                            Rep--;
                        }
                    }
                    break;
                case 0x40:
                    {
                        DataPtr++;
                        char Temp = Raw_[DataPtr];
                        DataPtr++;
                        while (Rep >= 0)
                        {
                            DataRaw.Add(Temp);
                            DataRaw.Add(Raw_[DataPtr]);
                            DataPtr++;
                            Rep--;
                        }
                    }
                    break;
                case 0x80:
                    {
                        DataPtr++;
                        char Temp = Raw_[DataPtr];
                        DataPtr++;
                        while (Rep >= 0)
                        {
                            DataRaw.Add(Raw_[DataPtr]);
                            DataRaw.Add(Temp);
                            DataPtr++;
                            Rep--;
                        }
                    }
                    break;
                case 0xC0:
                    {
                        DataPtr++;
                        char Temp1 = Raw_[DataPtr];
                        DataPtr++;
                        char Temp2 = Raw_[DataPtr];
                        DataPtr++;
                        while (Rep >= 0)
                        {
                            DataRaw.Add(Temp1);
                            DataRaw.Add(Temp2);
                            Rep--;
                        }
                    }
                    break;
            }
        }
    }
    else
    {
        if ((RawL & 1) != 0)
        {
            RawL--;
        }
        if ((IsXBIN) && (RawL > (FileW * FileH * 2)))
        {
            RawL = (FileW * FileH * 2);
        }
        DataRaw.AddRange(Raw_.GetRange(DataPtr, RawL));
    }
    DataRawLength = DataRaw.Count;
}

void XBIN::GetStr(Str &Str_, int CodecN)
{
    TextCodec Codec(CodecN, 1 + 2);

    Str DataRawS;
    Str_.Clear();
    Codec.Reset();
    Codec.EnqueueRaw(DataRaw);
    Codec.DequeueStr(DataRawS);


    if (DataPal.Count > 0)
    {
        Str_.Add(27);
        Str_.AddString("[2");
        for (int I = 0; I < DataPal.Count; I++)
        {
            Str_.Add(';');
            Str_.AddString(DataPal[I]);
        }
        Str_.AddString("V");
    }

    if (FontN >= 1)
    {
        Raw CharsMapR;
        Str CharsMapS;
        for (int I = 0; I < 256; I++)
        {
            CharsMapR.Add(I);
        }

        Codec.Reset();
        Codec.EnqueueRaw(CharsMapR);
        Codec.DequeueStr(CharsMapS);


        Str_.Add(27);
        Str_.AddString("[3;1;");
        Str_.AddString(FontSize);
        Str_.AddString("V");

        if (DataFont1.Count >= (FontSize * 256))
        {
            int FontPtr = 0;
            for (int I = 0; I < 256; I++)
            {
                Str_.Add(27);
                Str_.AddString("[3;2;");
                Str_.AddString(I);
                Str_.AddString(";");
                Str_.AddString(CharsMapS[I]);
                for (int II = 0; II < FontSize; II++)
                {
                    Str_.AddString(";");
                    Str_.AddString(DataFont1[FontPtr + II]);
                }
                if (((I >= 0xC0) && (I <= 0xDF)))
                {
                    Str_.AddString(";1V");
                }
                else
                {
                    Str_.AddString(";0V");
                }
                FontPtr += FontSize;
            }
        }

        Str_.Add(27);
        Str_.AddString("[3;3");
        Str_.AddString("V");
    }


    Str_.Add(27);
    Str_.AddString("[0m");
    int LastB = -1;
    int LastF = -1;
    bool LastBI = false;
    bool LastFI = false;
    for (int I = 0; I < DataRaw.Count; I += 2)
    {
        int DataAttr = DataRaw[I + 1];

        int NewB = DataAttr & 0x70;
        int NewF = DataAttr & 0x07;
        bool NewBI = ((DataAttr & 0x80) != 0);
        bool NewFI = ((DataAttr & 0x08) != 0);

        if ((NewB != LastB) || (NewF != LastF) || (NewBI != LastBI) || (NewFI != LastFI))
        {
            bool AttrNum = false;
            Str_.Add(27);
            Str_.AddString("[");

            if (LastBI != NewBI)
            {
                if (AttrNum) Str_.AddString(";");

                if (NewBI)
                {
                    Str_.AddString("5");
                }
                else
                {
                    Str_.AddString("25");
                }

                LastBI = NewBI;
                AttrNum = true;
            }

            if (LastB != NewB)
            {
                if (AttrNum) Str_.AddString(";");

                switch (NewB)
                {
                    case 0x00: Str_.AddString("40"); break;
                    case 0x40: Str_.AddString("41"); break;
                    case 0x20: Str_.AddString("42"); break;
                    case 0x60: Str_.AddString("43"); break;
                    case 0x10: Str_.AddString("44"); break;
                    case 0x50: Str_.AddString("45"); break;
                    case 0x30: Str_.AddString("46"); break;
                    case 0x70: Str_.AddString("47"); break;
                    case 0x80: Str_.AddString("100"); break;
                    case 0xC0: Str_.AddString("101"); break;
                    case 0xA0: Str_.AddString("102"); break;
                    case 0xE0: Str_.AddString("103"); break;
                    case 0x90: Str_.AddString("104"); break;
                    case 0xD0: Str_.AddString("105"); break;
                    case 0xB0: Str_.AddString("106"); break;
                    case 0xF0: Str_.AddString("107"); break;
                }

                LastB = NewB;
                AttrNum = true;
            }


            if (LastFI != NewFI)
            {
                if (AttrNum) Str_.AddString(";");

                if (NewFI)
                {
                    Str_.AddString("1");
                }
                else
                {
                    Str_.AddString("22");
                }

                LastFI = NewFI;
                AttrNum = true;
            }

            if (LastF != NewF)
            {
                if (AttrNum) Str_.AddString(";");

                switch (NewF)
                {
                    case 0x00: Str_.AddString("30"); break;
                    case 0x04: Str_.AddString("31"); break;
                    case 0x02: Str_.AddString("32"); break;
                    case 0x06: Str_.AddString("33"); break;
                    case 0x01: Str_.AddString("34"); break;
                    case 0x05: Str_.AddString("35"); break;
                    case 0x03: Str_.AddString("36"); break;
                    case 0x07: Str_.AddString("37"); break;
                    case 0x08: Str_.AddString("90"); break;
                    case 0x0C: Str_.AddString("91"); break;
                    case 0x0A: Str_.AddString("92"); break;
                    case 0x0E: Str_.AddString("93"); break;
                    case 0x09: Str_.AddString("94"); break;
                    case 0x0D: Str_.AddString("95"); break;
                    case 0x0B: Str_.AddString("96"); break;
                    case 0x0F: Str_.AddString("97"); break;
                }

                LastF = NewF;
                AttrNum = true;
            }

            Str_.AddString("m");
        }

        Str_.Add(DataRawS[I]);
    }
    Str_.Add(27);
    Str_.AddString("[0m");
}
