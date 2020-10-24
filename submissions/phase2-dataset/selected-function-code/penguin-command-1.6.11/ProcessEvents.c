void ProcessEvents()
{
    int i,Rand,b;

    SDL_PumpEvents();
    if (HighDetail) UndrawShot2(); else UndrawShot();
    UndrawFlyer();
    UndrawMissile();
    UndrawExplosion();
    UndrawBomb();
    if (!CannonDestroyed[0]) UndrawCannon(40,510);
    if (!CannonDestroyed[1]) UndrawCannon(400,510);
    if (!CannonDestroyed[2]) UndrawCannon(760,510);
    UndrawMouse();
    HandleMissile();
    if (HighDetail) HandleShot2(); else HandleShot();
    HandleFlyer();
    HandleExplosion(1);
    HandleBomb();
    HandleCursor();
    DrawMissile();
    if (HighDetail) DrawShot2(); else DrawShot();
    DrawBomb();
    DrawFlyer();
    DrawExplosion();
    if (!CannonDestroyed[0]) DrawCannon(40,510);
    if (!CannonDestroyed[1]) DrawCannon(400,510);
    if (!CannonDestroyed[2]) DrawCannon(760,510);
#ifdef JOYSTICK
    JoystickMove();
#endif
    DrawMouse();
    // Are all cities destroyed?
    if (!endgame) {
        endgame=1;
	for (i=0;i<6;i++)
	    if (CityDestroyed[i]==0)
		endgame=0;
    }
    // New Missiles
    if ((NewMissile<0)&&(Wave[WaveNum].MissilesToDestroy>MissilesCreated)) {
	Rand=abrand(0,99);
	if ((Rand<Wave[WaveNum].MirvChance)&&(MissileCount>0)) {
	    i=abrand(0,MissileCount-1);
	    b=0;
	    while ((Split(i))&&(b<100)) {
		i=abrand(0,MissileCount-1);
		b++;
	    }
	} else if (Rand>99-Wave[WaveNum].FlyerChance) {
	    CreateFlyer();
	} else if (Rand>99-Wave[WaveNum].BombChance-Wave[WaveNum].FlyerChance) {
	    CreateBomb();
	} else
	    CreateMissile(abrand(100,700),0);
	NewMissile+=Wave[WaveNum].TimeBetweenShots;
	MissilesCreated++;
    }
    NewMissile-=Speed;
    // New Wave
    if (
	(MissilesCreated==Wave[WaveNum].MissilesToDestroy)
	&&(MissileCount==0)
	&&(ExplosionCount==0)
	&&(BombCount==0)
	&&(FlyerCount==0)
	&&(ShotCount==0)
	) {
        WaveScoring();
	WaveNum++;
	StartWave();
    }    
}