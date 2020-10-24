int boxColor (SDL_Surface *dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color) 
{
 Uint8 *pixel, *pixellast;
 int x, dx;
 int dy;
 int pixx, pixy;
 Sint16 w,h,tmp;
 int result;
 Uint8 *colorptr;
#ifdef SURFACE_ALPHA_PIXEL
 Uint32 a;
 SDL_Rect srect;
 SDL_Rect drect;
#endif

 /* Clip diagonal and test if we have to draw */
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
  
 /* Order coordinates */
 if (x1>x2) {
  tmp=x1;
  x1=x2;
  x2=tmp;
 }
 if (y1>y2) {
  tmp=y1;
  y1=y2;
  y2=tmp;
 }
 
 /* Calculate width&height */
 w=x2-x1;
 h=y2-y1;
 
 /* Alpha check */
 if ((color & 255)==255) {

  /* No alpha-blending required */
  
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
  dx=w;
  dy=h;
  pixx = dst->format->BytesPerPixel;
  pixy = dst->pitch;
  pixel = ((Uint8*)dst->pixels) + pixx * (int)x1 + pixy * (int)y1;
  pixellast = pixel + pixx*dx + pixy*dy;
  
  /* Draw */
  switch(dst->format->BytesPerPixel) {
   case 1:
    for (; pixel<=pixellast; pixel += pixy) {
     memset(pixel,(Uint8)color,dx);
    }
    break;
   case 2:
    pixy -= (pixx*dx);
    for (; pixel<=pixellast; pixel += pixy) {
     for (x=0; x<dx; x++) {
      *(Uint16*)pixel = color;
      pixel += pixx;
     }
    }
    break;
   case 3:
    pixy -= (pixx*dx);
    for (; pixel<=pixellast; pixel += pixy) {
     for (x=0; x<dx; x++) {
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
       pixel[0] = (color >> 16) & 0xff;
       pixel[1] = (color >> 8) & 0xff;
       pixel[2] = color & 0xff;
      } else {
       pixel[0] = color & 0xff;
       pixel[1] = (color >> 8) & 0xff;
       pixel[2] = (color >> 16) & 0xff;
      }
      pixel += pixx;
     }
    }
    break;
   default: /* case 4*/
    pixy -= (pixx*dx);
    for (; pixel<=pixellast; pixel += pixy) {
     for (x=0; x<dx; x++) {
      *(Uint32*)pixel = color;
      pixel += pixx;
     }
    }
    break;
  }

  /* Unlock surface */
  SDL_UnlockSurface(dst);
  
  result=0;

 } else {

#ifdef SURFACE_ALPHA_PIXEL
 
  /* Setup source rectangle for pixel */
  srect.x=0;
  srect.y=0;
  srect.w=w;
  srect.h=h;
 
  /* Setup rectangle for line */
  drect.x=x1;
  drect.y=y1;
  drect.w=w;
  drect.h=h;

  /* Maybe deallocate existing surface if size is too small */
  if ((gfxPrimitivesBox!=NULL) && ((gfxPrimitivesBox->w<w) || (gfxPrimitivesBox->h<h))) {
   SDL_FreeSurface(gfxPrimitivesBox);
   gfxPrimitivesBox=NULL;
  }
 
  /* Create box surface in destination format if necessary */
  if (gfxPrimitivesBox==NULL) {
   gfxPrimitivesBox=SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_HWSURFACE | SDL_SRCALPHA, w, h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  }

  /* Get alpha */
  a=(color & (Uint32)0x000000ff);

  /* Toggle alpha blending if necessary, reset otherwise */
  if (a != 255) {
   SDL_SetAlpha (gfxPrimitivesBox, SDL_SRCALPHA, 255);
  } else {
   SDL_SetAlpha (gfxPrimitivesBox, 0, 255);
  }
 
  /* Draw color into pixel*/
  SDL_FillRect (gfxPrimitivesBox, &srect, color);
 
  /* Draw pixel onto destination surface */
  result=SDL_BlitSurface (gfxPrimitivesBox, &srect, dst, &drect);

#else
 
 result=filledRectAlpha(dst,x1,y1,x1+w,y1+h,color);

#endif

 }
 
 return(result);
}