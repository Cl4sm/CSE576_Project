#else
void PWRotateLeft(w)
     Widget w;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;
  Position x, y,delta, shift;
  Position half_width, half_height;
  XPoint hot;
  Pixel quad1, quad2, quad3, quad4;
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
  
  half_width = floor((to_x - from_x) / 2.0 + 0.5);
  half_height = floor((to_y - from_y ) / 2.0 + 0.5);
  shift = min((Position)(to_x - from_x), (Position)(to_y - from_y )) % 2;
  delta = min((Position) half_width, (Position) half_height) - shift;
  
  for (x = 0; x <= delta; x++) {
    for (y = 1 - shift; y <= delta; y++) {
      quad1 = GetPxl(PW, 
		     from_x + (Position)half_width + x, 
		     from_y + (Position)half_height + y);
      quad2 = GetPxl(PW, 
		     from_x + (Position)half_width + y, 
		     from_y + (Position)half_height - shift - x);
      quad3 = GetPxl(PW, 
		     from_x + (Position)half_width - shift - x, 
		     from_y + (Position)half_height - shift - y);
      quad4 = GetPxl(PW, 
		     from_x + (Position)half_width - shift - y, 
		     from_y + (Position)half_height + x);
      
      if (quad1 != quad4)
	{
	  DrawPoint(PW, from_x + (Position)half_width + x, 
		    from_y + (Position)half_height + y, quad4, Lazy);
	}
      if (quad2 != quad1)
	{
	  DrawPoint(PW, from_x + (Position)half_width + y, 
		    from_y + (Position)half_height - shift - x, quad1, Lazy);
	}
      if (quad3 != quad2)
	{
	  DrawPoint(PW, from_x + (Position)half_width - shift - x,
		    from_y + (Position)half_height - shift - y, quad2, Lazy);
	}
      if (quad4 != quad3)
	{
	  DrawPoint(PW, from_x + (Position)half_width - shift - y, 
		    from_y + (Position)half_height + x, quad3, Lazy);
	}
    }
  }

  if (!PWQueryMarked(w)) {
      if (QuerySet(PW->pixmap.hot.x, PW->pixmap.hot.y)) {
          hot.x = PW->pixmap.hot.x;
          hot.y = PW->pixmap.hot.y;
          PWRotatePoint( w, &hot.x, &hot.y, Left ); 
          if (QueryInPixmap(PW, hot.x, hot.y))
            PWSetHotSpot(w, hot.x, hot.y);
      }
      /* rotate extensions */
      if (PW->pixmap.rotateCallback) 
          PW->pixmap.rotateCallback( w, Left );
  }
}
