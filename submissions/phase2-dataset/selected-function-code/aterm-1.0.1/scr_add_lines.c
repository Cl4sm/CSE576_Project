void
scr_add_lines(const unsigned char *str, int nlines, int len)
{
    char            c;
    int             i, j, row, last_col, wherecursor, wrotespecial;
    text_t         *stp;
    rend_t         *srp;

    if (len <= 0)		/* sanity */
	return;

    last_col = TermWin.bcol;

    D_SCREEN((stderr, "scr_add_lines(*,%d,%d)", nlines, len));
    ZERO_SCROLLBACK;
    if (nlines > 0) 
	{
		nlines += (screen.cur.row - screen.bscroll);
		if ((nlines > 0) && (screen.tscroll == 0)&& (screen.bscroll == (TermWin.nrow - 1))) 
		{/* _at least_ this many lines need to be scrolled */
	    	scroll_text(screen.tscroll, screen.bscroll, nlines, 0);
	    	for (i = nlines, j = screen.bscroll + TermWin.saveLines; i--; j--) 
			{
				if (screen.text[j] == NULL)
		    		make_screen_mem(screen.text, screen.rend, j, rstyle);
				else
		    		blank_line(screen.text[j], screen.rend[j], TermWin.bcol, rstyle);
				screen.tlen[j] = 0;
	    	}
	    	screen.cur.row -= nlines;
		}
    }
#ifdef DEBUG_STRICT
    assert(screen.cur.col < last_col);
    assert(screen.cur.row < TermWin.nrow);
    assert(screen.cur.row >= -TermWin.nscrolled);
#else				/* drive with your eyes closed */
    MIN_IT(screen.cur.col, last_col - 1);
    MIN_IT(screen.cur.row, TermWin.nrow - 1);
    MAX_IT(screen.cur.row, -TermWin.nscrolled);
#endif
    row = screen.cur.row + TermWin.saveLines;

    if (ROW_AND_COL_IS_BEFORE(screen.cur.row, screen.cur.col,
		    	      selection.beg.row, selection.beg.col))
		wherecursor = -1;
    else if (ROW_AND_COL_IS_BEFORE(screen.cur.row, screen.cur.col,
    		                   selection.end.row, selection.end.col))
		wherecursor = 0;
    else
		wherecursor = 1;

    stp = screen.text[row];
    srp = screen.rend[row];

#ifdef MULTICHAR_SET
    if (lost_multi && screen.cur.col > 0
		&& ((srp[screen.cur.col - 1] & RS_multiMask) == RS_multi1)
		&& *str != '\n' && *str != '\r' && *str != '\t')
		chstat = WBYTE;
#endif

    for (wrotespecial = len, i = 0; i < len;) 
	{
		c = str[i++];
#ifdef MULTICHAR_SET
		if (chstat == WBYTE) 
		{
	    	rstyle |= RS_multiMask;	/* multibyte 2nd byte */
	    	chstat = SBYTE;
	    	if (encoding_method == EUCJ)
				c |= 0x80;	/* maybe overkill, but makes it selectable */
		} else if (chstat == SBYTE) 
		{
	    	if (multi_byte || (c & 0x80)) 
			{	/* multibyte 1st byte */
				rstyle &= ~RS_multiMask;
				rstyle |= RS_multi1;
				chstat = WBYTE;
				if (encoding_method == EUCJ)
		    		c |= 0x80;	/* maybe overkill, but makes it selectable */
	    	} else
#endif				/* MULTICHAR_SET */
				switch (c) 
				{
					case 127:
		    			wrotespecial--;
		    			continue;	/* yummmm..... */
					case '\t':
		    			wrotespecial--;
		    			scr_tab(1);
		    			continue;
					case '\n':
		    			wrotespecial--;
		    			if (screen.tlen[row] != -1)	/* XXX: think about this */
							MAX_IT(screen.tlen[row], screen.cur.col);
		    			screen.flags &= ~Screen_WrapNext;
		    			if (screen.cur.row == screen.bscroll) 
						{
							scroll_text(screen.tscroll, screen.bscroll, 1, 0);
							j = screen.bscroll + TermWin.saveLines;
							if (screen.text[j] == NULL)
			    				make_screen_mem(screen.text, screen.rend, j, rstyle);
							else
			    				blank_line(screen.text[j], screen.rend[j], TermWin.bcol, rstyle);
							screen.tlen[j] = 0;
		    			} else if (screen.cur.row < (TermWin.nrow - 1))
							row = (++screen.cur.row) + TermWin.saveLines;
		    			stp = screen.text[row];	/* _must_ refresh */
		    			srp = screen.rend[row];	/* _must_ refresh */
		    			continue;
					case '\r':
		    			wrotespecial--;
		    			if (screen.tlen[row] != -1)	/* XXX: think about this */
							MAX_IT(screen.tlen[row], screen.cur.col);
		    			screen.flags &= ~Screen_WrapNext;
		    			screen.cur.col = 0;
		    			continue;
					default:
#ifdef MULTICHAR_SET
					    rstyle &= ~RS_multiMask;
#endif
		    			break;
				}
#ifdef MULTICHAR_SET
		}
#endif

	if (screen.flags & Screen_WrapNext) {
	    screen.tlen[row] = -1;
	    if (screen.cur.row == screen.bscroll) {
		scroll_text(screen.tscroll, screen.bscroll, 1, 0);
		j = screen.bscroll + TermWin.saveLines;
		if (screen.text[j] == NULL)
		    make_screen_mem(screen.text, screen.rend, j, rstyle);
		else
		    blank_line(screen.text[j], screen.rend[j], TermWin.bcol,
			       rstyle);
		screen.tlen[j] = 0;
	    } else if (screen.cur.row < (TermWin.nrow - 1))
		row = (++screen.cur.row) + TermWin.saveLines;
	    stp = screen.text[row];	/* _must_ refresh */
	    srp = screen.rend[row];	/* _must_ refresh */
	    screen.cur.col = 0;
	    screen.flags &= ~Screen_WrapNext;
	}
	if (screen.flags & Screen_Insert)
	    scr_insdel_chars(1, INSERT);
	stp[screen.cur.col] = c;
	srp[screen.cur.col] = rstyle;
	/*
         * TODO: This might end up wrapping the tone sign in Thai
         */
	if (screen.cur.col < (last_col - 1))
	    screen.cur.col++;
	else {
	    screen.tlen[row] = last_col;
	    if (screen.flags & Screen_Autowrap)
		screen.flags |= Screen_WrapNext;
	    else
		screen.flags &= ~Screen_WrapNext;
	}
    }
    if (screen.tlen[row] != -1)	/* XXX: think about this */
	MAX_IT(screen.tlen[row], screen.cur.col);

/*
 * If we wrote anywhere in the selected area, kill the selection
 * XXX: should we kill the mark too?  Possibly, but maybe that
 *      should be a similar check.
 */
    if (ROW_AND_COL_IS_BEFORE(screen.cur.row, screen.cur.col,
		    	      selection.beg.row, selection.beg.col))
	i = -1;
    else if (ROW_AND_COL_IS_BEFORE(screen.cur.row, screen.cur.col,
    		                   selection.end.row, selection.end.col))
	i = 0;
    else
	i = 1;
    if (selection.op && current_screen == selection.screen
	&& wrotespecial != 0 && (i != wherecursor || i == 0))
	CLEAR_SELECTION;

#ifdef DEBUG_STRICT
    assert(screen.cur.row >= 0);
#else				/* drive with your eyes closed */
    MAX_IT(screen.cur.row, 0);
#endif
}
