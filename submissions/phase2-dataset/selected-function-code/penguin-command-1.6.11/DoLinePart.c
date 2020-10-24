int DoLinePart (SDL_Surface *Surface, Sint32 X1, Sint32 Y1, Sint32 X2, Sint32 Y2, Sint32 lower, Sint32 upper, Uint32 Color, int Callback (SDL_Surface *Surf, Sint32 X, Sint32 Y, Uint32 Color))
{ 
  Sint32 dx, dy, sdx, sdy, x, y, px, py; 
 
  dx = X2 - X1; 
  dy = Y2 - Y1; 
 
  sdx = (dx < 0) ? -1 : 1; 
  sdy = (dy < 0) ? -1 : 1; 
 
  dx = sdx * dx + 1; 
  dy = sdy * dy + 1; 
 
  x = y = 0; 
 
  px = X1; 
  py = Y1; 
 
  if (dx >= dy) 
    { 
      for (x = 0; x < dx; x++) 
	{ 
	  if ( (py<=upper)&&(py>=lower) )
          Callback(Surface, px, py, Color);
	 
	  y += dy; 
	  if (y >= dx) 
	    { 
	      y -= dx; 
	      py += sdy; 
	    } 
	  px += sdx; 
	} 
    } 
  else 
    { 
      for (y = 0; y < dy; y++) 
	{ 
 
	  if ( (py<=upper)&&(py>=lower) )
          Callback(Surface, px, py, Color);
 
	  x += dx; 
	  if (x >= dy) 
	    { 
	      x -= dy; 
	      px += sdx; 
	    } 
	  py += sdy; 
	} 
    } 
   return 0;
}