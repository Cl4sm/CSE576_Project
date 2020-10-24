int aaellipseColor(SDL_Surface *dst, Sint16 x, Sint16 y, Sint16 rx, Sint16 ry, Uint32 color)
{
 int result;
 Sint16 x1, y1, x2, y2;
 double angle;
 double deltaAngle;
 double drx, dry, dr;
 int posX, posY, oldPosX, oldPosY;
 int i, r;

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

 /* Draw */
 r=(rx+ry)>>1;
 dr=(double)r;
 drx=(double)rx;
 dry=(double)ry;
 deltaAngle=(2*M_PI)/dr;
 angle=deltaAngle;
 oldPosX=x+rx;
 oldPosY=y;

 result=0;
 for(i=0; i<r; i++) {
  posX=x+(int)(drx*cos(angle));
  posY=y+(int)(dry*sin(angle));
  result |= aalineColorInt (dst, oldPosX, oldPosY, posX, posY, color, 1);
  oldPosX=posX;
  oldPosY=posY;
  angle += deltaAngle;
 }

 return (result);
}