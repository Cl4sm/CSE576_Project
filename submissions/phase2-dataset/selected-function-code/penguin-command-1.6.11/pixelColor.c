int pixelColor (SDL_Surface *dst, Sint16 x, Sint16 y, Uint32 color) 
{
 Uint8 alpha;
 Uint32 mcolor;
 int result=0;

 /* Lock the surface */
 if ( SDL_MUSTLOCK(dst) ) {
  if ( SDL_LockSurface(dst) < 0 ) {
   return(-1);
  }
 }
 
 /* Setup color */
 alpha=color &0x000000ff;
 mcolor=SDL_MapRGBA(dst->format, (color & 0xff000000) >> 24, (color & 0x00ff0000) >> 16, (color & 0x0000ff00) >> 8, alpha);

 /* Draw */
 result = _putPixelAlpha(dst,x,y,mcolor,alpha);
	
 /* Unlock the surface */
 if (SDL_MUSTLOCK(dst) ) {
  SDL_UnlockSurface(dst);
 }

 return(result);
}