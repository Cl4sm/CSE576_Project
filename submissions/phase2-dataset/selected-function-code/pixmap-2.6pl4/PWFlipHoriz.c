void PWFlipHoriz(Widget w)
#else
void PWFlipHoriz(w)
     Widget w;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  register Position x, y;
  Position from_x, from_y, to_x, to_y;
  XPoint hot;
  float half;
  Pixel color1, color2;
  
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
  half = (float) (to_y - from_y) / 2.0 + 0.5;
  
  if (half == 0.0)
    return;
  
  for (x = from_x; x <= to_x; x++) 
    for (y = 0; y <  half; y++)
      if ((color1 = GetPxl(PW, x, from_y + y)) != 
	  (color2 = GetPxl(PW, x, to_y - y))) {
	DrawPoint(PW, x, from_y + y, color2, Lazy);
	DrawPoint(PW, x, to_y - y, color1, Lazy);
      }

  if (!PWQueryMarked(w)) {
      if (QuerySet(PW->pixmap.hot.x, PW->pixmap.hot.y)) {
          hot.x = PW->pixmap.hot.x;
          hot.y = PW->pixmap.hot.y;
          PWFlipPoint( w, &hot.x, &hot.y, Horizontal ); 
          PWSetHotSpot(w, hot.x, hot.y );
      }
      /* flip extensions */
      if (PW->pixmap.flipCallback) 
          PW->pixmap.flipCallback( w, Horizontal );
  }
}
