const ProgInitScreenObj = document.getElementById("InitScreen");

function ProgInitScreen()
{
    if (ProgStarted)
    {
        ProgInitScreenObj.style.display = "none";
        ProgInitScreenObj.innerHTML = "";
    }
    else
    {
        const FontSizeW = BrowserW() / 10;
        const FontSizeH = BrowserH() / 8;
        
        ProgInitScreenObj.style.fontSize = Math.min(FontSizeW, FontSizeH) + "px";
        
        let Msg = "";
        Msg += "Storage open " + ((FileLsReady + FileDbReady) ? "- OK" : "...") + "<br>";
        Msg += "Core load " + (ProgLoaded ? "- OK" : "...") + "<br>";
        Msg += "Core init " + (ProgInitialized ? "- OK" : "...") + "<br>";
        Msg += "Screen prepare " + (ScreenStarted ? "- OK" : "...") + "<br>";
        //Msg += "Core run " + (ProgStarted ? "- OK" : "...") + "<br>";
        ProgInitScreenObj.innerHTML = Msg;
    }
}

ProgInitScreen();

