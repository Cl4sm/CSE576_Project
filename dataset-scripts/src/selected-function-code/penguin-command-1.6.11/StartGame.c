void StartGame()
{
    int i;
    
    InitWaves();
    for (i=0;i<6;i++) CityDestroyed[i]=0;
    for (i=0;i<3;i++) CannonDestroyed[i]=0;
    ShotCount=0;
    MissileCount=0;
    ExplosionCount=0;
    BombCount=0;
    FlyerCount=0;
    WaveNum=StartLevel;
    Score=0;
    NewCity=0;
    MissilesCreated=0;

    if (ArcadeMode) ShowRules();
    SDL_SetEventFilter(GameEvents);
    StartWave();
    Now=SDL_GetTicks();
    Flicker=1;endgame=0;
    while ((!endgame)&&(!Quit)) {
        FullUpdate();
        GetSpeed();
	SDL_PollEvent(&event);
	ProcessEvents();
        //SDL_Delay(60);
    }
#ifdef USE_SOUND
//    Mix_FadeOutMusic(4000);
#endif
    PlaySound(GameoverSound);
    i=0;
    if (!Quit) 
	while (i<2000) {
    	    FullUpdate();
    	    GetSpeed();
	    ProcessEvents();
	    i+=Speed;
	}
    Flicker=0;
    printf("FPS %f\n",FramesPerSecond);
    SDL_SetEventFilter(NormalEvents);
}