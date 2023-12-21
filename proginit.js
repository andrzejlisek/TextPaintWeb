function ProgInit()
{
    if (ProgLoaded)
    {
        Module.ccall("Init", null, ["string"], ["1"]);
    }
    else
    {
        setTimeout(ProgInit, WaitTimeout);
    }
}

function ProgInitResult1()
{
    if (FileSystem0)
    {
        Module.ccall("Init", null, ["string"], ["2"]);
    }
    else
    {
        setTimeout(ProgInitResult1, WaitTimeout);
    }
}

function ProgInitResult2()
{
    if (FileSystem0)
    {
        Module.ccall("Init", null, ["string"], ["3"]);
    }
    else
    {
        setTimeout(ProgInitResult2, WaitTimeout);
    }
}

function ProgInitResult3()
{
    ScreenInit1();
}

function ProgInitAfterConf()
{
    ConfRepaint();
    Module.ccall("Init", null, ["string"], ["4"]);
}



function WindowResize()
{
    if (KeybReady)
    {
        ConfRepaint();
        KeybInit();
    }
}

window.addEventListener("resize", WindowResize, false);

