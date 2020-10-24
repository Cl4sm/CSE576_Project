void WaveScoring()
{
    int x,i,OldScore;

    UndrawMouse();
    SDL_SetEventFilter(NormalEvents);
    DrawCenteredText(140,"Wave Scoring");
    x=400-(ShotNum[0]+ShotNum[1]+ShotNum[2])*4;
//    printf("1: %d\t2: %d\t3: %d\tE: %d\t\n",ShotNum[0],ShotNum[1],ShotNum[2],x);
    OldScore=Score;
    if (!ArcadeMode)
    for (i=0;i<3;i++) {
        while(ShotNum[i]>0) {
	    Blit(x,200,ShotPic);
	    x+=8;
	    AddScore(10);
	    DrawShotNum(i);
	    sprintf(text,"  %d  ", Score-OldScore);
	    DrawCenteredText(235, text);
	    Update();
	    WaitWithMouse(70);
	    ShotNum[i]--;
	}
	DrawShotNum(i);
    }
    x=400;
    OldScore=Score;
    for (i=0;i<6;i++)
	if (!CityDestroyed[i]) x-=50;
    for (i=0;i<6;i++)
	if (!CityDestroyed[i]) {
	    Blit(x,285-ArcadeMode*100,CityPic);
	    x+=100;
	    AddScore(80);
	    sprintf(text,"  %d  ", Score-OldScore);
	    DrawCenteredText(380-ArcadeMode*100, text);
	    Update();
	    WaitWithMouse(400-ArcadeMode*100);
	}
    WaitWithMouse(3000-ArcadeMode*1500);
    Now=SDL_GetTicks();
//    sprintf(text,"Starting Wave #%d",WaveNum+1);
//    PutString(Screen, 400-SFont_TextWidth(text)/2, 300, text);
}