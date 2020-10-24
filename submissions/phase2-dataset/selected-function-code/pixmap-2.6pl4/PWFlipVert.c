#else
void PWFlipVert(w)
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
  half = (float) (to_x - from_x) / 2.0 + 0.5;
  
  if (half == 0)
    return;
  
  for (y = from_y; y <= to_y; y++)
    for (x = 0; x < half; x++)
      if ((color1 = GetPxl(PW, from_x + x, y)) != 
	  (color2 = GetPxl(PW, to_x - x, y))) {
	DrawPoint(PW, from_x + x, y, color2, Lazy);
	DrawPoint(PW, to_x - x, y, color1, Lazy);
      }

  if (!PWQueryMarked(w)) {
      if (QuerySet(PW->pixmap.hot.x, PW->pixmap.hot.y)) {
          hot.x = PW->pixmap.hot.x;
          hot.y = PW->pixmap.hot.y;
          PWFlipPoint( w, &hot.x, &hot.y, Vertical ); 
          PWSetHotSpot(w, hot.x, hot.y );
      }
      /* flip extensions */
      if (PW->pixmap.flipCallback) 
          PW->pixmap.flipCallback( w, Vertical );
  }
}
