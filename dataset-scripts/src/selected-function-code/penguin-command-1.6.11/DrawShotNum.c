void DrawShotNum(int Where)
{
    int x,y,shots;
    SDL_Rect rect;

    rect.x=8+Where*360;
    rect.y=550;
    rect.w=60;
    rect.h=45;
    BlitPart(rect.x,rect.y,BackPic,rect);
    y=0; shots=0;
    while (shots<ShotNum[Where]) {
	x=0;
	while ((shots<ShotNum[Where])&&(x<=y)) {
	    Blit((38+Where*360)+(x-y/(float)2)*10,550+y*7,ShotPic);
	    shots++;
	    x++;
	}
	y++;
    }
    SDL_BlitSurface(Screen, &rect, BackBuffer, &rect);
}