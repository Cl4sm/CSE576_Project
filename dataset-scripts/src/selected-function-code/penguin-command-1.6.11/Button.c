int Button(int x, int y, char *String, int prev, SDL_Surface *Background)
{
    SDL_Rect rect;
    int MouseX,MouseY;
    
    rect.x=x;
    rect.y=y;
    rect.w=SFont_TextWidth(String);
    rect.h=Font->h;
    
    SDL_GetMouseState(&MouseX, &MouseY);
    if ((MouseX>x)&&(MouseY>y)
	&&(MouseX<x+SFont_TextWidth(String))
	&&(MouseY<y+Font->h))
    {
	if (prev!=1) {
	    // clear
	    SDL_SetAlpha(Background,0,0);
	    SDL_BlitSurface(Background, &rect, Screen, &rect);
	    // draw
	    PutString(Screen, x, y, String);
	    AddThisRect(rect);
	    SDL_BlitSurface(Screen, &rect, BackBuffer, &rect);
	    PlaySound(ClickSound);
	}
	return 1;
    } else if (prev!=0) {
	// clear
	SDL_SetAlpha(Background,0,0);
	SDL_BlitSurface(Background, &rect, Screen, &rect);
	// draw
	PutString(Screen, x, y, String);
	AddThisRect(rect);
	// darken
	SDL_SetAlpha(Background,SDL_SRCALPHA,100);
	SDL_BlitSurface(Background, &rect, Screen, &rect);
        SDL_SetAlpha(Background,0,0);
	SDL_BlitSurface(Screen, &rect, BackBuffer, &rect);
    }
    
    return 0;
}