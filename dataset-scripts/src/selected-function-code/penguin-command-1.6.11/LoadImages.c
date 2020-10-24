void LoadImages()
{
    int i;
    char text[200];

    BackPic = LoadImage("back.jpg",0);
    SDL_FreeSurface(ShotPic);
    ShotPic = LoadImage("shot.png",3);
    CityPic = LoadImage("city.png",4);
    CityHitPic = LoadImage("cityhit.png",4);
    CannonHitPic = LoadImage("cannonhit.png",4);
    for (i=0;i<21;i++) {
	sprintf(text,"cannon%d.png",i+1);
	SDL_FreeSurface(Cannon[i]);
	Cannon[i] = LoadImage(text,3);
    }
    for (i=0;i<10;i++) {
	sprintf(text,"bomb%d.png",i+1);
	SDL_FreeSurface(BombPic[i]);
	BombPic[i] = LoadImage(text,3);
    }
    for (i=0;i<20;i++) {
	sprintf(text,"%dexplo.png",i+1);
	SDL_FreeSurface(ExplosionPic[i]);
	ExplosionPic[i] = LoadImage(text,3);
    }
    for (i=0;i<9;i++) {
	sprintf(text,"%dcursor.png",i+1);
	SDL_FreeSurface(CursorPic[i]);
	CursorPic[i] = LoadImage(text,3);
    }
    CursorPic[9] = CursorPic[0];
    for (i=0;i<16;i++) {
	sprintf(text,"%dflyer.png",i+1);
	SDL_FreeSurface(FlyerPic[i]);
	FlyerPic[i] = LoadImage(text,3);
    }
}