int aalineColorInt (SDL_Surface *dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color, int draw_endpoint)
{
 Sint32 xx0,yy0,xx1,yy1;
 int result;
 Uint32 intshift, erracc, erradj;
 Uint32 erracctmp, wgt, wgtcompmask;
 int dx, dy, tmp, xdir, y0p1, x0pxdir;

 /* Clip line and test if we have to draw */
 if (!(clipLine(dst,&x1,&y1,&x2,&y2))) {
  return(0);
 }

 /* Keep on working with 32bit numbers */
 xx0=x1;
 yy0=y1;
 xx1=x2;
 yy1=y2;
 
 /* Reorder points if required */ 
 if (yy0 > yy1) {
  tmp = yy0; yy0 = yy1; yy1 = tmp;
  tmp = xx0; xx0 = xx1; xx1 = tmp;
 }

 /* Calculate distance */
 dx = xx1 - xx0;
 dy = yy1 - yy0;

 /* Adjust for negative dx and set xdir */
 if (dx >= 0) {
   xdir=1;
 } else {
   xdir=-1;
   dx=(-dx);
 }

 /* Check for special cases */
 if (dx==0) {
   /* Vertical line */
   return (vlineColor(dst,x1,y1,y2,color));
 } else if (dy==0) {
   /* Horizontal line */
   return (hlineColor(dst,x1,x2,y1,color));
 } else if (dx==dy) {
   /* Diagonal line */
   return (lineColor(dst,x1,y1,x2,y2,color));
 } 
  
  /* Line is not horizontal, vertical or diagonal */
  result=0;
  /* Zero accumulator */
  erracc = 0;			
  /* # of bits by which to shift erracc to get intensity level */
  intshift = 32 - AAbits;
  /* Mask used to flip all bits in an intensity weighting */
  wgtcompmask = AAlevels - 1;

  /* Draw the initial pixel in the foreground color */
  result |= pixelColor (dst, x1, y1, color);

  /* x-major or y-major? */
  if (dy > dx) {

    /* y-major.  Calculate 16-bit fixed point fractional part of a pixel that
       X advances every time Y advances 1 pixel, truncating the result so that
       we won't overrun the endpoint along the X axis */
    /* Not-so-portable version: erradj = ((Uint64)dx << 32) / (Uint64)dy; */
    erradj = ((dx << 16) / dy)<<16;

    /* draw all pixels other than the first and last */
    x0pxdir=xx0+xdir;
    while (--dy) {
      erracctmp = erracc;
      erracc += erradj;
      if (erracc <= erracctmp) {
	/* rollover in error accumulator, x coord advances */
	xx0=x0pxdir;
	x0pxdir += xdir;
      }
      yy0++;			/* y-major so always advance Y */

      /* the AAbits most significant bits of erracc give us the intensity
	 weighting for this pixel, and the complement of the weighting for
	 the paired pixel. */
      wgt = (erracc >> intshift) & 255;      
      result |= pixelColorWeight (dst, xx0, yy0, color, 255-wgt);
      result |= pixelColorWeight (dst, x0pxdir, yy0, color, wgt);      
    }

  } else {

   /* x-major line.  Calculate 16-bit fixed-point fractional part of a pixel
      that Y advances each time X advances 1 pixel, truncating the result so
      that we won't overrun the endpoint along the X axis. */
   /* Not-so-portable version: erradj = ((Uint64)dy << 32) / (Uint64)dx; */
   erradj = ((dy << 16) / dx)<<16;
 
   /* draw all pixels other than the first and last */
   y0p1=yy0+1;
   while (--dx) {

    erracctmp = erracc;
    erracc += erradj;
    if (erracc <= erracctmp) {
      /* Accumulator turned over, advance y */
      yy0=y0p1;
      y0p1++;
    }
    xx0 += xdir;			/* x-major so always advance X */
    /* the AAbits most significant bits of erracc give us the intensity
       weighting for this pixel, and the complement of the weighting for
       the paired pixel. */
    wgt = (erracc >> intshift) & 255;
    result |= pixelColorWeight (dst, xx0, yy0, color, 255-wgt);
    result |= pixelColorWeight (dst, xx0, y0p1, color, wgt);
  }
 }

 /* Do we have to draw the endpoint */ 
 if (draw_endpoint) {
  /* Draw final pixel, always exactly intersected by the line and doesn't
     need to be weighted. */
  result |= pixelColor (dst, x2, y2, color);
 }

 return(result);
}