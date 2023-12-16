#include "editorclipboard.h"

EditorClipboard::EditorClipboard()
{

}

void EditorClipboard::SysClipboardSet()
{
    std::stringstream SysClpText;
    for (int i = 0; i < TextClipboard.CountLines(); i++)
    {
        Str Temp = TextClipboard.GetLineString(i);
        TextCodec::Transcode(Temp, TextCodec::UTF8, 0);
        SysClpText << Temp.ToString() << std::endl;
    }
    SysClpTextCopy = SysClpText.str();
}

bool EditorClipboard::SysClipboardGet()
{
    if ((SysClpTextPaste.size() > 0) && (SysClpTextPaste != SysClpTextCopy))
    {
        XList<std::string> SysClpText;
        TextWork::StringToLines(SysClpTextPaste, SysClpText);
        TextClipboard.Clear();
        for (int i = 0; i < SysClpText.Count; i++)
        {
            TextClipboard.AppendLine();
            Str Temp = Str::FromString(SysClpText[i]);
            TextCodec::Transcode(Temp, 0, TextCodec::UTF8);
            TextClipboard.SetLineString(i, Temp);
        }
    }
    return (SysClpTextPaste.size() > 0);
}

void EditorClipboard::TextClipboardWork(int X, int Y, int W, int H, int FontW, int FontH, bool Paste, int DiamondType)
{
    int W_, H_, TX, TY;
    if (DiamondType > 0)
    {
        if (W < 0)
        {
            TextClipboardWork(X + W, Y + W, 0 - W, H, FontW, FontH, Paste, DiamondType);
            return;
        }
        if (H < 0)
        {
            TextClipboardWork(X - H, Y + H, W, 0 - H, FontW, FontH, Paste, DiamondType);
            return;
        }

        X = X - (H * FontW);
        W_ = W + H;
        H_ = W + H;

        TX = H;
        TY = W;

        if ((DiamondType == 2) || (DiamondType == 6) || (DiamondType == 7))
        {
            W_++;
        }
        if ((DiamondType == 3) || (DiamondType == 7) || (DiamondType == 8))
        {
            H_++;
        }
        if ((DiamondType == 4) || (DiamondType == 8) || (DiamondType == 9))
        {
            X--;
            W_++;
        }
        if ((DiamondType == 5) || (DiamondType == 9) || (DiamondType == 6))
        {
            Y--;
            H_++;
        }
    }
    else
    {
        if (W < 0)
        {
            TextClipboardWork(X + W, Y, 0 - W, H, FontW, FontH, Paste, DiamondType);
            return;
        }
        if (H < 0)
        {
            TextClipboardWork(X, Y + H, W, 0 - H, FontW, FontH, Paste, DiamondType);
            return;
        }
        W_ = W;
        H_ = H;
        TX = 0;
        TY = 0;
    }

    bool PreserveFont = true;
    if ((FontW > 1) || (FontH > 1))
    {
        PreserveFont = false;
    }

    if (Paste)
    {
        if (SysClipboardGet() > 0)
        {
            for (int YY = 0; YY <= H_; YY++)
            {
                for (int XX = 0; XX <= W_; XX++)
                {
                    if (IsInsideSelection(W_, H_, XX, YY, TX, TY, DiamondType))
                    {
                        if ((TextClipboard.CountLines() > YY) && (TextClipboard.CountItems(YY) > XX))
                        {
                            TextClipboard.Get_(YY, XX);
                            if (!PreserveFont)
                            {
                                TextClipboard.Item_FontW = 0;
                                TextClipboard.Item_FontH = 0;
                            }
                            EditorData_.get()->CharPut(X + (XX * FontW), Y + (YY * FontH), TextClipboard, true);
                        }
                    }
                }
            }
        }
    }
    else
    {
        TextClipboard.Clear();
        for (int YY = 0; YY <= H_; YY++)
        {
            TextClipboard.AppendLine();
            for (int XX = 0; XX <= W_; XX++)
            {
                TextClipboard.BlankChar();
                if (IsInsideSelection(W_, H_, XX, YY, TX, TY, DiamondType))
                {
                    TextClipboard.Item_Char = EditorData_.get()->ElementGetVal(X + (XX * FontW), Y + (YY * FontH), true, false, 0);
                    TextClipboard.Item_ColorB = EditorData_.get()->ElementGetVal(X + (XX * FontW), Y + (YY * FontH), true, false, 1);
                    TextClipboard.Item_ColorF = EditorData_.get()->ElementGetVal(X + (XX * FontW), Y + (YY * FontH), true, false, 2);
                    TextClipboard.Item_ColorA = EditorData_.get()->ElementGetVal(X + (XX * FontW), Y + (YY * FontH), true, false, 3);
                    if (PreserveFont)
                    {
                        TextClipboard.Item_FontW = EditorData_.get()->ElementGetVal(X + (XX * FontW), Y + (YY * FontH), true, false, 4);
                        TextClipboard.Item_FontH = EditorData_.get()->ElementGetVal(X + (XX * FontW), Y + (YY * FontH), true, false, 5);
                    }
                }
                TextClipboard.Append(YY);
            }
        }
        SysClipboardSet();
    }
}

bool EditorClipboard::IsInsideSelection(int W, int H, int XX, int YY, int TX, int TY, int DiamondType)
{
    if (DiamondType > 0)
    {
        // Top left edge
        int D = 0;
        if ((XX + YY + D - TX) < 0)
        {
            return false;
        }

        // Top right edge
        D = 0;
        if ((DiamondType == 2) || (DiamondType == 4) || (DiamondType >= 6))
        {
            D = 1;
        }
        if ((XX - YY - D - TX) > 0)
        {
            return false;
        }

        // Bottom left edge
        D = 0;
        if ((DiamondType == 3) || (DiamondType == 5) || (DiamondType >= 6))
        {
            D = 1;
        }
        if ((YY - XX - D - TX) > 0)
        {
            return false;
        }

        // Bottom right edge
        D = 0;
        if ((DiamondType == 3) || (DiamondType == 5) || (DiamondType >= 6))
        {
            D = 1;
        }
        if ((YY - W + XX - D - TY) > 0)
        {
            return false;
        }
    }
    return true;
}
