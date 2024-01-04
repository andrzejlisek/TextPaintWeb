#include "textwork.h"

TextWork::TextWork()
{

}

std::string TextWork::LTrim(std::string S)
{
    int Idx = 0;
    while ((Idx < S.length()) && ((S[Idx] == ' ') || (S[Idx] == '\t') || (S[Idx] == '\r') || (S[Idx] == '\n')))
    {
        Idx++;
    }
    if (Idx > 0)
    {
        if (Idx == S.length())
        {
            return "";
        }
        return S.substr(Idx, S.length() - Idx);
    }
    return S;
}

std::string TextWork::RTrim(std::string S)
{
    int Idx = S.length() - 1;
    while ((Idx >= 0) && ((S[Idx] == ' ') || (S[Idx] == '\t') || (S[Idx] == '\r') || (S[Idx] == '\n')))
    {
        Idx--;
    }
    if (Idx < (((int)S.length()) - 1))
    {
        if (Idx < 0)
        {
            return "";
        }
        return S.substr(0, Idx + 1);
    }
    return S;
}

std::string TextWork::Trim(std::string S)
{
    return TextWork::LTrim(TextWork::RTrim(S));
}

std::string TextWork::LPad(std::string S, int N, std::string Chr)
{
    while (S.length() < N)
    {
        S = Chr + S;
    }
    return S;
}

std::string TextWork::RPad(std::string S, int N, std::string Chr)
{
    while (S.length() < N)
    {
        S = S + Chr;
    }
    return S;
}

std::string TextWork::NumPlusMinus(long Num)
{
    if (Num > 0)
    {
        return "+" + std::to_string(Num);
    }
    if (Num < 0)
    {
        return std::to_string(Num);
    }
    return " " + std::to_string(Num);
}

std::string TextWork::CharPrint(int C)
{
    if ((C >= 32) && (C <= 126))
    {
        return CharToStr(C);
    }
    else
    {
        if (C < 0)
        {
            C += 256;
        }
        return "<" + std::to_string(C) + ">";
    }
}

std::string TextWork::CharToStr(int C)
{
    std::string Str = "X";
    Str[0] = (char)C;
    return Str;
}


std::string TextWork::CharCode(int C, int Mode)
{
    if ((C < 0) || (C > 0x10FFFF))
    {
        switch (Mode)
        {
            case 0:
                return "??";
            case 1:
                return "????";
            case 2:
                return " ????";
        }
    }

    switch (Mode)
    {
        case 0:
            if (C < 256)
            {
                return Hex::IntToHex8(C);
            }
            if (C < 65536)
            {
                return Hex::IntToHex16(C);
            }
            return Hex::IntToHex16(C & 65535);
        case 1:
            if (C < 65536)
            {
                return Hex::IntToHex16(C);
            }
        case 2:
            if (C < 65536)
            {
                return " " + Hex::IntToHex16(C);
            }
    }
    if (C >= 0x100000)
    {
        return "G" + Hex::IntToHex16(C & 65535);
    }
    else
    {
        return Hex::IntToHex4(C >> 16) + Hex::IntToHex16(C & 65535);
    }
    return "";
}

int TextWork::StringSplit(std::string Str, char Chr, std::string * Array)
{
    int Count = 0;
    std::string Buf = "";
    for (int I = 0; I < Str.length(); I++)
    {
        if (Str[I] == Chr)
        {
            Array[Count] = Buf;
            Count++;
            Buf = "";
        }
        else
        {
            Buf = Buf + CharToStr(Str[I]);
        }
    }
    Array[Count] = Buf;
    Count++;
    return Count;
}

int TextWork::StringSplit(std::string Str, char Chr, XList<std::string> &Array)
{
    int Count = 0;
    std::string Buf = "";
    for (int I = 0; I < Str.length(); I++)
    {
        if (Str[I] == Chr)
        {
            Array.Add(Buf);
            Count++;
            Buf = "";
        }
        else
        {
            Buf = Buf + CharToStr(Str[I]);
        }
    }
    Array.Add(Buf);
    Count++;
    return Count;
}

std::string TextWork::StringReplace(std::string Str, std::string From, std::string To)
{
    int Pos = 0;
    while((Pos = Str.find(From, Pos)) != std::string::npos)
    {
       Str.replace(Pos, From.size(), To);
       Pos += To.length();
    }
    return Str;
}

int TextWork::CodeChar(std::string C)
{
    C = Trim(C);
    if (C == "")
    {
        return -1;
    }
    return Hex::HexToInt(C);
}

void TextWork::StringToLines(std::string S, XList<std::string> &Array)
{
    std::string Buf = "";
    Array.Clear();
    unsigned int LastChar = 0;
    for (int I = 0; I < S.size(); I++)
    {
        if ((S[I] == '\r') || (S[I] == '\n'))
        {
            if ((S[I] == '\r') || ((S[I] == '\n') && (LastChar != '\r')))
            {
                Array.Add(Buf);
                Buf = "";
            }
        }
        else
        {
            Buf = Buf + CharToStr(S[I]);
        }
        LastChar = S[I];
    }
    if (Buf.size() > 0)
    {
        Array.Add(Buf);
    }
}

void TextWork::StringToLines(Str S, XList<Str> &Array)
{
    Str Buf;
    Array.Clear();
    unsigned int LastChar = 0;
    for (int I = 0; I < S.Count; I++)
    {
        if ((S[I] == '\r') || (S[I] == '\n'))
        {
            if ((S[I] == '\r') || ((S[I] == '\n') && (LastChar != '\r')))
            {
                Array.Add(Buf);
                Buf.Clear();
            }
        }
        else
        {
            Buf.Add(S[I]);
        }
        LastChar = S[I];
    }
    if (Buf.Count > 0)
    {
        Array.Add(Buf);
    }
}

bool TextWork::StringStartsWith(std::string S, std::string X)
{
    int Offset = S.size() - X.size();
    if (Offset < 0)
    {
        return false;
    }
    for (int I = 0; I < X.size(); I++)
    {
        if (S[I] != X[I])
        {
            return false;
        }
    }
    return true;
}

bool TextWork::StringEndsWith(std::string S, std::string X)
{
    int Offset = S.size() - X.size();
    if (Offset < 0)
    {
        return false;
    }
    for (int I = 0; I < X.size(); I++)
    {
        if (S[I + Offset] != X[I])
        {
            return false;
        }
    }
    return true;
}

bool TextWork::StringStartsEndsWith(std::string S, std::string X1, std::string X2)
{
    return (StringStartsWith(S, X1) && StringEndsWith(S, X2));
}

int TextWork::StrToInt(std::string S, int Default)
{
    int S_ = S.size();
    if (S_ == 0)
    {
        return Default;
    }
    int N = 0;
    int I = 0;
    if (S[0] == '-')
    {
       if (S_ > 1)
       {
           I = 1;
       }
       else
       {
           return Default;
       }
    }
    while (I < S_)
    {
        N = N * 10;
        int C = S[I];
        if ((C < 48) || (C > 57)) return Default;
        N = N + ((int)S[I]) - 48;
        I++;
    }
    if (S[0] == '-') return (0 - N);
    return N;
}

long long TextWork::StrToLong(std::string S, long long Default)
{
    int S_ = S.size();
    if (S_ == 0)
    {
        return Default;
    }
    long long N = 0;
    int I = 0;
    if (S[0] == '-')
    {
       if (S_ > 1)
       {
           I = 1;
       }
       else
       {
           return Default;
       }
    }
    while (I < S_)
    {
        N = N * 10L;
        int C = S[I];
        if ((C < 48) || (C > 57)) return Default;
        N = N + ((long long)S[I]) - 48L;
        I++;
    }
    if (S[0] == '-') return (0L - N);
    return N;
}
