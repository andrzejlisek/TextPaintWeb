let KeybTouch = 0;
let KeybLineWidth = 1;
let KeybTextSize = 12;
let KeybBack = 'rgb(128, 128, 128)';
let KeybFore = 'rgb(255, 255, 255)';
let KeybCellW = 20;
let KeybCellH = 20;


let StateShift = false;
let StateCtrl = false;
let StateAlt = false;
let StateCaps = false;

function KeybEvent(Evt)
{
    let RegularKey = false;
    switch (Evt[4])
    {
        case "_Shift":
            StateShift = !StateShift;
            break;
        case "_Ctrl":
            StateCtrl = !StateCtrl;
            break;
        case "_Alt":
            StateAlt = !StateAlt;
            break;
        case "_Caps":
            StateCaps = !StateCaps;
            break;
        case "_Switch0":
            KeybCurrent = 0;
            break;
        case "_Switch1":
            KeybCurrent = 1;
            break;
        case "_Switch2":
            KeybCurrent = 2;
            break;
        default:
            RegularKey = true;
            break;
    }
    
    if (RegularKey)
    {
        let KeyChr = 0;
        
        if (Evt.length >= 8)
        {
            KeyChr = Evt[7];
        }
        if ((Evt.length >= 9) && StateShift)
        {
            KeyChr = Evt[8];
        }
        
        if (Evt[4].length == 4)
        {
            if (Evt[4].substr(0, 3) == "Key")
            {
                if (StateCaps)
                {
                    KeyChr = StateShift ? Evt[7] : Evt[8];
                }
            }
        }
        
        ProgEventKey(Evt[4], KeyChr, StateShift, StateCtrl, StateAlt)

        StateShift = false;
        StateCtrl = false;
        StateAlt = false;
    }

}

function KeybEventText(E)
{
    if (E.inputType == "insertLineBreak")
    {
            KeybEvent([0, 0, 0, 0, "Enter", "", "", 13]);
    }
    if (E.data)
    {
        let Str = "" + E.data + "";
        for (let I = 0; I < Str.length; I++)
        {
            let EvtName = "Character";
            let EvtChar = Str.charCodeAt(I);
            if ((EvtChar >= 48) && (EvtChar <= 57)) EvtName = "Digit" + String.fromCharCode(EvtChar);
            if ((EvtChar >= 65) && (EvtChar <= 90)) EvtName = "Key" + String.fromCharCode(EvtChar);
            if ((EvtChar >= 97) && (EvtChar <= 122)) EvtName = "Key" + String.fromCharCode(EvtChar - 32);
            KeybEvent([0, 0, 0, 0, EvtName, "", "", EvtChar]);
        }
    }
    KeybSvgTxt.value = "";
}

let KeybSvg = document.getElementById("KeybSvg");
let KeybSvgBlank = document.getElementById("KeybSvgBlank");
let KeybSvgScr = document.getElementById("KeybSvgScr");
let KeybSvgTxt = document.getElementById("KeybSvgFld");

let KeybW = 640;
let KeybH = 480;



let KeybCellX = 0;
let KeybCellY = 0;



let KeybCurrent = 0;

// Keyboard layout
let KeybLayout = [[],[], []];

// Left part
KeybLayout[0].push([ 0,  0, 29, 13, "",             "", ""]);

KeybLayout[0].push([ 0,  0,  2,  2, "Escape",       "Esc", "", 27]);
KeybLayout[0].push([ 3,  0,  2,  2, "F1",           "F1",  ""]);
KeybLayout[0].push([ 5,  0,  2,  2, "F2",           "F2",  ""]);
KeybLayout[0].push([ 7,  0,  2,  2, "F3",           "F3",  ""]);
KeybLayout[0].push([ 9,  0,  2,  2, "F4",           "F4",  ""]);
KeybLayout[0].push([12,  0,  2,  2, "F5",           "F5",  ""]);
KeybLayout[0].push([14,  0,  2,  2, "F6",           "F6",  ""]);
KeybLayout[0].push([16,  0,  2,  2, "F7",           "F7",  ""]);
KeybLayout[0].push([18,  0,  2,  2, "F8",           "F8",  ""]);
KeybLayout[0].push([21,  0,  2,  2, "F9",           "F9",  ""]);
KeybLayout[0].push([23,  0,  2,  2, "F10",          "F10", ""]);
KeybLayout[0].push([25,  0,  2,  2, "F11",          "F11", ""]);
KeybLayout[0].push([27,  0,  2,  2, "F12",          "F12", ""]);

KeybLayout[0].push([ 0,  3,  2,  2, "Backquote",    "`", "~", 96, 126]);
KeybLayout[0].push([ 2,  3,  2,  2, "Digit1",       "1", "!", 49, 33]);
KeybLayout[0].push([ 4,  3,  2,  2, "Digit2",       "2", "@", 50, 64]);
KeybLayout[0].push([ 6,  3,  2,  2, "Digit3",       "3", "#", 51, 35]);
KeybLayout[0].push([ 8,  3,  2,  2, "Digit4",       "4", "$", 52, 36]);
KeybLayout[0].push([10,  3,  2,  2, "Digit5",       "5", "%", 53, 37]);
KeybLayout[0].push([12,  3,  2,  2, "Digit6",       "6", "^", 54, 94]);
KeybLayout[0].push([14,  3,  2,  2, "Digit7",       "7", "&", 55, 38]);
KeybLayout[0].push([16,  3,  2,  2, "Digit8",       "8", "*", 56, 42]);
KeybLayout[0].push([18,  3,  2,  2, "Digit9",       "9", "(", 57, 40]);
KeybLayout[0].push([20,  3,  2,  2, "Digit0",       "0", ")", 48, 41]);
KeybLayout[0].push([22,  3,  2,  2, "Minus",        "-", "_", 45, 95]);
KeybLayout[0].push([24,  3,  2,  2, "Equal",        "=", "+", 61, 43]);
KeybLayout[0].push([26,  3,  3,  2, "Backspace",    "Back", "", 8]);

KeybLayout[0].push([ 0,  5,  3,  2, "Tab",          "Tab", "", 9]);
KeybLayout[0].push([ 3,  5,  2,  2, "KeyQ",         "Q", "", 113, 81]);
KeybLayout[0].push([ 5,  5,  2,  2, "KeyW",         "W", "", 119, 87]);
KeybLayout[0].push([ 7,  5,  2,  2, "KeyE",         "E", "", 101, 69]);
KeybLayout[0].push([ 9,  5,  2,  2, "KeyR",         "R", "", 114, 82]);
KeybLayout[0].push([11,  5,  2,  2, "KeyT",         "T", "", 116, 84]);
KeybLayout[0].push([13,  5,  2,  2, "KeyY",         "Y", "", 121, 89]);
KeybLayout[0].push([15,  5,  2,  2, "KeyU",         "U", "", 117, 85]);
KeybLayout[0].push([17,  5,  2,  2, "KeyI",         "I", "", 105, 73]);
KeybLayout[0].push([19,  5,  2,  2, "KeyO",         "O", "", 111, 79]);
KeybLayout[0].push([21,  5,  2,  2, "KeyP",         "P", "", 112, 80]);
KeybLayout[0].push([23,  5,  2,  2, "BracketLeft",  "[", "{", 91, 123]);
KeybLayout[0].push([25,  5,  2,  2, "BracketRight", "]", "}", 93, 125]);
KeybLayout[0].push([27,  5,  2,  2, "Backslash",    "\\", "|", 92, 124]);

KeybLayout[0].push([ 0,  7,  4,  2, "_Caps",        "Lock", "Caps"]);
KeybLayout[0].push([ 4,  7,  2,  2, "KeyA",         "A", "", 97, 65]);
KeybLayout[0].push([ 6,  7,  2,  2, "KeyS",         "S", "", 115, 83]);
KeybLayout[0].push([ 8,  7,  2,  2, "KeyD",         "D", "", 100, 68]);
KeybLayout[0].push([10,  7,  2,  2, "KeyF",         "F", "", 102, 70]);
KeybLayout[0].push([12,  7,  2,  2, "KeyG",         "G", "", 103, 71]);
KeybLayout[0].push([14,  7,  2,  2, "KeyH",         "H", "", 104, 72]);
KeybLayout[0].push([16,  7,  2,  2, "KeyJ",         "J", "", 106, 74]);
KeybLayout[0].push([18,  7,  2,  2, "KeyK",         "K", "", 107, 75]);
KeybLayout[0].push([20,  7,  2,  2, "KeyL",         "L", "", 108, 76]);
KeybLayout[0].push([22,  7,  2,  2, "Semicolon",    ";", ":", 59, 58]);
KeybLayout[0].push([24,  7,  2,  2, "Quote",        "\'", "\"", 39, 34]);
KeybLayout[0].push([26,  7,  3,  2, "Enter",        "Enter", "", 13]);

KeybLayout[0].push([ 0,  9,  5,  2, "_Shift",       "Shift", ""]);
KeybLayout[0].push([ 5,  9,  2,  2, "KeyZ",         "Z", "", 122, 90]);
KeybLayout[0].push([ 7,  9,  2,  2, "KeyX",         "X", "", 120, 88]);
KeybLayout[0].push([ 9,  9,  2,  2, "KeyC",         "C", "", 99, 67]);
KeybLayout[0].push([11,  9,  2,  2, "KeyV",         "V", "", 118, 86]);
KeybLayout[0].push([13,  9,  2,  2, "KeyB",         "B", "", 98, 66]);
KeybLayout[0].push([15,  9,  2,  2, "KeyN",         "N", "", 110, 78]);
KeybLayout[0].push([17,  9,  2,  2, "KeyM",         "M", "", 109, 77]);
KeybLayout[0].push([19,  9,  2,  2, "Comma",        ",", "<", 44, 60]);
KeybLayout[0].push([21,  9,  2,  2, "Period",       ".", ">", 46, 62]);
KeybLayout[0].push([23,  9,  2,  2, "Slash",        "/", "?", 47, 63]);
KeybLayout[0].push([25,  9,  4,  2, "_Shift",       "Shift", ""]);

KeybLayout[0].push([ 0, 11,  3,  2, "_Ctrl",        "Ctrl", ""]);
KeybLayout[0].push([ 3, 11,  3,  2, "_Alt",         "Alt", ""]);
KeybLayout[0].push([ 6, 11,  2,  2, "ArrowLeft",    String.fromCharCode(0x2190), ""]);
KeybLayout[0].push([ 8, 11,  2,  2, "ArrowRight",   String.fromCharCode(0x2192), ""]);
KeybLayout[0].push([10, 11,  9,  2, "Space",        "Space", ""]);
KeybLayout[0].push([19, 11,  2,  2, "ArrowUp",      String.fromCharCode(0x2191), ""]);
KeybLayout[0].push([21, 11,  2,  2, "ArrowDown",    String.fromCharCode(0x2193), ""]);
KeybLayout[0].push([23, 11,  3,  2, "_Switch1",     "SWITCH", "KEYB"]);
KeybLayout[0].push([26, 11,  3,  2, "_Ctrl",        "Ctrl", ""]);

// Right part
KeybLayout[1].push([ 0,  0, 15, 13, "",             "", ""]);
KeybLayout[1].push([ 7,  0,  6,  2, "",             "", ""]);
KeybLayout[1].push([13,  0,  2,  2, "Backspace",    "Back", "", 8]);

KeybLayout[1].push([ 0,  0,  2,  2, "_Shift",       "Shift", ""]);
KeybLayout[1].push([ 2,  0,  2,  2, "_Ctrl",        "Ctrl", ""]);
KeybLayout[1].push([ 4,  0,  2,  2, "_Alt",         "Alt", ""]);

KeybLayout[1].push([ 0,  3,  2,  2, "Insert",       "Insert", ""]);
KeybLayout[1].push([ 0,  5,  2,  2, "Delete",       "Delete", ""]);
KeybLayout[1].push([ 2,  3,  2,  2, "Home",         "Home", ""]);
KeybLayout[1].push([ 2,  5,  2,  2, "End",          "End", ""]);
KeybLayout[1].push([ 4,  3,  2,  2, "PageUp",       "Up", "Page"]);
KeybLayout[1].push([ 4,  5,  2,  2, "PageDown",     "Down", "Page"]);

KeybLayout[1].push([ 0, 11,  2,  2, "ArrowLeft",    String.fromCharCode(0x2190), ""]);
KeybLayout[1].push([ 4, 11,  2,  2, "ArrowRight",   String.fromCharCode(0x2192), ""]);
KeybLayout[1].push([ 2,  9,  2,  2, "ArrowUp",      String.fromCharCode(0x2191), ""]);
KeybLayout[1].push([ 2, 11,  2,  2, "ArrowDown",    String.fromCharCode(0x2193), ""]);

KeybLayout[1].push([ 7,  3,  2,  2, "_Switch2",       "SWI", "KEYB"]);
KeybLayout[1].push([ 9,  3,  2,  2, "NumpadDivide",   "/", "", 47]);
KeybLayout[1].push([11,  3,  2,  2, "NumpadMultiply", "*", "", 42]);
KeybLayout[1].push([13,  3,  2,  2, "NumpadSubtract", "-", "", 45]);
KeybLayout[1].push([13,  5,  2,  4, "NumpadAdd",      "+", "", 43]);
KeybLayout[1].push([ 7,  5,  2,  2, "Numpad7",        "7", "", 55]);
KeybLayout[1].push([ 9,  5,  2,  2, "Numpad8",        "8", "", 56]);
KeybLayout[1].push([11,  5,  2,  2, "Numpad9",        "9", "", 57]);
KeybLayout[1].push([ 7,  7,  2,  2, "Numpad4",        "4", "", 52]);
KeybLayout[1].push([ 9,  7,  2,  2, "Numpad5",        "5", "", 53]);
KeybLayout[1].push([11,  7,  2,  2, "Numpad6",        "6", "", 54]);
KeybLayout[1].push([ 7,  9,  2,  2, "Numpad1",        "1", "", 49]);
KeybLayout[1].push([ 9,  9,  2,  2, "Numpad2",        "2", "", 50]);
KeybLayout[1].push([11,  9,  2,  2, "Numpad3",        "3", "", 51]);

KeybLayout[1].push([ 7, 11,  2,  2, "Numpad0",        "0", "", 48]);
KeybLayout[1].push([ 9, 11,  2,  2, "NumpadDecimal",  ",", "", 44]);
KeybLayout[1].push([11, 11,  2,  2, "NumpadDecimal",  ".", "", 46]);
KeybLayout[1].push([13,  9,  2,  4, "NumpadEnter",    "Enter", ""]);


// Animation player
KeybLayout[2].push([ 0,  0, 12,  8, "",             "", ""]);

KeybLayout[2].push([ 0,  0,  2,  4, "PageUp",       "Up", "Page"]);
KeybLayout[2].push([10,  0,  2,  4, "PageDown",     "Down", "Page"]);
KeybLayout[2].push([ 0,  4,  2,  4, "Backspace",    "Back", "", 8]);
KeybLayout[2].push([10,  4,  2,  4, "Enter",        "Enter", "", 13]);

KeybLayout[2].push([ 2,  0,  2,  2, "BracketLeft",  "[", "{", 91, 123]);
KeybLayout[2].push([ 4,  0,  2,  2, "Slash",        "/", "?", 47, 63]);
KeybLayout[2].push([ 6,  0,  2,  2, "_Switch0",     "SWI", "KEYB"]);
KeybLayout[2].push([ 8,  0,  2,  2, "BracketRight", "]", "}", 93, 125]);

KeybLayout[2].push([ 2,  2,  2,  2, "Escape",       "Esc", "", 27]);
KeybLayout[2].push([ 4,  2,  2,  2, "Tab",          "Tab", "", 9]);
KeybLayout[2].push([ 6,  2,  2,  2, "Backquote",    "`", "~", 96, 126]);
KeybLayout[2].push([ 8,  2,  2,  2, "Period",       ".", ">", 46, 62]);

KeybLayout[2].push([ 2,  6,  2,  2, "Home",         "Home", ""]);
KeybLayout[2].push([ 4,  6,  4,  2, "Space",        "Space", ""]);
KeybLayout[2].push([ 8,  6,  2,  2, "End",          "End", ""]);

KeybLayout[2].push([ 2,  4,  2,  2, "ArrowLeft",    String.fromCharCode(0x2190), ""]);
KeybLayout[2].push([ 4,  4,  2,  2, "ArrowUp",      String.fromCharCode(0x2191), ""]);
KeybLayout[2].push([ 6,  4,  2,  2, "ArrowDown",    String.fromCharCode(0x2193), ""]);
KeybLayout[2].push([ 8,  4,  2,  2, "ArrowRight",   String.fromCharCode(0x2192), ""]);


function KeybRepaint()
{
    while (KeybSvg.firstChild)
    {
        KeybSvg.removeChild(KeybSvg.lastChild);
    }
    
    let Temp = document.createElementNS("http://www.w3.org/2000/svg", 'rect');
    Temp.setAttribute("x", 0 - 10);
    Temp.setAttribute("y", 0 - 10);
    Temp.setAttribute("width", KeybW + 20);
    Temp.setAttribute("height", KeybH + 20);
    Temp.setAttribute("shape-rendering", "crispEdges");
    Temp.setAttribute("stroke", "none");
    Temp.setAttribute("stroke-width", 0);
    Temp.setAttribute("fill", KeybBack);
    KeybSvg.appendChild(Temp);
    
    KeybCellW = Math.floor(KeybW / (KeybLayout[KeybCurrent][0][2] * 2));
    KeybCellH = Math.floor(KeybH / (KeybLayout[KeybCurrent][0][3] * 2));
    KeybCellW = KeybCellW * 2;
    KeybCellH = KeybCellH * 2;
    KeybCellX = Math.floor((KeybW - (KeybLayout[KeybCurrent][0][2] * KeybCellW)) / 2);
    KeybCellY = Math.floor((KeybH - (KeybLayout[KeybCurrent][0][3] * KeybCellH)) / 2);

    let KeybCellWH = (KeybCellW < KeybCellH) ? KeybCellW : KeybCellH;
    KeybTextSize = Math.floor(3 * (KeybCellWH / 4));

    
    for (let I = 1; I < KeybLayout[KeybCurrent].length; I++)
    {
        let K = KeybLayout[KeybCurrent][I];
        let K_X = (K[0] * KeybCellW) + KeybCellX;
        let K_Y = (K[1] * KeybCellH) + KeybCellY;
        let K_W = (K[2] * KeybCellW);
        let K_H = (K[3] * KeybCellH);
        
        let KeybBack_ = KeybBack;
        let KeybFore_ = KeybFore;
        switch (K[4])
        {
            case "_Shift":
                if (StateShift)
                {
                    KeybBack_ = KeybFore;
                    KeybFore_ = KeybBack;
                }
                break;
            case "_Ctrl":
                if (StateCtrl)
                {
                    KeybBack_ = KeybFore;
                    KeybFore_ = KeybBack;
                }
                break;
            case "_Alt":
                if (StateAlt)
                {
                    KeybBack_ = KeybFore;
                    KeybFore_ = KeybBack;
                }
                break;
            case "_Caps":
                if (StateCaps)
                {
                    KeybBack_ = KeybFore;
                    KeybFore_ = KeybBack;
                }
                break;
        }

        Temp = document.createElementNS("http://www.w3.org/2000/svg", 'rect');
        Temp.setAttribute("x", K_X);
        Temp.setAttribute("y", K_Y);
        Temp.setAttribute("width", K_W);
        Temp.setAttribute("height", K_H);
        Temp.setAttribute("shape-rendering", "crispEdges");
        Temp.setAttribute("stroke", KeybFore);
        Temp.setAttribute("stroke-width", KeybLineWidth);
        Temp.setAttribute("fill", KeybBack_);
        KeybSvg.appendChild(Temp);

        let Txt1 = K[5];
        let Txt2 = K[6];

        if (Txt2 == "")
        {
            Temp = document.createElementNS("http://www.w3.org/2000/svg", 'text');
            Temp.setAttribute("x", K_X + K_W / 2);
            Temp.setAttribute("y", K_Y + K_H / 2);
            Temp.setAttribute("text-anchor", "middle");
            Temp.setAttribute("dominant-baseline", "central");
            //Temp.setAttribute("alignment-baseline", "middle");
            Temp.setAttribute("font-size", KeybTextSize + "px");
            Temp.setAttribute("font-family", "Arial, Helvetica, Verdana, sans-serif");

            let Temp_ = document.createTextNode(Txt1);
            Temp.appendChild(Temp_);
            Temp.setAttribute("fill", KeybFore_);
            KeybSvg.appendChild(Temp);
        }
        else
        {
            Temp = document.createElementNS("http://www.w3.org/2000/svg", 'text');
            Temp.setAttribute("x", K_X + (K_W / 2));
            Temp.setAttribute("y", K_Y + (K_H / 2) - (K_H / 5));
            Temp.setAttribute("text-anchor", "middle");
            Temp.setAttribute("dominant-baseline", "central");
            //Temp.setAttribute("alignment-baseline", "middle");
            Temp.setAttribute("font-size", KeybTextSize + "px");
            Temp.setAttribute("font-family", "Arial, Helvetica, Verdana, sans-serif");

            let Temp_ = document.createTextNode(Txt2);
            Temp.appendChild(Temp_);
            Temp.setAttribute("fill", KeybFore_);
            KeybSvg.appendChild(Temp);

            Temp = document.createElementNS("http://www.w3.org/2000/svg", 'text');
            Temp.setAttribute("x", K_X + (K_W / 2));
            Temp.setAttribute("y", K_Y + (K_H / 2) + (K_H / 5));
            Temp.setAttribute("text-anchor", "middle");
            Temp.setAttribute("dominant-baseline", "central");
            //Temp.setAttribute("alignment-baseline", "middle");
            Temp.setAttribute("font-size", KeybTextSize + "px");
            Temp.setAttribute("font-family", "Arial, Helvetica, Verdana, sans-serif");

            Temp_ = document.createTextNode(Txt1);
            Temp.appendChild(Temp_);
            Temp.setAttribute("fill", KeybFore_);
            KeybSvg.appendChild(Temp);
        }
    }

    Temp = document.createElementNS("http://www.w3.org/2000/svg", 'rect');
    Temp.setAttribute("x", 0 - 10);
    Temp.setAttribute("y", 0 - 10);
    Temp.setAttribute("width", KeybW + 20);
    Temp.setAttribute("height", KeybH + 20);
    Temp.setAttribute("shape-rendering", "crispEdges");
    Temp.setAttribute("stroke", "none");
    Temp.setAttribute("stroke-width", 0);
    Temp.setAttribute("fill", "#80808001");
    KeybSvg.appendChild(Temp);
    
    switch (KeybCurrent)
    {
        case 0:
        case 2:
            {
                KeybSvgTxt.style.left = "0px";
                KeybSvgTxt.style.top = "0px";
                KeybSvgTxt.style.width = "1px";
                KeybSvgTxt.style.height = "1px";
                KeybSvgTxt.style.display = "none";
            }
            break;
        case 1:
            {
                let K = KeybLayout[1][1];
                let K_X = (K[0] * KeybCellW) + KeybCellX;
                let K_Y = (K[1] * KeybCellH) + KeybCellY;
                let K_W = (K[2] * KeybCellW);
                let K_H = (K[3] * KeybCellH);

                KeybSvgTxt.style.left = K_X + "px";
                KeybSvgTxt.style.top = K_Y + "px";
                KeybSvgTxt.style.width = K_W + "px";
                KeybSvgTxt.style.height = K_H + "px";
                KeybSvgTxt.style.display = "block";
            }
            break;
    }
}

var KeybMouseBtn = false;
var KeybMouseX = 0;
var KeybMouseY = 0;
var KeybMouseX_ = 0;
var KeybMouseY_ = 0;


function KeybMouseDown(X, Y)
{
    KeybMouseX = X;
    KeybMouseY = Y;
    KeybMouseX_ = X;
    KeybMouseY_ = Y;

    let KeybCurrent_ = KeybCurrent;
    for (let I = 1; I < KeybLayout[KeybCurrent_].length; I++)
    {
        let K = KeybLayout[KeybCurrent_][I];
        let K_X1 = (K[0] * KeybCellW) + KeybCellX;
        let K_Y1 = (K[1] * KeybCellH) + KeybCellY;
        let K_X2 = (K[2] * KeybCellW) + K_X1;
        let K_Y2 = (K[3] * KeybCellH) + K_Y1;
        if ((X >= K_X1) && (X < K_X2) && (Y >= K_Y1) && (Y < K_Y2))
        {
            KeybEvent(KeybLayout[KeybCurrent_][I]);
        }
    }

    KeybRepaint();
}

function KeybMouseMove(X, Y)
{
}

function KeybMouseUp(X, Y)
{
    KeybRepaint();
}




function KeybMouseDown_(Evt)
{
    if (KeybTouch == 0)
    {
        var _ = KeybSvg.getBoundingClientRect();
        KeybMouseBtn = true;
        KeybMouseDown(Evt.clientX + window.scrollX - _.left, Evt.clientY + window.scrollX - _.top);
    }
}

function KeybMouseMove_(Evt)
{
    if (KeybTouch == 0)
    {
        if (KeybMouseBtn)
        {
            var _ = KeybSvg.getBoundingClientRect();
            KeybMouseMove(Evt.clientX + window.scrollX - _.left, Evt.clientY + window.scrollX - _.top);
        }
    }
}

function KeybMouseUp_(Evt)
{
    if (KeybTouch == 0)
    {
        var _ = KeybSvg.getBoundingClientRect();
        KeybMouseUp(Evt.clientX + window.scrollX - _.left, Evt.clientY + window.scrollX - _.top);
        KeybMouseBtn = false;
    }
}


function KeybMouseDown_0(Evt)
{
    if ((KeybTouch == 1) && (Evt.touches.length > 0))
    {
        document.getElementsByTagName("BODY")[0].className = "lock-screen";
        var _ = KeybSvg.getBoundingClientRect();
        KeybMouseBtn = true;
        KeybMouseDown(Evt.touches[0].clientX + window.scrollX - _.left, Evt.touches[0].clientY + window.scrollX - _.top);
    }
}

function KeybMouseMove_0(Evt)
{
    if ((KeybTouch == 1) && (Evt.touches.length > 0))
    {
        if (KeybMouseBtn)
        {
            var _ = KeybSvg.getBoundingClientRect();
            KeybMouseMove(Evt.touches[0].clientX + window.scrollX - _.left, Evt.touches[0].clientY + window.scrollX - _.top);
        }
    }
}

function KeybMouseUp_0(Evt)
{
    if ((KeybTouch == 1))
    {
        document.getElementsByTagName("BODY")[0].className = "";
        var _ = KeybSvg.getBoundingClientRect();
        KeybMouseUp(0, 0);
        KeybMouseBtn = false;
    }
}

function KeybMouseReset(Evt)
{
    if ((KeybTouch == 1))
    {
        document.getElementsByTagName("BODY")[0].className = "";
    }
}


KeybSvg.addEventListener("mousedown", KeybMouseDown_);
KeybSvg.addEventListener("mousemove", KeybMouseMove_);
KeybSvg.addEventListener("mouseup", KeybMouseUp_);
KeybSvg.addEventListener("touchstart", KeybMouseDown_0);
KeybSvg.addEventListener("touchmove", KeybMouseMove_0);
KeybSvg.addEventListener("touchend", KeybMouseUp_0);
KeybSvg.addEventListener("touchcancel", KeybMouseReset);
KeybSvgFld.addEventListener('input', KeybEventText);

function KeybSetSize(W, H, H0)
{
    KeybSvg.setAttribute("width", W);
    KeybSvg.setAttribute("height", H);
    KeybSvgBlank.setAttribute("width", W);
    KeybSvgBlank.setAttribute("height", H0);
    KeybSvgScr.setAttribute("width", W);
    KeybSvgScr.setAttribute("height", H);
}

function KeybColorToStyle(N)
{
    let HexR = ScreenPaletteR[N].toString(16).padStart(2, '0');
    let HexG = ScreenPaletteG[N].toString(16).padStart(2, '0');
    let HexB = ScreenPaletteB[N].toString(16).padStart(2, '0');
    return "#" + HexR + HexG + HexB;
}

function KeybInit()
{
    let WinW = window.innerWidth;
    let WinH = window.innerHeight;

    let Obj1 = ConfigFileI("WinScreenSize");
    let Obj2 = ConfigFileI("WinKeyboardSize");
    
    let KeybColorB = HexToNum4(ConfigFileS("ColorKeyboard")[0]);
    let KeybColorF = HexToNum4(ConfigFileS("ColorKeyboard")[1]);
    
    ScreenDiv.style.width = Math.floor(WinW) + "px";
    ScreenDiv.style.height = Math.floor((Obj1 * WinH) / 100) + "px";

    document.getElementById("app").style["background-color"] = KeybColorToStyle(KeybColorB);
    document.getElementById("screendiv").style["background-color"] = KeybColorToStyle(KeybColorB);
    
    KeybBack = KeybColorToStyle(KeybColorB);
    KeybFore = KeybColorToStyle(KeybColorF);

    KeybW = Math.floor(WinW);
    KeybH = Math.floor((Obj2 * WinH) / 100);
    
    KeybSetSize(KeybW, KeybH, Math.floor(WinH));
    KeybRepaint();
    KeybReady = true;
    
    ScreenDisplayResize();
}

