void
drawbox_menubar(int x, int len, int state)
{
#ifdef MENUBAR
    GC              top, bot;

    x = Width2Pixel(x);
    len = Width2Pixel(len + HSPACE);
    if (x >= TermWin.width)
	return;
    else if (x + len >= TermWin.width)
	len = (TermWin_TotalWidth() - x);

#ifdef MENUBAR_SHADOW_IN
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

    Draw_Shadow(menuBar.win, top, bot,
		x, 0, len, menuBar_TotalHeight());
#endif
}
