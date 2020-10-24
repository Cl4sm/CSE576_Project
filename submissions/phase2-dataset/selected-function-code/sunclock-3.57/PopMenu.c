void
PopMenu(Context)
struct Sundata * Context;
{
	int    w, h, a, b, x=0, y=0;
	
	do_menu = 1 - do_menu;

        if (!do_menu) 
	  {
	  XDestroyWindow(dpy, Menu);
	  Menu = 0;
	  MenuCaller = NULL;
	  return;
	  }

        if (!Menu)
           Menu = newWindow(NULL, &MenuGeom, 2);

	XSelectInput(dpy, Menu, 0);
        MenuCaller = Context;

        MenuGeom.width = MENU_WIDTH * Context->gdata->menustrip - 6;
        MenuGeom.height = 2 * Context->gdata->menustrip;
   
	if (!getPlacement(Context->win, &Context->geom.x, 
                                        &Context->geom.y, &w, &h)) {
	   x = Context->geom.x + MenuGeom.x - horiz_drift - 5;
	   a = Context->geom.y + h + 6; 

           b = Context->geom.y - MenuGeom.height - MenuGeom.y - 2*vert_drift - 28;
           if (b < TOPTITLEBARHEIGHT ) b = TOPTITLEBARHEIGHT;
           if (a > (int) DisplayHeight(dpy,scr) 
                   - 2*MenuGeom.height -vert_drift -20)
              a = b;
	   y = (placement<=NE)? a : b;
	}
        setSizeHints(NULL, 2);
        XMoveWindow(dpy, Menu, x, y);
        XMapWindow(dpy, Menu);
        XMoveWindow(dpy, Menu, x, y);
        XFlush(dpy);
	usleep(2*TIMESTEP);
	menu_lasthint = ' ';
	setupMenu(-1);
        setProtocols(NULL, 2);
}
