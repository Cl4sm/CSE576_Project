void DrawShot()
{
    int i;
    
    lock();
    for (i=0;i<ShotCount;i++) {
	if (Shot[i].Erase==0) {
	    if (
		    ((int)Shot[i].x!=Shot[i].Pixels[Shot[i].CurrentPixel].x)
		    ||((int)Shot[i].y!=Shot[i].Pixels[Shot[i].CurrentPixel].y)	    
		)
	    {
		if (++Shot[i].CurrentPixel>=LINE_LENGTH)
		    printf("Line too long in function DrawShot\n");
		PutPixel(Screen,Shot[i].x,Shot[i].y,ShotColor);
		Shot[i].Pixels[Shot[i].CurrentPixel].x=Shot[i].x;
		Shot[i].Pixels[Shot[i].CurrentPixel].y=Shot[i].y;
		Shot[i].Proceeded=1;
		if (Shot[i].CurrentPixel>1) {
		    if (++Shot[i].CurrentPixel>=LINE_LENGTH)
			printf("Line too long in function DrawShot\n");
		    Shot[i].Pixels[Shot[i].CurrentPixel].x=(Shot[i].x+Shot[i].Pixels[Shot[i].CurrentPixel-1].x)/2;
		    Shot[i].Pixels[Shot[i].CurrentPixel].y=(Shot[i].y+Shot[i].Pixels[Shot[i].CurrentPixel-1].y)/2;
		    Shot[i].Proceeded=1;
		    PutPixel(Screen,Shot[i].Pixels[Shot[i].CurrentPixel].x,
		    Shot[i].Pixels[Shot[i].CurrentPixel].y,ShotColor);
		}
	    } else {
		Shot[i].Proceeded=0;
	    }
	}
    }
    unlock();
}