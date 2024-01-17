#include "binaryfile.h"

BinaryFile::BinaryFile()
{
    CurrentFileName = Str::FromString(SystemFileName);
    CurrentFileAttr = Str::FromString(SystemFileAttr);

    B64 = std::make_unique<TextCodec>(TextCodec::BASE64);

    ItemListClear();
}

void BinaryFile::Init(std::shared_ptr<ConfigFile> CF)
{
    XList<std::string> Temp;

    Temp.Clear();
    TextWork::StringSplit(CF.get()->ParamGetS("FileExtText"), ',', Temp);
    FileExtText.Clear();
    for (int I = 0; I < Temp.Count; I++)
    {
        FileExtText.Add(Str::FromString("." + Temp[I]).CaseLower());
    }

    Temp.Clear();
    TextWork::StringSplit(CF.get()->ParamGetS("FileExtAnsi"), ',', Temp);
    FileExtAnsi.Clear();
    for (int I = 0; I < Temp.Count; I++)
    {
        FileExtAnsi.Add(Str::FromString("." + Temp[I]).CaseLower());
    }

    Temp.Clear();
    TextWork::StringSplit(CF.get()->ParamGetS("FileExtBin"), ',', Temp);
    FileExtBin.Clear();
    for (int I = 0; I < Temp.Count; I++)
    {
        FileExtBin.Add(Str::FromString("." + Temp[I]).CaseLower());
    }

    Temp.Clear();
    TextWork::StringSplit(CF.get()->ParamGetS("FileExtXbin"), ',', Temp);
    FileExtXbin.Clear();
    for (int I = 0; I < Temp.Count; I++)
    {
        FileExtXbin.Add(Str::FromString("." + Temp[I]).CaseLower());
    }
}

void BinaryFile::ItemListClear()
{
    ListItemsN.Clear();
    ListItemsN.Add(SystemFileName);
}

void BinaryFile::ItemAdd(Str FileName)
{
    int Pos = ListItemsN.IndexOfBin(FileName);
    if (Pos < 0)
    {
        ListItemsN.AddSortByIndex(Pos, FileName);
    }
}

void BinaryFile::ItemFileAdd(Str FileName)
{
    if (FileName.Count == 0)
    {
        return;
    }
    if (ListDir.Count > 0)
    {
        FileName.InsertString("/");
        FileName.InsertRange(ListDir);
    }
    int Idx = ListItemsN.IndexOfBin(FileName);
    if (Idx < 0)
    {
        ItemAdd(FileName);
    }
    Idx = ListItemsN.IndexOfBin(FileName);
    if (Idx > 0)
    {
        TempData.Clear();
        FileExport(FileName);
        DirSearchFile = FileName;
        int Pos = DirSearchFile.IndexOf('/');
        if (Pos > 0)
        {
            DirSearchFile.RemoveRange(Pos);
        }
    }
}

void BinaryFile::ItemFileRemove(Str FileName)
{
    if (FileName.Count == 0)
    {
        return;
    }

    int Idx = ListItemsN.IndexOfBin(FileName);
    if (Idx > 0)
    {
        DirSearchFile = FileName;
        Screen::FileExport(3, FileName.ToString(), "", "");
        ListItemsN.Remove(Idx);
    }
}

int BinaryFile::GetFileType(int Idx)
{
    return GetFileType(DirItemName[Idx]);
}

int BinaryFile::GetFileType(Str FileName)
{
    if (FileName[FileName.Count - 1] == '/')
    {
        return -1;
    }
    else
    {
        int FType = CurrentFileAttrGet(1);
        if ((FType == 0) || (FType == 1))
        {
            for (int I = 0; I < FileExtText.Count; I++)
            {
                if (FileExt(FileName) == FileExtText[I])
                {
                    return 0;
                }
            }
            for (int I = 0; I < FileExtAnsi.Count; I++)
            {
                if (FileExt(FileName) == FileExtAnsi[I])
                {
                    return 1;
                }
            }
            for (int I = 0; I < FileExtBin.Count; I++)
            {
                if (FileExt(FileName) == FileExtBin[I])
                {
                    return 2;
                }
            }
            for (int I = 0; I < FileExtXbin.Count; I++)
            {
                if (FileExt(FileName) == FileExtXbin[I])
                {
                    return 3;
                }
            }
            return FType;
        }
        else
        {
            return FType - 2;
        }
    }
}

Str BinaryFile::FileExt(Str FileName)
{
    int ExtI = FileName.IndexOf('.', 0 - FileName.Count);
    if (ExtI >= 0)
    {
        return FileName.Substring(ExtI).CaseLower();
    }
    else
    {
        return Str();
    }
}

void BinaryFile::SetDir(Str Dir)
{
    int StdDir = true;
    bool Recursive = false;
    bool ResetCursor = true;
    Str ListDir0 = ListDir;

    if (Dir[0] == '?')
    {
        Recursive = true;
        Dir.PopFirst();
    }

    if (Dir == Str("."))
    {
        StdDir = false;
        ResetCursor = false;
    }

    if (Dir == Str("/"))
    {
        StdDir = false;
        ListDir.Clear();
    }

    if (Dir == Str(".."))
    {
        StdDir = false;
        int DirPos = ListDir.IndexOf('/', 0 - ListDir.Count - 1);

        DirSearchFile = ListDir;

        if (DirPos < 0)
        {
            ListDir.Clear();
        }
        else
        {
            ListDir.RemoveRange(DirPos);
        }
    }

    if (Dir[0] == '/')
    {
        Dir.PopFirst();
    }

    if (StdDir && (Dir.Count > 0))
    {
        Str NewDir;
        if (ListDir.Count > 0)
        {
            NewDir.AddRange(ListDir);
            NewDir.Add('/');
        }
        NewDir.AddRange(Dir);

        ListDir.Clear();
        ListDir.AddRange(NewDir);
    }

    if (Recursive)
    {
        Screen::FileImport(2, ListDir.ToString(), "~RECUR");
        ListDir = ListDir0;
    }
    else
    {
        DirItemName.Clear();
        DirItemIdx = 0;
        DirItemOff = 0;
        ItemListClear();
        Screen::FileImport(2, ListDir.ToString(), "~DIR");
    }
}

void BinaryFile::Save(Str FileName, Str &Text)
{
    int Idx = ListItemsN.IndexOfBin(FileName);
    if (Idx >= 0)
    {
        std::unique_ptr<TextCodec> Codec = std::make_unique<TextCodec>(AttribValGet(CurrentFileAttr, 0));
        TempData.Clear();
        Codec.get()->Reset();
        Codec.get()->AddBOM();
        Codec.get()->EnqueueStr(Text);
        Codec.get()->DequeueRaw(TempData);
    }
}

void BinaryFile::Load(Str FileName, Str &Text, int CRLF)
{
    int Idx = ListItemsN.IndexOfBin(FileName);
    if (Idx >= 0)
    {
        std::unique_ptr<TextCodec> Codec = std::make_unique<TextCodec>(AttribValGet(CurrentFileAttr, 0));
        Codec.get()->Reset();
        Codec.get()->EnqueueRaw(TempData);
        Codec.get()->RemoveBOM();
        if (CRLF == 0)
        {
            Codec.get()->DequeueStr(Text);
        }
        if (CRLF == 1)
        {
            Str Text_;
            Codec.get()->DequeueStr(Text_);
            Text.Clear();
            for (int I = 0; I < Text_.Count; I++)
            {
                switch (Text_[I])
                {
                    default:
                        Text.Add(Text_[I]);
                        break;
                    case 13:
                        Text.Add(13);
                        Text.Add(10);
                        break;
                    case 10:
                        if (I == 0)
                        {
                            Text.Add(13);
                            Text.Add(10);
                        }
                        else
                        {
                            if (Text_[I - 1] != 13)
                            {
                                Text.Add(13);
                                Text.Add(10);
                            }
                        }
                        break;
                }
            }
        }
        if (CRLF == 2)
        {
            Str Text_;
            Codec.get()->DequeueStr(Text_);
            Text.Clear();
            for (int I = 0; I < Text_.Count; I++)
            {
                switch (Text_[I])
                {
                    default:
                        Text.Add(Text_[I]);
                        break;
                    case 13:
                        if (I == (Text_.Count - 1))
                        {
                            Text.Add(10);
                        }
                        else
                        {
                            if (Text_[I + 1] != 10)
                            {
                                Text.Add(10);
                            }
                        }
                        break;
                }
            }
        }
    }
}

void BinaryFile::LoadRaw(Str FileName, Raw &Text)
{
    Text.Clear();
    Text.AddRange(TempData);
}

std::string BinaryFile::LoadToString(Str FileName, int CRLF)
{
    Str X;
    Load(FileName, X, CRLF);
    return X.ToString();
}

std::string BinaryFile::LoadToStringConfig()
{
    Str X;
    Load(SystemFileName, X, 2);
    X.Add('\n');
    int I = 0;

    XList<Str> Index;
    Str Index_;

    while (X[I] == '~')
    {
        I++;
        Index_.Clear();
        while (X[I] != '\n')
        {
            Index_.Add(X[I]);
            I++;
        }
        Index.Add(Index_);
        I++;
    }
    X.PopLast();
    X.RemoveRange(0, I + 1);

    if ((Index.Count % 2) != 0)
    {
        Index.PopLast();
    }

    if (X.Count > 0)
    {
        if (X[0] == TextCodec::BOM)
        {
            X.PopFirst();
        }
    }

    return X.ToString();
}

void BinaryFile::SaveFromString(Str FileName, std::string S)
{
    Str X = Str::FromString(S);
    Save(FileName, X);
}

void BinaryFile::SaveFromStringConfig(std::string S)
{
    Str X = Str::FromString(S);
    Save(SystemFileName, X);
}

void BinaryFile::FileImportSys()
{
    FileImportWaiting = true;
    Screen::FileImport(1, SystemFileName, SystemFileAttr);
}

void BinaryFile::FileImport(Str FileName)
{
    FileImportWaiting = true;

    Screen::FileImport(1, FileName.ToString(), "");
}

void BinaryFile::FileExport(Str FileName)
{
    Str B64Data;
    B64.get()->Reset();
    B64.get()->Decode(TempData, B64Data);
    Screen::FileExport(1, FileName.ToString(), "", B64Data.ToString());
}

bool BinaryFile::EventFile(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4)
{
    if (EvtParam2 == 0)
    {
        return true;
    }

    if (EvtName == "FileImport")
    {
        if ((EvtParam4 == 2) && (EvtParam2 == 6))
        {
            Str NumX(EvtParam0);
            Str NumXOp(EvtParam0);
            int NumX_ = NumX.IndexOf('|');

            NumXOp.RemoveRange(NumX_);
            NumX.RemoveRange(0, NumX_ + 1);

            if (NumXOp.ToString() == "NUMBER")
            {
                BrowserUploadCurrent = 1;
                BrowserUploadCount = std::stoi(NumX.ToString());
            }
            if (NumXOp.ToString() == "NUMADD")
            {
                BrowserUploadCount += std::stoi(NumX.ToString());
            }

            ProcessInfo.Clear();
            ProcessInfo.AddString(BrowserUploadCurrent);
            ProcessInfo.AddString("/");
            ProcessInfo.AddString(BrowserUploadCount);
        }

        int Kind = (EvtParam2 % 10);
        if ((EvtParam4 == 0) && ((Kind == 1) || (Kind == 2) || (Kind == 3) || (Kind == 6)))
        {
            if ((EvtParam2 >= 20) && (EvtParam2 < 30))
            {
                EventFileName = EvtParam0;
            }
            int Idx = ListItemsN.IndexOfBin(EventFileName);
            if (Idx < 0)
            {
                if ((Kind == 1) || (Kind == 2) || (Kind == 6))
                {
                    ItemAdd(Str(EventFileName));
                }
                Idx = ListItemsN.IndexOfBin(EventFileName);
            }
            if (Idx >= 0)
            {
                if ((EvtParam2 >= 20) && (EvtParam2 < 30))
                {
                    B64.get()->Reset();
                    TempData.Clear();
                }

                if (EvtParam2 < 20)
                {
                    Str B64Data = Str::FromString(EvtParam0);
                    B64.get()->EnqueueStr(B64Data);
                    B64.get()->DequeueRaw(TempData);
                }

                if (EvtParam2 < 10)
                {
                    FileImportWaiting = false;
                    BrowserDownloadProc1(EventFileName);
                }

                if ((EvtParam2 == 2) || (EvtParam2 == 3))
                {
                    Str FileList = TextCodec::Code(TempData, 0);
                    Str FileListBuf;

                    if (EvtParam2 == 2)
                    {
                        DirItemName.Clear();
                        DirItemIdx = 0;
                        DirItemOff = 0;
                        ItemListClear();
                    }

                    for (int I = 0; I < FileList.Count; I++)
                    {
                        if (FileList[I] == '\n')
                        {
                            if (EvtParam2 == 2)
                            {
                                int ListPos = DirItemName.IndexOfBin(FileListBuf);
                                if (ListPos < 0)
                                {
                                    ItemAdd(FileListBuf);
                                    ListPos = DirItemName.IndexOfBin(FileListBuf);
                                }

                                DirItemName.AddSortByIndex(ListPos, FileListBuf);
                            }
                            if (EvtParam2 == 3)
                            {
                                BrowserDownload.Add(FileListBuf);
                            }

                            FileListBuf.Clear();
                        }
                        else
                        {
                            FileListBuf.Add(FileList[I]);
                        }
                    }

                    if (EvtParam2 == 3)
                    {
                        BrowserDownloadCount = BrowserDownload.Count;
                        BrowserDownloadProc0();
                    }

                    if (EvtParam2 == 2)
                    {
                        if (ListDir.Count > 0)
                        {
                            DirItemName.Insert(Str::FromString("../"));
                        }

                        DirItemIdx = DirItemName.IndexOfBin(CurrentFileName);
                        if (DirItemIdx < 0)
                        {
                            DirItemIdx = 0;
                        }

                        if (DirSearchFile.Count > 0)
                        {
                            DirItemIdx = DirItemName.IndexOfBin(DirSearchFile);
                            if (DirItemIdx < 0)
                            {
                                DirItemIdx = 0 - DirItemIdx;
                                if (DirItemIdx >= DirItemName.Count)
                                {
                                    DirItemIdx = 0;
                                }
                            }
                            DirSearchFile.Clear();
                        }
                    }
                }

                if (EvtParam2 == 6)
                {
                    BrowserUploadCurrent++;
                    ProcessInfo.Clear();
                    if (BrowserUploadCurrent > BrowserUploadCount)
                    {
                        ProcessInfo.AddString("~");
                    }
                    else
                    {
                        ProcessInfo.AddString(BrowserUploadCurrent);
                        ProcessInfo.AddString("/");
                        ProcessInfo.AddString(BrowserUploadCount);
                    }
                    FileExport(EventFileName);
                }

                if (PreInit)
                {
                    return false;
                }
                else
                {
                    if (EvtParam2 < 10)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }

    if (EvtName == "FileExport")
    {
        if (EvtParam4 == 0)
        {
            EventFileName = EvtParam0;
            int Idx = ListItemsN.IndexOfBin(EventFileName);

            if (Idx >= 0)
            {
                return true;
            }
        }
    }

    return false;
}

void BinaryFile::SysSaveConfig()
{
    Str B64Data;
    B64.get()->Reset();
    B64.get()->Decode(TempData, B64Data);
    int Idx = ListItemsN.IndexOfBin(SystemFileName);
    if (Idx >= 0)
    {
        Screen::FileExport(1, SystemFileName, "", B64Data.ToString());
    }
}

void BinaryFile::ManagerInfoPush()
{
    DirItemIdx_ = DirItemIdx;
    DirItemOff_ = DirItemOff;
    ListDir_ = ListDir.Copy();
}

void BinaryFile::ManagerInfoPop()
{
    ListDir = ListDir_.Copy();
    SetDir(Str("."));
    DirItemIdx = DirItemIdx_;
    DirItemOff = DirItemOff_;
}

void BinaryFile::FileUpload()
{
    if (ListDir.Count > 0)
    {
        Screen::FileImport(6, ListDir.ToString() + "/", "~BROWSE");
    }
    else
    {
        Screen::FileImport(6, "", "~BROWSE");
    }
}

void BinaryFile::FileDownload()
{
    Str FileName = DirItemName[DirItemIdx];

    if (FileName.Count == 0)
    {
        return;
    }

    if ((FileName.ToString() == "../") || (FileName.ToString() == ".."))
    {
        return;
    }

    if (FileName[FileName.Count - 1] == '/')
    {
        Str DirX = FileName.Copy();
        DirX.PopLast();
        Screen::FileExport(6, DirX.ToString(), "~BEGIN", "");

        DirX = FileName.Copy();
        DirX.Insert('?');
        SetDir(DirX);
    }
    else
    {
        Screen::FileExport(6, FileName.ToString(), "~BEGIN", "");
        BrowserDownload.Add(FileName);
        BrowserDownloadCount = BrowserDownload.Count;
        BrowserDownloadProc0();
    }
}

void BinaryFile::BrowserDownloadProc0()
{
    if (BrowserDownload.Count > 0)
    {
        ProcessInfo.Clear();
        ProcessInfo.AddString((BrowserDownloadCount - BrowserDownload.Count + 1));
        ProcessInfo.AddString("/");
        ProcessInfo.AddString(BrowserDownloadCount);
        FileImport(BrowserDownload[0]);
    }
}

void BinaryFile::BrowserDownloadProc1(Str FileName)
{
    int Pos = BrowserDownload.IndexOf(FileName);

    if (Pos >= 0)
    {
        Str B64Data;
        B64.get()->Reset();
        B64.get()->Decode(TempData, B64Data);
        Screen::FileExport(6, FileName.ToString(), "", B64Data.ToString());

        BrowserDownload.Remove(Pos);

        if (BrowserDownload.Count > 0)
        {
            BrowserDownloadProc0();
        }
        else
        {
            BrowserDownloadCount = 0;
            ProcessInfo.Clear();
            ProcessInfo.AddString("~");
            Screen::FileExport(6, "", "~END", "");
        }
    }
}

int BinaryFile::AttribValGet(Str AttribS, int AttribN)
{
    std::string Attrib_ = AttribS.ToString() + "|";
    int Idx = 0;
    Str Buf;
    for (int I = 0; I < Attrib_.size(); I++)
    {
        if (Attrib_[I] != '|')
        {
            Buf.Add(Attrib_[I]);
        }
        else
        {
            if (Idx == AttribN)
            {
                return std::stoi(Buf.ToString());
            }
            Buf.Clear();
            Idx++;
        }
    }
    return 0;
}

Str BinaryFile::AttribValSet(Str AttribS, int AttribN, int Val)
{
    int Vals[2];
    Vals[0] = AttribValGet(AttribS, 0);
    Vals[1] = AttribValGet(AttribS, 1);
    Vals[AttribN] = Val;
    AttribS.Clear();
    AttribS.AddString(Vals[0]);
    AttribS.AddString("|");
    AttribS.AddString(Vals[1]);
    AttribS.AddString("|");
    return AttribS;
}

int BinaryFile::CurrentFileAttrGet(int AttribN)
{
    return AttribValGet(CurrentFileAttr, AttribN);
}

void BinaryFile::CurrentFileAttrSet(int AttribN, int Val)
{
    CurrentFileAttr = AttribValSet(CurrentFileAttr, AttribN, Val);
}
