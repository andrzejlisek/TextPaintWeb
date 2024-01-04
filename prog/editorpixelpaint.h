#ifndef EDITORPIXELPAINT_H
#define EDITORPIXELPAINT_H

#include "editordata.h"
#include "editorpixelpaintstate.h"
#include "editorclipboard.h"
#include "configfile.h"
#include <map>
#include "textwork.h"

class EditorPixelPaint
{
public:
    EditorPixelPaintState PPS;
    EditorPixelPaint();
    std::shared_ptr<EditorData> EditorData_;
    std::shared_ptr<EditorClipboard> EditorClipboard_;
    struct PaintMode
    {
        std::string Name = "";
        int CharW = 0;
        int CharH = 0;
        std::map<int, int> IntToChar;
        std::map<int, int> CharToInt;
    };
    int PaintModeCount = 0;
    bool IsCharPaint();
    void SelectPaintMode();
    void Init(std::shared_ptr<ConfigFile> CF);
    void PaintStart();
    int GetCursorPosXSize();
    int GetCursorPosYSize();
    int GetCursorPosX();
    int GetCursorPosY();

    void MoveCursor(int Direction);
    void SwapCursors(int RotMode);
    int CustomCharColor();
    void CustomCharSet(int C);
    int CustomCharGet();
    void Paint();
    Str GetStatusInfo();

    int PxlNum = 0;
    int PxlBit = 0;
    int GetPxlCoB0(int X, int Y);
    int GetPxlCoF0(int X, int Y);
    int GetPxlCoA0(int X, int Y);
    int GetPixel0(int X, int Y);
    int GetPxlCoB(int X, int Y);
    int GetPxlCoF(int X, int Y);
    int GetPxlCoA(int X, int Y);

    int GetPixel(int X, int Y);
    void SetPixel(int X, int Y);
    void SetPixel(int X, int Y, int PxlColor, int AnsiCB, int AnsiCF, int AnsiCA);
    void SetPixelBuf(int X, int Y);

    void PaintLine();
    void PaintRect();
    void PaintEllipse();
    void PaintFill();
    void PaintFlipRot(int Direction);
    void PaintMove(int Direction);
    void PaintInvert();

    void ClipboardCopy();
    void ClipboardPaste();

    void UndoRedoBegin();
    void UndoRedoEnd();
    void UndoRedoUndo();
    void UndoRedoRedo();
private:
    XList<PaintMode> PaintMode_;
    int CharGet(int X, int Y, bool Space);
    int ColoBGet(int X, int Y, bool Space);
    int ColoFGet(int X, int Y, bool Space);
    int ColoAGet(int X, int Y, bool Space);
    void CharPut(int X, int Y, int Ch, int ColB, int ColF, int ColA);
    void SwapCursorsX();
    void SwapCursorsY();

    XList<int> PixelStackN;
    XList<int> PixelStackB;
    void PixelStackPush();
    void PixelStackPop();

    XList<int> PixBufX;
    XList<int> PixBufY;
    void PixBufClear();

    void PaintEllipseWork(int X0, int Y0, int RX, int RY, int PX, int PY, bool M1, bool M2);

    XList<EditorPixelPaintState> UndoRedoItemOld;
    XList<EditorPixelPaintState> UndoRedoItemNew;
    int UndoRedoItemIndex = 0;
};

#endif // EDITORPIXELPAINT_H
