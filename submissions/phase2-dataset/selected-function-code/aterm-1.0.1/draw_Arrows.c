draw_Arrows(int name, int state)
{
#ifdef MENUBAR
    GC              top, bot;

    int             i;

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

    if (!Arrows_x)
	return;

    for (i = 0; i < NARROWS; i++) {
	const int       w = Width2Pixel(1);
	const int       y = (menuBar_TotalHeight() - w) / 2;
	int             x = Arrows_x + (5 * Width2Pixel(i)) / 4;

	if (!name || name == Arrows[i].name)
	    Draw_Triangle(menuBar.win, top, bot, x, y, w,
			  Arrows[i].name);
    }
    XFlush(Xdisplay);
#endif
}
