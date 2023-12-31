#include "binaryfile.h"

BinaryFile::BinaryFile()
{
    B64 = std::make_unique<TextCodec>(TextCodec::BASE64);

    DefaultAttrib = std::make_unique<BinaryFileItem>(Str(), 0, 0, 65001, true);

    ItemAdd(BinaryFileItem(Str(SystemFile0), 5, -1, 65001, false), true);

    Disp.Clear();
    for (int I = 0; I < ListItemsN.Count; I++)
    {
        Str N;
        N.AddRange(ListItemsN[I]);
        N.Add(PtrSep);
        N.AddString(I);
        Disp.Add(N);
    }
}

void BinaryFile::ItemAdd(BinaryFileItem X, bool Replace)
{
    int Pos = ListItemsN.IndexOfBin(X.Name);
    if (Pos >= 0)
    {
        if (!Replace)
        {
            return;
        }

        ListItems[Pos] = X;
        return;
    }
    else
    {
        ListItems.AddSortByIndex(Pos, X);
        ListItemsN.AddSortByIndex(Pos, X.Name);
    }
}

void BinaryFile::ItemAdd(Str FileName)
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
    if (Idx >= 0)
    {
        ListItems[Idx].AttribSet(DefaultAttrib.get()->AttribGet());
    }
    else
    {
        ItemAdd(BinaryFileItem(FileName, 2, -1, DefaultAttrib.get()->AttribGet()), false);
        SetDir(Str("."));
    }
    Idx = ListItemsN.IndexOfBin(FileName);
    if (Idx > 0)
    {
        TempData.Clear();
        FileExport(Idx, true);
        for (int I = 0; I < Disp.Count; I++)
        {
            if (ItemName(I) == FileName)
            {
                ItemIndex = I;
            }
        }
    }
}

void BinaryFile::ItemRemove(Str FileName)
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
    Str FileName_ = FileName.Copy();
    FileName_.Add('/');

    Str SystemFile0_ = Str(SystemFile0);
    for (int I = 0; I < ListItemsN.Count; I++)
    {
        if ((ListItemsN[I].Count >= FileName.Count) && (ListItemsN[I] != SystemFile0_))
        {
            if ((ListItemsN[I] == FileName) || ListItemsN[I].GetRange(0, FileName_.Count) == FileName_)
            {
                TempData.Clear();
                ListItems[I].Type += 100;
                FileExport(I, true);
                ListItems[I].Type -= 100;
                ListItems.Remove(I);
                ListItemsN.Remove(I);
                I--;
            }
        }
    }
}

BinaryFileItem &BinaryFile::ItemGet(int Idx)
{
    int T = 0;
    if (Idx >= 0)
    {
        T = ItemType(Idx);
    }
    else
    {
        T = IdxCurrent;
    }
    if (T >= 0)
    {
        return ListItems[T];
    }
    else
    {
        return ListItems[0];
    }
}

Str BinaryFile::ItemName(int Idx)
{
    int T = Disp[Idx].Count - 1;
    while (Disp[Idx][T] != PtrSep)
    {
        T--;
    }
    while (Disp[Idx][T] == PtrSep)
    {
        T--;
    }
    return Disp[Idx].Substring(0, T + 1);
}

int BinaryFile::ItemType0(int Idx)
{
    int T = ItemType(Idx);
    if (T < 0) T = 0;
    return T;
}

int BinaryFile::ItemType(int Idx)
{
    if (Disp[Idx][Disp[Idx].Count - 1] == PtrSep)
    {
        return -1;
    }
    else
    {
        Str PtrN;
        int T = Disp[Idx].Count - 1;
        while (Disp[Idx][T] != PtrSep)
        {
            PtrN.Insert(Disp[Idx][T]);
            T--;
        }
        return std::stoi(PtrN.ToString());
    }
}

int BinaryFile::ItemCount()
{
    return Disp.Count;
}

void BinaryFile::SetDir(Str Dir)
{
    int StdDir = true;
    bool Recursive = false;
    bool ResetCursor = true;

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

    Disp.Clear();

    XList<Str> DispDir;

    Str DispSepX;
    while (DispSepX.Count < PtrSepNum)
    {
        DispSepX.Add(PtrSep);
    }

    for (int I = 0; I < ListItemsN.Count; I++)
    {
        Str N = ListItemsN[I];
        if (ListDir.Count > 0)
        {
            if (N.Count > (ListDir.Count))
            {
                if ((N.Substring(0, ListDir.Count) == ListDir) && (N[ListDir.Count] == '/'))
                {
                    int DirPos = N.IndexOf('/', ListDir.Count + 1);
                    if ((DirPos < 0) || (Recursive))
                    {
                        N.AddRange(DispSepX);
                        N.AddString(I);
                        N.RemoveRange(0, ListDir.Count + 1);
                        Disp.AddSort(N);
                    }
                    else
                    {
                        Str NN = N.Substring(0, DirPos);
                        if (!DispDir.Contains(NN))
                        {
                            DispDir.Add(NN);
                            NN.AddRange(DispSepX);
                            NN.RemoveRange(0, ListDir.Count + 1);
                            Disp.AddSort(NN);
                        }
                    }
                }
            }
        }
        else
        {
            int DirPos = N.IndexOf('/');
            if ((DirPos < 0) || (Recursive))
            {
                N.AddRange(DispSepX);
                N.AddString(I);
                Disp.AddSort(N);
            }
            else
            {
                Str NN = N.Substring(0, DirPos);
                if (!DispDir.Contains(NN))
                {
                    DispDir.Add(NN);
                    NN.AddRange(DispSepX);
                    Disp.AddSort(NN);
                }
            }
        }
    }

    if (ListDir.Count > 0)
    {
        std::string UpDirName = "..@";
        UpDirName[2] = PtrSep;
        Disp.Insert(Str(UpDirName));
    }

    if (ResetCursor)
    {
        ItemIndex = 0;
        /*while (ItemType(ItemIndex) < 0)
        {
            ItemIndex++;
        }*/
        ListDispOffset = 0;
    }
    if ((ItemIndex > 0) && (ItemIndex >= Disp.Count) && (Disp.Count > 0))
    {
        ItemIndex = Disp.Count - 1;
    }
    if (ListDispOffset > ItemIndex)
    {
        ListDispOffset = ItemIndex;
    }
}

void BinaryFile::Save(Str &Text)
{
    if (ItemType(ItemIndex) >= 0)
    {
        ListItems[ItemType(ItemIndex)].Save(Text, TempData);
    }
}

void BinaryFile::Load(Str &Text)
{
    if (ItemType(ItemIndex) >= 0)
    {
        ListItems[ItemType(ItemIndex)].Load(TempData, Text);
    }
}

void BinaryFile::LoadRaw(Raw &Text)
{
    Text.Clear();
    Text.AddRange(TempData);
}

std::string BinaryFile::LoadToString()
{
    Str X;
    Load(X);
    return X.ToString();
}

std::string BinaryFile::LoadToStringConfig()
{
    Str X;
    Load(X);
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

    for (int I = 0; I < Index.Count; I += 2)
    {
        ItemAdd(BinaryFileItem(Index[I], 2, 0, Index[I + 1].ToString()), false);
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

void BinaryFile::SaveFromString(std::string S)
{
    Str X = Str::FromString(S);
    Save(X);
}

bool BinaryFile::IsSystemFile()
{
    if (ItemName(ItemIndex).ToString() == SystemFile0)
    {
        return true;
    }
    return false;
}

void BinaryFile::FileImportSys()
{
    FileImportWaiting = true;
    int Idx = ListItemsN.IndexOfBin(SystemFile0);
    if (Idx >= 0)
    {
        ListItems[Idx].EventId = Screen::FileImport(ListItems[Idx].Type, ListItems[Idx].Name.ToString(), ListItems[Idx].AttribGet());
    }
}

void BinaryFile::FileImport(int Idx, bool Absolute)
{
    FileImportWaiting = true;
    if (!Absolute)
    {
        if (Idx < 0)
        {
            Idx = ItemIndex;
        }
        Idx = ItemType0(Idx);
    }

    ListItems[Idx].EventId = Screen::FileImport(ListItems[Idx].Type, ListItems[Idx].Name.ToString(), ListItems[Idx].AttribGet());
}

void BinaryFile::FileExport(int Idx, bool Absolute)
{
    if (!Absolute)
    {
        if (Idx < 0)
        {
            Idx = ItemIndex;
        }
        Idx = ItemType0(Idx);
    }

    Str B64Data;
    B64.get()->Reset();
    B64.get()->Decode(TempData, B64Data);
    ListItems[Idx].EventId = Screen::FileExport(ListItems[Idx].Type, ListItems[Idx].Name.ToString(), ListItems[Idx].AttribGet(), B64Data.ToString());
}

void BinaryFile::FileExportAttr(int Idx, bool Absolute)
{
    if (!Absolute)
    {
        if (Idx < 0)
        {
            Idx = ItemIndex;
        }
        Idx = ItemType0(Idx);
    }

    ListItems[Idx].EventId = Screen::FileExport(ListItems[Idx].Type + 200, ListItems[Idx].Name.ToString(), ListItems[Idx].AttribGet(), "");
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
        if (EvtParam4 == 0)
        {
            if ((EvtParam2 >= 20) && (EvtParam2 < 30))
            {
                EventFileName = EvtParam0;
            }
            int Idx = ListItemsN.IndexOfBin(EventFileName);
            if (Idx < 0)
            {
                if (EvtParam2 == 26)
                {
                    ItemAdd(BinaryFileItem(Str(EventFileName), 2, -1, DefaultAttrib.get()->AttribGet()), true);
                    Idx = ListItemsN.IndexOfBin(EventFileName);
                }
            }
            else
            {
                if (EvtParam2 == 26)
                {
                    ListItems[Idx].AttribSet(DefaultAttrib.get()->AttribGet());
                }
            }
            IdxCurrent = Idx;
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
                    BrowserDownloadProc(Idx);
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
                    FileExport(Idx, true);
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
            IdxCurrent = Idx;

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
    FileExport(0, false);
}

void BinaryFile::ManagerInfoPush()
{
    ItemIndex_ = ItemIndex;
    ListDispOffset_ = ListDispOffset;
    ListDir_ = ListDir.Copy();
}

void BinaryFile::ManagerInfoPop()
{
    ListDir = ListDir_.Copy();
    SetDir(Str("."));
    ItemIndex = ItemIndex_;
    ListDispOffset = ListDispOffset_;
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

    int ItemPtr = ItemType(ItemIndex);

    Str FileName = ItemName(ItemIndex);
    if (ListDir.Count > 0) { FileName.Insert('/'); }
    FileName.InsertRange(ListDir);
    Screen::FileExport(6, FileName.ToString(), "~BEGIN", "");

    BrowserDownload.Clear();

    if (ItemPtr >= 0)
    {
        BrowserDownload.Add(ItemPtr);
    }
    else
    {
        if (ItemName(ItemIndex) != Str(".."))
        {
            Str ListDir__ = ListDir.Copy();
            int ItemIndex__ = ItemIndex;
            int ListDispOffset__ = ListDispOffset;

            Str DirX = ItemName(ItemIndex);
            DirX.Insert('?');
            SetDir(DirX);

            for (int I = 0; I < Disp.Count; I++)
            {
                if (ItemType(I) >= 0)
                {
                    BrowserDownload.Add(ItemType(I));
                }
            }

            ListDir = ListDir__.Copy();
            SetDir(Str("."));
            ItemIndex = ItemIndex__;
            ListDispOffset = ListDispOffset__;
        }
    }

    BrowserDownloadCount = BrowserDownload.Count;

    BrowserDownloadProc2();
}

void BinaryFile::BrowserDownloadProc(int Idx)
{
    int Pos = BrowserDownload.IndexOf(Idx);

    if (Pos >= 0)
    {
        ListItems[Idx].Browser(1);
        FileExport(Idx, true);
        ListItems[Idx].Browser(0);
        BrowserDownload.Remove(Pos);
        BrowserDownloadProc2();
    }
}

void BinaryFile::BrowserDownloadProc2()
{
    ProcessInfo.Clear();
    if (BrowserDownload.Count > 0)
    {
        ProcessInfo.AddString((BrowserDownloadCount - BrowserDownload.Count + 1));
        ProcessInfo.AddString("/");
        ProcessInfo.AddString(BrowserDownloadCount);

        FileImport(BrowserDownload[0], true);
    }
    else
    {
        ProcessInfo.AddString("~");
        Screen::FileExport(6, "", "~END", "");
    }
}
