#ifndef EDITORUNDO_H
#define EDITORUNDO_H

#include "ansilineoccupyitem.h"

class EditorUndo
{
public:
    EditorUndo();
    bool BufferEnabled = false;
    void BufferStart();
    void BufferStop();
    void BufferAdd(int X, int Y, AnsiLineOccupyItem Old, AnsiLineOccupyItem New);
    void BufferAdd(int X, int Y, int Mode, int W, int H);
};

#endif // EDITORUNDO_H
