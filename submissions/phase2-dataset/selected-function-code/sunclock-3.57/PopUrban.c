void
PopUrban(Context)
struct Sundata * Context;
{
	int    i, w, h, a, b, x=0, y=0;
	
	do_urban = 1 - do_urban;

        if (!do_urban) {
	  XDestroyWindow(dpy, Urban);
	  Urban = 0;
	  UrbanCaller = NULL;
	  return;
	}

        if (!Urban)
           Urban = newWindow(NULL, &UrbanGeom, 6);

        XSelectInput(dpy, Urban, 0);
        UrbanCaller = Context;

	UrbanGeom.height = UrbanGeom.h_mini
                          = (22 * Context->gdata->menustrip)/4;
	setupUrban(-2);
        for (i=0; i<=4; i++) {
	   w = (urban_w[i]/ 
              XTextWidth(UrbanCaller->gdata->font[MENUFONT], "_", 1)) - 2;
	   resetStringLength(w, &urban_entry[i]);
	}

	if (!getPlacement(Context->win, &Context->geom.x, &Context->geom.y, &w, &h)) {
	   x = Context->geom.x + UrbanGeom.x - horiz_drift - 5;
	   a = Context->geom.y + h + 6;
           if (do_menu && Context == MenuCaller) 
               a += MenuGeom.height + MenuGeom.y + vert_drift + 2;
           b = Context->geom.y - UrbanGeom.height - UrbanGeom.y - 2*vert_drift -28;
           if (b < TOPTITLEBARHEIGHT ) b = TOPTITLEBARHEIGHT;
           if (a > (int) DisplayHeight(dpy,scr) 
                   - 2*UrbanGeom.height -vert_drift -20)
              a = b;
	   y = (placement<=NE)? a : b;              
	}
        setSizeHints(NULL, 6);
        XMoveWindow(dpy, Urban, x, y);
        XResizeWindow(dpy, Urban, UrbanGeom.width, UrbanGeom.height);
        XMapRaised(dpy, Urban);
        XMoveWindow(dpy, Urban, x, y);
        XFlush(dpy);
	usleep(2*TIMESTEP);
	setupUrban(-1);
        setProtocols(NULL, 6);
}
