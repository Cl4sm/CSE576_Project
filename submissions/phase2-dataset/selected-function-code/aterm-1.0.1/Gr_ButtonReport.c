Gr_ButtonReport(int but, int x, int y)
{
#ifdef RXVT_GRAPHICS
    grwin_t        *grwin;

    for (grwin = gr_root; grwin != NULL; grwin = grwin->next)
	if ((x > grwin->x) &&
	    (y > grwin->y) &&
	    (x < grwin->x + grwin->w) &&
	    (y < grwin->y + grwin->h))
	    break;

    if (grwin == NULL)
	return;

    x = GRX_SCALE * (x - grwin->x) / grwin->w;
    y = GRX_SCALE * (y - grwin->y) / grwin->h;
    tt_printf("\033%c%ld;%d;%d;\n", but, (long)grwin->win, x, y);
#endif
}
