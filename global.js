let KeybBlockShortcuts = true;
let GuiSettingsShown = false;
let ProgLoaded = false;
let ProgStarted = false;
let Stopwatch = 0;
let VTTEST_ = document.getElementById("vttest").contentWindow;

let ProgInit_ConfData = "";

let ScreenTimerCounter = 0;
let ScreenTimerPeriod = 100;

let WaitTimeout = 100;

let ScreenCursorSteady = false;

let KeybReady = false;

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

