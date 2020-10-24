void
drawParallels(Context)
struct Sundata * Context;
{
        Pixel pixel;
        static  double val[5] = { -66.55, -23.45, 0.0, 23.45, 66.55 };
	double  f1, f2, spacing;
        int     i, b1, b2, parmode, numdigits;

        if (!Context->wintype || !Context->flags.parallel) return; 

	parmode = Context->flags.parallel & 3;

	if (Context->zoom.paralspacing)
	   spacing = Context->zoom.paralspacing;
	else
	   spacing = getSpacing(Context, 0);
        
        /* b = (int) (89.9/spacing); */
	f1 = (double)(Context->zoom.dy+Context->geom.height)/
                ((double)Context->zoom.height);
	f2 = (double)Context->zoom.dy/((double)Context->zoom.height);

	b1 = rint(0.7 + (90.0 - f1*180.0)/spacing);
	b2 = rint(-0.7 + (90.0 - f2*180.0)/spacing);

        pixel = Context->gdata->pixel[PARALLELCOLOR];

        if (Context->flags.colorlevel==FULLCOLORS || 
            Context->flags.colorlevel==MONOCHROME) {
           XSetFont(dpy, Context->gdata->pixgc, 
                         Context->gdata->font[COORDFONT]->fid);
        } else {
           XSetFont(dpy, Context->gdata->wingc, 
                         Context->gdata->font[COORDFONT]->fid);
           XSetForeground(dpy, Context->gdata->wingc, pixel);
	}
     
	numdigits = (spacing<1.0);
	if (parmode)
        for (i=b1; i<=b2; i++) if (i!=0 || Context->flags.parallel <=3)
           drawParallel(Context, pixel, i*spacing, 3, Context->flags.dotted, 
                       parmode, numdigits);

        pixel = Context->gdata->pixel[TROPICCOLOR];
        if (Context->flags.colorlevel==MANYCOLORS) 
           XSetForeground(dpy, Context->gdata->wingc, pixel);

	if (Context->flags.parallel & 8) {
           for (i=0; i<5; i++)
              drawParallel(Context, pixel, val[i], 3, 1, -parmode, numdigits);
	}
}
