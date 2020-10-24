void
drawbox_menuitem(int y, int state)
{
#ifdef MENUBAR
    GC              top, bot;

#ifdef MENU_SHADOW_IN
    state = -state;
#endif
    switch (state) {
    case +1:
	top = topShadowGC;
	bot = botShadowGC;
	break;			/* SHADOW_OUT */
    case -1:
	top = botShadowGC;
	bot = topShadowGC;
	break;			/* SHADOW_IN */
    default:
	top = bot = neutralGC;
	break;			/* neutral */
    }

    Draw_Shadow(ActiveMenu->win, top, bot,
		SHADOW + 0,
		SHADOW + y,
		ActiveMenu->w - 2 * (SHADOW),
		menu_height() + 2 * MENU_MARGIN);
    XFlush(Xdisplay);
#endif
}
