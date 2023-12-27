#include "core3keys.h"

Core3Keys::Core3Keys()
{

}

void Core3Keys::Init()
{
    for (int I = 0; I < TextCodec::CodecListNumber.Count; I++)
    {
        TextCodec E(TextCodec::CodecListNumber[I]);
        if (E.EncodingNumber >= 0)
        {
            EncodingList.Add(E);
        }
    }
    EncodingListI = -1;
    EncodingListState = 0;

    EncodingKey1.Add('`');
    EncodingKey1.Add('~');
    EncodingKey1.Add(27);
    EncodingKey1.Add(9);

    EncodingKey2.Add(8);
    EncodingKey2.Add(13);
    EncodingKey2.Add(10);
    EncodingKey2.Add(32);
}

void Core3Keys::EventKey(std::string KeyName, int KeyChar, bool ModShift, bool ModCtrl, bool ModAlt)
{
    std::string KeyId = KeyName + "|" + std::to_string(KeyChar) + "|" + std::to_string(ModShift) + std::to_string(ModCtrl) + std::to_string(ModAlt);
    if (KeyId0 != KeyId)
    {
        KeyId0 = KeyId;
        KeyCounter = 0;
    }
    else
    {
        KeyCounter++;
        if (KeyCounter == 4)
        {
            AppExit = true;
            return;
        }
    }

    if (EncodingKey1.Contains(KeyChar))
    {
        if (EncodingListState > 0)
        {
            EncodingListState = 0;
        }
        else
        {
            EncodingListI--;
            if (EncodingListI < 0)
            {
                EncodingListI = EncodingList.Count - 1;
            }
        }
    }
    else
    {
        if (EncodingKey2.Contains(KeyChar))
        {
            if (EncodingListState == 0)
            {
                if (EncodingListI >= 0)
                {
                    if (EncodingList[EncodingListI].EncodingOneByte)
                    {
                        EncodingListState = 1;
                    }
                    else
                    {
                        EncodingListI++;
                        if (EncodingListI == EncodingList.Count)
                        {
                            EncodingListI = 0;
                        }
                    }
                }
                else
                {
                    EncodingListI++;
                    if (EncodingListI == EncodingList.Count)
                    {
                        EncodingListI = 0;
                    }
                }
            }
            else
            {
                EncodingListState += 16;
                if (EncodingListState >= 273)
                {
                    EncodingListState -= 272;
                }
            }
        }
        else
        {
            if (EncodingListState > 0)
            {
                EncodingListState++;
                if (EncodingListState == 273)
                {
                    EncodingListState = 1;
                }
            }
            else
            {
                EncodingListI++;
                if (EncodingListI == EncodingList.Count)
                {
                    EncodingListI = 0;
                }
            }
        }
    }

    Screen::ScreenTextMove(0, 1, 0, 0, ScreenW, ScreenH - 1);
    Str KeyInfoText;

    KeyInfoText.AddRange(Str::FromString(std::to_string(EncodingList[EncodingListI].EncodingNumber)).LPad(5, ' '));
    KeyInfoText.Add(':');
    KeyInfoText.Add(' ');
    if (EncodingListState == 0)
    {
        KeyInfoText.AddRange(Str::FromString(EncodingList[EncodingListI].EncodingName).RPad(17, ' '));
    }
    if ((EncodingListState >= 1) && (EncodingListState <= 16))
    {
        for (int I = 0; I < 16; I++)
        {
            KeyInfoText.Add(EncodingList[EncodingListI].MapByteToText[I + (EncodingListState * 16 - 16)]);
        }
        KeyInfoText.Add(32);
    }
    if ((EncodingListState >= 17) && (EncodingListState <= 272))
    {
        KeyInfoText.AddRange(Str::FromString(TextWork::CharCode(EncodingListState - 17, 0)));
        KeyInfoText.Add('=');
        KeyInfoText.AddRange(Str::FromString(TextWork::CharCode(EncodingList[EncodingListI].MapByteToText[EncodingListState - 17], 0)).RPad(14, ' '));
    }
    KeyInfoText.Add(34);
    KeyInfoText.AddRange(Str::FromString(KeyName));
    KeyInfoText.Add(34);
    KeyInfoText.Add(32);
    KeyInfoText.AddRange(Str::FromString(KeyChar));
    KeyInfoText.Add(32);
    KeyInfoText.AddRange(Str::FromString(TextWork::CharCode(KeyChar, 0)));
    KeyInfoText.Add('h');
    if (KeyChar >= 32)
    {
        KeyInfoText.Add(32);
        KeyInfoText.Add(39);
        KeyInfoText.Add(KeyChar);
        KeyInfoText.Add(39);
    }
    KeyInfoText.Add(32);
    if (ModShift)
    {
        KeyInfoText.AddRange(Str::FromString("[Shift]"));
    }
    if (ModCtrl)
    {
        KeyInfoText.AddRange(Str::FromString("[Ctrl]"));
    }
    if (ModAlt)
    {
        KeyInfoText.AddRange(Str::FromString("[Alt]"));
    }


    if (KeyInfoText.Count > ScreenW)
    {
        KeyInfoText.RemoveRange(ScreenW, KeyInfoText.Count - ScreenW);
    }
    if (KeyInfoText.Count < ScreenW)
    {
        KeyInfoText.AddRange(Str::Spaces(ScreenW - KeyInfoText.Count));
    }
    Screen::ScreenWriteText0(KeyInfoText, Screen::TextNormalBack, Screen::TextNormalFore);

    Screen::ScreenCursorMove(0, ScreenH - 1);
    Screen::ScreenRefresh();
}

void Core3Keys::EventOther(std::string EvtName, std::string EvtParam0, int EvtParam1, int EvtParam2, int EvtParam3, int EvtParam4)
{
    switch (_(EvtName.c_str()))
    {
        case _("Resize"):
            ScreenW = EvtParam1;
            ScreenH = EvtParam2;
            Screen::ScreenResize(ScreenW, ScreenH);
            Screen::ScreenClear(Screen::TextNormalBack, Screen::TextNormalFore);
            Screen::ScreenCursorMove(0, ScreenH - 1);
            Screen::ScreenRefresh();
            break;
    }
}
