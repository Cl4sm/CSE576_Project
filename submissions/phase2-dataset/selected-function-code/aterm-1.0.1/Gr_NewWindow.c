Gr_NewWindow(int nargs, int args[])
{
#ifdef RXVT_GRAPHICS
    int             x, y;
    unsigned int    w, h;
    Window          win;
    grwin_t        *grwin;
    Cursor          cursor;

    if (nargs != 4) {
	print_error("NewWindow: 4 args needed, got %d\n", nargs);
	return;
    }
    x = args[0] * TermWin.width / GRX_SCALE + TermWin_internalBorder;
    y = args[1] * TermWin.height / GRX_SCALE + TermWin_internalBorder;
    w = args[2] * TermWin.width / GRX_SCALE;
    h = args[3] * TermWin.height / GRX_SCALE;
    win = XCreateSimpleWindow(Xdisplay, TermWin.vt,
			      x, y, w, h,
			      0,
			      PixColors[Color_fg],
			      PixColors[Color_bg]);

    cursor = XCreateFontCursor(Xdisplay, XC_crosshair);
    XDefineCursor(Xdisplay, win, cursor);
    XMapWindow(Xdisplay, win);
    XSelectInput(Xdisplay, win, ExposureMask);

    grwin = (grwin_t *) MALLOC(sizeof(grwin_t));
    grwin->win = win;
    grwin->x = x;
    grwin->y = y;
    grwin->w = w;
    grwin->h = h;
    grwin->screen = 0;
    grwin->prev = NULL;
    grwin->next = gr_root;
    if (grwin->next)
	grwin->next->prev = grwin;
    gr_root = grwin;
    grwin->graphics = NULL;
    graphics_up++;

    tt_printf("\033W%ld\n", (long)grwin->win);
#endif
}
