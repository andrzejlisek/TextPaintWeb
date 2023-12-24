function ScreenTimerStart()
{
    if (ProgStarted)
    {
        ScreenTimerTick();
    }
    else
    {
        setTimeout(ScreenTimerStart, WaitTimeout);
    }
}

function ScreenTimerTick()
{
    ScreenTimerCounter++;
    if (ScreenTimerCounter == ScreenTimerCounterLoop)
    {
        ScreenTimerCounter = 0;
    }
    
    if ((ScreenTimerCounter % ScreenTimerCursor) == 0)
    {
        ScreenTimerCursorDisp = !ScreenTimerCursorDisp;
        if (ScreenTimerCursorDisp)
        {
            ScreenDrawCursor(true);
        }
        else
        {
            ScreenDrawCursor(ScreenCursorSteady);
        }
    }

    if ((ScreenTimerCounter % ScreenTimerBlink) == 0)
    {
        ScreenTimerBlinkDisp = !ScreenTimerBlinkDisp;
        if (ScreenTimerBlinkDisp)
        {
            ScreenDrawBlink(true);
        }
        else
        {
            ScreenDrawBlink(false);
        }
    }
 
    if ((ScreenTimerCounter % ScreenTimerTickEvent) == 0)
    {
        ProgEventTick();
    }

    VTTEST_.VTData("");

    setTimeout(ScreenTimerTick, ScreenTimerPeriod);
}
