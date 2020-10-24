newWindow(Context, Geom, num)
struct Sundata * Context;
struct Geometry * Geom;
int num;
{
        Window 		        win = 0;
	int                     strip;

	if (num<=1) {
 	   strip = (num)? Context->gdata->mapstrip :
                          Context->gdata->clockstrip;
	   Context->hstrip = strip;
	   if (Geom->mask & XNegative)
	      Geom->x = DisplayWidth(dpy, scr) - Geom->width + Geom->x;
	   if (Geom->mask & YNegative)
	      Geom->y = DisplayHeight(dpy, scr) - Geom->height + Geom->y-strip;
	} else
	   strip = 0;

	win = XCreateSimpleWindow(dpy, Root,
                      Geom->x, Geom->y, 
                      Geom->width, Geom->height + strip, 0,
		      black, white);
        if (win) setClassHints(win, num);
	return win;
}
