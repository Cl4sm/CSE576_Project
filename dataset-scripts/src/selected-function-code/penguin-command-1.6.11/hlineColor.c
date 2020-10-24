int hlineColor (SDL_Surface *dst, Sint16 x1, Sint16 x2, Sint16 y, Uint32 color) 
{
 Sint16 left,right,top,bottom;
 Uint8 *pixel,*pixellast;
 int dx;
 int pixx, pixy;
 Sint16 w;
 Sint16 xtmp;
 int result=-1;
 Uint8 *colorptr;
#ifdef SURFACE_ALPHA_PIXEL
 Uint32 a;
 SDL_Rect srect;
 SDL_Rect drect;
#endif
 
 /* Get clipping boundary */
 left = dst->clip_rect.x;
 right = dst->clip_rect.x+dst->clip_rect.w-1;
 top = dst->clip_rect.y;
 bottom = dst->clip_rect.y+dst->clip_rect.h-1;

 /* Swap x1, x2 if required */
 if (x1>x2) {
  xtmp=x1; x1=x2; x2=xtmp;
 }

 /* Visible */
 if ((x1>right) || (x2<left) || (y<top) || (y>bottom)) {
  return(0);
 }
 
 /* Clip x */
 if (x1<left) { 
  x1=left;
 } 
 if (x2>right) {
  x2=right;
 }
  
 /* Calculate width */
 w=x2-x1;
 
 /* Sanity check on width */
 if (w<0) {
  return(0);
 }
 
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
  pixx = dst->format->BytesPerPixel;
  pixy = dst->pitch;
  pixel = ((Uint8*)dst->pixels) + pixx * (int)x1 + pixy * (int)y;
  
  /* Draw */
  switch(dst->format->BytesPerPixel) {
   case 1:
    memset (pixel, color, dx);
    break;
   case 2:
    pixellast = pixel + dx + dx;
    for (; pixel<=pixellast; pixel += pixx) {
     *(Uint16*)pixel = color;
    }
    break;
   case 3:
    pixellast = pixel + dx + dx + dx;
    for (; pixel<=pixellast; pixel += pixx) {
     if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
      pixel[0] = (color >> 16) & 0xff;
      pixel[1] = (color >> 8) & 0xff;
      pixel[2] = color & 0xff;
     } else {
      pixel[0] = color & 0xff;
      pixel[1] = (color >> 8) & 0xff;
      pixel[2] = (color >> 16) & 0xff;
     }
    }
    break;
   default: /* case 4*/
    dx = dx + dx;
    pixellast = pixel + dx + dx;
    for (; pixel<=pixellast; pixel += pixx) {
     *(Uint32*)pixel = color;
    }
    break;
  }

  /* Unlock surface */
  SDL_UnlockSurface(dst);

  /* Set result code */
  result=0;

 } else {

  /* Alpha blending blit */

#ifdef SURFACE_ALPHA_PIXEL

  /* Adjust width for Rect setup */
  w++;
 
  /* Setup source rectangle for pixel */
  srect.x=0;
  srect.y=0;
  srect.w=w;
  srect.h=1;
 
  /* Setup rectangle for destination line */
  drect.x=x1;
  drect.y=y;
  drect.w=w;
  drect.h=1;
 
  /* Maybe deallocate existing surface if size is too small */
  if ((gfxPrimitivesHline!=NULL) && (gfxPrimitivesHline->w<w) ) {
   SDL_FreeSurface(gfxPrimitivesHline);
   gfxPrimitivesHline=NULL;
  }
 
  /* Create horizontal line surface in destination format if necessary */
  if (gfxPrimitivesHline==NULL) {
   gfxPrimitivesHline=SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_HWSURFACE | SDL_SRCALPHA, w, 1, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  }

  /* Get alpha */
  a=(color & (Uint32)0x000000ff);

  /* Toggle alpha blending if necessary, reset otherwise */
  if (a != 255) { 
   SDL_SetAlpha (gfxPrimitivesHline, SDL_SRCALPHA, 255); 
  } else {
   SDL_SetAlpha (gfxPrimitivesHline, 0, 255);
  }
 
  /* Draw color into pixel*/
  SDL_FillRect (gfxPrimitivesHline, &srect, color);
 
  /* Draw pixel onto destination surface */
  result=SDL_BlitSurface (gfxPrimitivesHline, &srect, dst, &drect);

#else

 result=HLineAlpha (dst, x1, x1+w, y, color);

#endif

 }
  
 return(result);
}