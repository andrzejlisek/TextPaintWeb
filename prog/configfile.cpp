#include "configfile.h"

ConfigFile::ConfigFile()
{

}

ConfigFile::~ConfigFile()
{

}

void ConfigFile::FileLoad(int N, std::string RawS)
{
    std::string Buf = "";
    Raw[N].clear();
    char C0 = ' ';
    char C = ' ';
    for (unsigned int I = 0; I < RawS.length(); I++)
    {
        C = RawS[I];
        if ((C == '\r') || (C == '\n'))
        {
            if (C0 != '\r')
            {
                Raw[N].push_back(Buf);
                Buf = "";
            }
        }
        else
        {
            Buf += C;
        }
    }
    if (Buf != "")
    {
        Raw[N].push_back(Buf);
        Buf = "";
    }
}

std::string ConfigFile::FileSave(int N)
{
    std::stringstream SS;
    for (unsigned int I = 0; I < Raw[N].size(); I++)
    {
        SS << Raw[N][I] << std::endl;
    }
    return SS.str();
}

void ConfigFile::ParamClear()
{
    for (int II = 0; II < RawN; II++)
    {
        Raw[II].clear();
    }
}

void ConfigFile::ParamRemove(std::string Name)
{
    std::string NameX = Name + "=";
    for (int II = 0; II < RawN; II++)
    {
        for (unsigned int I = 0; I < Raw[II].size(); I++)
        {
            if (Raw[II][I].length() >= NameX.length())
            {
                if (Raw[II][I].substr(0, NameX.length()) == NameX)
                {
                    Raw[II][I] = "";
                    return;
                }
            }
        }
    }
}

void ConfigFile::ParamSet(std::string Name, std::string Value)
{
    std::string NameX = Name + "=";
    for (int II = 0; II < RawN; II++)
    {
        for (unsigned int I = 0; I < Raw[II].size(); I++)
        {
            if (Raw[II][I].length() >= NameX.length())
            {
                if (Raw[II][I].substr(0, NameX.length()) == NameX)
                {
                    Raw[II][I] = NameX + Value;
                    return;
                }
            }
        }
    }
    Raw[9].push_back(NameX + Value);
}

void ConfigFile::ParamSet(std::string Name, char * Value)
{
    ParamSet(Name, std::string(Value));
}

void ConfigFile::ParamSet(std::string Name, int Value)
{
    ParamSet(Name, std::to_string(Value));
}

void ConfigFile::ParamSet(std::string Name, long long Value)
{
    ParamSet(Name, std::to_string(Value));
}

bool ConfigFile::ParamExists(std::string Name)
{
    for (int II = 0; II < RawN; II++)
    {
        for (unsigned int I = 0; I < Raw[II].size(); I++)
        {
            std::string S = Raw[II][I];
            int X = -1;
            for (unsigned int II = 0; II < S.length(); II++)
            {
                if (S.at(II) == '=')
                {
                    X = II;
                    break;
                }
            }
            if (X > 0)
            {
                if (Name == S.substr(0, X))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void ConfigFile::ParamGet(std::string Name, std::string &Value)
{
    for (int II = 0; II < RawN; II++)
    {
        for (unsigned int I = 0; I < Raw[II].size(); I++)
        {
            std::string S = Raw[II][I];
            int X = -1;
            for (unsigned int II = 0; II < S.length(); II++)
            {
                if (S.at(II) == '=')
                {
                    X = II;
                    break;
                }
            }
            if (X > 0)
            {
                if (Name == S.substr(0, X))
                {
                    Value = S.substr(X + 1);
                }
            }
        }
    }
}

void ConfigFile::ParamGet(std::string Name, int &Value)
{
    std::string Val = "";
    ParamGet(Name, Val);
    if (Val != "")
    {
        Value = std::stoi(Val);
    }
}

void ConfigFile::ParamGet(std::string Name, long long &Value)
{
    std::string Val = "";
    ParamGet(Name, Val);
    if (Val != "")
    {
        Value = std::stoll(Val);
    }
}

void ConfigFile::ParamGet(std::string Name, bool &Value)
{
    std::string Val = "";
    ParamGet(Name, Val);
    if (Val == "1")
    {
        Value = true;
    }
    if (Val == "0")
    {
        Value = false;
    }
}

std::string ConfigFile::ParamGetS(std::string Name)
{
    std::string X = "";
    ParamGet(Name, X);
    return X;
}

int ConfigFile::ParamGetI(std::string Name)
{
    int X = 0;
    ParamGet(Name, X);
    return X;
}

long long ConfigFile::ParamGetL(std::string Name)
{
    long long X = 0;
    ParamGet(Name, X);
    return X;
}

bool ConfigFile::ParamGetB(std::string Name)
{
    bool X = false;
    ParamGet(Name, X);
    return X;
}
