menubar_expose(void)
{
#ifdef MENUBAR
    menu_t         *menu;
    int             x;

    if (delay_menu_drawing || !menubar_visible())
	return;

    if (menubarGC == None) {
    /* Create the graphics context */
	XGCValues       gcvalue;

	gcvalue.font = TermWin.font->fid;

	gcvalue.foreground = (Xdepth <= 2 ?
			      PixColors[Color_fg] :
			      PixColors[Color_Black]);
	menubarGC = XCreateGC(Xdisplay, menuBar.win,
			      GCForeground | GCFont,
			      &gcvalue);

	gcvalue.foreground = PixColors[Color_scroll];
	neutralGC = XCreateGC(Xdisplay, menuBar.win,
			      GCForeground,
			      &gcvalue);

	gcvalue.foreground = PixColors[Color_bottomShadow];
	botShadowGC = XCreateGC(Xdisplay, menuBar.win,
				GCForeground | GCFont,
				&gcvalue);

	gcvalue.foreground = PixColors[Color_topShadow];
	topShadowGC = XCreateGC(Xdisplay, menuBar.win,
				GCForeground,
				&gcvalue);
    }
/* make sure the font is correct */
    XSetFont(Xdisplay, menubarGC, TermWin.font->fid);
    XSetFont(Xdisplay, botShadowGC, TermWin.font->fid);
    XClearWindow(Xdisplay, menuBar.win);

    menu_hide_all();

    x = 0;
    if (CurrentBar != NULL) {
	for (menu = CurrentBar->head; menu != NULL; menu = menu->next) {
	    int             len = menu->len;

	    x = (menu->x + menu->len + HSPACE);

#ifdef DEBUG_MENU_LAYOUT
	    print_menu_descendants(menu);
#endif

	    if (x >= TermWin.ncol)
		len = (TermWin.ncol - (menu->x + HSPACE));

	    drawbox_menubar(menu->x, len, +1);

	    XDrawString(Xdisplay,
			menuBar.win, menubarGC,
			(Width2Pixel(menu->x) + Width2Pixel(HSPACE) / 2 ),
			menuBar_height() - MENU_MARGIN,
			menu->name, len);

	    if (x >= TermWin.ncol)
		break;
	}
    }
    drawbox_menubar(x, TermWin.ncol, (CurrentBar ? +1 : -1));

/* add the menuBar title, if it exists and there's plenty of room */
    Arrows_x = 0;
    if (x < TermWin.ncol) {
	char           *str, title[256];
	int             len, ncol = TermWin.ncol;

	if (x < (ncol - (NARROWS + 1))) {
	    ncol -= (NARROWS + 1);
	    Arrows_x = Width2Pixel(ncol);
	}
	draw_Arrows(0, +1);

	str = (CurrentBar && CurrentBar->title ? CurrentBar->title : "%n-%v");
	for (len = 0; str[0] && len < sizeof(title) - 1; str++) {
	    const char     *s = NULL;

	    switch (str[0]) {
	    case '%':
		str++;
		switch (str[0]) {
		case 'n':
		    s = rs_name;
		    break;	/* resource name */
		case 'v':
		    s = VERSION;
		    break;	/* version number */
		case '%':
		    s = "%";
		    break;	/* literal '%' */
		}
		if (s != NULL)
		    while (*s && len < sizeof(title) - 1)
			title[len++] = *s++;
		break;

	    default:
		title[len++] = str[0];
		break;
	    }
	}
	title[len] = '\0';

	ncol -= (x + len + HSPACE);
	if (len > 0 && ncol >= 0)
	    XDrawString(Xdisplay,
			menuBar.win, menubarGC,
			Width2Pixel(x) + Width2Pixel(ncol + HSPACE) / 2,
			menuBar_height() - MENU_MARGIN,
			title, len);
    }
#endif
}
