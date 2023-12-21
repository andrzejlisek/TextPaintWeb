var ScreenW = 0;
var ScreenH = 0;
var ScreenCellW = 8;
var ScreenCellH = 16;
var ScreenWW = 0;
var ScreenHH = 0;
let ScreenCursorX = 0;
let ScreenCursorY = 0;
let ScreenCursorSize = 0;
let ScreenBlinkState = false;
let ScreenCellFontX = [];
let ScreenCellFontY = [];

var ScreenObj = document.getElementById("Screen");
var ScreenCtx = ScreenObj.getContext('2d');
var ScreenDataC = [];
var ScreenDataB = [];
var ScreenDataF = [];
var ScreenDataA = [];
var ScreenDataW = [];
var ScreenDataH = [];
var ScreenDataB_ = [];
var ScreenDataF_ = [];
var ScreenData_0 = [];
var ScreenData_1 = [];
var ScreenData0;
var ScreenData1;
var ScreenDataChr0;
var ScreenDataChr1;


let ScreenLineOffsetVal = [];
let ScreenLineOffsetBlank = [];
let ScreenLineOffsetBack = [];
let ScreenLineOffsetFore = [];
let ScreenLineOffsetAttr = [];

var ScreenPalette1R = [];
var ScreenPalette1G = [];
var ScreenPalette1B = [];
var ScreenPalette2R = [];
var ScreenPalette2G = [];
var ScreenPalette2B = [];

var ScreenDefaultBack = 0;
var ScreenDefaultFore = 7;


let ScreenCellH2 = 8;



ScreenGlyphBuf0 = {};
ScreenGlyphBuf1 = {};


let ScreenSET_ANSIIgnoreConcealed = false;
let ScreenSET_ANSIReverseMode = 0;
let ScreenSET_ANSIColors = true;
let ScreenSET_ANSIColorBlink = false;
let ScreenSET_ANSIColorBold = false
let ScreenSET_Blink = 1;
let ScreenAttrB = true;
let ScreenAttrI = true;
let ScreenAttrU = true;
let ScreenAttrS = true;

function ScreenSetDisplayConfig(Repaint)
{
    ScreenCursorSteady = ConfigFileB("WinSteadyCursor");
    ScreenSET_ANSIIgnoreConcealed = ConfigFileB("ANSIIgnoreConcealed");
    ScreenSET_ANSIReverseMode = ConfigFileI("ANSIReverseMode");
    ScreenSET_ANSIColors = ConfigFileB("ANSIColors");
    ScreenSET_ANSIColorBlink = ConfigFileB("ANSIColorBlink");
    ScreenSET_ANSIColorBold = ConfigFileB("ANSIColorBold");

    ScreenAttrB = ((ConfigFileI("DisplayAttrib") & 1) > 0) ? true : false;
    ScreenAttrI = ((ConfigFileI("DisplayAttrib") & 2) > 0) ? true : false;
    ScreenAttrU = ((ConfigFileI("DisplayAttrib") & 4) > 0) ? true : false;
    ScreenAttrS = ((ConfigFileI("DisplayAttrib") & 8) > 0) ? true : false;

    ScreenSET_Blink = ConfigFileI("DisplayBlink");
    
    ScreenGlyphBuf0 = {};
    ScreenGlyphBuf1 = {};
    
    if (Repaint)
    {
        ScreenRepaint();
    }
}


let ScreenCalcColor_Back = 0;
let ScreenCalcColor_Fore = 7;
let ScreenCalcColor_Temp = 0;
function ScreenCalcColor(Back, Fore, Attr)
{
    if (ScreenSET_ANSIColors)
    {
        ScreenCalcColor_Back = (Back >= 0) ? Back : ScreenDefaultBack;
        ScreenCalcColor_Fore = (Fore >= 0) ? Fore : ScreenDefaultFore;
    }
    else
    {
        ScreenCalcColor_Back = ScreenDefaultBack;
        ScreenCalcColor_Fore = ScreenDefaultFore;
    }
    
    if (Attr < 0)
    {
        return;
    }
    
    if (ScreenSET_ANSIReverseMode == 1)
    {
        if ((Attr & 16) != 0)
        {
            ScreenCalcColor_Temp = ScreenCalcColor_Fore;
            ScreenCalcColor_Fore = ScreenCalcColor_Back;
            ScreenCalcColor_Back = ScreenCalcColor_Temp;
        }
    }

    if (((Attr & 1) != 0) && (ScreenSET_ANSIColorBold))
    {
        if (ScreenCalcColor_Fore < 8)
        {
            if ((ScreenCalcColor_Fore >= 0) && (ScreenCalcColor_Fore < 8))
            {
                ScreenCalcColor_Fore += 8;
            }
        }
        else
        {
            if ((ScreenCalcColor_Fore >= 8) && (ScreenCalcColor_Fore < 16))
            {
                ScreenCalcColor_Fore -= 8;
            }
        }
    }

    if (((Attr & 8) != 0) && (ScreenSET_ANSIColorBlink))
    {
        if (ScreenCalcColor_Back < 8)
        {
            if ((ScreenCalcColor_Back >= 0) && (ScreenCalcColor_Back < 8))
            {
                ScreenCalcColor_Back += 8;
            }
        }
        else
        {
            if ((ScreenCalcColor_Back >= 8) && (ScreenCalcColor_Back < 16))
            {
                ScreenCalcColor_Back -= 8;
            }
        }
    }

    if (ScreenSET_ANSIReverseMode == 2)
    {
        if ((Attr & 16) != 0)
        {
            ScreenCalcColor_Temp = ScreenCalcColor_Fore;
            ScreenCalcColor_Fore = ScreenCalcColor_Back;
            ScreenCalcColor_Back = ScreenCalcColor_Temp;
        }
    }

    if ((Attr & 128) > 0)
    {
        ScreenCalcColor_Temp = ScreenCalcColor_Fore;
        ScreenCalcColor_Fore = ScreenCalcColor_Back;
        ScreenCalcColor_Back = ScreenCalcColor_Temp;
    }

    if (((Attr & 32) != 0) && (!ScreenSET_ANSIIgnoreConcealed))
    {
        ScreenCalcColor_Fore = ScreenCalcColor_Back;
    }
}



function ScreenCharBlank(Buf, ScrIdx)
{
    let Chr = 32;
    let Back = ScreenDataB[ScrIdx];
    let Fore = ScreenDataF[ScrIdx];
    let Back_ = ScreenDataB_[ScrIdx];
    let Fore_ = ScreenDataF_[ScrIdx];
    let Attr = 0;
    let FontW = 0;
    let FontH = 0;

    let CharIdx = (Chr < 65536) ? ((Chr << 16) + (Back << 12) + (Fore << 8) + (Attr)) : 0;
    let CharData0 = ScreenGlyphBuf0[CharIdx];
    let CharData1 = ScreenGlyphBuf1[CharIdx];
    
    if ((CharIdx == 0) || (!CharData0))
    {
        let AttrB = ScreenAttrB ? Attr & 1 : 0;
        let AttrI = ScreenAttrI ? Attr & 2 : 0;
        let AttrU = ScreenAttrU ? Attr & 4 : 0;
        let AttrK = Attr & 8;
        let AttrR = Attr & 16;
        let AttrC = Attr & 32;
        let AttrS = ScreenAttrS ? Attr & 64 : 0;
        let AttrN = Attr & 128;
    
        let CharVals = ScreenFont1.GetGlyph(Chr);
        CharData0 = ScreenCtx.createImageData(ScreenCellW, ScreenCellH);
        CharData1 = ScreenCtx.createImageData(ScreenCellW, ScreenCellH);
        let N = ScreenCellW * ScreenCellH;
        let Ptr4 = 0;
        let PxlOn = false;
        let I = ScreenCellFontY[FontH][ScreenCellH];
        let ScreenCellFontX_ = ScreenCellFontX[FontW];
        let ScreenCellFontY_ = ScreenCellFontY[FontH];
        for (let Y = 0; Y < ScreenCellH; Y++)
        {
            for (let X = 0; X < ScreenCellW; X++)
            {
                PxlOn = false;
                if ((AttrU) && (Y == (ScreenCellH - 1))) { PxlOn = true; }
                if ((AttrS) && (Y == (ScreenCellH2 - 1))) { PxlOn = true; }
                
                if (PxlOn)
                {
                    CharData0.data[Ptr4 + 0] = ScreenPalette1R[Fore_];
                    CharData0.data[Ptr4 + 1] = ScreenPalette1G[Fore_];
                    CharData0.data[Ptr4 + 2] = ScreenPalette1B[Fore_];
                    if (AttrK && (ScreenSET_Blink > 0))
                    {
                        if (ScreenSET_Blink == 1)
                        {
                            CharData1.data[Ptr4 + 0] = ScreenPalette2R[Fore_];
                            CharData1.data[Ptr4 + 1] = ScreenPalette2G[Fore_];
                            CharData1.data[Ptr4 + 2] = ScreenPalette2B[Fore_];
                        }
                        else
                        {
                            CharData1.data[Ptr4 + 0] = ScreenPalette1R[Back_];
                            CharData1.data[Ptr4 + 1] = ScreenPalette1G[Back_];
                            CharData1.data[Ptr4 + 2] = ScreenPalette1B[Back_];
                        }
                    }
                    else
                    {
                        CharData1.data[Ptr4 + 0] = ScreenPalette1R[Fore_];
                        CharData1.data[Ptr4 + 1] = ScreenPalette1G[Fore_];
                        CharData1.data[Ptr4 + 2] = ScreenPalette1B[Fore_];
                    }
                }
                else
                {
                    CharData0.data[Ptr4 + 0] = ScreenPalette1R[Back_];
                    CharData0.data[Ptr4 + 1] = ScreenPalette1G[Back_];
                    CharData0.data[Ptr4 + 2] = ScreenPalette1B[Back_];
                    if (AttrK && (ScreenSET_Blink == 1))
                    {
                        CharData1.data[Ptr4 + 0] = ScreenPalette2R[Back_];
                        CharData1.data[Ptr4 + 1] = ScreenPalette2G[Back_];
                        CharData1.data[Ptr4 + 2] = ScreenPalette2B[Back_];
                    }
                    else
                    {
                        CharData1.data[Ptr4 + 0] = ScreenPalette1R[Back_];
                        CharData1.data[Ptr4 + 1] = ScreenPalette1G[Back_];
                        CharData1.data[Ptr4 + 2] = ScreenPalette1B[Back_];
                    }
                }
                CharData0.data[Ptr4 + 3] = 255;
                CharData1.data[Ptr4 + 3] = 255;
                Ptr4 += 4;
            }
            I += ScreenCellFontY_[Y];
        }
        ScreenGlyphBuf0[CharIdx] = CharData0;
        ScreenGlyphBuf1[CharIdx] = CharData1;
    }
    
    if (Buf == 0)
    {
        return CharData0;
    }
    else
    {
        return CharData1;
    }
}

function ScreenCharGlyph(Buf, ScrIdx)
{
    let Chr = ScreenDataC[ScrIdx];
    let Back = ScreenDataB[ScrIdx];
    let Fore = ScreenDataF[ScrIdx];
    let Back_ = ScreenDataB_[ScrIdx];
    let Fore_ = ScreenDataF_[ScrIdx];
    let Attr = ScreenDataA[ScrIdx];
    let FontW = ScreenDataW[ScrIdx];
    let FontH = ScreenDataH[ScrIdx];

    let CharIdx = (Chr < 65536) ? ((Chr << 16) + (Back << 12) + (Fore << 8) + (Attr)) : 0;
    let CharData0 = ScreenGlyphBuf0[CharIdx];
    let CharData1 = ScreenGlyphBuf1[CharIdx];
    
    if ((FontW > 0) || (FontH > 0) || (CharIdx == 0) || (!CharData0))
    {
        let AttrB = ScreenAttrB ? Attr & 1 : 0;
        let AttrI = ScreenAttrI ? Attr & 2 : 0;
        let AttrU = ScreenAttrU ? Attr & 4 : 0;
        let AttrK = Attr & 8;
        let AttrR = Attr & 16;
        let AttrC = Attr & 32;
        let AttrS = ScreenAttrS ? Attr & 64 : 0;
        let AttrN = Attr & 128;
    
        let CharVals = ScreenFont1.GetGlyph(Chr);
        CharData0 = ScreenCtx.createImageData(ScreenCellW, ScreenCellH);
        CharData1 = ScreenCtx.createImageData(ScreenCellW, ScreenCellH);
        let N = ScreenCellW * ScreenCellH;
        let Ptr4 = 0;
        let PxlOn = false;
        let I = ScreenCellFontY[FontH][ScreenCellH];
        let ScreenCellFontX_ = ScreenCellFontX[FontW];
        let ScreenCellFontY_ = ScreenCellFontY[FontH];
        for (let Y = 0; Y < ScreenCellH; Y++)
        {
            for (let X = 0; X < ScreenCellW; X++)
            {
                PxlOn = false;
                if (AttrI && (Y >= ScreenCellH2))
                {
                    if (CharVals[I + ScreenCellFontX_[X + 2]])
                    {
                        PxlOn = true;
                    }
                    if (AttrB && CharVals[I])
                    {
                        PxlOn = true;
                    }
                }
                else
                {
                    if (CharVals[I + ScreenCellFontX_[X + 1]])
                    {
                        PxlOn = true;
                    }
                    if (AttrB && CharVals[I + ScreenCellFontX_[X]])
                    {
                        PxlOn = true;
                    }
                }
                if ((AttrU) && (Y == (ScreenCellH - 1))) { PxlOn = true; }
                if ((AttrS) && (Y == (ScreenCellH2 - 1))) { PxlOn = true; }
                
                if (PxlOn)
                {
                    CharData0.data[Ptr4 + 0] = ScreenPalette1R[Fore_];
                    CharData0.data[Ptr4 + 1] = ScreenPalette1G[Fore_];
                    CharData0.data[Ptr4 + 2] = ScreenPalette1B[Fore_];
                    if (AttrK && (ScreenSET_Blink > 0))
                    {
                        if (ScreenSET_Blink == 1)
                        {
                            CharData1.data[Ptr4 + 0] = ScreenPalette2R[Fore_];
                            CharData1.data[Ptr4 + 1] = ScreenPalette2G[Fore_];
                            CharData1.data[Ptr4 + 2] = ScreenPalette2B[Fore_];
                        }
                        else
                        {
                            CharData1.data[Ptr4 + 0] = ScreenPalette1R[Back_];
                            CharData1.data[Ptr4 + 1] = ScreenPalette1G[Back_];
                            CharData1.data[Ptr4 + 2] = ScreenPalette1B[Back_];
                        }
                    }
                    else
                    {
                        CharData1.data[Ptr4 + 0] = ScreenPalette1R[Fore_];
                        CharData1.data[Ptr4 + 1] = ScreenPalette1G[Fore_];
                        CharData1.data[Ptr4 + 2] = ScreenPalette1B[Fore_];
                    }
                }
                else
                {
                    CharData0.data[Ptr4 + 0] = ScreenPalette1R[Back_];
                    CharData0.data[Ptr4 + 1] = ScreenPalette1G[Back_];
                    CharData0.data[Ptr4 + 2] = ScreenPalette1B[Back_];
                    if (AttrK && (ScreenSET_Blink == 1))
                    {
                        CharData1.data[Ptr4 + 0] = ScreenPalette2R[Back_];
                        CharData1.data[Ptr4 + 1] = ScreenPalette2G[Back_];
                        CharData1.data[Ptr4 + 2] = ScreenPalette2B[Back_];
                    }
                    else
                    {
                        CharData1.data[Ptr4 + 0] = ScreenPalette1R[Back_];
                        CharData1.data[Ptr4 + 1] = ScreenPalette1G[Back_];
                        CharData1.data[Ptr4 + 2] = ScreenPalette1B[Back_];
                    }
                }
                CharData0.data[Ptr4 + 3] = 255;
                CharData1.data[Ptr4 + 3] = 255;
                Ptr4 += 4;
            }
            I += ScreenCellFontY_[Y];
        }
        if ((FontW == 0) && (FontH == 0))
        {
            ScreenGlyphBuf0[CharIdx] = CharData0;
            ScreenGlyphBuf1[CharIdx] = CharData1;
        }
    }
    
    if (Buf == 0)
    {
        return CharData0;
    }
    else
    {
        return CharData1;
    }
}

            
function ScreenChar(X, Y, Chr, Back, Fore, Attr, FontW, FontH)
{
    if (X < 0) { return; }
    if (Y < 0) { return; }
    if (X >= ScreenW) { return; }
    if (Y >= ScreenH) { return; }
    
    let ScrIdx = Y * ScreenW + X;
    let CharIdx = (Chr < 65536) ? ((Chr << 16) + (Back << 12) + (Fore << 8) + (Attr)) : 0;
    let CharIdx_ = (FontW << 8) + (FontH);
    
    if ((CharIdx != 0) && (ScreenData_0[ScrIdx] == CharIdx) && (ScreenData_1[ScrIdx] == CharIdx_))
    {
        return;
    }
    
    ScreenCalcColor(Back, Fore, Attr);
    ScreenData_0[ScrIdx] = CharIdx;
    ScreenData_1[ScrIdx] = CharIdx_;
    ScreenDataC[ScrIdx] = Chr;
    ScreenDataB[ScrIdx] = Back;
    ScreenDataF[ScrIdx] = Fore;
    ScreenDataB_[ScrIdx] = ScreenCalcColor_Back;
    ScreenDataF_[ScrIdx] = ScreenCalcColor_Fore;
    ScreenDataA[ScrIdx] = Attr;
    ScreenDataW[ScrIdx] = FontW;
    ScreenDataH[ScrIdx] = FontH;
    
    let CharData0 = ScreenCharGlyph(0, ScrIdx);
    let CharData1 = ScreenCharGlyph(1, ScrIdx);
    let CharData0_;
    let CharData1_;
    
    let OffsetMode = 0;
    let OffsetVal = 0;
    let OffsetPtr = 0;
    let OffsetOtherHalf = false;
    if (ScreenLineOffsetVal[Y] > 0)
    {
        OffsetMode = 1;
        OffsetVal = ScreenLineOffsetVal[Y];
        OffsetPtr = (OffsetVal * ScreenCellW) << 2;
        if ((Y < (ScreenH - 1)) && (ScreenLineOffsetBlank[Y] == 0))
        {
            OffsetOtherHalf = true;
        }
    }
    if (ScreenLineOffsetVal[Y] < 0)
    {
        OffsetMode = 2;
        OffsetVal = 0 - ScreenLineOffsetVal[Y];
        OffsetPtr = ((ScreenCellH - OffsetVal) * ScreenCellW) << 2;
        if ((Y > 0) && (ScreenLineOffsetBlank[Y] == 0))
        {
            OffsetOtherHalf = true;
        }
    }

    let PtrI = 0;
    let PtrO = 0;
    
    switch (OffsetMode)
    {
        default:
            PtrI = 0;
            for (let Y_ = 0; Y_ < ScreenCellH; Y_++)
            {
                for (let X_ = 0; X_ < ScreenCellW; X_++)
                {
                    ScreenDataChr0.data[PtrI + 0] = CharData0.data[PtrI + 0];
                    ScreenDataChr0.data[PtrI + 1] = CharData0.data[PtrI + 1];
                    ScreenDataChr0.data[PtrI + 2] = CharData0.data[PtrI + 2];
                    ScreenDataChr1.data[PtrI + 0] = CharData1.data[PtrI + 0];
                    ScreenDataChr1.data[PtrI + 1] = CharData1.data[PtrI + 1];
                    ScreenDataChr1.data[PtrI + 2] = CharData1.data[PtrI + 2];
                    PtrI += 4;
                }
            }
            break;
        case 1:

            PtrO = 0;

            // Current line
            PtrI = OffsetPtr;
            for (let Y_ = 0; Y_ < ScreenCellH - OffsetVal; Y_++)
            {
                for (let X_ = 0; X_ < ScreenCellW; X_++)
                {
                    ScreenDataChr0.data[PtrO + 0] = CharData0.data[PtrI + 0];
                    ScreenDataChr0.data[PtrO + 1] = CharData0.data[PtrI + 1];
                    ScreenDataChr0.data[PtrO + 2] = CharData0.data[PtrI + 2];
                    ScreenDataChr1.data[PtrO + 0] = CharData1.data[PtrI + 0];
                    ScreenDataChr1.data[PtrO + 1] = CharData1.data[PtrI + 1];
                    ScreenDataChr1.data[PtrO + 2] = CharData1.data[PtrI + 2];
                    PtrI += 4;
                    PtrO += 4;
                }
            }

            // Blank or lower line
            if (OffsetOtherHalf)
            {
                CharData0_ = ScreenCharGlyph(0, ScrIdx + ScreenW);
                CharData1_ = ScreenCharGlyph(1, ScrIdx + ScreenW);
            }
            else
            {
                CharData0_ = ScreenCharBlank(0, ScrIdx);
                CharData1_ = ScreenCharBlank(1, ScrIdx);
            }
            PtrI = 0;
            for (let Y_ = ScreenCellH - OffsetVal; Y_ < ScreenCellH; Y_++)
            {
                for (let X_ = 0; X_ < ScreenCellW; X_++)
                {
                    ScreenDataChr0.data[PtrO + 0] = CharData0_.data[PtrI + 0];
                    ScreenDataChr0.data[PtrO + 1] = CharData0_.data[PtrI + 1];
                    ScreenDataChr0.data[PtrO + 2] = CharData0_.data[PtrI + 2];
                    ScreenDataChr1.data[PtrO + 0] = CharData1_.data[PtrI + 0];
                    ScreenDataChr1.data[PtrO + 1] = CharData1_.data[PtrI + 1];
                    ScreenDataChr1.data[PtrO + 2] = CharData1_.data[PtrI + 2];
                    PtrI += 4;
                    PtrO += 4;
                }
            }

            break;
        case 2:

            PtrO = 0;

            // Blank or upper line
            if (OffsetOtherHalf)
            {
                CharData0_ = ScreenCharGlyph(0, ScrIdx - ScreenW);
                CharData1_ = ScreenCharGlyph(1, ScrIdx - ScreenW);
            }
            else
            {
                CharData0_ = ScreenCharBlank(0, ScrIdx);
                CharData1_ = ScreenCharBlank(1, ScrIdx);
            }
            PtrI = OffsetPtr;
            for (let Y_ = ScreenCellH - OffsetVal; Y_ < ScreenCellH; Y_++)
            {
                for (let X_ = 0; X_ < ScreenCellW; X_++)
                {
                    ScreenDataChr0.data[PtrO + 0] = CharData0_.data[PtrI + 0];
                    ScreenDataChr0.data[PtrO + 1] = CharData0_.data[PtrI + 1];
                    ScreenDataChr0.data[PtrO + 2] = CharData0_.data[PtrI + 2];
                    ScreenDataChr1.data[PtrO + 0] = CharData1_.data[PtrI + 0];
                    ScreenDataChr1.data[PtrO + 1] = CharData1_.data[PtrI + 1];
                    ScreenDataChr1.data[PtrO + 2] = CharData1_.data[PtrI + 2];
                    PtrI += 4;
                    PtrO += 4;
                }

            }

            // Current line
            PtrI = 0;
            for (let Y_ = 0; Y_ < ScreenCellH - OffsetVal; Y_++)
            {
                for (let X_ = 0; X_ < ScreenCellW; X_++)
                {
                    ScreenDataChr0.data[PtrO + 0] = CharData0.data[PtrI + 0];
                    ScreenDataChr0.data[PtrO + 1] = CharData0.data[PtrI + 1];
                    ScreenDataChr0.data[PtrO + 2] = CharData0.data[PtrI + 2];
                    ScreenDataChr1.data[PtrO + 0] = CharData1.data[PtrI + 0];
                    ScreenDataChr1.data[PtrO + 1] = CharData1.data[PtrI + 1];
                    ScreenDataChr1.data[PtrO + 2] = CharData1.data[PtrI + 2];
                    PtrI += 4;
                    PtrO += 4;
                }
            }

            break;
    }


    // Drawing character on the screen memory
    PtrI = 0;
    PtrO = (((Y * ScreenCellH) * ScreenWW) + (X * ScreenCellW)) << 2;
    for (let Y_ = 0; Y_ < ScreenCellH; Y_++)
    {
        for (let X_ = 0; X_ < ScreenCellW; X_++)
        {
            ScreenData0.data[PtrO + 0] = ScreenDataChr0.data[PtrI + 0];
            ScreenData0.data[PtrO + 1] = ScreenDataChr0.data[PtrI + 1];
            ScreenData0.data[PtrO + 2] = ScreenDataChr0.data[PtrI + 2];
            ScreenData1.data[PtrO + 0] = ScreenDataChr1.data[PtrI + 0];
            ScreenData1.data[PtrO + 1] = ScreenDataChr1.data[PtrI + 1];
            ScreenData1.data[PtrO + 2] = ScreenDataChr1.data[PtrI + 2];

            PtrI += 4;
            PtrO += 4;
        }
        PtrO = PtrO - (ScreenCellW << 2) + ((ScreenWW) << 2);
    }

    // Drawing character on the screen directly
    if (ScreenBlinkState)
    {
        ScreenCtx.putImageData(ScreenDataChr1, X * ScreenCellW, Y * ScreenCellH);
    }
    else
    {
        ScreenCtx.putImageData(ScreenDataChr0, X * ScreenCellW, Y * ScreenCellH);
    }
    if ((X == ScreenCursorX) && (Y == ScreenCursorY))
    {
        ScreenDrawCursorWork();
    }
}

function ScreenRepaintChar(X, Y)
{
    let Ptr = Y * ScreenW + X;
    ScreenData_0[Ptr] = -1;
    ScreenChar(X, Y, ScreenDataC[Ptr], ScreenDataB[Ptr], ScreenDataF[Ptr], ScreenDataA[Ptr], ScreenDataW[Ptr], ScreenDataH[Ptr]);
}

function ScreenRepaint()
{
    let Ptr = 0;
    for (let Y = 0; Y < ScreenH; Y++)
    {
        for (let X = 0; X < ScreenW; X++)
        {
            ScreenData_0[Ptr] = -1;
            ScreenChar(X, Y, ScreenDataC[Ptr], ScreenDataB[Ptr], ScreenDataF[Ptr], ScreenDataA[Ptr], ScreenDataW[Ptr], ScreenDataH[Ptr]);
            Ptr++;
        }
    }
}

function ScreenResize(NewW, NewH)
{
    let N = NewW * NewH;
    if ((ScreenW != NewW) || (ScreenH != NewH))
    {
        let ScreenDataC_ = ScreenDataC;
        let ScreenDataB_ = ScreenDataB;
        let ScreenDataF_ = ScreenDataF;
        let ScreenDataA_ = ScreenDataA;
        let ScreenDataW_ = ScreenDataW;
        let ScreenDataH_ = ScreenDataH;

        ScreenDataC = [];
        ScreenDataB = [];
        ScreenDataF = [];
        ScreenDataA = [];
        ScreenDataW = [];
        ScreenDataH = [];
        ScreenData_0 = [];
        ScreenData_1 = [];

        for (let I = 0; I < N; I++)
        {
            ScreenDataC[I] = 32;
            ScreenDataB[I] = ScreenDefaultBack;
            ScreenDataF[I] = ScreenDefaultFore;
            ScreenDataA[I] = 0;
            ScreenDataW[I] = 0;
            ScreenDataH[I] = 0;
            ScreenData_0[I] = -1;
            ScreenData_1[I] = -1;
        }
        
        if (ScreenDataC.length > 0)
        {
            let IdxW = ScreenW < NewW ? ScreenW : NewW;
            let IdxH = ScreenH < NewH ? ScreenH : NewH;
            
            let PtrI = 0;
            let PtrO = 0;
            for (let Y = 0; Y < IdxH; Y++)
            {
                for (let X = 0; X < IdxW; X++)
                {
                    ScreenDataC[PtrO] = ScreenDataC_[PtrI];
                    ScreenDataB[PtrO] = ScreenDataB_[PtrI];
                    ScreenDataF[PtrO] = ScreenDataF_[PtrI];
                    ScreenDataA[PtrO] = ScreenDataA_[PtrI];
                    ScreenDataW[PtrO] = ScreenDataW_[PtrI];
                    ScreenDataH[PtrO] = ScreenDataH_[PtrI];
                
                    PtrI++;
                    PtrO++;
                }
                PtrI = PtrI - IdxW + ScreenW;
                PtrO = PtrO - IdxW + NewW;
            }
        }
        
        ScreenLineOffsetVal = [];
        ScreenLineOffsetBlank = [];
        ScreenLineOffsetBack = [];  
        ScreenLineOffsetFore = [];
        ScreenLineOffsetAttr = [];
        for (let I = 0; I < NewH; I++)
        {
            ScreenLineOffsetVal[I] = 0;
            ScreenLineOffsetBlank[I] = 0;
            ScreenLineOffsetBack[I] = ScreenDefaultBack;
            ScreenLineOffsetFore[I] = ScreenDefaultFore;
            ScreenLineOffsetAttr[I] = 0;
        }
    }
    
    ScreenW = NewW;
    ScreenH = NewH;
    ScreenWW = ScreenW * ScreenCellW;
    ScreenHH = ScreenH * ScreenCellH;
    ScreenObj.width = ScreenWW;
    ScreenObj.height = ScreenHH;

    ScreenCtx = ScreenObj.getContext('2d');
    ScreenData0 = ScreenCtx.getImageData(0, 0, ScreenWW, ScreenHH);
    ScreenData1 = ScreenCtx.getImageData(0, 0, ScreenWW, ScreenHH);
    N = ScreenWW * ScreenHH;
    for (let I = 0; I < N; I++)
    {
        ScreenData0.data[I * 4 + 0] = 0;
        ScreenData0.data[I * 4 + 1] = 0;
        ScreenData0.data[I * 4 + 2] = 0;
        ScreenData0.data[I * 4 + 3] = 255;
        ScreenData1.data[I * 4 + 0] = 0;
        ScreenData1.data[I * 4 + 1] = 0;
        ScreenData1.data[I * 4 + 2] = 0;
        ScreenData1.data[I * 4 + 3] = 255;
    }

    ScreenDataChr0 = ScreenCtx.createImageData(ScreenCellW, ScreenCellH);
    ScreenDataChr1 = ScreenCtx.createImageData(ScreenCellW, ScreenCellH);
    N = ScreenCellW * ScreenCellH;
    for (let I = 0; I < N; I++)
    {
        ScreenDataChr0.data[I * 4 + 3] = 255;
        ScreenDataChr1.data[I * 4 + 3] = 255;
    }
    
    ScreenClear(ScreenDefaultBack, ScreenDefaultFore);
    //ScreenRepaint();
}

function ScreenTest()
{
    let Ptr = 0;
    for (let Y = 0; Y < 16; Y++)
    {
        for (let X = 0; X < 16; X++)
        {
            ScreenChar(X * 2 + 1, Y, Ptr, X, Y, 0, 0, 0);

            ScreenChar(X * 2 + 41, Y, Ptr + 256, X, Y, 0, 0, 0);
            Ptr++;
        }
    }
}

function ScreenClear(ColorBack, ColorFore)
{
    for (let Y = 0; Y < ScreenH; Y++)
    {
        ScreenLineOffset(Y, 0, 0, ColorBack, ColorFore, 0);
        for (let X = 0; X < ScreenW; X++)
        {
            ScreenChar(X, Y, 32, ColorBack, ColorFore, 0, 0, 0);
        }
    }
}

function ScreenCursorMove(X, Y)
{
    if ((ScreenCursorX != X) || (ScreenCursorY != Y))
    {
        ScreenDrawCursor(false);
    }
    ScreenCursorX = X;
    ScreenCursorY = Y;
}

function ScreenTextMove(X1, Y1, X2, Y2, W, H)
{
    let YStart = Y2;
    let YStop = Y2 + H;
    let YAdv = 1;
    let YDelta = Y1 - Y2;

    if (Y1 < Y2)
    {
        YStart = Y1 + H;
        YStop = Y1;
        YAdv = -1;
        YDelta = Y1 - Y2;
    }

    let XStart = X2;
    let XStop = X2 + W;
    let XAdv = 1;
    let XDelta = X1 - X2;

    if (X1 < X2)
    {
        XStart = X1 + W;
        XStop = X1;
        XAdv = -1;
        XDelta = X1 - X2;
    }

    for (let Y = YStart; Y != YStop; Y += YAdv)
    {
        for (let X = XStart; X != XStop; X += XAdv)
        {
            let ScrIdx = (Y + YDelta) * ScreenW + (X + XDelta);

            let _C = ScreenDataC[ScrIdx];
            let _B = ScreenDataB[ScrIdx];
            let _F = ScreenDataF[ScrIdx];
            let _A = ScreenDataA[ScrIdx];
            let _W = ScreenDataW[ScrIdx];
            let _H = ScreenDataH[ScrIdx];
            
            ScreenChar(X, Y, _C, _B, _F, _A, _W, _H);
        }
    }
}

function ScreenLineOffset(Y, Offset, Blank, ColorBack, ColorFore, ColorAttr)
{
    if (Y < 0) { return; }
    if (Y >= ScreenH) { return; }
    let OldOffset = ScreenLineOffsetVal[Y]
    switch (ScreenCellH)
    {
        case 8:
            ScreenLineOffsetVal[Y] = Offset;
            break;
        case 16:
            ScreenLineOffsetVal[Y] = Offset << 1;
            break;
        case 24:
            ScreenLineOffsetVal[Y] = Offset + Offset + Offset;
            break;
        case 32:
            ScreenLineOffsetVal[Y] = Offset << 2;
            break;
        default:
            ScreenLineOffsetVal[Y] = Math.floor((Offset * ScreenCellH) / 8);
            break;
    }
    ScreenLineOffsetBlank[Y] = Blank;
    ScreenLineOffsetBack[Y] = ColorBack;
    ScreenLineOffsetFore[Y] = ColorFore;
    ScreenLineOffsetAttr[Y] = ColorAttr;
    if (OldOffset != ScreenLineOffset[Y])
    {
        for (let X = 0; X < ScreenW; X++)
        {
            ScreenRepaintChar(X, Y);
        }
    }
}

function ScreenDrawBlink(State)
{
    ScreenBlinkState = State;
    if (ScreenBlinkState)
    {
        ScreenCtx.putImageData(ScreenData1, 0, 0);
    }
    else
    {
        ScreenCtx.putImageData(ScreenData0, 0, 0);
    }
}

let CursorState = false;

function ScreenDrawCursorWork()
{
    if (CursorState)
    {
        let CursorX_ = ScreenCursorX * ScreenCellW;
        let CursorY_ = ScreenCursorY * ScreenCellH + (ScreenCellH - ScreenCursorSize);
        let CursorData = ScreenCtx.getImageData(CursorX_, CursorY_, ScreenCellW, ScreenCursorSize);
        let Ptr1 = 0;
        let Ptr2 = (CursorY_ * (ScreenWW)) + CursorX_ << 2;
        let PtrX = ScreenCursorY * ScreenW + ScreenCursorX;
        for (let I = 0; I < ScreenCursorSize; I++)
        {
            for (let II = 0; II < ScreenCellW; II++)
            {
                CursorData.data[Ptr1 + 0] = ScreenPalette1R[ScreenDataF_[PtrX]];
                CursorData.data[Ptr1 + 1] = ScreenPalette1G[ScreenDataF_[PtrX]];
                CursorData.data[Ptr1 + 2] = ScreenPalette1B[ScreenDataF_[PtrX]];
                ScreenData0.data[Ptr2 + 0] = ScreenPalette1R[ScreenDataF_[PtrX]];
                ScreenData0.data[Ptr2 + 1] = ScreenPalette1R[ScreenDataF_[PtrX]];

                ScreenData0.data[Ptr2 + 2] = ScreenPalette1R[ScreenDataF_[PtrX]];
                ScreenData1.data[Ptr2 + 0] = ScreenPalette1G[ScreenDataF_[PtrX]];

                ScreenData1.data[Ptr2 + 1] = ScreenPalette1G[ScreenDataF_[PtrX]];
                ScreenData1.data[Ptr2 + 2] = ScreenPalette1G[ScreenDataF_[PtrX]];
                Ptr1 += 4;
                Ptr2 += 4;
            }
            Ptr2 = Ptr2 - (ScreenCellW << 2) + ((ScreenWW) << 2);
        }
        ScreenCtx.putImageData(CursorData, CursorX_, CursorY_);
    }
}

function ScreenDrawCursor(State)
{
    if (CursorState && (!State))
    {
        CursorState = false;
        ScreenRepaintChar(ScreenCursorX, ScreenCursorY);
        return;
    }
    if ((!CursorState) && State)
    {
        CursorState = true;
        ScreenDrawCursorWork();
        return;
    }
}



let ScreenFont1;
let ScreenFont2;


function ScreenInit1()
{
    KeybTouch = ConfigFileI("WinTouchScreen");

    ScreenPalette1R = [0x00, 0xAA, 0x00, 0xAA, 0x00, 0xAA, 0x00, 0xAA, 0x55, 0xFF, 0x55, 0xFF, 0x55, 0xFF, 0x55, 0xFF];
    ScreenPalette1G = [0x00, 0x00, 0xAA, 0xAA, 0x00, 0x00, 0xAA, 0xAA, 0x55, 0x55, 0xFF, 0xFF, 0x55, 0x55, 0xFF, 0xFF];
    ScreenPalette1B = [0x00, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 0xFF, 0xFF, 0xFF, 0xFF];
    ScreenPalette2R = [0x00, 0x71, 0x00, 0x71, 0x00, 0x71, 0x00, 0x71, 0x39, 0xAA, 0x39, 0xAA, 0x39, 0xAA, 0x39, 0xAA];
    ScreenPalette2G = [0x00, 0x00, 0x71, 0x71, 0x00, 0x00, 0x71, 0x71, 0x39, 0x39, 0xAA, 0xAA, 0x39, 0x39, 0xAA, 0xAA];
    ScreenPalette2B = [0x00, 0x00, 0x00, 0x00, 0x71, 0x71, 0x71, 0x71, 0x39, 0x39, 0x39, 0x39, 0xAA, 0xAA, 0xAA, 0xAA];
    ScreenFont1 = new ScreenFont(ConfigFileS("WinFontName"));
    ScreenFont2 = new ScreenFont(ConfigFileS("DuospaceFontName"));

    setTimeout(ScreenInit2, 20);
}

function ScreenInit2()
{
    if (ScreenFont1.Ready && ScreenFont2.Ready)
    {
        ScreenSetDisplayConfig(false);
        ScreenTimerPeriod = ConfigFileI("WinTimer");
    
        ScreenCellW = ScreenFont1.CellW;
        ScreenCellH = ScreenFont1.CellH;
        ScreenCellH2 = ScreenCellH / 2;
        ScreenCursorSize = Math.floor(ScreenFont1.CellH / 8);


        ScreenCellFontX = [];
        ScreenCellFontX.push([0]);
        ScreenCellFontX.push([0]);
        ScreenCellFontX.push([3]);
        for (let I = 0; I < ScreenCellW; I++)
        {
            ScreenCellFontX[0].push(I);
            ScreenCellFontX[1].push(Math.floor(I / 2));
            ScreenCellFontX[2].push(Math.floor(I / 2) + 4);
        }
        ScreenCellFontX.push([7]);
        ScreenCellFontX.push([4]);
        ScreenCellFontX.push([7]);


        ScreenCellFontY = [];
        ScreenCellFontY.push([]);
        ScreenCellFontY.push([]);
        ScreenCellFontY.push([]);
        for (let I = 0; I < ScreenCellH; I++)
        {
            ScreenCellFontY[0].push(ScreenCellW);
            ScreenCellFontY[1].push(((I % 2) == 0) ? 0 : ScreenCellW);
            ScreenCellFontY[2].push(((I % 2) == 0) ? 0 : ScreenCellW);
        }
        ScreenCellFontY[0].push(0);
        ScreenCellFontY[1].push(0);
        ScreenCellFontY[2].push(ScreenCellW * ScreenCellH2);

        

        
        ScreenResize(80, 24);
        ScreenClear(ScreenDefaultBack, ScreenDefaultFore);
        //ScreenTest();
        ScreenTimerStart();
        ProgInitAfterConf();
    }
    else
    {
        setTimeout(ScreenInit2, 20);
    }
}

