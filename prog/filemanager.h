#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include "str.h"
#include "raw.h"
#include <memory>
#include "binaryfile.h"
#include <iostream>

class FileManager
{
public:
    std::shared_ptr<BinaryFile> BinaryFile_;
    FileManager();
    int PopupFore = 0;
    int PopupBack = 0;
    int Pos = 0;
    int SizeW = 32;
    int SizeH = 16;
    void Repaint();
    void EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void Open();
    bool RequestRepaint = false;
    bool RequestCloseNew = false;
    bool RequestCloseOld = false;
    int RepaintDepth = 0;
private:
    int MarginHead = 1;
    int MarginFoot = 3;
};

#endif // FILEMANAGER_H
