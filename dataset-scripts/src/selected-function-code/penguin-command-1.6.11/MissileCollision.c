void MissileCollision(int x, int y, int rad)
{
    int i,wings;

//    printf("ads %d - %d - %d\n",x,y,rad);
    for (i=0;i<MissileCount;i++)
    {
	if (Missile[i].Erase==0)
	if (hypot(Missile[i].x-x,Missile[i].y-y)<rad) {
	    CreateExplosion(Missile[i].x,Missile[i].y);
	    RemoveMissile(i);
	    AddScore(20);
	    PlaySound(MissileExplosionSound);
	}
    }
    for (i=0;i<FlyerCount;i++) {
	if (Flyer[i].AnimPhase<=8)
	    wings=8-Flyer[i].AnimPhase;
	else
	    wings=Flyer[i].AnimPhase-8;
//	printf("%d\n",wings);
    
	if ( ( (x+rad>Flyer[i].x) && (x-rad<Flyer[i].x+FlyerPic[0]->w)
	    && (y+rad>Flyer[i].y+FlyerPic[0]->h/2-4) && (y-rad<Flyer[i].y+FlyerPic[0]->h/2+4) )
	    ||
	    (  (x+rad>Flyer[i].x) && (x-rad<Flyer[i].x+FlyerPic[0]->w)
	    && (y+rad>Flyer[i].y+FlyerPic[0]->h/2-wings*4) && (y-rad<Flyer[i].y+FlyerPic[0]->h/2+wings*4) ) )
	{
	    CreateExplosion(Flyer[i].x+abrand(0,FlyerPic[0]->w),Flyer[i].y+FlyerPic[0]->h/2+abrand(-wings*4,+wings*4));
	    CreateExplosion(Flyer[i].x+abrand(0,FlyerPic[0]->w),Flyer[i].y+FlyerPic[0]->h/2+abrand(-wings*4,+wings*4));
	    CreateExplosion(Flyer[i].x+abrand(0,FlyerPic[0]->w),Flyer[i].y+FlyerPic[0]->h/2+abrand(-wings*4,+wings*4));
	    RemoveFlyer(i);
	    AddScore(40);
	    PlaySound(MissileExplosionSound);
	    
	}
    }
}