#include "editordata.h"

EditorData::EditorData()
{

}

int EditorData::ElementGet(AnsiLineOccupyEx Data, int KindType, int X, int Y, int Default)
{
    ElementGet_Kind.Clear();
    ElementGet_Count.Clear();
    int MostCount = 1;
    for (int YY = Y; YY < (Y + CursorFontH); YY++)
    {
        for (int XX = X; XX < (X + CursorFontW); XX++)
        {
            int Kind = Default;
            if ((Data.CountLines() > YY) && (YY >= 0))
            {
                if ((Data.CountItems(YY) > XX) && (XX >= 0))
                {
                    Data.Get_(YY, XX);
                    switch (KindType)
                    {
                        case 0: Kind = Data.Item_Char; break;
                        case 1: Kind = Data.Item_ColorB; break;
                        case 2: Kind = Data.Item_ColorF; break;
                        case 3: Kind = Data.Item_ColorA; break;
                        case 4: Kind = Data.Item_FontW; break;
                        case 5: Kind = Data.Item_FontH; break;
                    }
                }
            }
            int Idx = ElementGet_Kind.IndexOf(Kind);
            if (Idx >= 0)
            {
                ElementGet_Count[Idx] = ElementGet_Count[Idx] + 1;
                if (MostCount < ElementGet_Count[Idx])
                {
                    MostCount = ElementGet_Count[Idx];
                }
            }
            else
            {
                ElementGet_Kind.Add(Kind);
                ElementGet_Count.Add(1);
            }
        }
    }
    for (int i = 0; i < ElementGet_Kind.Count; i++)
    {
        if (ElementGet_Count[i] == MostCount)
        {
            return ElementGet_Kind[i];
        }
    }
    return Default;
}


AnsiLineOccupyItem EditorData::ElementGetObj(int X, int Y, bool Space, bool SingleCell)
{
    AnsiLineOccupyItem Item;
    Item.Item_Char = ElementGetVal(X, Y, Space, SingleCell, 0);
    Item.Item_ColorB = ElementGetVal(X, Y, Space, SingleCell, 1);
    Item.Item_ColorF = ElementGetVal(X, Y, Space, SingleCell, 2);
    Item.Item_ColorA = ElementGetVal(X, Y, Space, SingleCell, 3);
    Item.Item_FontW = ElementGetVal(X, Y, Space, SingleCell, 4);
    Item.Item_FontH = ElementGetVal(X, Y, Space, SingleCell, 5);
    return Item;
}

int EditorData::ElementGetVal(int X, int Y, bool Space, bool SingleCell, int KindType)
{
    if (SingleCell || ((CursorFontW == 1) && (CursorFontH == 1)))
    {
        if ((TextBuffer.CountLines() > Y) && (Y >= 0))
        {
            if ((TextBuffer.CountItems(Y) > X) && (X >= 0))
            {
                TextBuffer.Get(Y, X);
                switch (KindType)
                {
                    case 0: return TextBuffer.Item_Char;
                    case 1: return TextBuffer.Item_ColorB;
                    case 2: return TextBuffer.Item_ColorF;
                    case 3: return TextBuffer.Item_ColorA;
                    case 4: return TextBuffer.Item_FontW;
                    case 5: return TextBuffer.Item_FontH;
                }
            }
        }
        switch (KindType)
        {
            case 0:
                if (Space)
                {
                    return Str::SpaceChar0;
                }
                else
                {
                    return -1;
                }
            case 1:
            case 2:
                return -1;
            default:
                return 0;
        }
    }
    else
    {
        switch (KindType)
        {
            case 0:
                return ElementGet(TextBuffer, KindType, X, Y, Space ? Str::SpaceChar0 : -1);
            case 1:
            case 2:
                return ElementGet(TextBuffer, KindType, X, Y, -1);
            default:
                return ElementGet(TextBuffer, KindType, X, Y, 0);
        }
    }
}

int EditorData::CharGet(int X, int Y, bool Space, bool SingleCell)
{
    return ElementGetVal(X, Y, Space, SingleCell, 0);
}

int EditorData::ColoBGet(int X, int Y, bool Space, bool SingleCell)
{
    return ElementGetVal(X, Y, Space, SingleCell, 1);
}

int EditorData::ColoFGet(int X, int Y, bool Space, bool SingleCell)
{
    return ElementGetVal(X, Y, Space, SingleCell, 2);
}

void EditorData::TextInsert(int X, int Y, int W, int H, int InsDelMode)
{
    if (W < 0)
    {
        TextInsert(X + W, Y, 0 - W, H, InsDelMode);
        return;
    }
    if (H < 0)
    {
        TextInsert(X, Y + H, W, 0 - H, InsDelMode);
        return;
    }
    if (X < 0)
    {
        TextInsert(0, Y, W + X, H, InsDelMode);
        return;
    }
    if (Y < 0)
    {
        TextInsert(X, 0, W, H + Y, InsDelMode);
        return;
    }

    int W_ = (W + 1) * CursorFontW;
    int H_ = (H + 1) * CursorFontH;

    TextBuffer.BlankChar();
    AnsiLineOccupyEx ColoBuffer = TextBuffer.CloneData();
    AnsiLineOccupyEx TempBuffer;
    switch (InsDelMode)
    {
        case 0:
        case 20:
            for (int i = Y; i < (Y + H_); i++)
            {
                if (TextBuffer.CountLines() > i)
                {
                    if (TextBuffer.CountItemsTrim(i) > X)
                    {
                        if (ToggleDrawText)
                        {
                            TextBuffer.Insert(i, X, W_);
                        }
                        else
                        {
                            TextBuffer.Append(i, W_);
                        }
                        if (ToggleDrawColo)
                        {
                            ColoBuffer.Insert(i, X, W_);
                        }
                        else
                        {
                            ColoBuffer.Append(i, W_);
                        }
                    }
                }
            }
            break;
        case 1:
        case 21:
            if (Y < TextBuffer.CountLines())
            {
                for (int i = Y; i < TextBuffer.CountLines(); i++)
                {
                    TextBuffer.PadRight(i, X + W_ + 1);
                    ColoBuffer.PadRight(i, X + W_ + 1);
                }
                for (int i = 0; i < H_; i++)
                {
                    TextBuffer.AppendLine();
                    TextBuffer.Append(TextBuffer.CountLines() - 1, X + W_ + 1);
                    ColoBuffer.AppendLine();
                    ColoBuffer.Append(ColoBuffer.CountLines() - 1, X + W_ + 1);
                }
                for (int i = (TextBuffer.CountLines() - H_ - 0); i > Y; i--)
                {
                    if (ToggleDrawText)
                    {
                        TempBuffer.Clear();
                        TempBuffer.AppendLineCopy(TextBuffer, i - 1);
                        TempBuffer.Crop(0, X, W_);

                        TextBuffer.Delete(i + H_ - 1, X, W_);
                        TextBuffer.Insert(i + H_ - 1, X, TempBuffer, 0);
                    }
                    if (ToggleDrawColo)
                    {
                        TempBuffer.Clear();
                        TempBuffer.AppendLineCopy(ColoBuffer, i - 1);
                        TempBuffer.Crop(0, X, W_);

                        ColoBuffer.Delete(i + H_ - 1, X, W_);
                        ColoBuffer.Insert(i + H_ - 1, X, TempBuffer, 0);
                    }
                    TextBuffer.Trim(i + H_ - 1);
                    ColoBuffer.Trim(i + H_ - 1);
                }
                TextBuffer.BlankChar();
                ColoBuffer.BlankChar();
                for (int i = Y; i < (Y + H_); i++)
                {
                    if (ToggleDrawText)
                    {
                        TextBuffer.Delete(i, X, W_);
                        TextBuffer.Insert(i, X, W_);
                    }
                    if (ToggleDrawColo)
                    {
                        ColoBuffer.Delete(i, X, W_);
                        ColoBuffer.Insert(i, X, W_);
                    }
                    TextBuffer.Trim(i);
                    ColoBuffer.Trim(i);
                }
            }
            break;
        case 2:
        case 22:
            for (int i = 0; i < TextBuffer.CountLines(); i++)
            {
                if (TextBuffer.CountItemsTrim(i) > X)
                {
                    if (ToggleDrawText)
                    {
                        TextBuffer.Insert(i, X, W_);
                    }
                    else
                    {
                        TextBuffer.Append(i, W_);
                    }
                    if (ToggleDrawColo)
                    {
                        ColoBuffer.Insert(i, X, W_);
                    }
                    else
                    {
                        ColoBuffer.Append(i, W_);
                    }
                }
            }
            break;
        case 3:
        case 23:
            if (Y < TextBuffer.CountLines())
            {
                for (int i = 0; i < H_; i++)
                {
                    if (ToggleDrawText)
                    {
                        TextBuffer.InsertLine(Y);
                    }
                    else
                    {
                        TextBuffer.AppendLine();
                    }
                    if (ToggleDrawColo)
                    {
                        ColoBuffer.InsertLine(Y);
                    }
                    else
                    {
                        ColoBuffer.AppendLine();
                    }
                }
                for (int i = Y; i < TextBuffer.CountLines(); i++)
                {
                    if (TextBuffer.CountItems(i) < ColoBuffer.CountItems(i))
                    {
                        TextBuffer.PadRight(i, ColoBuffer.CountItems(i));
                    }
                    if (ColoBuffer.CountItems(i) < TextBuffer.CountItems(i))
                    {
                        ColoBuffer.PadRight(i, TextBuffer.CountItems(i));
                    }
                }
            }
            break;
        case 4:
        case 24:
            for (int i = Y; i < (Y + H_); i++)
            {
                if (TextBuffer.CountLines() > i)
                {
                    if (TextBuffer.CountItemsTrim(i) > X)
                    {
                        TextBuffer.BlankChar();
                        TextBuffer.Append(i, W_);
                        ColoBuffer.BlankChar();
                        ColoBuffer.Append(i, W_);
                        if (ToggleDrawText)
                        {
                            TempBuffer.Clear();
                            TempBuffer.AppendLineCopy(TextBuffer, i);
                            TempBuffer.Crop(0, X + W_ - CursorFontW, CursorFontW);
                            TextBuffer.Delete(i, X + W_ - CursorFontW, CursorFontW);
                            TextBuffer.Insert(i, X, TempBuffer, 0);
                        }
                        if (ToggleDrawColo)
                        {
                            TempBuffer.Clear();
                            TempBuffer.AppendLineCopy(ColoBuffer, i);
                            TempBuffer.Crop(0, X + W_ - CursorFontW, CursorFontW);
                            ColoBuffer.Delete(i, X + W_ - CursorFontW, CursorFontW);
                            ColoBuffer.Insert(i, X, TempBuffer, 0);
                        }
                        TextBuffer.Trim(i);
                        ColoBuffer.Trim(i);
                    }
                }
            }
            break;
        case 5:
        case 25:
            if (Y < TextBuffer.CountLines())
            {
                TextBuffer.BlankChar();
                ColoBuffer.BlankChar();
                for (int i = Y; i < (Y + H_); i++)
                {
                    if (TextBuffer.CountLines() <= i)
                    {
                        TextBuffer.AppendLine();
                    }
                    TextBuffer.PadRight(i, (X + W_ + 1));
                    if (ColoBuffer.CountLines() <= i)
                    {
                        ColoBuffer.AppendLine();
                    }
                    ColoBuffer.PadRight(i, (X + W_ + 1));
                }
                TempBuffer.Clear();
                TempBuffer.AppendLine();
                TempBuffer.AppendLine();
                for (int i = 0; i < CursorFontH; i++)
                {
                    TempBuffer.AppendLineCopy(TextBuffer, Y + i + H_ - CursorFontH);
                    TempBuffer.AppendLineCopy(ColoBuffer, Y + i + H_ - CursorFontH);
                    TempBuffer.Crop((i << 1) + 2, X, W_);
                    TempBuffer.Crop((i << 1) + 3, X, W_);
                }
                for (int i = (Y + H_ - CursorFontH - 1); i > (Y - 1); i--)
                {
                    if (ToggleDrawText)
                    {
                        TempBuffer.LineCopy(TextBuffer, i, 0);
                        TempBuffer.Crop(0, X, W_);

                        TextBuffer.Delete(i + CursorFontH, X, W_);
                        TextBuffer.Insert(i + CursorFontH, X, TempBuffer, 0);
                    }
                    if (ToggleDrawColo)
                    {
                        TempBuffer.LineCopy(ColoBuffer, i, 1);
                        TempBuffer.Crop(1, X, W_);

                        ColoBuffer.Delete(i + CursorFontH, X, W_);
                        ColoBuffer.Insert(i + CursorFontH, X, TempBuffer, 1);
                    }
                    TextBuffer.Trim(i + CursorFontH);
                    ColoBuffer.Trim(i + CursorFontH);
                }
                for (int i = 0; i < CursorFontH; i++)
                {
                    if (ToggleDrawText)
                    {
                        TextBuffer.Delete(Y + i, X, W_);
                        TextBuffer.Insert(Y + i, X, TempBuffer, (i << 1) + 2);
                    }
                    if (ToggleDrawColo)
                    {
                        ColoBuffer.Delete(Y + i, X, W_);
                        ColoBuffer.Insert(Y + i, X, TempBuffer, (i << 1) + 3);
                    }
                    TextBuffer.Trim(Y + i);
                    ColoBuffer.Trim(Y + i);
                }
            }
            break;
    }
    if (InsDelMode < 20)
    {
        EditorUndo_.get()->BufferAdd(X, Y, InsDelMode, W, H);
        //!!!!TextDisplay(0);
    }
    TextBuffer.MergeColor(ColoBuffer);
    switch (InsDelMode)
    {
        case 0:
        case 10:
        case 1:
        case 11:
            for (int i_Y = Y; i_Y < (Y + H_); i_Y += CursorFontH)
            {
                for (int i_X = X; i_X < (X + W_); i_X += CursorFontW)
                {
                    for (int i_YY = 0; i_YY < CursorFontH; i_YY++)
                    {
                        for (int i_XX = 0; i_XX < CursorFontW; i_XX++)
                        {
                            while (TextBuffer.CountLines() <= (Y + H_))
                            {
                                TextBuffer.AppendLine();
                            }
                            TextBuffer.BlankChar();
                            TextBuffer.PadRight(i_Y + i_YY, X + W_);

                            TextBuffer.Get(i_Y + i_YY, i_X + i_XX);
                            if (ToggleDrawText)
                            {
                                TextBuffer.Item_Char = DrawCharI;
                                TextBuffer.Item_FontW = CoreStatic::FontSizeCode(CursorFontW, i_XX);
                                TextBuffer.Item_FontH = CoreStatic::FontSizeCode(CursorFontH, i_YY);
                            }
                            if (ToggleDrawColo)
                            {
                                TextBuffer.Item_ColorB = DrawColoBI;
                                TextBuffer.Item_ColorF = DrawColoFI;
                                TextBuffer.Item_ColorA = DrawColoAI;
                            }
                            TextBuffer.Set(i_Y + i_YY, i_X + i_XX);
                            if (InsDelMode < 20)
                            {
                                TextBuffer.Get(i_Y + i_YY, i_X + i_XX);
                                EditorUndo_.get()->BufferAdd(i_X + i_XX, i_Y + i_YY, AnsiLineOccupyItem(), TextBuffer.CopyItemObj());
                            }
                        }
                    }
                }
            }
            break;
    }
}

void EditorData::TextDelete(int X, int Y, int W, int H, int InsDelMode)
{
    if (W < 0)
    {
        TextDelete(X + W, Y, 0 - W, H, InsDelMode);
        return;
    }
    if (H < 0)
    {
        TextDelete(X, Y + H, W, 0 - H, InsDelMode);
        return;
    }
    if (X < 0)
    {
        TextDelete(0, Y, W + X, H, InsDelMode);
        return;
    }
    if (Y < 0)
    {
        TextDelete(X, 0, W, H + Y, InsDelMode);
        return;
    }

    int W_ = (W + 1) * CursorFontW;
    int H_ = (H + 1) * CursorFontH;

    AnsiLineOccupyEx ColoBuffer = TextBuffer.CloneData();
    AnsiLineOccupyEx TempBuffer;
    switch (InsDelMode)
    {
        case 0:
        case 20:
            for (int i = Y; i < (Y + H_); i++)
            {
                if (TextBuffer.CountLines() > i)
                {
                    if (TextBuffer.CountItemsTrim(i) > X)
                    {
                        int RemCount = W_;
                        if (TextBuffer.CountItems(i) <= (X + W_))
                        {
                            RemCount = TextBuffer.CountItems(i) - X;
                        }
                        if (InsDelMode < 20)
                        {
                            for (int ii = 0; ii < RemCount; ii++)
                            {
                                TextBuffer.Get(i, X + ii);
                                EditorUndo_.get()->BufferAdd(X + ii, i, TextBuffer.CopyItemObj(), AnsiLineOccupyItem());
                            }
                        }
                        if (ToggleDrawText)
                        {
                            TextBuffer.BlankChar();
                            TextBuffer.Append(i, RemCount);
                            TextBuffer.Delete(i, X, RemCount);
                        }
                        if (ToggleDrawColo)
                        {
                            ColoBuffer.BlankChar();
                            ColoBuffer.Append(i, RemCount);
                            ColoBuffer.Delete(i, X, RemCount);
                        }
                        TextBuffer.Trim(i);
                        ColoBuffer.Trim(i);
                    }
                }
            }
            break;
        case 1:
        case 21:
            if (Y < TextBuffer.CountLines())
            {
                for (int i = Y; i < TextBuffer.CountLines(); i++)
                {
                    TextBuffer.BlankChar();
                    TextBuffer.PadRight(i, X + W_ + 1);
                    ColoBuffer.BlankChar();
                    ColoBuffer.PadRight(i, X + W_ + 1);
                }
                if (InsDelMode < 20)
                {
                    for (int i = Y; i < (Y + H_); i++)
                    {
                        for (int ii = X; ii < (X + W_); ii++)
                        {
                            if (TextBuffer.CountLines() > i)
                            {
                                if (TextBuffer.CountItems(i) > ii)
                                {
                                    TextBuffer.Get(i, ii);
                                    EditorUndo_.get()->BufferAdd(ii, i, TextBuffer.CopyItemObj(), AnsiLineOccupyItem());
                                }
                            }
                        }
                    }
                }
                for (int i = Y; i < TextBuffer.CountLines(); i++)
                {
                    if (ToggleDrawText)
                    {
                        TextBuffer.Delete(i, X, W_);
                    }
                    if (ToggleDrawColo)
                    {
                        ColoBuffer.Delete(i, X, W_);
                    }
                    if (ToggleDrawText)
                    {
                        if ((i + H_) < TextBuffer.CountLines())
                        {
                            TempBuffer.Clear();
                            TempBuffer.AppendLineCopy(TextBuffer, i + H_);
                            TempBuffer.Crop(0, X, W_);
                            TextBuffer.Insert(i, X, TempBuffer, 0);
                        }
                        else
                        {
                            TextBuffer.BlankChar();
                            TextBuffer.Insert(i, X, W_);
                        }
                    }
                    if (ToggleDrawColo)
                    {
                        if ((i + H_) < ColoBuffer.CountLines())
                        {
                            TempBuffer.Clear();
                            TempBuffer.AppendLineCopy(ColoBuffer, i + H_);
                            TempBuffer.Crop(0, X, W_);
                            ColoBuffer.Insert(i, X, TempBuffer, 0);
                        }
                        else
                        {
                            ColoBuffer.BlankChar();
                            ColoBuffer.Insert(i, X, W_);
                        }
                    }

                    TextBuffer.Trim(i);
                    ColoBuffer.Trim(i);
                }
            }
            break;
        case 2:
        case 22:
            for (int i = 0; i < TextBuffer.CountLines(); i++)
            {
                if (TextBuffer.CountItemsTrim(i) > X)
                {
                    int RemCount = W_;
                    if (TextBuffer.CountItems(i) <= (X + W_))
                    {
                        RemCount = TextBuffer.CountItems(i) - X;
                    }
                    if (InsDelMode < 20)
                    {
                        for (int ii = 0; ii < RemCount; ii++)
                        {
                            TextBuffer.Get(i, X + ii);
                            EditorUndo_.get()->BufferAdd(X + ii, i, TextBuffer.CopyItemObj(), AnsiLineOccupyItem());
                        }
                    }
                    if (ToggleDrawText)
                    {
                        TextBuffer.BlankChar();
                        TextBuffer.Append(i, RemCount);
                        TextBuffer.Delete(i, X, RemCount);
                    }
                    if (ToggleDrawColo)
                    {
                        ColoBuffer.BlankChar();
                        ColoBuffer.Append(i, RemCount);
                        ColoBuffer.Delete(i, X, RemCount);
                    }
                    TextBuffer.Trim(i);
                    ColoBuffer.Trim(i);
                }
            }
            break;
        case 3:
        case 23:
            if (Y < TextBuffer.CountLines())
            {
                for (int i = 0; i < H_; i++)
                {
                    if (TextBuffer.CountLines() > Y)
                    {
                        if (InsDelMode < 20)
                        {
                            for (int ii = 0; ii < TextBuffer.CountItems(Y); ii++)
                            {
                                TextBuffer.Get(Y, ii);
                                EditorUndo_.get()->BufferAdd(ii, Y + i, TextBuffer.CopyItemObj(), AnsiLineOccupyItem());
                            }
                        }
                        if (ToggleDrawText)
                        {
                            TextBuffer.DeleteLine(Y);
                            TextBuffer.AppendLine();
                        }
                        if (ToggleDrawColo)
                        {
                            ColoBuffer.DeleteLine(Y);
                            ColoBuffer.AppendLine();
                        }
                    }
                }
                for (int i = Y; i < TextBuffer.CountLines(); i++)
                {
                    if (TextBuffer.CountItems(i) < ColoBuffer.CountItems(i))
                    {
                        TextBuffer.PadRight(i, ColoBuffer.CountItems(i));
                    }
                    if (ColoBuffer.CountItems(i) < TextBuffer.CountItems(i))
                    {
                        ColoBuffer.PadRight(i, TextBuffer.CountItems(i));
                    }
                }
            }
            break;
        case 4:
        case 24:
            for (int i = Y; i < (Y + H_); i++)
            {
                TextBuffer.BlankChar();
                if (TextBuffer.CountLines() > i)
                {
                    if (TextBuffer.CountItemsTrim(i) > X)
                    {
                        TextBuffer.Append(i, W_);
                        ColoBuffer.Append(i, W_);
                        if (ToggleDrawText)
                        {
                            TempBuffer.Clear();
                            TempBuffer.AppendLineCopy(TextBuffer, i);
                            TempBuffer.Crop(0, X, CursorFontW);

                            TextBuffer.Delete(i, X, CursorFontW);
                            TextBuffer.Insert(i, X + W_ - CursorFontW, TempBuffer, 0);
                        }
                        if (ToggleDrawColo)
                        {
                            TempBuffer.Clear();
                            TempBuffer.AppendLineCopy(ColoBuffer, i);
                            TempBuffer.Crop(0, X, CursorFontW);

                            ColoBuffer.Delete(i, X, CursorFontW);
                            ColoBuffer.Insert(i, X + W_ - CursorFontW, TempBuffer, 0);
                        }
                        TextBuffer.Trim(i);
                        ColoBuffer.Trim(i);
                    }
                }
            }
            break;
        case 5:
        case 25:
            if (Y < TextBuffer.CountLines())
            {
                TextBuffer.BlankChar();
                ColoBuffer.BlankChar();
                for (int i = Y; i < (Y + H_); i++)
                {
                    if (TextBuffer.CountLines() <= i)
                    {
                        TextBuffer.AppendLine();
                    }
                    TextBuffer.PadRight(i, X + W_ + 1);
                    if (ColoBuffer.CountLines() <= i)
                    {
                        ColoBuffer.AppendLine();
                    }
                    ColoBuffer.PadRight(i, X + W_ + 1);
                }
                TempBuffer.Clear();
                TempBuffer.AppendLine();
                TempBuffer.AppendLine();
                for (int i = 0; i < CursorFontH; i++)
                {
                    TempBuffer.AppendLineCopy(TextBuffer, Y + i);
                    TempBuffer.AppendLineCopy(ColoBuffer, Y + i);
                    TempBuffer.Crop((i << 1) + 2, X, W_);
                    TempBuffer.Crop((i << 1) + 3, X, W_);
                }
                for (int i = (Y + CursorFontH); i < (Y + H_); i++)
                {
                    if (ToggleDrawText)
                    {
                        TempBuffer.LineCopy(TextBuffer, i, 0);
                        TempBuffer.Crop(0, X, W_);

                        TextBuffer.Delete(i - CursorFontH, X, W_);
                        TextBuffer.Insert(i - CursorFontH, X, TempBuffer, 0);
                    }
                    if (ToggleDrawColo)
                    {
                        TempBuffer.LineCopy(ColoBuffer, i, 1);
                        TempBuffer.Crop(1, X, W_);

                        ColoBuffer.Delete(i - CursorFontH, X, W_);
                        ColoBuffer.Insert(i - CursorFontH, X, TempBuffer, 1);
                    }
                    TextBuffer.Trim(i - CursorFontH);
                    ColoBuffer.Trim(i - CursorFontH);
                }
                for (int i = 0; i < CursorFontH; i++)
                {
                    if (ToggleDrawText)
                    {
                        TextBuffer.Delete(Y + H_ - CursorFontH + i, X, W_);
                        TextBuffer.Insert(Y + H_ - CursorFontH + i, X, TempBuffer, (i << 1) + 2);
                    }
                    if (ToggleDrawColo)
                    {
                        ColoBuffer.Delete(Y + H_ - CursorFontH + i, X, W_);
                        ColoBuffer.Insert(Y + H_ - CursorFontH + i, X, TempBuffer, (i << 1) + 3);
                    }
                    TextBuffer.Trim(Y + H_ - CursorFontH + i);
                    ColoBuffer.Trim(Y + H_ - CursorFontH + i);
                }
            }
            break;
    }
    if (InsDelMode < 20)
    {
        EditorUndo_.get()->BufferAdd(X, Y, InsDelMode + 10, W, H);
        TextBuffer.TrimLines();
        //!!!!!!!!!TextDisplay(0);
    }
    TextBuffer.MergeColor(ColoBuffer);
}

void EditorData::DisplayMove(int Dir, int WinTxtW, int WinTxtH)
{
    switch (Dir)
    {
        case 1:
            ScrCharDisp_.InsertLine(0);
            ScrCharDisp_.PadRightTab(0, WinTxtW);
            ScrCharDisp_.DeleteLine(WinTxtH);
            break;
        case 2:
            ScrCharDisp_.DeleteLine(0);
            ScrCharDisp_.AppendLine();
            ScrCharDisp_.PadRightTab(ScrCharDisp_.CountLines() - 1, WinTxtW);
            break;
        case 3:
            ScrCharDisp_.BlankChar();
            for (int i = 0; i < WinTxtH; i++)
            {
                ScrCharDisp_.Insert(i, 0);
                ScrCharDisp_.Delete(i, WinTxtW);
            }
            break;
        case 4:
            ScrCharDisp_.BlankChar();
            for (int i = 0; i < WinTxtH; i++)
            {
                ScrCharDisp_.Delete(i, 0);
                ScrCharDisp_.Append(i);
            }
            break;
    }
}

int EditorData::CursorXBase()
{
    return CursorX % CursorFontW;
}

int EditorData::CursorYBase()
{
    return CursorY % CursorFontH;
}

void EditorData::CharPut(int X, int Y, AnsiLineOccupyItem Item, bool SingleCell)
{
    CharPutLineRepaint.Clear();
    //CharPutLineRepaint --> TextDisplayLine(CharPutLineRepaint[i]);


    AnsiLineOccupyItem Item_;

    int CursorFontW_ = SingleCell ? 1 : CursorFontW;
    int CursorFontH_ = SingleCell ? 1 : CursorFontH;

    if (X < 0)
    {
        return;
    }
    if (Y < 0)
    {
        return;
    }

    while (TextBuffer.CountLines() <= (Y + CursorFontH - 1))
    {
        TextBuffer.AppendLine();
        CharPutLineRepaint.Add(TextBuffer.CountLines() - 1);
    }

    for (int YY = 0; YY < CursorFontH_; YY++)
    {
        for (int XX = 0; XX < CursorFontW_; XX++)
        {
            Item_.Item_Char = ElementGetVal(X + XX, Y + YY, true, true, 0);
            Item_.Item_ColorB = ElementGetVal(X + XX, Y + YY, true, true, 1);
            Item_.Item_ColorF = ElementGetVal(X + XX, Y + YY, true, true, 2);
            Item_.Item_ColorA = ElementGetVal(X + XX, Y + YY, true, true, 3);
            Item_.Item_FontW = ElementGetVal(X + XX, Y + YY, true, false, 4);
            Item_.Item_FontH = ElementGetVal(X + XX, Y + YY, true, false, 5);

            if (!ToggleDrawText)
            {
                Item.Item_Char = Item_.Item_Char;
                Item.Item_FontW = Item_.Item_FontW;
                Item.Item_FontH = Item_.Item_FontH;
            }
            if (!ToggleDrawColo)
            {
                Item.Item_ColorB = Item_.Item_ColorB;
                Item.Item_ColorF = Item_.Item_ColorF;
                Item.Item_ColorA = Item_.Item_ColorA;
            }

            if (!SingleCell)
            {
                if (ToggleDrawText)
                {
                    Item.Item_FontW = CoreStatic::FontSizeCode(CursorFontW_, XX);
                    Item.Item_FontH = CoreStatic::FontSizeCode(CursorFontH_, YY);
                }
            }

            if (TextBuffer.CountItems(Y + YY) > (X + XX))
            {
                TextBuffer.CopyItem(Item);
                TextBuffer.Set_(Y + YY, X + XX);
                TextBuffer.Trim(Y + YY);
                if (Str::SpaceChars.Contains(Item.Item_Char) && (Item.Item_ColorB < 0) && (Item.Item_ColorF < 0) && (Item.Item_ColorA == 0))
                {
                    TextBuffer.Trim(Y + YY);
                }
            }
            else
            {
                if ((!Str::SpaceChars.Contains(Item.Item_Char)) || (Item.Item_ColorB >= 0) || (Item.Item_ColorF >= 0) || (Item.Item_ColorA > 0))
                {
                    TextBuffer.CopyItem(Item);
                    TextBuffer.Set_(Y + YY, X + XX);
                }
            }

            if (EditorUndo_.get()->BufferEnabled)
            {
                EditorUndo_.get()->BufferAdd(X + XX, Y + YY, Item_, Item);
            }
        }
    }
    while ((TextBuffer.CountLines() > 0) && (TextBuffer.CountItems(TextBuffer.CountLines() - 1) == 0))
    {
        TextBuffer.DeleteLine(TextBuffer.CountLines() - 1);
        CharPutLineRepaint.Add(TextBuffer.CountLines());
    }
    for (int YY = 0; YY < CursorFontH_; YY++)
    {
        CharPutLineRepaint.Add(Y + YY);
    }
}

void EditorData::CharPut0(int X, int Y, int Ch)
{
    AnsiLineOccupyItem Item;
    Item.Item_Char = Ch;
    Item.Item_ColorB = DrawColoBI;
    Item.Item_ColorF = DrawColoFI;
    Item.Item_ColorA = DrawColoAI;
    Item.Item_FontW = 0;
    Item.Item_FontH = 0;
    CharPut(X, Y, Item, false);
}

void EditorData::CharPut(int X, int Y, int Ch)
{
    AnsiLineOccupyItem Item;
    Item.Item_Char = Ch;
    Item.Item_ColorB = DrawColoBI;
    Item.Item_ColorF = DrawColoFI;
    Item.Item_ColorA = DrawColoAI;
    Item.Item_FontW = -1;
    Item.Item_FontH = -1;
    CharPut(X, Y, Item, false);
}

void EditorData::CharPutDbl(int X, int Y, int Offset, int Ch)
{
    AnsiLineOccupyItem Item;
    Item.Item_Char = Ch;
    Item.Item_ColorB = DrawColoBI;
    Item.Item_ColorF = DrawColoFI;
    Item.Item_ColorA = DrawColoAI;
    Item.Item_FontW = -1;
    Item.Item_FontH = -1;
    CharPut(X, Y, Item, false);

    int Ch_ = 0;//!!!!!!!!!!!!!!!!!!!!!!!!!!CharDouble(Ch);
    if (Ch_ != 0)
    {
        AnsiLineOccupyItem Item_;
        Item_.Item_Char = Ch_;
        Item_.Item_ColorB = DrawColoBI;
        Item_.Item_ColorF = DrawColoFI;
        Item_.Item_ColorA = DrawColoAI;
        Item_.Item_FontW = -1;
        Item_.Item_FontH = -1;
        CharPut(X + Offset, Y, Item_, false);
    }
}

void EditorData::UndoRedoBegin()
{
    EditorUndo_.get()->ItemBegin(CursorX, CursorY, CursorXSize, CursorYSize, CursorFontW, CursorFontH, ToggleDrawText, ToggleDrawColo);
}

void EditorData::UndoRedoEnd()
{
    EditorUndo_.get()->ItemEnd(CursorX, CursorY, CursorXSize, CursorYSize, CursorFontW, CursorFontH);
    TextBuffer.TrimLines();
}

void EditorData::UndoRedoAction(EditorUndoItem::EntryParams Params, XList<AnsiLineOccupyItemUndoRedo> Items, bool Rev)
{
    int I_Begin = 0;
    int I_End = Items.Count;
    int I_Step = 1;
    if (Rev)
    {
        I_Begin = Items.Count - 1;
        I_End = -1;
        I_Step = -1;
    }

    for (int i = I_Begin; i != I_End; i += I_Step)
    {
        if (Items[i].ActionType < 0)
        {
            CharPut(Items[i].X, Items[i].Y, Items[i].CopyItemObj(), true);
        }
        else
        {
            if (Rev)
            {
                if (Items[i].ActionType < 10)
                {
                    TextDelete(Items[i].X, Items[i].Y, Items[i].W, Items[i].H, Items[i].ActionType + 20);
                }
                else
                {
                    TextInsert(Items[i].X, Items[i].Y, Items[i].W, Items[i].H, Items[i].ActionType + 10);
                }
            }
            else
            {
                if (Items[i].ActionType < 10)
                {
                    TextInsert(Items[i].X, Items[i].Y, Items[i].W, Items[i].H, Items[i].ActionType + 20);
                }
                else
                {
                    TextDelete(Items[i].X, Items[i].Y, Items[i].W, Items[i].H, Items[i].ActionType + 10);
                }
            }
        }
    }

    CursorX = Params.CursorX;
    CursorY = Params.CursorY;
    CursorXSize = Params.CursorW;
    CursorYSize = Params.CursorH;
    CursorFontW = Params.FontW;
    CursorFontH = Params.FontH;
}

void EditorData::UndoRedoUndo()
{
    EditorUndoItem Item = EditorUndo_.get()->ItemUndo();
    if (Item.Valid)
    {
        bool ToggleDrawText_ = ToggleDrawText;
        bool ToggleDrawColo_ = ToggleDrawColo;
        ToggleDrawText = Item.ToggleDrawText;
        ToggleDrawColo = Item.ToggleDrawColo;
        UndoRedoAction(Item.EntryParamsOld, Item.ItemsOld, true);
        ToggleDrawText = ToggleDrawText_;
        ToggleDrawColo = ToggleDrawColo_;
        TextBuffer.TrimLines();
    }
}

void EditorData::UndoRedoRedo()
{
    EditorUndoItem Item = EditorUndo_.get()->ItemRedo();
    if (Item.Valid)
    {
        bool ToggleDrawText_ = ToggleDrawText;
        bool ToggleDrawColo_ = ToggleDrawColo;
        ToggleDrawText = Item.ToggleDrawText;
        ToggleDrawColo = Item.ToggleDrawColo;
        UndoRedoAction(Item.EntryParamsNew, Item.ItemsNew, false);
        ToggleDrawText = ToggleDrawText_;
        ToggleDrawColo = ToggleDrawColo_;
        TextBuffer.TrimLines();
    }
}
