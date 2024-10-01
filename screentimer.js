let ScreenTimerFramesX = 0;

function ScreenTimerStart()
{
    if (ScreenStarted && ProgStarted)
    {
        ScreenTimerFramesX = 0;
        requestAnimationFrame(ScreenTimerTick);
    }
}

function ScreenTimerTick()
{
    if (ScreenTimerFramesX > 0)
    {
        ScreenTimerFramesX--;
        requestAnimationFrame(ScreenTimerTick);
        return;
    }
    ScreenTimerFramesX = ScreenTimerFrames - 1;

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
            ScreenDrawCursor(ScreenDisplayCursor1);
        }
        else
        {
            ScreenDrawCursor(ScreenDisplayCursor0);
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
    requestAnimationFrame(ScreenTimerTick);
}

