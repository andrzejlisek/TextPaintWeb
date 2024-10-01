let ScreenW = 0;
let ScreenH = 0;
let ScreenCellW = 8;
let ScreenCellH = 16;
let ScreenWW = 0;
let ScreenHH = 0;
let ScreenCursorX = 0;
let ScreenCursorY = 0;
let ScreenCursorOpt = 0;
let ScreenCursorDbl = false;
let ScreenCursorSizeH = 0;
let ScreenCursorSizeV = 0;
let ScreenBlinkState = false;
let ScreenCellFontX = [];
let ScreenCellFontY = [];

let ScreenBellVolume = 25;
let ScreenBellFreq = 800;
let ScreenBellTime = 100;

let ScreenMouseX = 0;
let ScreenMouseY = 0;
let ScreenCursorHide = false;
let ScreenCursorTerm = 0;

var ScreenDiv = document.getElementById("screendiv");
var ScreenVP = document.getElementById("screenvp");
var ScreenVP0 = document.getElementById("screenmouse");
var Screen0Obj = document.getElementById("Screen0");
var Screen0Ctx = Screen0Obj.getContext("2d", { willReadFrequently: true });
var Screen1Obj = document.getElementById("Screen1");
var Screen1Ctx = Screen1Obj.getContext("2d", { willReadFrequently: true });


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
let ScreenCursorData;

let ScreenLineOffsetVal = [];
let ScreenLineOffsetValArray = [];
let ScreenLineOffsetBlank = [];

var ScreenPaletteR = [];
var ScreenPaletteG = [];
var ScreenPaletteB = [];

var ScreenDefaultBack = 0;
var ScreenDefaultFore = 7;


let ScreenCellH2 = 8;



const ScreenGlyphBufMaxFontSize = 10;

let ScreenGlyphBuf0 = [];
let ScreenGlyphBuf1 = [];
let ScreenGlyphBufFontMap;

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
let ScreenColorBlending = false;
let ScreenColorBlendingGamma = 0;
let ScreenColorBlendingGamma1 = 0;
let ScreenColorBlendingGamma2 = 0;
let ScreenColorBlendingChars = {};
let ScreenDisplayInterpolate = 0;
let ScreenDisplayInterpolate_ = 0;

function ScreenGlyphBufClear()
{
    let Ptr = 0;
    ScreenGlyphBufFontMap = [];
    for (let I = 0; I < ScreenGlyphBufMaxFontSize; I++)
    {
        let MapX = [];
        for (let II = 0; II < ScreenGlyphBufMaxFontSize; II++)
        {
            MapX.push(Ptr);
            Ptr++;
        }
        ScreenGlyphBufFontMap.push(MapX);
    }

    ScreenGlyphBuf0 = [];
    ScreenGlyphBuf1 = [];
    for (let I = 0; I < Ptr; I++)
    {
        ScreenGlyphBuf0.push({});
        ScreenGlyphBuf1.push({});
    }
}

function ScreenSetDisplayConfigCursorReset()
{
    ScreenCursorTerm = 0;
    ScreenCursorHide = false;
}

function ScreenSetDisplayConfigCursor()
{
    ScreenDisplayCursor0 = (ScreenDisplayCursor == 2) || (ScreenDisplayCursor == 4) || (ScreenDisplayCursor == 6);
    ScreenDisplayCursor1 = (ScreenDisplayCursor > 0);
    ScreenDisplayCursorShape = (ScreenDisplayCursor > 0) ? Math.floor((ScreenDisplayCursor - 1) / 2) : 0;

    if (ScreenCursorTerm > 0)
    {
        ScreenDisplayCursor0 = (ScreenCursorTerm == 2) || (ScreenCursorTerm == 4) || (ScreenCursorTerm == 6);
        ScreenDisplayCursor1 = true;
        ScreenDisplayCursorShape = Math.floor((ScreenCursorTerm - 1) / 2);
    }

    if (ScreenCursorHide)
    {
        ScreenDisplayCursor0 = false;
        ScreenDisplayCursor1 = false;
    }
}

function ScreenSetDisplayConfig(Repaint)
{
    let IsChanged = false;
    let CalcGamma = false;

    if (ScreenColorBlendingGamma != ConfigFileI("ColorBlendingGamma")) { IsChanged = true; CalcGamma = true; }
    if (ScreenDisplayInterpolate != ConfigFileI("DisplayInterpolate")) IsChanged = true;
    if (ScreenColorBlending != ConfigFileB("ColorBlending")) IsChanged = true;
    if (ScreenDisplayCursor != ConfigFileI("DisplayCursor")) IsChanged = true;
    if (ScreenSET_ANSIIgnoreConcealed != ConfigFileB("ANSIIgnoreConcealed")) IsChanged = true;
    if (ScreenSET_ANSIReverseMode != ConfigFileI("ANSIReverseMode")) IsChanged = true;
    if (ScreenSET_ANSIColors != ConfigFileB("ANSIColors")) IsChanged = true;
    if (ScreenSET_ANSIColorBlink != ConfigFileB("ANSIColorBlink")) IsChanged = true;
    if (ScreenSET_ANSIColorBold != ConfigFileB("ANSIColorBold")) IsChanged = true;

    if (ScreenAttrB != ((ConfigFileI("DisplayAttrib") & 1) > 0)) IsChanged = true;
    if (ScreenAttrI != ((ConfigFileI("DisplayAttrib") & 2) > 0)) IsChanged = true;
    if (ScreenAttrU != ((ConfigFileI("DisplayAttrib") & 4) > 0)) IsChanged = true;
    if (ScreenAttrS != ((ConfigFileI("DisplayAttrib") & 8) > 0)) IsChanged = true;

    if (ScreenSET_Blink != ConfigFileI("DisplayBlink")) IsChanged = true;

    ScreenBellVolume = ConfigFileI("BellVolume");
    ScreenBellFreq = ConfigFileI("BellFrequency");
    ScreenBellTime = ConfigFileI("BellDuration");


    ScreenColorBlendingGamma = ConfigFileI("ColorBlendingGamma");
    ScreenDisplayInterpolate = ConfigFileI("DisplayInterpolate");
    ScreenColorBlending = ConfigFileB("ColorBlending");
    ScreenDisplayCursor = ConfigFileI("DisplayCursor");
    ScreenSET_ANSIIgnoreConcealed = ConfigFileB("ANSIIgnoreConcealed");
    ScreenSET_ANSIReverseMode = ConfigFileI("ANSIReverseMode");
    ScreenSET_ANSIColors = ConfigFileB("ANSIColors");
    ScreenSET_ANSIColorBlink = ConfigFileB("ANSIColorBlink");
    ScreenSET_ANSIColorBold = ConfigFileB("ANSIColorBold");

    ScreenSetDisplayConfigCursor();

    ScreenAttrB = ((ConfigFileI("DisplayAttrib") & 1) > 0) ? true : false;
    ScreenAttrI = ((ConfigFileI("DisplayAttrib") & 2) > 0) ? true : false;
    ScreenAttrU = ((ConfigFileI("DisplayAttrib") & 4) > 0) ? true : false;
    ScreenAttrS = ((ConfigFileI("DisplayAttrib") & 8) > 0) ? true : false;

    ScreenSET_Blink = ConfigFileI("DisplayBlink");
    
    if (ScreenDisplayInterpolate_ != ScreenDisplayInterpolate)
    {
        switch (ScreenDisplayInterpolate)
        {
            case 0:
            case 2:
            case 3:
                Screen0Obj.className = "canvas0";
                Screen1Obj.className = "canvas0";
                break;
            case 1:
                Screen0Obj.className = "canvas1";
                Screen1Obj.className = "canvas1";
                break;
        }
        ScreenDisplayResize();
        ScreenDisplayInterpolate_ = ScreenDisplayInterpolate;
    }
    
    ScreenGlyphBufClear();

    if (CalcGamma)
    {
        ScreenColorBlendingGamma1 = ScreenColorBlendingGamma / 1000.0;
        ScreenColorBlendingGamma2 = 1000.0 / ScreenColorBlendingGamma;
        ScreenCalcBlendMap = {};
    }
    if (Repaint && IsChanged)
    {
        ScreenRepaint();
    }
}

function ScreenCalcColorNega(ScreenColor)
{
    if (ScreenColor < 8)
    {
        return 7 - ScreenColor;
    }
    else
    {
        return 23 - ScreenColor;
    }
}


let ScreenCalcColor_Back = 0;
let ScreenCalcColor_Fore = 7;
let ScreenCalcColor_Temp = 0;
function ScreenCalcColor(Back, Fore, Attr)
{
    if (ScreenSET_ANSIColors || (Back > 15) || (Fore > 15))
    {
        ScreenCalcColor_Back = (Back >= 0) ? (Back & 15) : ScreenDefaultBack;
        ScreenCalcColor_Fore = (Fore >= 0) ? (Fore & 15) : ScreenDefaultFore;
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



let ScreenCalcBlendMap = {};

function ScreenCalcBlend(Val1, Prop1, Val2, Prop2)
{
    const Key = Val1 + (Val2 << 8) + (Prop1 << 16) + (Prop2 << 20);
    if (Key in ScreenCalcBlendMap)
    {
        return ScreenCalcBlendMap[Key];
    }
    else
    {
        const _Val1 = Math.pow(Val1 / 255.0, ScreenColorBlendingGamma1) * Prop1;
        const _Val2 = Math.pow(Val2 / 255.0, ScreenColorBlendingGamma1) * Prop2;
        const Val = Math.round(Math.pow((_Val1 + _Val2) / (Prop1 + Prop2), ScreenColorBlendingGamma2) * 255.0);
        ScreenCalcBlendMap[Key] = Val;
        return Val;
    }
}


function ScreenCharGlyph(Buf, ScrIdx)
{
    let Chr = ScreenDataC[ScrIdx];
    const Back = ScreenDataB[ScrIdx];
    const Fore = ScreenDataF[ScrIdx];
    const Back_ = ScreenDataB_[ScrIdx];
    const Fore_ = ScreenDataF_[ScrIdx];
    const Attr = ScreenDataA[ScrIdx];
    const FontW = ScreenDataW[ScrIdx];
    const FontH = ScreenDataH[ScrIdx];

    const FontIdx = ((FontW < ScreenGlyphBufMaxFontSize) && (FontH < ScreenGlyphBufMaxFontSize)) ? ScreenGlyphBufFontMap[FontW][FontH] : -1;
    const CharIdx = (Chr < 65536) ? ((Chr << 16) + (Back_ << 12) + (Fore_ << 8) + (Attr)) : 0;
    let CharData0 = (FontIdx >= 0) ? ScreenGlyphBuf0[FontIdx][CharIdx] : undefined;
    let CharData1 = (FontIdx >= 0) ? ScreenGlyphBuf1[FontIdx][CharIdx] : undefined;
    
    if ((FontIdx < 0) || (CharIdx == 0) || (!CharData0))
    {
        let AttrB = ScreenAttrB ? Attr & 1 : 0;
        let AttrI = ScreenAttrI ? Attr & 2 : 0;
        let AttrU = ScreenAttrU ? Attr & 4 : 0;
        let AttrK = Attr & 8;
        let AttrR = Attr & 16;
        let AttrC = Attr & 32;
        let AttrS = ScreenAttrS ? Attr & 64 : 0;
        let AttrN = Attr & 128;

        let Back1R = ScreenPaletteR[Back_];
        let Back1G = ScreenPaletteG[Back_];
        let Back1B = ScreenPaletteB[Back_];

        let Fore1R = ScreenPaletteR[Fore_];
        let Fore1G = ScreenPaletteG[Fore_];
        let Fore1B = ScreenPaletteB[Fore_];

        let Back2R = ScreenPaletteR[Back_ + 16];
        let Back2G = ScreenPaletteG[Back_ + 16];
        let Back2B = ScreenPaletteB[Back_ + 16];

        let Fore2R = ScreenPaletteR[Fore_ + 16];
        let Fore2G = ScreenPaletteG[Fore_ + 16];
        let Fore2B = ScreenPaletteB[Fore_ + 16];

        if (ScreenColorBlending && (ScreenFont.Mode < 10))
        {
            if (Chr in ScreenColorBlendingChars)
            {
                const BlendData = ScreenColorBlendingChars[Chr];
                Chr = BlendData[0];
                let Prop1 = BlendData[1];
                let Prop2 = BlendData[2];

                if (BlendData[3])
                {
                    Back1R = ScreenCalcBlend(Back1R, Prop1, Fore1R, Prop2);
                    Back1G = ScreenCalcBlend(Back1G, Prop1, Fore1G, Prop2);
                    Back1B = ScreenCalcBlend(Back1B, Prop1, Fore1B, Prop2);

                    Back2R = ScreenCalcBlend(Back2R, Prop1, Fore2R, Prop2);
                    Back2G = ScreenCalcBlend(Back2G, Prop1, Fore2G, Prop2);
                    Back2B = ScreenCalcBlend(Back2B, Prop1, Fore2B, Prop2);
                }
                else
                {
                    Fore1R = ScreenCalcBlend(Back1R, Prop1, Fore1R, Prop2);
                    Fore1G = ScreenCalcBlend(Back1G, Prop1, Fore1G, Prop2);
                    Fore1B = ScreenCalcBlend(Back1B, Prop1, Fore1B, Prop2);

                    Fore2R = ScreenCalcBlend(Back2R, Prop1, Fore2R, Prop2);
                    Fore2G = ScreenCalcBlend(Back2G, Prop1, Fore2G, Prop2);
                    Fore2B = ScreenCalcBlend(Back2B, Prop1, Fore2B, Prop2);
                }
            }
        }
    
        let CharVals = ScreenFont.GetGlyph(Chr);
        
        CharData0 = Screen0Ctx.getImageData(0, 0, ScreenCellW, ScreenCellH);
        CharData1 = Screen1Ctx.getImageData(0, 0, ScreenCellW, ScreenCellH);
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
                    CharData0.data[Ptr4 + 0] = Fore1R;
                    CharData0.data[Ptr4 + 1] = Fore1G;
                    CharData0.data[Ptr4 + 2] = Fore1B;
                    if (AttrK && (ScreenSET_Blink > 0))
                    {
                        if (ScreenSET_Blink == 1)
                        {
                            CharData1.data[Ptr4 + 0] = Fore2R;
                            CharData1.data[Ptr4 + 1] = Fore2G;
                            CharData1.data[Ptr4 + 2] = Fore2B;
                        }
                        else
                        {
                            CharData1.data[Ptr4 + 0] = ScreenPaletteR[Back_];
                            CharData1.data[Ptr4 + 1] = ScreenPaletteG[Back_];
                            CharData1.data[Ptr4 + 2] = ScreenPaletteB[Back_];
                        }
                    }
                    else
                    {
                        CharData1.data[Ptr4 + 0] = Fore1R;
                        CharData1.data[Ptr4 + 1] = Fore1G;
                        CharData1.data[Ptr4 + 2] = Fore1B;
                    }
                }
                else
                {
                    CharData0.data[Ptr4 + 0] = Back1R;
                    CharData0.data[Ptr4 + 1] = Back1G;
                    CharData0.data[Ptr4 + 2] = Back1B;
                    if (AttrK && (ScreenSET_Blink > 0))
                    {
                        if (ScreenSET_Blink == 1)
                        {
                            CharData1.data[Ptr4 + 0] = Back2R;
                            CharData1.data[Ptr4 + 1] = Back2G;
                            CharData1.data[Ptr4 + 2] = Back2B;
                        }
                        else
                        {
                            CharData1.data[Ptr4 + 0] = ScreenPaletteR[Back_];
                            CharData1.data[Ptr4 + 1] = ScreenPaletteG[Back_];
                            CharData1.data[Ptr4 + 2] = ScreenPaletteB[Back_];
                        }
                    }
                    else
                    {
                        CharData1.data[Ptr4 + 0] = Back1R;
                        CharData1.data[Ptr4 + 1] = Back1G;
                        CharData1.data[Ptr4 + 2] = Back1B;
                    }
                }
                CharData0.data[Ptr4 + 3] = 255;
                CharData1.data[Ptr4 + 3] = 255;
                Ptr4 += 4;
            }
            I += ScreenCellFontY_[Y];
        }
        if (FontIdx >= 0)
        {
            ScreenGlyphBuf0[FontIdx][CharIdx] = CharData0;
            ScreenGlyphBuf1[FontIdx][CharIdx] = CharData1;
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
    ScreenCalcColor(Back, Fore, Attr);
    
    if (ScreenMousePaint(X, Y))
    {
        ScreenCalcColor_Back = ScreenCalcColorNega(ScreenCalcColor_Back);
        ScreenCalcColor_Fore = ScreenCalcColorNega(ScreenCalcColor_Fore);
    }
    
    let ScrIdx = Y * ScreenW + X;
    const CharIdx0 = ((Chr & 32767) << 16) + (ScreenCalcColor_Back << 12) + (ScreenCalcColor_Fore << 8) + (Attr);
    const CharIdx1 = ((Chr >> 15) << 16) + (FontW << 8) + (FontH);
    if ((ScreenData_0[ScrIdx] == CharIdx0) && (ScreenData_1[ScrIdx] == CharIdx1))
    {
        return;
    }
    
    ScreenData_0[ScrIdx] = CharIdx0;
    ScreenData_1[ScrIdx] = CharIdx1;
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

    const XX = X * ScreenCellW;
    const YY = Y * ScreenCellH;
    switch (OffsetMode)
    {
        default:
            {
                Screen0Ctx.putImageData(CharData0, XX, YY);
                Screen1Ctx.putImageData(CharData1, XX, YY);
            }
            break;
        case 1:
            {
                const T1 = OffsetVal;
                const T2 = ScreenCellH - OffsetVal;

                if (T2 > 0)
                {
                    Screen0Ctx.putImageData(CharData0, XX, YY - T1, 0, T1, ScreenCellW, T2);
                    Screen1Ctx.putImageData(CharData1, XX, YY - T1, 0, T1, ScreenCellW, T2);
                }

                if (OffsetOtherHalf)
                {
                    ScrIdx += ScreenW;
                }
                else
                {
                    ScrIdx = ScreenH * ScreenW + X;
                }
                CharData0_ = ScreenCharGlyph(0, ScrIdx);
                CharData1_ = ScreenCharGlyph(1, ScrIdx);

                Screen0Ctx.putImageData(CharData0_, XX, YY + T2, 0, 0, ScreenCellW, T1);
                Screen1Ctx.putImageData(CharData1_, XX, YY + T2, 0, 0, ScreenCellW, T1);
            }
            break;
        case 2:
            {
                const T1 = OffsetVal;
                const T2 = ScreenCellH - OffsetVal;

                if (T2 > 0)
                {
                    Screen0Ctx.putImageData(CharData0, XX, YY + T1, 0, 0, ScreenCellW, T2);
                    Screen1Ctx.putImageData(CharData1, XX, YY + T1, 0, 0, ScreenCellW, T2);
                }

                if (OffsetOtherHalf)
                {
                    ScrIdx -= ScreenW;
                }
                else
                {
                    ScrIdx = ScreenH * ScreenW + X;
                }
                CharData0_ = ScreenCharGlyph(0, ScrIdx);
                CharData1_ = ScreenCharGlyph(1, ScrIdx);

                Screen0Ctx.putImageData(CharData0_, XX, YY - T2, 0, T2, ScreenCellW, T1);
                Screen1Ctx.putImageData(CharData1_, XX, YY - T2, 0, T2, ScreenCellW, T1);
            }
            break;
    }

    if ((X == ScreenCursorX) && (Y == ScreenCursorY))
    {
        ScreenDrawCursorWork();
    }
}

function ScreenRepaintChar_(X, Y)
{
    if ((X >= 0) && (Y >= 0))
    {
        let Ptr = Y * ScreenW + X;
        ScreenData_0[Ptr] = -1;
        ScreenChar(X, Y, ScreenDataC[Ptr], ScreenDataB[Ptr], ScreenDataF[Ptr], ScreenDataA[Ptr], ScreenDataW[Ptr], ScreenDataH[Ptr]);
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

function ScreenResize(NewW, NewH, DataClear)
{
    let N = (NewW * NewH) + NewW;
    if ((ScreenW != NewW) || (ScreenH != NewH))
    {
        DataClear = true;
    
        let ScreenDataC_ = ScreenDataC;
        let ScreenDataB_1 = ScreenDataB;
        let ScreenDataF_1 = ScreenDataF;
        let ScreenDataB_2 = ScreenDataB_;
        let ScreenDataF_2 = ScreenDataF_;
        let ScreenDataA_ = ScreenDataA;
        let ScreenDataW_ = ScreenDataW;
        let ScreenDataH_ = ScreenDataH;

        ScreenDataC = [];
        ScreenDataB = [];
        ScreenDataF = [];
        ScreenDataB_ = [];
        ScreenDataF_ = [];
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
            ScreenDataB_[I] = ScreenDefaultBack;
            ScreenDataF_[I] = ScreenDefaultFore;
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
                    ScreenDataB[PtrO] = ScreenDataB_1[PtrI];
                    ScreenDataF[PtrO] = ScreenDataF_1[PtrI];
                    ScreenDataB_[PtrO] = ScreenDataB_2[PtrI];
                    ScreenDataF_[PtrO] = ScreenDataF_2[PtrI];
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
        for (let I = 0; I < NewH; I++)
        {
            ScreenLineOffsetVal[I] = 0;
            ScreenLineOffsetBlank[I] = 0;
        }
    }
    
    ScreenW = NewW;
    ScreenH = NewH;
    ScreenWW = ScreenW * ScreenCellW;
    ScreenHH = ScreenH * ScreenCellH;
    Screen0Obj.width = ScreenWW;
    Screen0Obj.height = ScreenHH;
    Screen1Obj.width = ScreenWW;
    Screen1Obj.height = ScreenHH;

    Screen0Ctx = Screen0Obj.getContext("2d", { willReadFrequently: true });
    Screen1Ctx = Screen1Obj.getContext("2d", { willReadFrequently: true });
    ScreenData0 = Screen0Ctx.getImageData(0, 0, ScreenWW, ScreenHH);
    ScreenData1 = Screen1Ctx.getImageData(0, 0, ScreenWW, ScreenHH);
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

    ScreenDataChr0 = Screen0Ctx.getImageData(0, 0, ScreenCellW, ScreenCellH);
    ScreenDataChr1 = Screen1Ctx.getImageData(0, 0, ScreenCellW, ScreenCellH);
    N = ScreenCellW * ScreenCellH;
    for (let I = 0; I < N; I++)
    {
        ScreenDataChr0.data[I * 4 + 3] = 255;
        ScreenDataChr1.data[I * 4 + 3] = 255;
    }

    ScreenCursorData = Screen0Ctx.getImageData(0, 0, ScreenCellW, ScreenCellH);
    ScreenDrawCursorColor = -1;
    N = ScreenCellW * ScreenCellH;
    for (let I = 0; I < N; I++)
    {
        ScreenCursorData.data[I * 4 + 3] = 255;
    }
    
    
    
    if (DataClear)
    {
        ScreenClear(ScreenDefaultBack, ScreenDefaultFore);
    }
    //ScreenRepaint();
    
    ScreenDisplayResize();
    ConfRepaint();
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
        ScreenLineOffsetVal[Y] = -1;
        ScreenLineOffset(Y, 0, 0, ColorBack, ColorFore, 0);
        for (let X = 0; X < ScreenW; X++)
        {
            ScreenChar(X, Y, 32, ColorBack, ColorFore, 0, 0, 0);
        }
    }
}

function ScreenCursorMove(X, Y, Opt)
{
    if ((ScreenCursorX != X) || (ScreenCursorY != Y) || (ScreenCursorOpt != Opt))
    {
        ScreenDrawCursor(false);
    }
    ScreenCursorX = X;
    ScreenCursorY = Y;
    ScreenCursorOpt = Opt;
    if (Opt)
    {
        ScreenCursorDbl = true;
    }
    else
    {
        ScreenCursorDbl = false;
    }
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
    
    if (XDelta == 0)
    {
        if (YDelta == 1)
        {
            let ScrIdxI = (Y1 - 1) * ScreenW;
            let ScrIdxO = (ScreenH) * ScreenW;
            for (let X = XStart; X != XStop; X += XAdv)
            {
                ScreenDataC[ScrIdxO] = ScreenDataC[ScrIdxI];
                ScreenDataB[ScrIdxO] = ScreenDataB[ScrIdxI];
                ScreenDataF[ScrIdxO] = ScreenDataF[ScrIdxI];
                ScreenDataB_[ScrIdxO] = ScreenDataB_[ScrIdxI];
                ScreenDataF_[ScrIdxO] = ScreenDataF_[ScrIdxI];
                ScreenDataA[ScrIdxO] = ScreenDataA[ScrIdxI];
                ScreenDataW[ScrIdxO] = ScreenDataW[ScrIdxI];
                ScreenDataH[ScrIdxO] = ScreenDataH[ScrIdxI];
                
                ScrIdxI++;
                ScrIdxO++;
            }
        }
        if (YDelta == (-1))
        {
            let ScrIdxI = (Y1 + H) * ScreenW;
            let ScrIdxO = (ScreenH) * ScreenW;
            for (let X = XStart; X != XStop; X += XAdv)
            {
                ScreenDataC[ScrIdxO] = ScreenDataC[ScrIdxI];
                ScreenDataB[ScrIdxO] = ScreenDataB[ScrIdxI];
                ScreenDataF[ScrIdxO] = ScreenDataF[ScrIdxI];
                ScreenDataB_[ScrIdxO] = ScreenDataB_[ScrIdxI];
                ScreenDataF_[ScrIdxO] = ScreenDataF_[ScrIdxI];
                ScreenDataA[ScrIdxO] = ScreenDataA[ScrIdxI];
                ScreenDataW[ScrIdxO] = ScreenDataW[ScrIdxI];
                ScreenDataH[ScrIdxO] = ScreenDataH[ScrIdxI];
                
                ScrIdxI++;
                ScrIdxO++;
            }
        }
    }

    for (let Y = YStart; Y != YStop; Y += YAdv)
    {
        for (let X = XStart; X != XStop; X += XAdv)
        {
            const ScrIdx = (Y + YDelta) * ScreenW + (X + XDelta);

            const _C = ScreenDataC[ScrIdx];
            const _B = ScreenDataB[ScrIdx];
            const _F = ScreenDataF[ScrIdx];
            const _A = ScreenDataA[ScrIdx];
            const _W = ScreenDataW[ScrIdx];
            const _H = ScreenDataH[ScrIdx];
            
            ScreenChar(X, Y, _C, _B, _F, _A, _W, _H);
        }
    }
}

function ScreenLineOffset(Y, Offset, Blank, ColorBack, ColorFore, ColorAttr)
{
    if (Y < 0) { return; }
    if (Y >= ScreenH) { return; }
    let OldOffset = ScreenLineOffsetVal[Y]
    if (Offset >= 0)
    {
        ScreenLineOffsetVal[Y] = ScreenLineOffsetValArray[Offset];
    }
    else
    {
        ScreenLineOffsetVal[Y] = 0 - ScreenLineOffsetValArray[0 - Offset];
    }
    ScreenLineOffsetBlank[Y] = Blank;
    if ((OldOffset != 0) || (OldOffset != ScreenLineOffsetVal[Y]))
    {
        for (let X = 0; X < ScreenW; X++)
        {
            ScreenRepaintChar(X, Y);
        }
    }
}

let CursorState = false;
let ScreenDrawCursorColor = -1;

function ScreenDrawCursorWork()
{
    if (CursorState && (ScreenLineOffsetVal[ScreenCursorY] == 0))
    {
        let CursorX_ = ScreenCursorX * ScreenCellW;
        let CursorY_ = ScreenCursorY * ScreenCellH;
        let CurW = ScreenCellW;
        let CurH = ScreenCellH;
        switch (ScreenDisplayCursorShape)
        {
            case 0:
                CursorY_ += (ScreenCellH - ScreenCursorSizeV);
                CurH = ScreenCursorSizeV;
                break;
            case 1:
                CurW = ScreenCursorSizeH;
                break;
        }
        const ScreenDrawCursorColor_ = ScreenDataF_[ScreenCursorY * ScreenW + ScreenCursorX];
        if (ScreenDrawCursorColor != ScreenDrawCursorColor_)
        {
            ScreenDrawCursorColor = ScreenDrawCursorColor_;
            let Ptr1 = 0;
            let Ptr2 = (CursorY_ * (ScreenWW)) + CursorX_ << 2;
            for (let I = 0; I < ScreenCellH; I++)
            {
                for (let II = 0; II < ScreenCellW; II++)
                {
                    ScreenCursorData.data[Ptr1 + 0] = ScreenPaletteR[ScreenDrawCursorColor];
                    ScreenCursorData.data[Ptr1 + 1] = ScreenPaletteG[ScreenDrawCursorColor];
                    ScreenCursorData.data[Ptr1 + 2] = ScreenPaletteB[ScreenDrawCursorColor];
                    Ptr1 += 4;
                    Ptr2 += 4;
                }
                Ptr2 = Ptr2 - (ScreenCellW << 2) + ((ScreenWW) << 2);
            }
        }
        Screen0Ctx.putImageData(ScreenCursorData, CursorX_, CursorY_, 0, 0, CurW, CurH);
        Screen1Ctx.putImageData(ScreenCursorData, CursorX_, CursorY_, 0, 0, CurW, CurH);
        if (ScreenCursorDbl)
        {
            Screen0Ctx.putImageData(ScreenCursorData, CursorX_ + CurW, CursorY_, 0, 0, CurW, CurH);
            Screen1Ctx.putImageData(ScreenCursorData, CursorX_ + CurW, CursorY_, 0, 0, CurW, CurH);
        }
    }
}

function ScreenDrawCursor(State)
{
    if (CursorState && (!State))
    {
        CursorState = false;
        ScreenRepaintChar(ScreenCursorX, ScreenCursorY);
        if (ScreenCursorDbl)
        {
            ScreenRepaintChar(ScreenCursorX + 1, ScreenCursorY);
        }
        return;
    }
    if ((!CursorState) && State)
    {
        CursorState = true;
        ScreenDrawCursorWork();
        return;
    }
}



let ScreenFont;


function ScreenInit1()
{
    KeybTouch = ConfigFileI("WinTouchScreen");
    ScreenGlyphBufClear();
    
    let ParamI = 1;
    let ParamStr = ConfigFileS("ColorBlending_" + ParamI);
    while (ParamStr != "")
    {
        const ParamStr_ = ParamStr.split(',');
        if (ParamStr_.length >= 5)
        {
            ScreenColorBlendingChars[HexToNum(ParamStr_[0])] = [ HexToNum(ParamStr_[1]), parseInt(ParamStr_[2]), parseInt(ParamStr_[3]), parseInt(ParamStr_[4]) ];
        }
        ParamI++;
        ParamStr = ConfigFileS("ColorBlending_" + ParamI);
    }
    
    ScreenPaletteR = [0, 128, 128, 128, 128, 128, 128, 255, 0, 128, 128, 128, 128, 128, 128, 255, 0, 128, 128, 128, 128, 128, 128, 255, 0, 128, 128, 128, 128, 128, 128, 255];
    ScreenPaletteG = [0, 128, 128, 128, 128, 128, 128, 255, 0, 128, 128, 128, 128, 128, 128, 255, 0, 128, 128, 128, 128, 128, 128, 255, 0, 128, 128, 128, 128, 128, 128, 255];
    ScreenPaletteB = [0, 128, 128, 128, 128, 128, 128, 255, 0, 128, 128, 128, 128, 128, 128, 255, 0, 128, 128, 128, 128, 128, 128, 255, 0, 128, 128, 128, 128, 128, 128, 255];

    ScreenSetPalette(ConfigFileS("Palette" + ConfigFileS("PaletteSelect") + "Colors"), false);

    const F1 = ConfigFileS("Font" + ConfigFileS("FontSelect") + "File1");
    const F2 = ConfigFileS("Font" + ConfigFileS("FontSelect") + "File2");
    const F3 = ConfigFileS("Font" + ConfigFileS("FontSelect") + "File3");

    ScreenSetFont(F1, F2, F3, ConfigFileI("FontMode"), false);
}

function ScreenInit2()
{
    ScreenSetDisplayConfig(false);
    ScreenTimerFrames = ConfigFileI("TimerFrames");
    ScreenTimerCursor = ConfigFileI("TimerCursor");
    ScreenTimerBlink = ConfigFileI("TimerBlink");
    ScreenTimerTickEvent = ConfigFileI("TimerTick");

    ScreenTimerCounterLoop = LCM(ScreenTimerCursor, ScreenTimerBlink);
    ScreenTimerCounterLoop = LCM(ScreenTimerCounterLoop, ScreenTimerTickEvent);

    ScreenResize(1, 1, true);
    ScreenClear(ScreenDefaultBack, ScreenDefaultFore);
    //ScreenTest();
    ScreenStarted = true;
    ScreenTimerStart();
    ProgInitAfterConf();
}


function ScreenSetPalette(Colors, Repaint)
{
    if (Repaint)
    {
        ScreenGlyphBufClear();
    }

    if ((Colors.length >= (64 * 3)))
    {
        for (let I = 0; I < 32; I++)
        {
            ScreenPaletteR[I] = HexToNum8(Colors.substr(I * 6 + 0, 2));
            ScreenPaletteG[I] = HexToNum8(Colors.substr(I * 6 + 2, 2));
            ScreenPaletteB[I] = HexToNum8(Colors.substr(I * 6 + 4, 2));
        }
    }

    if (Repaint)
    {
        ScreenRepaint();
    }
}

function ScreenSetFont(File1, File2, File3, CharMode, Repaint)
{
    if (Repaint)
    {
        ScreenFontCreate(File1, File2, File3, CharMode, ScreenSetFontRepaint1);
    }
    else
    {
        ScreenFontCreate(File1, File2, File3, CharMode, ScreenSetFontRepaint0);
    }
}

function ScreenSetFontCustom(FontSize)
{
    if (ScreenCellW > 8)
    {
        ScreenFontCreateCustom(9, FontSize, ScreenSetFontRepaint1);
    }
    else
    {
        ScreenFontCreateCustom(8, FontSize, ScreenSetFontRepaint1);
    }
}

function ScreenSetFontAfterCreate()
{
    ConfigFileSet("_WinBitmapFontPageList", ScreenFont.ScreenPages);
    ConfigFileSet("_WinBitmapFontDouble1", ScreenFont.ScreenDbl1);
    ConfigFileSet("_WinBitmapFontDouble2", ScreenFont.ScreenDbl2);
    ProgEventOtherFile("ScreenFont", "", 0, 0, 0, 0);

    ScreenCellW = ScreenFont.CellW;
    ScreenCellH = ScreenFont.CellH;
    ScreenCellH2 = ScreenCellH / 2;
    ScreenCursorSizeH = Math.floor(ScreenFont.CellW / 8);
    if (ScreenCursorSizeH == 0) ScreenCursorSizeH = 1;
    ScreenCursorSizeV = Math.floor(ScreenFont.CellH / 8);
    if (ScreenCursorSizeV == 0) ScreenCursorSizeV = 1;

    for (let I = 0; I <= 16; I++)
    {
        ScreenLineOffsetValArray[I] = Math.floor((I * ScreenCellH) / 16);
    }

    ScreenCellFontX = [];
    ScreenCellFontY = [];
    for (let FontSize = 1; FontSize <= 32; FontSize++)
    {
        let CellFontXNums = [0];
        for (let I = 0; I < ScreenCellW; I++)
        {
            for (let II = 0; II < FontSize; II++)
            {
                CellFontXNums.push(I);
            }
        }
        CellFontXNums.push(ScreenCellW - 1);

        let NumX = 0;
        let NumY = 1;
        let LineY = 0;
    
        for (let Pos = 0; Pos < FontSize; Pos++)
        {
            let CellFontXItem = [];
            
            for (let NumI = 0; NumI <= (ScreenCellW + 1); NumI++)
            {
                CellFontXItem.push(CellFontXNums[NumX]);
                NumX++;
            }
            NumX -= 2;
            
            ScreenCellFontX.push(CellFontXItem);

            let LineY_ = LineY * ScreenCellW;
            let CellFontYItem = [];
            for (let NumI = 0; NumI < ScreenCellH; NumI++)
            {
                if ((NumY % FontSize) == 0)
                {
                    CellFontYItem.push(ScreenCellW);
                    LineY++;
                }
                else
                {
                    CellFontYItem.push(0);
                }
                NumY++;
            }
            CellFontYItem.push(LineY_);

            ScreenCellFontY.push(CellFontYItem);
        }
    }
}

function ScreenSetFontRepaint0()
{
    ScreenFont = ScreenFontTemp[0];
    ScreenFontTemp.shift();
    ScreenSetFontAfterCreate();
    ScreenInit2();
}

function ScreenSetFontRepaint1()
{
    if (ScreenFontTemp.length > 0)
    {
        while (ScreenFontTemp.length > 1)
        {
            ScreenFontTemp.shift();
        }
        ScreenFont = ScreenFontTemp[0];
        ScreenFontTemp.shift();
        ScreenSetFontAfterCreate();
        ScreenGlyphBufClear();
        ScreenResize(ScreenW, ScreenH, false);
        ScreenRepaint();
    }
}

function ScreenDrawBlink(State)
{
    ScreenBlinkState = State;
    if (ScreenBlinkState)
    {
        //Screen0Obj.style["z-index"] = 1;
        //Screen1Obj.style["z-index"] = 2;
        Screen1Obj.style["display"] = "none";
    }
    else
    {
        //Screen0Obj.style["z-index"] = 2;
        //Screen1Obj.style["z-index"] = 1;
        Screen1Obj.style["display"] = "";
    }
}


function ScreenDisplayResize()
{
    const ScrW = parseInt(ScreenDiv.style.width.substr(0, ScreenDiv.style.width.length - 2));
    const ScrH = parseInt(ScreenDiv.style.height.substr(0, ScreenDiv.style.height.length - 2));
    switch (ScreenDisplayInterpolate)
    {
        case 0:
        case 1:
            ScreenVP.style.width = ScrW + "px";
            ScreenVP.style.height = ScrH + "px";
            ScreenVP.style.left = "0px";
            ScreenVP.style.top = "0px";

            Screen0Obj.style.width = ScrW + "px";
            Screen0Obj.style.height = ScrH + "px";
            Screen1Obj.style.width = ScrW + "px";
            Screen1Obj.style.height = ScrH + "px";
            break;
        case 2:
            let ScrW_ = parseFloat(ScrW) * BrowserF();
            let ScrH_ = parseFloat(ScrH) * BrowserF();
            let CanW = (ScrW_ > ScreenWW) ? Math.floor(ScrW_ / ScreenWW) * ScreenWW : ScreenWW;
            let CanH = (ScrH_ > ScreenHH) ? Math.floor(ScrH_ / ScreenHH) * ScreenHH : ScreenHH;
            while ((ScrW_ < CanW) && (CanW > 1))
            {
                CanW = CanW >> 1;
            }
            while ((ScrH_ < CanH) && (CanH > 1))
            {
                CanH = CanH >> 1;
            }
            ScrW_ = ScrW_ / BrowserF();
            ScrH_ = ScrH_ / BrowserF();
            CanW = CanW / BrowserF();
            CanH = CanH / BrowserF();

            ScreenVP.style.width = Math.round(CanW) + "px";
            ScreenVP.style.height = Math.round(CanH) + "px";
            ScreenVP.style.left = Math.round((ScrW_ - CanW) / 2) + "px";
            ScreenVP.style.top = Math.round((ScrH_ - CanH) / 2) + "px";

            Screen0Obj.style.width = CanW + "px";
            Screen0Obj.style.height = CanH + "px";
            Screen1Obj.style.width = CanW + "px";
            Screen1Obj.style.height = CanH + "px";
            break;
        case 3:
            ScreenVP.style.width = ScrW + "px";
            ScreenVP.style.height = ScrH + "px";
            ScreenVP.style.left = "0px";
            ScreenVP.style.top = "0px";
            
            Screen0Obj.style.width = ScrW + "px";
            Screen0Obj.style.height = ScrH + "px";
            Screen1Obj.style.width = ScrW + "px";
            Screen1Obj.style.height = ScrH + "px";
            break;
    }

    ScreenVP0.style.width = ScreenVP.style.width;
    ScreenVP0.style.height = ScreenVP.style.height;
    ScreenVP0.style.left = ScreenVP.style.left;
    ScreenVP0.style.top = ScreenVP.style.top;

    ScreenDrawBlink(ScreenBlinkState);
}

