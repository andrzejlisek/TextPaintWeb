#include "coreansi.h"

int CoreAnsi::AnsiGetFontW(int N, int X)
{
    if (AnsiGetFontSize(N) == 0)
    {
        return 0;
    }
    else
    {
        if ((X % 2) == 0)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
}

int CoreAnsi::AnsiGetFontH(int N)
{
    int S = AnsiGetFontSize(N);
    if (S == 2)
    {
        return 1;
    }
    if (S == 3)
    {
        return 1;
    }
    return 0;
}


int CoreAnsi::AnsiGetFontSize(int N)
{
    if (AnsiState_.__AnsiFontSizeAttr.Count > N)
    {
        return AnsiState_.__AnsiFontSizeAttr[N];
    }
    else
    {
        return 0;
    }
}

void CoreAnsi::AnsiClearFontSize(int FromPos)
{
    if (FromPos < 0)
    {
        AnsiState_.__AnsiFontSizeAttr.Clear();
    }
    else
    {
        if (FromPos < AnsiState_.__AnsiFontSizeAttr.Count)
        {
            AnsiState_.__AnsiFontSizeAttr.RemoveRange(FromPos, AnsiState_.__AnsiFontSizeAttr.Count - FromPos);
        }
    }
}

void CoreAnsi::AnsiSetFontSize(int N, int V, bool Rearrange)
{
    // 0 - Single-width
    // 1 - Double-width
    // 2 - Double-height, top half
    // 3 - Double-height, bottom half

    while (AnsiState_.__AnsiFontSizeAttr.Count <= N)
    {
        AnsiState_.__AnsiFontSizeAttr.Add(0);
    }
    int OldV = AnsiGetFontSize(N);
    AnsiState_.__AnsiFontSizeAttr[N] = V;

    if (Rearrange)
    {
        int FontH_ = 0;
        if (V == 2) FontH_ = 1;
        if (V == 3) FontH_ = 2;

        // Refresh font size
        if ((OldV > 0) && (V > 0))
        {
            for (int i = 0; i < AnsiMaxX; i++)
            {
                AnsiGet(i, N);
                AnsiChar(i, N, AnsiGetLast.Item_Char, AnsiGetLast.Item_ColorB, AnsiGetLast.Item_ColorF, AnsiGetLast.Item_FontW, FontH_, AnsiGetLast.Item_ColorA);
            }
            AnsiRepaintLine(N);
        }

        // Stretch text
        if ((OldV == 0) && (V > 0))
        {
            for (int i = (AnsiMaxX - 1); i >= 0; i--)
            {
                AnsiGet(i, N);
                AnsiChar(i * 2 + 0, N, AnsiGetLast.Item_Char, AnsiGetLast.Item_ColorB, AnsiGetLast.Item_ColorF, 1, FontH_, AnsiGetLast.Item_ColorA);
                AnsiChar(i * 2 + 1, N, AnsiGetLast.Item_Char, AnsiGetLast.Item_ColorB, AnsiGetLast.Item_ColorF, 2, FontH_, AnsiGetLast.Item_ColorA);
            }
            AnsiRepaintLine(N);
        }

        // Shrink text
        if ((OldV > 0) && (V == 0))
        {
            for (int i = 0; i < AnsiMaxX; i++)
            {
                AnsiGet(i * 2, N);
                AnsiChar(i, N, AnsiGetLast.Item_Char, AnsiGetLast.Item_ColorB, AnsiGetLast.Item_ColorF, 0, FontH_, AnsiGetLast.Item_ColorA);
            }
            AnsiRepaintLine(N);
        }
    }
}

void CoreAnsi::AnsiAttributesSave()
{
    TempMemo.Add(AnsiState_.__AnsiBack);
    TempMemo.Add(AnsiState_.__AnsiFore);
    TempMemo.Add(AnsiState_.__AnsiAttr);
}

void CoreAnsi::AnsiAttributesLoad()
{
    AnsiState_.__AnsiAttr = TempMemo.PopLast();
    AnsiState_.__AnsiFore = TempMemo.PopLast();
    AnsiState_.__AnsiBack = TempMemo.PopLast();
}

int CoreAnsi::AnsiColor16(int R, int G, int B)
{
    if ((R < 0) || (G < 0) || (B < 0))
    {
        return -1;
    }
    int RGBIdx = R * 65536 + G * 256 + B;
    if (AnsiColor16_.count(RGBIdx) > 0)
    {
        return AnsiColor16_[RGBIdx];
    }

    int RGBmax = std::max(std::max(R, G), B);
    int RGBmin = std::min(std::min(R, G), B);
    int RGBdiff = (RGBmax - RGBmin);
    int L = R + G + B;
    int H = 0;

    if (RGBdiff > 0)
    {
        if (RGBmax == R)
        {
            H = ((G - B) * 600) / RGBdiff;
            if (H < 0)
            {
                H += 3600;
            }
        }
        if (RGBmax == G)
        {
            H = ((B - R) * 600) / RGBdiff + 1200;
        }
        if (RGBmax == B)
        {
            H = ((R - G) * 600) / RGBdiff + 2400;
        }
    }
    int S = 0;
    int _128_3 = 128 * 3;
    int _255_3 = 255 * 3;
    if (L < _128_3)
    {
        if (L > 0)
        {
            S = ((3 * 20 * RGBdiff) / (L));
        }
    }
    else
    {
        if (L < _255_3)
        {
            S = ((3 * 20 * RGBdiff) / (_255_3 - L));
        }
    }

    int ColorIdx = -1;

    if (L < (ColorThresholdBlackWhite) * 3)
    {
        ColorIdx = 0;
    }
    else
    {
        if (L > ((255 - ColorThresholdBlackWhite) * 3))
        {
            ColorIdx = 15;
        }
        else
        {
            if (L < _128_3)
            {
                if (S <= ColorThresholdGray)
                {
                    ColorIdx = 8;
                }
                else
                {
                    if ((H >= 3300) || (H <= 300))
                    {
                        ColorIdx = 1;
                    }
                    if ((H > 300) && (H < 900))
                    {
                        ColorIdx = 3;
                    }
                    if ((H >= 900) && (H <= 1500))
                    {
                        ColorIdx = 2;
                    }
                    if ((H > 1500) && (H < 2100))
                    {
                        ColorIdx = 6;
                    }
                    if ((H >= 2100) && (H <= 2700))
                    {
                        ColorIdx = 4;
                    }
                    if ((H > 2700) && (H < 3300))
                    {
                        ColorIdx = 5;
                    }
                }
            }
            else
            {
                if (S <= ColorThresholdGray)
                {
                    ColorIdx = 7;
                }
                else
                {
                    if ((H > 3300) || (H < 300))
                    {
                        ColorIdx = 9;
                    }
                    if ((H >= 300) && (H <= 900))
                    {
                        ColorIdx = 11;
                    }
                    if ((H > 900) && (H < 1500))
                    {
                        ColorIdx = 10;
                    }
                    if ((H >= 1500) && (H <= 2100))
                    {
                        ColorIdx = 14;
                    }
                    if ((H > 2100) && (H < 2700))
                    {
                        ColorIdx = 12;
                    }
                    if ((H >= 2700) && (H <= 3300))
                    {
                        ColorIdx = 13;
                    }
                }
            }
        }
    }

    AnsiColor16_[RGBIdx] = ColorIdx;

    return ColorIdx;
}
