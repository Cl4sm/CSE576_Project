void CreateShot(int x, int y, int button)
{
    int SourceX=100,SourceY=515,i;

    if (y > 510) y = 510;

    if (button>3) {
	//printf("Wrong mouse button\n");
	return;
    }
    if (LeftHandedMouse) {
	button=4-button;
    }

    SourceX=-300+button*350;
    if ((ShotCount==0)||(x!=Shot[ShotCount-1].DestX)||(y!=Shot[ShotCount-1].DestY))
    if (ShotNum[button-1]>0) {
	if (ShotCount<MAX_SHOT_NUM) {

    	    ShotType *This;
	    float Vec;

	    Vec=100*hypot((x-SourceX),(y-SourceY));
    	    This=&Shot[ShotCount];
	    This->x=SourceX;
    	    This->y=SourceY;
    	    This->xspeed=SPEED*(x-SourceX)/Vec;
	    This->yspeed=SPEED*(y-SourceY)/Vec;
    	    This->CurrentPixel=0;
    	    This->Erase=0;
	    This->SourceX=SourceX;
	    This->SourceY=SourceY;
	    This->DestX=x;
	    This->DestY=y;
	    for (i=0;i<LINE_LENGTH;i++) 
		{This->Pixels[i].x=0;This->Pixels[i].y=0;}
	    ShotCount++;
	    if (ShotNum[button-1]==10) 
		ShotReload[button-1]=RELOAD;
	    ShotNum[button-1]--;
    	    DrawShotNum(button-1);
	    ActivateCursor();
	} else {
	    printf("Too many Shots\n");
	}
    }
    
//    Line(SourceX,SourceY,x,y,ShotColor);
//    Blit(x,y,ShotPic);
}