void PWRight(Widget w)
#else
void PWRight(w)
     Widget w;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  register Position x, y;
  Pixel first, right, left;
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
    first = right = GetPxl(PW, from_x, y);
    for(x = from_x + 1; x <= to_x; x++) {
      left = GetPxl(PW, x, y);
      if (right != left)
	{
	  DrawPoint(PW, x, y, right, Lazy);
	}
      right = left;
    }
    if(first != left)
      {
	DrawPoint(PW, from_x, y, left, Lazy);
      }
  }

  if (!PWQueryMarked(w)) {
      if (QuerySet(PW->pixmap.hot.x, PW->pixmap.hot.y)) {
          PWSetHotSpot(w,
		       (PW->pixmap.hot.x + 1) % PW->pixmap.image->width,
		       PW->pixmap.hot.y);
      }
      if (PW->pixmap.translateCallback) 
          PW->pixmap.translateCallback( w, 1, 0 );
  }
}
