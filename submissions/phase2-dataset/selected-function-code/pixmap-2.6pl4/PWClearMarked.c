#else
void PWClearMarked(w)
     Widget w;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  
  if (QuerySet(PW->pixmap.mark.from_x, PW->pixmap.mark.from_y))
    {
      Position from_x = PW->pixmap.mark.from_x, 
      from_y = PW->pixmap.mark.from_y, to_x = PW->pixmap.mark.to_x, 
      to_y = PW->pixmap.mark.to_y;
      Position x, y;
      QuerySwap(from_x, to_x);
      QuerySwap(from_y, to_y);
      
      for (x = from_x; x <= to_x; x++)
	for (y = from_y; y <= to_y; y++)
	  {
	    ClearPoint(PW, x, y, Lazy);
	  }
    }
}
