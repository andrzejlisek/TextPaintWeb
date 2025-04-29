function ProgInit()
{
    ProgInitScreen();
    if (ProgLoaded && FileLsReady && FileDbReady)
    {
        _ProgInit = Module.cwrap("Init", null, ["string"]);
        _ProgEventKey = Module.cwrap("EventKey", null, ["string", "number", "number", "number", "number"]);
        _ProgEventTick = Module.cwrap("EventTick", null, null);
        ProgEventOther = Module.cwrap("EventOther", null, ["string", "string", "number", "number", "number", "number"]);
        ProgEventOtherFile = Module.cwrap("EventOtherFile", null, ["string", "string", "number", "number", "number", "number"]);
        _ProgInit("1");
    }
}

function ProgInitAfterConf()
{
    if (WebSocketObjectReady)
    {
        FileClpGuiRepaint();
        ConfRepaint();
        _ProgInit("3");
    }
    else
    {
        setTimeout(ProgInitAfterConf, 100);
    }
}






function WindowResize()
{
    ProgInitScreen();
    if (KeybReady)
    {
        FileClpGuiRepaint();
        ConfRepaint();
        KeybInit();
    }
}
window.addEventListener("resize", WindowResize, false);


function KeyPress(e)
{
    ScreenBellInit();

    if (!ProgStarted)
    {
        return;
    }

    if (GuiSettingsShown)
    {
        return;
    }

    if (FileClpGuiVisible())
    {
        return;
    }

    if (document.activeElement == KeybSvgTxt)
    {
        return;
    }

    if (KeybBlockShortcuts)
    {
        e.preventDefault();
    }

    //https://keycode.info/
    //console.log(e.keyCode);
    switch(e.keyCode)
    {
        default:
            // key  location  code  which  
            let Chr = 0;
            if (e.key.length == 1)
            {
                Chr = e.key.charCodeAt(0);
            }
            else
            {
                switch (e.key)
                {
                    case "Escape": Chr = 27; break;
                    case "Tab": Chr = 9; break;
                    case "Enter": Chr = 13; break;
                    case "Backspace": Chr = 8; break;
                }
            }
            switch (e.key)
            {
                case "Shift":
                case "Control":
                case "Alt":
                case "AltGraph":
                case "Meta":
                    break;
                default:
                    //console.log("KeyPress: " + "    " + Chr + "    " + e.code + "    " + e.key);
                    //ProgEventKey(Chr + "|" + (e.shiftKey ? "1" : "0") + (e.ctrlKey ? "1" : "0") + (e.altKey ? "1" : "0") + e.code);
                    ProgEventKey(e.code, Chr, (e.shiftKey ? 1 : 0), (e.ctrlKey ? 1 : 0), (e.altKey ? 1 : 0));
                    break;
            }
            //console.log(e);
            return false;
    }
}
document.addEventListener("keydown", KeyPress, false);


function goodbye(e)
{
    if (!DebugMode)
    {
        if(!e) e = window.event;
        //e.cancelBubble is supported by IE - this will kill the bubbling process.
        e.cancelBubble = true;
        e.returnValue = 'You sure you want to leave?'; //This is displayed on the dialog

        //e.stopPropagation works in Firefox.
        if (e.stopPropagation)
        {
            e.stopPropagation();
            e.preventDefault();
        }
    }
}
window.onbeforeunload = goodbye; 

