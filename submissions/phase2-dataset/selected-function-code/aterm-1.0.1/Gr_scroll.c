void
Gr_scroll(int count)
{
#ifdef RXVT_GRAPHICS
    static short    prev_start = 0;
    grwin_t        *grwin, *next;

    if ((count == 0) && (prev_start == TermWin.view_start))
	return;

    prev_start = TermWin.view_start;

    for (grwin = gr_root; grwin != NULL; grwin = next) {
	next = grwin->next;
	grwin->y -= (count * TermWin.fheight);
	if ((grwin->y + grwin->h) < -(TermWin.saveLines * TermWin.fheight))
	    Gr_DestroyWindow(grwin);
	else
	    XMoveWindow(Xdisplay, grwin->win,
			grwin->x,
			grwin->y + (TermWin.view_start * TermWin.fheight));
    }
#endif
}
