#include "ansisauce.h"

AnsiSauce::AnsiSauce()
{

}

void AnsiSauce::NonSauceInfo(std::string Label, long Value)
{
    NonSauceInfo(Label, std::to_string(Value));
}

void AnsiSauce::NonSauceInfo(std::string Label, std::string Value)
{
    NonSauceInfo1.Add(Label);
    NonSauceInfo2.Add(Value);
}

void AnsiSauce::CreateInfo()
{
    InfoRaw.Clear();
    Exists = false;
    SauceIdx = -1;
    CommentIdx = -1;

    if (SauceIdx < 0)
    {
        SauceIdx = Raw_.Count - 129;
        if (GetByte(SauceIdx + 1) != 'S') { SauceIdx = -1; }
        if (GetByte(SauceIdx + 2) != 'A') { SauceIdx = -1; }
        if (GetByte(SauceIdx + 3) != 'U') { SauceIdx = -1; }
        if (GetByte(SauceIdx + 4) != 'C') { SauceIdx = -1; }
        if (GetByte(SauceIdx + 5) != 'E') { SauceIdx = -1; }
    }

    Field01Version = "";
    Field02Title = "";
    Field03Author = "";
    Field04Group = "";
    Field05Date = "";
    Field06FileSize = 0;
    Field07DataType = 0;
    Field08FileType = 0;
    Field09TInfo1 = 0;
    Field10TInfo2 = 0;
    Field11TInfo3 = 0;
    Field12TInfo4 = 0;
    Field13Comments = 0;
    Field14TFlags = 0;
    Field15TInfoS = "";
    Comment.Clear();
    Info.Clear();

    for (int i = 0; i < NonSauceInfo1.Count; i++)
    {
        Info.Add(NonSauceInfo1[i] + ": " + NonSauceInfo2[i]);
    }
    Info.Add("");

    if (SauceIdx >= 0)
    {
        Exists = true;
        ReadSAUCE();
        if (Field13Comments > 0)
        {
            CommentIdx = SauceIdx - (64 * Field13Comments) - 5;
            while ((CommentIdx + 64) < 0)
            {
                CommentIdx += 64;
            }
            ReadCOMNT();
        }

        std::string Info_;

        Info.Add("Version: " + (Field01Version));
        Info.Add("Title: " + (Field02Title));
        Info.Add("Author: " + (Field03Author));
        Info.Add("Group: " + (Field04Group));
        Info.Add("Date: " + (Field05Date));
        Info.Add("FileSize: " + std::to_string(Field06FileSize));
        if (Field07DataType <= 8)
        {
            Info.Add("DataType: " + std::to_string(Field07DataType) + " -> " + NameDataType[Field07DataType]);
            XList<std::string> Name_;
            int CopyIdx = 0;
            switch (Field07DataType)
            {
                case 0:
                    while (NameFileType0[CopyIdx] != "_")
                    {
                        Name_.Add(NameFileType0[CopyIdx]);
                        CopyIdx++;
                    }
                    break;
                case 1:
                    while (NameFileType1[CopyIdx] != "_")
                    {
                        Name_.Add(NameFileType1[CopyIdx]);
                        CopyIdx++;
                    }
                    break;
                case 2:
                    while (NameFileType2[CopyIdx] != "_")
                    {
                        Name_.Add(NameFileType2[CopyIdx]);
                        CopyIdx++;
                    }
                    break;
                case 3:
                    while (NameFileType3[CopyIdx] != "_")
                    {
                        Name_.Add(NameFileType3[CopyIdx]);
                        CopyIdx++;
                    }
                    break;
                case 4:
                    while (NameFileType4[CopyIdx] != "_")
                    {
                        Name_.Add(NameFileType4[CopyIdx]);
                        CopyIdx++;
                    }
                    break;
                case 5:
                    while (NameFileType5[CopyIdx] != "_")
                    {
                        Name_.Add(NameFileType5[CopyIdx]);
                        CopyIdx++;
                    }
                    break;
                case 6:
                    while (NameFileType6[CopyIdx] != "_")
                    {
                        Name_.Add(NameFileType6[CopyIdx]);
                        CopyIdx++;
                    }
                    break;
                case 7:
                    while (NameFileType7[CopyIdx] != "_")
                    {
                        Name_.Add(NameFileType7[CopyIdx]);
                        CopyIdx++;
                    }
                    break;
                case 8:
                    while (NameFileType8[CopyIdx] != "_")
                    {
                        Name_.Add(NameFileType8[CopyIdx]);
                        CopyIdx++;
                    }
                    break;
            }
            if (Field08FileType < Name_.Count)
            {
                Info.Add("FileType: " + std::to_string(Field08FileType) + " -> " + Name_[Field08FileType]);
            }
            else
            {
                Info.Add("FileType: " + std::to_string(Field08FileType));
            }
        }
        else
        {
            Info.Add("DataType: " + std::to_string(Field07DataType));
            Info.Add("FileType: " + std::to_string(Field08FileType));
        }
        Info.Add("Info number 1: " + std::to_string(Field09TInfo1));
        Info.Add("Info number 2: " + std::to_string(Field10TInfo2));
        Info.Add("Info number 3: " + std::to_string(Field11TInfo3));
        Info.Add("Info number 4: " + std::to_string(Field12TInfo4));
        Info.Add("Comment lines: " + std::to_string(Field13Comments));
        Info_ = "Flags: ";
        Info_ = Info_ + (((Field14TFlags & 128) > 0) ? "1" : "0");
        Info_ = Info_ + (((Field14TFlags & 64) > 0) ? "1" : "0");
        Info_ = Info_ + (((Field14TFlags & 32) > 0) ? "1" : "0");
        Info_ = Info_ + (((Field14TFlags & 16) > 0) ? "1" : "0");
        Info_ = Info_ + (((Field14TFlags & 8) > 0) ? "1" : "0");
        Info_ = Info_ + (((Field14TFlags & 4) > 0) ? "1" : "0");
        Info_ = Info_ + (((Field14TFlags & 2) > 0) ? "1" : "0");
        Info_ = Info_ + (((Field14TFlags & 1) > 0) ? "1" : "0");
        Info_ = Info_ + " -> ";
        switch (Field14TFlags & (16 + 8))
        {
            case 0:
                Info_ = Info_ + "AR:none, ";
                break;
            case 8:
                Info_ = Info_ + "AR:legacy, ";
                break;
            case 16:
                Info_ = Info_ + "AR:modern, ";
                break;
            case 24:
                Info_ = Info_ + "AR:unknown, ";
                break;
        }
        switch (Field14TFlags & (4 + 2))
        {
            case 0:
                Info_ = Info_ + "Sp:legacy, ";
                break;
            case 2:
                Info_ = Info_ + "Sp:8px, ";
                break;
            case 4:
                Info_ = Info_ + "Sp:9px, ";
                break;
            case 6:
                Info_ = Info_ + "Sp:unknown, ";
                break;
        }
        switch (Field14TFlags & 1)
        {
            case 0:
                Info_ = Info_ + "Bk:7+blink ";
                break;
            case 1:
                Info_ = Info_ + "Bk:16 ";
                break;
        }
        Info.Add(Info_);
        Info.Add("Text info: " + (Field15TInfoS));

        std::string CommentNum1 = "";
        std::string CommentNum2 = "";
        std::string CommentNum3 = "";
        if ((Comment.Count >= 10))
        {
            CommentNum1 = " ";
        }
        if (Comment.Count >= 100)
        {
            CommentNum1 = "  ";
            CommentNum2 = " ";
        }
        for (int i = 0; i < std::min(Comment.Count, 9); i++)
        {
            Info.Add("Comment " + CommentNum1 + std::to_string(i + 1) + ": " + (Comment[i]));
        }
        for (int i = 9; i < std::min(Comment.Count, 99); i++)
        {
            Info.Add("Comment " + CommentNum2 + std::to_string(i + 1) + ": " + (Comment[i]));
        }
        for (int i = 99; i < Comment.Count; i++)
        {
            Info.Add("Comment " + CommentNum3 + std::to_string(i + 1) + ": " + (Comment[i]));
        }
    }
    else
    {
        Info.Add("SAUCE information not exists");
    }
}

void AnsiSauce::LoadRaw(Raw &Raw__)
{
    NonSauceInfo1.Clear();
    NonSauceInfo2.Clear();
    Raw_ = Raw__;
}

int AnsiSauce::GetRawNum(Str FileCtx, int Idx)
{
    if (FileCtx.Count > Idx)
    {
        return FileCtx[Idx];
    }
    return 0;
}

std::string AnsiSauce::GetRawChar(Str FileCtx, int Idx)
{
    if (FileCtx.Count > Idx)
    {
        return Str::IntToStr(FileCtx[Idx]);
    }
    return "_";
}

std::string AnsiSauce::GetChar(int Idx)
{
    unsigned char X = (unsigned char)GetByte(Idx);
    if ((X >= 32) && (X <= 127))
    {
        return Str::IntToStr(X);
    }
    else
    {
        if ((X == 0) || (X == 8) || (X == 9) || (X == 10) || (X == 13))
        {
            return " ";
        }
        else
        {
            return ".";
        }
    }
}

int AnsiSauce::GetByte(int Idx)
{
    if ((Idx >= 0) && (Raw_.Count > Idx))
    {
        return Raw_[Idx];
    }
    return 0;
}

void AnsiSauce::ReadSAUCE()
{
    for (int i = 0; i < (Raw_.Count - SauceIdx - 1); i++)
    {
        InfoRaw.Add(Raw_[SauceIdx + 1 + i]);
    }

    int Ptr = SauceIdx + 6;
    for (int i = 0; i < 2; i++)
    {
        Field01Version = Field01Version + GetChar(Ptr);
        Ptr++;
    }
    for (int i = 0; i < 35; i++)
    {
        Field02Title = Field02Title + GetChar(Ptr);
        Ptr++;
    }
    for (int i = 0; i < 20; i++)
    {
        Field03Author = Field03Author + GetChar(Ptr);
        Ptr++;
    }
    for (int i = 0; i < 20; i++)
    {
        Field04Group = Field04Group + GetChar(Ptr);
        Ptr++;
    }
    for (int i = 0; i < 8; i++)
    {
        Field05Date = Field05Date + GetChar(Ptr);
        Ptr++;
    }
    Field06FileSize += ((long)GetByte(Ptr + 0));
    Field06FileSize += ((long)GetByte(Ptr + 1) * 256L);
    Field06FileSize += ((long)GetByte(Ptr + 2) * 65536L);
    Field06FileSize += ((long)GetByte(Ptr + 3) * 16777216L);
    Ptr += 4;
    Field07DataType += GetByte(Ptr);
    Ptr += 1;
    Field08FileType += GetByte(Ptr);
    Ptr += 1;
    Field09TInfo1 += (GetByte(Ptr + 0));
    Field09TInfo1 += (GetByte(Ptr + 1) * 256);
    Ptr += 2;
    Field10TInfo2 += (GetByte(Ptr + 0));
    Field10TInfo2 += (GetByte(Ptr + 1) * 256);
    Ptr += 2;
    Field11TInfo3 += (GetByte(Ptr + 0));
    Field11TInfo3 += (GetByte(Ptr + 1) * 256);
    Ptr += 2;
    Field12TInfo4 += (GetByte(Ptr + 0));
    Field12TInfo4 += (GetByte(Ptr + 1) * 256);
    Ptr += 2;
    Field13Comments += GetByte(Ptr);
    Ptr += 1;
    Field14TFlags += GetByte(Ptr);
    Ptr += 1;
    for (int i = 0; i < 22; i++)
    {
        Field15TInfoS = Field15TInfoS + GetChar(Ptr);
        Ptr++;
    }
}

void AnsiSauce::ReadCOMNT()
{
    for (int i = 0; i < (Raw_.Count - CommentIdx - 1); i++)
    {
        InfoRaw.Add(Raw_[CommentIdx + 1 + i]);
    }

    int Ptr = CommentIdx + 6;
    while (true)
    {
        std::string CommentLine = "";
        for (int i = 0; i < 64; i++)
        {
            CommentLine = CommentLine + GetChar(Ptr + i);
        }
        Comment.Add(CommentLine);
        Ptr += 64;
        if (Ptr >= Raw_.Count)
        {
            break;
        }
        if ((SauceIdx >= 0) && (Ptr >= SauceIdx))
        {
            break;
        }
    }
}
