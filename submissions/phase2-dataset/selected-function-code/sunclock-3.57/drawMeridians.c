void
drawMeridians(Context)
struct Sundata * Context;
{
        int     i, b1, b2, numdigits;
	double  spacing, f1, f2;

        if (!Context->wintype || !Context->flags.meridian) return; 

	if (Context->zoom.meridspacing)
	   spacing = Context->zoom.meridspacing;
	else
	   spacing = getSpacing(Context, 1);
        
        /* b = (int) (179.9/spacing); */
	f1 = (double)Context->zoom.dx/((double)Context->zoom.width);
	f2 = (double)(Context->zoom.dx+Context->geom.width)/
                ((double)Context->zoom.width);

	b1 = rint(0.7 + (f1*360.0 - 180.0)/spacing);
	b2 = rint(-0.7 + (f2*360.0 - 180.0)/spacing);

        if (Context->flags.colorlevel==FULLCOLORS || 
            Context->flags.colorlevel==MONOCHROME) {
           XSetFont(dpy, Context->gdata->pixgc, 
                         Context->gdata->font[COORDFONT]->fid);
        } else {
           XSetFont(dpy, Context->gdata->wingc, 
                         Context->gdata->font[COORDFONT]->fid);
           XSetForeground(dpy, Context->gdata->wingc, 
                          Context->gdata->pixel[MERIDIANCOLOR]);
	}
 
	numdigits = (spacing<1.0);
        for (i=b1; i<=b2; i++)
          drawMeridian(Context, i*spacing, 3, Context->flags.dotted, numdigits);
}
