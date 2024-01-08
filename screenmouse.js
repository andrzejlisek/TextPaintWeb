let ScreenBellUnInitialized = true;
let ScreenBellContext;


function ScreenBellInit()
{
    if (ScreenBellUnInitialized)
    {
        ScreenBellContext = new (window.AudioContext || window.webkitAudioContext)();
        ScreenBellUnInitialized = false;
    }
}

function ScreenBell()
{
    if (ScreenBellUnInitialized || (ScreenBellVolume == 0) || (ScreenBellFreq == 0) || (ScreenBellTime == 0))
    {
        return;
    }
    let ScreenBellGain = ScreenBellContext.createGain();
    let ScreenBellOsci = ScreenBellContext.createOscillator();
    ScreenBellGain.gain.value = parseFloat(ScreenBellVolume) / 100.0;
    ScreenBellOsci.connect(ScreenBellGain);
    ScreenBellGain.connect(ScreenBellContext.destination);
    ScreenBellOsci.type = "square";
    ScreenBellOsci.frequency.value = ScreenBellFreq;
    ScreenBellOsci.start();
    ScreenBellOsci.stop(ScreenBellContext.currentTime + (parseFloat(ScreenBellTime) / 1000));
}


let ScreenMouseHighlightListX = [];
let ScreenMouseHighlightListY = [];

function ScreenMouseHighlightRepaintClear()
{
    if (ScreenMouseActive)
    {
        ScreenMouseActive = false;
        for (let I = 0; I < ScreenMouseHighlightListX.length; I++)
        {
            ScreenRepaintChar_(ScreenMouseHighlightListX[I], ScreenMouseHighlightListY[I]);
        }
        ScreenMouseActive = true;
    }
    ScreenMouseHighlightListX = [];
    ScreenMouseHighlightListY = [];
}

function ScreenMouseHighlightRepaintDraw()
{
    let X1 = ScreenMouseHighlightX;
    let Y1 = ScreenMouseHighlightY;
    let X2 = ScreenMouseProgMoveX;
    let Y2 = ScreenMouseProgMoveY;
    if (Y1 > Y2)
    {
        let _ = X1;
        X1 = X2;
        X2 = _;
        _ = Y1;
        Y1 = Y2;
        Y2 = _;
    }
    Y1 = Math.min(Math.max(Y1, ScreenMouseHighlightF), ScreenMouseHighlightL);
    Y2 = Math.min(Math.max(Y2, ScreenMouseHighlightF), ScreenMouseHighlightL);
    if (Y1 == Y2)
    {
        X1 = Math.min(ScreenMouseProgMoveX, ScreenMouseHighlightX);
        X2 = Math.max(ScreenMouseProgMoveX, ScreenMouseHighlightX);
        for (let XX = X1; XX <= X2; XX++)
        {
            ScreenMouseHighlightListX.push(XX);
            ScreenMouseHighlightListY.push(Y1);
            ScreenRepaintChar_(XX, Y1);
        }
    }
    else
    {
        for (let XX = X1; XX < ScreenW; XX++)
        {
            ScreenMouseHighlightListX.push(XX);
            ScreenMouseHighlightListY.push(Y1);
            ScreenRepaintChar_(XX, Y1);
        }
        for (let YY = (Y1 + 1); YY < Y2; YY++)
        {
            for (let XX = 0; XX < ScreenW; XX++)
            {
                ScreenMouseHighlightListX.push(XX);
                ScreenMouseHighlightListY.push(YY);
                ScreenRepaintChar_(XX, YY);
            }
        }
        for (let XX = 0; XX <= X2; XX++)
        {
            ScreenMouseHighlightListX.push(XX);
            ScreenMouseHighlightListY.push(Y2);
            ScreenRepaintChar_(XX, Y2);
        }
    }
}

function ScreenMouseHighlightRepaint()
{
    if (ScreenMouseActive && ScreenMouseHighlight)
    {
        ScreenMouseHighlightRepaintClear();
        ScreenMouseHighlightRepaintDraw();
    }
}

function ScreenMouseSetHighlight(Work, X, Y, F, L)
{
    ScreenMouseHighlight = Work > 0 ? true : false;
    ScreenMouseHighlightX = X - 1;
    ScreenMouseHighlightY = Y - 1;
    ScreenMouseHighlightF = F - 1;
    ScreenMouseHighlightL = L - 2;
    ScreenMouseHighlightRepaintClear();
}

function ScreenMousePaint(X, Y)
{
    if (ScreenMouseActive)
    {
        if ((X == ScreenMouseProgMoveX) && (Y == ScreenMouseProgMoveY))
        {
            return true;
        }
        if (ScreenMouseHighlight)
        {
            for (let I = 0; I < ScreenMouseHighlightListX.length; I++)
            {
                if ((X == ScreenMouseHighlightListX[I]) && (Y == ScreenMouseHighlightListY[I]))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

function ScreenMouseTurn1()
{
    ScreenObj.style.cursor = "none";
    ScreenMouseActive = true;
    ScreenRepaintChar_(ScreenMouseProgMoveX, ScreenMouseProgMoveY);
    for (let I = 0; I < ScreenMouseHighlightListX.length; I++)
    {
        ScreenRepaintChar_(ScreenMouseHighlightListX[I], ScreenMouseHighlightListY[I]);
    }
}

function ScreenMouseTurn0()
{
    ScreenObj.style.cursor = "";
    ScreenMouseActive = false;
    ScreenRepaintChar_(ScreenMouseProgMoveX, ScreenMouseProgMoveY);
    for (let I = 0; I < ScreenMouseHighlightListX.length; I++)
    {
        ScreenRepaintChar_(ScreenMouseHighlightListX[I], ScreenMouseHighlightListY[I]);
    }
}


// 1 - lewy
// 2 - sroskowy
// 3 - prawy
// 4 - kolko w gore
// 5 - kolko w dol

let ScreenMouseBtn1 = false;
let ScreenMouseBtn2 = false;
let ScreenMouseBtn3 = false;


function ScreenMouseCalcX(X)
{
    const ScrW = parseInt(ScreenVP.style.width.substr(0, ScreenVP.style.width.length - 2));
    return Math.floor(X * ScreenW / ScrW);
}

function ScreenMouseCalcY(Y)
{
    const ScrH = parseInt(ScreenVP.style.height.substr(0, ScreenVP.style.height.length - 2));
    return Math.floor(Y * ScreenH / ScrH);
}



let ScreenMouseAreaUse = true;
let ScreenMouseAreaX1 = -1;
let ScreenMouseAreaY1 = -1;
let ScreenMouseAreaX2 = -1;
let ScreenMouseAreaY2 = -1;

function ScreenMouseShowConfig()
{
    if ((ScreenMouseAreaX1 < 0) || (ScreenMouseAreaY1 < 0) || (ScreenMouseAreaX2 < 0) || (ScreenMouseAreaY2 < 0))
    {
        return true
    }
    if ((ScreenMouseAreaX1 == ScreenMouseAreaX2) && (ScreenMouseAreaY1 == ScreenMouseAreaY2))
    {
        return true;
    }
    return false;
}

function ScreenMouseScreenArea()
{
    if (ScreenMouseAreaUse && (!ScreenMouseShowConfig()))
    {
        let Txt = "";
        let Txt_ = [];
        if (ScreenMouseAreaX1 > ScreenMouseAreaX2)
        {
            const Temp = ScreenMouseAreaX1;
            ScreenMouseAreaX1 = ScreenMouseAreaX2;
            ScreenMouseAreaX2 = Temp;
        }
        if (ScreenMouseAreaY1 > ScreenMouseAreaY2)
        {
            const Temp = ScreenMouseAreaY1;
            ScreenMouseAreaY1 = ScreenMouseAreaY2;
            ScreenMouseAreaY2 = Temp;
        }
        for (let Y = ScreenMouseAreaY1; Y <= ScreenMouseAreaY2; Y++)
        {
            Txt = "";
            let ScrIdx = Y * ScreenW + ScreenMouseAreaX1;
            for (let X = ScreenMouseAreaX1; X <= ScreenMouseAreaX2; X++)
            {
                if (ScreenDataC[ScrIdx] >= 32)
                {
                    Txt = Txt + String_fromCharCode(ScreenDataC[ScrIdx]);
                }
                else
                {
                    Txt = Txt + " ";
                }
                ScrIdx++;
            }
            Txt_.push(Txt.trimEnd());
        }
        Txt = Txt_.join("\n");
        if (navigator.clipboard.writeText)
        {
            navigator.clipboard.writeText(Txt).then((_) => {}).catch((_) => {});
        }
    }
}



function ScreenMouseDown(X_, Y_, Btn)
{
    const X = ScreenMouseCalcX(X_);
    const Y = ScreenMouseCalcY(Y_);
    ScreenMouseAreaX1 = X;
    ScreenMouseAreaY1 = Y;
    if (((Btn & 1) > 0) && (!ScreenMouseBtn1))
    {
        if (ScreenMouseActive) ProgEventOther("Mouse", "Down", X, Y, 1, 0);
        ScreenMouseBtn1 = true;
    }
    if (((Btn & 4) > 0) && (!ScreenMouseBtn2))
    {
        if (ScreenMouseActive) ProgEventOther("Mouse", "Down", X, Y, 2, 0);
        ScreenMouseBtn2 = true;
    }
    if (((Btn & 2) > 0) && (!ScreenMouseBtn3))
    {
        if (ScreenMouseActive) ProgEventOther("Mouse", "Down", X, Y, 3, 0);
        ScreenMouseBtn3 = true;
    }
    if (((Btn & 256) > 0))
    {
        if (ScreenMouseActive) ProgEventOther("Mouse", "Down", X, Y, 4, 0);
    }
    if (((Btn & 512) > 0))
    {
        if (ScreenMouseActive) ProgEventOther("Mouse", "Down", X, Y, 5, 0);
    }
}

function ScreenMouseMove(X_, Y_, Btn)
{
    const X = ScreenMouseCalcX(X_);
    const Y = ScreenMouseCalcY(Y_);
    if ((X == ScreenMouseProgMoveX) && (Y == ScreenMouseProgMoveY))
    {
        return;
    }
    let TX = ScreenMouseProgMoveX;
    let TY = ScreenMouseProgMoveY;
    ScreenMouseProgMoveX = -1;
    ScreenMouseProgMoveY = -1;
    ScreenRepaintChar_(TX, TY);
    ScreenMouseProgMoveX = X;
    ScreenMouseProgMoveY = Y;
    ScreenRepaintChar_(X, Y);
    if (Btn != 0)
    {
        ScreenMouseHighlightRepaint();
    }
    if (ScreenMouseBtn1)
    {
        if (ScreenMouseActive) ProgEventOther("Mouse", "Move", X, Y, 1, 0);
    }
    else
    {
        if (ScreenMouseBtn2)
        {
            if (ScreenMouseActive) ProgEventOther("Mouse", "Move", X, Y, 2, 0);
        }
        else
        {
            if (ScreenMouseBtn3)
            {
                if (ScreenMouseActive) ProgEventOther("Mouse", "Move", X, Y, 3, 0);
            }
            else
            {
                if (ScreenMouseActive) ProgEventOther("Mouse", "Move", X, Y, 0, 0);
            }
        }
    }
}

function ScreenMouseUp(X_, Y_, Btn)
{
    const X = ScreenMouseCalcX(X_);
    const Y = ScreenMouseCalcY(Y_);
    //ScreenMouseHighlightRepaint();
    ScreenMouseAreaX2 = X;
    ScreenMouseAreaY2 = Y;
    if (((Btn & 1) == 0) && (ScreenMouseBtn1))
    {
        if (ScreenMouseActive) ProgEventOther("Mouse", "Up", X, Y, 1, 0);
        ScreenMouseBtn1 = false;
    }
    if (((Btn & 4) == 0) && (ScreenMouseBtn2))
    {
        if (ScreenMouseActive) ProgEventOther("Mouse", "Up", X, Y, 2, 0);
        ScreenMouseBtn2 = false;
    }
    if (((Btn & 2) == 0) && (ScreenMouseBtn3))
    {
        if (ScreenMouseActive) ProgEventOther("Mouse", "Up", X, Y, 3, 0);
        ScreenMouseBtn3 = false;
    }
    ScreenMouseScreenArea();
    
    
    if ((KeybTouch == 1) && (Btn == 0))
    {
        ScreenMouseMove(-1, -1, 0);
    }
}

let ScreenMouseLastX = 0;
let ScreenMouseLastY = 0;

function ScreenMouseDown_(Evt)
{
    if (KeybTouch == 0)
    {
        var _ = ScreenVP.getBoundingClientRect();
        ScreenMouseLastX = Evt.clientX + window.scrollX - _.left;
        ScreenMouseLastY = Evt.clientY + window.scrollX - _.top;
        ScreenMouseDown(ScreenMouseLastX, ScreenMouseLastY, Evt.buttons);
    }
}

function ScreenMouseMove_(Evt)
{
    if (KeybTouch == 0)
    {
        var _ = ScreenVP.getBoundingClientRect();
        ScreenMouseLastX = Evt.clientX + window.scrollX - _.left;
        ScreenMouseLastY = Evt.clientY + window.scrollX - _.top;
        ScreenMouseMove(ScreenMouseLastX, ScreenMouseLastY, Evt.buttons);
    }
}

function ScreenMouseUp_(Evt)
{
    if (KeybTouch == 0)
    {
        var _ = ScreenVP.getBoundingClientRect();
        ScreenMouseLastX = Evt.clientX + window.scrollX - _.left;
        ScreenMouseLastY = Evt.clientY + window.scrollX - _.top;
        ScreenMouseUp(ScreenMouseLastX, ScreenMouseLastY, Evt.buttons);
    }
}

function ScreenMouseScroll_(Evt)
{
    if (KeybTouch == 0)
    {
        if (Evt.deltaY < 0)
        {
            ScreenMouseDown(ScreenMouseLastX, ScreenMouseLastY, 256);
        }
        if (Evt.deltaY > 0)
        {
            ScreenMouseDown(ScreenMouseLastX, ScreenMouseLastY, 512);
        }
    }
}

function ScreenMouseDown_0(Evt)
{
    if ((KeybTouch == 1) && (Evt.touches.length > 0))
    {
        document.getElementsByTagName("BODY")[0].className = "lock-screen";
        var _ = ScreenVP.getBoundingClientRect();
        ScreenMouseLastX = Evt.touches[0].clientX + window.scrollX - _.left;
        ScreenMouseLastY = Evt.touches[0].clientY + window.scrollX - _.top;
        switch (KeybMouseBtnNr)
        {
            case 1:
                ScreenMouseDown(Evt.touches[0].clientX + window.scrollX - _.left, Evt.touches[0].clientY + window.scrollX - _.top, 1);
                break;
            case 2:
                ScreenMouseDown(Evt.touches[0].clientX + window.scrollX - _.left, Evt.touches[0].clientY + window.scrollX - _.top, 4);
                break;
            case 3:
                ScreenMouseDown(Evt.touches[0].clientX + window.scrollX - _.left, Evt.touches[0].clientY + window.scrollX - _.top, 2);
                break;
            case 4:
                ScreenMouseDown(Evt.touches[0].clientX + window.scrollX - _.left, Evt.touches[0].clientY + window.scrollX - _.top, 256);
                break;
            case 5:
                ScreenMouseDown(Evt.touches[0].clientX + window.scrollX - _.left, Evt.touches[0].clientY + window.scrollX - _.top, 512);
                break;
        }
    }
}

function ScreenMouseMove_0(Evt)
{
    if ((KeybTouch == 1) && (Evt.touches.length > 0))
    {
        var _ = ScreenVP.getBoundingClientRect();
        ScreenMouseLastX = Evt.touches[0].clientX + window.scrollX - _.left;
        ScreenMouseLastY = Evt.touches[0].clientY + window.scrollX - _.top;
        switch (KeybMouseBtnNr)
        {
            case 1:
                ScreenMouseMove(Evt.touches[0].clientX + window.scrollX - _.left, Evt.touches[0].clientY + window.scrollX - _.top, 1);
                break;
            case 2:
                ScreenMouseMove(Evt.touches[0].clientX + window.scrollX - _.left, Evt.touches[0].clientY + window.scrollX - _.top, 4);
                break;
            case 3:
                ScreenMouseMove(Evt.touches[0].clientX + window.scrollX - _.left, Evt.touches[0].clientY + window.scrollX - _.top, 2);
                break;
        }
    }
}

function ScreenMouseUp_0(Evt)
{
    if ((KeybTouch == 1))
    {
        document.getElementsByTagName("BODY")[0].className = "";
        var _ = ScreenVP.getBoundingClientRect();
        ScreenMouseUp(ScreenMouseLastX, ScreenMouseLastY, 0);
    }
}

function ScreenMouseReset(Evt)
{
    ScreenMouseAreaUse = false;
    ScreenMouseUp(ScreenMouseLastX, ScreenMouseLastY, 0);
    ScreenMouseAreaUse = true;
    ScreenMouseMove(-1, -1, 0);
    if ((KeybTouch == 1))
    {
        document.getElementsByTagName("BODY")[0].className = "";
    }
}

ScreenObj.addEventListener("mousedown", ScreenMouseDown_);
ScreenObj.addEventListener("mousemove", ScreenMouseMove_);
ScreenObj.addEventListener("mouseup", ScreenMouseUp_);
ScreenObj.addEventListener("mouseout", ScreenMouseReset);
ScreenObj.addEventListener("touchstart", ScreenMouseDown_0);
ScreenObj.addEventListener("touchmove", ScreenMouseMove_0);
ScreenObj.addEventListener("touchend", ScreenMouseUp_0);
ScreenObj.addEventListener("touchcancel", ScreenMouseReset);
ScreenObj.addEventListener("wheel", ScreenMouseScroll_);
ScreenObj.addEventListener("contextmenu", event => event.preventDefault(), false);

