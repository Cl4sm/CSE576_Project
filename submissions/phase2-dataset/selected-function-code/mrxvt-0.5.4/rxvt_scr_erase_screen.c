void
rxvt_scr_erase_screen(rxvt_t* r, int page, int mode)
{
    int		num;
    int32_t	row, row_offset;
    rend_t	ren;
    XGCValues	gcvalue;

    PVTS(r, page)->want_refresh = 1;
    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_scr_erase_screen %d (%d) at screen row: %d\n", page, mode, CURROW));
    ZERO_SCROLLBACK(r, page);
    RESET_CHSTAT(r, page);
    row_offset = (int32_t)SVLINES;

    switch (mode)
    {
	case 0:		/* erase to end of screen */
	    rxvt_selection_check(r, page, 1);
	    rxvt_scr_erase_line(r, page, 0);
	    row = CURROW + 1;	/* possible OOB */
	    num = r->TermWin.nrow - row;
	    break;
	case 1:		/* erase to beginning of screen */
	    rxvt_selection_check(r, page, 3);
	    rxvt_scr_erase_line(r, page, 1);
	    row = 0;
	    num = CURROW;
	    break;
	case 2:		/* erase whole screen */
	    /*
	     * 2006-02-15 gi1242: As pointed out by Sabit Sayeed, Gnome terminal
	     * scrolls the text off screen, instead of wiping it out completely.
	     * That's seems much better so let's do it here.
	     */
	    if( PVTS(r, page)->current_screen == PRIMARY )
	    {
		/*
		 * Only scroll if the primary screen is bieng cleared.
		 */
		int sr;

		/*
		 * Find the last non-empty line to save.
		 */
		for( sr = SVLINES + r->TermWin.nrow - 1; sr >= SVLINES; sr--)
		{
		    int non_empty = 0, sc;

		    for( sc = 0; sc < PSCR( r, page).tlen[sr]; sc++)
			if (
			      PSCR( r, page).text[sr][sc] != '\0'
			      && PSCR( r, page).text[sr][sc] != ' '
			   )
			{
			    non_empty = 1;
			    break;
			}
		    ;
		    if( non_empty ) break;
		}

		sr -=  SVLINES /* - 1 */;   /* Dump last non-empty line */
		rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "Saving %d lines\n", sr));

		if( sr > 0)
		    rxvt_scroll_text(r, page,
			    PSCR(r, page).tscroll, PSCR(r, page).bscroll,
			    sr, 0);
	    }
	    else rxvt_selection_check(r, page, 3);

	    row = 0;
	    num = r->TermWin.nrow;
	    break;
	default:
	    return;
    }

    r->h->refresh_type |= REFRESH_BOUNDS;
    if(
	 SEL(r).op && SEL(r).vt == page &&
	 PVTS(r, page)->current_screen == SEL(r).screen &&
	 (
	   (SEL(r).beg.row >= row && SEL(r).beg.row <= row + num) ||
	   (SEL(r).end.row >= row && SEL(r).end.row <= row + num)
	 )
      )
    {
	CLEAR_SELECTION(r);
    }

    if (row >= r->TermWin.nrow)	/* Out Of Bounds */
	return;

    MIN_IT(num, (r->TermWin.nrow - row));
    if (PVTS(r, page)->rstyle & (RS_RVid | RS_Uline))
	ren = (rend_t) ~RS_None;

    else if (GET_BASEBG(PVTS(r, page)->rstyle) == Color_bg)
    {
	ren = DEFAULT_RSTYLE;
	CLEAR_ROWS(row, num);
    }

    else
    {
	ren = (PVTS(r, page)->rstyle & (RS_fgMask | RS_bgMask));

	gcvalue.foreground = r->pixColors[GET_BGCOLOR(PVTS(r, page)->rstyle)];
	XChangeGC(r->Xdisplay, r->TermWin.gc, GCForeground, &gcvalue);
	ERASE_ROWS(row, num);

	gcvalue.foreground = r->pixColors[Color_fg];
	XChangeGC(r->Xdisplay, r->TermWin.gc, GCForeground, &gcvalue);
    }

    for (; num--; row++)
    {
	rxvt_blank_screen_mem(r, page, PSCR(r, page).text,
		PSCR(r, page).rend,
		(unsigned int)(row + row_offset), PVTS(r, page)->rstyle);
	PSCR(r, page).tlen[row + row_offset] = 0;
	rxvt_blank_line(PVTS(r, page)->drawn_text[row],
		PVTS(r, page)->drawn_rend[row],
		(unsigned int) r->TermWin.ncol, ren);
    }
}