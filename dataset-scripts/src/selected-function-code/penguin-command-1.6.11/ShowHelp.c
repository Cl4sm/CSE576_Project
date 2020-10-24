void ShowHelp()
{
    float anim=0;
    SDL_Rect rect;
    SDL_Event WaitEvent;
    char DetailString[100],LevelString[100],text[100];
    int Level=3,SoundV=3,MusicV=3;
	
    Blit(0,0,BackPic);
    ExplosionCount=0;
    XCenteredString(Screen,10,"< Penguin Command Help & Options >");
    PutString(Screen,20,60,"Objective: Defend cities!");
    PutString(Screen,20,100,"Controls: Mouse, or mouse + a,s,d keys");
    PutString(Screen,20,140,"Score:");
    PutString(Screen,150,185,"10    Remaining Shot");
    PutString(Screen,150,220,"20    Destroyed Missile");
    PutString(Screen,150,255,"30    Destroyed Smartbomb");
    PutString(Screen,150,290,"80    Remaining City");
    Blit(80-ShotPic->w/2,200-ShotPic->h/2,ShotPic);
    Blit(80-CityPic->w/2,310-CityPic->h/2,CityPic);
    XCenteredString(Screen,380,"< Settings >");
    SDL_BlitSurface(Screen,NULL,BackBuffer,NULL);
    SDL_BlitSurface(Screen,NULL,FadeBuffer,NULL);
    Update();
    Now=SDL_GetTicks();
    while (Level || SoundV || MusicV) {
		SDL_PollEvent(&WaitEvent);
		while (   (SDL_GetMouseState(NULL,NULL)==SDL_BUTTON(1))
				  ||(SDL_GetMouseState(NULL,NULL)==SDL_BUTTON(2))
				  ||(SDL_GetMouseState(NULL,NULL)==SDL_BUTTON(3)) )
	    {SDL_PollEvent(&WaitEvent);}
        while (WaitEvent.type!=SDL_KEYDOWN && WaitEvent.type!=SDL_MOUSEBUTTONDOWN) {
    	    SDL_PollEvent(&WaitEvent);
			UndrawMouse();
			// Explosion
			UndrawExplosion();
			if (ExplosionCount<1) CreateExplosion(80,235);
			HandleExplosion(0);
			DrawExplosion();
			// Bomb
			anim+=Speed/100;
			if ((int)anim>=10) anim-=10;
			rect.x=80-BombPic[0]->w/2; rect.y=270-BombPic[0]->h/2;
			rect.w=BombPic[0]->w; rect.h=BombPic[0]->h;
			SDL_BlitSurface( BackBuffer, &rect, Screen, &rect);
			Blit(rect.x,rect.y,BombPic[(int)anim]);
			AddThisRect(blitrect);
			// detail button
			if (LowDetail) strcpy(DetailString,"Low Detail");
			if (MediumDetail) strcpy(DetailString,"Medium Detail");
			if (HighDetail) strcpy(DetailString,"High Detail");
			//	    Detail=Button(400-SFont_TextWidth(DetailString)/2,430,DetailString,Detail,FadeBuffer);
			// level start button
			sprintf(LevelString,"Starting at Wave %d", StartLevel+1);
			Level=Button(400-SFont_TextWidth(LevelString)/2,430,LevelString,Level,FadeBuffer);
			// sound buttons
			sprintf(text,"Music Volume: %d", MusicVol);
			MusicV=Button(390-SFont_TextWidth(text),490,text,MusicV,FadeBuffer);
			sprintf(text,"Sound Volume: %d", SoundVol);
			SoundV=Button(410,490,text,SoundV,FadeBuffer);
			// display
			GetSpeed();
			DrawMouse();
			Update();
		}
		if (SoundV) {
			if (SDL_GetMouseState(NULL,NULL)==SDL_BUTTON(1))
				if (SoundVol<15) SoundVol++;
			if (SDL_GetMouseState(NULL,NULL)==SDL_BUTTON(3))
				if (SoundVol>0) SoundVol--;
			SoundV=3;
			rect.x=400;
			rect.y=490;
			rect.w=400;
			rect.h=Font->h;
			AddThisRect(rect);
            SDL_SetAlpha( FadeBuffer, 0, 0);
			SDL_BlitSurface( FadeBuffer, &rect, BackBuffer, &rect);
			SDL_BlitSurface( FadeBuffer, &rect, Screen, &rect);
			SetVol();
		}
		if (MusicV) {
			if (SDL_GetMouseState(NULL,NULL)==SDL_BUTTON(1))
				if (MusicVol<15) MusicVol++;
			if (SDL_GetMouseState(NULL,NULL)==SDL_BUTTON(3))
				if (MusicVol>0) MusicVol--;
			MusicV=3;
			rect.x=0;
			rect.y=490;
			rect.w=400;
			rect.h=Font->h;
			AddThisRect(rect);
            SDL_SetAlpha( FadeBuffer, 0, 0);
			SDL_BlitSurface( FadeBuffer, &rect, BackBuffer, &rect);
			SDL_BlitSurface( FadeBuffer, &rect, Screen, &rect);
#ifdef USE_SOUND
			Mix_VolumeMusic(MusicVol*7);
#endif
		}
		if (Level) {
			if (SDL_GetMouseState(NULL,NULL)==SDL_BUTTON(1))
				if (StartLevel<30) StartLevel+=5;
			if (SDL_GetMouseState(NULL,NULL)==SDL_BUTTON(3))
				if (StartLevel>0) StartLevel-=5;
			Level=3;
			rect.x=100;
			rect.y=430;
			rect.w=600;
			rect.h=Font->h;
            SDL_SetAlpha( FadeBuffer, 0, 0);
			SDL_BlitSurface( FadeBuffer, &rect, BackBuffer, &rect);
			SDL_BlitSurface( FadeBuffer, &rect, Screen, &rect);
			AddThisRect(rect);
		}
		SDL_Delay(10);
		while (SDL_GetMouseState(NULL,NULL)==SDL_BUTTON(1)) {SDL_PollEvent(&WaitEvent);SDL_Delay(10);}
    }
	//    SetDetail();
	//    LoadImages();
	//    SDL_ShowCursor(HWCursor);
}