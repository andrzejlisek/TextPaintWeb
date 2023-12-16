#include "editorundo.h"

EditorUndo::EditorUndo()
{

}

void EditorUndo::BufferStart()
{
    BufferEnabled = true;
}

void EditorUndo::BufferStop()
{
    BufferEnabled = false;
}

void EditorUndo::BufferAdd(int X, int Y, AnsiLineOccupyItem Old, AnsiLineOccupyItem New)
{
    /*bool UndoBufNew = true;
    for (int i = 0; i < UndoBufferItem_.X.Count; i++)
    {
        if (UndoBufferItem_.X[i] == (X + XX))
        {
            if (UndoBufferItem_.Y[i] == (Y + YY))
            {
                UndoBufferItem_.ItemNew[i] = Item;
                UndoBufNew = false;
                break;
            }
        }
    }
    if (UndoBufNew)
    {
    }*/
}

void EditorUndo::BufferAdd(int X, int Y, int Mode, int W, int H)
{

}
