#ifndef BINARYFILE_H
#define BINARYFILE_H

#include <memory>
#include "textcodec.h"
#include <fstream>
#include <iostream>
#include "str.h"
#include "screen.h"
#include "configfile.h"
#include "ansisauce.h"

class BinaryFile
{
public:
    AnsiSauce AnsiSauce_;
    BinaryFile();
    Str CurrentFileName;
    Str CurrentFileAttr;
    Str DirSearchFile;
    int CurrentFileAttrGet(int AttribN);
    bool CurrentFileAttrSet(int AttribN, int Val);

    Str ListDir;

    void ItemAdd(Str FileName);
    void ItemFileAdd(Str FileName);
    void ItemFileRemove(Str FileName);

    void SetDir(Str Dir);

    void Save(Str FileName, Str &Text);
    void Load(Str FileName, Str &Text, int CRLF);
    void LoadRaw(Str FileName, Raw &Text);
    void LoadSauce(Raw &Text);
    std::string LoadToString(Str FileName, int CRLF);
    std::string LoadToStringConfig();
    void SaveFromString(Str FileName, std::string S);
    void SaveFromStringConfig(std::string S);

    void FileImportSys();
    void FileImport(Str FileName);
    void FileExport(Str FileName);

    bool EventFile(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4);

    void SysSaveConfig();

    void ManagerInfoPush();
    void ManagerInfoPop();

    int SystemFileCount = 1;
    bool FileImportWaiting = false;
    bool PreInit = true;

    void FileUpload();
    void FileDownload();

    Str ProcessInfo;

    std::string SystemFileName = "!!config.txt";
    std::string SystemFileAttr = "65001|0|";

    XList<Str> DirItemName;

    bool IsFileOmmit(int Idx);
    bool IsFileOmmit(Str FileName);
    int GetFileType(int Idx);
    int GetFileType(Str FileName);

    int DirItemIdx = 0;
    int DirItemOff = 0;

    int DirItemIdx_ = 0;
    int DirItemOff_ = 0;
    void Init(std::shared_ptr<ConfigFile> CF);
private:
    XList<Str> FileExtText;
    XList<Str> FileExtAnsi;
    XList<Str> FileExtBin;
    XList<Str> FileExtXbin;
    XList<Str> FileExtOmmit;

    void ItemListClear();

    std::string EventFileName = "";

    XList<Str> ListItemsN;
    int PtrSep = 255;
    int PtrSepNum = 8;

    Raw TempData;
    std::unique_ptr<TextCodec> B64;

    Str ListDir_;

    int BrowserUploadCurrent;
    int BrowserUploadCount;
    XList<Str> BrowserDownload;
    int BrowserDownloadCount = 0;
    void BrowserDownloadProc0();
    void BrowserDownloadProc1(Str FileName);

    int AttribValGet(Str AttribS, int AttribN);
    Str AttribValSet(Str AttribS, int AttribN, int Val);
    Str FileExt(Str FileName);
};

#endif // BINARYFILE_H
