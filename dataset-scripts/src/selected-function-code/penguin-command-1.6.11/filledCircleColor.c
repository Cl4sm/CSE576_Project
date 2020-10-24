int filledCircleColor(SDL_Surface *dst, Sint16 x, Sint16 y, Sint16 r, Uint32 color)
{
 int result;
 Sint16 x1,y1,x2,y2;
 Sint16 cx = 0;
 Sint16 cy = r;
 Sint16 ocx = (Sint16)0xffff;
 Sint16 ocy = (Sint16)0xffff;
 Sint16 df = 1 - r;
 Sint16 d_e = 3;
 Sint16 d_se = -2 * r + 5;
 Sint16 xpcx, xmcx, xpcy, xmcy;
 Sint16 ypcy, ymcy, ypcx, ymcx;

 /* Sanity check radius */
 if (r<0) {
  return(-1);
 }

 /* Special case for r=0 - draw a point */
 if (r==0) {
  return(pixelColor (dst, x, y, color));  
 }

 /* Test bounding box */
 x1=x-r;
 y1=y-r;
 x2=x+r;
 y2=y+r;
 if (!(clipLine(dst,&x1,&y1,&x2,&y2))) {
  return(0);
 }

 /* Draw */
 result=0;
 do {
   xpcx=x+cx;
   xmcx=x-cx;
   xpcy=x+cy;
   xmcy=x-cy;
   if (ocy!=cy) {
    if (cy>0) {
     ypcy=y+cy;
     ymcy=y-cy;
     result |= hlineColor(dst,xmcx,xpcx,ypcy,color);
     result |= hlineColor(dst,xmcx,xpcx,ymcy,color);
    } else {
     result |= hlineColor(dst,xmcx,xpcx,y,color);
    }
    ocy=cy;
   }
   if (ocx!=cx) {
    if (cx!=cy) {
     if (cx>0) {
      ypcx=y+cx;
      ymcx=y-cx;
      result |= hlineColor(dst,xmcy,xpcy,ymcx,color);
      result |= hlineColor(dst,xmcy,xpcy,ypcx,color);
     } else {
      result |= hlineColor(dst,xmcy,xpcy,y,color);
     }
    }
    ocx=cx;
   }
   /* Update */
   if (df < 0)  {
    df += d_e;
    d_e += 2;
    d_se += 2;
   } else {
    df += d_se;
    d_e += 2;
    d_se += 4;
    cy--;
   }
   cx++;
 } while(cx <= cy);

 return(result);
}