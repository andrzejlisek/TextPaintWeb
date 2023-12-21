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

    int DispLen = SizeH - 3;

    if (BinaryFile_.get()->ListDispOffset < (BinaryFile_.get()->ListIndex - DispLen + 1))
    {
        BinaryFile_.get()->ListDispOffset = (BinaryFile_.get()->ListIndex - DispLen + 1);
        RepaintDepth = 2;
    }

    if (BinaryFile_.get()->ListDispOffset > (BinaryFile_.get()->ListIndex))
    {
        BinaryFile_.get()->ListDispOffset = (BinaryFile_.get()->ListIndex);
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
            if ((Idx + BinaryFile_.get()->ListDispOffset) == (BinaryFile_.get()->ListIndex))
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

    Screen::ScreenCursorMove(PosX + 1, PosY + 2 + Sel);
    Screen::ScreenRefresh();
}

void FileManager::EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    RequestRepaint = false;
    RequestClose = false;
    switch (_(KeyName.c_str()))
    {
        case _("ArrowUp"):
            if (BinaryFile_.get()->ListIndex > 0)
            {
                BinaryFile_.get()->ListIndex--;
            }
            else
            {
                BinaryFile_.get()->ListIndex = (BinaryFile_.get()->ItemCount() - 1);
            }
            RepaintDepth = 3;
            Repaint();
            break;
        case _("ArrowDown"):
            if (BinaryFile_.get()->ListIndex < (BinaryFile_.get()->ItemCount() - 1))
            {
                BinaryFile_.get()->ListIndex++;
            }
            else
            {
                BinaryFile_.get()->ListIndex = 0;
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
            RequestRepaint = true;
            RequestClose = true;
            break;
        case _("Enter"):
            if (BinaryFile_.get()->ItemType(BinaryFile_.get()->ListIndex) >= 0)
            {
                BinaryFile_.get()->ListIndex = BinaryFile_.get()->ListIndex;
                RequestRepaint = true;
                RequestClose = true;
            }
            else
            {
                BinaryFile_.get()->SetDir(BinaryFile_.get()->ItemName(BinaryFile_.get()->ListIndex));
                RepaintDepth = 2;
                Repaint();
            }
            break;
    }
}

void FileManager::Open()
{
    BinaryFile_.get()->ListDispOffset = BinaryFile_.get()->ListDispOffset;
    RequestRepaint = false;
    RequestClose = false;
    RepaintDepth = 0;
    Repaint();
}
