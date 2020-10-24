int ellipseColor (SDL_Surface *dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color)
{
 int result;
 Sint16 x1, y1, x2, y2;
 int ix, iy;
 int h, i, j, k;
 int oh, oi, oj, ok;
 int xmh, xph, ypk, ymk;
 int xmi, xpi, ymj, ypj;
 int xmj, xpj, ymi, ypi;
 int xmk, xpk, ymh, yph;
 Uint8 *colorptr;
  
 /* Sanity check radii */  
 if ((rx<0) || (ry<0)) {
  return(-1);
 }  

 /* Special case for rx=0 - draw a vline */
 if (rx==0) {
  return(vlineColor (dst, x, y-ry, y+ry, color));  
 }
 /* Special case for ry=0 - draw a hline */
 if (ry==0) {
  return(hlineColor (dst, x-rx, x+rx, y, color));  
 }

 /* Test bounding box */
 x1=x-rx;
 y1=y-ry;
 x2=x+rx;
 y2=y+ry;
 if (!(clipLine(dst,&x1,&y1,&x2,&y2))) {
  return(0);
 }

 /* Init vars */
 oh = oi = oj = ok = 0xFFFF;
 
 /* Draw */
 result=0;
 /* Check alpha */
 if ((color & 255)==255) {

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
  
  if (rx > ry) {
  	ix = 0;
   	iy = rx * 64;

 		do {
	 		h = (ix + 32) >> 6;
	 		i = (iy + 32) >> 6;
	 		j = (h * ry) / rx;
	 		k = (i * ry) / rx;

			if (((ok!=k) && (oj!=k)) || ((oj!=j) && (ok!=j)) || (k!=j)) {
			 xph=x+h;
			 xmh=x-h;
 			 if (k>0) {
 			  ypk=y+k;
 			  ymk=y-k;
 			  result |= fastPixelColorNolock (dst,xmh,ypk, color);
 			  result |= fastPixelColorNolock (dst,xph,ypk, color);
 			  result |= fastPixelColorNolock (dst,xmh,ymk, color);
 			  result |= fastPixelColorNolock (dst,xph,ymk, color);
			 } else {
 			  result |= fastPixelColorNolock (dst,xmh,y, color);
 			  result |= fastPixelColorNolock (dst,xph,y, color);
			 }
 			 ok=k;
			 xpi=x+i;
			 xmi=x-i;
			 if (j>0) {
			  ypj=y+j;
			  ymj=y-j;
 			  result |= fastPixelColorNolock (dst,xmi,ypj, color);
 			  result |= fastPixelColorNolock (dst,xpi,ypj, color);
 			  result |= fastPixelColorNolock (dst,xmi,ymj, color);
 			  result |= fastPixelColorNolock (dst,xpi,ymj, color);
			 } else {
 			  result |= fastPixelColorNolock (dst,xmi,y, color);
 			  result |= fastPixelColorNolock (dst,xpi,y, color);
			 }
			 oj=j;
			}

			ix = ix + iy / rx;
	 		iy = iy - ix / rx;

		} while (i > h);
  } else {
  	ix = 0;
   	iy = ry * 64;

  	do {
	 		h = (ix + 32) >> 6;
	 		i = (iy + 32) >> 6;
	 		j = (h * rx) / ry;
	 		k = (i * rx) / ry;

			if (((oi!=i) && (oh!=i)) || ((oh!=h) && (oi!=h) && (i!=h))) {
			 xmj=x-j;
			 xpj=x+j;
 			 if (i>0) {
 			  ypi=y+i;
 			  ymi=y-i;
	 		  result |= fastPixelColorNolock (dst,xmj,ypi,color);
	 		  result |= fastPixelColorNolock (dst,xpj,ypi,color);
	 		  result |= fastPixelColorNolock (dst,xmj,ymi,color);
	 		  result |= fastPixelColorNolock (dst,xpj,ymi,color);
		  	 } else {
	 		  result |= fastPixelColorNolock (dst,xmj,y,color);
	 		  result |= fastPixelColorNolock (dst,xpj,y,color);
			 }
			 oi=i;
			 xmk=x-k;
			 xpk=x+k;
			 if (h>0) {
			  yph=y+h;
			  ymh=y-h;
 	 		  result |= fastPixelColorNolock (dst,xmk,yph,color);
 	 		  result |= fastPixelColorNolock (dst,xpk,yph,color);
 	 		  result |= fastPixelColorNolock (dst,xmk,ymh,color);
 	 		  result |= fastPixelColorNolock (dst,xpk,ymh,color);
			 } else {
 	 		  result |= fastPixelColorNolock (dst,xmk,y,color);
 	 		  result |= fastPixelColorNolock (dst,xpk,y,color);			 
			 }
			 oh=h;
			}
			
	 		ix = ix + iy / ry;
	 		iy = iy - ix / ry;

  	} while(i > h);
  }

  /* Unlock surface */
  SDL_UnlockSurface(dst);

 } else {
  
 if (rx > ry) {
  	ix = 0;
   	iy = rx * 64;

 		do {
	 		h = (ix + 32) >> 6;
	 		i = (iy + 32) >> 6;
	 		j = (h * ry) / rx;
	 		k = (i * ry) / rx;

			if (((ok!=k) && (oj!=k)) || ((oj!=j) && (ok!=j)) || (k!=j)) {
			 xph=x+h;
			 xmh=x-h;
 			 if (k>0) {
 			  ypk=y+k;
 			  ymk=y-k;
 			  result |= pixelColor (dst,xmh,ypk, color);
 			  result |= pixelColor (dst,xph,ypk, color);
 			  result |= pixelColor (dst,xmh,ymk, color);
 			  result |= pixelColor (dst,xph,ymk, color);
			 } else {
 			  result |= pixelColor (dst,xmh,y, color);
 			  result |= pixelColor (dst,xph,y, color);
			 }
 			 ok=k;
			 xpi=x+i;
			 xmi=x-i;
			 if (j>0) {
			  ypj=y+j;
			  ymj=y-j;
 			  result |= pixelColor (dst,xmi,ypj, color);
 			  result |= pixelColor (dst,xpi,ypj, color);
 			  result |= pixelColor (dst,xmi,ymj, color);
 			  result |= pixelColor (dst,xpi,ymj, color);
			 } else {
 			  result |= pixelColor (dst,xmi,y, color);
 			  result |= pixelColor (dst,xpi,y, color);
			 }
			 oj=j;
			}

			ix = ix + iy / rx;
	 		iy = iy - ix / rx;

		} while (i > h);
  } else {
  	ix = 0;
   	iy = ry * 64;

  	do {
	 		h = (ix + 32) >> 6;
	 		i = (iy + 32) >> 6;
	 		j = (h * rx) / ry;
	 		k = (i * rx) / ry;

			if (((oi!=i) && (oh!=i)) || ((oh!=h) && (oi!=h) && (i!=h))) {
			 xmj=x-j;
			 xpj=x+j;
 			 if (i>0) {
 			  ypi=y+i;
 			  ymi=y-i;
	 		  result |= pixelColor (dst,xmj,ypi,color);
	 		  result |= pixelColor (dst,xpj,ypi,color);
	 		  result |= pixelColor (dst,xmj,ymi,color);
	 		  result |= pixelColor (dst,xpj,ymi,color);
		  	 } else {
	 		  result |= pixelColor (dst,xmj,y,color);
	 		  result |= pixelColor (dst,xpj,y,color);
			 }
			 oi=i;
			 xmk=x-k;
			 xpk=x+k;
			 if (h>0) {
			  yph=y+h;
			  ymh=y-h;
 	 		  result |= pixelColor (dst,xmk,yph,color);
 	 		  result |= pixelColor (dst,xpk,yph,color);
 	 		  result |= pixelColor (dst,xmk,ymh,color);
 	 		  result |= pixelColor (dst,xpk,ymh,color);
			 } else {
 	 		  result |= pixelColor (dst,xmk,y,color);
 	 		  result |= pixelColor (dst,xpk,y,color);			 
			 }
			 oh=h;
			}
			
	 		ix = ix + iy / ry;
	 		iy = iy - ix / ry;

  	} while(i > h);
 }

 } /* Alpha check */
 
 return(result);
}