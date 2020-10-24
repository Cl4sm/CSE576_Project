Gr_Geometry(grwin_t * grwin, grcmd_t * data)
{
#ifdef RXVT_GRAPHICS
    if (grwin)
	tt_printf("\033G%ld %d %d %u %u %d %d %ld %ld %d\n",
		  (long)grwin->win,
		  grwin->x, grwin->y, grwin->w, grwin->h,
		  TermWin.fwidth,
		  TermWin.fheight,
		  (long)GRX_SCALE * TermWin.fwidth / grwin->w,
		  (long)GRX_SCALE * TermWin.fheight / grwin->h,
		  Xdepth);
    else			/* rxvt terminal window size */
	tt_printf("\033G0 0 0 %d %d %d %d %ld %ld %d\n",
		  TermWin.width - 2 * TermWin_internalBorder,
		  TermWin.height - 2 * TermWin_internalBorder,
		  TermWin.fwidth,
		  TermWin.fheight,
		  (long)GRX_SCALE * TermWin.fwidth
		        / (TermWin.width - 2 * TermWin_internalBorder),
		  (long)GRX_SCALE * TermWin.fheight
		        / (TermWin.height - 2 * TermWin_internalBorder),
		  Xdepth);
#endif
}
