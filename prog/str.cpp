#include "str.h"

Str::Str()
{

}

Str::Str(std::string T)
{
    AddString(T);
}

Str Str::Copy()
{
    Str L;
    for (int I = 0; I < Count; I++)
    {
        L.Data.push_back(Data[I]);
    }
    L.Count += Count;
    return L;
}

int Str::TrimEndLenCol()
{
    int I = Count;
    bool Work = (I > 0);
    if (Work)
    {
        Work = (Data[I - 1] == 0);
    }
    while (Work)
    {
        I--;
        Work = (I > 0);
        if (Work)
        {
            Work = (Data[I - 1] == 0);
        }
    }
    return I;
}

int Str::TrimEndLength()
{
    int I = Count;
    bool Work = (I > 0);
    if (Work)
    {
        Work = SpaceChars.Contains(Data[I - 1]);
    }
    while (Work)
    {
        I--;
        Work = (I > 0);
        if (Work)
        {
            Work = SpaceChars.Contains(Data[I - 1]);
        }
    }
    return I;
}


Str Str::TrimEnC()
{
    Str L = Copy();
    bool Work = (L.Count > 0);
    if (Work)
    {
        Work = (L[L.Count - 1] == 0);
    }
    while (Work)
    {
        L.Remove(L.Count - 1);
        Work = (L.Count > 0);
        if (Work)
        {
            Work = (L[L.Count - 1] == 0);
        }
    }
    return L;
}

Str Str::TrimEnd()
{
    Str L = Copy();
    bool Work = (L.Count > 0);
    if (Work)
    {
        Work = SpaceChars.Contains(L[L.Count - 1]);
    }
    while (Work)
    {
        L.Remove(L.Count - 1);
        Work = (L.Count > 0);
        if (Work)
        {
            Work = SpaceChars.Contains(L[L.Count - 1]);
        }
    }
    return L;
}

Str Str::FromString(std::string T)
{
    Str L;
    for (int i = 0; i < T.length(); i++)
    {
        int C = T[i];
        if (C < 0)
        {
            C = C + 256;
        }
        L.Add(C);
    }
    return L;
}

Str Str::FromString(int I)
{
    return Str::FromString(std::to_string(I));
}

Str Str::CodeToInt(std::string STR)
{
    Str S;
    for (int i = 0; i < STR.length() / 2; i++)
    {
        std::string STR_Char = STR.substr(i * 2, 2);
        if (STR_Char[0] == '_')
        {
            S.Add((int)(STR_Char[1]));
        }
        else
        {
            S.Add(Hex::HexToInt(STR_Char));
        }
    }
    return S;
}

std::string Str::IntToStr(int I)
{
    Str L;
    L.Add(I);
    return L.ToString();
}

Str Str::Substring(int Pos, int Count_)
{
    Str X;
    for (int I = Pos; I < (Count_ + Pos); I++)
    {
        X.Add(Data[I]);
    }
    return X;
}

Str Str::Substring(int Pos)
{
    Str X;
    for (int I = Pos; I < Count; I++)
    {
        X.Add(Data[I]);
    }
    return X;
}

Str Str::CaseLower()
{
    Str X = Copy();
    for (int I = 0; I < X.Count; I++)
    {
        if ((X[I] >= 65) && (X[I] <= 90))
        {
            X[I] = X[I] + 32;
        }
    }
    return X;
}

Str Str::CaseUpper()
{
    Str X = Copy();
    for (int I = 0; I < X.Count; I++)
    {
        if ((X[I] >= 97) && (X[I] <= 122))
        {
            X[I] = X[I] - 32;
        }
    }
    return X;
}

void Str::AddString(int T)
{
    AddString(std::to_string(T));
}

void Str::AddString(std::string T)
{
    AddRange(Str::FromString(T));
}

void Str::InsertString(int T)
{
    InsertString(std::to_string(T));
}

void Str::InsertString(std::string T)
{
    InsertRange(Str::FromString(T));
}


Str Str::BlkCol(int I)
{
    Str L;
    while (I > 0)
    {
        L.Add(0);
        I--;
    }
    return L;
}

Str Str::Spaces(int I)
{
    Str L;
    while (I > 0)
    {
        L.Add(SpaceChar0);
        I--;
    }
    return L;
}

std::string Str::ToString()
{
    std::stringstream SS;
    for (int i = 0; i < Count; i++)
    {
        SS << ((char)Data[i]);
    }
    return SS.str();
}
