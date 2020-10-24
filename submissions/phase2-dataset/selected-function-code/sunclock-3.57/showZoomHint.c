void
showZoomHint(but_pos)
int but_pos;
{
        static int move_pos = -1;
	char hint[120];
	int v;

        if (move_pos>=0) drawButton(Zoom, move_pos, -2);
        if (but_pos>=0) drawButton(Zoom, but_pos, -1);
	move_pos = but_pos;

	if (!do_zoom || zoom_lasthint==zoom_newhint) return;

	zoom_lasthint = zoom_newhint;
	v = ZoomGeom.height - ZoomCaller->gdata->menustrip;

	if (zoom_newhint=='\033')
           strcpy(hint, Label[L_ESCMENU]);
	else
	if (zoom_newhint==' ')
           sprintf(hint, 
                "magx = %.3f, magy = %.3f,  lon = %.3f°, lat = %.3f°", 
                ZoomCaller->newzoom.fx, ZoomCaller->newzoom.fy,
	 	360.0 * ZoomCaller->newzoom.fdx - 180.0, 
                90.0 - ZoomCaller->newzoom.fdy*180.0);
        else
	   strcpy(hint, Help[getNumCmd(zoom_newhint)]);

	BasicSettings(ZoomCaller);
        XSetWindowBackground(dpy, Zoom, ZoomCaller->gdata->pixel[MENUBGCOLOR]);
	XClearArea(dpy, Zoom, 0, v+1, ZoomGeom.width, 
             ZoomCaller->gdata->menustrip-1, False);
	XDrawImageString(dpy, Zoom, ZoomCaller->gdata->wingc, 4, 
              v + ZoomCaller->gdata->font[MENUFONT]->max_bounds.ascent + 3,
              hint, strlen(hint));
}
