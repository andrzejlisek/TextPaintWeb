#ifndef CORE0EDITOR_H
#define CORE0EDITOR_H

#include "corecommon.h"
#include "editordata.h"
#include "editorundo.h"
#include "editorsemi.h"
#include "editorinfo.h"
#include "editorchar.h"
#include "editorpixelpaint.h"
#include "editorclipboard.h"
#include "ansifile.h"

class Core0Editor : public CoreCommon
{
public:
    Core0Editor();
    void Init();
    void EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void EventOther(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4);
    void EventTick();
private:
    int TickRepaintX = 2;
    int TickRepaint = 0;
    bool UseAnsiLoad = false;
    bool UseAnsiSave = false;
    int FileReadSteps = 0;
    int FileREnc = 0;
    int FileWEnc = 0;

    XList<int> CursorSetX;
    XList<int> CursorSetY;

    void EventKey_Editor(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void EventKey_Editor_1(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void EventKey_Editor_3(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void EventKey_Editor_4(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void EventKey_Info(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    void EventKey_Charmap(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt);
    int CursorX0();
    int CursorY0();
    void CursorChar_(int XX, int YY, int X, int Y, bool Show);
    void CursorChar0(int X, int Y, bool Show);
    void CursorCharX(int X, int Y, bool Show);
    void CursorLine(bool Show);
    void UndoBufferStart();
    void UndoBufferStop();
    void UndoBufferUndo();
    void UndoBufferRedo();

    void CursorLimit();
    void CursorEquivPos(int Dir);
    void MoveCursor(int Direction);
    void TextInsert(int X, int Y, int W, int H, int InsDelMode);
    void TextDelete(int X, int Y, int W, int H, int InsDelMode);

    void SetInfo(bool Enable, int N);

    std::shared_ptr<EditorData> EditorData_;
    std::shared_ptr<EditorUndo> EditorUndo_;
    std::shared_ptr<EditorSemi> EditorSemi_;
    std::shared_ptr<EditorInfo> EditorInfo_;
    std::shared_ptr<EditorChar> EditorChar_;
    std::shared_ptr<EditorPixelPaint> EditorPixelPaint_;
    std::shared_ptr<EditorClipboard> EditorClipboard_;
    enum DisplayStateDef { Editor, Info, Charmap, FileMan, DispConfig };
    int DisplayState = Editor;
    int CursorType = 0;
    int CursorFontW = 1;
    int CursorFontH = 1;
    bool CursorDisplay = true;

    int TextInsDelMode = 0;
    int TextMoveDir = 0;

    enum WorkStateDef { WriteText, WriteChar, DrawChar, DrawPixel };
    WorkStateDef WorkState = WorkStateDef::WriteText;
    int CursorXSize = 0;
    int CursorYSize = 0;
    int DisplayX = 0;
    int DisplayY = 0;
    int WinTxtW = 0;
    int WinTxtH = 0;
    int TextBeyondLineBack = 8;
    int TextBeyondLineFore = 7;
    int TextBeyondEndBack = 7;
    int TextBeyondEndFore = 7;
    int TextBeyondLineMargin = 0;

    int MaxlineSize = 10000;

    void EditorScreenRefresh();
    void ScreenRefresh(bool Force);
    void TextRepaint(bool Force);

    int FontMaxSize = 32;
    int FontMaxSizeCode = 527;

    std::string BeyondIndicator();

    int StatusCursorChar = 32;
    int StatusCursorCharDbl = 0;
    int StatusCursorColoB = -1;
    int StatusCursorColoF = -1;
    int StatusCursorColoA = 0;

    void CharmapPaintCursor(int X, int Y, int Chr, bool Sel);
    void CharmapPaint(int Depth);

    void FileLoad();
    void FileLoad2();
    void FileSave();
};

#endif // CORE0EDITOR_H
