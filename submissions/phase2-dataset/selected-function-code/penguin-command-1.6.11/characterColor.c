int characterColor (SDL_Surface *dst, Sint16 x, Sint16 y, char c, Uint32 color)
{
 SDL_Rect srect;
 SDL_Rect drect;
 int result;
 int ix, iy, k;
 unsigned char *charpos;
 unsigned char bits[8]={128,64,32,16,8,4,2,1};
 unsigned char *bitpos;
 Uint8 *curpos;
 int forced_redraw;

 /* Setup source rectangle for 8x8 bitmap */
 srect.x=0;
 srect.y=0;
 srect.w=8;
 srect.h=8;
 
 /* Setup destination rectangle for 8x8 bitmap */
 drect.x=x;
 drect.y=y;
 drect.w=8;
 drect.h=8;

 /* Create new 8x8 bitmap surface if not already present */
 if (gfxPrimitivesFont[(unsigned char)c]==NULL) {
  gfxPrimitivesFont[(unsigned char)c]=SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_HWSURFACE | SDL_SRCALPHA, 8, 8, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
  /* Check pointer */
  if (gfxPrimitivesFont[(unsigned char)c]==NULL) {
   return (-1);
  }
  /* Definitely redraw */
  forced_redraw=1;
 } else {
  forced_redraw=0;
 }

 /* Check if color has changed */
 if ((gfxPrimitivesFontColor[(unsigned char)c]!=color) || (forced_redraw)) {
   /* Redraw character */
   SDL_SetAlpha (gfxPrimitivesFont[(unsigned char)c], SDL_SRCALPHA, 255);
   gfxPrimitivesFontColor[(unsigned char)c]=color;

   /* Variable setup */
   k = (unsigned char)c;
   k *= 8;
   charpos=gfxPrimitivesFontdata;
   charpos += k;   
  
   /* Clear bitmap */
   curpos=(Uint8 *)gfxPrimitivesFont[(unsigned char)c]->pixels;
   memset (curpos, 0, 8*8*4);
 
   /* Drawing loop */
   for (iy=0; iy<8; iy++) {
    bitpos=bits;
    for (ix=0; ix<8; ix++) {
     if ((*charpos & *bitpos)==*bitpos) {
      memcpy(curpos,&color,4);
     } 
     bitpos++;
     curpos += 4;;
    }
    charpos++;
   }
 }

 /* Draw bitmap onto destination surface */
 result=SDL_BlitSurface (gfxPrimitivesFont[(unsigned char)c], &srect, dst, &drect);
 
 return(result);
}