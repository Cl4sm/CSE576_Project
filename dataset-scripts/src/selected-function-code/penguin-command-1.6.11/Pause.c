void Pause(const SDL_Event *event)
{
    pause=1;    

    // clean input queue
    while ( event->type == SDL_KEYDOWN )
	SDL_PollEvent((SDL_Event *)event);
	
    // write message to screen
    SDL_BlitSurface(Screen,NULL,FadeBuffer,NULL);
    XCenteredString(Screen,250,"Game Paused");
    SDL_UpdateRect(Screen,0,0,0,0);

    // wait
    while ( event->type != SDL_KEYDOWN )
	SDL_PollEvent((SDL_Event *)event);

    // Clean pic
    SDL_BlitSurface(FadeBuffer,NULL,Screen,NULL);
    SDL_UpdateRect(Screen,0,0,0,0);
    Now=SDL_GetTicks();
    pause=0;
}