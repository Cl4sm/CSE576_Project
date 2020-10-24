void
createGCs(Context)
struct Sundata * Context;
{
        XGCValues               gcv;
	int                     mask;

        if (!Context->mappix && (Context->flags.colorlevel<FULLCOLORS))
           Context->mappix = XCreatePixmap(dpy, Root, 
              Context->geom.width, Context->geom.height, 1);

        if (Context->gdata->links==0) {
           if (verbose)
              fprintf(stderr, "Creating new GCs, mode = %d\n", 
                     Context->flags.colorlevel);
           getFonts(Context);
        } else {
           if (verbose)
              fprintf(stderr, "Using previous GC settings (%d links)\n", 
                     Context->gdata->links);
	   return;
        }

        mask = GCForeground | GCBackground | GCFont;
	gcv.background = white;
	gcv.foreground = black;

        if (!Context->gdata->wingc) {
           gcv.font = Context->gdata->font[MENUFONT]->fid;
           Context->gdata->wingc = XCreateGC(dpy, Root, mask, &gcv);
	}

        gcv.font = Context->gdata->font[COORDFONT]->fid;
        if (Context->flags.colorlevel == FULLCOLORS) {
           gcv.background = black;
	   gcv.foreground = white;
           Context->gdata->pixgc = XCreateGC(dpy, textpix, mask, &gcv);
        } else {
           mask |= GCFunction;
	   gcv.function = GXinvert;
           Context->gdata->pixgc = XCreateGC(dpy, Context->mappix, mask, &gcv);
	}
}
