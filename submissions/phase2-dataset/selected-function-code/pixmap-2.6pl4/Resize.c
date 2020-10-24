static void Resize(PW)
     PixmapWidget PW;
{
    Dimension squareW, squareH;

    if (PW->pixmap.resize == True)
      {
	squareW = max(1, 
		      ((int)PW->core.width - 2 * (int)PW->pixmap.distance) / 
		      (int)PW->pixmap.width);
	squareH = max(1, 
		      ((int)PW->core.height - 2 * (int)PW->pixmap.distance) / 
		      (int)PW->pixmap.height);

	if (PW->pixmap.proportional)
	  PW->pixmap.squareW = PW->pixmap.squareH = min(squareW, squareH);
	else {
	  PW->pixmap.squareW = squareW;
	  PW->pixmap.squareH = squareH;
	}
      }

    PW->pixmap.horizOffset = max((Position)PW->pixmap.distance, 
				 (Position)(PW->core.width - 
					    PW->pixmap.width * 
					    PW->pixmap.squareW) / 2);
    PW->pixmap.vertOffset = max((Position)PW->pixmap.distance, 
				(Position)(PW->core.height - 
					   PW->pixmap.height * 
					   PW->pixmap.squareH) / 2);

    PW->pixmap.grid &= ((PW->pixmap.squareW > PW->pixmap.grid_tolerance) && 
			(PW->pixmap.squareH > PW->pixmap.grid_tolerance));
}
