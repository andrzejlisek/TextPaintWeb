

let ConfLbl1 = document.getElementById("ConfLbl1");
let ConfLbl2 = document.getElementById("ConfLbl2");
let ConfLbl3 = document.getElementById("ConfLbl3");
let ConfLbl4 = document.getElementById("ConfLbl4");
let ConfLbl5 = document.getElementById("ConfLbl5");
let ConfLbl6 = document.getElementById("ConfLbl6");
let ConfLbl7 = document.getElementById("ConfLbl7");

let ConfBtn11 = document.getElementById("ConfBtn11");
let ConfBtn12 = document.getElementById("ConfBtn12");
let ConfBtn13 = document.getElementById("ConfBtn13");
let ConfBtn14 = document.getElementById("ConfBtn14");

let ConfBtn21 = document.getElementById("ConfBtn21");
let ConfBtn22 = document.getElementById("ConfBtn22");
let ConfBtn23 = document.getElementById("ConfBtn23");
let ConfBtn24 = document.getElementById("ConfBtn24");

let ConfBtn31 = document.getElementById("ConfBtn31");
let ConfBtn32 = document.getElementById("ConfBtn32");
let ConfBtn33 = document.getElementById("ConfBtn33");
let ConfBtn34 = document.getElementById("ConfBtn34");
let ConfBtn35 = document.getElementById("ConfBtn35");

let ConfBtn41 = document.getElementById("ConfBtn41");
let ConfBtn42 = document.getElementById("ConfBtn42");
let ConfBtn43 = document.getElementById("ConfBtn43");
let ConfBtn44 = document.getElementById("ConfBtn44");
let ConfBtn45 = document.getElementById("ConfBtn45");

let ConfBtn51 = document.getElementById("ConfBtn51");
let ConfBtn52 = document.getElementById("ConfBtn52");
let ConfBtn53 = document.getElementById("ConfBtn53");
let ConfBtn54 = document.getElementById("ConfBtn54");
let ConfBtn55 = document.getElementById("ConfBtn55");
let ConfBtn56 = document.getElementById("ConfBtn56");

let ConfBtn61 = document.getElementById("ConfBtn61");
let ConfBtn62 = document.getElementById("ConfBtn62");
let ConfBtn63 = document.getElementById("ConfBtn63");
let ConfBtn64 = document.getElementById("ConfBtn64");

let ConfBtn71 = document.getElementById("ConfBtn71");
let ConfBtn72 = document.getElementById("ConfBtn72");
let ConfBtn73 = document.getElementById("ConfBtn73");
let ConfBtn74 = document.getElementById("ConfBtn74");

let ConfBtnAll = [];
ConfBtnAll.push([ConfBtn11, ConfBtn12, ConfBtn13, ConfBtn14]);
ConfBtnAll.push([ConfBtn21, ConfBtn22, ConfBtn23, ConfBtn24]);
ConfBtnAll.push([ConfBtn31, ConfBtn32, ConfBtn33, ConfBtn34, ConfBtn35]);
ConfBtnAll.push([ConfBtn41, ConfBtn42, ConfBtn43, ConfBtn44, ConfBtn45]);
ConfBtnAll.push([ConfBtn51, ConfBtn52, ConfBtn53, ConfBtn54, ConfBtn55, ConfBtn56]);
ConfBtnAll.push([ConfBtn61, ConfBtn62, ConfBtn63, ConfBtn64]);
ConfBtnAll.push([ConfBtn71, ConfBtn72, ConfBtn73, ConfBtn74]);

let ConfLblAll = [ConfLbl1, ConfLbl2, ConfLbl3, ConfLbl4, ConfLbl5, ConfLbl6, ConfLbl7];

function ConfIsFullScreen()
{
    if (!document.mozFullScreen && !document.webkitIsFullScreen && !document.fullscreen)
    {
        return false;
    }
    else
    {
        return true;
    }
}

function ConfFullScreen()
{
    var VideoElement = document.getElementById("app");
    if (!document.mozFullScreen && !document.webkitIsFullScreen && !document.fullscreen)
    {
        if (VideoElement.mozRequestFullScreen)
        {
            VideoElement.mozRequestFullScreen();
        }
        else
        {
            if (VideoElement.webkitRequestFullScreen)
            {
                VideoElement.webkitRequestFullScreen(Element.ALLOW_KEYBOARD_INPUT);
            }
            else
            {
                if (VideoElement.requestFullscreen)
                {
                    VideoElement.requestFullscreen();
                }
            }
        }
    }
    else
    {
        if (document.mozCancelFullScreen)
        {
            document.mozCancelFullScreen();
        }
        else
        {
            if (document.webkitCancelFullScreen)
            {
                document.webkitCancelFullScreen();
            }
            else
            {
                if (document.exitFullscreen)
                {
                    document.exitFullscreen();
                }
            }
        }
    }
}


function ConfRepaint()
{
    ConfBtn11.value = (KeybTouch == 1) ? "Touch\nscreen" : "Mouse";
    ConfBtn12.value = "Restart";
    ConfBtn13.value = ConfIsFullScreen() ? "Full\nscreen" : "Window";
    ConfBtn14.value = "Return\nto program";
    //ConfBtn12.value = KeybBlockShortcuts ? "Block\nshortcuts" : "Allow\nshortcuts";

    ConfBtn21.value = "+ " + ConfigFileI("WinScreenSize") + " +\nScreen";
    ConfBtn22.value = "- " + ConfigFileI("WinScreenSize") + " -\nScreen";
    ConfBtn23.value = "+ " + ConfigFileI("WinKeyboardSize") + " +\nKeyboard";
    ConfBtn24.value = "- " + ConfigFileI("WinKeyboardSize") + " -\nKeyboard";

    if (ConfigFileI("ANSIDOS") == 0) ConfBtn51.value = "VTx\nISO";
    if (ConfigFileI("ANSIDOS") == 1) ConfBtn51.value = "DOS";
    if (ConfigFileI("ANSIDOS") == 2) ConfBtn51.value = "VTx\nDEC";
    ConfBtn52.value = (ConfigFileI("ANSI8bit")) ? "8-bit ctrl" : "7-bit ctrl";
    ConfBtn53.value = (ConfigFileI("ANSIPrintBackspace")) ? "Backsp\nas char" : "Backsp\nas ctrl";
    ConfBtn54.value = (ConfigFileI("ANSIPrintTab")) ? "Tab\nas char" : "Tab\nas ctrl";
    if (ConfigFileI("ANSIReadCR") == 0) ConfBtn55.value = "CR\nas CR";
    if (ConfigFileI("ANSIReadCR") == 1) ConfBtn55.value = "CR\nas CR+LF";
    if (ConfigFileI("ANSIReadCR") == 2) ConfBtn55.value = "Ignore\nCR";
    if (ConfigFileI("ANSIReadLF") == 0) ConfBtn56.value = "LF\nas LF";
    if (ConfigFileI("ANSIReadLF") == 1) ConfBtn56.value = "LF\nas CR+LF";
    if (ConfigFileI("ANSIReadLF") == 2) ConfBtn56.value = "Ignore\nLF";

    ConfBtn31.value = ScreenSET_ANSIColors ? "Color" : "Mono";
    if (ScreenSET_ANSIReverseMode == 0) ConfBtn32.value = "No\nreverse";
    if (ScreenSET_ANSIReverseMode == 1) ConfBtn32.value = "Reverse\nbefore";
    if (ScreenSET_ANSIReverseMode == 2) ConfBtn32.value = "Reverse\nafter";
    ConfBtn33.value = ScreenSET_ANSIColorBold ? "Bold\nas color" : "No bold\ncolor";
    ConfBtn34.value = ScreenSET_ANSIColorBlink ? "Blink\nas color" : "No blink\ncolor";
    ConfBtn35.value = ScreenSET_ANSIIgnoreConcealed ? "Use\nconce" : "Ignore\nconce";

    if (ScreenSET_Blink == 0) ConfBtn41.value = "Steady";
    if (ScreenSET_Blink == 1) ConfBtn41.value = "VTx\nblink";
    if (ScreenSET_Blink == 2) ConfBtn41.value = "DOS\nblink";
    ConfBtn42.value = ScreenAttrB ? "Use\nbold" : "No\nbold";
    ConfBtn43.value = ScreenAttrI ? "Use\nItalic" : "No\nItalic";
    ConfBtn44.value = ScreenAttrU ? "Use\nunder" : "No\nunder";
    ConfBtn45.value = ScreenAttrS ? "Use\nstrike" : "No\nstrike";

    let ScrS = ScreenW + "x" + ScreenH;

    ConfBtn61.value = ScrS + "\n-10 cols";
    ConfBtn62.value = ScrS + "\n-1 col";
    ConfBtn63.value = ScrS + "\n+1 col";
    ConfBtn64.value = ScrS + "\n+10 cols";

    ConfBtn71.value = ScrS + "\n-10 rows";
    ConfBtn72.value = ScrS + "\n-1 row";
    ConfBtn73.value = ScrS + "\n+1 row";
    ConfBtn74.value = ScrS + "\n+10 rows";
    
    let FntSizeW = Math.floor((window.innerWidth * 1.0) / 40.0);
    let FntSizeH = Math.floor((window.innerHeight * 1.0) / 30.0);
    let FntSize = (FntSizeW < FntSizeH) ? FntSizeW : FntSizeH;
    for (let I = 0; I < ConfBtnAll.length; I++)
    {
        for (let II = 0; II < ConfBtnAll[I].length; II++)
        {
            ConfBtnAll[I][II].style["font-size"] = FntSize + "px";
        }
    }
    for (let I = 0; I < ConfLblAll.length; I++)
    {
        ConfLblAll[I].style["font-size"] = FntSize + "px";
    }
}

function ConfShow()
{
    GuiSettingsShown = true;
    document.getElementById("ConfGui").style["display"] = "block";
}

function ConfClick(id)
{
    let Temp;
    switch (id)
    {
        case 11:
            KeybTouch = 1 - KeybTouch;
            ConfigFileGui("WinTouchScreen", KeybTouch);
            break;
        case 12:
            location.reload(true);
            //KeybBlockShortcuts = !KeybBlockShortcuts;
            break;
        case 13:
            ConfFullScreen();
            break;
        case 14:
            document.getElementById("ConfGui").style["display"] = "none";
            GuiSettingsShown = false;
            break;

        case 21:
            Temp = ConfigFileI("WinScreenSize");
            Temp = Temp + 5;
            if (Temp > 100) Temp = 100;
            ConfigFileGui("WinScreenSize", Temp);
            KeybInit();
            break;
        case 22:
            Temp = ConfigFileI("WinScreenSize");
            Temp = Temp - 5;
            if (Temp < 0) Temp = 0;
            ConfigFileGui("WinScreenSize", Temp);
            KeybInit();
            break;
        case 23:
            Temp = ConfigFileI("WinKeyboardSize");
            Temp = Temp + 5;
            if (Temp > 100) Temp = 100;
            ConfigFileGui("WinKeyboardSize", Temp);
            KeybInit();
            break;
        case 24:
            Temp = ConfigFileI("WinKeyboardSize");
            Temp = Temp - 5;
            if (Temp < 0) Temp = 0;
            ConfigFileGui("WinKeyboardSize", Temp);
            KeybInit();
            break;

        case 51:
            Temp = ConfigFileI("ANSIDOS");
            Temp++;
            if (Temp == 3) Temp = 0;
            ConfigFileGui("ANSIDOS", Temp);
            break;
        case 52:
            Temp = ConfigFileB("ANSI8bit");
            Temp = !Temp;
            ConfigFileGui("ANSI8bit", Temp);
            break;
        case 53:
            Temp = ConfigFileB("ANSIPrintBackspace");
            Temp = !Temp;
            ConfigFileGui("ANSIPrintBackspace", Temp);
            break;
        case 54:
            Temp = ConfigFileB("ANSIPrintTab");
            Temp = !Temp;
            ConfigFileGui("ANSIPrintTab", Temp);
            break;
        case 55:
            Temp = ConfigFileI("ANSIReadCR");
            Temp++;
            if (Temp == 3) Temp = 0;
            ConfigFileGui("ANSIReadCR", Temp);
            break;
        case 56:
            Temp = ConfigFileI("ANSIReadLF");
            Temp++;
            if (Temp == 3) Temp = 0;
            ConfigFileGui("ANSIReadLF", Temp);
            break;

        case 31:
            ScreenSET_ANSIColors = !ScreenSET_ANSIColors;
            ConfigFileGui("ANSIColors", ScreenSET_ANSIColors);
            ScreenSetDisplayConfig(true);
            break;
        case 32:
            ScreenSET_ANSIReverseMode++;
            if (ScreenSET_ANSIReverseMode == 3) ScreenSET_ANSIReverseMode = 0;
            ConfigFileGui("ANSIReverseMode", ScreenSET_ANSIReverseMode);
            ScreenSetDisplayConfig(true);
            break;
        case 33:
            ScreenSET_ANSIColorBold = !ScreenSET_ANSIColorBold;
            ConfigFileGui("ANSIColorBold", ScreenSET_ANSIColorBold);
            ScreenSetDisplayConfig(true);
            break;
        case 34:
            ScreenSET_ANSIColorBlink = !ScreenSET_ANSIColorBlink;
            ConfigFileGui("ANSIColorBlink", ScreenSET_ANSIColorBlink);
            ScreenSetDisplayConfig(true);
            break;
        case 35:
            ScreenSET_ANSIIgnoreConcealed = !ScreenSET_ANSIIgnoreConcealed;
            ConfigFileGui("ANSIIgnoreConcealed", ScreenSET_ANSIIgnoreConcealed);
            ScreenSetDisplayConfig(true);
            break;
            
        case 41:
            ScreenSET_Blink++;
            if (ScreenSET_Blink==3) ScreenSET_Blink = 0;
            ConfigFileGui("DisplayBlink", ScreenSET_Blink);
            ScreenSetDisplayConfig(true);
            break;
        case 42:
            ScreenAttrB = !ScreenAttrB;
            Temp = 0;
            if (ScreenAttrB) Temp += 1;
            if (ScreenAttrI) Temp += 2;
            if (ScreenAttrU) Temp += 4;
            if (ScreenAttrS) Temp += 8;
            ConfigFileGui("DisplayAttrib", Temp);
            ScreenSetDisplayConfig(true);
            break;
        case 43:
            ScreenAttrI = !ScreenAttrI;
            Temp = 0;
            if (ScreenAttrB) Temp += 1;
            if (ScreenAttrI) Temp += 2;
            if (ScreenAttrU) Temp += 4;
            if (ScreenAttrS) Temp += 8;
            ConfigFileGui("DisplayAttrib", Temp);
            ScreenSetDisplayConfig(true);
            break;
        case 44:
            ScreenAttrU = !ScreenAttrU;
            Temp = 0;
            if (ScreenAttrB) Temp += 1;
            if (ScreenAttrI) Temp += 2;
            if (ScreenAttrU) Temp += 4;
            if (ScreenAttrS) Temp += 8;
            ConfigFileGui("DisplayAttrib", Temp);
            ScreenSetDisplayConfig(true);
            break;
        case 45:
            ScreenAttrS = !ScreenAttrS;
            Temp = 0;
            if (ScreenAttrB) Temp += 1;
            if (ScreenAttrI) Temp += 2;
            if (ScreenAttrU) Temp += 4;
            if (ScreenAttrS) Temp += 8;
            ConfigFileGui("DisplayAttrib", Temp);
            ScreenSetDisplayConfig(true);
            break;
            
        case 61:
            ScreenW -= 10;
            ScreenResize(ScreenW, ScreenH);
            ProgEventOtherFile("Resize", "", ScreenW, ScreenH, 0, 0);
            break;
        case 62:
            ScreenW -= 1;
            ScreenResize(ScreenW, ScreenH);
            ProgEventOtherFile("Resize", "", ScreenW, ScreenH, 0, 0);
            break;
        case 63:
            ScreenW += 1;
            ScreenResize(ScreenW, ScreenH);
            ProgEventOtherFile("Resize", "", ScreenW, ScreenH, 0, 0);
            break;
        case 64:
            ScreenW += 10;
            ScreenResize(ScreenW, ScreenH);
            ProgEventOtherFile("Resize", "", ScreenW, ScreenH, 0, 0);
            break;
            
        case 71:
            ScreenH -= 10;
            ScreenResize(ScreenW, ScreenH);
            ProgEventOtherFile("Resize", "", ScreenW, ScreenH, 0, 0);
            break;
        case 72:
            ScreenH -= 1;
            ScreenResize(ScreenW, ScreenH);
            ProgEventOtherFile("Resize", "", ScreenW, ScreenH, 0, 0);
            break;
        case 73:
            ScreenH += 1;
            ScreenResize(ScreenW, ScreenH);
            ProgEventOtherFile("Resize", "", ScreenW, ScreenH, 0, 0);
            break;
        case 74:
            ScreenH += 10;
            ScreenResize(ScreenW, ScreenH);
            ProgEventOtherFile("Resize", "", ScreenW, ScreenH, 0, 0);
            break;
    }
    
    ConfRepaint();
}

