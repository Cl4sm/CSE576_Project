menu_select(XButtonEvent * ev)
{
#ifdef MENUBAR
    menuitem_t     *thisitem, *item = NULL;
    int             this_y, y;

    Window          unused_root, unused_child;
    int             unused_root_x, unused_root_y;
    unsigned int    unused_mask;

    if (ActiveMenu == NULL)
	return 0;

    XQueryPointer(Xdisplay, ActiveMenu->win,
		  &unused_root, &unused_child,
		  &unused_root_x, &unused_root_y,
		  &(ev->x), &(ev->y),
		  &unused_mask);

    if (ActiveMenu->parent != NULL && (ev->x < 0 || ev->y < 0)) {
	menu_hide();
	return 1;
    }
/* determine the menu item corresponding to the Y index */
    y = 0;
    if (ev->x >= 0 &&
	ev->x <= (ActiveMenu->w - SHADOW)) {
	for (item = ActiveMenu->head; item != NULL; item = item->next) {
	    int             h = menu_height();

	    if (isSeparator(item->name)) {
		h = SEPARATOR_HEIGHT;
	    } else if (ev->y >= y && ev->y < (y + h)) {
		break;
	    }
	    y += h;
	}
    }
    if (item == NULL && ev->type == ButtonRelease) {
	menu_hide_all();
	return 0;
    }
    thisitem = item;
    this_y = y;

/* erase the last item */
    if (ActiveMenu->item != NULL) {
	if (ActiveMenu->item != thisitem) {
	    for (y = 0, item = ActiveMenu->head;
		 item != NULL;
		 item = item->next) {
		int             h = menu_height();

		if (isSeparator(item->name)) {
		    h = SEPARATOR_HEIGHT;
		} else if (item == ActiveMenu->item) {
		/* erase old menuitem */
		    drawbox_menuitem(y, 0);	/* No Shadow */
		    if (item->entry.type == MenuSubMenu)
			drawtriangle(ActiveMenu->w, y, +1);
		    break;
		}
		y += h;
	    }
	} else {
	    switch (ev->type) {
	    case ButtonRelease:
		switch (item->entry.type) {
		case MenuLabel:
		case MenuSubMenu:
		    menu_hide_all();
		    break;

		case MenuAction:
		case MenuTerminalAction:
		    drawbox_menuitem(this_y, -1);
		/*
		 * use select for timing
		 * remove menu before sending keys to the application
		 */
		    {
#ifdef __CYGWIN32__
			struct timeval tv;

			tv.tv_sec = 0;
			tv.tv_usec = MENU_DELAY_USEC;
			select(0, NULL, NULL, NULL, &tv);
#else
			struct itimerval tv;

			tv.it_value.tv_sec = 0;
			tv.it_value.tv_usec = MENU_DELAY_USEC;
			select(0, NULL, NULL, NULL, &tv.it_value);
#endif
		    }
		    menu_hide_all();
#ifndef DEBUG_MENU
		    action_dispatch(&(item->entry.action));
#else				/* DEBUG_MENU */
		    printf("%s: %s\n", item->name, item->entry.action.str);
#endif				/* DEBUG_MENU */
		    break;
		}
		break;

	    default:
		if (item->entry.type == MenuSubMenu)
		    goto DoMenu;
		break;
	    }
	    return 0;
	}
    }
  DoMenu:
    ActiveMenu->item = thisitem;
    y = this_y;
    if (thisitem != NULL) {
	item = ActiveMenu->item;
	if (item->entry.type != MenuLabel)
	    drawbox_menuitem(y, +1);
	if (item->entry.type == MenuSubMenu) {
	    int             x;

	    drawtriangle(ActiveMenu->w, y, -1);

	    x = ev->x + (ActiveMenu->parent ?
			 ActiveMenu->x :
			 Width2Pixel(ActiveMenu->x));

	    if (x >= item->entry.submenu.menu->x) {
		ActiveMenu = item->entry.submenu.menu;
		menu_show();
		return 1;
	    }
	}
    }
#endif
    return 0;
}
