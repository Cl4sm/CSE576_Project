void
showUrbanHint(str)
char * str;
{
	char hint[128];
	int l, v;

	if (!do_urban || urban_lasthint==urban_newhint) return;

	*hint = '\0';

	urban_lasthint = urban_newhint;
	v = UrbanGeom.height - UrbanCaller->gdata->menustrip;

	if (urban_newhint=='\033')
           strcpy(hint, Label[L_ESCMENU]);
	else
	if (urban_newhint==' ')
           strcpy(hint, Help[getNumCmd('U')]);
	else {
	   if (str) 
              strncpy(hint, str, 125);
	   else {
	      l = getNumCmd(urban_newhint);
	      if (l>=0 && l<N_HELP) {
		 strcpy(hint, Help[l]);
	      }
	   }
	}

	l = strlen(hint);

        BasicSettings(UrbanCaller);
        XSetWindowBackground(dpy, Urban, UrbanCaller->gdata->pixel[MENUBGCOLOR]);

	XClearArea(dpy, Urban, 0, v+1, UrbanGeom.width, 
             UrbanCaller->gdata->menustrip-1, False);
        XDrawImageString(dpy, Urban, UrbanCaller->gdata->wingc, 
              4, v + UrbanCaller->gdata->font[MENUFONT]->max_bounds.ascent + 3,
              hint, l);
}
