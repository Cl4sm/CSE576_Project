void SFont_InternalInput( SDL_Surface *Dest, SFont_FontInfo *Font, int x, int y, int PixelWidth, char *text)
{
    SDL_Event event;
    int ch=-1,blink=0;
    long blinktimer=0;
    SDL_Surface *Back;
    SDL_Rect rect;
//    int ofs=(text[0]-33)*2+1;
//    int leftshift=(Font->CharPos[ofs]-Font->CharPos[ofs-1])/2;
    
    Back = SDL_AllocSurface(Dest->flags,
    			    Dest->w,
    			    Font->h,
    			    Dest->format->BitsPerPixel,
    			    Dest->format->Rmask,
    			    Dest->format->Gmask,
			    Dest->format->Bmask, 0);
    rect.x=0;
    rect.y=y;
    rect.w=Dest->w;
    rect.h=Font->Surface->h;
    SDL_BlitSurface(Dest, &rect, Back, NULL);
    PutString2(Dest,Font,x,y,text);
    SDL_UpdateRects(Dest, 1, &rect);
        
    // start input
    SDL_EnableUNICODE(1);
    blinktimer=SDL_GetTicks();
    while (ch!=SDLK_RETURN) {
	if (event.type==SDL_KEYDOWN) {
	    ch=event.key.keysym.unicode;
	    if (((ch>31)||(ch=='\b')) && (ch<128)) {
		if ((ch=='\b')&&(strlen(text)>0))
		    text[strlen(text)-1]='\0';
		else if (ch!='\b')
		    sprintf(text,"%s%c",text,ch);
	        if (SFont_TextWidth2(Font,text)>PixelWidth) text[strlen(text)-1]='\0';
		SDL_BlitSurface( Back, NULL, Dest, &rect);
		PutString2(Dest, Font, x, y, text);
		SDL_UpdateRects(Dest, 1, &rect);
//		printf("%s ## %d\n",text,strlen(text));
		SDL_WaitEvent(&event);
	    }
	}
	if (SDL_GetTicks()>blinktimer) {
	    blink=1-blink;
	    blinktimer=SDL_GetTicks()+500;
	    if (blink) {
		PutString2(Dest, Font, x+SFont_TextWidth2(Font,text), y, "|");
		SDL_UpdateRects(Dest, 1, &rect);
//		SDL_UpdateRect(Dest, x+SFont_TextWidth2(Font,text), y, SFont_TextWidth2(Font,"|"), Font->Surface->h);
	    } else {
		SDL_BlitSurface( Back, NULL, Dest, &rect);
		PutString2(Dest, Font, x, y, text);
		SDL_UpdateRects(Dest, 1, &rect);
//		SDL_UpdateRect(Dest, x-(Font->CharPos[ofs]-Font->CharPos[ofs-1])/2, y, PixelWidth, Font->Surface->h);
	    }
	}
	SDL_Delay(1);
	SDL_PollEvent(&event);
    }
    text[strlen(text)]='\0';
    SDL_FreeSurface(Back);
}