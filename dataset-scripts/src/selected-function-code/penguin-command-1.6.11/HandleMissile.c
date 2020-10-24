void HandleMissile()
{
    int i;
    
    for (i=0;i<MissileCount;i++)
    {
	// Hit!
	if ((Missile[i].y>=510)&&(Missile[i].Erase==0)) {
	    CreateExplosion(Missile[i].x,Missile[i].y);
	    DestroyTarget(Missile[i].Target,Missile[i].TargetX);
	    PlaySound(MissileExplosionSound);
	}
	// Delete Missile
	if  (
		((Missile[i].y>=510)
		||((int)Missile[i].x<0)
		||((int)Missile[i].x>=800))
		||(Missile[i].Erase>0)
	    )
	    RemoveMissile(i);
	else {
	    // Move Missile
	    Missile[i].x+=Missile[i].xspeed*Speed;
    	    Missile[i].y+=Missile[i].yspeed*Speed;
	    // Split Missile (Mirv)
	    if (Missile[i].Split) {
	        CreateMissile(Missile[i].x,Missile[i].y);
		Missile[i].Split=0;
	    }
	}
    }
}