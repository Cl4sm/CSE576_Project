menubar_select(XButtonEvent * ev)
{
#ifdef MENUBAR
    menu_t         *menu = NULL;

/* determine the pulldown menu corresponding to the X index */
    if (ev->y >= 0 && ev->y <= menuBar_height() && CurrentBar != NULL) {
	for (menu = CurrentBar->head; menu != NULL; menu = menu->next) {
	    int             x = Width2Pixel(menu->x);
	    int             w = Width2Pixel(menu->len + HSPACE);

	    if ((ev->x >= x && ev->x < x + w))
		break;
	}
    }
    switch (ev->type) {
    case ButtonRelease:
	menu_hide_all();
	break;

    case ButtonPress:
	if (menu == NULL && Arrows_x && ev->x >= Arrows_x) {
	    int             i;

	    for (i = 0; i < NARROWS; i++) {
		if (ev->x >= (Arrows_x + (Width2Pixel(4 * i + i)) / 4) &&
		    ev->x < (Arrows_x + (Width2Pixel(4 * i + i + 4)) / 4)) {
		    draw_Arrows(Arrows[i].name, -1);
		/*
		 * use select for timing
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
		    draw_Arrows(Arrows[i].name, +1);
#ifdef DEBUG_MENUARROWS
		    printf("'%c': ", Arrows[i].name);

		    if (CurrentBar == NULL ||
			(CurrentBar->arrows[i].type != MenuAction &&
			 CurrentBar->arrows[i].type != MenuTerminalAction)) {
			if (Arrows[i].str != NULL && Arrows[i].str[0])
			    printf("(default) \\033%s\n",
				   &(Arrows[i].str[2]));
		    } else {
			printf("%s\n", CurrentBar->arrows[i].str);
		    }
#else				/* DEBUG_MENUARROWS */
		    if (CurrentBar == NULL ||
			action_dispatch(&(CurrentBar->arrows[i]))) {
			if (Arrows[i].str != NULL &&
			    Arrows[i].str[0] != 0)
			    tt_write((Arrows[i].str + 1),
				     Arrows[i].str[0]);
		    }
#endif				/* DEBUG_MENUARROWS */
		    return;
		}
	    }
	}
    /*drop */

    default:
    /*
     * press menubar or move to a new entry
     */
	if (menu != NULL && menu != ActiveMenu) {
	    menu_hide_all();	/* pop down old menu */
	    ActiveMenu = menu;
	    menu_show();	/* pop up new menu */
	}
	break;
    }
#endif
}
