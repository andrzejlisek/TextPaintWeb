#include "ansilineoccupyitemundoredo.h"

AnsiLineOccupyItemUndoRedo::AnsiLineOccupyItemUndoRedo()
{

}

AnsiLineOccupyItemUndoRedo::AnsiLineOccupyItemUndoRedo(AnsiLineOccupyItem _)
{
    Item_Char = _.Item_Char;
    Item_ColorB = _.Item_ColorB;
    Item_ColorF = _.Item_ColorF;
    Item_ColorA = _.Item_ColorA;
    Item_FontW = _.Item_FontW;
    Item_FontH = _.Item_FontH;
}
