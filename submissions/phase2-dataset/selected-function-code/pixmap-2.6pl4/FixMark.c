void FixMark(PW)
    PixmapWidget PW;
{
    if (QuerySet(PW->pixmap.mark.from_x, PW->pixmap.mark.from_y)) {
	PW->pixmap.mark.from_x = min(PW->pixmap.mark.from_x, 
				     PW->pixmap.image->width);
	PW->pixmap.mark.from_y = min(PW->pixmap.mark.from_y, 
				     PW->pixmap.image->height);
	PW->pixmap.mark.to_x = min(PW->pixmap.mark.to_x, 
				   PW->pixmap.image->width);
	PW->pixmap.mark.to_y = min(PW->pixmap.mark.to_y, 
				   PW->pixmap.image->height);
	
	if((PW->pixmap.mark.from_x == PW->pixmap.mark.from_y) &&
	   (PW->pixmap.mark.to_x   == PW->pixmap.mark.to_y))
	    PW->pixmap.mark.from_x = 
		PW->pixmap.mark.from_y =
		    PW->pixmap.mark.to_x = 
			PW->pixmap.mark.to_y = NotSet;
    }
}
