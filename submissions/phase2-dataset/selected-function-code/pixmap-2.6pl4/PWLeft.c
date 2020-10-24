void PWLeft(Widget w)
#else
void PWLeft(w)
     Widget w;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  register Position x, y;
  Pixel first, left, right;
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
  
  if ((to_x - from_x) == 0)
    return;
  
  for(y = from_y; y <= to_y; y++) {
    first = left = GetPxl(PW, to_x, y);
    for(x = to_x - 1; x >= from_x; x--) {
      right = GetPxl(PW, x, y);
      if (left != right)
	{
	  DrawPoint(PW, x, y, left, Lazy);
	}
      left = right;
    }
    if(first != right)
      {
	DrawPoint(PW, to_x, y, right, Lazy);
      }
  }

  if (!PWQueryMarked(w)) {
      if (QuerySet(PW->pixmap.hot.x, PW->pixmap.hot.y)) {
          PWSetHotSpot(w,
		       (PW->pixmap.hot.x - 1 + PW->pixmap.image->width) % 
		           PW->pixmap.image->width,
		       PW->pixmap.hot.y);
      }
      if (PW->pixmap.translateCallback) 
          PW->pixmap.translateCallback( w, -1, 0 );
  }
}
