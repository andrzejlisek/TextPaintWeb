let ExistsSharedArrayBuffer = false;
let DebugMode = true;
let KeybBlockShortcuts = true;
let GuiSettingsShown = false;

let FileLsReady = false;
let FileDbReady = false;

let ProgLoaded = false;
let ProgInitialized = false;
let ProgStarted = false;
let ScreenStarted = false;
let Stopwatch = [];
let StopwatchOffset = 0;
let VTTEST_ = document.getElementById("vttest").contentWindow;

let ProgInit_ConfData = "";

let ScreenTimerPeriod = 100;
let ScreenTimerCounter = 0;
let ScreenTimerCounterLoop = 10;
let ScreenTimerCursor = 1;
let ScreenTimerCursorDisp = false;
let ScreenTimerBlink = 5;
let ScreenTimerBlinkDisp = false;
let ScreenTimerTickEvent = 1;

let WaitTimeout = 500;

let ScreenCursorSteady = false;

let KeybReady = false;

if (typeof SharedArrayBuffer !== 'undefined')
{
    ExistsSharedArrayBuffer = true;
}
else
{
    ExistsSharedArrayBuffer = false;
}

function HexToNum4(X)
{
    let N = "0123456789ABCDEF0123456789abcdef".indexOf(X);
    if (N >= 16)
    {
        N -= 16;
    }
    if (N < 0)
    {
        return 0;
    }
    return N;
}

function HexToNum8(X)
{
    let N1 = HexToNum4(X[0]);
    let N2 = HexToNum4(X[1]);
    return (N1 << 4) + N2;
}

function HexToNum(X)
{
    let N = 0;
    for (let I = 0; I < X.length; I++)
    {
        N = N * 16;
        N = N + HexToNum4(X[I]);
    }
    return N;
}

function StopwatchReset()
{
    StopwatchOffset = performance.now();
    Stopwatch = [];
}

function StopwatchTime()
{
    Stopwatch.push(performance.now() - StopwatchOffset);
}

function ASCII(CharVal, NonPrintable)
{
    if ((CharVal >= 32) && (CharVal <= 126))
    {
        return CharVal;
    }
    if (CharVal == 13)
    {
        return 10;
    }
    if (CharVal == 10)
    {
        return 10;
    }
    return NonPrintable;
}

function StringASCII(Str, NonPrintable, Type)
{
    if ((typeof NonPrintable) == "string")
    {
        NonPrintable = NonPrintable.charCodeAt(0);
    }
    let StrASCII = "";
    for (let I = 0; I < Str.length; I++)
    {
        StrASCII = StrASCII + String.fromCharCode(ASCII(Str.charCodeAt(I), NonPrintable));
    }
    return StrASCII;
}

function StringBufDecode(Str)
{
    let StrDec = "";
    let DecodeState = 0;
    let TempNum = 0;
    for (let I = 0; I < Str.length; I++)
    {
        switch (DecodeState)
        {
            case 0:
                if (Str[I] == '_')
                {
                    DecodeState = 1;
                    TempNum = 0;
                }
                else
                {
                    StrDec = StrDec + Str[I];
                }
                break;
            case 1:
                if (Str[I] == ',')
                {
                    DecodeState = 0;
                    StrDec = StrDec + String.fromCharCode(TempNum);
                }
                else
                {
                    TempNum = TempNum * 10;
                    TempNum = TempNum + Str.charCodeAt(I) - 48;
                }
                break;
        }
    }
    return StrDec;
}

function DataTextToBlob(Data)
{
    const DataStr = atob(Data);
    const DataNum = new Array(DataStr.length);
    for (let i = 0; i < DataStr.length; i++)
    {
        DataNum[i] = DataStr.charCodeAt(i);
    }
    return new Blob([new Uint8Array(DataNum)], {type: "application/octet-stream"});
}

function BrowserW()
{
    return window.innerWidth;
}

function BrowserH()
{
    return window.innerHeight;
}

function BrowserF()
{
    return window.devicePixelRatio;
}

