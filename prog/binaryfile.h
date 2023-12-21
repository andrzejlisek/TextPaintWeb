#ifndef BINARYFILE_H
#define BINARYFILE_H

#include <memory>
#include "textcodec.h"
#include <fstream>
#include <iostream>
#include "str.h"
#include "screen.h"
#include "binaryfileitem.h"

class BinaryFile
{
public:
    BinaryFile();
    int ListIndex = 0;
    int ListDispOffset = 0;
    int ListDispOffset_ = 0;
    Str ListDir;

    BinaryFileItem &ItemGet(int idx);
    void ItemAdd(BinaryFileItem X);
    Str ItemName(int Idx);
    int ItemType0(int Idx);
    int ItemType(int Idx);
    int ItemCount();

    void SetDir(Str Dir);

    void SetCRLF(int CR, int LF);
    void Save(Str &Text);
    void Load(Str &Text);
    std::string LoadToString();
    void SaveFromString(std::string S);
    bool IsSystemFile();

    void FileImportSys(int Idx);
    void FileImport(int Idx);
    void FileExport(int Idx);

    bool EventFileImport(int Id, int Kind, std::string Data);
    bool EventFileExport(int Id, int Kind);

    void SysSaveConfig();

    int SystemFileCount = 2;
private:
    XList<Str> Disp;
    XList<BinaryFileItem> ListItems;
    int PtrSep = 126;
    int PtrSepNum = 8;

    Raw TempData;
    std::string SystemFile0 = "!!config.txt";
    std::string SystemFile1 = "!!system.txt";
    std::unique_ptr<TextCodec> B64;
};

#endif // BINARYFILE_H
