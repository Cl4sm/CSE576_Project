void PWRedrawPoints(Widget w, Position from_x, Position from_y, 
		     Position to_x, Position to_y)
#else
void PWRedrawPoints(w, from_x, from_y, to_x, to_y)
     Widget  w;
     Position from_x, from_y, to_x, to_y;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  register int x, y;
  
  QuerySwap(from_x, to_x);
  QuerySwap(from_y, to_y);
  from_x = max(0, from_x);
  from_y = max(0, from_y);
  to_x = min(PW->pixmap.image->width - 1, to_x);
  to_y = min(PW->pixmap.image->height - 1, to_y);
  
  for (x = from_x; x <= to_x; x++)
    for (y = from_y; y <= to_y; y++)
      DrawPoint(PW, x, y, GetPxl(PW, x, y), NotLazy);
}
