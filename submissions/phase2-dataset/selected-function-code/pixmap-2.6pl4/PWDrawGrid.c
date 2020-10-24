		Position to_x, Position to_y)
#else
void PWDrawGrid(w, from_x, from_y, to_x, to_y)
     Widget w;
     Position from_x, from_y, to_x, to_y;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  int i;
  
  QuerySwap(from_x, to_x);
  QuerySwap(from_y, to_y);
  from_x = max(0, from_x);
  from_y = max(0, from_y);
  to_x = min(PW->pixmap.image->width - 1, to_x);
  to_y = min(PW->pixmap.image->height - 1, to_y);
  
  for(i = from_x + (from_x == 0); i <= to_x; i++)
    XDrawLine(dpy, XtWindow(PW), 
	      PW->pixmap.framing_gc,
	      InWindowX(PW, i), InWindowY(PW, from_y),
	      InWindowX(PW, i), InWindowY(PW, to_y + 1));
  
  for(i = from_y + (from_y == 0); i <= to_y; i++)
    XDrawLine(dpy, XtWindow(PW), 
	      PW->pixmap.framing_gc,
	      InWindowX(PW, from_x), InWindowY(PW, i),
	      InWindowX(PW, to_x + 1), InWindowY(PW, i));
}
