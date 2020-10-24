#else
void PWUnmark(w)
     Widget w;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  
  PW->pixmap.buffer_mark = PW->pixmap.mark;
  
  if (QuerySet(PW->pixmap.mark.from_x, PW->pixmap.mark.from_y)) {
    XFillRectangle(dpy, XtWindow(PW), PW->pixmap.highlighting_gc,
		   InWindowX(PW, PW->pixmap.mark.from_x), 
		   InWindowY(PW, PW->pixmap.mark.from_y), 
		   InWindowX(PW, PW->pixmap.mark.to_x + 1) - 
		   InWindowX(PW, PW->pixmap.mark.from_x),
		   InWindowY(PW, PW->pixmap.mark.to_y + 1) -
		   InWindowY(PW, PW->pixmap.mark.from_y));
    
    PW->pixmap.mark.from_x = PW->pixmap.mark.from_y = NotSet;
    PW->pixmap.mark.to_x = PW->pixmap.mark.to_y = NotSet;
  }
}
