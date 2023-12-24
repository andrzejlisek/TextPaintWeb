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

    int DispLen = SizeH - MarginHead - MarginFoot;

    if (BinaryFile_.get()->ListDispOffset < (BinaryFile_.get()->ItemIndex - DispLen + 1))
    {
        BinaryFile_.get()->ListDispOffset = (BinaryFile_.get()->ItemIndex - DispLen + 1);
        RepaintDepth = 2;
    }

    if (BinaryFile_.get()->ListDispOffset > (BinaryFile_.get()->ItemIndex))
    {
        BinaryFile_.get()->ListDispOffset = (BinaryFile_.get()->ItemIndex);
        RepaintDepth = 2;
    }


    // File list
    if (RepaintDepth <= 2)
    {
        for (int YY = 1; YY < (SizeH + 1); YY++)
        {
            for (int XX = 1; XX < (SizeW + 1); XX++)
            {
                Screen::ScreenChar(PosX + XX, PosY + YY, 32, PopupBack, PopupFore, 0, 0, 0);
            }
        }

        int DirL = std::min(BinaryFile_.get()->ListDir.Count, SizeW);
        int DirO = BinaryFile_.get()->ListDir.Count - DirL;
        for (int I = 0; I < DirL; I++)
        {
            Screen::ScreenChar(PosX + I + 1, PosY + 1, BinaryFile_.get()->ListDir[I + DirO], PopupBack, PopupFore, 0, 0, 0);
        }

        for (int Idx = BinaryFile_.get()->ListDispOffset; Idx < std::min(BinaryFile_.get()->ItemCount(), DispLen + BinaryFile_.get()->ListDispOffset); Idx++)
        {
            int ItemL = std::min(BinaryFile_.get()->ItemName(Idx).Count, SizeW - 1);
            int ItemO = BinaryFile_.get()->ItemName(Idx).Count - ItemL;
            int PopupBack_ = PopupBack;
            int PopupFore_ = PopupFore;
            if (BinaryFile_.get()->ItemType(Idx) < 0)
            {
                PopupBack_ = PopupFore;
                PopupFore_ = PopupBack;
            }
            int ItemY = PosY + 1 + Idx + 1 - BinaryFile_.get()->ListDispOffset;
            for (int I = 0; I < ItemL; I++)
            {
                Screen::ScreenChar(PosX + I + 2, ItemY, BinaryFile_.get()->ItemName(Idx)[I + ItemO], PopupBack_, PopupFore_, 0, 0, 0);
            }
            if (ItemO > 0)
            {
                Screen::ScreenChar(PosX + 0 + 2, ItemY, '.', PopupBack_, PopupFore_, 0, 0, 0);
                Screen::ScreenChar(PosX + 1 + 2, ItemY, '.', PopupBack_, PopupFore_, 0, 0, 0);
                Screen::ScreenChar(PosX + 2 + 2, ItemY, '.', PopupBack_, PopupFore_, 0, 0, 0);
            }
        }
    }

    // Select cursor
    int Sel = 0;
    if (RepaintDepth <= 3)
    {
        for (int Idx = 0; Idx < DispLen; Idx++)
        {
            if ((Idx + BinaryFile_.get()->ListDispOffset) == (BinaryFile_.get()->ItemIndex))
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

    // File info and keyboard shortcuts
    Str Info1;
    Str Info2;
    Str Info3;
    int ItemIdx = BinaryFile_.get()->ItemIndex;
    if (BinaryFile_.get()->ItemType(ItemIdx) >= 0)
    {
        Info1.AddString("Ansi: ");
        Info1.AddString(BinaryFile_.get()->ItemGet(ItemIdx).Ansi ? "Y" : "N");
        Info1.AddString("  Codec: ");
        Info1.AddString(BinaryFile_.get()->ItemGet(ItemIdx).Codec.get()->EncodingNumber);
        Info1.AddString("/");
        Info1.AddString(BinaryFile_.get()->ItemGet(ItemIdx).Codec.get()->EncodingName);
    }
    else
    {
        Info1.AddString("Directory");
    }
    Info2.AddString("Ins - New file  Home - Upload");
    Info3.AddString("Del - Remove    End - Download");
    Info1.AddPad(SizeW, 32);
    Info2.AddPad(SizeW, 32);
    Info3.AddPad(SizeW, 32);
    for (int I = 0; I < SizeW; I++)
    {
        Screen::ScreenChar(PosX + 1 + I, PosY + 1 + SizeH - MarginFoot, Info1[I], PopupBack, PopupFore, 0, 0, 0);
        Screen::ScreenChar(PosX + 1 + I, PosY + 2 + SizeH - MarginFoot, Info2[I], PopupBack, PopupFore, 0, 0, 0);
        Screen::ScreenChar(PosX + 1 + I, PosY + 3 + SizeH - MarginFoot, Info3[I], PopupBack, PopupFore, 0, 0, 0);
    }

    Screen::ScreenCursorMove(PosX + 1, PosY + 2 + Sel);
    Screen::ScreenRefresh();
}

void FileManager::EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    RequestRepaint = false;
    RequestCloseOld = false;
    RequestCloseNew = false;
    switch (_(KeyName.c_str()))
    {
        case _("ArrowUp"):
            if (BinaryFile_.get()->ItemIndex > 0)
            {
                BinaryFile_.get()->ItemIndex--;
            }
            else
            {
                BinaryFile_.get()->ItemIndex = (BinaryFile_.get()->ItemCount() - 1);
            }
            RepaintDepth = 3;
            Repaint();
            break;
        case _("ArrowDown"):
            if (BinaryFile_.get()->ItemIndex < (BinaryFile_.get()->ItemCount() - 1))
            {
                BinaryFile_.get()->ItemIndex++;
            }
            else
            {
                BinaryFile_.get()->ItemIndex = 0;
            }
            RepaintDepth = 3;
            Repaint();
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
            BinaryFile_.get()->ManagerInfoPop();
            RequestRepaint = true;
            RequestCloseOld = true;
            break;
        case _("Enter"):
        case _("NumpadEnter"):
            if (BinaryFile_.get()->ItemType(BinaryFile_.get()->ItemIndex) >= 0)
            {
                RequestRepaint = true;
                RequestCloseNew = true;
            }
            else
            {
                BinaryFile_.get()->SetDir(BinaryFile_.get()->ItemName(BinaryFile_.get()->ItemIndex));
                RepaintDepth = 2;
                Repaint();
            }
            break;
    }
}

void FileManager::Open()
{
    BinaryFile_.get()->ManagerInfoPush();
    RequestRepaint = false;
    RequestCloseOld = false;
    RequestCloseNew = false;
    RepaintDepth = 0;
    Repaint();
}
