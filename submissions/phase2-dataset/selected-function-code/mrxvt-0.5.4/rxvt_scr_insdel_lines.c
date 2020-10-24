void
rxvt_scr_insdel_lines(rxvt_t* r, int page, int count, int insdel)
{
    int		 end;

    ZERO_SCROLLBACK(r, page);
    RESET_CHSTAT(r, page);
    rxvt_selection_check(r, page, 1);

    if (CURROW > PSCR(r, page).bscroll)
	return;

    end = PSCR(r, page).bscroll - CURROW + 1;
    if (count > end)
    {
	if (insdel == DELETE)
	    return;
	else if (insdel == INSERT)
	    count = end;
    }
    PSCR(r, page).flags &= ~Screen_WrapNext;

    rxvt_scroll_text(r, page, CURROW,
	PSCR(r, page).bscroll, insdel * count, 0);
}