#include "configfile.h"

ConfigFile::ConfigFile()
{

}

ConfigFile::~ConfigFile()
{

}

void ConfigFile::FileLoad(std::string FileName)
{
    /*ParamClear();
    std::fstream F(FileName, std::ios::in);
    if (F.is_open())
    {
        std::stringstream SS;
        SS << F.rdbuf();
        std::string RawS = SS.str();
        F.close();
        std::string Buf = "";
        for (unsigned int I = 0; I < RawS.length(); I++)
        {
            char C = RawS[I];
            if ((C == '\r') || (C == '\n'))
            {
                if (Buf != "")
                {
                    Raw.push_back(Buf);
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
            Raw.push_back(Buf);
            Buf = "";
        }
    }*/
}

void ConfigFile::FileSave(std::string FileName)
{
    /*std::fstream F(FileName, std::ios::out);
    if (F.is_open())
    {
        for (unsigned int I = 0; I < Raw.size(); I++)
        {
            if (Raw[I] != "")
            {
                F << Raw[I] << std::endl;
            }
        }
        F.close();
    }*/
}

void ConfigFile::ParamClear()
{
    Raw.clear();
}

void ConfigFile::ParamRemove(std::string Name)
{
    std::string NameX = Name + "=";
    for (unsigned int I = 0; I < Raw.size(); I++)
    {
        if (Raw[I].length() >= NameX.length())
        {
            if (Raw[I].substr(0, NameX.length()) == NameX)
            {
                Raw[I] = "";
                return;
            }
        }
    }
}

void ConfigFile::ParamSet(std::string Name, std::string Value)
{
    std::string NameX = Name + "=";
    for (unsigned int I = 0; I < Raw.size(); I++)
    {
        if (Raw[I].length() >= NameX.length())
        {
            if (Raw[I].substr(0, NameX.length()) == NameX)
            {
                Raw[I] = NameX + Value;
                return;
            }
        }
    }
    Raw.push_back(NameX + Value);
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

/*void ConfigFile::ParamSet(std::string Name, bool Value)
{
    std::string X = "";
    if (Value)
    {
        X = "1";
        ParamSet(Name, X);
    }
    else
    {
        X = "0";
        ParamSet(Name, X);
    }
}*/

void ConfigFile::ParamGet(std::string Name, std::string &Value)
{
    for (unsigned int I = 0; I < Raw.size(); I++)
    {
        std::string S = Raw[I];
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
