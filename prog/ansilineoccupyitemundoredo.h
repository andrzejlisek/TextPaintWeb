#ifndef ANSILINEOCCUPYITEMUNDOREDO_H
#define ANSILINEOCCUPYITEMUNDOREDO_H

#include "ansilineoccupyitem.h"

class AnsiLineOccupyItemUndoRedo : public AnsiLineOccupyItem
{
public:
    int X;
    int Y;
    int W;
    int H;
    int ActionType;
    AnsiLineOccupyItemUndoRedo();
    AnsiLineOccupyItemUndoRedo(AnsiLineOccupyItem _);
};

#endif // ANSILINEOCCUPYITEMUNDOREDO_H
