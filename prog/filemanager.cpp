#include "filemanager.h"

FileManager::FileManager()
{

}

void FileManager::Repaint()
{
    int PosX = -1;
    int PosY = -1;
    if ((Pos & 1) > 0)
    {
        PosX = Screen::CurrentW - SizeW - 1;
    }
    if ((Pos & 2) > 0)
    {
        PosY = Screen::CurrentH - SizeH - 1;
    }

    // Background
    if (RepaintDepth <= 1)
    {
        for (int YY = 0; YY < (SizeH + 2); YY++)
        {
            Screen::ScreenChar(PosX + 0, PosY + YY, 32, PopupFore, PopupBack, 0, 0, 0);
            Screen::ScreenChar(PosX + SizeW + 1, PosY + YY, 32, PopupFore, PopupBack, 0, 0, 0);
        }
        for (int XX = 0; XX < (SizeW + 1); XX++)
        {
            Screen::ScreenChar(PosX + XX, PosY +  0, 32, PopupFore, PopupBack, 0, 0, 0);
            Screen::ScreenChar(PosX + XX, PosY + SizeH + 1, 32, PopupFore, PopupBack, 0, 0, 0);
        }
    }

    if (RepaintDepth <= 2)
    {
        for (int YY = 1; YY < (SizeH + 1); YY++)
        {
            for (int XX = 1; XX < (SizeW + 1); XX++)
            {
                Screen::ScreenChar(PosX + XX, PosY + YY, 32, PopupBack, PopupFore, 0, 0, 0);
            }
        }
    }

    switch (ManagerState)
    {
        case ManagerStateDef::Files:
            RepaintFiles(PosX, PosY);
            return;
        case ManagerStateDef::Attrib:
            RepaintAttrib(PosX, PosY);
            return;
        case ManagerStateDef::Insert:
            RepaintInsert(PosX, PosY);
            return;
        case ManagerStateDef::Delete:
            RepaintDelete(PosX, PosY);
            return;
        case ManagerStateDef::Process:
            RepaintProcess(PosX, PosY);
            return;
    }

}

void FileManager::RepaintFiles(int PosX, int PosY)
{
    int DispLen = SizeH - MarginHead - MarginFoot;

    if (BinaryFile_.get()->DirItemOff < (BinaryFile_.get()->DirItemIdx - DispLen + 1))
    {
        BinaryFile_.get()->DirItemOff = (BinaryFile_.get()->DirItemIdx - DispLen + 1);
        RepaintDepth = 2;
    }

    if (BinaryFile_.get()->DirItemOff > (BinaryFile_.get()->DirItemIdx))
    {
        BinaryFile_.get()->DirItemOff = (BinaryFile_.get()->DirItemIdx);
        RepaintDepth = 2;
    }


    // File list
    if (RepaintDepth <= 2)
    {
        int DirL = std::min(BinaryFile_.get()->ListDir.Count, SizeW);
        int DirO = BinaryFile_.get()->ListDir.Count - DirL;
        for (int I = 0; I < DirL; I++)
        {
            Screen::ScreenChar(PosX + I + 1, PosY + 1, BinaryFile_.get()->ListDir[I + DirO], PopupBack, PopupFore, 0, 0, 0);
        }
        for (int I = DirL; I < SizeW; I++)
        {
            Screen::ScreenChar(PosX + I + 1, PosY + 1, ' ', PopupBack, PopupFore, 0, 0, 0);
        }

        for (int Idx = BinaryFile_.get()->DirItemOff; Idx < std::min(BinaryFile_.get()->DirItemName.Count, DispLen + BinaryFile_.get()->DirItemOff); Idx++)
        {
            Str DirItemName = BinaryFile_.get()->DirItemName[Idx];
            if (DirItemName[DirItemName.Count - 1] == '/')
            {
                DirItemName.PopLast();
            }
            int DirItemNameSlash = DirItemName.IndexOf('/', 0 - (DirItemName.Count + 10));
            if (DirItemNameSlash >= 0)
            {
                DirItemName.RemoveRange(0, DirItemNameSlash + 1);
            }
            int ItemL = std::min(DirItemName.Count, SizeW - 1);
            int ItemO = DirItemName.Count - ItemL;
            int PopupBack_ = PopupBack;
            int PopupFore_ = PopupFore;
            if (BinaryFile_.get()->GetFileType(Idx) < 0)
            {
                PopupBack_ = PopupFore;
                PopupFore_ = PopupBack;
            }
            int ItemY = PosY + 1 + Idx + 1 - BinaryFile_.get()->DirItemOff;
            for (int I = 0; I < ItemL; I++)
            {
                Screen::ScreenChar(PosX + I + 2, ItemY, DirItemName[I], PopupBack_, PopupFore_, 0, 0, 0);
            }
            for (int I = ItemL; I < (SizeW - 1); I++)
            {
                Screen::ScreenChar(PosX + I + 2, ItemY, ' ', PopupBack, PopupFore, 0, 0, 0);
            }
        }
    }

    // Select cursor
    int Sel = 0;
    if (RepaintDepth <= 3)
    {
        if (BinaryFile_.get()->DirItemName.Count > 0)
        {
            for (int Idx = 0; Idx < DispLen; Idx++)
            {
                if ((Idx + BinaryFile_.get()->DirItemOff) == (BinaryFile_.get()->DirItemIdx))
                {
                    Sel = Idx;
                    Screen::ScreenChar(PosX + 0 + 1, PosY + 1 + Idx + 1, '>', PopupBack, PopupFore, 0, 0, 0);
                }
                else
                {
                    Screen::ScreenChar(PosX + 0 + 1, PosY + 1 + Idx + 1, ' ', PopupBack, PopupFore, 0, 0, 0);
                }
            }
        }
    }

    // File info and keyboard shortcuts
    Str Info1;
    Str Info2;
    Str Info3;
    Str Info4;
    Info1.AddString("Ins - New file  Home - Upload");
    Info2.AddString("Del - Remove    End - Download");
    Info3.AddString("Space - Read and write options");
    {
        switch (BinaryFile_.get()->CurrentFileAttrGet(1))
        {
            case 0: Info4.AddString("Auto/T"); break;
            case 1: Info4.AddString("Auto/A"); break;
            case 2: Info4.AddString("Text"); break;
            case 3: Info4.AddString("Ansi"); break;
            case 4: Info4.AddString("Bin"); break;
            case 5: Info4.AddString("XBin"); break;
        }
        int CodecN = BinaryFile_.get()->CurrentFileAttrGet(0);
        Info4.AddString("   ");
        Info4.AddString(CodecN);
        int CodecNN = TextCodec::CodecListNumber.IndexOfBin(CodecN);
        if (CodecNN >= 0)
        {
            Info4.AddString("/");
            Info4.AddString(TextCodec::CodecListName[CodecNN]);
        }
    }
    Info1.AddPad(SizeW, 32);
    Info2.AddPad(SizeW, 32);
    Info3.AddPad(SizeW, 32);
    Info4.AddPad(SizeW, 32);
    for (int I = 0; I < SizeW; I++)
    {
        Screen::ScreenChar(PosX + 1 + I, PosY + 1 + SizeH - MarginFoot, Info1[I], PopupBack, PopupFore, 0, 0, 0);
        Screen::ScreenChar(PosX + 1 + I, PosY + 2 + SizeH - MarginFoot, Info2[I], PopupBack, PopupFore, 0, 0, 0);
        Screen::ScreenChar(PosX + 1 + I, PosY + 3 + SizeH - MarginFoot, Info3[I], PopupBack, PopupFore, 0, 0, 0);
        Screen::ScreenChar(PosX + 1 + I, PosY + 4 + SizeH - MarginFoot, Info4[I], PopupBack, PopupFore, 0, 0, 0);
    }

    Screen::ScreenCursorMove(PosX + 1, PosY + 2 + Sel);
    Screen::ScreenRefresh();
}

void FileManager::RepaintAttrib(int PosX, int PosY)
{
    Screen::ScreenCursorMove(PosX + 1, PosY + 1);
    switch (AttribParam)
    {
        case 0:
            Screen::ScreenWriteText(Str("  "), PopupBack, PopupFore);
            Screen::ScreenWriteText(Str(" Codec "), PopupFore, PopupBack);
            Screen::ScreenWriteText(Str("  "), PopupBack, PopupFore);
            Screen::ScreenWriteText(Str(" Type "), PopupBack, PopupFore);

            if (AttribValOffset[0] >= AttribVal[0])
            {
                AttribValOffset[0] =  AttribVal[0];
            }

            if (AttribValOffset[0] <= (AttribVal[0] - SizeH + 2))
            {
                AttribValOffset[0] =  (AttribVal[0] - SizeH + 2);
            }

            for (int I = 0; I < (SizeH - 1); I++)
            {
                Screen::ScreenCursorMove(PosX + 1, PosY + 2 + I);
                if (((I + AttribValOffset[0]) >= 0) && ((I + (AttribValOffset[0])) < TextCodec::CodecListNumber.Count))
                {
                    Screen::ScreenWriteText(Str(" " + std::to_string(TextCodec::CodecListNumber[I + AttribValOffset[0]]) + "/" + TextCodec::CodecListName[I + AttribValOffset[0]]), PopupBack, PopupFore);
                }
            }

            Screen::ScreenCursorMove(PosX + 1, PosY + 2 + AttribVal[0] - AttribValOffset[0]);
            Screen::ScreenWriteText(Str(">"), PopupBack, PopupFore);
            Screen::ScreenCursorMove(PosX + 1, PosY + 2 + AttribVal[0] - AttribValOffset[0]);
            break;
        case 1:
            Screen::ScreenWriteText(Str("  "), PopupBack, PopupFore);
            Screen::ScreenWriteText(Str(" Codec "), PopupBack, PopupFore);
            Screen::ScreenWriteText(Str("  "), PopupBack, PopupFore);
            Screen::ScreenWriteText(Str(" Type "), PopupFore, PopupBack);

            Screen::ScreenCursorMove(PosX + 1, PosY + 2);
            Screen::ScreenWriteText(Str(" Auto - Text"), PopupBack, PopupFore);
            Screen::ScreenCursorMove(PosX + 1, PosY + 3);
            Screen::ScreenWriteText(Str(" Auto - Ansi"), PopupBack, PopupFore);
            Screen::ScreenCursorMove(PosX + 1, PosY + 4);
            Screen::ScreenWriteText(Str(" Text"), PopupBack, PopupFore);
            Screen::ScreenCursorMove(PosX + 1, PosY + 5);
            Screen::ScreenWriteText(Str(" Ansi"), PopupBack, PopupFore);
            Screen::ScreenCursorMove(PosX + 1, PosY + 6);
            Screen::ScreenWriteText(Str(" Bin"), PopupBack, PopupFore);
            Screen::ScreenCursorMove(PosX + 1, PosY + 7);
            Screen::ScreenWriteText(Str(" XBin"), PopupBack, PopupFore);

            Screen::ScreenCursorMove(PosX + 1, PosY + 2 + AttribVal[1]);
            Screen::ScreenWriteText(Str(">"), PopupBack, PopupFore);
            Screen::ScreenCursorMove(PosX + 1, PosY + 2 + AttribVal[1]);
            break;
    }


    Screen::ScreenRefresh();
}

void FileManager::RepaintInsert(int PosX, int PosY)
{
    Screen::ScreenCursorMove(PosX + 1, PosY + 1);
    Screen::ScreenWriteText(Str("Enter file name and press ENTER."), PopupBack, PopupFore);

    Screen::ScreenCursorMove(PosX + 1, PosY + 2);
    Screen::ScreenWriteText(Str("To create sub-dir, use '/'"), PopupBack, PopupFore);

    Screen::ScreenCursorMove(PosX + 1, PosY + 3);
    Screen::ScreenWriteText(Str("in name, but not at the end."), PopupBack, PopupFore);

    Screen::ScreenCursorMove(PosX + 1, PosY + 4);
    Screen::ScreenWriteText(Str("You can not create empty dir."), PopupBack, PopupFore);


    int StrPosX = 1;
    int StrPosY = 6;
    Screen::ScreenCursorMove(PosX + StrPosX, PosY + StrPosY);

    for (int I = 0; I < InsertFileName.Count; I++)
    {
        Screen::ScreenWriteChar(InsertFileName[I], PopupBack, PopupFore);
        StrPosX++;
        if (StrPosX >= (SizeW + 1))
        {
            StrPosX = 1;
            StrPosY++;
            Screen::ScreenCursorMove(PosX + StrPosX, PosY + StrPosY);
        }
    }

    Screen::ScreenCursorMove(PosX + StrPosX, PosY + StrPosY);

    Screen::ScreenRefresh();
}

void FileManager::RepaintDelete(int PosX, int PosY)
{
    Screen::ScreenCursorMove(PosX + 2, PosY + 2);

    int ItemIdx = BinaryFile_.get()->DirItemIdx;
    if (BinaryFile_.get()->GetFileType(ItemIdx) >= 0)
    {
        Screen::ScreenWriteText(Str("Press ENTER to delete file."), PopupBack, PopupFore);
    }
    else
    {
        Screen::ScreenWriteText(Str("Press ENTER to delete sub-dir."), PopupBack, PopupFore);
    }

    Screen::ScreenCursorMove(PosX + 2, PosY + 4);
    Screen::ScreenWriteText(Str("Press other key to cancel."), PopupBack, PopupFore);


    Screen::ScreenRefresh();
}

void FileManager::RepaintProcess(int PosX, int PosY)
{
    int L = BinaryFile_.get()->ProcessInfo.Count;
    for (int I = 0; I < L; I++)
    {
        Screen::ScreenChar(PosX + 1 + I, PosY + 1, BinaryFile_.get()->ProcessInfo[I], PopupBack, PopupFore, 0, 0, 0);
    }

    Screen::ScreenCursorMove(PosX + 1 + L, PosY + 1);
    Screen::ScreenRefresh();
    if (L > 0)
    {
        if (BinaryFile_.get()->ProcessInfo[0] == '~')
        {
            BinaryFile_.get()->ProcessInfo.Clear();
            BinaryFile_.get()->SetDir(Str("."));
            ManagerState = ManagerStateDef::Files;
            Repaint();
        }
    }
}

void FileManager::EventTick()
{
    if (BinaryFile_.get()->ProcessInfo.Count > 0)
    {
        ManagerState = ManagerStateDef::Process;
        RepaintDepth = 2;
        Repaint();
    }
}

void FileManager::EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    RequestRepaint = false;
    RequestCloseOld = false;
    RequestCloseNew = false;
    switch (ManagerState)
    {
        case ManagerStateDef::Files:
            EventKeyFiles(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            return;
        case ManagerStateDef::Attrib:
            EventKeyAttrib(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            return;
        case ManagerStateDef::Insert:
            EventKeyInsert(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            return;
        case ManagerStateDef::Delete:
            EventKeyDelete(KeyName, KeyChar, ModShift, ModCtrl, ModAlt);
            return;
        case ManagerStateDef::Process:
            return;
    }
}

void FileManager::EventKeyFiles(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    switch (_(KeyName.c_str()))
    {
        case _("ArrowUp"):
            if (BinaryFile_.get()->DirItemName.Count > 0)
            {
                BinaryFile_.get()->DirItemIdx--;
                if (BinaryFile_.get()->DirItemIdx < 0)
                {
                    BinaryFile_.get()->DirItemIdx = (BinaryFile_.get()->DirItemName.Count - 1);
                }
                RepaintDepth = 3;
                Repaint();
            }
            break;
        case _("ArrowDown"):
            if (BinaryFile_.get()->DirItemName.Count > 0)
            {
                BinaryFile_.get()->DirItemIdx++;
                if (BinaryFile_.get()->DirItemIdx > (BinaryFile_.get()->DirItemName.Count - 1))
                {
                    BinaryFile_.get()->DirItemIdx = 0;
                }
                RepaintDepth = 3;
                Repaint();
            }
            break;
        case _("PageUp"):
            if (BinaryFile_.get()->DirItemName.Count > 0)
            {
                BinaryFile_.get()->DirItemIdx -= 10;
                if (BinaryFile_.get()->DirItemIdx < 0)
                {
                    BinaryFile_.get()->DirItemIdx = 0;
                }
                RepaintDepth = 3;
                Repaint();
            }
            break;
        case _("PageDown"):
            if (BinaryFile_.get()->DirItemName.Count > 0)
            {
                BinaryFile_.get()->DirItemIdx += 10;
                if (BinaryFile_.get()->DirItemIdx > (BinaryFile_.get()->DirItemName.Count - 1))
                {
                    BinaryFile_.get()->DirItemIdx = (BinaryFile_.get()->DirItemName.Count - 1);
                }
                RepaintDepth = 3;
                Repaint();
            }
            break;
        case _("Tab"):
            Pos++;
            if (Pos == 4)
            {
                Pos = 0;
            }
            RequestRepaint = true;
            RepaintDepth = 0;
            break;
        case _("Escape"):
        case _("Backspace"):
            BinaryFile_.get()->ManagerInfoPop();
            RequestRepaint = true;
            RequestCloseOld = true;
            break;
        case _("Enter"):
        case _("NumpadEnter"):
            if (BinaryFile_.get()->GetFileType(BinaryFile_.get()->DirItemIdx) >= 0)
            {
                BinaryFile_.get()->CurrentFileName = BinaryFile_.get()->DirItemName[BinaryFile_.get()->DirItemIdx];
                RequestRepaint = true;
                RequestCloseNew = true;
            }
            else
            {
                Str DirName = BinaryFile_.get()->DirItemName[BinaryFile_.get()->DirItemIdx];
                DirName.PopLast();
                int Idx_ = DirName.IndexOf('/', 0 - DirName.Count);
                if (Idx_ > 0)
                {
                    DirName.RemoveRange(0, Idx_ + 1);
                }
                BinaryFile_.get()->SetDir(DirName);
                RepaintDepth = 2;
                Repaint();
            }
            break;
        case _("Home"):
            BinaryFile_.get()->FileUpload();
            break;
        case _("End"):
            BinaryFile_.get()->FileDownload();
            break;
        case _("Insert"):
            InsertFileName.Clear();
            ManagerState = ManagerStateDef::Insert;
            RepaintDepth = 2;
            Repaint();
            break;
        case _("Delete"):
            ManagerState = ManagerStateDef::Delete;
            RepaintDepth = 2;
            Repaint();
            break;
        case _("Space"):
            AttribParam = 0;
            AttribVal[0] = TextCodec::CodecListNumber.IndexOf(BinaryFile_.get()->CurrentFileAttrGet(0));
            AttribVal[1] = BinaryFile_.get()->CurrentFileAttrGet(1);
            AttribValMax[0] = TextCodec::CodecListNumber.Count;
            AttribValMax[1] = 6;
            AttribValOffset[0] = 0;
            AttribValOffset[1] = 0;
            ManagerState = ManagerStateDef::Attrib;
            RepaintDepth = 2;
            Repaint();
            break;
    }
}

void FileManager::EventKeyAttrib(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    switch (_(KeyName.c_str()))
    {
        case _("Tab"):
            Pos++;
            if (Pos == 4)
            {
                Pos = 0;
            }
            RequestRepaint = true;
            RepaintDepth = 0;
            break;
        case _("ArrowUp"):
            AttribVal[AttribParam]--;
            if (AttribVal[AttribParam] < 0)
            {
                AttribVal[AttribParam] = AttribValMax[AttribParam] - 1;
            }
            RepaintDepth = 2;
            Repaint();
            break;
        case _("ArrowDown"):
            AttribVal[AttribParam]++;
            if (AttribVal[AttribParam] >= AttribValMax[AttribParam])
            {
                AttribVal[AttribParam] = 0;
            }
            RepaintDepth = 2;
            Repaint();
            break;
        case _("PageUp"):
            AttribVal[AttribParam] -= 10;
            if (AttribVal[AttribParam] < 0)
            {
                AttribVal[AttribParam] = AttribValMax[AttribParam] - 1;
            }
            RepaintDepth = 2;
            Repaint();
            break;
        case _("PageDown"):
            AttribVal[AttribParam] += 10;
            if (AttribVal[AttribParam] >= AttribValMax[AttribParam])
            {
                AttribVal[AttribParam] = 0;
            }
            RepaintDepth = 2;
            Repaint();
            break;
        case _("ArrowLeft"):
            AttribParam--;
            if (AttribParam < 0)
            {
                AttribParam = 1;
            }
            Repaint();
            break;
        case _("ArrowRight"):
            AttribParam++;
            if (AttribParam > 1)
            {
                AttribParam = 0;
            }
            Repaint();
            break;
        case _("Escape"):
        case _("Backspace"):
        case _("Enter"):
        case _("NumpadEnter"):
        case _("Space"):
            {
                bool X0 = BinaryFile_.get()->CurrentFileAttrSet(0, TextCodec::CodecListNumber[AttribVal[0]]);
                bool X1 = BinaryFile_.get()->CurrentFileAttrSet(1, AttribVal[1]);

                if (X0 || X1)
                {
                    CF.get()->ParamSet("FileCodec", TextCodec::CodecListNumber[AttribVal[0]]);
                    CF.get()->ParamSet("FileType", AttribVal[1]);
                    BinaryFile_.get()->SaveFromStringConfig(CF.get()->FileSave(0));
                    BinaryFile_.get()->SysSaveConfig();
                }
            }

            ManagerState = ManagerStateDef::Files;
            RepaintDepth = 2;
            Repaint();
            break;
    }
}

void FileManager::EventKeyInsert(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    switch (_(KeyName.c_str()))
    {
        case _("Tab"):
            Pos++;
            if (Pos == 4)
            {
                Pos = 0;
            }
            RequestRepaint = true;
            RepaintDepth = 0;
            break;
        case _("Enter"):
        case _("NumpadEnter"):
            if (InsertFileName.Count > 0)
            {
                BinaryFile_.get()->ItemFileAdd(InsertFileName);
                BinaryFile_.get()->SetDir(Str("."));
            }
            ManagerState = ManagerStateDef::Files;
            RepaintDepth = 2;
            Repaint();
            break;
        case _("Escape"):
            ManagerState = ManagerStateDef::Files;
            RepaintDepth = 2;
            Repaint();
            break;
        case _("Backspace"):
            if (InsertFileName.Count > 0)
            {
                InsertFileName.PopLast();
            }
            RepaintDepth = 2;
            Repaint();
            break;
        default:
            switch (KeyChar)
            {
                case '<':
                case '>':
                case '\\':
                case '\"':
                case '?':
                case '*':
                case ':':
                case '|':
                    break;
                default:
                    if ((KeyChar >= 32) && (KeyChar <= 126))
                    {
                        InsertFileName.Add(KeyChar);
                    }
                    RepaintDepth = 2;
                    Repaint();
                    break;
            }
    }
}

void FileManager::EventKeyDelete(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    switch (_(KeyName.c_str()))
    {
        case _("Tab"):
            Pos++;
            if (Pos == 4)
            {
                Pos = 0;
            }
            RequestRepaint = true;
            RepaintDepth = 0;
            break;
        case _("Enter"):
        case _("NumpadEnter"):
            BinaryFile_.get()->ItemFileRemove(BinaryFile_.get()->DirItemName[BinaryFile_.get()->DirItemIdx]);
            BinaryFile_.get()->SetDir(Str("."));
            ManagerState = ManagerStateDef::Files;
            RepaintDepth = 2;
            Repaint();
            break;
        default:
            ManagerState = ManagerStateDef::Files;
            RepaintDepth = 2;
            Repaint();
            break;
    }
}

void FileManager::Open(std::shared_ptr<ConfigFile> CF_)
{
    CF = CF_;
    ManagerState = ManagerStateDef::Files;
    BinaryFile_.get()->ManagerInfoPush();
    RequestRepaint = false;
    RequestCloseOld = false;
    RequestCloseNew = false;
    RepaintDepth = 0;
    Repaint();
}
