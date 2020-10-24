int
rxvt_scroll_text(rxvt_t* r, int page, int row1, int row2, int count, int spec)
{
    int		    i, j, ret;
    unsigned int    nscrolled;
    size_t 	    size;

    if (count == 0 || (row1 > row2))
	return 0;

    PVTS(r, page)->want_refresh = 1;
    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN,
		"rxvt_scroll_text %d (%d,%d,%d,%d): %s\n", page, row1, row2,
		count, spec, (PVTS(r, page)->current_screen == PRIMARY) ?
		    "Primary" : "Secondary" ));

    if (
	  (count > 0)
	  && (row1 == 0)
	  && (PVTS(r, page)->current_screen == PRIMARY)
       )
    {
	nscrolled = (unsigned int) PVTS(r, page)->nscrolled
					    + (unsigned int) count;

	if (nscrolled > (unsigned int)SVLINES)
	    PVTS(r, page)->nscrolled = SVLINES;
	else
	    PVTS(r, page)->nscrolled = (uint16_t)nscrolled;
    }
    else if (!spec)
	row1 += SVLINES;
    row2 += SVLINES;

    if (
	  SEL(r).op
	  && SEL(r).vt == page
	  && PVTS(r, page)->current_screen == SEL(r).screen
       )
    {
	i = SEL(r).beg.row + SVLINES;
	j = SEL(r).end.row + SVLINES;
	if (
	      (i < row1 && j > row1)
	      || (i < row2 && j > row2)
	      || (i - count < row1 && i >= row1)
	      || (i - count > row2 && i <= row2)
	      || (j - count < row1 && j >= row1)
	      || (j - count > row2 && j <= row2)
	   )
	{
	    CLEAR_ALL_SELECTION(r);
	    /* XXX: too aggressive? */
	    SEL(r).op = SELECTION_CLEAR;
	}
	else if (j >= row1 && j <= row2)
	{
	    /* move selected region too */
	    SEL(r).beg.row -= count;
	    SEL(r).end.row -= count;
	    SEL(r).mark.row -= count;
	}
    }

    /* _after_ PVTS(r, page)->nscrolled update */
    rxvt_selection_check(r, page, 0);

    PVTS(r, page)->num_scr += count;
    j = count;
    if (count < 0)
	count = -count;
    i = row2 - row1 + 1;
    MIN_IT(count, i);

    if (j > 0)
    {
	/* A: scroll up */

	/* A1: Copy lines that will get clobbered by the rotation */
	for (i = count - 1, j = row1; i >= 0; i--, j++)
	{
	    PVTS(r, page)->buf_text[i] = PSCR(r, page).text[j];
	    PVTS(r, page)->buf_rend[i] = PSCR(r, page).rend[j];
	}

	/* A2: Rotate lines */
	size = sizeof(*PSCR(r, page).tlen);
	MEMMOVE(&(PSCR(r, page).tlen[row1]), &(PSCR(r, page).tlen[row1+count]),
			(row2 - row1 - count + 1) * size);
	size = sizeof(*PSCR(r, page).text);
	MEMMOVE(&(PSCR(r, page).text[row1]), &(PSCR(r, page).text[row1+count]),
			(row2 - row1 - count + 1) * size);
	size = sizeof(*PSCR(r, page).rend);
	MEMMOVE(&(PSCR(r, page).rend[row1]), &(PSCR(r, page).rend[row1+count]),
			(row2 - row1 - count + 1) * size);

	j = row2 - count + 1;
	ret = i = count;
    }
    else /* if (j < 0) */
    {
	/* B: scroll down */

	/* B1: Copy lines that will get clobbered by the rotation */
	size = sizeof(*PSCR(r, page).text);
	MEMCPY(PVTS(r, page)->buf_text, &PSCR(r, page).text[row2 - count + 1], count * size);
	size = sizeof(*PSCR(r, page).rend);
	MEMCPY(PVTS(r, page)->buf_rend, &PSCR(r, page).rend[row2 - count + 1], count * size);

	/* B2: Rotate lines */
	size = sizeof(*PSCR(r, page).tlen);
	MEMMOVE(&(PSCR(r, page).tlen[row1 + count]), &(PSCR(r, page).tlen[row1]),
			(row2 - row1 - count + 1) * size);
	size = sizeof(*PSCR(r, page).text);
	MEMMOVE(&(PSCR(r, page).text[row1 + count]), &(PSCR(r, page).text[row1]),
			(row2 - row1 - count + 1) * size);
	size = sizeof(*PSCR(r, page).rend);
	MEMMOVE(&(PSCR(r, page).rend[row1 + count]), &(PSCR(r, page).rend[row1]),
			(row2 - row1 - count + 1) * size);

	j = row1, i = count;
	ret = -count;
    }

    /* C: Resurrect lines */
    size = sizeof(*PSCR(r, page).tlen);
    MEMSET(&PSCR(r, page).tlen[j], 0, count * size);
    size = sizeof(*PSCR(r, page).text);
    MEMCPY(&PSCR(r, page).text[j], PVTS(r, page)->buf_text, count * size);
    size = sizeof(*PSCR(r, page).rend);
    MEMCPY(&PSCR(r, page).rend[j], PVTS(r, page)->buf_rend, count * size);

    for (; i--; j++)
    {
	if (!spec)	/* line length may not equal TermWin.ncol */
	    rxvt_blank_screen_mem(r, page, PSCR(r, page).text,
		    PSCR(r, page).rend, (unsigned int)j,
		    PVTS(r, page)->rstyle);
    }

    return ret;
}