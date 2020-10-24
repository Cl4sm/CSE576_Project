int _putPixelAlpha(SDL_Surface *surface, Sint16 x, Sint16 y, Uint32 color, Uint8 alpha)
{
 Uint32 Rmask = surface->format->Rmask, Gmask = surface->format->Gmask, Bmask = surface->format->Bmask, Amask = surface->format->Amask;
 Uint32 R,G,B,A = 0;
	
 if (x>=clip_xmin(surface) && x<=clip_xmax(surface) && y>=clip_ymin(surface) && y<=clip_ymax(surface)) {

  switch (surface->format->BytesPerPixel) {
			case 1: { /* Assuming 8-bpp */
				if( alpha == 255 ){
					*((Uint8 *)surface->pixels + y*surface->pitch + x) = color;
				} else {
					Uint8 *pixel = (Uint8 *)surface->pixels + y*surface->pitch + x;
					
					Uint8 dR = surface->format->palette->colors[*pixel].r;
					Uint8 dG = surface->format->palette->colors[*pixel].g;
					Uint8 dB = surface->format->palette->colors[*pixel].b;
					Uint8 sR = surface->format->palette->colors[color].r;
					Uint8 sG = surface->format->palette->colors[color].g;
					Uint8 sB = surface->format->palette->colors[color].b;
					
					dR = dR + ((sR-dR)*alpha >> 8);
					dG = dG + ((sG-dG)*alpha >> 8);
					dB = dB + ((sB-dB)*alpha >> 8);
				
					*pixel = SDL_MapRGB(surface->format, dR, dG, dB);
				}
			}
			break;

			case 2: { /* Probably 15-bpp or 16-bpp */		
				if ( alpha == 255 ) {
					*((Uint16 *)surface->pixels + y*surface->pitch/2 + x) = color;
				} else {
					Uint16 *pixel = (Uint16 *)surface->pixels + y*surface->pitch/2 + x;
					Uint32 dc = *pixel;
				
					R = ((dc & Rmask) + (( (color & Rmask) - (dc & Rmask) ) * alpha >> 8)) & Rmask;
					G = ((dc & Gmask) + (( (color & Gmask) - (dc & Gmask) ) * alpha >> 8)) & Gmask;
					B = ((dc & Bmask) + (( (color & Bmask) - (dc & Bmask) ) * alpha >> 8)) & Bmask;
					if( Amask )
						A = ((dc & Amask) + (( (color & Amask) - (dc & Amask) ) * alpha >> 8)) & Amask;

					*pixel= R | G | B | A;
				}
			}
			break;

			case 3: { /* Slow 24-bpp mode, usually not used */
				Uint8 *pix = (Uint8 *)surface->pixels + y * surface->pitch + x*3;
				Uint8 rshift8=surface->format->Rshift/8;
				Uint8 gshift8=surface->format->Gshift/8;
				Uint8 bshift8=surface->format->Bshift/8;
				Uint8 ashift8=surface->format->Ashift/8;
				
				
				if ( alpha == 255 ) {
  					*(pix+rshift8) = color>>surface->format->Rshift;
  					*(pix+gshift8) = color>>surface->format->Gshift;
  					*(pix+bshift8) = color>>surface->format->Bshift;
					*(pix+ashift8) = color>>surface->format->Ashift;
				} else {
					Uint8 dR, dG, dB, dA=0;
					Uint8 sR, sG, sB, sA=0;
					
					pix = (Uint8 *)surface->pixels + y * surface->pitch + x*3;
					
					dR = *((pix)+rshift8); 
            				dG = *((pix)+gshift8);
            				dB = *((pix)+bshift8);
					dA = *((pix)+ashift8);
					
					sR = (color>>surface->format->Rshift)&0xff;
					sG = (color>>surface->format->Gshift)&0xff;
					sB = (color>>surface->format->Bshift)&0xff;
					sA = (color>>surface->format->Ashift)&0xff;
					
					dR = dR + ((sR-dR)*alpha >> 8);
					dG = dG + ((sG-dG)*alpha >> 8);
					dB = dB + ((sB-dB)*alpha >> 8);
					dA = dA + ((sA-dA)*alpha >> 8);

					*((pix)+rshift8) = dR; 
            				*((pix)+gshift8) = dG;
            				*((pix)+bshift8) = dB;
					*((pix)+ashift8) = dA;
				}
			}
			break;

			case 4: { /* Probably 32-bpp */
				if ( alpha == 255 ) {
					*((Uint32 *)surface->pixels + y*surface->pitch/4 + x) = color;
				} else {
					Uint32 *pixel = (Uint32 *)surface->pixels + y*surface->pitch/4 + x;
					Uint32 dc = *pixel;
			
					R = ((dc & Rmask) + (( (color & Rmask) - (dc & Rmask) ) * alpha >> 8)) & Rmask;
					G = ((dc & Gmask) + (( (color & Gmask) - (dc & Gmask) ) * alpha >> 8)) & Gmask;
					B = ((dc & Bmask) + (( (color & Bmask) - (dc & Bmask) ) * alpha >> 8)) & Bmask;
					if ( Amask )
						A = ((dc & Amask) + (( (color & Amask) - (dc & Amask) ) * alpha >> 8)) & Amask;
					
					*pixel = R | G | B | A;
				}
			}
			break;
  }
 }

 return(0);
}