#include "core0editor.h"

Core0Editor::Core0Editor()
{

}

void Core0Editor::Init()
{
    EditorData_ = std::make_shared<EditorData>();
    EditorUndo_ = std::make_shared<EditorUndo>();
    EditorSemi_ = std::make_shared<EditorSemi>();
    EditorInfo_ = std::make_shared<EditorInfo>();
    EditorChar_ = std::make_shared<EditorChar>();
    EditorPixelPaint_ = std::make_shared<EditorPixelPaint>();
    EditorClipboard_ = std::make_shared<EditorClipboard>();
    EditorData_.get()->EditorUndo_ = EditorUndo_;
    EditorSemi_.get()->EditorData_ = EditorData_;
    EditorChar_.get()->EditorData_ = EditorData_;
    EditorChar_.get()->BinaryFile_ = BinaryFile_;
    EditorPixelPaint_.get()->EditorData_ = EditorData_;
    EditorPixelPaint_.get()->EditorClipboard_ = EditorClipboard_;
    EditorClipboard_.get()->EditorData_ = EditorData_;
    EditorSemi_.get()->Init(CF);
    EditorChar_.get()->Init(CF);
    EditorPixelPaint_.get()->Init(CF);
    CoreAnsi_ = std::make_shared<CoreAnsi>(CF);
    DisplayConfig_.get()->CoreAnsi_ = CoreAnsi_;
    DisplayConfig_.get()->PopupBack = PopupBack;
    DisplayConfig_.get()->PopupFore = PopupFore;
    EditorChar_.get()->CoreAnsi_ = CoreAnsi_;
    ReadColor(CF.get()->ParamGetS("ColorBeyondLine"), TextBeyondLineBack, TextBeyondLineFore);
    ReadColor(CF.get()->ParamGetS("ColorBeyondEnd"), TextBeyondEndBack, TextBeyondEndFore);
    TextBeyondLineBack += 16;
    TextBeyondLineFore += 16;
    TextBeyondEndBack += 16;
    TextBeyondEndFore += 16;

    FileReadSteps = 0;

    EditorScreenRefresh();
    ScreenRefresh(true);

    //FileLoad();
}

void Core0Editor::EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    switch (DisplayState)
    {
        case DisplayStateDef::Editor:
            EventKey_Editor(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            break;
        case DisplayStateDef::Info:
            EventKey_Info(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            break;
        case DisplayStateDef::Charmap:
            EventKey_Charmap(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            break;
        case DisplayStateDef::FileMan:
            FileManager_.EventKey(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            if (FileManager_.RequestRepaint)
            {
                DisplayState = DisplayStateDef::Editor;
                EditorScreenRefresh();
                ScreenRefresh(true);
                DisplayState = DisplayStateDef::FileMan;
                FileManager_.Repaint();
            }
            if ((FileManager_.RequestCloseOld) || (FileManager_.RequestCloseNew))
            {
                DisplayState = DisplayStateDef::Editor;
                EditorScreenRefresh();
                ScreenRefresh(true);
            }
            break;
        case DisplayStateDef::DispConfig:
            DisplayConfig_.get()->Repaint();
            DisplayConfig_.get()->EventKey(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            if (DisplayConfig_.get()->RequestSave)
            {
                SaveConfig();
            }
            if (DisplayConfig_.get()->RequestRepaint)
            {
                DisplayState = DisplayStateDef::Charmap;
                CharmapPaint(0);
                DisplayState = DisplayStateDef::DispConfig;
                CharmapPaint(0);
            }
            if (DisplayConfig_.get()->RequestClose)
            {
                DisplayState = DisplayStateDef::Charmap;
                CharmapPaint(0);
            }
            else
            {
                if (DisplayConfig_.get()->RequestResize)
                {
                    EventOther("Resize", "", DisplayConfig_.get()->ResizeW, DisplayConfig_.get()->ResizeH, 1, 0);
                }
                else
                {
                    CharmapPaint(0);
                }
            }
            break;
    }
}

void Core0Editor::EventOther(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4)
{
    if ((EvtName == "FileImport") && (EvtParam2 != 0))
    {
        if (DisplayState == DisplayStateDef::Editor)
        {
            FileLoad2();
            return;
        }
    }

    std::string StateId = "__";
    switch (WorkState)
    {
        case WorkStateDef::DrawChar:
            StateId = "DrawChar_";
            break;
        case WorkStateDef::DrawPixel:
            StateId = "DrawPixel_";
            break;
        case WorkStateDef::WriteText:
        case WorkStateDef::WriteChar:
            break;
    }

    switch (_(EvtName.c_str()))
    {
        case _("Resize"):
            ScreenW = EvtParam1;
            ScreenH = EvtParam2;
            Screen::ScreenResize(ScreenW, ScreenH);
            TempMemoI.Add(DisplayState);
            switch (DisplayState)
            {
                case DisplayStateDef::Charmap:
                case DisplayStateDef::DispConfig:
                    DisplayState = DisplayStateDef::Editor;
                    break;
                default:
                    break;
            }
            EditorScreenRefresh();
            ScreenRefresh(true);
            DisplayState = TempMemoI.PopLast();
            switch (DisplayState)
            {
                case DisplayStateDef::Charmap:
                case DisplayStateDef::DispConfig:
                    CharmapPaint(0);
                    break;
                default:
                    break;
            }
            break;
    }

    switch (_((StateId + EvtName).c_str()))
    {
        case _("DrawChar_FileExport"):
            if ((EvtParam2 == 0) && (EvtParam4 == 0))
            {
                EditorClipboard_.get()->SysClpTextCopy = EvtParam0;
            }
            break;
        case _("DrawChar_FileImport"):
            if (EvtParam2 == 0)
            {
                if (EvtParam4 == 0)
                {
                    EditorClipboard_.get()->SysClpTextPaste = EvtParam0;
                }
                else
                {
                    EditorClipboard_.get()->SysClpTextPaste = EditorClipboard_.get()->SysClpTextCopy;
                }
                EditorClipboard_.get()->TextClipboardWork(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorXSize, EditorData_.get()->CursorYSize, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, true, EditorSemi_.get()->DiamondType);
                EditorScreenRefresh();
                ScreenRefresh(true);
                TickRepaint = TickRepaintX;
            }
            break;
        case _("DrawPixel_FileExport"):
            if ((EvtParam2 == 0) && (EvtParam4 == 0))
            {
                EditorClipboard_.get()->SysClpTextCopy = EvtParam0;
            }
            break;
        case _("DrawPixel_FileImport"):
            if (EvtParam2 == 0)
            {
                if (EvtParam4 == 0)
                {
                    EditorClipboard_.get()->SysClpTextPaste = EvtParam0;
                }
                else
                {
                    EditorClipboard_.get()->SysClpTextPaste = EditorClipboard_.get()->SysClpTextCopy;
                }
                EditorPixelPaint_.get()->ClipboardPaste();
                EditorScreenRefresh();
                ScreenRefresh(true);
                TickRepaint = TickRepaintX;
            }
            break;
    }
}

void Core0Editor::EventKey_Editor(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    CursorLine(false);
    switch (_(KeyName.c_str()))
    {
        case _("MouseMove"):
            return;
        case _("MouseBtn"):
            return;
        case _("FileDrop1"):
            return;
        case _("FileDrop2"):
            return;
        case _("F9"):
            DisplayState = DisplayStateDef::Charmap;
            EditorChar_.get()->FavPage = false;
            EditorChar_.get()->SelectChar = EditorData_.get()->DrawCharI;
            EditorChar_.get()->SelectColoB = EditorData_.get()->DrawColoBI;
            EditorChar_.get()->SelectColoF = EditorData_.get()->DrawColoFI;
            EditorChar_.get()->SelectColoA = EditorData_.get()->DrawColoAI;
            CharmapPaint(0);
            return;
        case _("Tab"):
            CursorType++;
            if (CursorType == 4)
            {
                CursorType = 0;
            }
            break;

        case _("F1"):
            if (WorkState == WorkStateDef::WriteText)
            {
                SetInfo(true, 1);
            }
            else
            {
                WorkState = WorkStateDef::WriteText;
            }
            break;
        case _("F2"):
            if (WorkState == WorkStateDef::WriteChar)
            {
                SetInfo(true, 2);
            }
            else
            {
                WorkState = WorkStateDef::WriteChar;
            }
            break;
        case _("F3"):
            if (WorkState == WorkStateDef::DrawChar)
            {
                SetInfo(true, 3);
            }
            else
            {
                WorkState = WorkStateDef::DrawChar;
            }
            break;
        case _("F4"):
            if (WorkState == WorkStateDef::DrawPixel)
            {
                SetInfo(true, 4);
            }
            else
            {
                WorkState = WorkStateDef::DrawPixel;
                EditorPixelPaint_.get()->PaintStart();
            }
            break;

        case _("F7"):
            FileSave();
            return;
        case _("F8"):
            FileLoad();
            return;

        case _("F10"):
            DisplayState = DisplayStateDef::FileMan;
            FileManager_.Open();
            return;

        case _("F12"):
            AppExit = true;
            return;
    }

    if ((WorkState != WorkStateDef::WriteText) && (WorkState != WorkStateDef::WriteChar))
    {
        switch (_(KeyName.c_str()))
        {
            case _("KeyZ"):
                UndoBufferUndo();
                break;
            case _("KeyX"):
                UndoBufferRedo();
                break;
        }
    }

    switch (WorkState)
    {
        case WorkStateDef::WriteText: // Edit text
        case WorkStateDef::WriteChar: // Edit text using characters
            EventKey_Editor_1(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            break;
        case WorkStateDef::DrawChar: // Paint rentangle or diamond with cursor and size
            EventKey_Editor_3(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            break;
        case WorkStateDef::DrawPixel: // Pixel paint
            EventKey_Editor_4(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            break;
    }


    if (DisplayState != DisplayStateDef::Info)
    {
        EditorScreenRefresh();
        ScreenRefresh(true);
    }
    else
    {
        ScreenRefresh(false);
    }
}

void Core0Editor::EventKey_Editor_1(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    switch (_(KeyName.c_str()))
    {
        case _("ArrowUp"):
            MoveCursor(0);
            break;
        case _("ArrowDown"):
            MoveCursor(1);
            break;
        case _("ArrowLeft"):
            MoveCursor(2);
            break;
        case _("ArrowRight"):
            MoveCursor(3);
            break;
        case _("PageUp"):
            MoveCursor(4);
            break;
        case _("End"):
            MoveCursor(5);
            break;
        case _("Home"):
            MoveCursor(6);
            break;
        case _("PageDown"):
            MoveCursor(7);
            break;

        case _("Escape"):
            {
                TextMoveDir++;
                if (TextMoveDir == 8)
                {
                    TextMoveDir = 0;
                }
            }
            break;
        case _("Enter"):
        case _("NumpadEnter"):
            {
                TextInsDelMode++;
                if (TextInsDelMode == 6)
                {
                    TextInsDelMode = 0;
                }
            }
            break;
        case _("Insert"):
            UndoBufferStart();
            TextInsert(EditorData_.get()->CursorX, EditorData_.get()->CursorY, 0, 0, TextInsDelMode);
            UndoBufferStop();
            break;
        case _("Delete"):
            UndoBufferStart();
            TextDelete(EditorData_.get()->CursorX, EditorData_.get()->CursorY, 0, 0, TextInsDelMode);
            UndoBufferStop();
            break;

        case _("Backspace"):
            {
                switch (TextMoveDir)
                {
                    case 0: MoveCursor(2); break;
                    case 2: MoveCursor(0); break;
                    case 4: MoveCursor(3); break;
                    case 6: MoveCursor(1); break;
                    case 1: MoveCursor(6); break;
                    case 3: MoveCursor(4); break;
                    case 5: MoveCursor(7); break;
                    case 7: MoveCursor(5); break;
                }
            }
            break;

        default:
            if (KeyChar >= 32)
            {
                UndoBufferStart();
                int CharToPut = -1;
                int CharToPutDbl = 0;
                if (WorkState == WorkStateDef::WriteChar)
                {
                    if (KeyChar <= 255)
                    {
                        CharToPut = EditorChar_.get()->FavChar[KeyChar];
                    }
                }
                else
                {
                    CharToPut = KeyChar;
                }
                if (CharToPut >= 0)
                {
                    CharToPutDbl = Screen::CharDouble(CharToPut);
                    if (CharToPutDbl != 0)
                    {
                        switch (TextMoveDir)
                        {
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                                MoveCursor(2);
                                break;
                        }
                    }
                    EditorData_.get()->CharPut0(EditorData_.get()->CursorX, EditorData_.get()->CursorY, CharToPut);
                }

                if (CharToPutDbl != 0)
                {
                    MoveCursor(3);
                    EditorData_.get()->CharPut0(EditorData_.get()->CursorX, EditorData_.get()->CursorY, CharToPutDbl);
                    switch (TextMoveDir)
                    {
                        case 0: MoveCursor(3); break;
                        case 2: MoveCursor(2); MoveCursor(1); break;
                        case 4: MoveCursor(2); MoveCursor(2); break;
                        case 6: MoveCursor(0); break;
                        case 1: MoveCursor(7); break;
                        case 3: MoveCursor(2); MoveCursor(5); break;
                        case 5: MoveCursor(2); MoveCursor(6); break;
                        case 7: MoveCursor(4); break;
                    }
                }
                else
                {
                    switch (TextMoveDir)
                    {
                        case 0: MoveCursor(3); break;
                        case 2: MoveCursor(1); break;
                        case 4: MoveCursor(2); break;
                        case 6: MoveCursor(0); break;
                        case 1: MoveCursor(7); break;
                        case 3: MoveCursor(5); break;
                        case 5: MoveCursor(6); break;
                        case 7: MoveCursor(4); break;
                    }
                }
                UndoBufferStop();
            }
            break;
    }
}

void Core0Editor::EventKey_Editor_3(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    EditorSemi_.get()->CursorX = EditorData_.get()->CursorX;
    EditorSemi_.get()->CursorY = EditorData_.get()->CursorY;
    switch (_(KeyName.c_str()))
    {
        case _("ArrowUp"):
            if (EditorSemi_.get()->FramePencil > 0)
            {
                UndoBufferStart();
                switch (EditorSemi_.get()->FramePencilLastCross)
                {
                    case 0:
                        {
                            EditorSemi_.get()->FrameCharPut(0, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
                        }
                        break;
                    case 3:
                        {
                            EditorSemi_.get()->FrameCharPut(6, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
                        }
                        break;
                    case 1:
                        {
                            EditorSemi_.get()->FrameCharPut(4, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
                        }
                        break;
                }
            }
            MoveCursor(0);
            if (EditorSemi_.get()->FramePencil > 0)
            {
                EditorSemi_.get()->FramePencilLastCross = 0;
                UndoBufferStop();
            }
            break;
        case _("ArrowDown"):
            if (EditorSemi_.get()->FramePencil > 0)
            {
                UndoBufferStart();
                switch (EditorSemi_.get()->FramePencilLastCross)
                {
                    case 0:
                        {
                            EditorSemi_.get()->FrameCharPut(1, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
                        }
                        break;
                    case 2:
                        {
                            EditorSemi_.get()->FrameCharPut(5, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
                        }
                        break;
                    case 4:
                        {
                            EditorSemi_.get()->FrameCharPut(7, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
                        }
                        break;
                }
            }
            MoveCursor(1);
            if (EditorSemi_.get()->FramePencil > 0)
            {
                EditorSemi_.get()->FramePencilLastCross = 0;
                UndoBufferStop();
            }
            break;
        case _("ArrowLeft"):
            if (EditorSemi_.get()->FramePencil > 0)
            {
                UndoBufferStart();
                switch (EditorSemi_.get()->FramePencilLastCross)
                {
                    case 0:
                        {
                            EditorSemi_.get()->FrameCharPut(2, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
                        }
                        break;
                    case 2:
                        {
                            EditorSemi_.get()->FrameCharPut(5, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
                        }
                        break;
                    case 3:
                        {
                            EditorSemi_.get()->FrameCharPut(6, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
                        }
                        break;
                }
            }
            MoveCursor(2);
            if (EditorSemi_.get()->FramePencil > 0)
            {
                if (EditorSemi_.get()->FramePencil == 2)
                {
                    MoveCursor(2);
                }
                EditorSemi_.get()->FramePencilLastCross = 0;
                UndoBufferStop();
            }
            break;
        case _("ArrowRight"):
            if (EditorSemi_.get()->FramePencil > 0)
            {
                UndoBufferStart();
                switch (EditorSemi_.get()->FramePencilLastCross)
                {
                    case 0:
                        {
                            EditorSemi_.get()->FrameCharPut(3, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
                        }
                        break;
                    case 1:
                        {
                            EditorSemi_.get()->FrameCharPut(4, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
                        }
                        break;
                    case 4:
                        {
                            EditorSemi_.get()->FrameCharPut(7, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
                        }
                        break;
                }
            }
            MoveCursor(3);
            if (EditorSemi_.get()->FramePencil > 0)
            {
                if (EditorSemi_.get()->FramePencil == 2)
                {
                    MoveCursor(3);
                }
                EditorSemi_.get()->FramePencilLastCross = 0;
                UndoBufferStop();
            }
            break;
        case _("PageUp"):
            if (EditorSemi_.get()->FramePencil > 0)
            {
                UndoBufferStart();
                EditorSemi_.get()->FrameCharPut(4, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
            }
            MoveCursor(4);
            if (EditorSemi_.get()->FramePencil > 0)
            {
                if (EditorSemi_.get()->FramePencil == 2)
                {
                    MoveCursor(3);
                }
                EditorSemi_.get()->FramePencilLastCross = 1;
                UndoBufferStop();
            }
            break;
        case _("End"):
            if (EditorSemi_.get()->FramePencil > 0)
            {
                UndoBufferStart();
                EditorSemi_.get()->FrameCharPut(5, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
            }
            MoveCursor(5);
            if (EditorSemi_.get()->FramePencil > 0)
            {
                if (EditorSemi_.get()->FramePencil == 2)
                {
                    MoveCursor(2);
                }
                EditorSemi_.get()->FramePencilLastCross = 2;
                UndoBufferStop();
            }
            break;
        case _("Home"):
            if (EditorSemi_.get()->FramePencil > 0)
            {
                UndoBufferStart();
                EditorSemi_.get()->FrameCharPut(6, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
            }
            MoveCursor(6);
            if (EditorSemi_.get()->FramePencil > 0)
            {
                if (EditorSemi_.get()->FramePencil == 2)
                {
                    MoveCursor(2);
                }
                EditorSemi_.get()->FramePencilLastCross = 3;
                UndoBufferStop();
            }
            break;
        case _("PageDown"):
            if (EditorSemi_.get()->FramePencil > 0)
            {
                UndoBufferStart();
                EditorSemi_.get()->FrameCharPut(7, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, EditorSemi_.get()->FramePencil == 2);
            }
            MoveCursor(7);
            if (EditorSemi_.get()->FramePencil > 0)
            {
                if (EditorSemi_.get()->FramePencil == 2)
                {
                    MoveCursor(3);
                }
                EditorSemi_.get()->FramePencilLastCross = 4;
                UndoBufferStop();
            }
            break;

        case _("KeyW"):
            EditorData_.get()->CursorYSize--;
            break;
        case _("KeyS"):
            EditorData_.get()->CursorYSize++;
            break;
        case _("KeyA"):
            EditorData_.get()->CursorXSize--;
            break;
        case _("KeyD"):
            EditorData_.get()->CursorXSize++;
            break;

        case _("KeyQ"):
            CursorEquivPos(-1);
            break;
        case _("KeyE"):
            CursorEquivPos(1);
            break;


        case _("Digit1"): // Change shape
            EditorSemi_.get()->DiamondType++;
            if (EditorSemi_.get()->DiamondType == 10)
            {
                EditorSemi_.get()->DiamondType = 0;
            }
            break;
        case _("Digit2"): // Character set
            if (EditorSemi_.get()->DiamondType == 0)
            {
                EditorSemi_.get()->SetFrameNext(1);
            }
            else
            {
                EditorSemi_.get()->SetFrameNext(2);
            }
            break;
        case _("Digit3"): // Hollow
            UndoBufferStart();
            if (EditorSemi_.get()->DiamondType == 0)
            {
                EditorSemi_.get()->RectangleDraw(0, 0, EditorData_.get()->CursorXSize, EditorData_.get()->CursorYSize, 1, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH);
            }
            else
            {
                EditorSemi_.get()->DiamondDraw(0, 0, EditorData_.get()->CursorXSize, EditorData_.get()->CursorYSize, 1, -1, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH);
            }
            UndoBufferStop();
            break;
        case _("Digit4"): // Filled
            UndoBufferStart();
            if (EditorSemi_.get()->DiamondType == 0)
            {
                EditorSemi_.get()->RectangleDraw(0, 0, EditorData_.get()->CursorXSize, EditorData_.get()->CursorYSize, 2, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH);
            }
            else
            {
                EditorSemi_.get()->DiamondDraw(0, 0, EditorData_.get()->CursorXSize, EditorData_.get()->CursorYSize, 2, -1, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH);
            }
            UndoBufferStop();
            break;
        case _("Digit5"): // Frame
            if (EditorSemi_.get()->FramePencil > 0)
            {
                EditorSemi_.get()->FramePencil = 0;
            }
            else
            {
                if (EditorSemi_.get()->DoubleDrawMode())
                {
                    EditorSemi_.get()->FramePencil = 2;
                }
                else
                {
                    EditorSemi_.get()->FramePencil = 1;
                }
            }
            EditorSemi_.get()->FramePencilLastCross = 0;
            break;

        case _("KeyT"):
        case _("Numpad7"):
            UndoBufferStart();
            if (EditorSemi_.get()->DiamondType == 0)
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[2]);
            }
            else
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[13]);
            }
            UndoBufferStop();
            break;
        case _("KeyY"):
        case _("Numpad8"):
            UndoBufferStart();
            if (EditorSemi_.get()->DiamondType == 0)
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[3]);
            }
            else
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[14]);
            }
            UndoBufferStop();
            break;
        case _("KeyU"):
        case _("Numpad9"):
            UndoBufferStart();
            if (EditorSemi_.get()->DiamondType == 0)
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[4]);
            }
            else
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[15]);
            }
            UndoBufferStop();
            break;
        case _("KeyG"):
        case _("Numpad4"):
            UndoBufferStart();
            if (EditorSemi_.get()->DiamondType == 0)
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[5]);
            }
            else
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[16]);
            }
            UndoBufferStop();
            break;
        case _("KeyH"):
        case _("Numpad5"):
            UndoBufferStart();
            if (EditorSemi_.get()->DiamondType == 0)
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[6]);
            }
            else
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[17]);
            }
            UndoBufferStop();
            break;
        case _("KeyJ"):
        case _("Numpad6"):
            UndoBufferStart();
            if (EditorSemi_.get()->DiamondType == 0)
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[7]);
            }
            else
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[18]);
            }
            UndoBufferStop();
            break;
        case _("KeyB"):
        case _("Numpad1"):
            UndoBufferStart();
            if (EditorSemi_.get()->DiamondType == 0)
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[8]);
            }
            else
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[19]);
            }
            UndoBufferStop();
            break;
        case _("KeyN"):
        case _("Numpad2"):
            UndoBufferStart();
            if (EditorSemi_.get()->DiamondType == 0)
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[9]);
            }
            else
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[20]);
            }
            UndoBufferStop();
            break;
        case _("KeyM"):
        case _("Numpad3"):
            UndoBufferStart();
            if (EditorSemi_.get()->DiamondType == 0)
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[10]);
            }
            else
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[21]);
            }
            UndoBufferStop();
            break;

        case _("KeyI"):
        case _("NumpadAdd"):
            UndoBufferStart();
            if (EditorSemi_.get()->DiamondType == 0)
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[1]);
            }
            else
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[12]);
            }
            UndoBufferStop();
            break;
        case _("KeyK"):
        case _("NumpadSubtract"):
            UndoBufferStart();
            if (EditorSemi_.get()->DiamondType == 0)
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[0]);
            }
            else
            {
                EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorSemi_.get()->FrameChar[11]);
            }
            UndoBufferStop();
            break;
        case _("Space"):
        case _("Numpad0"):
            UndoBufferStart();
            EditorData_.get()->CharPutDbl(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorFontW, EditorData_.get()->DrawCharI);
            UndoBufferStop();
            break;

        case _("Enter"):
        case _("NumpadEnter"):
            TextInsDelMode++;
            if (TextInsDelMode == 6)
            {
                TextInsDelMode = 0;
            }
            break;
        case _("Insert"):
            if (EditorSemi_.get()->DiamondType == 0)
            {
                UndoBufferStart();
                TextInsert(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorXSize, EditorData_.get()->CursorYSize, TextInsDelMode);
                UndoBufferStop();
            }
            break;
        case _("Delete"):
            if (EditorSemi_.get()->DiamondType == 0)
            {
                UndoBufferStart();
                TextDelete(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorXSize, EditorData_.get()->CursorYSize, TextInsDelMode);
                UndoBufferStop();
            }
            break;

        case _("KeyC"):
            EditorClipboard_.get()->TextClipboardWork(EditorData_.get()->CursorX, EditorData_.get()->CursorY, EditorData_.get()->CursorXSize, EditorData_.get()->CursorYSize, EditorData_.get()->CursorFontW, EditorData_.get()->CursorFontH, false, EditorSemi_.get()->DiamondType);
            Screen::FileExport(0, "", "", EditorClipboard_.get()->SysClpTextCopy);
            break;
        case _("KeyV"):
            Screen::FileImport(0, "", "");
            break;


        case _("Digit6"):
            if (EditorData_.get()->CursorFontW > 1)
            {
                EditorData_.get()->CursorFontW--;
            }
            break;
        case _("Digit7"):
            if (EditorData_.get()->CursorFontW < FontMaxSize)
            {
                EditorData_.get()->CursorFontW++;
            }
            break;
        case _("Digit8"):
            if (EditorData_.get()->CursorFontH > 1)
            {
                EditorData_.get()->CursorFontH--;
            }
            break;
        case _("Digit9"):
            if (EditorData_.get()->CursorFontH < FontMaxSize)
            {
                EditorData_.get()->CursorFontH++;
            }
            break;
    }
}

void Core0Editor::EventKey_Editor_4(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    switch (_(KeyName.c_str()))
    {
        case _("ArrowUp"):
        case _("ArrowDown"):
        case _("ArrowLeft"):
        case _("ArrowRight"):
        case _("PageUp"):
        case _("End"):
        case _("Home"):
        case _("PageDown"):
            if (EditorPixelPaint_.get()->PPS.PaintPencil)
            {
                UndoBufferStart();
            }
            switch (_(KeyName.c_str()))
            {
                case _("ArrowUp"):
                    EditorPixelPaint_.get()->MoveCursor(0);
                    break;
                case _("ArrowDown"):
                    EditorPixelPaint_.get()->MoveCursor(1);
                    break;
                case _("ArrowLeft"):
                    EditorPixelPaint_.get()->MoveCursor(2);
                    break;
                case _("ArrowRight"):
                    EditorPixelPaint_.get()->MoveCursor(3);
                    break;
                case _("PageUp"):
                    EditorPixelPaint_.get()->MoveCursor(4);
                    break;
                case _("End"):
                    EditorPixelPaint_.get()->MoveCursor(5);
                    break;
                case _("Home"):
                    EditorPixelPaint_.get()->MoveCursor(6);
                    break;
                case _("PageDown"):
                    EditorPixelPaint_.get()->MoveCursor(7);
                    break;
            }
            EditorData_.get()->CursorX = EditorPixelPaint_.get()->GetCursorPosX(); // !!!!
            EditorData_.get()->CursorY = EditorPixelPaint_.get()->GetCursorPosY(); // !!!!
            CursorLimit();
            if (EditorPixelPaint_.get()->PPS.PaintPencil)
            {
                EditorPixelPaint_.get()->Paint();
                UndoBufferStop();
            }
            break;

        case _("KeyQ"):
            EditorPixelPaint_.get()->SwapCursors(-1);
            EditorData_.get()->CursorX = EditorPixelPaint_.get()->GetCursorPosX(); // !!!!
            EditorData_.get()->CursorY = EditorPixelPaint_.get()->GetCursorPosY(); // !!!!
            CursorLimit();
            break;
        case _("KeyE"):
            EditorPixelPaint_.get()->SwapCursors(1);
            EditorData_.get()->CursorX = EditorPixelPaint_.get()->GetCursorPosX(); // !!!!
            EditorData_.get()->CursorY = EditorPixelPaint_.get()->GetCursorPosY(); // !!!!
            CursorLimit();
            break;

        case _("KeyR"):
            EditorPixelPaint_.get()->PPS.PaintColor++;
            if (EditorPixelPaint_.get()->PPS.PaintColor == 4)
            {
                EditorPixelPaint_.get()->PPS.PaintColor = 0;
            }
            if (EditorPixelPaint_.get()->PPS.PaintColor == 8)
            {
                EditorPixelPaint_.get()->PPS.PaintColor = 4;
            }
            break;
        case _("KeyF"):
            if (EditorPixelPaint_.get()->PPS.PaintColor < 4)
            {
                EditorPixelPaint_.get()->PPS.PaintColor += 4;
            }
            else
            {
                EditorPixelPaint_.get()->PPS.PaintColor -= 4;
            }
            break;

        case _("KeyN"):
            UndoBufferStart();
            EditorPixelPaint_.get()->PaintInvert();
            UndoBufferStop();
            break;

        case _("KeyW"):
            EditorPixelPaint_.get()->PPS.SizeY--;
            break;
        case _("KeyS"):
            EditorPixelPaint_.get()->PPS.SizeY++;
            break;
        case _("KeyA"):
            EditorPixelPaint_.get()->PPS.SizeX--;
            break;
        case _("KeyD"):
            EditorPixelPaint_.get()->PPS.SizeX++;
            break;

        case _("KeyI"):
        case _("KeyK"):
        case _("KeyJ"):
        case _("KeyL"):
            {
                UndoBufferStart();
                switch (_(KeyName.c_str()))
                {
                    case _("KeyI"):
                        EditorPixelPaint_.get()->PaintMove(0);
                        break;
                    case _("KeyK"):
                        EditorPixelPaint_.get()->PaintMove(1);
                        break;
                    case _("KeyJ"):
                        EditorPixelPaint_.get()->PaintMove(2);
                        break;
                    case _("KeyL"):
                        EditorPixelPaint_.get()->PaintMove(3);
                        break;
                }
                UndoBufferStop();
            }
            break;

        case _("KeyC"):
            EditorPixelPaint_.get()->ClipboardCopy();
            Screen::FileExport(0, "", "", EditorClipboard_.get()->SysClpTextCopy);
            break;
        case _("KeyV"):
            Screen::FileImport(0, "", "");
            break;

        case _("Digit1"):
            EditorPixelPaint_.get()->PPS.PaintModeN++;
            if (EditorPixelPaint_.get()->PPS.PaintModeN >= EditorPixelPaint_.get()->PaintModeCount)
            {
                EditorPixelPaint_.get()->PPS.PaintModeN = 0;
            }
            EditorPixelPaint_.get()->SelectPaintMode();
            break;
        case _("Digit2"):
            EditorPixelPaint_.get()->PPS.DefaultColor = !EditorPixelPaint_.get()->PPS.DefaultColor;
            break;
        case _("Digit3"):
            UndoBufferStart();
            if (EditorPixelPaint_.get()->PPS.PaintColor < 3)
            {
                EditorPixelPaint_.get()->Paint();
            }
            else
            {
                EditorPixelPaint_.get()->PaintFill();
            }
            UndoBufferStop();
            break;
        case _("Digit4"):
            UndoBufferStart();
            EditorPixelPaint_.get()->PaintLine();
            UndoBufferStop();
            break;
        case _("Digit5"):
            UndoBufferStart();
            EditorPixelPaint_.get()->PaintRect();
            UndoBufferStop();
            break;
        case _("Digit6"):
            UndoBufferStart();
            EditorPixelPaint_.get()->PaintEllipse();
            UndoBufferStop();
            break;
        case _("KeyP"):
            EditorPixelPaint_.get()->PPS.PaintPencil = !EditorPixelPaint_.get()->PPS.PaintPencil;
            if (EditorPixelPaint_.get()->PPS.PaintPencil)
            {
                UndoBufferStart();
                EditorPixelPaint_.get()->Paint();
                UndoBufferStop();
            }
            break;
        case _("KeyM"):
            EditorPixelPaint_.get()->PPS.PaintMoveRoll++;
            if (EditorPixelPaint_.get()->PPS.PaintMoveRoll == 5)
            {
                EditorPixelPaint_.get()->PPS.PaintMoveRoll = 0;
            }
            break;
    }
}

void Core0Editor::EventKey_Info(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    int Repaint = 0;
    switch (_(KeyName.c_str()))
    {
        case _("Escape"):
        case _("Enter"):
        case _("NumpadEnter"):
        case _("F1"):
        case _("F2"):
        case _("F3"):
        case _("F4"):
        case _("F5"):
        case _("F6"):
        case _("F7"):
        case _("F8"):
        case _("F9"):
        case _("F10"):
        case _("F11"):
        case _("F12"):
            SetInfo(false, 0);
            break;
        case _("ArrowUp"):
            Repaint = EditorInfo_.get()->Move(0);
            break;
        case _("ArrowDown"):
            Repaint = EditorInfo_.get()->Move(1);
            break;
        case _("ArrowLeft"):
            Repaint = EditorInfo_.get()->Move(2);
            break;
        case _("ArrowRight"):
            Repaint = EditorInfo_.get()->Move(3);
            break;
    }
    if (Repaint > 0)
    {
        switch(Repaint)
        {
            case 2:
                Screen::ScreenTextMove(0, 0, 0, 1, WinTxtW, WinTxtH - 1);
                EditorData_.get()->DisplayMove(1, WinTxtW, WinTxtH);
                break;
            case 3:
                Screen::ScreenTextMove(0, 1, 0, 0, WinTxtW, WinTxtH - 1);
                EditorData_.get()->DisplayMove(2, WinTxtW, WinTxtH);
                break;
            case 4:
                Screen::ScreenTextMove(0, 0, 1, 0, WinTxtW - 1, WinTxtH);
                EditorData_.get()->DisplayMove(3, WinTxtW, WinTxtH);
                break;
            case 5:
                Screen::ScreenTextMove(1, 0, 0, 0, WinTxtW - 1, WinTxtH);
                EditorData_.get()->DisplayMove(4, WinTxtW, WinTxtH);
                break;
        }
        TextRepaint(false);
        Screen::ScreenCursorMove(ScreenW - 1, ScreenH - 1);
        Screen::ScreenRefresh();
    }

    if (DisplayState != DisplayStateDef::Info)
    {
        EditorScreenRefresh();
        ScreenRefresh(true);
    }
    else
    {
        ScreenRefresh(false);
    }
}

void Core0Editor::EventKey_Charmap(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    EditorChar_.get()->EventKey(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
    CharmapPaint(EditorChar_.get()->RepaintDepth);
    switch (_(KeyName.c_str()))
    {
        case _("F4"):
            if (EditorChar_.get()->SelectorState == 2)
            {
                DisplayState = DisplayStateDef::DispConfig;
                DisplayConfig_.get()->Open();
                CharmapPaint(0);
            }
            break;
    }
}

void Core0Editor::EventTick()
{
    if (TickRepaint > 0)
    {
        TickRepaint--;
        if (TickRepaint == 0)
        {
            //??EventKey("", 0, false, false, false);
            EditorScreenRefresh();
            ScreenRefresh(true);
        }
    }
    if (DisplayState == DisplayStateDef::FileMan)
    {
        FileManager_.EventTick();
    }
}

void Core0Editor::CursorLimit()
{
    if (EditorData_.get()->CursorX < 0)
    {
        EditorData_.get()->CursorX = 0;
    }
    if (EditorData_.get()->CursorY < 0)
    {
        EditorData_.get()->CursorY = 0;
    }
    MoveCursor(-1);
}

void Core0Editor::CursorEquivPos(int Dir)
{
    if (WorkState == WorkStateDef::DrawChar)
    {
        int CursorXSize_X = EditorData_.get()->CursorXSize * EditorData_.get()->CursorFontW;
        int CursorYSize_Y = EditorData_.get()->CursorYSize * EditorData_.get()->CursorFontH;
        int CursorXSize_Y = EditorData_.get()->CursorXSize * EditorData_.get()->CursorFontH;
        int CursorYSize_X = EditorData_.get()->CursorYSize * EditorData_.get()->CursorFontW;
        if (EditorSemi_.get()->DiamondType == 0)
        {
            if (((EditorData_.get()->CursorXSize < 0) & (EditorData_.get()->CursorYSize < 0)) || ((EditorData_.get()->CursorXSize > 0) & (EditorData_.get()->CursorYSize > 0)))
            {
                if (Dir > 0) { EditorData_.get()->CursorX += CursorXSize_X; EditorData_.get()->CursorXSize = 0 - EditorData_.get()->CursorXSize; }
                if (Dir < 0) { EditorData_.get()->CursorY += CursorYSize_Y; EditorData_.get()->CursorYSize = 0 - EditorData_.get()->CursorYSize; }
                CursorLimit();
                return;
            }
            if (((EditorData_.get()->CursorXSize > 0) & (EditorData_.get()->CursorYSize < 0)) || ((EditorData_.get()->CursorXSize < 0) & (EditorData_.get()->CursorYSize > 0)))
            {
                if (Dir > 0) { EditorData_.get()->CursorY += CursorYSize_Y; EditorData_.get()->CursorYSize = 0 - EditorData_.get()->CursorYSize; }
                if (Dir < 0) { EditorData_.get()->CursorX += CursorXSize_X; EditorData_.get()->CursorXSize = 0 - EditorData_.get()->CursorXSize; }
                CursorLimit();
                return;
            }
            if ((EditorData_.get()->CursorXSize == 0) || (EditorData_.get()->CursorYSize == 0))
            {
                EditorData_.get()->CursorX += CursorXSize_X; EditorData_.get()->CursorXSize = 0 - EditorData_.get()->CursorXSize;
                EditorData_.get()->CursorY += CursorYSize_Y; EditorData_.get()->CursorYSize = 0 - EditorData_.get()->CursorYSize;
                CursorLimit();
                return;
            }
        }
        else
        {
            if (((EditorData_.get()->CursorXSize < 0) & (EditorData_.get()->CursorYSize < 0)) || ((EditorData_.get()->CursorXSize > 0) & (EditorData_.get()->CursorYSize > 0)))
            {
                if (Dir > 0) { EditorData_.get()->CursorX += CursorXSize_X; EditorData_.get()->CursorY += CursorXSize_Y; EditorData_.get()->CursorXSize = 0 - EditorData_.get()->CursorXSize; }
                if (Dir < 0) { EditorData_.get()->CursorX -= CursorYSize_X; EditorData_.get()->CursorY += CursorYSize_Y; EditorData_.get()->CursorYSize = 0 - EditorData_.get()->CursorYSize; }
                CursorLimit();
                return;
            }
            if (((EditorData_.get()->CursorXSize > 0) & (EditorData_.get()->CursorYSize < 0)) || ((EditorData_.get()->CursorXSize < 0) & (EditorData_.get()->CursorYSize > 0)))
            {
                if (Dir > 0) { EditorData_.get()->CursorX -= CursorYSize_X; EditorData_.get()->CursorY += CursorYSize_Y; EditorData_.get()->CursorYSize = 0 - EditorData_.get()->CursorYSize; }
                if (Dir < 0) { EditorData_.get()->CursorX += CursorXSize_X; EditorData_.get()->CursorY += CursorXSize_Y; EditorData_.get()->CursorXSize = 0 - EditorData_.get()->CursorXSize; }
                CursorLimit();
                return;
            }
            if (((EditorData_.get()->CursorXSize == 0) & (EditorData_.get()->CursorYSize != 0)) || ((EditorData_.get()->CursorXSize != 0) & (EditorData_.get()->CursorYSize == 0)))
            {
                EditorData_.get()->CursorX += (CursorXSize_X - CursorYSize_X);
                EditorData_.get()->CursorY += (CursorXSize_Y + CursorYSize_Y);
                EditorData_.get()->CursorXSize = 0 - EditorData_.get()->CursorXSize;
                EditorData_.get()->CursorYSize = 0 - EditorData_.get()->CursorYSize;
                CursorLimit();
                return;
            }
        }
    }
}

void Core0Editor::MoveCursor(int Direction)
{
    switch (Direction)
    {
        case 0:
            if (EditorData_.get()->CursorY >= EditorData_.get()->CursorFontH)
            {
                EditorData_.get()->CursorY -= EditorData_.get()->CursorFontH;
            }
            break;
        case 1:
            {
                EditorData_.get()->CursorY += EditorData_.get()->CursorFontH;
            }
            break;
        case 2:
            if (EditorData_.get()->CursorX >= EditorData_.get()->CursorFontW)
            {
                EditorData_.get()->CursorX -= EditorData_.get()->CursorFontW;
            }
            break;
        case 3:
            {
                EditorData_.get()->CursorX += EditorData_.get()->CursorFontW;
            }
            break;
        case 4:
            MoveCursor(0);
            MoveCursor(3);
            break;
        case 5:
            MoveCursor(1);
            MoveCursor(2);
            break;
        case 6:
            MoveCursor(0);
            MoveCursor(2);
            break;
        case 7:
            MoveCursor(1);
            MoveCursor(3);
            break;
    }
    while (DisplayY > EditorData_.get()->CursorY)
    {
        DisplayY--;
        Screen::ScreenTextMove(0, 0, 0, 1, WinTxtW, WinTxtH - 1);
        EditorData_.get()->DisplayMove(1, WinTxtW, WinTxtH);
    }
    while (DisplayY < (EditorData_.get()->CursorY - WinTxtH + 1))
    {
        DisplayY++;
        Screen::ScreenTextMove(0, 1, 0, 0, WinTxtW, WinTxtH - 1);
        EditorData_.get()->DisplayMove(2, WinTxtW, WinTxtH);
    }
    while (DisplayX > EditorData_.get()->CursorX)
    {
        DisplayX--;
        Screen::ScreenTextMove(0, 0, 1, 0, WinTxtW - 1, WinTxtH);
        EditorData_.get()->DisplayMove(3, WinTxtW, WinTxtH);
    }
    while (DisplayX < (EditorData_.get()->CursorX - WinTxtW + 1))
    {
        DisplayX++;
        Screen::ScreenTextMove(1, 0, 0, 0, WinTxtW - 1, WinTxtH);
        EditorData_.get()->DisplayMove(4, WinTxtW, WinTxtH);
    }
}

int Core0Editor::CursorX0()
{
    int T = (EditorData_.get()->CursorX - DisplayX) % EditorData_.get()->CursorFontW;
    if (T == 0)
    {
        return 0;
    }
    else
    {
        return 0 - (EditorData_.get()->CursorFontW - T);
    }
}

int Core0Editor::CursorY0()
{
    int T = (EditorData_.get()->CursorY - DisplayY) % EditorData_.get()->CursorFontH;
    if (T == 0)
    {
        return 0;
    }
    else
    {
        return 0 - (EditorData_.get()->CursorFontH - T);
    }
}

void Core0Editor::CursorChar_(int XX, int YY, int X, int Y, bool Show)
{
    if ((X != XX) || (Y != YY))
    {
        CursorChar0(X, Y, Show);
    }
}

void Core0Editor::CursorChar0(int X, int Y, bool Show)
{
    if (DisplayState == DisplayStateDef::Info)
    {
        return;
    }
    int XMin = std::max(0, X);
    int XMax = std::min(X + EditorData_.get()->CursorFontW, WinTxtW);
    int YMin = std::max(0, Y);
    int YMax = std::min(Y + EditorData_.get()->CursorFontH, WinTxtH);
    for (int YY = YMin; YY < YMax; YY++)
    {
        for (int XX = XMin; XX < XMax; XX++)
        {
            CursorSetX.Add(XX);
            CursorSetY.Add(YY);
        }
    }
}

void Core0Editor::CursorCharX(int X, int Y, bool Show)
{
    if (DisplayState == DisplayStateDef::Info)
    {
        return;
    }
    int XMin = std::max(0, X);
    int XMax = std::min(X + EditorData_.get()->CursorFontW, WinTxtW);
    int YMin = std::max(0, Y);
    int YMax = std::min(Y + EditorData_.get()->CursorFontH, WinTxtH);
    for (int YY = YMin; YY < YMax; YY++)
    {
        for (int XX = XMin; XX < XMax; XX++)
        {
            if ((XX != X) || (YY != Y))
            {
                CursorSetX.Add(XX);
                CursorSetY.Add(YY);
            }
        }
    }
}

void Core0Editor::CursorLine(bool Show)
{
    CursorSetX.Clear();
    CursorSetY.Clear();
    if (Show)
    {
        int XX = EditorData_.get()->CursorX - DisplayX;
        int YY = EditorData_.get()->CursorY - DisplayY;
        int XX0 = CursorX0();
        int YY0 = CursorY0();
        if (WorkState == WorkStateDef::DrawChar)
        {
            if (EditorSemi_.get()->DiamondType == 0)
            {
                int X1 = std::min(XX, XX + (EditorData_.get()->CursorXSize * EditorData_.get()->CursorFontW));
                int X2 = std::max(XX, XX + (EditorData_.get()->CursorXSize * EditorData_.get()->CursorFontW));
                int Y1 = std::min(YY, YY + (EditorData_.get()->CursorYSize * EditorData_.get()->CursorFontH));
                int Y2 = std::max(YY, YY + (EditorData_.get()->CursorYSize * EditorData_.get()->CursorFontH));

                X1 = std::max(std::min(X1, WinTxtW - 1), XX0);
                X2 = std::max(std::min(X2, WinTxtW - 1), XX0);
                Y1 = std::max(std::min(Y1, WinTxtH - 1), YY0);
                Y2 = std::max(std::min(Y2, WinTxtH - 1), YY0);

                for (int Y = Y1; Y <= Y2; Y += EditorData_.get()->CursorFontH)
                {
                    for (int X = X1; X <= X2; X += EditorData_.get()->CursorFontW)
                    {
                        if ((X != XX) || (Y != YY))
                        {
                            CursorChar0(X, Y, Show);
                        }
                    }
                }
            }
            else
            {
                int X1 = std::min(0, EditorData_.get()->CursorXSize);
                int X2 = std::max(0, EditorData_.get()->CursorXSize);
                int Y1 = std::min(0, EditorData_.get()->CursorYSize);
                int Y2 = std::max(0, EditorData_.get()->CursorYSize);

                for (int X_ = X1; X_ <= X2; X_++)
                {
                    for (int Y_ = Y1; Y_ <= Y2; Y_++)
                    {
                        int X__ = XX + ((X_ - Y_) * EditorData_.get()->CursorFontW);
                        int Y__ = YY + ((X_ + Y_) * EditorData_.get()->CursorFontH);

                        CursorChar_(XX, YY, X__, Y__, Show);

                        switch (EditorSemi_.get()->DiamondType)
                        {
                            case 1:
                                if ((X_ < X2) && (Y_ > Y1))
                                {
                                    CursorChar_(XX, YY, X__ + EditorData_.get()->CursorFontW, Y__, Show);
                                }
                                break;
                            case 2:
                                CursorChar_(XX, YY, X__ + EditorData_.get()->CursorFontW, Y__, Show);
                                break;
                            case 3:
                                CursorChar_(XX, YY, X__, Y__ + EditorData_.get()->CursorFontH, Show);
                                break;
                            case 4:
                                CursorChar_(XX, YY, X__ - EditorData_.get()->CursorFontW, Y__, Show);
                                break;
                            case 5:
                                CursorChar_(XX, YY, X__, Y__ - EditorData_.get()->CursorFontH, Show);
                                break;
                            case 6:
                                CursorChar_(XX, YY, X__ + EditorData_.get()->CursorFontW, Y__, Show);
                                CursorChar_(XX, YY, X__, Y__ - EditorData_.get()->CursorFontH, Show);
                                CursorChar_(XX, YY, X__ + EditorData_.get()->CursorFontW, Y__ - EditorData_.get()->CursorFontH, Show);
                                break;
                            case 7:
                                CursorChar_(XX, YY, X__ + EditorData_.get()->CursorFontW, Y__, Show);
                                CursorChar_(XX, YY, X__, Y__ + EditorData_.get()->CursorFontH, Show);
                                CursorChar_(XX, YY, X__ + EditorData_.get()->CursorFontW, Y__ + EditorData_.get()->CursorFontH, Show);
                                break;
                            case 8:
                                CursorChar_(XX, YY, X__ - EditorData_.get()->CursorFontW, Y__, Show);
                                CursorChar_(XX, YY, X__, Y__ + EditorData_.get()->CursorFontH, Show);
                                CursorChar_(XX, YY, X__ - EditorData_.get()->CursorFontW, Y__ + EditorData_.get()->CursorFontH, Show);
                                break;
                            case 9:
                                CursorChar_(XX, YY, X__ - EditorData_.get()->CursorFontW, Y__, Show);
                                CursorChar_(XX, YY, X__, Y__ - EditorData_.get()->CursorFontH, Show);
                                CursorChar_(XX, YY, X__ - EditorData_.get()->CursorFontW, Y__ - EditorData_.get()->CursorFontH, Show);
                                break;
                        }
                    }
                }
            }
        }
        if (WorkState == WorkStateDef::DrawPixel)
        {
            int XX2 = EditorPixelPaint_.get()->GetCursorPosXSize() - DisplayX;
            int YY2 = EditorPixelPaint_.get()->GetCursorPosYSize() - DisplayY;
            if ((XX != XX2) || (YY != YY2))
            {
                int XX3 = std::max(std::min(XX2, WinTxtW - 1), 0);
                int YY3 = std::max(std::min(YY2, WinTxtH - 1), 0);
                CursorChar_(XX, YY, XX3, YY3, Show);
                if ((XX2 < 0) || (XX2 >= WinTxtW))
                {
                    CursorChar_(XX, YY, XX3, YY3 - EditorData_.get()->CursorFontH, Show);
                    CursorChar_(XX, YY, XX3, YY3 + EditorData_.get()->CursorFontH, Show);
                }
                if ((YY2 < 0) || (YY2 >= WinTxtH))
                {
                    CursorChar_(XX, YY, XX3 - EditorData_.get()->CursorFontW, YY3, Show);
                    CursorChar_(XX, YY, XX3 + EditorData_.get()->CursorFontW, YY3, Show);
                }
            }
        }

        if (CursorDisplay)
        {
            CursorChar0(XX, YY, Show);
        }
        else
        {
            CursorCharX(XX, YY, Show);
        }
        if ((CursorType == 1) || (CursorType == 3))
        {
            for (int X = XX - EditorData_.get()->CursorFontW; X > (0 - EditorData_.get()->CursorFontW); X -= EditorData_.get()->CursorFontW)
            {
                CursorChar0(X, YY, Show);
            }
            for (int X = XX + EditorData_.get()->CursorFontW; X < WinTxtW; X += EditorData_.get()->CursorFontW)
            {
                CursorChar0(X, YY, Show);
            }
            for (int Y = YY - EditorData_.get()->CursorFontH; Y > (0 - EditorData_.get()->CursorFontH); Y -= EditorData_.get()->CursorFontH)
            {
                CursorChar0(XX, Y, Show);
            }
            for (int Y = YY + EditorData_.get()->CursorFontH; Y < WinTxtH; Y += EditorData_.get()->CursorFontH)
            {
                CursorChar0(XX, Y, Show);
            }
        }
        if ((CursorType == 2) || (CursorType == 3))
        {
            int XX_, YY_;
            XX_ = XX - EditorData_.get()->CursorFontW;
            YY_ = YY - EditorData_.get()->CursorFontH;
            while ((XX_ > (0 - EditorData_.get()->CursorFontW)) && (YY_ > (0 - EditorData_.get()->CursorFontH)))
            {
                CursorChar0(XX_, YY_, Show);
                XX_ -= EditorData_.get()->CursorFontW;
                YY_ -= EditorData_.get()->CursorFontH;
            }
            XX_ = XX + EditorData_.get()->CursorFontW;
            YY_ = YY + EditorData_.get()->CursorFontH;
            while ((XX_ < WinTxtW) && (YY_ < WinTxtH))
            {
                CursorChar0(XX_, YY_, Show);
                XX_ += EditorData_.get()->CursorFontW;
                YY_ += EditorData_.get()->CursorFontH;
            }
            XX_ = XX - EditorData_.get()->CursorFontW;
            YY_ = YY + EditorData_.get()->CursorFontH;
            while ((XX_ > (0 - EditorData_.get()->CursorFontW)) && (YY_ < WinTxtH))
            {
                CursorChar0(XX_, YY_, Show);
                XX_ -= EditorData_.get()->CursorFontW;
                YY_ += EditorData_.get()->CursorFontH;
            }
            XX_ = XX + EditorData_.get()->CursorFontW;
            YY_ = YY - EditorData_.get()->CursorFontH;
            while ((XX_ < WinTxtW) && (YY_ > (0 - EditorData_.get()->CursorFontH)))
            {
                CursorChar0(XX_, YY_, Show);
                XX_ += EditorData_.get()->CursorFontW;
                YY_ -= EditorData_.get()->CursorFontH;
            }
        }
    }
}

void Core0Editor::UndoBufferStart()
{
    EditorData_.get()->UndoRedoBegin();
    EditorPixelPaint_.get()->UndoRedoBegin();
}

void Core0Editor::UndoBufferStop()
{
    EditorData_.get()->UndoRedoEnd();
    EditorPixelPaint_.get()->UndoRedoEnd();
    CursorLimit();
}

void Core0Editor::UndoBufferUndo()
{
    EditorData_.get()->UndoRedoUndo();
    EditorPixelPaint_.get()->UndoRedoUndo();
    CursorLimit();
}

void Core0Editor::UndoBufferRedo()
{
    EditorData_.get()->UndoRedoRedo();
    EditorPixelPaint_.get()->UndoRedoRedo();
    CursorLimit();
}

void Core0Editor::TextInsert(int X, int Y, int W, int H, int InsDelMode)
{
    EditorData_.get()->TextInsert(X, Y, W, H, InsDelMode);
}

void Core0Editor::TextDelete(int X, int Y, int W, int H, int InsDelMode)
{
    EditorData_.get()->TextDelete(X, Y, W, H, InsDelMode);
}

void Core0Editor::EditorScreenRefresh()
{
    WinTxtW = ScreenW;
    WinTxtH = ScreenH - 1;

    EditorData_.get()->ScrCharDisp_.Clear();
    EditorData_.get()->ScrCharDisp_.BlankChar();
    for (int i = 0; i < WinTxtH; i++)
    {
        EditorData_.get()->ScrCharDisp_.AppendLine();
        EditorData_.get()->ScrCharDisp_.PadRight(i, WinTxtW);
    }

    CursorLimit();
}

void Core0Editor::ScreenRefresh(bool Force)
{
    CursorLine(true);
    TextRepaint(Force);
    switch (DisplayState)
    {
        case DisplayStateDef::Editor:
        case DisplayStateDef::Info:
        case DisplayStateDef::Charmap:
        case DisplayStateDef::FileMan:
        case DisplayStateDef::DispConfig:
            {
                Str StatusText;
                EditorData_.get()->CursorChar = EditorData_.get()->ElementGetVal(EditorData_.get()->CursorX, EditorData_.get()->CursorY, true, false, 0);
                if (Screen::CharDoubleInv(EditorData_.get()->CursorChar) >= 0)
                {
                    EditorData_.get()->CursorChar = Screen::CharDoubleInv(EditorData_.get()->CursorChar);
                }
                EditorData_.get()->CursorColoB = EditorData_.get()->ElementGetVal(EditorData_.get()->CursorX, EditorData_.get()->CursorY, true, false, 1);
                EditorData_.get()->CursorColoF = EditorData_.get()->ElementGetVal(EditorData_.get()->CursorX, EditorData_.get()->CursorY, true, false, 2);
                EditorData_.get()->CursorColoA = EditorData_.get()->ElementGetVal(EditorData_.get()->CursorX, EditorData_.get()->CursorY, true, false, 3);
                if (WorkState == WorkStateDef::DrawPixel)
                {
                    StatusText.AddString(EditorPixelPaint_.get()->PPS.FontW);
                    StatusText.AddString("x");
                    StatusText.AddString(EditorPixelPaint_.get()->PPS.FontH);
                    StatusText.AddString(" ");

                    StatusText.AddString(EditorPixelPaint_.get()->PPS.CanvasX);
                    StatusText.AddString(TextWork::NumPlusMinus(EditorPixelPaint_.get()->PPS.SizeX));
                    StatusText.AddString(BeyondIndicator());
                    StatusText.AddString(EditorPixelPaint_.get()->PPS.CanvasY);
                    StatusText.AddString(TextWork::NumPlusMinus(EditorPixelPaint_.get()->PPS.SizeY));
                }
                else
                {
                    StatusText.AddString(EditorData_.get()->CursorFontW);
                    StatusText.AddString("x");
                    StatusText.AddString(EditorData_.get()->CursorFontH);
                    StatusText.AddString(" ");

                    StatusText.AddString(EditorData_.get()->CursorX);
                    StatusText.AddString(BeyondIndicator());
                    StatusText.AddString(EditorData_.get()->CursorY);
                    if (DisplayState != DisplayStateDef::Info)
                    {
                        StatusCursorChar = EditorData_.get()->CursorChar;
                        StatusCursorCharDbl = Screen::CharDouble(StatusCursorChar);
                        StatusCursorColoB = EditorData_.get()->CursorColoB;
                        StatusCursorColoF = EditorData_.get()->CursorColoF;
                        StatusCursorColoA = EditorData_.get()->CursorColoA;
                    }
                    StatusText.AddString(" ");
                    StatusText.AddString(TextWork::CharCode(StatusCursorChar, 1));
                    StatusText.AddString(" ");
                    StatusText.Add(StatusCursorChar);
                    if (StatusCursorCharDbl != 0)
                    {
                        StatusText.Add(StatusCursorCharDbl);
                    }
                    StatusText.AddString(" ");
                    if ((StatusCursorColoB >= 0) && (StatusCursorColoB <= 15))
                    {
                        StatusText.AddString(Hex::IntToHex4(StatusCursorColoB));
                    }
                    else
                    {
                        StatusText.AddString("-");
                    }
                    if ((StatusCursorColoF >= 0) && (StatusCursorColoF <= 15))
                    {
                        StatusText.AddString(Hex::IntToHex4(StatusCursorColoF));
                    }
                    else
                    {
                        StatusText.AddString("-");
                    }
                    StatusText.AddString(" ");
                    StatusText.AddString(CoreStatic::GetAttribText(StatusCursorColoA));
                    StatusText.AddString(" ");
                }

                switch (WorkState)
                {
                    case WorkStateDef::WriteText:
                        if (TextMoveDir == 0) { StatusText.AddString("Text-R"); }
                        if (TextMoveDir == 1) { StatusText.AddString("Text-RD"); }
                        if (TextMoveDir == 2) { StatusText.AddString("Text-D"); }
                        if (TextMoveDir == 3) { StatusText.AddString("Text-DL"); }
                        if (TextMoveDir == 4) { StatusText.AddString("Text-L"); }
                        if (TextMoveDir == 5) { StatusText.AddString("Text-LU"); }
                        if (TextMoveDir == 6) { StatusText.AddString("Text-U"); }
                        if (TextMoveDir == 7) { StatusText.AddString("Text-UR"); }
                        break;
                    case WorkStateDef::WriteChar:
                        if (TextMoveDir == 0) { StatusText.AddString("Char-R"); }
                        if (TextMoveDir == 1) { StatusText.AddString("Char-RD"); }
                        if (TextMoveDir == 2) { StatusText.AddString("Char-D"); }
                        if (TextMoveDir == 3) { StatusText.AddString("Char-DL"); }
                        if (TextMoveDir == 4) { StatusText.AddString("Char-L"); }
                        if (TextMoveDir == 5) { StatusText.AddString("Char-LU"); }
                        if (TextMoveDir == 6) { StatusText.AddString("Char-U"); }
                        if (TextMoveDir == 7) { StatusText.AddString("Char-UR"); }
                        break;
                    case WorkStateDef::DrawChar:
                        if (EditorSemi_.get()->DiamondType == 0)
                        {
                            StatusText.AddString("Rect " + std::to_string(abs(EditorData_.get()->CursorXSize) + 1) + "x" + std::to_string(abs(EditorData_.get()->CursorYSize) + 1) + "  " + EditorSemi_.get()->GetFrameName(1));
                        }
                        else
                        {
                            StatusText.AddString("Dia " + std::to_string(abs(EditorData_.get()->CursorXSize) + 1) + "x" + std::to_string(abs(EditorData_.get()->CursorYSize) + 1) + "  " + EditorSemi_.get()->GetFrameName(2));
                        }
                        if (Screen::CharDouble(EditorData_.get()->DrawCharI) != 0)
                        {
                            StatusText.AddString(" ");
                        }
                        break;
                    case WorkStateDef::DrawPixel:
                        StatusText.AddRange(EditorPixelPaint_.get()->GetStatusInfo());
                        break;
                }
                if (WorkState != WorkStateDef::DrawPixel)
                {
                    switch (TextInsDelMode)
                    {
                        case 0: StatusText.AddString(" H-block"); break;
                        case 1: StatusText.AddString(" V-block"); break;
                        case 2: StatusText.AddString(" H-line"); break;
                        case 3: StatusText.AddString(" V-line"); break;
                        case 4: StatusText.AddString(" H-roll"); break;
                        case 5: StatusText.AddString(" V-roll"); break;
                    }
                }


                Screen::ScreenCursorMove(0, ScreenH - 1);
                if (StatusText.Count > ScreenW)
                {
                    StatusText.RemoveRange(ScreenW, StatusText.Count - ScreenW);
                }
                Screen::ScreenWriteText0(StatusText, StatusBack, StatusFore);
                Screen::ScreenWriteText0(Str::Spaces(ScreenW - StatusText.Count), StatusBack, StatusFore);
                if (DisplayState == DisplayStateDef::Info)
                {
                    Screen::ScreenCursorMove(ScreenW - 1, ScreenH - 1);
                }
                else
                {
                    Screen::ScreenCursorMove(EditorData_.get()->CursorX - DisplayX, EditorData_.get()->CursorY - DisplayY);
                }
                Screen::ScreenRefresh();
            }
            break;
    }
}

void Core0Editor::TextRepaint(bool Force)
{
    switch (DisplayState)
    {
        case DisplayStateDef::Editor:
        case DisplayStateDef::Charmap:
        case DisplayStateDef::DispConfig:
        case DisplayStateDef::FileMan:
            {
                int CountLinesDispY = EditorData_.get()->TextBuffer.CountLines() - DisplayY;
                for (int Y = 0; Y < WinTxtH; Y++)
                {
                    for (int X = 0; X < WinTxtW; X++)
                    {
                        if (!EditorData_.get()->TextBuffer.Get_(Y + DisplayY, X + DisplayX))
                        {
                            EditorData_.get()->TextBuffer.BlankChar();
                            EditorData_.get()->TextBuffer.Item_Char = 32;
                            if (Y < CountLinesDispY)
                            {
                                EditorData_.get()->TextBuffer.Item_ColorB = TextBeyondLineBack;
                                EditorData_.get()->TextBuffer.Item_ColorF = TextBeyondLineFore;
                            }
                            else
                            {
                                EditorData_.get()->TextBuffer.Item_ColorB = TextBeyondEndBack;
                                EditorData_.get()->TextBuffer.Item_ColorF = TextBeyondEndFore;
                            }
                        }
                        EditorData_.get()->ScrCharDisp_.Get(Y, X);
                        EditorData_.get()->ScrCharDisp_.CopyItem(EditorData_.get()->TextBuffer);
                        EditorData_.get()->ScrCharDisp_.Set(Y, X);
                        Screen::ScreenChar(X, Y, EditorData_.get()->TextBuffer.Item_Char, EditorData_.get()->TextBuffer.Item_ColorB, EditorData_.get()->TextBuffer.Item_ColorF, EditorData_.get()->TextBuffer.Item_ColorA, EditorData_.get()->TextBuffer.Item_FontW, EditorData_.get()->TextBuffer.Item_FontH);
                    }
                }

                for (int I = 0; I < CursorSetX.Count; I++)
                {
                    int X = CursorSetX[I];
                    int Y = CursorSetY[I];
                    EditorData_.get()->ScrCharDisp_.Get(Y, X);
                    int ColorB = EditorData_.get()->ScrCharDisp_.Item_ColorB;
                    int ColorF = EditorData_.get()->ScrCharDisp_.Item_ColorF;
                    if (ColorB < 0) ColorB = Screen::TextNormalBack;
                    if (ColorF < 0) ColorF = Screen::TextNormalFore;
                    EditorData_.get()->ScrCharDisp_.Item_ColorB = CoreStatic::ColorNegative(ColorB);
                    EditorData_.get()->ScrCharDisp_.Item_ColorF = CoreStatic::ColorNegative(ColorF);
                    Screen::ScreenChar(X, Y, EditorData_.get()->ScrCharDisp_.Item_Char, EditorData_.get()->ScrCharDisp_.Item_ColorB, EditorData_.get()->ScrCharDisp_.Item_ColorF, 0, EditorData_.get()->ScrCharDisp_.Item_FontW, EditorData_.get()->ScrCharDisp_.Item_FontH);
                }
            }
            break;
        case DisplayStateDef::Info:
            {
                for (int Y = 0; Y < WinTxtH; Y++)
                {
                    int Y_ = Y + EditorInfo_.get()->InfoY;
                    Str InfoTextLine;
                    if (Y_ < (EditorInfo_.get()->Info.Count))
                    {
                        int X_ = EditorInfo_.get()->InfoX;
                        if (X_ < (EditorInfo_.get()->Info[Y_].size()))
                        {
                            InfoTextLine.AddString(EditorInfo_.get()->Info[Y_].substr(X_));
                        }
                    }
                    InfoTextLine.AddPad(WinTxtW, 32);

                    for (int X = 0; X < WinTxtW; X++)
                    {
                        Screen::ScreenChar(X, Y, InfoTextLine[X], Screen::TextNormalBack + 16, Screen::TextNormalFore + 16, 0, 0, 0);
                    }
                }
            }
            break;
    }
}

std::string Core0Editor::BeyondIndicator()
{
    if ((EditorData_.get()->CursorY + EditorData_.get()->CursorFontH - 1) < EditorData_.get()->TextBuffer.CountLines())
    {
        for (int i = 0; i < EditorData_.get()->CursorFontH; i++)
        {
            if ((EditorData_.get()->CursorX + EditorData_.get()->CursorFontW - 1) >= EditorData_.get()->TextBuffer.CountItems(EditorData_.get()->CursorY + i))
            {
                return ";";
            }
        }
        return ":";
    }
    else
    {
        return ",";
    }
}

void Core0Editor::SetInfo(bool Enable, int N)
{
    if (Enable)
    {
        EditorInfo_.get()->CreateInfo(N);
        DisplayState = DisplayStateDef::Info;
        ScreenRefresh(true);
    }
    else
    {
        DisplayState = DisplayStateDef::Editor;
    }
}

void Core0Editor::CharmapPaintCursor(int X, int Y, int Chr, bool Sel, bool ChrPrev)
{
    int Chr1 = 32;
    if (EditorChar_.get()->FavPage)
    {
        if (ChrPrev && (Chr > 0))
        {
            Chr1 = Screen::CharDouble(EditorChar_.get()->FavChar[Chr - 1]);
            if (Chr1 == 0)
            {
                Chr1 = 32;
            }
        }
        Chr = EditorChar_.get()->FavChar[Chr];
    }
    else
    {
        Chr = Chr + (EditorChar_.get()->SelectCharPart(1) << 8);
        if (ChrPrev && (Chr > 0))
        {
            Chr1 = Screen::CharDouble(Chr - 1);
            if (Chr1 == 0)
            {
                Chr1 = 32;
            }
        }
    }

    int ChrDbl = Screen::CharDouble(Chr);

    if (Sel)
    {
        if (ChrDbl != 0)
        {
            ScreenChar0(X - 1, Y, '>', PopupBack, PopupFore);
        }
        else
        {
            ScreenChar0(X - 1, Y, '[', PopupBack, PopupFore);
        }
    }
    else
    {
        ScreenChar0(X - 1, Y, Chr1, PopupBack, PopupFore);
    }
    ScreenChar0(X + 0, Y, Chr, PopupBack, PopupFore);
    if (ChrDbl != 0)
    {
        ScreenChar0(X + 1, Y, ChrDbl, PopupBack, PopupFore);
    }
    else
    {
        if (Sel)
        {
            ScreenChar0(X + 1, Y, ']', PopupBack, PopupFore);
        }
        else
        {
            ScreenChar0(X + 1, Y, ' ', PopupBack, PopupFore);
        }
    }
}

void Core0Editor::CharmapPaint(int Depth)
{
    if (DisplayState == DisplayStateDef::DispConfig)
    {
        if (DisplayConfig_.get()->RequestRepaint)
        {
            Depth = 1;
        }
        else
        {
            Depth = 9;
        }
    }

    int FileListDispLen = 17;

    if ((Depth == 101) || (Depth == 102))
    {
        if (Depth == 102)
        {
            EditorData_.get()->DrawCharI = EditorChar_.get()->SelectCharCode();
            EditorData_.get()->DrawColoBI = EditorChar_.get()->SelectColoB;
            EditorData_.get()->DrawColoFI = EditorChar_.get()->SelectColoF;
            EditorData_.get()->DrawColoAI = EditorChar_.get()->SelectColoA;
        }

        DisplayState = DisplayStateDef::Editor;
        EditorScreenRefresh();
        ScreenRefresh(true);
        return;
    }

    int CharPosX = 0;
    int CharPosY = 0;
    switch (EditorChar_.get()->CharPosMode)
    {
        case 0:
            CharPosX = -1;
            CharPosY = -1;
            break;
        case 1:
            CharPosX = ScreenW - 34;
            CharPosY = -1;
            break;
        case 2:
            CharPosX = -1;
            CharPosY = ScreenH - 18;
            break;
        case 3:
            CharPosX = ScreenW - 34;
            CharPosY = ScreenH - 18;
            break;
    }

    // Repaint text
    if (Depth == 0)
    {
        //EditorScreenRefresh();
        ScreenRefresh(true);
    }

    // Background
    if (Depth <= 1)
    {
        for (int YY = 0; YY < 19; YY++)
        {
            for (int XX = 0; XX < 35; XX++)
            {
                ScreenChar0(CharPosX + XX, CharPosY + YY, 32, PopupBack, PopupFore);
            }
            ScreenChar0(CharPosX +  0, CharPosY + YY, 32, PopupFore, PopupBack);
            ScreenChar0(CharPosX + 34, CharPosY + YY, 32, PopupFore, PopupBack);
        }
        for (int XX = 0; XX < 35; XX++)
        {
            ScreenChar0(CharPosX + XX, CharPosY +  0, 32, PopupFore, PopupBack);
            ScreenChar0(CharPosX + XX, CharPosY + 18, 32, PopupFore, PopupBack);
        }
    }

    // Characters or colors
    if (Depth <= 2)
    {
        if (EditorChar_.get()->SelectorState == 1)
        {
            if (EditorChar_.get()->FavPage)
            {
                int Ptr = 0;
                for (int YY = 0; YY < 16; YY++)
                {
                    for (int XX = 0; XX < 16; XX++)
                    {
                        ScreenChar0(CharPosX + XX * 2 + 2, CharPosY + YY + 2, EditorChar_.get()->FavChar[Ptr], PopupBack, PopupFore);
                        int T = Screen::CharDouble(EditorChar_.get()->FavChar[Ptr]);
                        if (T != 0)
                        {
                            ScreenChar0(CharPosX + XX * 2 + 3, CharPosY + YY + 2, T, PopupBack, PopupFore);
                        }
                        Ptr++;
                    }
                }
            }
            else
            {
                int Ptr = EditorChar_.get()->SelectCharPart(1) << 8;
                for (int YY = 0; YY < 16; YY++)
                {
                    for (int XX = 0; XX < 16; XX++)
                    {
                        ScreenChar0(CharPosX + XX * 2 + 2, CharPosY + YY + 2, Ptr, PopupBack, PopupFore);
                        int T = Screen::CharDouble(Ptr);
                        if (T != 0)
                        {
                            ScreenChar0(CharPosX + XX * 2 + 3, CharPosY + YY + 2, T, PopupBack, PopupFore);
                        }
                        Ptr++;
                    }
                }
            }
        }
        if (EditorChar_.get()->SelectorState == 2)
        {
            for (int YY = 0; YY < 16; YY++)
            {
                for (int XX = 0; XX < 16; XX++)
                {
                    ScreenChar0(CharPosX + XX * 2 + 2, CharPosY + YY + 2, '#', YY + 16, XX + 16);
                }
            }
        }
    }

    // Selector cursor and information
    int ScrX = EditorChar_.get()->SelectColoF;
    int ScrY = EditorChar_.get()->SelectColoB;
    if (EditorChar_.get()->SelectorState == 1)
    {
        ScrX = EditorChar_.get()->SelectCharPart(2);
        ScrY = EditorChar_.get()->SelectCharPart(3);
    }
    int SelX = CharPosX + ScrX * 2 + 2;
    int SelY = CharPosY + ScrY + 2;
    if (Depth <= 3)
    {
        int Chr = EditorChar_.get()->SelectCharPart(0);
        if (EditorChar_.get()->SelectorState == 1)
        {
            if (Depth > 1)
            {
                if (ScrY > 0)
                {
                    CharmapPaintCursor(SelX, SelY - 1, Chr - 16, false, ScrX > 0);
                }
                if (ScrX > 0)
                {
                    CharmapPaintCursor(SelX - 2, SelY, Chr - 1, false, ScrX > 1);
                }
                if ((ScrX == 0) && (ScrY > 0))
                {
                    CharmapPaintCursor(SelX + 30, SelY - 1, Chr - 1, false, true);
                }
                if (ScrX < 15)
                {
                    CharmapPaintCursor(SelX + 2, SelY, Chr + 1, false, true);
                }
                if ((ScrX == 15) && (ScrY < 15))
                {
                    CharmapPaintCursor(SelX - 30, SelY + 1, Chr + 1, false, false);
                }
                if (ScrY < 15)
                {
                    CharmapPaintCursor(SelX, SelY + 1, Chr + 16, false, ScrX > 0);
                }
            }
            CharmapPaintCursor(SelX, SelY, Chr, true, ScrX > 0);

            std::string DispCode = TextWork::CharCode(EditorChar_.get()->SelectCharCode(), 2);

            ScreenChar0(CharPosX + 1, CharPosY + 1, DispCode[0], PopupBack, PopupFore);
            ScreenChar0(CharPosX + 2, CharPosY + 1, DispCode[1], PopupBack, PopupFore);
            ScreenChar0(CharPosX + 3, CharPosY + 1, DispCode[2], PopupBack, PopupFore);
            ScreenChar0(CharPosX + 4, CharPosY + 1, DispCode[3], PopupBack, PopupFore);
            ScreenChar0(CharPosX + 5, CharPosY + 1, DispCode[4], PopupBack, PopupFore);

            ScreenChar0(CharPosX + 6, CharPosY + 1, 32, PopupBack, PopupFore);
            ScreenChar0(CharPosX + 7, CharPosY + 1, 32, PopupBack, PopupFore);
            ScreenChar0(CharPosX + 8, CharPosY + 1, 32, PopupBack, PopupFore);
            ScreenChar0(CharPosX + 9, CharPosY + 1, 32, PopupBack, PopupFore);

            ScreenChar0(CharPosX + 7, CharPosY + 1, EditorChar_.get()->SelectCharCode(), PopupBack, PopupFore);
            int SelectCharCodeDbl = Screen::CharDouble(EditorChar_.get()->SelectCharCode());
            if (SelectCharCodeDbl != 0)
            {
                ScreenChar0(CharPosX + 8, CharPosY + 1, SelectCharCodeDbl, PopupBack, PopupFore);
            }

            if (EditorChar_.get()->FavPage)
            {
                int C = EditorChar_.get()->SelectCharFavGet();
                if ((C >= 32) && (C <= 126))
                {
                    ScreenChar0(CharPosX + 10, CharPosY + 1, '[', PopupBack, PopupFore);
                    ScreenChar0(CharPosX + 11, CharPosY + 1, C, PopupBack, PopupFore);
                    ScreenChar0(CharPosX + 12, CharPosY + 1, ']', PopupBack, PopupFore);
                }
                else
                {
                    ScreenChar0(CharPosX + 10, CharPosY + 1, 'F', PopupBack, PopupFore);
                    ScreenChar0(CharPosX + 11, CharPosY + 1, 'A', PopupBack, PopupFore);
                    ScreenChar0(CharPosX + 12, CharPosY + 1, 'V', PopupBack, PopupFore);
                }
            }
            else
            {
                ScreenChar0(CharPosX + 10, CharPosY + 1, 32, PopupBack, PopupFore);
                ScreenChar0(CharPosX + 11, CharPosY + 1, 32, PopupBack, PopupFore);
                ScreenChar0(CharPosX + 12, CharPosY + 1, 32, PopupBack, PopupFore);
            }

            if (EditorChar_.get()->SelectToFav >= 0)
            {
                DispCode = TextWork::CharCode(EditorChar_.get()->SelectToFav, 2);
            }
            else
            {
                DispCode = "     ";
            }

            ScreenChar0(CharPosX + 24, CharPosY + 1, DispCode[0], PopupBack, PopupFore);
            ScreenChar0(CharPosX + 25, CharPosY + 1, DispCode[1], PopupBack, PopupFore);
            ScreenChar0(CharPosX + 26, CharPosY + 1, DispCode[2], PopupBack, PopupFore);
            ScreenChar0(CharPosX + 27, CharPosY + 1, DispCode[3], PopupBack, PopupFore);
            ScreenChar0(CharPosX + 28, CharPosY + 1, DispCode[4], PopupBack, PopupFore);

            if (EditorChar_.get()->WinBitmapEnabled)
            {
                ScreenChar0(CharPosX + 30, CharPosY + 1, ' ', PopupBack, PopupFore);
                ScreenChar0(CharPosX + 31, CharPosY + 1, ' ', PopupBack, PopupFore);
                ScreenChar0(CharPosX + 32, CharPosY + 1, ' ', PopupBack, PopupFore);
            }
            else
            {
                ScreenChar0(CharPosX + 30, CharPosY + 1, 'A', PopupBack, PopupFore);
                ScreenChar0(CharPosX + 31, CharPosY + 1, 'L', PopupBack, PopupFore);
                ScreenChar0(CharPosX + 32, CharPosY + 1, 'L', PopupBack, PopupFore);
            }
        }
        if (EditorChar_.get()->SelectorState == 2)
        {
            if (Depth > 1)
            {
                if ((ScrX < 0) && (ScrY < 0))
                {
                    for (int I = 1; I <= 16; I++)
                    {
                        ScreenChar0(SelX + 1, SelY + I, ' ', PopupBack, PopupFore);
                        ScreenChar0(SelX + 3, SelY + I, ' ', PopupBack, PopupFore);

                        ScreenChar0(SelX + 31, SelY + I, ' ', PopupBack, PopupFore);
                        ScreenChar0(SelX + 33, SelY + I, ' ', PopupBack, PopupFore);
                    }
                    for (int I = 1; I <= 33; I += 2)
                    {
                        ScreenChar0(SelX + I, SelY + 1, ' ', PopupBack, PopupFore);
                        ScreenChar0(SelX + I, SelY + 16, ' ', PopupBack, PopupFore);
                    }
                }

                if (ScrX >= 0)
                {
                    if (ScrY < 0)
                    {
                        for (int I = 1; I <= 16; I++)
                        {
                            if (ScrX > 0)
                            {
                                ScreenChar0(SelX - 3, SelY + I, ' ', PopupBack, PopupFore);
                            }
                            if (ScrX == 0)
                            {
                                ScreenChar0(SelX + 29, SelY + I, ' ', PopupBack, PopupFore);
                                ScreenChar0(SelX + 31, SelY + I, ' ', PopupBack, PopupFore);
                            }
                            if (ScrX < 15)
                            {
                                ScreenChar0(SelX + 3, SelY + I, ' ', PopupBack, PopupFore);
                            }
                            if (ScrX == 15)
                            {
                                ScreenChar0(SelX - 31, SelY + I, ' ', PopupBack, PopupFore);
                                ScreenChar0(SelX - 29, SelY + I, ' ', PopupBack, PopupFore);
                            }
                        }
                    }
                    if (ScrY == 0)
                    {
                        for (int I = 0; I <= 15; I++)
                        {
                            ScreenChar0(SelX - 1, SelY + I, ' ', PopupBack, PopupFore);
                            ScreenChar0(SelX + 1, SelY + I, ' ', PopupBack, PopupFore);
                        }
                    }
                    if (ScrY > 0)
                    {
                        ScreenChar0(SelX - 1, SelY - 1, ' ', PopupBack, PopupFore);
                        ScreenChar0(SelX + 1, SelY - 1, ' ', PopupBack, PopupFore);
                    }
                    if ((ScrY >= 0) && (ScrY < 15))
                    {
                        ScreenChar0(SelX - 1, SelY + 1, ' ', PopupBack, PopupFore);
                        ScreenChar0(SelX + 1, SelY + 1, ' ', PopupBack, PopupFore);
                    }
                    if (ScrY == 15)
                    {
                        for (int I = 0; I <= 15; I++)
                        {
                            ScreenChar0(SelX - 1, SelY - I, ' ', PopupBack, PopupFore);
                            ScreenChar0(SelX + 1, SelY - I, ' ', PopupBack, PopupFore);
                        }
                    }
                }

                if (ScrY >= 0)
                {
                    if (ScrX < 0)
                    {
                        if (ScrY > 0)
                        {
                            ScreenChar0(SelX +  1, SelY - 1, ' ', PopupBack, PopupFore);
                            ScreenChar0(SelX + 33, SelY - 1, ' ', PopupBack, PopupFore);
                        }
                        if (ScrY < 15)
                        {
                            ScreenChar0(SelX +  1, SelY + 1, ' ', PopupBack, PopupFore);
                            ScreenChar0(SelX + 33, SelY + 1, ' ', PopupBack, PopupFore);
                        }
                        ScreenChar0(SelX + 31, SelY, ' ', PopupBack, PopupFore);
                        ScreenChar0(SelX +  3, SelY, ' ', PopupBack, PopupFore);
                    }
                    if (ScrX == 0)
                    {
                        ScreenChar0(SelX + 29, SelY, ' ', PopupBack, PopupFore);
                        ScreenChar0(SelX + 31, SelY, ' ', PopupBack, PopupFore);
                    }
                    if (ScrX > 0)
                    {
                        ScreenChar0(SelX - 3, SelY, ' ', PopupBack, PopupFore);
                    }
                    if ((ScrX >= 0) && (ScrX < 15))
                    {
                        ScreenChar0(SelX + 3, SelY, ' ', PopupBack, PopupFore);
                    }
                    if (ScrX == 15)
                    {
                        ScreenChar0(SelX - 31, SelY, ' ', PopupBack, PopupFore);
                        ScreenChar0(SelX - 29, SelY, ' ', PopupBack, PopupFore);
                    }
                }
            }

            if ((ScrX >= 0) && (ScrY >= 0))
            {
                ScreenChar0(SelX - 1, SelY, '[', PopupBack, PopupFore);
                ScreenChar0(SelX + 1, SelY, ']', PopupBack, PopupFore);
            }
            if ((ScrX < 0) && (ScrY >= 0))
            {
                ScreenChar0(SelX +  1, SelY, '[', PopupBack, PopupFore);
                ScreenChar0(SelX + 33, SelY, ']', PopupBack, PopupFore);
            }
            if ((ScrX >= 0) && (ScrY < 0))
            {
                for (int I = 1; I <= 16; I++)
                {
                    ScreenChar0(SelX - 1, SelY + I, '[', PopupBack, PopupFore);
                    ScreenChar0(SelX + 1, SelY + I, ']', PopupBack, PopupFore);
                }
            }

            if (ScrY >= 0)
            {
                ScreenChar0(CharPosX + 2, CharPosY + 1, Hex::IntToHex4(ScrY)[0], PopupBack, PopupFore);
            }
            else
            {
                ScreenChar0(CharPosX + 2, CharPosY + 1, '-', PopupBack, PopupFore);
            }
            if (ScrX >= 0)
            {
                ScreenChar0(CharPosX + 3, CharPosY + 1, Hex::IntToHex4(ScrX)[0], PopupBack, PopupFore);
            }
            else
            {
                ScreenChar0(CharPosX + 3, CharPosY + 1, '-', PopupBack, PopupFore);
            }

            std::string StateMsg = " Text+color ";
            if (!EditorData_.get()->ToggleDrawText)
            {
                StateMsg = " Color      ";
            }
            if (!EditorData_.get()->ToggleDrawColo)
            {
                StateMsg = " Text       ";
            }

            std::string EOL1 = std::to_string(CoreAnsi_.get()->ANSI_CR);
            std::string EOL2 = std::to_string(CoreAnsi_.get()->ANSI_LF);

            StateMsg = CoreStatic::GetAttribText(EditorChar_.get()->SelectColoA) + StateMsg + EOL1 + EOL2 + TextWork::LPad(std::to_string(CoreAnsi_.get()->AnsiMaxX) + "x" + std::to_string(CoreAnsi_.get()->AnsiMaxY), 7, " ");

            for (int i = 0; i < StateMsg.length(); i++)
            {
                ScreenChar0(CharPosX + 5 + i, CharPosY + 1, StateMsg[i], PopupBack, PopupFore);
            }
        }
    }

    // Cursor position
    if (ScrX < 0) SelX = 1;
    if (ScrY < 0) SelY = 1;
    Screen::ScreenCursorMove(SelX, SelY);
    Screen::ScreenRefresh();

    if (DisplayState == DisplayStateDef::DispConfig)
    {
        DisplayConfig_.get()->Repaint();
    }
}

void Core0Editor::FileLoad()
{
    BinaryFile_.get()->FileImport(-1, false);
    //FileLoad2();
}

void Core0Editor::FileLoad2()
{
    TempMemoB.Add(EditorData_.get()->ToggleDrawText);
    TempMemoB.Add(EditorData_.get()->ToggleDrawColo);
    TempMemoI.Add(EditorData_.get()->DrawColoBI);
    TempMemoI.Add(EditorData_.get()->DrawColoFI);
    TempMemoI.Add(EditorData_.get()->DrawColoAI);
    EditorData_.get()->DrawColoBI = -1;
    EditorData_.get()->DrawColoFI = -1;
    EditorData_.get()->DrawColoAI = 0;
    EditorData_.get()->ToggleDrawText = true;
    EditorData_.get()->ToggleDrawColo = true;

    Str FileTxt;
    BinaryFile_.get()->Load(FileTxt);


    if (BinaryFile_.get()->ItemGet(-1).Ansi)
    {
        CoreAnsi_.get()->AnsiTerminalResize(CoreAnsi_.get()->AnsiMaxX, CoreAnsi_.get()->AnsiMaxY, 0);
        CoreAnsi_.get()->AnsiProcessReset(true, false, 0);
        CoreAnsi_.get()->AnsiRingBell = false;
        CoreAnsi_.get()->AnsiProcessSupply(FileTxt);
        if (FileReadSteps > 0)
        {
            CoreAnsi_.get()->AnsiProcess(FileReadSteps);
        }
        else
        {
            CoreAnsi_.get()->AnsiProcess(-1);
        }
        EditorData_.get()->TextBuffer.Clear();
        int i_ = 0;


        // Before screen
        for (int i = 0; i < CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy1__.CountLines(); i++)
        {
            EditorData_.get()->TextBuffer.AppendLine();
            int LineMax = (CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy1__.CountItems(i)) - 1;
            for (int ii = 0; ii <= LineMax; ii++)
            {
                CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy1__.Get(i, ii);
                EditorData_.get()->TextBuffer.CopyItem(CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy1__);
                EditorData_.get()->TextBuffer.Append(i_);
            }
            i_++;
        }

        // Screen
        for (int i = 0; i < CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy__.CountLines(); i++)
        {
            EditorData_.get()->TextBuffer.AppendLine();
            int LineMax = (CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy__.CountItems(i)) - 1;
            for (int ii = 0; ii <= LineMax; ii++)
            {
                CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy__.Get(i, ii);
                EditorData_.get()->TextBuffer.CopyItem(CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy__);
                EditorData_.get()->TextBuffer.Append(i_);
            }
            i_++;
        }

        // After screen
        for (int i = (CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy2__.CountLines() - 1); i >= 0; i--)
        {
            EditorData_.get()->TextBuffer.AppendLine();
            int LineMax = (CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy2__.CountItems(i)) - 1;
            for (int ii = 0; ii <= LineMax; ii++)
            {
                CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy2__.Get(i, ii);
                EditorData_.get()->TextBuffer.CopyItem(CoreAnsi_.get()->AnsiState_.__AnsiLineOccupy2__);
                EditorData_.get()->TextBuffer.Append(i_);
            }
            i_++;
        }
    }
    else
    {
        XList<Str> FileTxtList;
        TextWork::StringToLines(FileTxt, FileTxtList);
        EditorData_.get()->TextBuffer.Clear();
        EditorData_.get()->TextBuffer.BlankChar();
        for (int Y = 0; Y < FileTxtList.Count; Y++)
        {
            for (int X = 0; X < FileTxtList[Y].Count; X++)
            {
                EditorData_.get()->CharPut0(X, Y, FileTxtList[Y][X]);
            }
        }
    }
    EditorData_.get()->TextBuffer.TrimLines();

    EditorScreenRefresh();
    ScreenRefresh(true);
    EditorData_.get()->DrawColoAI = TempMemoI.PopLast();
    EditorData_.get()->DrawColoFI = TempMemoI.PopLast();
    EditorData_.get()->DrawColoBI = TempMemoI.PopLast();
    EditorData_.get()->ToggleDrawColo = TempMemoB.PopLast();
    EditorData_.get()->ToggleDrawText = TempMemoB.PopLast();
    TickRepaint = TickRepaintX;
}

void Core0Editor::FileSave()
{
    AnsiFile AnsiFile_;
    AnsiFile_.Reset();
    Str FileTxt;
    for (int i = 0; i < EditorData_.get()->TextBuffer.CountLines(); i++)
    {
        if (BinaryFile_.get()->ItemGet(-1).Ansi)
        {
            FileTxt.AddRange(AnsiFile_.Process(EditorData_.get()->TextBuffer, i, CoreAnsi_.get()->AnsiMaxX));
        }
        else
        {
            FileTxt.AddRange(EditorData_.get()->TextBuffer.GetLineString(i));
            FileTxt.Add('\n');
        }
    }
    BinaryFile_.get()->Save(FileTxt);
    BinaryFile_.get()->FileExport(-1, false);
}
