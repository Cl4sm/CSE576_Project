drawMeridian(Context, lon, step, thickness, numdigits)
struct Sundata * Context;
double lon;
int step;
int thickness;
int numdigits;
{
        int ilon, i0, i1, i, j, jp, k, min, max;
	char s[10], format[10];

        ilon = int_longitude(Context, lon);

        i = Context->flags.parallel & 3 ;
	min = 0;
	max = Context->geom.width;
	if (i==2) min = coordvalwidth;
	if (i==3) max = max-coordvalwidth;

        if (ilon<min || ilon>=max) return;

        i0 = Context->geom.height/2;
        i1 = 1+i0/step;
	if (Context->flags.meridian>=2) {
           sprintf(format, "%%.%df°", numdigits);
	   sprintf(s, format, lon);
	   i = 2*XTextWidth(Context->gdata->font[COORDFONT], s, strlen(s))/5;
	   min = Context->gdata->font[COORDFONT]->max_bounds.ascent +
               Context->gdata->font[COORDFONT]->max_bounds.descent + 3;
	   max = (int) Context->geom.height;
	   if (Context->flags.meridian==2) {
	      j = Context->geom.height-3;
	      max = max-min-1;
	      min = 0;
	   } else
	      j = min-4;
	   if (Context->flags.colorlevel==FULLCOLORS)
              XPutStringImage(Context, ilon-i, j, s, strlen(s), 1);
	   else
	   if (Context->flags.colorlevel>=MANYCOLORS)
              XDrawString(dpy, Context->win, Context->gdata->wingc, 
                       ilon-i, j, s, strlen(s));
	   else
              XDrawString(dpy, Context->mappix, Context->gdata->pixgc, 
                       ilon-i, j, s, strlen(s));
	} else {
	   min = 0;
	   max = (int) Context->geom.height - 1 ;
	}

        for (i=-i1; i<i1; i+=1) {
           j = i0+step*i-thickness;
           jp = i0+step*i+thickness;
           if (j<0) j = 0;
           if (jp>max) jp = max;
	   if (jp<min) continue;
	   if (j>max) continue;
	   if (Context->flags.colorlevel==FULLCOLORS) {
	      for (k=j; k<=jp; k++)
	         SetPixelLight(Context, ilon, k,
                               Context->gdata->pixel[MERIDIANCOLOR]);
	   } else
	   if (Context->flags.colorlevel >= MANYCOLORS)
              XDrawLine(dpy, Context->win, Context->gdata->wingc, 
                        ilon, j, ilon, jp);
	   else
	      XDrawLine(dpy, Context->mappix, Context->gdata->pixgc, 
                        ilon, j, ilon, jp);
	}
}
