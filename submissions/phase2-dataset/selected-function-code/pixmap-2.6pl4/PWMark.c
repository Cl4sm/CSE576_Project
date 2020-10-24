	    Position to_x, Position to_y)
#else
void PWMark(w, from_x, from_y, to_x, to_y)
     Widget w;
     Position from_x, from_y, to_x, to_y;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  
  PWUnmark(w);
  
  if (QuerySet(from_x, from_y)) {
    if ((from_x == to_x) && (from_y == to_y)) {
      /*
	PW->pixmap.mark.from_x = 0;
	PW->pixmap.mark.from_y = 0;
	PW->pixmap.mark.to_x = PW->pixmap.image->width - 1;
	PW->pixmap.mark.to_y = PW->pixmap.image->height - 1;
	*/
      return;
    }
    else {
      QuerySwap(from_x, to_x);
      QuerySwap(from_y, to_y);
      from_x = max(0, from_x);
      from_y = max(0, from_y);
      to_x = min(PW->pixmap.image->width - 1, to_x);
      to_y = min(PW->pixmap.image->height - 1, to_y);
      
      PW->pixmap.mark.from_x = from_x;
      PW->pixmap.mark.from_y = from_y;
      PW->pixmap.mark.to_x = to_x;
      PW->pixmap.mark.to_y = to_y;
    }
    
    XFillRectangle(dpy, XtWindow(PW), PW->pixmap.highlighting_gc,
		   InWindowX(PW, PW->pixmap.mark.from_x),
		   InWindowY(PW, PW->pixmap.mark.from_y), 
		   InWindowX(PW, PW->pixmap.mark.to_x + 1) -
		   InWindowX(PW, PW->pixmap.mark.from_x),
		   InWindowY(PW, PW->pixmap.mark.to_y +1) - 
		   InWindowY(PW, PW->pixmap.mark.from_y));
  }
}
