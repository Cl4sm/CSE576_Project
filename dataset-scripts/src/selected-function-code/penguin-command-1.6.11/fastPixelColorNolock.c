int fastPixelColorNolock (SDL_Surface *dst, Sint16 x, Sint16 y, Uint32 color)
{
  int bpp; 
  Uint8 *p; 
      
  /* Honor clipping setup at pixel level */
  if ( (x >= clip_xmin(dst)) && 
       (x <= clip_xmax(dst)) && 
       (y >= clip_ymin(dst)) && 
       (y <= clip_ymax(dst)) ) {

    /* Get destination format */
    bpp = dst->format->BytesPerPixel;
    p = (Uint8 *)dst->pixels + y * dst->pitch + x * bpp;
    switch(bpp) {
    case 1:
        *p = color;
        break;
    case 2:
        *(Uint16 *)p = color;
        break;
    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (color >> 16) & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = color & 0xff;
        } else {
            p[0] = color & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = (color >> 16) & 0xff;
        }
        break;
    case 4:
        *(Uint32 *)p = color;
        break;
    } /* switch */
    

  }
  
  return(0);
}