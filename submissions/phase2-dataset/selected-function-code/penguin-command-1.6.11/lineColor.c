int lineColor(SDL_Surface *dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color)
{
 int pixx, pixy;
 int x,y;
 int dx,dy;
 int ax,ay;
 int sx,sy;
 int swaptmp;
 Uint8 *pixel;
 Uint8 *colorptr;

 /* Clip line and test if we have to draw */
 if (!(clipLine(dst,&x1,&y1,&x2,&y2))) {
  return(0);
 }

 /* Test for special cases of straight lines or single point */
 if (x1==x2) {
  if (y1<y2) {
   return(vlineColor(dst, x1, y1, y2, color));
  } else if (y1>y2) {
   return(vlineColor(dst, x1, y2, y1, color));
  } else {
   return(pixelColor(dst, x1, y1, color));
  }
 }
 if (y1==y2) { 
  if (x1<x2) {
   return(hlineColor(dst, x1, x2, y1, color));
  } else if (x1>x2) { 
   return(hlineColor(dst, x2, x1, y1, color));
  }
 }

 /* Variable setup */
 dx = x2 - x1;
 dy = y2 - y1;
 sx = (dx >= 0) ? 1 : -1;
 sy = (dy >= 0) ? 1 : -1;

 /* Check for alpha blending */
 if ((color & 255)==255) { 

  /* No alpha blending - use fast pixel routines */

  /* Setup color */
  colorptr=(Uint8 *)&color;
  if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
   color=SDL_MapRGBA(dst->format, colorptr[0], colorptr[1], colorptr[2], colorptr[3]);
  } else {
   color=SDL_MapRGBA(dst->format, colorptr[3], colorptr[2], colorptr[1], colorptr[0]);
  }
  
  /* Lock surface */
  SDL_LockSurface(dst);

  /* More variable setup */
  dx = sx * dx + 1;
  dy = sy * dy + 1;
  pixx = dst->format->BytesPerPixel;
  pixy = dst->pitch;
  pixel = ((Uint8*)dst->pixels) + pixx * (int)x1 + pixy * (int)y1;
  pixx *= sx;
  pixy *= sy;
  if (dx < dy) {
   swaptmp = dx; dx = dy; dy = swaptmp;
   swaptmp = pixx; pixx = pixy; pixy = swaptmp;
  }

  /* Draw */
  x=0;
  y=0;
  switch(dst->format->BytesPerPixel) {
   case 1:
    for(; x < dx; x++, pixel += pixx) {
     *pixel = color;
     y += dy; 
     if (y >= dx) {
      y -= dx; pixel += pixy;
     }
    }
    break;
   case 2:
    for (; x < dx; x++, pixel += pixx) {
     *(Uint16*)pixel = color;
     y += dy; 
     if (y >= dx) {
      y -= dx; 
      pixel += pixy;
     }
    }
    break;
   case 3:
    for(; x < dx; x++, pixel += pixx) {
     if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      pixel[0] = (color >> 16) & 0xff;
      pixel[1] = (color >> 8) & 0xff;
      pixel[2] = color & 0xff;
     } else {
      pixel[0] = color & 0xff;
      pixel[1] = (color >> 8) & 0xff;
      pixel[2] = (color >> 16) & 0xff;
     }
     y += dy; 
     if (y >= dx) {
      y -= dx; 
      pixel += pixy;
     }
    }
    break;
   default: /* case 4 */
     for(; x < dx; x++, pixel += pixx) {
      *(Uint32*)pixel = color;
      y += dy; 
      if (y >= dx) {
       y -= dx; 
       pixel += pixy;
      }
     }
     break;
  }

  /* Unlock surface */
  SDL_UnlockSurface(dst);

 } else {

  /* Alpha blending required - use single-pixel blits */

  ax = ABS(dx) << 1;
  ay = ABS(dy) << 1;
  x = x1;
  y = y1;
  if (ax > ay) {
    int d = ay - (ax >> 1);
    while (x != x2) {
     pixelColor(dst, x, y, color);
     if (d > 0 || (d == 0 && sx == 1)) {
      y += sy;
      d -= ax;
     } 
     x += sx;
     d += ay;
    }
  } else {
    int d = ax - (ay >> 1);
    while (y != y2) {
     pixelColor(dst, x, y, color);
     if (d > 0 || ((d == 0) && (sy == 1))) {
      x += sx;
      d -= ay;
     }
     y += sy;
     d += ax;
    }
  }
  pixelColor(dst, x, y, color);

 }
 
 return(0);
}