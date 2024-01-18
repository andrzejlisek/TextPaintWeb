#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include "str.h"
#include "raw.h"
#include <memory>
#include "binaryfile.h"
#include <iostream>
#include "configfile.h"

class FileManager
{
public:
    std::shared_ptr<BinaryFile> BinaryFile_;
    std::shared_ptr<ConfigFile> CF;
    FileManager();
    int PopupFore = 0;
    int PopupBack = 0;
    int Pos = 0;
    int SizeW = 32;
    int SizeH = 16;
    void Repaint();
    void RepaintFiles(int PosX, int PosY);
    void RepaintAttrib(int PosX, int PosY);
    void RepaintInsert(int PosX, int PosY);
    void RepaintDelete(int PosX, int PosY);
    void RepaintProcess(int PosX, int PosY);
    void EventTick();
    void EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void EventKeyFiles(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void EventKeyAttrib(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void EventKeyInsert(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void EventKeyDelete(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void Open(std::shared_ptr<ConfigFile> CF_);
    bool RequestRepaint = false;
    bool RequestCloseNew = false;
    bool RequestCloseOld = false;
    int RepaintDepth = 0;

private:
    Str InsertFileName;
    int MarginHead = 1;
    int MarginFoot = 4;
    enum ManagerStateDef { Files, Insert, Delete, Process, Attrib };
    ManagerStateDef ManagerState = ManagerStateDef::Files;

    int AttribParam = 0;
    int AttribVal[2];
    int AttribValMax[2];
    int AttribValOffset[2];
};

#endif // FILEMANAGER_H
