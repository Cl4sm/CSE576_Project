int filledEllipseColor (SDL_Surface *dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color)
{
 int result;
 Sint16 x1,y1,x2,y2;
 int ix, iy;
 int h, i, j, k;
 int oh, oi, oj, ok;
 int xmh, xph;
 int xmi, xpi;
 int xmj, xpj;
 int xmk, xpk;
 
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
 if (rx > ry) {
  	ix = 0;
   	iy = rx * 64;

 		do {
	 		h = (ix + 32) >> 6;
	 		i = (iy + 32) >> 6;
	 		j = (h * ry) / rx;
	 		k = (i * ry) / rx;

			if ((ok!=k) && (oj!=k)) {
			 xph=x+h;
			 xmh=x-h;
 			 if (k>0) {
	 		  result |= hlineColor (dst,xmh,xph,y+k,color);
	   		  result |= hlineColor (dst,xmh,xph,y-k,color);
			 } else {
	   		  result |= hlineColor (dst,xmh,xph,y,color);
			 }
 			 ok=k;
			}
			if ((oj!=j) && (ok!=j) && (k!=j))  {
			 xmi=x-i;
			 xpi=x+i;
			 if (j>0) {
 	 		  result |= hlineColor (dst,xmi,xpi,y+j,color);
	  		  result |= hlineColor (dst,xmi,xpi,y-j,color);
			 } else {
	  		  result |= hlineColor (dst,xmi,xpi,y  ,color);
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

			if ((oi!=i) && (oh!=i)) {
			 xmj=x-j;
			 xpj=x+j; 
 			 if (i>0) {
	 		  result |= hlineColor (dst,xmj,xpj,y+i,color);
	   		  result |= hlineColor (dst,xmj,xpj,y-i,color);
		  	 } else {
	   		  result |= hlineColor (dst,xmj,xpj,y,color);
			 }
			 oi=i;
			}
			if ((oh!=h) && (oi!=h) && (i!=h)) {
			 xmk=x-k;
			 xpk=x+k;
			 if (h>0) {
 	 		  result |= hlineColor (dst,xmk,xpk,y+h,color);
	  		  result |= hlineColor (dst,xmk,xpk,y-h,color);
			 } else {
	  		  result |= hlineColor (dst,xmk,xpk,y  ,color);
			 }
			 oh=h;
			}
			
	 		ix = ix + iy / ry;
	 		iy = iy - ix / ry;

  	} while(i > h);
 }

 return(result);
}