#include "editorundo.h"

EditorUndo::EditorUndo()
{

}

void EditorUndo::ItemBegin(int X, int Y, int W, int H, int FontW, int FontH, bool DrawText, bool DrawColo)
{
    EditorUndoItem::EntryParams Params;
    Params.CursorX = X;
    Params.CursorY = Y;
    Params.CursorW = W;
    Params.CursorH = H;
    Params.FontW = FontW;
    Params.FontH = FontH;

    CurrentItem.Valid = true;
    CurrentItem.EntryParamsOld = Params;
    CurrentItem.ToggleDrawText = DrawText;
    CurrentItem.ToggleDrawColo = DrawColo;

    CurrentItem.ItemsOld.Clear();
    CurrentItem.ItemsNew.Clear();

    BufferEnabled = true;
}

void EditorUndo::ItemEnd(int X, int Y, int W, int H, int FontW, int FontH)
{
    EditorUndoItem::EntryParams Params;
    Params.CursorX = X;
    Params.CursorY = Y;
    Params.CursorW = W;
    Params.CursorH = H;
    Params.FontW = FontW;
    Params.FontH = FontH;

    CurrentItem.EntryParamsNew = Params;

    if (Items.Count > ItemIdx)
    {
        Items.RemoveRange(ItemIdx);
    }
    Items.Add(CurrentItem);
    ItemIdx++;

    BufferEnabled = false;
}

void EditorUndo::BufferAdd(int X, int Y, AnsiLineOccupyItem Old, AnsiLineOccupyItem New)
{
    if (BufferEnabled)
    {
        AnsiLineOccupyItemUndoRedo _Old(Old);
        AnsiLineOccupyItemUndoRedo _New(New);
        _Old.X = X;
        _Old.Y = Y;
        _Old.ActionType = -1;
        _New.X = X;
        _New.Y = Y;
        _New.ActionType = -1;
        CurrentItem.ItemsOld.Add(_Old);
        CurrentItem.ItemsNew.Add(_New);
    }
}

void EditorUndo::BufferAdd(int X, int Y, int ActionType, int W, int H)
{
    if (BufferEnabled)
    {
        AnsiLineOccupyItemUndoRedo _Old;
        AnsiLineOccupyItemUndoRedo _New;
        _Old.X = X;
        _Old.Y = Y;
        _Old.W = W;
        _Old.H = H;
        _Old.ActionType = ActionType;
        _New.X = X;
        _New.Y = Y;
        _New.W = W;
        _New.H = H;
        _New.ActionType = ActionType;
        CurrentItem.ItemsOld.Add(_Old);
        CurrentItem.ItemsNew.Add(_New);
    }
}

EditorUndoItem EditorUndo::ItemUndo()
{
    EditorUndoItem Item;
    if (ItemIdx > 0)
    {
        ItemIdx--;
        Item = Items[ItemIdx];
    }
    else
    {
        Item.Valid = false;
    }
    return Item;
}

EditorUndoItem EditorUndo::ItemRedo()
{
    EditorUndoItem Item;
    if (ItemIdx < Items.Count)
    {
        Item = Items[ItemIdx];
        ItemIdx++;
    }
    else
    {
        Item.Valid = false;
    }
    return Item;
}
