void
setSizeHints(Context, num)
struct Sundata *                Context;
int				num;
{
	XSizeHints		xsh;
	struct Geometry *       Geom = NULL;
	Window			win = 0;

        if (num<=1) {
	    if (!Context) return;
	    win = Context->win;
	    Geom = &Context->geom;
	    xsh.flags = PSize | PMinSize;
	    if (Geom->mask & (XValue | YValue)) {
		xsh.x = Geom->x;
		xsh.y = Geom->y;
                xsh.flags |= USPosition; 
	    }
	    xsh.width = Geom->width;
	    xsh.height = Geom->height + Context->hstrip;
	    if (do_dock && Context==Seed) {
              xsh.max_width = xsh.min_width = xsh.width;
              xsh.max_height = xsh.min_height = xsh.height;
	      xsh.flags |= PMaxSize | PMinSize;
	    } else {
              xsh.min_width = Geom->w_mini;
              xsh.min_height = Geom->h_mini + Context->hstrip;
	    }
	} else 
	if (num>=2) {
	    xsh.flags = PSize | USPosition | PMinSize;
	    if (num==2) {
	      win = Menu;
	      Geom = &MenuGeom;
	      xsh.flags |= PMaxSize;
	      xsh.min_width = xsh.max_width = Geom->width;
              xsh.min_height = xsh.max_height = Geom->height;
	    }
	    if (num==3) {
	      win = Filesel;
	      Geom = &FileselGeom;
	      xsh.min_width = Geom->w_mini;
              xsh.min_height = Geom->h_mini;
	    }
	    if (num==4) {
	      win = Zoom;
	      Geom = &ZoomGeom;
	      xsh.min_width = Geom->w_mini;
              xsh.min_height = Geom->h_mini;
	    }
	    if (num==5) {
	      win = Option;
	      Geom = &OptionGeom;
	      xsh.flags |= PMaxSize;
	      xsh.min_width = Geom->w_mini;
              xsh.max_width = 2000;
              xsh.min_height = xsh.max_height = Geom->h_mini;
	    }
	    if (num==6) {
	      win = Urban;
	      Geom = &UrbanGeom;
 	      xsh.flags |= PMaxSize;
              xsh.max_width = 2000;
              xsh.min_width = Geom->w_mini;
              xsh.min_height = xsh.max_height = Geom->h_mini;
	    }
	    xsh.x = Geom->x;
	    xsh.y = Geom->y;
	    xsh.width = Geom->width;
	    xsh.height = Geom->height;
	}

	if (!win) return;
	XSetNormalHints(dpy, win, &xsh);
}
