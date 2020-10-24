#else
void PWSet(w)
     Widget w;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  register Position x, y;
  register Position from_x, from_y, to_x, to_y;
  
  if (PWQueryMarked(w)) {
    from_x = PW->pixmap.mark.from_x;
    from_y = PW->pixmap.mark.from_y;
    to_x = PW->pixmap.mark.to_x;
    to_y = PW->pixmap.mark.to_y;
  }
  else {
    from_x = 0;
    from_y = 0;
    to_x = PW->pixmap.image->width - 1;
    to_y = PW->pixmap.image->height - 1;
  }
  for (x = from_x; x <= to_x; x++)
    for (y = from_y; y <= to_y; y++)
      {
	DrawPoint(PW, x, y, PW->pixmap.foreground_pixel, Lazy);
      }

}
