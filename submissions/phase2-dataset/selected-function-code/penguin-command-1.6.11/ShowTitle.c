void ShowTitle()
{
    int start=3,help=3,hiscore=3,quit=3,delay=0,credits=3,arcade=3;
    float NewExplosion=0;

    // show Title
    printf ("** Blit Background **\n");
    Blit(0,0,TitlePic);
    printf ("** Background image drawn **\n");
    if (!Loaded) {
	printf("** Loading Font **\n");
	Font = LoadImage("abc.png",4);
	printf("** Init Font **\n");
	InitFont( Font );
	//SDL_EventState(SDL_KEYUP, SDL_IGNORE);
	printf("** Loading Images **\n");
	// load...
	LoadImages();
	Loaded=1;
	MenuEvents();
    }
	SDL_SetAlpha(Font, SDL_SRCALPHA, 0);
    ExplosionCount=0;CursorAnim=0;
    SDL_BlitSurface(TitlePic,NULL,BackBuffer,NULL);
    // menu
    while (!Quit) {
	if (delay) {
	    Blit(0,0,TitlePic);
	    SDL_BlitSurface(TitlePic,NULL,BackBuffer,NULL);
	    while (event.type==SDL_MOUSEBUTTONDOWN)
		SDL_PollEvent(&event);
	    Now=SDL_GetTicks();
	    start=3;help=3;hiscore=3;quit=3;credits=3;arcade=3;
	}
	UndrawMouse();
	hiscore=Button(50,300,"Hiscores",hiscore,TitlePic);
	start=Button(400-SFont_TextWidth("Normal Game")/2,300,"Normal Game",start,TitlePic);
	arcade=Button(400-SFont_TextWidth("Arcade Game")/2,340,"Arcade Game",arcade,TitlePic);
	help=Button(750-SFont_TextWidth("Help&Conf"),300,"Help&Conf",help,TitlePic);
	credits=Button(750-SFont_TextWidth("Credits"),450,"Credits",credits,TitlePic);
	quit=Button(50,450,"Quit",quit,TitlePic);
	// Fancy explosions
	UndrawExplosion();
	if ((ExplosionCount<10)&&(NewExplosion<0)) {
	    CreateExplosion(abrand(0,799),abrand(0,280));
	    NewExplosion+=abrand(0,600)+50;
	}
	NewExplosion-=Speed;
	HandleExplosion(0);
	DrawExplosion();
	GetSpeed();
	// Explosions end
	DrawMouse();
	Update();
	delay=0;
	SDL_PollEvent(&event);
	if (event.type==SDL_MOUSEBUTTONDOWN) {
	    if (hiscore) {
		ShowHiscore();
		SDL_WaitEvent(&event);
	    }
	    if (start) {
		ArcadeMode=0;
		break;
	    }
	    if (arcade) {
		ArcadeMode=1;
		break;
	    }
	    if (help) ShowHelp();
	    if (credits) ShowCredits();
	    if (quit) Quit=1;
	    delay=1;
	}
    }
    ExplosionCount=0;
//    SDL_WaitEvent(&event);
}