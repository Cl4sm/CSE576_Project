int main(int argc, char *argv[])
{
// intialisation
//    if ( SDL_Init(SDL_INIT_AUDIO ) < 0 ) ComplainAndExit();
    ReadCommandLine(argv);
    printf("** Starting SDL init **\n");
    init_SDL();
    SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
    SDL_WM_SetCaption("Penguin Command","Penguin Command");
    srand((unsigned)time(NULL));
#ifdef JOYSTICK
/* joystick */
    printf("** Finding Joysticks **\n");
    JoystickInit();
#endif
// load data
    printf("** Loading Title Screen **\n");
    TitlePic = LoadImage("title.jpg",0);
    InitSound();
// set volume
    ReadOptions();
// show title screen
    printf("** Playing Music **\n");
    SetVol();
    SDL_EnableUNICODE(1);
//    LoadCursor();
//    SetDetail();
    SDL_ShowCursor(HWCursor);
    printf("** Ready to enter Menu **\n");
// start game
	PlayMusic();
    while (!Quit) {
#ifdef USE_SOUND
		if (sound) {
			printf("** Fade in Music **\n");
			//Mix_FadeInMusic(Music[abrand(0,MUSICNUM-1)],0,5000);
		}
#endif
		printf("** Showing Title Screen **\n");
		ShowTitle();
		if (Quit) break;
		StartGame();
		FinalScore();
		ShowHiscore();
    }
// user wants to quit
#ifdef USE_SOUND
    Mix_FadeOutMusic(1000);
#endif
    {SDL_Rect rect; rect.x=0;rect.y=0;rect.w=800;rect.h=600; SDL_FillRect(BackBuffer,&rect,0);}
    FadeScreen(0.9);
    WriteOptions();
//    printf("Awaiting SDL_Quit()\n");
    SDL_Quit();
//    printf("SDL_Quit() finished.\n");
    return 0;
}