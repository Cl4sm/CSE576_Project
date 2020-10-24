void DestroyTarget(int TargetNum, int TargetX)
{
    SDL_Rect rect;

    if (TargetNum<6) {
	rect.x=TargetX-50;
	rect.y=490;
	rect.w=100;
	rect.h=CityPic->h;
	SDL_BlitSurface(BackPic,&rect,BackBuffer,&rect);
	BlitToBB(rect.x,rect.y,CityHitPic);
	BlitPart(rect.x,rect.y,BackBuffer,rect);
	if ( ! CityDestroyed[TargetNum] ) {
	    CityDestroyed[TargetNum]=1;
	    CityNum--;
	}
    } else if (TargetNum<9) {
	rect.x=TargetX-CannonHitPic->w/2;
	rect.y=510;
	rect.w=CannonHitPic->w;
	rect.h=CannonHitPic->h;
	SDL_BlitSurface(BackPic,&rect,BackBuffer,&rect);
	BlitToBB(rect.x,rect.y,CannonHitPic);
	SDL_SetAlpha(BackBuffer,0,0);
	BlitPart(rect.x,rect.y,BackBuffer,rect);
	CannonDestroyed[TargetNum-6]=1;
	ShotNum[TargetNum-6]=-1;
	DrawShotNum(TargetNum-6);
    } else printf("WARNING: Target was %d!\n",TargetNum);
}