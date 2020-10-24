int vlineColor (SDL_Surface *dst, Sint16 x, Sint16 y1, Sint16 y2, Uint32 color) 
{
 Sint16 left,right,top,bottom;
 Uint8 *pixel, *pixellast;
 int dy;
 int pixx, pixy;
 Sint16 h;
 Sint16 ytmp;
 int result=-1;
 Uint8 *colorptr;
#ifdef SURFACE_ALPHA_PIXEL
 SDL_Rect srect;
 SDL_Rect drect;
 Uint32 a;
#endif
 
 /* Get clipping boundary */
 left = dst->clip_rect.x;
 right = dst->clip_rect.x+dst->clip_rect.w-1;
 top = dst->clip_rect.y;
 bottom = dst->clip_rect.y+dst->clip_rect.h-1;

 /* Swap y1, y2 if required */
 if (y1>y2) {
  ytmp=y1; y1=y2; y2=ytmp;
 }

 /* Visible */
 if ((y2<top) || (y1>bottom) || (x<left) || (x>right)) {
  return(0);
 }

 /* Clip y */
 if (y1<top) { 
  y1=top;
 } 
 if (y2>bottom) {
  y2=bottom;
 }
   
 /* Calculate height */
 h=y2-y1;
 
 /* Sanity check on height */
 if (h<0) {
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
  dy=h;
  pixx = dst->format->BytesPerPixel;
  pixy = dst->pitch;
  pixel = ((Uint8*)dst->pixels) + pixx * (int)x + pixy * (int)y1;
  pixellast = pixel + pixy*dy;
  
  /* Draw */
  switch(dst->format->BytesPerPixel) {
   case 1:
    for (; pixel<=pixellast; pixel += pixy) {
     *(Uint8*)pixel = color;
    }
    break;
   case 2:
    for (; pixel<=pixellast; pixel += pixy) {
     *(Uint16*)pixel = color;
    }
    break;
   case 3:
    for (; pixel<=pixellast; pixel += pixy) {
     if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
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
    for (; pixel<=pixellast; pixel += pixy) {
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

  /* Adjust height for Rect setup */
  h++;
  
  /* Setup source rectangle for pixel */
  srect.x=0;
  srect.y=0;
  srect.w=1;
  srect.h=h;
 
  /* Setup rectangle for line */
  drect.x=x;
  drect.y=y1;
  drect.w=1;
  drect.h=h;

  /* Maybe deallocate existing surface if size is too small */
  if ( (gfxPrimitivesVline!=NULL) && (gfxPrimitivesVline->h<h) ) {
   SDL_FreeSurface(gfxPrimitivesVline);
   gfxPrimitivesVline=NULL;
  }
 
  /* Create horizontal line surface in destination format if necessary */
  if (gfxPrimitivesVline==NULL) {
   gfxPrimitivesVline=SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_HWSURFACE | SDL_SRCALPHA, 1, h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  }

  /* Get alpha */
  a=(color & (Uint32)0x000000ff);

  /* Toggle alpha blending if necessary, reset otherwise */
  if (a != 255) {
   SDL_SetAlpha (gfxPrimitivesVline, SDL_SRCALPHA, 255);
  } else {
   SDL_SetAlpha (gfxPrimitivesVline, 0, 255);
  }
 
  /* Draw color into pixel*/
  SDL_FillRect (gfxPrimitivesVline, &srect, color);
 
  /* Draw Vline onto destination surface */
  result=SDL_BlitSurface (gfxPrimitivesVline, &srect, dst, &drect);

#else

 result=VLineAlpha(dst,x, y1, y1+h, color);

#endif
 
 } 

 return(result);
}