void
Gr_ChangeScreen(void)
{
#ifdef RXVT_GRAPHICS
    grwin_t        *grwin, *next;

    for (grwin = gr_root; grwin != NULL; grwin = next) {
	next = grwin->next;
	if (grwin->y + grwin->h > 0) {
	    if (grwin->screen == 1) {
		XMapWindow(Xdisplay, grwin->win);
		grwin->screen = 0;
	    } else {
		XUnmapWindow(Xdisplay, grwin->win);
		grwin->screen = 1;
	    }
	}
    }
#endif
}
