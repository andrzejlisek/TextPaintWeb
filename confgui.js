let ConfLbl1 = document.getElementById("ConfLbl1");
let ConfLbl2 = document.getElementById("ConfLbl2");
let ConfLbl3 = document.getElementById("ConfLbl3");
let ConfLbl4 = document.getElementById("ConfLbl4");
let ConfLbl5 = document.getElementById("ConfLbl5");
let ConfLbl6 = document.getElementById("ConfLbl6");
let ConfLbl7 = document.getElementById("ConfLbl7");
let ConfLbl8 = document.getElementById("ConfLbl8");
let ConfLbl9 = document.getElementById("ConfLbl9");

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
let ConfBtn36 = document.getElementById("ConfBtn36");

let ConfBtn41 = document.getElementById("ConfBtn41");
let ConfBtn42 = document.getElementById("ConfBtn42");
let ConfBtn43 = document.getElementById("ConfBtn43");
let ConfBtn44 = document.getElementById("ConfBtn44");
let ConfBtn45 = document.getElementById("ConfBtn45");
let ConfBtn46 = document.getElementById("ConfBtn46");

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
let ConfBtn65 = document.getElementById("ConfBtn65");

let ConfBtn71 = document.getElementById("ConfBtn71");
let ConfBtn72 = document.getElementById("ConfBtn72");
let ConfBtn73 = document.getElementById("ConfBtn73");
let ConfBtn74 = document.getElementById("ConfBtn74");
let ConfBtn75 = document.getElementById("ConfBtn75");

let ConfBtn81 = document.getElementById("ConfBtn81");
let ConfBtn82 = document.getElementById("ConfBtn82");
let ConfBtn83 = document.getElementById("ConfBtn83");
let ConfBtn84 = document.getElementById("ConfBtn84");
let ConfBtn85 = document.getElementById("ConfBtn85");

let ConfBtn91 = document.getElementById("ConfBtn91");
let ConfBtn92 = document.getElementById("ConfBtn92");
let ConfBtn93 = document.getElementById("ConfBtn93");
let ConfBtn94 = document.getElementById("ConfBtn94");
let ConfBtn95 = document.getElementById("ConfBtn95");

let ConfBtnAll = [];
ConfBtnAll.push([ConfBtn11, ConfBtn12, ConfBtn13, ConfBtn14]);
ConfBtnAll.push([ConfBtn21, ConfBtn22, ConfBtn23, ConfBtn24]);
ConfBtnAll.push([ConfBtn31, ConfBtn32, ConfBtn33, ConfBtn34, ConfBtn35, ConfBtn36]);
ConfBtnAll.push([ConfBtn41, ConfBtn42, ConfBtn43, ConfBtn44, ConfBtn45, ConfBtn46]);
ConfBtnAll.push([ConfBtn51, ConfBtn52, ConfBtn53, ConfBtn54, ConfBtn55, ConfBtn56]);
ConfBtnAll.push([ConfBtn61, ConfBtn62, ConfBtn63, ConfBtn64, ConfBtn65]);
ConfBtnAll.push([ConfBtn71, ConfBtn72, ConfBtn73, ConfBtn74, ConfBtn75]);
ConfBtnAll.push([ConfBtn81, ConfBtn82, ConfBtn83, ConfBtn84, ConfBtn85]);
ConfBtnAll.push([ConfBtn91, ConfBtn92, ConfBtn93, ConfBtn94, ConfBtn95]);

let ConfLblAll = [ConfLbl1, ConfLbl2, ConfLbl3, ConfLbl4, ConfLbl5, ConfLbl6, ConfLbl7, ConfLbl8, ConfLbl9];

let ConfScreenStep = [1, 10, 100, 1000, 10000, 100000, 1000000];

function ConfScreenStepDisp(Idx, Dir, Val)
{
    if (Idx >= 0)
    {
        if (Dir > 0)
        {
            return ["", "-", "+"][Dir] + ConfScreenStep[Idx];
        }
        else
        {
            if (Dir == (-1))
            {
                Val = Val - ConfScreenStep[Idx];
                if (Val < 1) Val = 1;
                return Val;
            }
            else
            {
                if (Val > 1000000000) Val = 1000000000;
                return Val + ConfScreenStep[Idx];
            }
        }
    }
    else
    {
        if (Dir > 0)
        {
            return ["", "/", "*"][Dir] + (1 - Idx);
        }
        else
        {
            if (Dir == (-1))
            {
                Val = Math.round(Val / (1 - Idx));
                if (Val < 1) Val = 1;
                return Val;
            }
            else
            {
                Val = Math.round(Val * (1 - Idx));
                if (Val > 1000000000) Val = 1000000000;
                if (Val < 1) Val = 1000000000;
                return Val;
            }
        }
    }
}


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
    ConfBtn35.value = ScreenSET_ANSIIgnoreConcealed ? "Ignore\nconce" : "Use\nconce";
    ConfBtn36.value = ScreenColorBlending ? "Blend\ncolors" : "Do not\nblend";

    if (ScreenSET_Blink == 0) ConfBtn41.value = "Steady";
    if (ScreenSET_Blink == 1) ConfBtn41.value = "VTx\nblink";
    if (ScreenSET_Blink == 2) ConfBtn41.value = "DOS\nblink";
    ConfBtn42.value = ScreenAttrB ? "Use\nbold" : "No\nbold";
    ConfBtn43.value = ScreenAttrI ? "Use\nItalic" : "No\nItalic";
    ConfBtn44.value = ScreenAttrU ? "Use\nunder" : "No\nunder";
    ConfBtn45.value = ScreenAttrS ? "Use\nstrike" : "No\nstrike";
    if (ScreenDisplayInterpolate == 0) ConfBtn46.value = "Stretch\nNo inter";
    if (ScreenDisplayInterpolate == 1) ConfBtn46.value = "Stretch\nInter";
    if (ScreenDisplayInterpolate == 2) ConfBtn46.value = "No\nstretch";


    let ScrS = ScreenW + "x" + ScreenH;

    ConfBtn61.value = ConfigFileB("WinAuto") ? "Auto\nresize" : "No\nresize";
    ConfBtn62.value = ScrS + "\n-10 cols";
    ConfBtn63.value = ScrS + "\n-1 col";
    ConfBtn64.value = ScrS + "\n+1 col";
    ConfBtn65.value = ScrS + "\n+10 cols";

    ConfBtn71.value = ConfigFileB("WinAuto") ? "Auto\nresize" : "No\nresize";
    ConfBtn72.value = ScrS + "\n-10 rows";
    ConfBtn73.value = ScrS + "\n-1 row";
    ConfBtn74.value = ScrS + "\n+1 row";
    ConfBtn75.value = ScrS + "\n+10 rows";

    if (ScreenDisplayCursor == 0) ConfBtn81.value = "Hidden\ncursor";
    if (ScreenDisplayCursor == 1) ConfBtn81.value = "Blinking\nunder";
    if (ScreenDisplayCursor == 2) ConfBtn81.value = "Steady\nunder";
    if (ScreenDisplayCursor == 3) ConfBtn81.value = "Blinking\nbar";
    if (ScreenDisplayCursor == 4) ConfBtn81.value = "Steady\nbar";
    if (ScreenDisplayCursor == 5) ConfBtn81.value = "Blinking\nbox";
    if (ScreenDisplayCursor == 6) ConfBtn81.value = "Steady\nbox";
    
    ConfBtn82.value = "ANI " + ConfigFileI("TimerStep") + "\n" + ConfScreenStepDisp(ConfigFileI("TimerStepChange"), 1);
    ConfBtn83.value = "ANI " + ConfigFileI("TimerStep") + "\n" + ConfScreenStepDisp(ConfigFileI("TimerStepChange"), 2);
    ConfBtn84.value = "TXT " + ConfigFileI("TimerStepText") + "\n" + ConfScreenStepDisp(ConfigFileI("TimerStepChange"), 1);
    ConfBtn85.value = "TXT " + ConfigFileI("TimerStepText") + "\n" + ConfScreenStepDisp(ConfigFileI("TimerStepChange"), 2);

    if (ConfigFileI("ANSIScrollSmooth") == 0) ConfBtn91.value = "No\nscroll";
    if (ConfigFileI("ANSIScrollSmooth") == 1) ConfBtn91.value = "Scroll\n1 step";
    if (ConfigFileI("ANSIScrollSmooth") == 2) ConfBtn91.value = "Scroll\n2 steps";
    if (ConfigFileI("ANSIScrollSmooth") == 3) ConfBtn91.value = "Scroll\n4 steps";
    if (ConfigFileI("ANSIScrollSmooth") == 4) ConfBtn91.value = "Scroll\n8 steps";
    if (ConfigFileI("ANSIScrollSmooth") == 5) ConfBtn91.value = "Scroll\n16 steps";

    ConfBtn92.value = ConfigFileI("ANSIScrollChars") + "\n" + ConfScreenStepDisp(ConfigFileI("TimerStepChange"), 1);
    ConfBtn93.value = ConfigFileI("ANSIScrollChars") + "\n" + ConfScreenStepDisp(ConfigFileI("TimerStepChange"), 2);
    ConfBtn94.value = "Dec\nChange";
    ConfBtn95.value = "Inc\nChange";
    
    const FntSizeW = Math.floor((BrowserW() * 1.0) / 40.0);
    const FntSizeH = Math.floor((BrowserH() * 1.0) / 30.0);
    const FntSize = Math.min(FntSizeW, FntSizeH);
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
    if (ScreenMouseActive)
    {
        return;
    }
    if ((typeof ScreenMouseShowConfig) == "function")
    {
        if (!ScreenMouseShowConfig())
        {
            return;
        }
    }
    GuiSettingsShown = true;
    document.getElementById("ConfGui").style["display"] = "block";
    ConfRepaint();
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
            {
                if (confirm("Reset configuration?"))
                {
                    ConfigFileReset();
                    if (confirm("Clear file storage?"))
                    {
                        IndexClear();
                    }
                }
                else
                {
                    location.reload(true);
                }
            }
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
            ConfigFileGui("ANSIColors", !ScreenSET_ANSIColors);
            ScreenSetDisplayConfig(true);
            break;
        case 32:
            ConfigFileGui("ANSIReverseMode", (ScreenSET_ANSIReverseMode + 1) % 3);
            ScreenSetDisplayConfig(true);
            break;
        case 33:
            ConfigFileGui("ANSIColorBold", !ScreenSET_ANSIColorBold);
            ScreenSetDisplayConfig(true);
            break;
        case 34:
            ConfigFileGui("ANSIColorBlink", !ScreenSET_ANSIColorBlink);
            ScreenSetDisplayConfig(true);
            break;
        case 35:
            ConfigFileGui("ANSIIgnoreConcealed", !ScreenSET_ANSIIgnoreConcealed);
            ScreenSetDisplayConfig(true);
            break;
        case 36:
            ConfigFileGui("ColorBlending", !ScreenColorBlending);
            ScreenSetDisplayConfig(true);
            break;
            
        case 41:
            ConfigFileGui("DisplayBlink", (ScreenSET_Blink + 1) % 3);
            ScreenSetDisplayConfig(true);
            break;
        case 42:
            Temp = 0;
            if (!ScreenAttrB) Temp += 1;
            if ( ScreenAttrI) Temp += 2;
            if ( ScreenAttrU) Temp += 4;
            if ( ScreenAttrS) Temp += 8;
            ConfigFileGui("DisplayAttrib", Temp);
            ScreenSetDisplayConfig(true);
            break;
        case 43:
            Temp = 0;
            if ( ScreenAttrB) Temp += 1;
            if (!ScreenAttrI) Temp += 2;
            if ( ScreenAttrU) Temp += 4;
            if ( ScreenAttrS) Temp += 8;
            ConfigFileGui("DisplayAttrib", Temp);
            ScreenSetDisplayConfig(true);
            break;
        case 44:
            Temp = 0;
            if ( ScreenAttrB) Temp += 1;
            if ( ScreenAttrI) Temp += 2;
            if (!ScreenAttrU) Temp += 4;
            if ( ScreenAttrS) Temp += 8;
            ConfigFileGui("DisplayAttrib", Temp);
            ScreenSetDisplayConfig(true);
            break;
        case 45:
            Temp = 0;
            if ( ScreenAttrB) Temp += 1;
            if ( ScreenAttrI) Temp += 2;
            if ( ScreenAttrU) Temp += 4;
            if (!ScreenAttrS) Temp += 8;
            ConfigFileGui("DisplayAttrib", Temp);
            ScreenSetDisplayConfig(true);
            break;
        case 46:
            ConfigFileGui("DisplayInterpolate", (ScreenDisplayInterpolate + 1) % 3);
            ScreenSetDisplayConfig(true);
            break;            

        case 61:
            Temp = ConfigFileB("WinAuto");
            Temp = !Temp;
            ConfigFileGui("WinAuto", Temp);
            break;
        case 62:
            ProgEventOtherFile("Resize", "", ScreenW - 10, ScreenH, 1, 0);
            break;
        case 63:
            ProgEventOtherFile("Resize", "", ScreenW -  1, ScreenH, 1, 0);
            break;
        case 64:
            ProgEventOtherFile("Resize", "", ScreenW +  1, ScreenH, 1, 0);
            break;
        case 65:
            ProgEventOtherFile("Resize", "", ScreenW + 10, ScreenH, 1, 0);
            break;
            
        case 71:
            Temp = ConfigFileB("WinAuto");
            Temp = !Temp;
            ConfigFileGui("WinAuto", Temp);
            break;
        case 72:
            ProgEventOtherFile("Resize", "", ScreenW, ScreenH - 10, 1, 0);
            break;
        case 73:
            ProgEventOtherFile("Resize", "", ScreenW, ScreenH -  1, 1, 0);
            break;
        case 74:
            ProgEventOtherFile("Resize", "", ScreenW, ScreenH +  1, 1, 0);
            break;
        case 75:
            ProgEventOtherFile("Resize", "", ScreenW, ScreenH + 10, 1, 0);
            break;

        case 81:
            ScreenDisplayCursor = (ScreenDisplayCursor + 1) % 7;
            ConfigFileGui("DisplayCursor", ScreenDisplayCursor);
            ScreenSetDisplayConfigCursorReset();
            ScreenSetDisplayConfigCursor();
            break;
        case 82:
            Temp = ConfScreenStepDisp(ConfigFileI("TimerStepChange"), -1, ConfigFileI("TimerStep"));
            ConfigFileGui("TimerStep", Temp);
            break;
        case 83:
            Temp = ConfScreenStepDisp(ConfigFileI("TimerStepChange"), -2, ConfigFileI("TimerStep"));
            ConfigFileGui("TimerStep", Temp);
            break;
        case 84:
            Temp = ConfScreenStepDisp(ConfigFileI("TimerStepChange"), -1, ConfigFileI("TimerStepText"));
            ConfigFileGui("TimerStepText", Temp);
            break;
        case 85:
            Temp = ConfScreenStepDisp(ConfigFileI("TimerStepChange"), -2, ConfigFileI("TimerStepText"));
            ConfigFileGui("TimerStepText", Temp);
            break;

        case 91:
            Temp = ConfigFileI("ANSIScrollSmooth");
            Temp = (Temp + 1) % 6;
            ConfigFileGui("ANSIScrollSmooth", Temp);
            break;
        case 92:
            Temp = ConfScreenStepDisp(ConfigFileI("TimerStepChange"), -1, ConfigFileI("ANSIScrollChars"));
            ConfigFileGui("ANSIScrollChars", Temp);
            break;
        case 93:
            Temp = ConfScreenStepDisp(ConfigFileI("TimerStepChange"), -2, ConfigFileI("ANSIScrollChars"));
            ConfigFileGui("ANSIScrollChars", Temp);
            break;
        case 94:
            Temp = ConfigFileI("TimerStepChange");
            if (Temp > -9) Temp--;
            ConfigFileGui("TimerStepChange", Temp);
            break;
        case 95:
            Temp = ConfigFileI("TimerStepChange");
            if (Temp < (ConfScreenStep.length - 1)) Temp++;
            ConfigFileGui("TimerStepChange", Temp);
            break;
    }
    
    ConfRepaint();
}

