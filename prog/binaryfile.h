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
    int ItemIndex = 0;
    int ListDispOffset = 0;
    Str ListDir;

    std::unique_ptr<BinaryFileItem> DefaultAttrib;
    BinaryFileItem &ItemGet(int idx);
    void ItemAdd(BinaryFileItem X, bool Replace);
    void ItemAdd(Str FileName);
    void ItemRemove(Str FileName);
    Str ItemName(int Idx);
    int ItemType0(int Idx);
    int ItemType(int Idx);
    int ItemCount();

    void SetDir(Str Dir);

    void Save(Str &Text);
    void Load(Str &Text);
    void LoadRaw(Raw &Text);
    std::string LoadToString();
    std::string LoadToStringConfig();
    void SaveFromString(std::string S);
    bool IsSystemFile();

    void FileImportSys();
    void FileImport(int Idx, bool Absolute);
    void FileExport(int Idx, bool Absolute);
    void FileExportAttr(int Idx, bool Absolute);

    int IdxCurrent;

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

private:
    std::string EventFileName = "";

    XList<Str> Disp;
    XList<BinaryFileItem> ListItems;
    XList<Str> ListItemsN;
    int PtrSep = 255;
    int PtrSepNum = 8;

    Raw TempData;
    std::string SystemFile0 = "!!config.txt";
    std::unique_ptr<TextCodec> B64;

    int ItemIndex_ = 0;
    int ListDispOffset_ = 0;
    Str ListDir_;

    int BrowserUploadCurrent;
    int BrowserUploadCount;
    int BrowserDownloadCount;
    XList<int> BrowserDownload;
    void BrowserDownloadProc(int Idx);
    void BrowserDownloadProc2();
};

#endif // BINARYFILE_H
