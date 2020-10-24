showMapImage(Context)
struct Sundata * Context;
{
        if (button_pressed) return;

        if (!Context->flags.mapped) {
	   Context->flags.update = 0;
	   return;
	}

        if (Context->flags.update>=2) {
           if (Context->flags.colorlevel == FULLCOLORS) {
	       drawAll(Context);
               XPutImage(dpy, Context->win, Context->gdata->wingc, 
                    Context->xim, 0, 0, 0, 0,
                    Context->geom.width, Context->geom.height);
           } else {
	       XSetBackground(dpy, Context->gdata->wingc, 
                     Context->gdata->pixel[CLOCKBGCOLOR+Context->wintype]);
	       XSetForeground(dpy, Context->gdata->wingc, 
                     Context->gdata->pixel[CLOCKFGCOLOR+Context->wintype]);
               XCopyPlane(dpy, Context->mappix, Context->win, 
		    Context->gdata->wingc,
                    0, 0, Context->geom.width, Context->geom.height, 0, 0, 1);
	       Context->flags.objects &= 3;
	   }
        }

        if (Context->flags.update) {
           Context->flags.update = 0;
           if (Context->flags.colorlevel >= FEWCOLORS &&
               Context->flags.colorlevel <= MANYCOLORS) drawAll(Context);
        }
}
