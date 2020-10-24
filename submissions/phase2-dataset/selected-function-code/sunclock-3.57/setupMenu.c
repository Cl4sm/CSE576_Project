void
setupMenu(mode)
int mode;
{
	int i, l;
        static int j=-1;
        static char c;

	if (!do_menu) return;

        if (j>=0) WeakChars[j] = c;
	l = strlen(WeakChars);

        if (MenuCaller->wintype) {
	   if (do_dock) j = 1; else j = 0;
	} else {
	   j = -1;
	   if (do_dock) {
 	      if (MenuCaller != Seed) j = l-4;
	   } else j = l-5;
	}
        if (j>=0) {
	   c = WeakChars[j] ;
           WeakChars[j] = '\0';
	}

        BasicSettings(MenuCaller);
        XSetWindowColormap(dpy, Menu, MenuCaller->gdata->cmap);
        XSetWindowBackground(dpy, Menu, MenuCaller->gdata->pixel[MENUBGCOLOR]);
        XClearArea(dpy, Menu,  0, 0, MenuGeom.width, MenuGeom.height, False);

	for (i=0; i<=N_MENU; i++)
            drawButton(Menu, i, 0);
        menu_lasthint = '\0';
	showMenuHint(-1);
}
