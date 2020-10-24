static int clipLine(SDL_Surface *dst, Sint16 *x1, Sint16 *y1, Sint16 *x2, Sint16 *y2)
{
 Sint16 left,right,top,bottom;
 int code1, code2;
 int draw = 0;
 Sint16 swaptmp;
 float m;

 /* Get clipping boundary */
 left = dst->clip_rect.x;
 right = dst->clip_rect.x+dst->clip_rect.w-1;
 top = dst->clip_rect.y;
 bottom = dst->clip_rect.y+dst->clip_rect.h-1;

 while (1) {
  code1 = clipEncode (*x1, *y1, left, top, right, bottom);
  code2 = clipEncode (*x2, *y2, left, top, right, bottom);
  if (CLIP_ACCEPT(code1, code2)) {
   draw = 1;
   break;
  } else if (CLIP_REJECT(code1, code2))
   break;
  else {
   if(CLIP_INSIDE (code1)) {
    swaptmp = *x2; *x2 = *x1; *x1 = swaptmp;
    swaptmp = *y2; *y2 = *y1; *y1 = swaptmp;
    swaptmp = code2; code2 = code1; code1 = swaptmp;
   }
   if (*x2 != *x1) {      
    m = (*y2 - *y1) / (float)(*x2 - *x1);
   } else {
    m = 1.0f;
   }
   if (code1 & CLIP_LEFT_EDGE) {
    *y1 += (Sint16)((left - *x1) * m);
    *x1 = left; 
   } else if (code1 & CLIP_RIGHT_EDGE) {
    *y1 += (Sint16)((right - *x1) * m);
    *x1 = right; 
   } else if (code1 & CLIP_BOTTOM_EDGE) {
    if (*x2 != *x1) {
     *x1 += (Sint16)((bottom - *y1) / m);
    }
    *y1 = bottom;
   } else if (code1 & CLIP_TOP_EDGE) {
    if (*x2 != *x1) {
     *x1 += (Sint16)((top - *y1) / m);
    }
    *y1 = top;
   } 
  }
 }
 
 return draw;
}