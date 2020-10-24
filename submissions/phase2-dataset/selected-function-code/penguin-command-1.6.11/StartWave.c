void StartWave()
{
    int i,dummy=0;

    NewMissile=0;
    MissilesCreated=0;
    BlitToBB(0,0,BackPic);
    // New City?
    for (i=0;i<6;i++)
        if (!CityDestroyed[i]) dummy++;
    i=abrand(0,5);
    if (dummy<6) {
        if ((!NewCity)||(ArcadeMode)) {
    	    while (!CityDestroyed[i])
		i=abrand(0,5);
    	    CityDestroyed[i]=0;
	    if (i<3)
		Blit(70+i*100,490,CityPic);
	    else
		Blit(130+i*100,490,CityPic);
	    PlaySound(CitySound);
	    WaitWithMouse(100);
	    Update();
	    WaitWithMouse(200);
	    NewCity=1;
	}
	else NewCity=0;
    }
    CityNum=0;
    for (i=0;i<6;i++)
        if (!CityDestroyed[i]) CityNum++;
    // Draw Cities
    dummy=0;
    for (i=0;i<3;i++)
	if (CityDestroyed[i])
	    BlitToBB(70+i*100,490,CityHitPic);
	else
	    BlitToBB(70+i*100,490,CityPic);
    dummy=0;
    for (i=0;i<3;i++)
	if (CityDestroyed[i+3])
	    BlitToBB(430+i*100,490,CityHitPic);
	else
	    BlitToBB(430+i*100,490,CityPic);
    // Printf "Staring Wave" message
    AddScore(0);
    Blit(0,0,BackBuffer);
    sprintf(text,"Starting Wave #%d",WaveNum+1);
    PutString(Screen, 400-SFont_TextWidth(text)/2, 170, text);
    sprintf(text,"Score %d x",WaveNum/2+1);
    PutString(Screen, 400-SFont_TextWidth(text)/2, 220, text);
    DrawCenteredText(370, "Defend Cities!");
    for (i=0;i<3;i++) {
	if ( CannonDestroyed[i] ) {
	    ShotNum[i]=3;
	    CannonDestroyed[i]=0;
	} else
	    ShotNum[i]=10;
	DrawShotNum(i);
    }
    Update();
    // Wait 2 seconds
    WaitWithMouse(2000-1000*ArcadeMode);
    Blit(0,0,BackBuffer);
    for (i=0;i<3;i++) DrawShotNum(i);
    SDL_SetEventFilter(GameEvents);
}