#else
void PWUp(w)
     Widget w;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  register Position x, y;
  Pixel first, up, down;
  Position from_x, from_y, to_x, to_y;
  
  if (PWQueryMarked(w)) {
    from_x = PW->pixmap.mark.from_x;
    from_y = PW->pixmap.mark.from_y;
    to_x = PW->pixmap.mark.to_x;
    to_y = PW->pixmap.mark.to_y;
  }
  else {
    from_x = 0;
    from_y = 0;
    to_x = PW->pixmap.width - 1;
    to_y = PW->pixmap.height - 1;
  }
  
  if ((to_y - from_y) == 0)
    return;
  
  for(x = from_x; x <= to_x; x++) {
    first = up = GetPxl(PW, x, to_y);
    for(y = to_y - 1; y >= from_y; y--) {
      down = GetPxl(PW, x, y);
      if (up != down) 
	{
	  DrawPoint(PW, x, y, up, Lazy);
	}
      up = down;
    }
    if (first != down)
      {
	DrawPoint(PW, x, to_y, down, Lazy);
      }
  }
  
  if (!PWQueryMarked(w)) {
      if (QuerySet(PW->pixmap.hot.x, PW->pixmap.hot.y)) {
          PWSetHotSpot(w,
		       PW->pixmap.hot.x,
		       (PW->pixmap.hot.y - 1 + PW->pixmap.image->height) % 
		       PW->pixmap.image->height);
      }
      if (PW->pixmap.translateCallback) 
          PW->pixmap.translateCallback( w, 0, -1 );
  }
}
