void DrawImageData(PW, image, mask_image, at_x, at_y, value, mode)
     PixmapWidget PW;
     XImage *image, *mask_image;
     Position at_x, at_y;
     int value;
     DrawingMode mode;
{
  Position x, y;
  Pixel      A, B;
  register Pixel Cl, Fg, val;
  
  Cl = PW->pixmap.clear_pixel;
  Fg = PW->pixmap.foreground_pixel;
  
  for (x = 0; x < image->width; x++) 
    for (y = 0; y < image->height; y++) {
      if (!QueryInPixmap(PW, at_x + x, at_y + y)) break;
      A = GetPxlFromImageAndMask(image, mask_image, x, y);
      B = GetPxl(PW, at_x + x, at_y + y);
      switch (value)
	{
	case Clear:
	  if ((mode == NotLazy) || (B != Cl)) val = Cl;
	  else val = NotSet;
	  break;
	case Set:
	  if ((mode == NotLazy) || (B != A)) val = A;
	  else val = NotSet;
	  break;
	case Invert:
	  if (((mode == NotLazy) && (A != Cl)) || ((A != Cl) && (B != Cl)))
	    val = Cl;
	  else if (((mode == NotLazy) && (A == Cl)) || 
		   ((A == Cl) && (B != Fg))) val = Fg;
	  else val = NotSet;
	  break;
	case Highlight:
	  if (A != B) PWDrawPoint((Widget)PW, at_x + x, at_y + y, A);
	  break;
	}
      if ((val != NotSet) && (val != Highlight))
	{
	  DrawPoint(PW, at_x + x, at_y + y, val, mode);
	}
      else 
	if (value == Highlight) HighlightSquare(PW, x, y);
	
    }
}
