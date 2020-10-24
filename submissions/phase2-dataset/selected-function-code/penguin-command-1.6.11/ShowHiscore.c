void ShowHiscore()
{
    char text[200];
    int i;
    SDL_Event WaitEvent;
    
    BlitToBB(0,0,BackPic);
    PutString(BackBuffer,400-SFont_TextWidth("<Penguin Command Hiscores>")/2,70,"<Penguin Command Hiscores>");
    for (i=0;i<10;i++) {
	sprintf(text,"%s", Player[i]);
	PutString(BackBuffer,200,150+i*30,text);
	sprintf(text,"%d", Hiscore[i]);
	PutString(BackBuffer,600-SFont_TextWidth(text),150+i*30,text);
    }
    FadeScreen(1);
    SDL_WaitEvent(&WaitEvent);
    while ((WaitEvent.type!=SDL_KEYDOWN)&&(WaitEvent.type!=SDL_MOUSEBUTTONDOWN)) {
	SDL_PollEvent(&WaitEvent);
	UndrawMouse();
	DrawMouse();
	Update();
	SDL_WaitEvent(&WaitEvent);
    }
    Now=SDL_GetTicks();                          
}