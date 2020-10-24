void
getFonts(Context)
Sundata * Context;
{
int i, h, hp;

        for (i=0; i<NUMFONTS; i++)
	    if (!(Context->gdata->font[i] = getFont(i))) exit(1);

        Context->gdata->clockstrip = 
            Context->gdata->font[CLOCKSTRIPFONT]->max_bounds.ascent +
            Context->gdata->font[CLOCKSTRIPFONT]->max_bounds.descent + 4;

        Context->gdata->mapstrip = 
            Context->gdata->font[MAPSTRIPFONT]->max_bounds.ascent + 
            Context->gdata->font[MAPSTRIPFONT]->max_bounds.descent + 8;

        Context->gdata->menustrip = 
            Context->gdata->font[MENUFONT]->max_bounds.ascent + 
            Context->gdata->font[MENUFONT]->max_bounds.descent + 8;

	Context->gdata->charspace = Context->gdata->menustrip+4;

	if (option_changes & 1) {
	    FileselGeom.width = SEL_WIDTH * Context->gdata->menustrip;
            FileselGeom.height = (11+4*SEL_HEIGHT)*Context->gdata->menustrip/5;
	}

	if (Context->flags.colorlevel < FULLCOLORS) return;

        h = Context->gdata->font[CITYFONT]->max_bounds.ascent + 
            Context->gdata->font[CITYFONT]->max_bounds.descent;
        hp = Context->gdata->font[COORDFONT]->max_bounds.ascent + 
             Context->gdata->font[COORDFONT]->max_bounds.descent;
        if (hp>h) h = hp;
	if (h>textheight) {
           textheight = h;
	   if (textpix) {
	      XFreePixmap(dpy, textpix);
	      textpix = 0;
	   }
	}
        if (!textpix)
           textpix = XCreatePixmap(dpy, Root, textwidth, textheight, 1);
}
