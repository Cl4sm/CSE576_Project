void CreateMissile(int x, int y)
{
    int i;
    
    if (MissileCount<MISSILE_MAX_NUM) {

        MissileType *This;
	float Vec;

        This=&Missile[MissileCount];
	This->SourceX=This->x=x;
        This->SourceY=This->y=y;
	This->Target=abrand(0,8);
	if (This->Target<3)
	    This->TargetX = 120+This->Target*100;
	else if (This->Target<6)
	    This->TargetX = 480+(This->Target-3)*100;
	else 
	    This->TargetX = 40+(This->Target-6)*360;
	Vec=100*hypot(This->TargetX-This->x,510-This->y);
	This->xspeed=Wave[WaveNum].MissileSpeed*(This->TargetX-This->x)/Vec;
        This->yspeed=Wave[WaveNum].MissileSpeed*(510-This->y)/Vec;
        This->CurrentPixel=0;
        This->Erase=0;
        This->Split=0;
	for (i=0;i<MISSILE_LENGTH;i++) 
	    {This->Pixels[i].x=0;This->Pixels[i].y=0;}
	MissileCount++;
    }
    else printf("Too many Missiles\n");
}