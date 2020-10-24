void
PopFilesel(Context)
struct Sundata * Context;
{
        int a, b, w, h, x=0, y=0;

	if (do_filesel)
            do_filesel = 0;
	else
	    do_filesel = 1;

        if (!do_filesel) 
	  {
	  XDestroyWindow(dpy, Filesel);
	  Filesel = 0;
	  FileselCaller = NULL;
	  if (dirtable) free_dirlist(dirtable);
	  dirtable = NULL;
	  return;
	  }

        if (!Filesel)
           Filesel = newWindow(NULL, &FileselGeom, 3);

        XSelectInput(dpy, Filesel, 0);
	FileselCaller = Context;
	filesel_shift = 0;

	if (!getPlacement(Context->win, &Context->geom.x, 
                                        &Context->geom.y, &w, &h)) {
	   x = Context->geom.x + FileselGeom.x - horiz_drift - 5;
	   a = Context->geom.y + h + 6;
           if (do_menu && Context == MenuCaller) 
               a += MenuGeom.height + MenuGeom.y + vert_drift + 2;
           b = Context->geom.y - FileselGeom.height - FileselGeom.y - 2*vert_drift - 28;
           if (b < TOPTITLEBARHEIGHT ) b = TOPTITLEBARHEIGHT;
           if (a > (int) DisplayHeight(dpy,scr) 
                   - FileselGeom.height - vert_drift - 20)
               a = b;
	   y = (placement<=NE)? a : b;
	}

        setSizeHints(NULL, 3);
        XMoveWindow(dpy, Filesel, x, y);
        XMapRaised(dpy, Filesel);
        XMoveWindow(dpy, Filesel, x, y);
	XFlush(dpy);
	usleep(2*TIMESTEP);
	setupFilesel(-1);
        setProtocols(NULL, 3);
}
