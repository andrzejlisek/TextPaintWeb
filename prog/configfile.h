#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class ConfigFile
{
public:
    ConfigFile();
    ~ConfigFile();
    std::vector<std::string> Raw[10];
    int RawN = 10;

    void FileLoad(int N, std::string RawS);
    std::string FileSave(int N);

    void ParamClear();
    void ParamRemove(std::string Name);

    void ParamSet(std::string Name, char * Value);
    void ParamSet(std::string Name, std::string Value);
    void ParamSet(std::string Name, int Value);
    void ParamSet(std::string Name, long long Value);

    bool ParamExists(std::string Name);

    void ParamGet(std::string Name, std::string &Value);
    void ParamGet(std::string Name, int &Value);
    void ParamGet(std::string Name, long long &Value);
    void ParamGet(std::string Name, bool &Value);

    std::string ParamGetS(std::string Name);
    int ParamGetI(std::string Name);
    long long ParamGetL(std::string Name);
    bool ParamGetB(std::string Name);
};

#endif // CONFIGFILE_H
