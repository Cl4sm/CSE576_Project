int rectangleColor (SDL_Surface *dst, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint32 color) 
{
 int result;
 Sint16 w,h, xtmp, ytmp;


 /* Swap x1, x2 if required */
 if (x1>x2) {
  xtmp=x1; x1=x2; x2=xtmp;
 }

 /* Swap y1, y2 if required */
 if (y1>y2) {
  ytmp=y1; y1=y2; y2=ytmp;
 }
 
 /* Calculate width&height */
 w=x2-x1;
 h=y2-y1;

 /* Sanity check */
 if ((w<0) || (h<0)) {
  return(0);
 }

 /* Test for special cases of straight lines or single point */
 if (x1==x2) {
  if (y1==y2) {
   return(pixelColor(dst, x1, y1, color));
  } else {
   return(vlineColor(dst, x1, y1, y2, color));
  }
 } else {
  if (y1==y2) { 
   return(hlineColor(dst, x1, x2, y1, color));
  }
 }
 
 /* Draw rectangle */
 result=0;
 result |= vlineColor(dst, x1, y1, y2, color);
 result |= vlineColor(dst, x2, y1, y2, color);
 result |= hlineColor(dst, x1, x2, y1, color);
 result |= hlineColor(dst, x1, x2, y2, color);
 
 return(result);

}