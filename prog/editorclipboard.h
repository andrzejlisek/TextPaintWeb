#ifndef EDITORCLIPBOARD_H
#define EDITORCLIPBOARD_H

#include "editordata.h"
#include "textwork.h"
#include <iostream>

class EditorClipboard
{
public:
    EditorClipboard();
    std::shared_ptr<EditorData> EditorData_;
    std::string SysClpTextPaste;
    std::string SysClpTextCopy;
    void SysClipboardSet();
    bool SysClipboardGet();
    void TextClipboardWork(int X, int Y, int W, int H, int FontW, int FontH, bool Paste, int DiamondType);
    AnsiLineOccupyEx TextClipboard;
private:
    bool IsInsideSelection(int W, int H, int XX, int YY, int TX, int TY, int DiamondType);
};

#endif // EDITORCLIPBOARD_H
