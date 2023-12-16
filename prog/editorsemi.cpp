#include "editorsemi.h"

EditorSemi::EditorSemi()
{

}

void EditorSemi::Init(std::shared_ptr<ConfigFile> CF)
{
    /*
        0 -
        1 |

        2---3---4
        |   |   |
        5---6---7
        |   |   |
        8---9---10

        11 \
        12 /

              13
             /  \
           16    14
          /  \  /  \
        19    17    15
          \  /  \  /
           20    18
             \  /
              21
     */

    Frame1N.Add("Char");
    Frame2N.Add("Char");
    Frame1C.Add(XList<int>());
    Frame2C.Add(XList<int>());
    for (int i = 0; i < 11; i++)
    {
        Frame1C[0].Add(32);
        Frame2C[0].Add(32);
    }

    for (int i = 0; i >= 0; i++)
    {
        std::string Buf[20];
        int BufL = TextWork::StringSplit(CF.get()->ParamGetS("Frame1_" + std::to_string(i)), ',', Buf);
        if (BufL == 12)
        {
            Frame1N.Add(Buf[0]);
            XList<int> Temp;
            for (int ii = 0; ii < 11; ii++)
            {
                Temp.Add(TextWork::CodeChar(Buf[ii + 1]));
            }
            Frame1C.Add(Temp);
        }
        else
        {
            i = -2;
        }
    }
    for (int i = 0; i >= 0; i++)
    {
        std::string Buf[20];
        int BufL = TextWork::StringSplit(CF.get()->ParamGetS("Frame2_" + std::to_string(i)), ',', Buf);
        if (BufL == 12)
        {
            Frame2N.Add(Buf[0]);
            XList<int> Temp;
            for (int ii = 0; ii < 11; ii++)
            {
                Temp.Add(TextWork::CodeChar(Buf[ii + 1]));
            }
            Frame2C.Add(Temp);
        }
        else
        {
            i = -2;
        }
    }

    SetFrame(0, 0);
}

std::string EditorSemi::GetFrameName(int F)
{
    if (F == 1)
    {
        if (Frame1I == 0)
        {
            return TextWork::CharCode(EditorData_.get()->DrawCharI, 1) + " " + TextWork::CharToStr(EditorData_.get()->DrawCharI);
        }
        else
        {
            return Frame1N[Frame1I];
        }
    }
    if (F == 2)
    {
        if (Frame2I == 0)
        {
            return TextWork::CharCode(EditorData_.get()->DrawCharI, 1) + " " + TextWork::CharToStr(EditorData_.get()->DrawCharI);
        }
        else
        {
            return Frame2N[Frame2I];
        }
    }
    return "";
}

void EditorSemi::SetFrame(int F1, int F2)
{
    Frame1I = F1;
    Frame2I = F2;
    for (int i = 0; i < 11; i++)
    {
        FrameChar[i] = Frame1C[F1][i];
        FrameChar[i + 11] = Frame2C[F2][i];
    }
}

void EditorSemi::SetFrameNext(int N)
{
    if (N == 1)
    {
        Frame1I++;
        if (Frame1I >= Frame1C.Count)
        {
            Frame1I = 0;
        }
    }
    if (N == 2)
    {
        Frame2I++;
        if (Frame2I >= Frame2C.Count)
        {
            Frame2I = 0;
        }
    }
    std::cout << Frame1C.Count << "____" << Frame2C.Count << std::endl;
    SetFrame(Frame1I, Frame2I);
}



int EditorSemi::CharGet(int X, int Y)
{
    return EditorData_.get()->CharGet(CursorX + X, CursorY + Y, true, false);
}

void EditorSemi::CharPut(int X, int Y, int C)
{
    AnsiLineOccupyItem Item;
    Item.BlankChar();
    Item.Item_Char = C;
    Item.Item_ColorB = EditorData_.get()->DrawColoBI;
    Item.Item_ColorF = EditorData_.get()->DrawColoFI;
    Item.Item_ColorA = EditorData_.get()->DrawColoAI;
    EditorData_.get()->CharPut(CursorX + X, CursorY + Y, Item, false);
}

void EditorSemi::CharPutDbl(int X, int Y, int C, int Offset)
{
    AnsiLineOccupyItem Item;
    Item.BlankChar();
    Item.Item_Char = C;
    Item.Item_ColorB = EditorData_.get()->DrawColoBI;
    Item.Item_ColorF = EditorData_.get()->DrawColoFI;
    Item.Item_ColorA = EditorData_.get()->DrawColoAI;
    EditorData_.get()->CharPut(CursorX + X, CursorY + Y, Item, false);

    int C_ = 0;//!!!!!!!!!!!!!!!!!!!!Screen_.CharDouble(C);
    if (C_ != 0)
    {
        AnsiLineOccupyItem Item_;
        Item_.BlankChar();
        Item_.Item_Char = C_;
        Item_.Item_ColorB = EditorData_.get()->DrawColoBI;
        Item_.Item_ColorF = EditorData_.get()->DrawColoFI;
        Item_.Item_ColorA = EditorData_.get()->DrawColoAI;
        EditorData_.get()->CharPut(CursorX + X + Offset, CursorY + Y, Item_, false);
    }
}

void EditorSemi::FrameCharPut(int Dir, int FontW, int FontH, bool CharDbl)
{
    int FontW_ = CharDbl ? (FontW + FontW) : FontW;
    int CharSide = 0;

    XList<int> CharT( 7, FrameChar[1],  FrameChar[2],  FrameChar[3],  FrameChar[4],  FrameChar[5],  FrameChar[6],  FrameChar[7]  );
    XList<int> CharB( 7, FrameChar[1],  FrameChar[5],  FrameChar[6],  FrameChar[7],  FrameChar[8],  FrameChar[9],  FrameChar[10] );
    XList<int> CharL( 7, FrameChar[0],  FrameChar[2],  FrameChar[3],  FrameChar[5],  FrameChar[6],  FrameChar[8],  FrameChar[9]  );
    XList<int> CharR( 7, FrameChar[0],  FrameChar[3],  FrameChar[4],  FrameChar[6],  FrameChar[7],  FrameChar[9],  FrameChar[10] );

    XList<int> CharTR(7, FrameChar[12], FrameChar[13], FrameChar[14], FrameChar[15], FrameChar[16], FrameChar[17], FrameChar[18] );
    XList<int> CharBL(7, FrameChar[12], FrameChar[16], FrameChar[17], FrameChar[18], FrameChar[19], FrameChar[20], FrameChar[21] );
    XList<int> CharTL(7, FrameChar[11], FrameChar[13], FrameChar[14], FrameChar[16], FrameChar[17], FrameChar[19], FrameChar[20] );
    XList<int> CharBR(7, FrameChar[11], FrameChar[14], FrameChar[15], FrameChar[17], FrameChar[18], FrameChar[20], FrameChar[21] );

    switch (Dir)
    {
        case 0:
            {
                CharSide += (CharB.Contains(CharGet( 0, FontH)) ? 1 : 0);
                CharSide += (CharL.Contains(CharGet(-FontW_,  0)) ? 2 : 0);
                CharSide += (CharR.Contains(CharGet(FontW_,  0)) ? 4 : 0);
                switch (CharSide)
                {
                    case 0: CharPutDbl(0, 0, FrameChar[1], FontW);  break;
                    case 1: CharPutDbl(0, 0, FrameChar[1], FontW);  break;
                    case 2: CharPutDbl(0, 0, FrameChar[10], FontW); break;
                    case 3: CharPutDbl(0, 0, FrameChar[7], FontW);  break;
                    case 4: CharPutDbl(0, 0, FrameChar[8], FontW);  break;
                    case 5: CharPutDbl(0, 0, FrameChar[5], FontW);  break;
                    case 6: CharPutDbl(0, 0, FrameChar[9], FontW);  break;
                    case 7: CharPutDbl(0, 0, FrameChar[6], FontW);  break;
                }
            }
            break;
        case 1:
            {
                CharSide += (CharT.Contains(CharGet( 0, -FontH)) ? 1 : 0);
                CharSide += (CharR.Contains(CharGet(FontW_,  0)) ? 2 : 0);
                CharSide += (CharL.Contains(CharGet(-FontW_,  0)) ? 4 : 0);
                switch (CharSide)
                {
                    case 0: CharPutDbl(0, 0, FrameChar[1], FontW);  break;
                    case 1: CharPutDbl(0, 0, FrameChar[1], FontW);  break;
                    case 2: CharPutDbl(0, 0, FrameChar[2], FontW);  break;
                    case 3: CharPutDbl(0, 0, FrameChar[5], FontW);  break;
                    case 4: CharPutDbl(0, 0, FrameChar[4], FontW);  break;
                    case 5: CharPutDbl(0, 0, FrameChar[7], FontW);  break;
                    case 6: CharPutDbl(0, 0, FrameChar[3], FontW);  break;
                    case 7: CharPutDbl(0, 0, FrameChar[6], FontW);  break;
                }
            }
            break;
        case 2:
            {
                CharSide += (CharR.Contains(CharGet(FontW_,  0)) ? 1 : 0);
                CharSide += (CharB.Contains(CharGet( 0, FontH)) ? 2 : 0);
                CharSide += (CharT.Contains(CharGet( 0, -FontH)) ? 4 : 0);
                switch (CharSide)
                {
                    case 0: CharPutDbl(0, 0, FrameChar[0], FontW);  break;
                    case 1: CharPutDbl(0, 0, FrameChar[0], FontW);  break;
                    case 2: CharPutDbl(0, 0, FrameChar[4], FontW);  break;
                    case 3: CharPutDbl(0, 0, FrameChar[3], FontW);  break;
                    case 4: CharPutDbl(0, 0, FrameChar[10], FontW); break;
                    case 5: CharPutDbl(0, 0, FrameChar[9], FontW);  break;
                    case 6: CharPutDbl(0, 0, FrameChar[7], FontW);  break;
                    case 7: CharPutDbl(0, 0, FrameChar[6], FontW);  break;
                }
            }
            break;
        case 3:
            {
                CharSide += (CharL.Contains(CharGet(-FontW_,  0)) ? 1 : 0);
                CharSide += (CharT.Contains(CharGet( 0, -FontH)) ? 2 : 0);
                CharSide += (CharB.Contains(CharGet( 0, FontH)) ? 4 : 0);
                switch (CharSide)
                {
                    case 0: CharPutDbl(0, 0, FrameChar[0], FontW);  break;
                    case 1: CharPutDbl(0, 0, FrameChar[0], FontW);  break;
                    case 2: CharPutDbl(0, 0, FrameChar[8], FontW);  break;
                    case 3: CharPutDbl(0, 0, FrameChar[9], FontW);  break;
                    case 4: CharPutDbl(0, 0, FrameChar[2], FontW);  break;
                    case 5: CharPutDbl(0, 0, FrameChar[3], FontW);  break;
                    case 6: CharPutDbl(0, 0, FrameChar[5], FontW);  break;
                    case 7: CharPutDbl(0, 0, FrameChar[6], FontW);  break;
                }
            }
            break;
        case 4:
            {
                CharSide += (CharBL.Contains(CharGet(-FontW_, FontH)) ? 1 : 0);
                CharSide += (CharTL.Contains(CharGet(-FontW_, -FontH)) ? 2 : 0);
                CharSide += (CharBR.Contains(CharGet(FontW_, FontH)) ? 4 : 0);
                switch (CharSide)
                {
                    case 0: CharPutDbl(0, 0, FrameChar[12], FontW); break;
                    case 1: CharPutDbl(0, 0, FrameChar[12], FontW); break;
                    case 2: CharPutDbl(0, 0, FrameChar[21], FontW); break;
                    case 3: CharPutDbl(0, 0, FrameChar[18], FontW); break;
                    case 4: CharPutDbl(0, 0, FrameChar[19], FontW); break;
                    case 5: CharPutDbl(0, 0, FrameChar[16], FontW); break;
                    case 6: CharPutDbl(0, 0, FrameChar[20], FontW); break;
                    case 7: CharPutDbl(0, 0, FrameChar[17], FontW); break;
                }
            }
            break;
        case 5:
            {
                CharSide += (CharTR.Contains(CharGet(FontW_, -FontH)) ? 1 : 0);
                CharSide += (CharBR.Contains(CharGet(FontW_, FontH)) ? 2 : 0);
                CharSide += (CharTL.Contains(CharGet(-FontW_, -FontH)) ? 4 : 0);
                switch (CharSide)
                {
                    case 0: CharPutDbl(0, 0, FrameChar[12], FontW); break;
                    case 1: CharPutDbl(0, 0, FrameChar[12], FontW); break;
                    case 2: CharPutDbl(0, 0, FrameChar[13], FontW); break;
                    case 3: CharPutDbl(0, 0, FrameChar[16], FontW); break;
                    case 4: CharPutDbl(0, 0, FrameChar[15], FontW); break;
                    case 5: CharPutDbl(0, 0, FrameChar[18], FontW); break;
                    case 6: CharPutDbl(0, 0, FrameChar[14], FontW); break;
                    case 7: CharPutDbl(0, 0, FrameChar[17], FontW); break;
                }
            }
            break;
        case 6:
            {
                CharSide += (CharTL.Contains(CharGet(FontW_, FontH)) ? 1 : 0);
                CharSide += (CharTR.Contains(CharGet(-FontW_, FontH)) ? 2 : 0);
                CharSide += (CharBL.Contains(CharGet(FontW_, -FontH)) ? 4 : 0);
                switch (CharSide)
                {
                    case 0: CharPutDbl(0, 0, FrameChar[11], FontW); break;
                    case 1: CharPutDbl(0, 0, FrameChar[11], FontW); break;
                    case 2: CharPutDbl(0, 0, FrameChar[15], FontW); break;
                    case 3: CharPutDbl(0, 0, FrameChar[14], FontW); break;
                    case 4: CharPutDbl(0, 0, FrameChar[21], FontW); break;
                    case 5: CharPutDbl(0, 0, FrameChar[20], FontW); break;
                    case 6: CharPutDbl(0, 0, FrameChar[18], FontW); break;
                    case 7: CharPutDbl(0, 0, FrameChar[17], FontW); break;
                }
            }
            break;
        case 7:
            {
                CharSide += (CharTL.Contains(CharGet(-FontW_, -FontH)) ? 1 : 0);
                CharSide += (CharTR.Contains(CharGet(FontW_, -FontH)) ? 2 : 0);
                CharSide += (CharBL.Contains(CharGet(-FontW_, FontH)) ? 4 : 0);
                switch (CharSide)
                {
                    case 0: CharPutDbl(0, 0, FrameChar[11], FontW); break;
                    case 1: CharPutDbl(0, 0, FrameChar[11], FontW); break;
                    case 2: CharPutDbl(0, 0, FrameChar[19], FontW); break;
                    case 3: CharPutDbl(0, 0, FrameChar[20], FontW); break;
                    case 4: CharPutDbl(0, 0, FrameChar[13], FontW); break;
                    case 5: CharPutDbl(0, 0, FrameChar[14], FontW); break;
                    case 6: CharPutDbl(0, 0, FrameChar[16], FontW); break;
                    case 7: CharPutDbl(0, 0, FrameChar[17], FontW); break;
                }
            }
            break;
    }
}

bool EditorSemi::DoubleDrawMode()
{
    return false;
    if (Frame1I == 0)
    {
        //!!!!!!!!!!!!!!!!!if (DrawCharIdbl != 0)
        {
            return true;
        }
    }
    else
    {
        if (DiamondType == 0)
        {
            for (int i = 0; i <= 10; i++)
            {
                //!!!!!!!!!!!!if (Core_.Screen_.CharDouble(FrameChar[i]) != 0)
                {
                    return true;
                }
            }
        }
        else
        {
            for (int i = 11; i <= 21; i++)
            {
                //!!!!!!!!!!!if (Core_.Screen_.CharDouble(FrameChar[i]) != 0)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void EditorSemi::FrameCharPut1(int X, int Y, bool ForceT, bool ForceB, bool ForceL, bool ForceR, int FontW, int FontH, bool DoubleChar)
{
    int CharSide = 0;
    XList<int> CharT(7, FrameChar[1],  FrameChar[2],  FrameChar[3],  FrameChar[4],  FrameChar[5],  FrameChar[6],  FrameChar[7]  );
    XList<int> CharB(7, FrameChar[1],  FrameChar[5],  FrameChar[6],  FrameChar[7],  FrameChar[8],  FrameChar[9],  FrameChar[10] );
    XList<int> CharL(7, FrameChar[0],  FrameChar[2],  FrameChar[3],  FrameChar[5],  FrameChar[6],  FrameChar[8],  FrameChar[9]  );
    XList<int> CharR(7, FrameChar[0],  FrameChar[3],  FrameChar[4],  FrameChar[6],  FrameChar[7],  FrameChar[9],  FrameChar[10] );
    CharSide += ((ForceT || CharT.Contains(CharGet(X + 0, Y - FontH))) ? 1 : 0);
    CharSide += ((ForceB || CharB.Contains(CharGet(X + 0, Y + FontH))) ? 2 : 0);
    if (DoubleChar)
    {
        CharSide += ((ForceL || CharL.Contains(CharGet(X - FontW - FontW, Y + 0))) ? 4 : 0);
        CharSide += ((ForceR || CharR.Contains(CharGet(X + FontW + FontW, Y + 0))) ? 8 : 0);
    }
    else
    {
        CharSide += ((ForceL || CharL.Contains(CharGet(X - FontW, Y + 0))) ? 4 : 0);
        CharSide += ((ForceR || CharR.Contains(CharGet(X + FontW, Y + 0))) ? 8 : 0);
    }
    switch (CharSide)
    {
        case 0:  CharPutDbl(X, Y, FrameChar[6], FontW); break;
        case 1:  CharPutDbl(X, Y, FrameChar[1], FontW); break;
        case 2:  CharPutDbl(X, Y, FrameChar[1], FontW); break;
        case 3:  CharPutDbl(X, Y, FrameChar[1], FontW); break;
        case 4:  CharPutDbl(X, Y, FrameChar[0], FontW); break;
        case 5:  CharPutDbl(X, Y, FrameChar[10], FontW); break;
        case 6:  CharPutDbl(X, Y, FrameChar[4], FontW); break;
        case 7:  CharPutDbl(X, Y, FrameChar[7], FontW); break;
        case 8:  CharPutDbl(X, Y, FrameChar[0], FontW); break;
        case 9:  CharPutDbl(X, Y, FrameChar[8], FontW); break;
        case 10: CharPutDbl(X, Y, FrameChar[2], FontW); break;
        case 11: CharPutDbl(X, Y, FrameChar[5], FontW); break;
        case 12: CharPutDbl(X, Y, FrameChar[0], FontW); break;
        case 13: CharPutDbl(X, Y, FrameChar[9], FontW); break;
        case 14: CharPutDbl(X, Y, FrameChar[3], FontW); break;
        case 15: CharPutDbl(X, Y, FrameChar[6], FontW); break;
    }
}

void EditorSemi::FrameCharPut2(int X, int Y, bool ForceTR, bool ForceBL, bool ForceTL, bool ForceBR, int FontW, int FontH, bool DoubleChar)
{
    int CharSide = 0;
    XList<int> CharTR(7, FrameChar[12], FrameChar[13], FrameChar[14], FrameChar[15], FrameChar[16], FrameChar[17], FrameChar[18] );
    XList<int> CharBL(7, FrameChar[12], FrameChar[16], FrameChar[17], FrameChar[18], FrameChar[19], FrameChar[20], FrameChar[21] );
    XList<int> CharTL(7, FrameChar[11], FrameChar[13], FrameChar[14], FrameChar[16], FrameChar[17], FrameChar[19], FrameChar[20] );
    XList<int> CharBR(7, FrameChar[11], FrameChar[14], FrameChar[15], FrameChar[17], FrameChar[18], FrameChar[20], FrameChar[21] );
    if (DoubleChar)
    {
        CharSide += ((ForceTR || CharTR.Contains(CharGet(X + FontW + FontW, Y - FontH))) ? 1 : 0);
        CharSide += ((ForceBL || CharBL.Contains(CharGet(X - FontW - FontW, Y + FontH))) ? 2 : 0);
        CharSide += ((ForceTL || CharTL.Contains(CharGet(X - FontW - FontW, Y - FontH))) ? 4 : 0);
        CharSide += ((ForceBR || CharBR.Contains(CharGet(X + FontW + FontW, Y + FontH))) ? 8 : 0);
    }
    else
    {
        CharSide += ((ForceTR || CharTR.Contains(CharGet(X + FontW, Y - FontH))) ? 1 : 0);
        CharSide += ((ForceBL || CharBL.Contains(CharGet(X - FontW, Y + FontH))) ? 2 : 0);
        CharSide += ((ForceTL || CharTL.Contains(CharGet(X - FontW, Y - FontH))) ? 4 : 0);
        CharSide += ((ForceBR || CharBR.Contains(CharGet(X + FontW, Y + FontH))) ? 8 : 0);
    }
    switch (CharSide)
    {
        case 0:  CharPutDbl(X, Y, FrameChar[17], FontW); break;
        case 1:  CharPutDbl(X, Y, FrameChar[12], FontW); break;
        case 2:  CharPutDbl(X, Y, FrameChar[12], FontW); break;
        case 3:  CharPutDbl(X, Y, FrameChar[12], FontW); break;
        case 4:  CharPutDbl(X, Y, FrameChar[11], FontW); break;
        case 5:  CharPutDbl(X, Y, FrameChar[21], FontW); break;
        case 6:  CharPutDbl(X, Y, FrameChar[15], FontW); break;
        case 7:  CharPutDbl(X, Y, FrameChar[18], FontW); break;
        case 8:  CharPutDbl(X, Y, FrameChar[11], FontW); break;
        case 9:  CharPutDbl(X, Y, FrameChar[19], FontW); break;
        case 10: CharPutDbl(X, Y, FrameChar[13], FontW); break;
        case 11: CharPutDbl(X, Y, FrameChar[16], FontW); break;
        case 12: CharPutDbl(X, Y, FrameChar[11], FontW); break;
        case 13: CharPutDbl(X, Y, FrameChar[20], FontW); break;
        case 14: CharPutDbl(X, Y, FrameChar[14], FontW); break;
        case 15: CharPutDbl(X, Y, FrameChar[17], FontW); break;
    }
}

void EditorSemi::RectangleDraw(int X, int Y, int W__, int H__, int T, int FontW, int FontH)
{
    int W = W__ * FontW;
    int H = H__ * FontH;
    if (W < 0)
    {
        RectangleDraw(X + W, Y, 0 - W__, H__, T, FontW, FontH);
        return;
    }
    if (H < 0)
    {
        RectangleDraw(X, Y + H, W__, 0 - H__, T, FontW, FontH);
        return;
    }

    bool DoubleCharMode = DoubleDrawMode();
    if (DoubleCharMode)
    {
        if ((W % (FontW << 1)) == 0)
        {
            W -= FontW;
        }
    }
    if (W <= 0)
    {
        return;
    }

    if (T == 1)
    {
        if (Frame1I == 0)
        {
            if (DoubleCharMode)
            {
                for (int i = 0; i < W; i += (FontW << 1))
                {
                    CharPut(X + i, Y + 0, EditorData_.get()->DrawCharI);
                    CharPut(X + i + FontW, Y + 0, EditorData_.get()->DrawCharIdbl);
                    CharPut(X + i, Y + H, EditorData_.get()->DrawCharI);
                    CharPut(X + i + FontW, Y + H, EditorData_.get()->DrawCharIdbl);
                }
                for (int i = 0; i <= H; i += FontH)
                {
                    CharPut(X + 0, Y + i, EditorData_.get()->DrawCharI);
                    CharPut(X + FontW, Y + i, EditorData_.get()->DrawCharIdbl);
                    CharPut(X + W - FontW, Y + i, EditorData_.get()->DrawCharI);
                    CharPut(X + W - 0, Y + i, EditorData_.get()->DrawCharIdbl);
                }
            }
            else
            {
                for (int i = 0; i <= W; i += FontW)
                {
                    CharPut(X + i, Y + 0, EditorData_.get()->DrawCharI);
                    CharPut(X + i, Y + H, EditorData_.get()->DrawCharI);
                }
                for (int i = 0; i <= H; i += FontH)
                {
                    CharPut(X + 0, Y + i, EditorData_.get()->DrawCharI);
                    CharPut(X + W, Y + i, EditorData_.get()->DrawCharI);
                }
            }
        }
        else
        {
            if (DoubleCharMode)
            {
                for (int i = (FontW << 1); i < (W - FontW); i += (FontW << 1))
                {
                    FrameCharPut1(X + i, Y + 0, false, false, true, true, FontW, FontH, true);
                    FrameCharPut1(X + i, Y + H, false, false, true, true, FontW, FontH, true);
                }
                for (int i = FontH; i < H; i += FontH)
                {
                    FrameCharPut1(X + 0, Y + i, true, true, false, false, FontW, FontH, true);
                    FrameCharPut1(X + W - FontW, Y + i, true, true, false, false, FontW, FontH, true);
                }
                if ((W != 1) && (H != 0))
                {
                    FrameCharPut1(X + 0, Y + 0, false, true, false, true, FontW, FontH, true);
                    FrameCharPut1(X + W - FontW, Y + 0, false, true, true, false, FontW, FontH, true);
                    FrameCharPut1(X + 0, Y + H, true, false, false, true, FontW, FontH, true);
                    FrameCharPut1(X + W - FontW, Y + H, true, false, true, false, FontW, FontH, true);
                }
                if ((W == 1) && (H != 0))
                {
                    FrameCharPut1(X + 0, Y + 0, false, true, false, false, FontW, FontH, true);
                    FrameCharPut1(X + 0, Y + H, true, false, false, false, FontW, FontH, true);
                }
                if ((W != 1) && (H == 0))
                {
                    FrameCharPut1(X + 0, Y + 0, false, false, false, true, FontW, FontH, true);
                    FrameCharPut1(X + W - FontW, Y + 0, false, false, true, false, FontW, FontH, true);
                }
                if ((W == 1) && (H == 0))
                {
                    FrameCharPut1(X + 0, Y + 0, false, false, false, false, FontW, FontH, true);
                }
            }
            else
            {
                for (int i = FontW; i < W; i += FontW)
                {
                    FrameCharPut1(X + i, Y + 0, false, false, true, true, FontW, FontH, false);
                    FrameCharPut1(X + i, Y + H, false, false, true, true, FontW, FontH, false);
                }
                for (int i = FontH; i < H; i += FontH)
                {
                    FrameCharPut1(X + 0, Y + i, true, true, false, false, FontW, FontH, false);
                    FrameCharPut1(X + W, Y + i, true, true, false, false, FontW, FontH, false);
                }
                if ((W != 0) && (H != 0))
                {
                    FrameCharPut1(X + 0, Y + 0, false, true, false, true, FontW, FontH, false);
                    FrameCharPut1(X + W, Y + 0, false, true, true, false, FontW, FontH, false);
                    FrameCharPut1(X + 0, Y + H, true, false, false, true, FontW, FontH, false);
                    FrameCharPut1(X + W, Y + H, true, false, true, false, FontW, FontH, false);
                }
                if ((W == 0) && (H != 0))
                {
                    FrameCharPut1(X + 0, Y + 0, false, true, false, false, FontW, FontH, false);
                    FrameCharPut1(X + 0, Y + H, true, false, false, false, FontW, FontH, false);
                }
                if ((W != 0) && (H == 0))
                {
                    FrameCharPut1(X + 0, Y + 0, false, false, false, true, FontW, FontH, false);
                    FrameCharPut1(X + W, Y + 0, false, false, true, false, FontW, FontH, false);
                }
                if ((W == 0) && (H == 0))
                {
                    FrameCharPut1(X + 0, Y + 0, false, false, false, false, FontW, FontH, false);
                }
            }
        }
    }
    if (T == 2)
    {
        if (Frame1I == 0)
        {
            if (DoubleCharMode)
            {
                for (int i = 0; i <= H; i += FontH)
                {
                    for (int ii = 0; ii <= W; ii += (FontW << 1))
                    {
                        CharPut(X + ii, Y + i, EditorData_.get()->DrawCharI);
                        CharPut(X + ii + FontW, Y + i, EditorData_.get()->DrawCharIdbl);
                    }
                }
            }
            else
            {
                for (int i = 0; i <= H; i += FontH)
                {
                    for (int ii = 0; ii <= W; ii += FontW)
                    {
                        CharPut(X + ii, Y + i, EditorData_.get()->DrawCharI);
                    }
                }
            }
        }
        else
        {
            if (DoubleCharMode)
            {
                if ((W != 0) && (H != 0))
                {
                    for (int i = (FontW << 1); i < W; i += (FontW << 1))
                    {
                        FrameCharPut1(X + i, Y + 0, false, true, true, true, FontW, FontH, true);
                        FrameCharPut1(X + i, Y + H, true, false, true, true, FontW, FontH, true);
                    }
                    for (int i = FontH; i < H; i += FontH)
                    {
                        FrameCharPut1(X + 0, Y + i, true, true, false, true, FontW, FontH, true);
                        FrameCharPut1(X + W - FontW, Y + i, true, true, true, false, FontW, FontH, true);
                    }
                    for (int i_Y = FontH; i_Y < H; i_Y += FontH)
                    {
                        for (int i_X = (FontW << 1); i_X < W; i_X += (FontW << 1))
                        {
                            FrameCharPut1(X + i_X, Y + i_Y, true, true, true, true, FontW, FontH, true);
                        }
                    }
                    FrameCharPut1(X + 0, Y + 0, false, true, false, true, FontW, FontH, true);
                    FrameCharPut1(X + W - FontW, Y + 0, false, true, true, false, FontW, FontH, true);
                    FrameCharPut1(X + 0, Y + H, true, false, false, true, FontW, FontH, true);
                    FrameCharPut1(X + W - FontW, Y + H, true, false, true, false, FontW, FontH, true);
                }
                if ((W == 1) && (H != 0))
                {
                    for (int i = FontH; i < H; i += FontH)
                    {
                        FrameCharPut1(X + 0, Y + i, true, true, false, false, FontW, FontH, true);
                        FrameCharPut1(X + W - FontW, Y + i, true, true, false, false, FontW, FontH, true);
                    }
                    FrameCharPut1(X + 0, Y + 0, false, true, false, false, FontW, FontH, true);
                    FrameCharPut1(X + 0, Y + H, true, false, false, false, FontW, FontH, true);
                }
                if ((W != 1) && (H == 0))
                {
                    for (int i = (FontW << 1); i < W; i += (FontW << 1))
                    {
                        FrameCharPut1(X + i, Y + 0, false, false, true, true, FontW, FontH, true);
                        FrameCharPut1(X + i, Y + H, false, false, true, true, FontW, FontH, true);
                    }
                    FrameCharPut1(X + 0, Y + 0, false, false, false, true, FontW, FontH, true);
                    FrameCharPut1(X + W, Y + 0, false, false, true, false, FontW, FontH, true);
                }
                if ((W == 1) && (H == 0))
                {
                    FrameCharPut1(X + 0, Y + 0, false, false, false, false, FontW, FontH, true);
                }
            }
            else
            {
                if ((W != 0) && (H != 0))
                {
                    for (int i = FontW; i < W; i += FontW)
                    {
                        FrameCharPut1(X + i, Y + 0, false, true, true, true, FontW, FontH, false);
                        FrameCharPut1(X + i, Y + H, true, false, true, true, FontW, FontH, false);
                    }
                    for (int i = FontH; i < H; i += FontH)
                    {
                        FrameCharPut1(X + 0, Y + i, true, true, false, true, FontW, FontH, false);
                        FrameCharPut1(X + W, Y + i, true, true, true, false, FontW, FontH, false);
                    }
                    for (int i_Y = FontH; i_Y < H; i_Y += FontH)
                    {
                        for (int i_X = FontW; i_X < W; i_X += FontW)
                        {
                            FrameCharPut1(X + i_X, Y + i_Y, true, true, true, true, FontW, FontH, false);
                        }
                    }
                    FrameCharPut1(X + 0, Y + 0, false, true, false, true, FontW, FontH, false);
                    FrameCharPut1(X + W, Y + 0, false, true, true, false, FontW, FontH, false);
                    FrameCharPut1(X + 0, Y + H, true, false, false, true, FontW, FontH, false);
                    FrameCharPut1(X + W, Y + H, true, false, true, false, FontW, FontH, false);
                }
                if ((W == 0) && (H != 0))
                {
                    for (int i = FontH; i < H; i += FontH)
                    {
                        FrameCharPut1(X + 0, Y + i, true, true, false, false, FontW, FontH, false);
                        FrameCharPut1(X + W, Y + i, true, true, false, false, FontW, FontH, false);
                    }
                    FrameCharPut1(X + 0, Y + 0, false, true, false, false, FontW, FontH, false);
                    FrameCharPut1(X + 0, Y + H, true, false, false, false, FontW, FontH, false);
                }
                if ((W != 0) && (H == 0))
                {
                    for (int i = FontW; i < W; i += FontW)
                    {
                        FrameCharPut1(X + i, Y + 0, false, false, true, true, FontW, FontH, false);
                        FrameCharPut1(X + i, Y + H, false, false, true, true, FontW, FontH, false);
                    }
                    FrameCharPut1(X + 0, Y + 0, false, false, false, true, FontW, FontH, false);
                    FrameCharPut1(X + W, Y + 0, false, false, true, false, FontW, FontH, false);
                }
                if ((W == 0) && (H == 0))
                {
                    FrameCharPut1(X + 0, Y + 0, false, false, false, false, FontW, FontH, false);
                }
            }
        }
    }
}

void EditorSemi::DiamondDraw(int X, int Y, int W__, int H__, int T, int TT, int FontW, int FontH)
{
    int W = W__ * FontW;
    int H = H__ * FontH;
    int W_ = W__ * FontH;
    int H_ = H__ * FontW;
    if (W < 0)
    {
        DiamondDraw(X + W, Y + W_, 0 - W__, H__, T, TT, FontW, FontH);
        return;
    }
    if (H < 0)
    {
        DiamondDraw(X - H_, Y + H, W__, 0 - H__, T, TT, FontW, FontH);
        return;
    }

    if (TT < 0)
    {
        switch (DiamondType)
        {
            case 1: DiamondDraw(X +     0, Y +     0, W__, H__, T, 0, FontW, FontH); return;
            case 2: DiamondDraw(X +     0, Y +     0, W__, H__, T, 1, FontW, FontH); return;
            case 3: DiamondDraw(X +     0, Y +     0, W__, H__, T, 2, FontW, FontH); return;
            case 4: DiamondDraw(X - FontW, Y +     0, W__, H__, T, 1, FontW, FontH); return;
            case 5: DiamondDraw(X +     0, Y - FontH, W__, H__, T, 2, FontW, FontH); return;
            case 6: DiamondDraw(X +     0, Y +     0, W__, H__, T, 3, FontW, FontH); return;
            case 7: DiamondDraw(X +     0, Y + FontH, W__, H__, T, 3, FontW, FontH); return;
            case 8: DiamondDraw(X - FontW, Y + FontH, W__, H__, T, 3, FontW, FontH); return;
            case 9: DiamondDraw(X - FontW, Y +     0, W__, H__, T, 3, FontW, FontH); return;
        }
    }

    bool DoubleCharMode = DoubleDrawMode();

    if (T == 1)
    {
        if (Frame2I == 0)
        {
            if (DoubleCharMode)
            {
                if ((TT == 1) || (TT == 3))
                {
                    int TX = 0;
                    int TY = 0;
                    if (TT == 3)
                    {
                        TY = FontH;
                    }
                    int ii = 0;
                    for (int i = 0; i <= W; i += FontW)
                    {
                        CharPut(X + i + TX, Y + ii - TY, EditorData_.get()->DrawCharI);
                        CharPut(X + i + TX + FontW, Y + ii - TY, EditorData_.get()->DrawCharIdbl);
                        CharPut(X + i - H_, Y + ii + H, EditorData_.get()->DrawCharI);
                        CharPut(X + i - H_ + FontW, Y + ii + H, EditorData_.get()->DrawCharIdbl);
                        ii += FontH;
                    }
                    ii = 0;
                    for (int i = 0; i <= H; i += FontH)
                    {
                        CharPut(X - ii, Y + i - TY, EditorData_.get()->DrawCharI);
                        CharPut(X - ii + FontW, Y + i - TY, EditorData_.get()->DrawCharIdbl);
                        CharPut(X + W - ii + TX, Y + i + W_, EditorData_.get()->DrawCharI);
                        CharPut(X + W - ii + TX + FontW, Y + i + W_, EditorData_.get()->DrawCharIdbl);
                        ii += FontW;
                    }
                }
            }
            else
            {
                if (TT == 3)
                {
                    int ii = 0;
                    for (int i = 0; i <= W; i += FontW)
                    {
                        CharPut(X + i + FontW, Y + ii - FontH, EditorData_.get()->DrawCharI);
                        CharPut(X + i - H_, Y + ii + H, EditorData_.get()->DrawCharI);
                        ii += FontH;
                    }
                    ii = 0;
                    for (int i = 0; i <= H; i += FontH)
                    {
                        CharPut(X - ii, Y + i - FontH, EditorData_.get()->DrawCharI);
                        CharPut(X + W - ii + FontW, Y + i + W_, EditorData_.get()->DrawCharI);
                        ii += FontW;
                    }
                }
                else
                {
                    int TX = 0;
                    int TY = 0;
                    if (TT == 1)
                    {
                        TX = FontW;
                    }
                    if (TT == 2)
                    {
                        TY = FontH;
                    }
                    int ii = 0;
                    for (int i = 0; i <= W; i += FontW)
                    {
                        CharPut(X + i + TX, Y + ii, EditorData_.get()->DrawCharI);
                        CharPut(X + i - H_, Y + ii + H + TY, EditorData_.get()->DrawCharI);
                        ii += FontH;
                    }
                    ii = 0;
                    for (int i = 0; i <= H; i += FontH)
                    {
                        CharPut(X - ii, Y + i, EditorData_.get()->DrawCharI);
                        CharPut(X + W - ii + TX, Y + i + W_ + TY, EditorData_.get()->DrawCharI);
                        ii += FontW;
                    }
                }
            }
        }
        else
        {
            if (DoubleCharMode)
            {
                if ((TT == 1) || (TT == 3))
                {
                    int TX = 0;
                    int TY = 0;
                    bool StdCfg = true;

                    if (TT == 1)
                    {
                        if ((W == 0) && (H == 0))
                        {
                            FrameCharPut2(X, Y, false, false, false, false, FontW, FontH, true);
                            StdCfg = false;
                        }
                        else
                        {
                            if ((W > 0) && (H == 0))
                            {
                                int ii = 0;
                                for (int i = 0; i <= W; i += FontW)
                                {
                                    FrameCharPut2(X + i + TX, Y + ii, false, false, true, true, FontW, FontH, true);
                                    ii += FontH;
                                }
                                StdCfg = false;
                            }
                            if ((W == 0) && (H > 0))
                            {
                                int ii = 0;
                                for (int i = 0; i <= H; i += FontH)
                                {
                                    FrameCharPut2(X - ii, Y + i, true, true, false, false, FontW, FontH, true);
                                    ii += FontW;
                                }
                                StdCfg = false;
                            }
                        }
                    }

                    if (StdCfg)
                    {
                        if (TT == 3)
                        {
                            TY = FontH;
                        }

                        int ii = FontH;
                        for (int i = FontW; i < W; i += FontW)
                        {
                            FrameCharPut2(X + i + TX, Y + ii - TY, false, false, true, true, FontW, FontH, true);
                            FrameCharPut2(X + i - H_, Y + ii + H, false, false, true, true, FontW, FontH, true);
                            ii += FontH;
                        }
                        if (TT == 3)
                        {
                            FrameCharPut2(X - H_, Y + H, false, false, true, true, FontW, FontH, true);
                            FrameCharPut2(X + W + TX, Y + W_ - TY, false, false, true, true, FontW, FontH, true);
                        }

                        ii = FontW;
                        for (int i = FontH; i < H; i += FontH)
                        {
                            FrameCharPut2(X - ii, Y + i - TY, true, true, false, false, FontW, FontH, true);
                            FrameCharPut2(X + W - ii + TX, Y + i + W_, true, true, false, false, FontW, FontH, true);
                            ii += FontW;
                        }
                        if (TT == 3)
                        {
                            FrameCharPut2(X + W + TX, Y + W_, true, true, false, false, FontW, FontH, true);
                            FrameCharPut2(X - H_, Y + H - TY, true, true, false, false, FontW, FontH, true);
                        }

                        FrameCharPut2(X, Y - TY, false, true, false, true, FontW, FontH, true);
                        FrameCharPut2(X + W - H_, Y + W_ + H, true, false, true, false, FontW, FontH, true);
                        if (TT == 1)
                        {
                            FrameCharPut2(X + W + TX, Y + W_, false, true, true, false, FontW, FontH, true);
                            FrameCharPut2(X - H_, Y + H, true, false, false, true, FontW, FontH, true);
                        }
                    }
                }
            }
            else
            {
                if (TT == 3)
                {
                    int ii = 0;
                    for (int i = 0; i <= W; i += FontW)
                    {
                        FrameCharPut2(X + i + FontW, Y + ii - FontH, false, false, true, true, FontW, FontH, false);
                        FrameCharPut2(X + i - H_, Y + ii + H, false, false, true, true, FontW, FontH, false);
                        ii += FontH;
                    }
                    ii = 0;
                    for (int i = 0; i <= H; i += FontH)
                    {
                        FrameCharPut2(X - ii, Y + i - FontH, true, true, false, false, FontW, FontH, false);
                        FrameCharPut2(X + W - ii + FontW, Y + i + W_, true, true, false, false, FontW, FontH, false);
                        ii += FontW;
                    }
                }
                else
                {
                    int TX = 0;
                    int TY = 0;
                    bool StdCfg = true;

                    if (TT == 0)
                    {
                        if (StdCfg)
                        {

                            if (TT == 1)
                            {
                                TX = FontW;
                            }
                            if (TT == 2)
                            {
                                TY = FontH;
                            }

                            int ii = FontH;
                            for (int i = FontW; i < W; i += FontW)
                            {
                                FrameCharPut2(X + i + TX, Y + ii, false, false, true, true, FontW, FontH, false);
                                FrameCharPut2(X + i - H_, Y + ii + H + TY, false, false, true, true, FontW, FontH, false);
                                ii += FontH;
                            }
                            if (TT == 1)
                            {
                                FrameCharPut2(X + TX, Y, false, false, true, true, FontW, FontH, false);
                                FrameCharPut2(X + W - H_, Y + W_ + H + TY, false, false, true, true, FontW, FontH, false);
                            }
                            if (TT == 2)
                            {
                                FrameCharPut2(X - H_, Y + H + TY, false, false, true, true, FontW, FontH, false);
                                FrameCharPut2(X + W + TX, Y + W_, false, false, true, true, FontW, FontH, false);
                            }

                            ii = FontW;
                            for (int i = FontH; i < H; i += FontH)
                            {
                                FrameCharPut2(X - ii, Y + i, true, true, false, false, FontW, FontH, false);
                                FrameCharPut2(X + W - ii + TX, Y + i + W_ + TY, true, true, false, false, FontW, FontH, false);
                                ii += FontW;
                            }
                            if (TT == 1)
                            {
                                FrameCharPut2(X, Y, true, true, false, false, FontW, FontH, false);
                                FrameCharPut2(X + W - H_ + TX, Y + H + W_ + TY, true, true, false, false, FontW, FontH, false);
                            }
                            if (TT == 2)
                            {
                                FrameCharPut2(X + W + TX, Y + W_ + TY, true, true, false, false, FontW, FontH, false);
                                FrameCharPut2(X - H_, Y + H, true, true, false, false, FontW, FontH, false);
                            }

                            if (TT != 1)
                            {
                                FrameCharPut2(X, Y, false, true, false, true, FontW, FontH, false);
                                FrameCharPut2(X + W - H_, Y + W_ + H + TY, true, false, true, false, FontW, FontH, false);
                            }
                            if (TT != 2)
                            {
                                FrameCharPut2(X + W + TX, Y + W_, false, true, true, false, FontW, FontH, false);
                                FrameCharPut2(X - H_, Y + H, true, false, false, true, FontW, FontH, false);
                            }
                        }
                        if ((W == 0) && (H == 0))
                        {
                            FrameCharPut2(X, Y, false, false, false, false, FontW, FontH, false);
                            StdCfg = false;
                        }
                        else
                        {
                            if ((W > 0) && (H == 0))
                            {
                                int ii = 0;
                                for (int i = 0; i <= W; i += FontW)
                                {
                                    FrameCharPut2(X + i + TX, Y + ii, false, false, true, true, FontW, FontH, false);
                                    ii += FontH;
                                }
                                StdCfg = false;
                            }
                            if ((W == 0) && (H > 0))
                            {
                                int ii = 0;
                                for (int i = 0; i <= H; i += FontH)
                                {
                                    FrameCharPut2(X - ii, Y + i, true, true, false, false, FontW, FontH, false);
                                    ii += FontW;
                                }
                                StdCfg = false;
                            }
                        }
                    }

                    if (StdCfg)
                    {

                        if (TT == 1)
                        {
                            TX = FontW;
                        }
                        if (TT == 2)
                        {
                            TY = FontH;
                        }

                        int ii = FontH;
                        for (int i = FontW; i < W; i += FontW)
                        {
                            FrameCharPut2(X + i + TX, Y + ii, false, false, true, true, FontW, FontH, false);
                            FrameCharPut2(X + i - H_, Y + ii + H + TY, false, false, true, true, FontW, FontH, false);
                            ii += FontH;
                        }
                        if (TT == 1)
                        {
                            FrameCharPut2(X + TX, Y, false, false, true, true, FontW, FontH, false);
                            FrameCharPut2(X + W - H_, Y + W_ + H + TY, false, false, true, true, FontW, FontH, false);
                        }
                        if (TT == 2)
                        {
                            FrameCharPut2(X - H_, Y + H + TY, false, false, true, true, FontW, FontH, false);
                            FrameCharPut2(X + W + TX, Y + W_, false, false, true, true, FontW, FontH, false);
                        }

                        ii = FontW;
                        for (int i = FontH; i < H; i += FontH)
                        {
                            FrameCharPut2(X - ii, Y + i, true, true, false, false, FontW, FontH, false);
                            FrameCharPut2(X + W - ii + TX, Y + i + W_ + TY, true, true, false, false, FontW, FontH, false);
                            ii += FontW;
                        }
                        if (TT == 1)
                        {
                            FrameCharPut2(X, Y, true, true, false, false, FontW, FontH, false);
                            FrameCharPut2(X + W - H_ + TX, Y + H + W_ + TY, true, true, false, false, FontW, FontH, false);
                        }
                        if (TT == 2)
                        {
                            FrameCharPut2(X + W + TX, Y + W_ + TY, true, true, false, false, FontW, FontH, false);
                            FrameCharPut2(X - H_, Y + H, true, true, false, false, FontW, FontH, false);
                        }

                        if (TT != 1)
                        {
                            FrameCharPut2(X, Y, false, true, false, true, FontW, FontH, false);
                            FrameCharPut2(X + W - H_, Y + W_ + H + TY, true, false, true, false, FontW, FontH, false);
                        }
                        if (TT != 2)
                        {
                            FrameCharPut2(X + W + TX, Y + W_, false, true, true, false, FontW, FontH, false);
                            FrameCharPut2(X - H_, Y + H, true, false, false, true, FontW, FontH, false);
                        }
                    }
                }
            }
        }
    }
    if (T == 2)
    {
        int i_X_, i_Y_;
        if (Frame2I == 0)
        {
            if (DoubleCharMode)
            {
                if (TT == 1)
                {
                    i_Y_ = 0;
                    for (int i_Y = 0; i_Y <= H; i_Y += FontH)
                    {
                        i_X_ = 0;
                        for (int i_X = 0; i_X <= W; i_X += FontW)
                        {
                            CharPut(X + i_X - i_Y_, Y + i_X_ + i_Y, EditorData_.get()->DrawCharI);
                            CharPut(X + i_X - i_Y_ + FontW, Y + i_X_ + i_Y, EditorData_.get()->DrawCharIdbl);
                            i_X_ += FontH;
                        }
                        i_Y_ += FontW;
                    }
                }
            }
            else
            {
                if (TT == 3)
                {
                    i_Y_ = 0 - FontW;
                    for (int i_Y = 0 - FontH; i_Y <= H; i_Y += FontH)
                    {
                        i_X_ = 0;
                        for (int i_X = 0; i_X <= W; i_X += FontW)
                        {
                            CharPut(X + i_X - i_Y_, Y + i_X_ + i_Y, EditorData_.get()->DrawCharI);
                            i_X_ += FontH;
                        }
                        i_Y_ += FontW;
                    }
                    i_Y_ = 0 - FontW;
                    for (int i_Y = 0 - FontH; i_Y < H; i_Y += FontH)
                    {
                        i_X_ = 0 - FontH;
                        for (int i_X = 0 - FontW; i_X <= W; i_X += FontW)
                        {
                            CharPut(X + i_X - i_Y_, Y + i_X_ + i_Y + FontH, EditorData_.get()->DrawCharI);
                            i_X_ += FontH;
                        }
                        i_Y_ += FontW;
                    }
                }
                else
                {
                    i_Y_ = 0;
                    for (int i_Y = 0; i_Y <= H; i_Y += FontH)
                    {
                        i_X_ = 0;
                        for (int i_X = 0; i_X <= W; i_X += FontW)
                        {
                            CharPut(X + i_X - i_Y_, Y + i_X_ + i_Y, EditorData_.get()->DrawCharI);
                            i_X_ += FontH;
                        }
                        i_Y_ += FontW;
                    }
                    if (TT == 0)
                    {
                        i_Y_ = FontW;
                        for (int i_Y = FontH; i_Y <= H; i_Y += FontH)
                        {
                            i_X_ = 0;
                            for (int i_X = 0; i_X < W; i_X += FontW)
                            {
                                CharPut(X + i_X - i_Y_ + FontW, Y + i_X_ + i_Y, EditorData_.get()->DrawCharI);
                                i_X_ += FontH;
                            }
                            i_Y_ += FontW;
                        }
                    }
                    if (TT == 1)
                    {
                        i_Y_ = 0;
                        for (int i_Y = 0; i_Y <= H; i_Y += FontH)
                        {
                            i_X_ = 0;
                            for (int i_X = 0; i_X <= W; i_X += FontW)
                            {
                                CharPut(X + i_X - i_Y_ + FontW, Y + i_X_ + i_Y, EditorData_.get()->DrawCharI);
                                i_X_ += FontH;
                            }
                            i_Y_ += FontW;
                        }
                    }
                    if (TT == 2)
                    {
                        i_Y_ = 0;
                        for (int i_Y = 0; i_Y <= H; i_Y += FontH)
                        {
                            i_X_ = 0;
                            for (int i_X = 0; i_X <= W; i_X += FontW)
                            {
                                CharPut(X + i_X - i_Y_, Y + i_X_ + i_Y + FontH, EditorData_.get()->DrawCharI);
                                i_X_ += FontH;
                            }
                            i_Y_ += FontW;
                        }
                    }
                }
            }
        }
        else
        {
            if (DoubleCharMode)
            {
                if (TT == 1)
                {
                    int TX = 0;
                    int TY = 0;
                    bool StdCfg = true;

                    if ((W == 0) && (H == 0))
                    {
                        StdCfg = false;
                        FrameCharPut2(X, Y, false, false, false, false, FontW, FontH, true);
                    }
                    else
                    {
                        if ((W > 0) && (H == 0))
                        {
                            int ii = 0;
                            for (int i = 0; i <= W; i += FontW)
                            {
                                FrameCharPut2(X + i + TX, Y + ii, false, false, true, true, FontW, FontH, true);
                                ii += FontH;
                            }
                            StdCfg = false;
                        }
                        if ((W == 0) && (H > 0))
                        {
                            int ii = 0;
                            for (int i = 0; i <= H; i += FontH)
                            {
                                FrameCharPut2(X - ii, Y + i, true, true, false, false, FontW, FontH, true);
                                ii += FontW;
                            }
                            StdCfg = false;
                        }
                    }

                    if (StdCfg)
                    {
                        i_Y_ = (FontW - TX);
                        for (int i_Y = FontH; i_Y < (H + TY); i_Y += FontH)
                        {
                            i_X_ = FontH;
                            for (int i_X = FontW; i_X < (W + TX + TY); i_X += FontW)
                            {
                                FrameCharPut2(X + i_X - i_Y_, Y + i_X_ + i_Y, true, true, true, true, FontW, FontH, true);
                                i_X_ += FontH;
                            }
                            i_Y_ += FontW;
                        }

                        i_X_ = FontH;
                        for (int i_X = FontW; i_X < W; i_X += FontW)
                        {
                            FrameCharPut2(X + i_X + TX, Y + i_X_, false, true, true, true, FontW, FontH, true);
                            FrameCharPut2(X + i_X - H_, Y + i_X_ + H + TY, true, false, true, true, FontW, FontH, true);
                            i_X_ += FontH;
                        }

                        i_Y_ = FontW;
                        for (int i_Y = FontH; i_Y < H; i_Y += FontH)
                        {
                            FrameCharPut2(X - i_Y_, Y + i_Y, true, true, false, true, FontW, FontH, true);
                            FrameCharPut2(X + W - i_Y_ + TX, Y + i_Y + W_ + TY, true, true, true, false, FontW, FontH, true);
                            i_Y_ += FontW;
                        }

                        FrameCharPut2(X, Y, false, true, false, true, FontW, FontH, true);
                        FrameCharPut2(X + W - H_, Y + W_ + H + TY, true, false, true, false, FontW, FontH, true);
                        FrameCharPut2(X + W + TX, Y + W_, false, true, true, false, FontW, FontH, true);
                        FrameCharPut2(X - H_, Y + H, true, false, false, true, FontW, FontH, true);
                    }
                }
            }
            else
            {
                if (TT == 3)
                {
                    i_Y_ = 0;
                    for (int i_Y = 0; i_Y < H; i_Y += FontH)
                    {
                        i_X_ = 0;
                        for (int i_X = 0; i_X <= W; i_X += FontW)
                        {
                            FrameCharPut2(X + i_X - i_Y_, Y + i_X_ + i_Y, true, true, true, true, FontW, FontH, false);
                            i_X_ += FontH;
                        }
                        i_Y_ += FontW;
                    }
                    i_Y_ = 0 - FontW;
                    for (int i_Y = 0 - FontH; i_Y < H; i_Y += FontH)
                    {
                        i_X_ = 0;
                        for (int i_X = 0; i_X < W; i_X += FontW)
                        {
                            FrameCharPut2(X + i_X - i_Y_, Y + i_X_ + i_Y + FontH, true, true, true, true, FontW, FontH, false);
                            i_X_ += FontH;
                        }
                        i_Y_ += FontW;
                    }

                    i_X_ = 0;
                    for (int i_X = 0; i_X <= W; i_X += FontW)
                    {
                        FrameCharPut2(X + i_X + FontW, Y + i_X_ - FontH, false, true, true, true, FontW, FontH, false);
                        FrameCharPut2(X + i_X - H_, Y + i_X_ + H, true, false, true, true, FontW, FontH, false);
                        i_X_ += FontH;
                    }

                    i_Y_ = 0;
                    for (int i_Y = 0; i_Y <= H; i_Y += FontH)
                    {
                        FrameCharPut2(X - i_Y_, Y + i_Y - FontH, true, true, false, true, FontW, FontH, false);
                        FrameCharPut2(X + W - i_Y_ + FontW, Y + i_Y + W_, true, true, true, false, FontW, FontH, false);
                        i_Y_ += FontW;
                    }
                }
                else
                {
                    int TX = 0;
                    int TY = 0;
                    bool StdCfg = true;

                    if (TT == 0)
                    {
                        if ((W == 0) && (H == 0))
                        {
                            StdCfg = false;
                            FrameCharPut2(X, Y, false, false, false, false, FontW, FontH, false);
                        }
                        else
                        {
                            if ((W > 0) && (H == 0))
                            {
                                int ii = 0;
                                for (int i = 0; i <= W; i += FontW)
                                {
                                    FrameCharPut2(X + i + TX, Y + ii, false, false, true, true, FontW, FontH, false);
                                    ii += FontH;
                                }
                                StdCfg = false;
                            }
                            if ((W == 0) && (H > 0))
                            {
                                int ii = 0;
                                for (int i = 0; i <= H; i += FontH)
                                {
                                    FrameCharPut2(X - ii, Y + i, true, true, false, false, FontW, FontH, false);
                                    ii += FontW;
                                }
                                StdCfg = false;
                            }
                        }
                    }

                    if (StdCfg)
                    {
                        if (TT == 1)
                        {
                            TX = FontW;
                        }
                        if (TT == 2)
                        {
                            TY = FontH;
                        }

                        i_Y_ = (FontW - TX);
                        for (int i_Y = (FontH - (TX > 0 ? FontH : 0)); i_Y < (H + TY); i_Y += FontH)
                        {
                            i_X_ = FontH;
                            for (int i_X = FontW; i_X < (W + TX + TY); i_X += FontW)
                            {
                                FrameCharPut2(X + i_X - i_Y_, Y + i_X_ + i_Y, true, true, true, true, FontW, FontH, false);
                                i_X_ += FontH;
                            }
                            i_Y_ += FontW;
                        }
                        i_Y_ = 0;
                        for (int i_Y = 0; i_Y < H; i_Y += FontH)
                        {
                            i_X_ = 0;
                            for (int i_X = 0; i_X < W; i_X += FontW)
                            {
                                FrameCharPut2(X + i_X - i_Y_, Y + i_X_ + i_Y + FontH, true, true, true, true, FontW, FontH, false);
                                i_X_ += FontH;
                            }
                            i_Y_ += FontW;
                        }

                        i_X_ = FontH;
                        for (int i_X = FontW; i_X < W; i_X += FontW)
                        {
                            FrameCharPut2(X + i_X + TX, Y + i_X_, false, true, true, true, FontW, FontH, false);
                            FrameCharPut2(X + i_X - H_, Y + i_X_ + H + TY, true, false, true, true, FontW, FontH, false);
                            i_X_ += FontH;
                        }
                        if (TT == 1)
                        {
                            FrameCharPut2(X + TX, Y, false, true, true, true, FontW, FontH, false);
                            FrameCharPut2(X + W - H_, Y + W_ + H + TY, true, false, true, true, FontW, FontH, false);
                        }
                        if (TT == 2)
                        {
                            FrameCharPut2(X - H_, Y + H + TY, true, false, true, true, FontW, FontH, false);
                            FrameCharPut2(X + W + TX, Y + W_, false, true, true, true, FontW, FontH, false);
                        }

                        i_Y_ = FontW;
                        for (int i_Y = FontH; i_Y < H; i_Y += FontH)
                        {
                            FrameCharPut2(X - i_Y_, Y + i_Y, true, true, false, true, FontW, FontH, false);
                            FrameCharPut2(X + W - i_Y_ + TX, Y + i_Y + W_ + TY, true, true, true, false, FontW, FontH, false);
                            i_Y_ += FontW;
                        }
                        if (TT == 1)
                        {
                            FrameCharPut2(X, Y, true, true, false, true, FontW, FontH, false);
                            FrameCharPut2(X + W - H_ + TX, Y + H + W_ + TY, true, true, true, false, FontW, FontH, false);
                        }
                        if (TT == 2)
                        {
                            FrameCharPut2(X + W + TX, Y + W_ + TY, true, true, true, false, FontW, FontH, false);
                            FrameCharPut2(X - H_, Y + H, true, true, false, true, FontW, FontH, false);
                        }

                        if (TT != 1)
                        {
                            FrameCharPut2(X, Y, false, true, false, true, FontW, FontH, false);
                            FrameCharPut2(X + W - H_, Y + W_ + H + TY, true, false, true, false, FontW, FontH, false);
                        }
                        if (TT != 2)
                        {
                            FrameCharPut2(X + W + TX, Y + W_, false, true, true, false, FontW, FontH, false);
                            FrameCharPut2(X - H_, Y + H, true, false, false, true, FontW, FontH, false);
                        }
                    }
                }
            }
        }
    }
}
