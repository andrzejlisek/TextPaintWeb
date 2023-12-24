#ifndef EDITORDATA_H
#define EDITORDATA_H

#include <memory>
#include "ansilineoccupyex.h"
#include "editorundo.h"
#include "corestatic.h"

class EditorData
{
public:
    std::shared_ptr<EditorUndo> EditorUndo_;
    EditorData();
    AnsiLineOccupyEx TextBuffer;
    AnsiLineOccupyEx ScrCharDisp_;
    XList<int> ElementGet_Kind;
    XList<int> ElementGet_Count;
    int CursorFontW = 1;
    int CursorFontH = 1;
    AnsiLineOccupyItem ElementGetObj(int X, int Y, bool Space, bool SingleCell);
    int ElementGetVal(int X, int Y, bool Space, bool SingleCell, int KindType);
    int CharGet(int X, int Y, bool Space, bool SingleCell);
    int ColoBGet(int X, int Y, bool Space, bool SingleCell);
    int ColoFGet(int X, int Y, bool Space, bool SingleCell);
    bool ToggleDrawText = true;
    bool ToggleDrawColo = true;
    void TextInsert(int X, int Y, int W, int H, int InsDelMode);
    void TextDelete(int X, int Y, int W, int H, int InsDelMode);

    void DisplayMove(int Dir, int WinTxtW, int WinTxtH);

    int DrawCharI = 32;
    int DrawCharIdbl = 0;
    int DrawColoBI = -1;
    int DrawColoFI = -1;
    int DrawColoAI = 0;
    int CursorChar = 32;
    int CursorColoB = -1;
    int CursorColoF = -1;
    int CursorColoA = 0;

    int CursorX = 0;
    int CursorY = 0;
    int CursorXBase();
    int CursorYBase();

    XList<int> CharPutLineRepaint;

    void CharPut(int X, int Y, AnsiLineOccupyItem Item, bool SingleCell);
    void CharPut0(int X, int Y, int Ch);
    void CharPut(int X, int Y, int Ch);
    void CharPutDbl(int X, int Y, int Offset, int Ch);

private:
    int ElementGet(AnsiLineOccupyEx Data, int KindType, int X, int Y, int Default);
};

#endif // EDITORDATA_H
