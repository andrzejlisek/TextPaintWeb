#ifndef EDITORUNDO_H
#define EDITORUNDO_H

#include "ansilineoccupyitem.h"
#include <iostream>
#include "editorundoitem.h"
#include <memory>

class EditorUndo
{
public:
    EditorUndo();
    bool BufferEnabled = false;
    void ItemBegin(int X, int Y, int W, int H, int FontW, int FontH, bool DrawText, bool DrawColo);
    void ItemEnd(int X, int Y, int W, int H, int FontW, int FontH);
    void BufferAdd(int X, int Y, AnsiLineOccupyItem Old, AnsiLineOccupyItem New);
    void BufferAdd(int X, int Y, int ActionType, int W, int H);
    EditorUndoItem ItemUndo();
    EditorUndoItem ItemRedo();
private:
    EditorUndoItem CurrentItem;

    XList<EditorUndoItem> Items;
    int ItemIdx = 0;
};

#endif // EDITORUNDO_H
