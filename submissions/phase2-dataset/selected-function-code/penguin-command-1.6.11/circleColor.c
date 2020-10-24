int circleColor(SDL_Surface *dst, Sint16 x, Sint16 y, Sint16 r, Uint32 color)
{
 int result;
 Sint16 x1,y1,x2,y2;
 Sint16 cx = 0;
 Sint16 cy = r;
 Sint16 ocx = (Sint16)0xffff;
 Sint16 ocy = (Sint16)0xffff;
 Sint16 df = 1 - r;
 Sint16 d_e = 3;
 Sint16 d_se = -2 * r + 5;
 Sint16 xpcx, xmcx, xpcy, xmcy;
 Sint16 ypcy, ymcy, ypcx, ymcx;
 Uint8 *colorptr;
  
 /* Sanity check radius */
 if (r<0) {
  return(-1);
 }

 /* Special case for r=0 - draw a point */
 if (r==0) {
  return(pixelColor (dst, x, y, color));  
 }

 /* Test if bounding box of circle is visible */
 x1=x-r;
 y1=y-r;
 x2=x+r;
 y2=y+r;
 if (!(clipLine(dst,&x1,&y1,&x2,&y2))) {
  return(0);
 }

 /* Draw circle */
 result=0; 
 /* Alpha Check */
 if ((color & 255) ==255) {

  /* No Alpha - direct memory writes */

  /* Setup color */
  colorptr=(Uint8 *)&color;
  if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
   color=SDL_MapRGBA(dst->format, colorptr[0], colorptr[1], colorptr[2], colorptr[3]);
  } else {
   color=SDL_MapRGBA(dst->format, colorptr[3], colorptr[2], colorptr[1], colorptr[0]);
  }

  /* Lock surface */
  SDL_LockSurface(dst);
  
  /* Draw */
  do {
   if ((ocy!=cy) || (ocx!=cx)) {
    xpcx=x+cx;
    xmcx=x-cx;
    if (cy>0) {
     ypcy=y+cy;
     ymcy=y-cy;
     result |= fastPixelColorNolock(dst,xmcx,ypcy,color);
     result |= fastPixelColorNolock(dst,xpcx,ypcy,color);
     result |= fastPixelColorNolock(dst,xmcx,ymcy,color);
     result |= fastPixelColorNolock(dst,xpcx,ymcy,color);
    } else {
     result |= fastPixelColorNolock(dst,xmcx,y,color);
     result |= fastPixelColorNolock(dst,xpcx,y,color);
    }
    ocy=cy;
    xpcy=x+cy;
    xmcy=x-cy;
    if (cx>0) {
     ypcx=y+cx;
     ymcx=y-cx;
     result |= fastPixelColorNolock(dst,xmcy,ypcx,color);
     result |= fastPixelColorNolock(dst,xpcy,ypcx,color);
     result |= fastPixelColorNolock(dst,xmcy,ymcx,color);
     result |= fastPixelColorNolock(dst,xpcy,ymcx,color);
    } else {
     result |= fastPixelColorNolock(dst,xmcy,y,color);
     result |= fastPixelColorNolock(dst,xpcy,y,color);
    }
    ocx=cx;
   }
   /* Update */
   if (df < 0)  {
    df += d_e;
    d_e += 2;
    d_se += 2;
   } else {
    df += d_se;
    d_e += 2;
    d_se += 4;
    cy--;
   }
   cx++;
  } while(cx <= cy);

  /* Unlock surface */
  SDL_UnlockSurface(dst);

 } else {
 
  /* Using Alpha - blended pixel blits */
  
  do {
   /* Draw */
   if ((ocy!=cy) || (ocx!=cx)) {
    xpcx=x+cx;
    xmcx=x-cx;
    if (cy>0) {
     ypcy=y+cy;
     ymcy=y-cy;
     result |= pixelColor(dst,xmcx,ypcy,color);
     result |= pixelColor(dst,xpcx,ypcy,color);
     result |= pixelColor(dst,xmcx,ymcy,color);
     result |= pixelColor(dst,xpcx,ymcy,color);
    } else {
     result |= pixelColor(dst,xmcx,y,color);
     result |= pixelColor(dst,xpcx,y,color);
    }
    ocy=cy;
    xpcy=x+cy;
    xmcy=x-cy;
    if (cx>0) {
     ypcx=y+cx;
     ymcx=y-cx;
     result |= pixelColor(dst,xmcy,ypcx,color);
     result |= pixelColor(dst,xpcy,ypcx,color);
     result |= pixelColor(dst,xmcy,ymcx,color);
     result |= pixelColor(dst,xpcy,ymcx,color);
    } else {
     result |= pixelColor(dst,xmcy,y,color);
     result |= pixelColor(dst,xpcy,y,color);
    }
    ocx=cx;
   }
   /* Update */
   if (df < 0)  {
    df += d_e;
    d_e += 2;
    d_se += 2;
   } else {
    df += d_se;
    d_e += 2;
    d_se += 4;
    cy--;
   }
   cx++;
  } while(cx <= cy);

 } /* Alpha check */
 
 return(result);
}