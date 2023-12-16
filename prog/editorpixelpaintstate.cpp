#include "editorpixelpaintstate.h"

EditorPixelPaintState::EditorPixelPaintState()
{

}

void EditorPixelPaintState::ObjCopy(EditorPixelPaintState &Src, EditorPixelPaintState &Dst)
{
    Dst.PaintModeN = Src.PaintModeN;
    Dst.CanvasXBase = Src.CanvasXBase;
    Dst.CanvasYBase = Src.CanvasYBase;
    Dst.CanvasX = Src.CanvasX;
    Dst.CanvasY = Src.CanvasY;
    Dst.SizeX = Src.SizeX;
    Dst.SizeY = Src.SizeY;
    Dst.CharX = Src.CharX;
    Dst.CharY = Src.CharY;
    Dst.CharW = Src.CharW;
    Dst.CharH = Src.CharH;
    Dst.FontW = Src.FontW;
    Dst.FontH = Src.FontH;
    Dst.DefaultColor = Src.DefaultColor;
    Dst.PaintColor = Src.PaintColor;
    Dst.PaintPencil = Src.PaintPencil;
    Dst.PaintMoveRoll = Src.PaintMoveRoll;
}

void EditorPixelPaintState::SetState(EditorPixelPaintState _)
{
    ObjCopy(_, *this);
}

EditorPixelPaintState EditorPixelPaintState::GetState()
{
    EditorPixelPaintState _;
    ObjCopy(*this, _);
    return _;
}
