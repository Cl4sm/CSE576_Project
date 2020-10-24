void HandleExplosion(int InGame)
{
    int i,rad;

    for (i=0;i<ExplosionCount;i++) {
	rad=Explosion[i].rad;
	Explosion[i].oldrad=Explosion[i].rad;
	Explosion[i].rad+=Speed/(float)20;
	if (rad>=EXPLOSION_SIZE) rad-=2*(rad-EXPLOSION_SIZE);
	if (InGame) {
	    MissileCollision(Explosion[i].x,Explosion[i].y,rad);
	    BombCollision(Explosion[i].x,Explosion[i].y,rad);
	}
	// Delete explosions
	if (Explosion[i].rad>=2*EXPLOSION_SIZE) {
	    AddThisRect(Explosion[i].Restore);
	    Explosion[i]=Explosion[ExplosionCount-1];    
	    i--;
	    ExplosionCount--;
	}
    }    
}