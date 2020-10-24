void HandleShot()
{
    int i,a;
    
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
 		for (a=Shot[i].CurrentPixel;a>=0;a--) RemoveShotPix(i,a);
		Shot[i]=Shot[ShotCount-1];
		ShotCount--;
	    }
	    else {
		SDL_SetAlpha(BackBuffer,SDL_SRCALPHA,Shot[i].Erase);
		for (a=Shot[i].CurrentPixel;a>=0;a--) RemoveShotPix(i,a);
 	    }
	}
    }
    if (ArcadeMode) Reload();
}