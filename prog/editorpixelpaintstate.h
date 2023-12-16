#ifndef EDITORPIXELPAINTSTATE_H
#define EDITORPIXELPAINTSTATE_H


class EditorPixelPaintState
{
public:
    EditorPixelPaintState();
    int PaintModeN = 0;

    int CanvasXBase = 0;
    int CanvasYBase = 0;
    int CanvasX = 0;
    int CanvasY = 0;
    int SizeX = 0;
    int SizeY = 0;

    int CharX = 0;
    int CharY = 0;
    int CharW = 1;
    int CharH = 1;

    int FontW = 1;
    int FontH = 1;

    bool DefaultColor = false;

    bool PaintPencil = false;
    int PaintMoveRoll = 0;
    int PaintColor = 0;

    void SetState(EditorPixelPaintState _);
    EditorPixelPaintState GetState();
private:
    void ObjCopy(EditorPixelPaintState &Src, EditorPixelPaintState &Dst);
};

#endif // EDITORPIXELPAINTSTATE_H
