int polygonColor (SDL_Surface *dst, Sint16 *vx, Sint16 *vy, int n, Uint32 color)
{
 int result;
 int i;
 Sint16 *x1, *y1, *x2, *y2;

 /* Sanity check */
 if (n<3) {
  return(-1);
 }

 /* Pointer setup */ 
 x1=x2=vx;
 y1=y2=vy;
 x2++;
 y2++;
 
 /* Draw */
 result=0;
 for (i=1; i<n; i++) {
  result |= lineColor (dst, *x1, *y1, *x2, *y2, color);
  x1=x2;
  y1=y2;
  x2++; 
  y2++; 
 }
 result |= lineColor (dst, *x1, *y1, *vx, *vy, color);

 return(result);
}