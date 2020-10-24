void ShowRules()
{
    int i;

    Blit(0,0,BackPic);
    for (i=0;i<6;i++)
	if (i<3)
	    Blit(70+i*100,490,CityPic);
	else
	    Blit(130+i*100,490,CityPic);
    
    XCenteredString(Screen,50,"< Arcade Mode >");
    PutString(Screen,10,160,"-No extra points for remaining shots.");
    PutString(Screen,10,200,"-More cities result in faster shot regeneration.");
    PutString(Screen,10,240,"-One city is regenerated per wave.");
    PutString(Screen,10,280,"-The game is more difficult.");
    XCenteredString(Screen,400,"click to continue");
    SDL_BlitSurface(Screen, NULL, BackBuffer, NULL);
    Update();
    while (SDL_GetMouseState(NULL,NULL)==SDL_BUTTON(1)) {
	SDL_PollEvent(&event);
	SDL_Delay(10);
    }
    while ((event.type!=SDL_KEYDOWN)&&(event.type!=SDL_MOUSEBUTTONDOWN)) {
	SDL_PollEvent(&event);
	UndrawMouse();
	DrawMouse();
	GetSpeed();
	Update();
    }
}