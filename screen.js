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

var ScreenObj = document.getElementById("Screen");
var ScreenCtx = ScreenObj.getContext('2d');
var ScreenDataC = [];
var ScreenDataB = [];
var ScreenDataF = [];
var ScreenDataA = [];
var ScreenDataW = [];
var ScreenDataH = [];
var ScreenData_0 = [];
var ScreenData_1 = [];
var ScreenData0;
var ScreenData1;

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


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
let SET_ANSIIgnoreConcealed = false;
let SET_ANSIReverseMode = 0;
let SET_ANSIColors = true;
let SET_ANSIColorBlink = false;
let SET_ANSIColorBold = false

//SET_ANSIColorBlink = true;
SET_ANSIColorBold = true;
SET_ANSIReverseMode = 2;
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


let ScreenCalcColor_Back = 0;
let ScreenCalcColor_Fore = 7;
let ScreenCalcColor_Temp = 0;
function ScreenCalcColor(Back, Fore, Attr)
{
    if (SET_ANSIColors)
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
    
    if (SET_ANSIReverseMode == 1)
    {
        if ((Attr & 16) != 0)
        {
            ScreenCalcColor_Temp = ScreenCalcColor_Fore;
            ScreenCalcColor_Fore = ScreenCalcColor_Back;
            ScreenCalcColor_Back = ScreenCalcColor_Temp;
        }
    }

    if (((Attr & 1) != 0) && (SET_ANSIColorBold))
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

    if (((Attr & 8) != 0) && (SET_ANSIColorBlink))
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

    if (SET_ANSIReverseMode == 2)
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

    if (((Attr & 32) != 0) && (!SET_ANSIIgnoreConcealed))
    {
        ScreenCalcColor_Fore = ScreenCalcColor_Back;
    }
}


            
function ScreenChar(X, Y, Chr, Back, Fore, Attr, FontW, FontH)
{
    if (X < 0) { return; }
    if (Y < 0) { return; }
    if (X >= ScreenW) { return; }
    if (Y >= ScreenH) { return; }
    ScreenCalcColor(Back, Fore, Attr);
    Back = ScreenCalcColor_Back;
    Fore = ScreenCalcColor_Fore;
    
    let ScrIdx = Y * ScreenW + X;
    let CharIdx = (Chr < 65536) ? ((Chr << 16) + (Back << 12) + (Fore << 8) + (Attr)) : 0;
    let CharIdx_ = (FontW << 8) + (FontH);
    
    if ((CharIdx != 0) && (ScreenData_0[ScrIdx] == CharIdx) && (ScreenData_1[ScrIdx] == CharIdx_))
    {
        return;
    }
    
    ScreenData_0[ScrIdx] = CharIdx;
    ScreenData_1[ScrIdx] = CharIdx_;
    ScreenDataC[ScrIdx] = Chr;
    ScreenDataB[ScrIdx] = Back;
    ScreenDataF[ScrIdx] = Fore;
    ScreenDataA[ScrIdx] = Attr;
    ScreenDataW[ScrIdx] = FontW;
    ScreenDataH[ScrIdx] = FontH;
    
    let CharData0 = ScreenGlyphBuf0[CharIdx];
    let CharData1 = ScreenGlyphBuf1[CharIdx];
    if ((CharIdx == 0) || (!CharData0))
    {
        let AttrB = Attr & 1;
        let AttrI = Attr & 2;
        let AttrU = Attr & 4;
        let AttrK = Attr & 8;
        let AttrR = Attr & 16;
        let AttrC = Attr & 32;
        let AttrS = Attr & 64;
        //let AttrN = Attr & 128;
    
    
    
        let CharVals = ScreenFont1.GetGlyph(Chr);
        CharData0 = ScreenCtx.createImageData(ScreenCellW, ScreenCellH);
        CharData1 = ScreenCtx.createImageData(ScreenCellW, ScreenCellH);
        let N = ScreenCellW * ScreenCellH;
        let Ptr4 = 0;
        let PxlOn = false;
        let I = 0;
        for (let Y = 0; Y < ScreenCellH; Y++)
        {
            for (let X = 0; X < ScreenCellW; X++)
            {
                PxlOn = false;
                if (AttrI && (Y >= ScreenCellH2))
                {
                    if (X < (ScreenCellW - 1))
                    {
                        if (CharVals[I + 1])
                        {
                            PxlOn = true;
                        }
                    }
                    if (AttrB && CharVals[I])
                    {
                        PxlOn = true;
                    }
                }
                else
                {
                    if (CharVals[I])
                    {
                        PxlOn = true;
                    }
                    if (AttrB && (X > 0))
                    {
                        if (CharVals[I - 1])
                        {
                            PxlOn = true;
                        }
                    }
                }
                if ((AttrU) && (Y == (ScreenCellH - 1))) { PxlOn = true; }
                if ((AttrS) && (Y == (ScreenCellH2 - 1))) { PxlOn = true; }
                
                if (PxlOn)
                {
                    CharData0.data[Ptr4 + 0] = ScreenPalette1R[Fore];
                    CharData0.data[Ptr4 + 1] = ScreenPalette1G[Fore];
                    CharData0.data[Ptr4 + 2] = ScreenPalette1B[Fore];
                    if (AttrK)
                    {
                        CharData1.data[Ptr4 + 0] = ScreenPalette2R[Fore];
                        CharData1.data[Ptr4 + 1] = ScreenPalette2G[Fore];
                        CharData1.data[Ptr4 + 2] = ScreenPalette2B[Fore];
                    }
                    else
                    {
                        CharData1.data[Ptr4 + 0] = ScreenPalette1R[Fore];
                        CharData1.data[Ptr4 + 1] = ScreenPalette1G[Fore];
                        CharData1.data[Ptr4 + 2] = ScreenPalette1B[Fore];
                    }
                }
                else
                {
                    CharData0.data[Ptr4 + 0] = ScreenPalette1R[Back];
                    CharData0.data[Ptr4 + 1] = ScreenPalette1G[Back];
                    CharData0.data[Ptr4 + 2] = ScreenPalette1B[Back];
                    if (AttrK)
                    {
                        CharData1.data[Ptr4 + 0] = ScreenPalette2R[Back];
                        CharData1.data[Ptr4 + 1] = ScreenPalette2G[Back];
                        CharData1.data[Ptr4 + 2] = ScreenPalette2B[Back];
                    }
                    else
                    {
                        CharData1.data[Ptr4 + 0] = ScreenPalette1R[Back];
                        CharData1.data[Ptr4 + 1] = ScreenPalette1G[Back];
                        CharData1.data[Ptr4 + 2] = ScreenPalette1B[Back];
                    }
                }
                CharData0.data[Ptr4 + 3] = 255;
                CharData1.data[Ptr4 + 3] = 255;
                Ptr4 += 4;
                I++;
            }
        }
        ScreenGlyphBuf0[CharIdx] = CharData0;
        ScreenGlyphBuf1[CharIdx] = CharData1;
    }
    let PtrI = 0;
    let PtrO = (((Y * ScreenCellH) * (ScreenWW)) + (X * ScreenCellW)) << 2;
    
    for (let Y_ = 0; Y_ < ScreenCellH; Y_++)
    {
        for (let X_ = 0; X_ < ScreenCellW; X_++)
        {
            ScreenData0.data[PtrO + 0] = CharData0.data[PtrI + 0];
            ScreenData0.data[PtrO + 1] = CharData0.data[PtrI + 1];
            ScreenData0.data[PtrO + 2] = CharData0.data[PtrI + 2];
            ScreenData1.data[PtrO + 0] = CharData1.data[PtrI + 0];
            ScreenData1.data[PtrO + 1] = CharData1.data[PtrI + 1];
            ScreenData1.data[PtrO + 2] = CharData1.data[PtrI + 2];

            PtrI += 4;
            PtrO += 4;
        }
        PtrO = PtrO - (ScreenCellW << 2) + ((ScreenWW) << 2);
    }
    if (ScreenBlinkState)
    {
        ScreenCtx.putImageData(CharData1, X * ScreenCellW, Y * ScreenCellH);
    }
    else
    {
        ScreenCtx.putImageData(CharData0, X * ScreenCellW, Y * ScreenCellH);
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
    
    ScreenRepaint();
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
        for (let X = 0; X < ScreenW; X++)
        {
            ScreenChar(X, Y, 32, ColorBack, ColorFore, 0, 0, 0);
        }
    }
}

function ScreenCursorMove(X, Y)
{
    ScreenDrawCursor(false);
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

function ScreenWrite(Chr, ColorBack, ColorFore)
{
    ScreenData_0[ScreenCursorY * ScreenW + ScreenCursorX] = -1;
    ScreenChar(ScreenCursorX, ScreenCursorY, Chr, ColorBack, ColorFore, 0, 0, 0);
    ScreenCursorX++;
    if (ScreenCursorX == ScreenW)
    {
        ScreenCursorX = 0;
        ScreenCursorY++;
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

function ScreenDrawCursor(State)
{
    if (State)
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
                CursorData.data[Ptr1 + 0] = ScreenPalette1R[ScreenDataF[PtrX]];
                CursorData.data[Ptr1 + 1] = ScreenPalette1G[ScreenDataF[PtrX]];
                CursorData.data[Ptr1 + 2] = ScreenPalette1B[ScreenDataF[PtrX]];
                ScreenData0.data[Ptr2 + 0] = ScreenPalette1R[ScreenDataF[PtrX]];
                ScreenData0.data[Ptr2 + 1] = ScreenPalette1R[ScreenDataF[PtrX]];
                ScreenData0.data[Ptr2 + 2] = ScreenPalette1R[ScreenDataF[PtrX]];
                ScreenData1.data[Ptr2 + 0] = ScreenPalette1G[ScreenDataF[PtrX]];
                ScreenData1.data[Ptr2 + 1] = ScreenPalette1G[ScreenDataF[PtrX]];
                ScreenData1.data[Ptr2 + 2] = ScreenPalette1G[ScreenDataF[PtrX]];
                Ptr1 += 4;
                Ptr2 += 4;
            }
            Ptr2 = Ptr2 - (ScreenCellW << 2) + ((ScreenWW) << 2);
        }
        ScreenCtx.putImageData(CursorData, CursorX_, CursorY_);
    }
    else
    {
        ScreenRepaintChar(ScreenCursorX, ScreenCursorY);
    }
}


let ScreenFont1;
let ScreenFont2;


function ScreenInit1()
{
    ScreenPalette1R = [0x00, 0xAA, 0x00, 0xAA, 0x00, 0xAA, 0x00, 0xAA, 0x55, 0xFF, 0x55, 0xFF, 0x55, 0xFF, 0x55, 0xFF];
    ScreenPalette1G = [0x00, 0x00, 0xAA, 0xAA, 0x00, 0x00, 0xAA, 0xAA, 0x55, 0x55, 0xFF, 0xFF, 0x55, 0x55, 0xFF, 0xFF];
    ScreenPalette1B = [0x00, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 0xFF, 0xFF, 0xFF, 0xFF];
    ScreenPalette2R = [0x00, 0x71, 0x00, 0x71, 0x00, 0x71, 0x00, 0x71, 0x39, 0xAA, 0x39, 0xAA, 0x39, 0xAA, 0x39, 0xAA];
    ScreenPalette2G = [0x00, 0x00, 0x71, 0x71, 0x00, 0x00, 0x71, 0x71, 0x39, 0x39, 0xAA, 0xAA, 0x39, 0x39, 0xAA, 0xAA];
    ScreenPalette2B = [0x00, 0x00, 0x00, 0x00, 0x71, 0x71, 0x71, 0x71, 0x39, 0x39, 0x39, 0x39, 0xAA, 0xAA, 0xAA, 0xAA];
    ScreenFont1 = new ScreenFont(ScreenFontUrl1);
    ScreenFont2 = new ScreenFont(ScreenFontUrl2);

    setTimeout(ScreenInit2, 20);
}

function ScreenInit2()
{
    if (ScreenFont1.Ready && ScreenFont2.Ready)
    {
        ScreenCellW = ScreenFont1.CellW;
        ScreenCellH = ScreenFont1.CellH;
        ScreenCellH2 = ScreenCellH / 2;
        ScreenCursorSize = Math.floor(ScreenFont1.CellH / 8);
        
        ScreenResize(80, 24);
        ScreenClear(ScreenDefaultBack, ScreenDefaultFore);
        //ScreenTest();
        ScreenTimerStart();
        ProgInit();
    }
    else
    {
        setTimeout(ScreenInit2, 20);
    }
}

ScreenInit1();
