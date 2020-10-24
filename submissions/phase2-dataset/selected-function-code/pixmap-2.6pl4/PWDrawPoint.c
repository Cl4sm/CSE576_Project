void PWDrawPoint(Widget w, Position x, Position y, int value)
#else
void PWDrawPoint(w, x, y, value)
     Widget  w;
     Position      x, y;
     int           value;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  
  if (QueryInPixmap(PW, x, y)) 
    {
      if (value == Highlight)
	HighlightSquare(PW, x, y);
      else if ((value == Clear) || 
	       ((value == Invert) && 
		(GetPxl(PW, x, y) != PW->pixmap.clear_pixel)))
	{
	  ClearPoint(PW, x, y, Lazy);
	}
      else /* value == Set || (value == Invert && Pxl == clear_pixel) */
	{
	  DrawPoint(PW, x, y, PW->pixmap.foreground_pixel, Lazy);
	}
    }
}
