#include "binaryfile.h"

BinaryFile::BinaryFile()
{
    B64 = std::make_unique<TextCodec>(1);

    ItemAdd(BinaryFileItem(Str(SystemFile0), 5, -1, 65001, false));

    Disp.Clear();
    for (int I = 0; I < ListItems.Count; I++)
    {
        Str N;
        N.AddRange(ListItems[I].Name);
        N.Add(PtrSep);
        N.AddString(I);
        Disp.Add(N);
    }
}

int BinaryFile::FindName(Str Name)
{
    int Pos = -1;
    for (int I = 0; I < ListItems.Count; I++)
    {
        if (ListItems[I].Name == Name)
        {
            return I;
        }
    }
    return Pos;
}

void BinaryFile::ItemAdd(BinaryFileItem X)
{
    int Pos = FindName(X.Name);
    if (Pos >= 0)
    {
        ListItems.Remove(Pos);
    }
    ListItems.Add(X);
}

BinaryFileItem &BinaryFile::ItemGet(int Idx)
{
    int T = ItemType(Idx);
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

    if (Dir == Str("."))
    {
        StdDir = false;
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
        Dir.Remove(0);
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

    for (int I = 0; I < ListItems.Count; I++)
    {
        Str N = ListItems[I].Name;
        if (ListDir.Count > 0)
        {
            if (N.Count > (ListDir.Count))
            {
                if ((N.Substring(0, ListDir.Count) == ListDir) && (N[ListDir.Count] == '/'))
                {
                    int DirPos = N.IndexOf('/', ListDir.Count + 1);
                    if (DirPos < 0)
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
            if (DirPos < 0)
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
        Disp.Insert(Str("..~"));
    }

    ItemIndex = 0;
    while (ItemType(ItemIndex) < 0)
    {
        ItemIndex++;
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
        Index_.Clear();
        while (X[I] != '\n')
        {
            Index_.Add(X[I]);
            I++;
        }
        Index_.PopFirst();
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
        ItemAdd(BinaryFileItem(Index[I], 5, 0, Index[I + 1].ToString()));
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
    int Idx = FindName(SystemFile0);
    if (Idx >= 0)
    {
        ListItems[Idx].EventId = Screen::FileImport(ListItems[Idx].Type, ListItems[Idx].Name.ToString(), ListItems[Idx].AttribGet());
    }
}

void BinaryFile::FileImport(int Idx)
{
    FileImportWaiting = true;
    if (Idx < 0)
    {
        Idx = ItemIndex;
    }
    Idx = ItemType0(Idx);

    ListItems[Idx].EventId = Screen::FileImport(ListItems[Idx].Type, ListItems[Idx].Name.ToString(), ListItems[Idx].AttribGet());
}

void BinaryFile::FileExport(int Idx)
{
    if (Idx < 0)
    {
        Idx = ItemIndex;
    }
    Idx = ItemType0(Idx);

    if (IsSystemFile() && TempData.Count < 2)
    {
        ListItems[Idx].EventId = Screen::FileExport(ListItems[Idx].Type + 100, ListItems[Idx].Name.ToString(), ListItems[Idx].AttribGet(), "");
    }
    else
    {
        Str B64Data;
        B64.get()->Reset();
        B64.get()->Decode(TempData, B64Data);
        ListItems[Idx].EventId = Screen::FileExport(ListItems[Idx].Type, ListItems[Idx].Name.ToString(), ListItems[Idx].AttribGet(), B64Data.ToString());
    }
}

bool BinaryFile::EventFile(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4)
{
    if (EvtParam2 == 0)
    {
        return true;
    }

    if (EvtName == "FileImport")
    {
        if (EvtParam4 == 0)
        {
            if ((EvtParam2 >= 20) && (EvtParam2 < 30))
            {
                EventFileName = EvtParam0;
            }
            int Idx = FindName(EventFileName);
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
                }

                if (PreInit)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
    }

    if (EvtName == "FileExport")
    {
        if (EvtParam4 == 0)
        {
            EventFileName = EvtParam0;
            int Idx = FindName(EventFileName);
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
    FileExport(0);
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
