void
PopOption(Context)
struct Sundata * Context;
{
	int    w, h, a, b, x=0, y=0;
	
	do_option = 1 - do_option;

        if (!do_option) 
	  {
	  XDestroyWindow(dpy, Option);
	  Option = 0;
	  OptionCaller = NULL;
	  return;
	  }

        if (!Option)
           Option = newWindow(NULL, &OptionGeom, 5);

        XSelectInput(dpy, Option, 0);
        OptionCaller = Context;

	w = ((OptionGeom.width-86) / 
              XTextWidth(OptionCaller->gdata->font[MENUFONT], "_", 1)) - 2;
	resetStringLength(w, &option_entry);

	if (runlevel>=RUNTIMEOPTION) 
           option_newhint = '\n';
	else
	   option_newhint = ' ';

	OptionGeom.height = OptionGeom.h_mini
                          = (15 * Context->gdata->menustrip)/4;

	if (!getPlacement(Context->win, &Context->geom.x, &Context->geom.y, &w, &h)) {
	   x = Context->geom.x + OptionGeom.x - horiz_drift - 5;
	   a = Context->geom.y + h + 6;
           if (do_menu && Context == MenuCaller) 
               a += MenuGeom.height + MenuGeom.y + vert_drift + 2;
           b = Context->geom.y - OptionGeom.height - OptionGeom.y - 2*vert_drift - 28;
           if (b < TOPTITLEBARHEIGHT ) b = TOPTITLEBARHEIGHT;
           if (a > (int) DisplayHeight(dpy,scr) 
                   - 2*OptionGeom.height -vert_drift -20)
              a = b;
	   y = (placement<=NE)? a : b;              
	}
        setSizeHints(NULL, 5);
        XMoveWindow(dpy, Option, x, y);
        XMapRaised(dpy, Option);
        XMoveWindow(dpy, Option, x, y);
        XFlush(dpy);
	usleep(2*TIMESTEP);
	option_lasthint = '\0';
	option_newhint = ' ';
	setupOption(-1);
        setProtocols(NULL, 5);
}
