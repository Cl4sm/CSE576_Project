int _filledRectAlpha (SDL_Surface *surface, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color, Uint8 alpha)
{
 Uint32 Rmask = surface->format->Rmask, Gmask = surface->format->Gmask, Bmask = surface->format->Bmask, Amask = surface->format->Amask;
 Uint32 R,G,B,A=0;
 Sint16 x,y;

 switch (surface->format->BytesPerPixel) {
		case 1: { /* Assuming 8-bpp */
			Uint8 *row, *pixel;
			Uint8 dR, dG, dB;
			
			Uint8 sR = surface->format->palette->colors[color].r;
			Uint8 sG = surface->format->palette->colors[color].g;
			Uint8 sB = surface->format->palette->colors[color].b;
			
			for(y = y1; y<=y2; y++){
				row = (Uint8 *)surface->pixels + y*surface->pitch;
				for(x = x1; x <= x2; x++){
					pixel = row + x;
					
					dR = surface->format->palette->colors[*pixel].r;
					dG = surface->format->palette->colors[*pixel].g;
					dB = surface->format->palette->colors[*pixel].b;
					
					dR = dR + ((sR-dR)*alpha >> 8);
					dG = dG + ((sG-dG)*alpha >> 8);
					dB = dB + ((sB-dB)*alpha >> 8);
				
					*pixel = SDL_MapRGB(surface->format, dR, dG, dB);
				}
			}
		}
		break;

		case 2: { /* Probably 15-bpp or 16-bpp */
			Uint16 *row, *pixel;
			Uint32 dR=(color & Rmask),dG=(color & Gmask),dB=(color & Bmask),dA=(color & Amask);
			
			for(y = y1; y<=y2; y++){
				row = (Uint16 *)surface->pixels + y*surface->pitch/2;
				for(x = x1; x <= x2; x++){
					pixel = row + x;

					R = ((*pixel & Rmask) + (( dR - (*pixel & Rmask) ) * alpha >> 8)) & Rmask;
					G = ((*pixel & Gmask) + (( dG - (*pixel & Gmask) ) * alpha >> 8)) & Gmask;
					B = ((*pixel & Bmask) + (( dB - (*pixel & Bmask) ) * alpha >> 8)) & Bmask;
					if( Amask )
						A = ((*pixel & Amask) + (( dA - (*pixel & Amask) ) * alpha >> 8)) & Amask;

					*pixel= R | G | B | A;
				}
			}
		}
		break;

		case 3: { /* Slow 24-bpp mode, usually not used */
			Uint8 *row,*pix;
			Uint8 dR, dG, dB, dA;
  			Uint8 rshift8=surface->format->Rshift/8; 
			Uint8 gshift8=surface->format->Gshift/8; 
			Uint8 bshift8=surface->format->Bshift/8;
			Uint8 ashift8=surface->format->Ashift/8;
			
			Uint8 sR = (color>>surface->format->Rshift)&0xff;
			Uint8 sG = (color>>surface->format->Gshift)&0xff;
			Uint8 sB = (color>>surface->format->Bshift)&0xff;
			Uint8 sA = (color>>surface->format->Ashift)&0xff;
				
			for(y = y1; y<=y2; y++){
				row = (Uint8 *)surface->pixels + y * surface->pitch;
				for(x = x1; x <= x2; x++){
					pix = row + x*3;

					dR = *((pix)+rshift8); 
            				dG = *((pix)+gshift8);
            				dB = *((pix)+bshift8);
					dA = *((pix)+ashift8);
					
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
					
		}
		break;

		case 4: { /* Probably 32-bpp */
			Uint32 *row, *pixel;
			Uint32 dR=(color & Rmask),dG=(color & Gmask),dB=(color & Bmask),dA=(color & Amask);
		
			for(y = y1; y<=y2; y++){
				row = (Uint32 *)surface->pixels + y*surface->pitch/4;
				for(x = x1; x <= x2; x++){
					pixel = row + x;

					R = ((*pixel & Rmask) + (( dR - (*pixel & Rmask) ) * alpha >> 8)) & Rmask;
					G = ((*pixel & Gmask) + (( dG - (*pixel & Gmask) ) * alpha >> 8)) & Gmask;
					B = ((*pixel & Bmask) + (( dB - (*pixel & Bmask) ) * alpha >> 8)) & Bmask;
					if( Amask )
						A = ((*pixel & Amask) + (( dA - (*pixel & Amask) ) * alpha >> 8)) & Amask;

					*pixel= R | G | B | A;
				}
			}
		}
		break;
 }
	
 return(0);
}