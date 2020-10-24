 * EXT: button 1 or 3 release
 */
/* EXTPROTO */
void
rxvt_selection_make(rxvt_t* r, int page, Time tm)
{
    int		    i, col, end_col, row, end_row;
    unsigned char*  new_selection_text;
    unsigned char*  str;
    text_t*	    t;
#ifdef MULTICHAR_SET
    rend_t*	    tr;
#endif
#ifdef ACS_ASCII
    rend_t*	    re;
#endif

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_selection_make %d (): sel.op=%d, sel.clicks=%d\n", page, SEL(r).op, SEL(r).clicks));
    switch (SEL(r).op)
    {
	case SELECTION_CONT:
	    break;
	case SELECTION_INIT:
	    CLEAR_SELECTION(r);
	    /* FALLTHROUGH */
	case SELECTION_BEGIN:
	    SEL(r).op = SELECTION_DONE;
	    /* FALLTHROUGH */
	default:
	    return;
    }
    SEL(r).op = SELECTION_DONE;
    SEL(r).vt = page;	/* update selection vt */

    if (SEL(r).clicks == 4)
	return;		/* nothing selected, go away */

    assert ((SEL(r).end.row - SEL(r).beg.row + 1) > 0);
    assert ((r->TermWin.ncol + 1) > 0);
    i = (SEL(r).end.row - SEL(r).beg.row + 1)
	* (r->TermWin.ncol + 1) + 1;
    /* possible integer overflow? */
    assert (i > 0);
    str = rxvt_malloc(i * sizeof(char));

    new_selection_text = (unsigned char *)str;

    col = SEL(r).beg.col;
    MAX_IT(col, 0);
    row = SEL(r).beg.row + SVLINES;
    end_row = SEL(r).end.row + SVLINES;

    /*
    ** A: rows before end row
    */
    for (; row < end_row; row++, col = 0)
    {
	t = &(PSCR(r, page).text[row][col]);
#ifdef MULTICHAR_SET
	tr = &(PSCR(r, page).rend[row][col]);
#endif	/* MULTICHAR_SET */
#ifdef ACS_ASCII
	re = &(PSCR(r, page).rend[row][col]);
#endif
	if ((end_col = PSCR(r, page).tlen[row]) == -1)
	    end_col = r->TermWin.ncol;


	/*
	** Looks like a completely mess. Think about the logic here
	** carefully. ;-)
	** Patch source:
	** http://gentoo.nedlinux.nl/distfiles/rxvt-2.7.10-rk.patch
	*/
	for (; col < end_col; col++, str++, t++)
	{
#ifdef MULTICHAR_SET
	    if (
		  (ENC_EUCJ == r->encoding_method) && (*t & 0x80)
		  && !(*tr & RS_multiMask)
	       )
	    {
		*str++ = 0x8E;
	    }
	    tr ++;
#endif	/* MULTICHAR_SET */
#ifdef ACS_ASCII
	    if ((*re++ & RS_acsFont) && *t >= 0x60 && *t < 0x80)
		*str = r->h->rs[Rs_acs_chars][(*t) - 0x60];
	    else
#endif	/* ACS_ASCII */
	    *str = *t;
	}


	if (PSCR(r, page).tlen[row] != -1)
	{
#ifdef DONT_SELECT_TRAILING_SPACES
	    STRIP_TRAILING_SPACE(str, new_selection_text);
#endif
	    *str++ = '\n';
	}
    }

    /*
    ** B: end row
    */
    t = &(PSCR(r, page).text[row][col]);
#ifdef MULTICHAR_SET
    tr = &(PSCR(r, page).rend[row][col]);
#endif	/* MULTICHAR_SET */
#ifdef ACS_ASCII
    re = &(PSCR(r, page).rend[row][col]);
#endif
    end_col = PSCR(r, page).tlen[row];
    if (end_col == -1 || SEL(r).end.col <= end_col)
	end_col = SEL(r).end.col;
    MIN_IT(end_col, r->TermWin.ncol);	/* CHANGE */


    /*
    ** Looks like a completely mess. Think about the logic here
    ** carefully. ;-)
    ** Patch source:
    ** http://gentoo.nedlinux.nl/distfiles/rxvt-2.7.10-rk.patch
    */
    for (; col < end_col; col++, str++, t++)
    {
#ifdef MULTICHAR_SET
	if (
	      (ENC_EUCJ == r->encoding_method) && (*t & 0x80)
	      && !(*tr & RS_multiMask)
	   )
	{
	    *str++ = 0x8E;
	}
	tr ++;
#endif	/* MULTICHAR_SET */
#ifdef ACS_ASCII
	if ((*re++ & RS_acsFont) && *t >= 0x60 && *t < 0x80)
	    *str = r->h->rs[Rs_acs_chars][(*t) - 0x60];
	else
#endif	/* ACS_ASCII */
	*str = *t;
    }

#ifdef DONT_SELECT_TRAILING_SPACES
    STRIP_TRAILING_SPACE(str, new_selection_text);
#endif


#ifndef NO_OLD_SELECTION
    if (r->selection_style == OLD_SELECT)
	if (end_col == r->TermWin.ncol)
	{
	    *str++ = '\n';
	}
#endif
#ifndef NO_NEW_SELECTION
    if (r->selection_style != OLD_SELECT)
	if (end_col != SEL(r).end.col)
	{
	    *str++ = '\n';
	}
#endif
    *str = '\0';
    if ((i = STRLEN((char *)new_selection_text)) == 0)
    {
	rxvt_free(new_selection_text);
	return;
    }
    SEL(r).len = i;
    if (SEL(r).text)
	rxvt_free(SEL(r).text);
    SEL(r).text = new_selection_text;

    XSetSelectionOwner(r->Xdisplay, XA_PRIMARY, PVTS(r, page)->vt, tm);
    if (XGetSelectionOwner(r->Xdisplay, XA_PRIMARY) != PVTS(r, page)->vt)
	rxvt_msg (DBG_ERROR, DBG_SCREEN, "can't get primary selection");
    XChangeProperty(r->Xdisplay, XROOT, XA_CUT_BUFFER0, XA_STRING, 8,
	PropModeReplace, SEL(r).text, (int)SEL(r).len);
    r->h->selection_time = tm;