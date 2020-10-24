PopZoom(Context)
struct Sundata * Context;
{
        int a, b, w, h, x=0, y=0;

	if (do_zoom) {
	    XDestroyWindow(dpy, Zoom);
	    Zoom = 0;
	    ZoomCaller = NULL;
	    zoom_active = 1;
            do_zoom = 0;
	    if (zoompix) {
	       XFreePixmap(dpy, zoompix);
	       zoompix = 0;
	    }
	    return;
	} else
	    do_zoom = 1;

        if (!Zoom)
           Zoom = newWindow(NULL, &ZoomGeom, 4);

        Context->newzoom = Context->zoom;
        XSelectInput(dpy, Zoom, 0);

	ZoomCaller = Context;
	zoom_shift = 0;
	zoom_mode = 0;
	zoom_active = do_zoomsync;
	zoom_newhint = ' ';

	if (!getPlacement(Context->win, &Context->geom.x, &Context->geom.y, &w, &h)) {
	   x = Context->geom.x + ZoomGeom.x - horiz_drift - 5;
	   a = Context->geom.y + h + 6;
           if (do_menu && Context == MenuCaller) 
               a += MenuGeom.height + MenuGeom.y + vert_drift + 2;
           b = Context->geom.y - ZoomGeom.height - ZoomGeom.y - 2*vert_drift - 28;
           if (b < TOPTITLEBARHEIGHT ) b = TOPTITLEBARHEIGHT;
           if (a > (int) DisplayHeight(dpy,scr) 
                   - ZoomGeom.height - vert_drift -20)
              a = b;
	   y = (placement<=NE)? a : b;              
	}

        setSizeHints(NULL, 4);
        XMoveWindow(dpy, Zoom, x, y);
        XMapRaised(dpy, Zoom);
        XMoveWindow(dpy, Zoom, x, y);
        XFlush(dpy);
	usleep(2*TIMESTEP);
	option_lasthint = '\0';
        setupZoom(-1);
        setProtocols(NULL, 4);
}
