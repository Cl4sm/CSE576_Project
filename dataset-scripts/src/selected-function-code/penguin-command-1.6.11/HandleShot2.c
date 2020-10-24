void HandleShot2()
{
    int i;
    
    for (i=0;i<ShotCount;i++)
    {
	if (Shot[i].Erase==0)
	{
    	    // Move Shot
	    Shot[i].x+=Shot[i].xspeed*Speed;
    	    Shot[i].y+=Shot[i].yspeed*Speed;
	    if (Shot[i].x>=800) {
		printf("WARNING: shot too far right!\n");
	    }
	}
	// Delete Shot
	if  (Shot[i].y-Shot[i].DestY<=0)
	{
	    Shot[i].y=Shot[i].DestY;
	    Shot[i].x=Shot[i].DestX;
    	    if (Shot[i].Erase==0) CreateExplosion(Shot[i].x,Shot[i].y);
	    Shot[i].Erase+=Speed/(float)7;
	    if (Shot[i].Erase>255) {
    		SDL_SetAlpha(BackBuffer,0,0);
//		UndrawLine(Shot[i].x,Shot[i].y,Shot[i].SourceX,Shot[i].SourceY,ShotColor);
		UndrawThisShot(i);
		Shot[i]=Shot[ShotCount-1];
		ShotCount--;
	    }
	    else {
/*		SDL_Rect rect;
		
		rect.x = Shot[i].DestX*Shot[i].Erase + Shot[i].SourceX*(1-Shot[i].Erase);
		rect.y = Shot[i].DestY*Shot[i].Erase + Shot[i].SourceY*(1-Shot[i].Erase);
		rect.w = rect.h = Speed;
		SDL_BlitSurface( BackBuffer, &rect, Screen, &rect);
		
		Line(Shot[i].x,Shot[i].y,Shot[i].SourceX,Shot[i].SourceY,ShotColor);
		SDL_SetAlpha(BackBuffer,SDL_SRCALPHA,Shot[i].Erase);
		UndrawLine(Shot[i].x,Shot[i].y,Shot[i].SourceX,Shot[i].SourceY,ShotColor);*/
 	    }
	}
    }
    if (ArcadeMode) Reload();
}