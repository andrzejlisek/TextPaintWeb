#include "binaryfile.h"

BinaryFile::BinaryFile()
{
    B64 = std::make_unique<TextCodec>(1);

    ItemAdd(BinaryFileItem(Str(SystemFile0), std::make_shared<TextCodec>(65001), 4, -1, false));
    ItemAdd(BinaryFileItem(Str(SystemFile1), std::make_shared<TextCodec>(65001), 3, -1, false));

    Disp.Clear();
    for (int I = 0; I < ListItems.Count; I++)
    {
        Str N;
        N.AddRange(ListItems[I].Name);
        N.Add(PtrSep);
        N.AddString(std::to_string(I));
        Disp.Add(N);
    }
}

void BinaryFile::ItemAdd(BinaryFileItem X)
{
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
                        N.AddString(std::to_string(I));
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
                N.AddString(std::to_string(I));
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

    ListIndex = 0;
    while (ItemType(ListIndex) < 0)
    {
        ListIndex++;
    }
}

void BinaryFile::SetCRLF(int CR, int LF)
{
    //ListItems[ListIndex].Codec.get()->CR = CR;
    //ListItems[ListIndex].Codec.get()->LF = LF;
}

void BinaryFile::Save(Str &Text)
{
    if (ItemType(ListIndex) >= 0)
    {
        ListItems[ItemType(ListIndex)].Save(Text, TempData);
    }
}

void BinaryFile::Load(Str &Text)
{
    if (ItemType(ListIndex) >= 0)
    {
        ListItems[ItemType(ListIndex)].Load(TempData, Text);
    }
}

std::string BinaryFile::LoadToString()
{
    Str X;
    Load(X);
    return X.ToString();
}

void BinaryFile::SaveFromString(std::string S)
{
    Str X = Str::FromString(S);
    Save(X);
}

bool BinaryFile::IsSystemFile()
{
    if (ItemName(ListIndex).ToString() == SystemFile0)
    {
        return true;
    }
    if (ItemName(ListIndex).ToString() == SystemFile1)
    {
        return true;
    }
    return false;
}

void BinaryFile::FileImportSys(int Idx)
{
    ListItems[ItemType(Idx)].EventId = Screen::FileImport(9, ListItems[Idx].Name.ToString());
}

void BinaryFile::FileImport(int Idx)
{
    if (Idx < 0)
    {
        Idx = ListIndex;
    }
    Idx = ItemType0(Idx);

    ListItems[Idx].EventId = Screen::FileImport(ListItems[Idx].Type, ListItems[Idx].Name.ToString());
}

void BinaryFile::FileExport(int Idx)
{
    if (Idx < 0)
    {
        Idx = ListIndex;
    }
    Idx = ItemType0(Idx);

    if (IsSystemFile() && TempData.Count < 2)
    {
        ListItems[Idx].EventId = Screen::FileExport(ListItems[Idx].Type + 100, ListItems[Idx].Name.ToString(), "");
    }
    else
    {
        Str B64Data;
        B64.get()->Reset();
        B64.get()->Decode(TempData, B64Data);
        ListItems[Idx].EventId = Screen::FileExport(ListItems[Idx].Type, ListItems[Idx].Name.ToString(), B64Data.ToString());
    }
}

bool BinaryFile::EventFileImport(int Id, int Kind, std::string Data)
{
    if (Kind == 0)
    {
        return true;
    }
    for (int I = 0; I < ListItems.Count; I++)
    {
        if (ListItems[I].EventId == Id)
        {
            if ((Kind >= 20) && (Kind < 30))
            {
                B64.get()->Reset();
                TempData.Clear();
            }

            Str B64Data = Str::FromString(Data);
            B64.get()->EnqueueStr(B64Data);
            B64.get()->DequeueRaw(TempData);

            if (Kind < 10)
            {
                ListItems[I].EventId = 0;
            }

            if (Kind < 9)
            {
                return true;
            }
            return false;
        }
    }
    return false;
}

bool BinaryFile::EventFileExport(int Id, int Kind)
{
    if (Kind == 0)
    {
        return true;
    }
    for (int I = 0; I < ListItems.Count; I++)
    {
        if (ListItems[I].EventId == Id)
        {
            ListItems[I].EventId = 0;
            return true;
        }
    }
    return false;
}

void BinaryFile::SysSaveConfig()
{
    FileExport(0);
}
