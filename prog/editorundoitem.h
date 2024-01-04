#ifndef EDITORUNDOITEM_H
#define EDITORUNDOITEM_H

#include "ansilineoccupyitemundoredo.h"

class EditorUndoItem
{
public:
    struct EntryParams
    {
        int CursorX;
        int CursorY;
        int CursorW;
        int CursorH;
        int FontW;
        int FontH;
    };
    EditorUndoItem();
    bool Valid;
    int ToggleDrawText;
    int ToggleDrawColo;
    EntryParams EntryParamsOld;
    EntryParams EntryParamsNew;
    XList<AnsiLineOccupyItemUndoRedo> ItemsOld;
    XList<AnsiLineOccupyItemUndoRedo> ItemsNew;
};

#endif // EDITORUNDOITEM_H
