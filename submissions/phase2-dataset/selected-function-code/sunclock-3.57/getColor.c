getColor(Context, num)
Sundata *        Context;
int              num;
{

        XColor                  c;
        XColor                  e;
        register Status         s;

        ++total_colors;

        if (Context->flags.colorlevel == MONOCHROME) goto monochrome;

        s = XAllocNamedColor(dpy, Context->gdata->cmap, Color[num], &c, &e);

        if (s != (Status)1) {
            fprintf(stderr, "%s: warning: can't allocate color `%s'\n",
                    ProgName, Color[num]);
            color_alloc_failed = 1;
 	 monochrome:
	    s = XAllocNamedColor(dpy, Context->gdata->cmap, 
                (num<FALLBACKTOWHITE)? "White" : "Black", &c, &e);
	} 
         
        return(c.pixel);
}
