drawtriangle(int x, int y, int state)
{
#ifdef MENUBAR
    GC              top, bot;
    int             w;

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

    w = menu_height() / 2;

    x -= (SHADOW + MENU_MARGIN) + (3 * w / 2);
    y += (SHADOW + MENU_MARGIN) + (w / 2);

    Draw_Triangle(ActiveMenu->win, top, bot, x, y, w, 'r');
#endif
}
