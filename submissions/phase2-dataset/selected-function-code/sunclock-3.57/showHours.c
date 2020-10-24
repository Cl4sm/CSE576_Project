void
showHours(Context)
struct Sundata * Context;
{
        int i, x;
        char s[128];

        if (Context->flags.hours_shown) return;
        clearStrip(Context);
        for (i=0; i<24; i++) {
            sprintf(s, "%d", i);
            x = ((i*Context->zoom.width)/24 + 2*Context->zoom.width
                - (Context->gdata->mapstrip+5)*strlen(s)/8 
                + (int)(Context->sunlon*Context->zoom.width/360.0))%
                  Context->zoom.width + 1 - Context->zoom.dx;
            if (x>=0 && x<Context->geom.width) {
               XSetBackground(dpy, Context->gdata->wingc, 
                   Context->gdata->pixel[MAPSTRIPBGCOLOR]);
               XSetForeground(dpy, Context->gdata->wingc, 
                   Context->gdata->pixel[MAPSTRIPFGCOLOR]);
               XSetFont(dpy, Context->gdata->wingc, 
                   Context->gdata->font[MAPSTRIPFONT]->fid);
               XDrawImageString(dpy, Context->win, 
                  Context->gdata->wingc, 
                  x, Context->gdata->font[MENUFONT]->max_bounds.ascent + 
                     Context->geom.height + 4, 
                  s, strlen(s));
	    }
        }
        Context->flags.hours_shown = 1;
}
