void
menu_show(void)
{
#ifdef MENUBAR
    int             x, y, xright;
    menuitem_t     *item;

    if (ActiveMenu == NULL)
	return;

    x = ActiveMenu->x;
    if (ActiveMenu->parent == NULL) {
	register int    h;

	drawbox_menubar(x, ActiveMenu->len, -1);
	x = Width2Pixel(x);

	ActiveMenu->y = 1;
	ActiveMenu->w = Menu_PixelWidth(ActiveMenu);

	if ((x + ActiveMenu->w) >= TermWin.width)
	    x = (TermWin_TotalWidth() - ActiveMenu->w);

    /* find the height */
	for (h = 0, item = ActiveMenu->head; item != NULL; item = item->next) {
	    if (isSeparator(item->name))
		h += SEPARATOR_HEIGHT;
	    else
		h += menu_height();
	}
	ActiveMenu->h = h + 2 * (SHADOW + MENU_MARGIN);
    }
    if (ActiveMenu->win == None) {
	ActiveMenu->win = XCreateSimpleWindow(Xdisplay, TermWin.vt,
					      x,
					      ActiveMenu->y,
					      ActiveMenu->w,
					      ActiveMenu->h,
					      0,
					      PixColors[Color_fg],
					      PixColors[Color_scroll]);
	XMapWindow(Xdisplay, ActiveMenu->win);
    }
    Draw_Shadow(ActiveMenu->win,
		topShadowGC, botShadowGC,
		0, 0,
		ActiveMenu->w, ActiveMenu->h);

/* determine the correct right-alignment */
    for (xright = 0, item = ActiveMenu->head; item != NULL; item = item->next)
	if (item->len2 > xright)
	    xright = item->len2;

    for (y = 0, item = ActiveMenu->head;
	 item != NULL;
	 item = item->next) {
	const int       xoff = (SHADOW + Width2Pixel(HSPACE) / 2);
	const int       yoff = (SHADOW + MENU_MARGIN);
	register int    h;
	GC              gc = menubarGC;

	if (isSeparator(item->name)) {
	    Draw_Shadow(ActiveMenu->win,
			topShadowGC, botShadowGC,
			xoff,
			yoff + y + SEPARATOR_HALFHEIGHT,
			ActiveMenu->w - (2 * xoff),
			0);
	    h = SEPARATOR_HEIGHT;
	} else {
	    char           *name = item->name;
	    int             len = item->len;

	    if (item->entry.type == MenuLabel) {
		gc = botShadowGC;
	    } else if (item->entry.type == MenuSubMenu) {
		int             x1, y1;
		menuitem_t     *it;
		menu_t         *menu = item->entry.submenu.menu;

		drawtriangle(ActiveMenu->w, y, +1);

		name = menu->name;
		len = menu->len;

		y1 = ActiveMenu->y + y;

	    /* place sub-menu at midpoint of parent menu */
		menu->w = Menu_PixelWidth(menu);
		x1 = ActiveMenu->w / 2;

	    /* right-flush menu if it's too small */
		if (x1 > menu->w)
		    x1 += (x1 - menu->w);
		x1 += x;

	    /* find the height of this submenu */
		for (h = 0, it = menu->head; it != NULL; it = it->next) {
		    if (isSeparator(it->name))
			h += SEPARATOR_HEIGHT;
		    else
			h += menu_height();
		}
		menu->h = h + 2 * (SHADOW + MENU_MARGIN);

	    /* ensure menu is in window limits */
		if ((x1 + menu->w) >= TermWin.width)
		    x1 = (TermWin_TotalWidth() - menu->w);

		if ((y1 + menu->h) >= TermWin.height)
		    y1 = (TermWin_TotalHeight() - menu->h);

		menu->x = (x1 < 0 ? 0 : x1);
		menu->y = (y1 < 0 ? 0 : y1);
	    } else if (item->name2 && !strcmp(name, item->name2))
		name = NULL;

	    if (len && name)
		XDrawString(Xdisplay,
			    ActiveMenu->win, gc,
			    xoff,
			    yoff + y + menu_height() - (2 * MENU_MARGIN),
			    name, len);

	    len = item->len2;
	    name = item->name2;
	    if (len && name)
		XDrawString(Xdisplay,
			    ActiveMenu->win, gc,
			    ActiveMenu->w - (xoff + Width2Pixel(xright)),
			    yoff + y + menu_height() - (2 * MENU_MARGIN),
			    name, len);

	    h = menu_height();
	}
	y += h;
    }
#endif
}
