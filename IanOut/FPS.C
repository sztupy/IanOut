DWORD                   dwFrameCount;
DWORD                   dwFrameTime;
DWORD                   dwFrames;
DWORD                   dwFramesLast;


void DisplayFrameRate( void )
{
    DWORD               time2;
    char                buff[256];
    
    dwFrameCount++;
    time2 = timeGetTime() - dwFrameTime;
    if( time2 > 1000 )
    {
        dwFrames = (dwFrameCount*1000)/time2;
        dwFrameTime = timeGetTime();
        dwFrameCount = 0;
    }
    if( dwFrames == 0 )
    {
        return;
    }
    
    if (dwFrames != dwFramesLast)
    {
        dwFramesLast = dwFrames;
    }
    
    if( dwFrames > 99 )
    {
        dwFrames = 99;
    }
    buff[0] = (char)((dwFrames / 10) + '0');
    buff[1] = (char)((dwFrames % 10) + '0');
    buff[2] = '\0';
    bltScore(buff, ScreenX/2-25, 10);
}
