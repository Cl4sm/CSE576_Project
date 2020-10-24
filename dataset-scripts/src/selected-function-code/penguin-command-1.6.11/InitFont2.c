void InitFont2(SFont_FontInfo *Font)
{
    int x = 0, i = 0;

    if ( Font->Surface==NULL ) {
		printf("The font has not been loaded!\n");
		exit(1);
    }

    if (SDL_MUSTLOCK(Font->Surface)) SDL_LockSurface(Font->Surface);

    while ( x < Font->Surface->w ) {
	if(GetPixel(Font->Surface,x,0)==SDL_MapRGB(Font->Surface->format,255,0,255)) { 
    	    Font->CharPos[i++]=x;
    	    while (( x < Font->Surface->w-1) && (GetPixel(Font->Surface,x,0)==SDL_MapRGB(Font->Surface->format,255,0,255)))
		x++;
	    Font->CharPos[i++]=x;
	}
	x++;
    }
    if (SDL_MUSTLOCK(Font->Surface)) SDL_UnlockSurface(Font->Surface);

    Font->h=Font->Surface->h;
    SDL_SetColorKey(Font->Surface, SDL_SRCCOLORKEY, GetPixel(Font->Surface, 0, Font->Surface->h-1));
}