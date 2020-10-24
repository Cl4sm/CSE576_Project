void
drawCities(Context)
struct Sundata * Context;
{
City *c;
        if (!Context->wintype || !Context->flags.citymode) return; 

	if (Context->flags.colorlevel==MONOCHROME ||
            Context->flags.colorlevel==FULLCOLORS)
           XSetFont(dpy, Context->gdata->pixgc, 
                         Context->gdata->font[CITYFONT]->fid);
        else
           XSetFont(dpy, Context->gdata->wingc, 
                         Context->gdata->font[CITYFONT]->fid);

        if (Context->lastmarked && Context->mark1.city != Context->lastmarked
            && Context->flags.colorlevel>MONOCHROME) {
	   erase_obj = 1;
	   c = Context->lastmarked;
           drawObject(Context, c->lon, c->lat, c->size, 1, c->name);
	   erase_obj = 0;
	}

        for (c = cityroot; c; c = c->next) {
	   if (c!=Context->mark1.city && c!=Context->mark2.city)
              drawObject(Context, c->lon, c->lat, c->size, 
                         -(c==cityinit), c->name);
	}

       	c = Context->mark2.city;
	if (c && c!=&Context->pos2)
           drawObject(Context, c->lon, c->lat, c->size, 2, c->name);

       	c = Context->mark1.city;
	if (c && c!=&Context->pos1) {
           drawObject(Context, c->lon, c->lat, c->size, 1, c->name);
	   Context->lastmarked = c;
	}
}
