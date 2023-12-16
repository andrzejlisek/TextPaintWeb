ScreenTimerCounter = 0;
ScreenTimerPeriod = 100;

function ScreenTimerStart()
{
    if (ProgStarted)
    {
        ScreenTimerTick();
    }
    else
    {
        setTimeout(ScreenTimerStart, 20);
    }
}

function ScreenTimerTick()
{
    ScreenTimerCounter++;
    
    switch (ScreenTimerCounter)
    {
        case 1:
        case 3:
        case 7:
        case 9:
            ScreenDrawCursor(false);
            ProgEventTick();
            VTTEST_.VTData("");
            break;
        case 5:
            ScreenDrawCursor(false);
            ScreenDrawBlink(true);
            ProgEventTick();
            VTTEST_.VTData("");
            break;
        case 0:
        case 2:
        case 4:
        case 6:
        case 8:
            ScreenDrawCursor(true);
            ProgEventTick();
            VTTEST_.VTData("");
            break;
        case 10:
            ScreenDrawCursor(true);
            ScreenDrawBlink(false);
            ProgEventTick();
            VTTEST_.VTData("");
            ScreenTimerCounter = 0;
            break;
    }
    

    setTimeout(ScreenTimerTick, ScreenTimerPeriod);
}
