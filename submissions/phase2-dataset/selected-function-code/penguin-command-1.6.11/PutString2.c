void PutString2(SDL_Surface *Surface, SFont_FontInfo *Font, int x, int y, char *text)
{
    int ofs;
    int i=0;
    SDL_Rect srcrect,dstrect; 

    while (text[i]!='\0') {
        if (text[i]==' ') {
            x+=Font->CharPos[2]-Font->CharPos[1];
            i++;
	}
	else {
//	    printf("-%c- %c - %u\n",228,text[i],text[i]);
	    ofs=(text[i]-33)*2+1;
//	    printf("printing %c %d\n",text[i],ofs);
            srcrect.w = dstrect.w = (Font->CharPos[ofs+2]+Font->CharPos[ofs+1])/2-(Font->CharPos[ofs]+Font->CharPos[ofs-1])/2;
            srcrect.h = dstrect.h = Font->Surface->h-1;
            srcrect.x = (Font->CharPos[ofs]+Font->CharPos[ofs-1])/2;
            srcrect.y = 1;
    	    dstrect.x = x-(float)(Font->CharPos[ofs]-Font->CharPos[ofs-1])/2;
	    dstrect.y = y;

	    SDL_BlitSurface( Font->Surface, &srcrect, Surface, &dstrect); 

            x+=Font->CharPos[ofs+1]-Font->CharPos[ofs];
            i++;
        }
    }
}