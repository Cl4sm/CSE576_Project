void ShowCredits()
{
    float MusicDown=110;

    BlitToBB(0,0,BackPic);
    XCenteredString(BackBuffer,10,"<Penguin Command Credits>");
    PutString(BackBuffer,20,60 ,"Code:");
    PutString(BackBuffer,20,90,"- Karl Bartel <karlb@gmx.net>");
    PutString(BackBuffer,20,140,"Graphics:");
    PutString(BackBuffer,20,170,"- Karl Bartel");
    PutString(BackBuffer,20,200,"- Shane Simmons");
    PutString(BackBuffer,20,140+MusicDown,"Music:");
    PutString(BackBuffer,20,170+MusicDown,"- Skaven of the Future Crew");
    PutString(BackBuffer,100,200+MusicDown,"Data Jack");
    PutString(BackBuffer,100,230+MusicDown,"Ice Frontier");
    PutString(BackBuffer,100,260+MusicDown,"Rama Gardens");
    PutString(BackBuffer,20,310+MusicDown,"- Seth Peelle");
    PutString(BackBuffer,100,340+MusicDown,"Penguin Night");
    XCenteredString(BackBuffer,560,"http://www.linux-games.com/");
    FadeScreen(1);
    Now=SDL_GetTicks();
    while (SDL_GetMouseState(NULL,NULL)==SDL_BUTTON(1)) {SDL_PollEvent(&event);SDL_Delay(10);}
    while ((event.type!=SDL_KEYDOWN)&&(event.type!=SDL_MOUSEBUTTONDOWN)) {
	SDL_PollEvent(&event);
	UndrawMouse();
	DrawMouse();
        GetSpeed();
	Update();
    }
}