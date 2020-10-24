void
rxvt_process_nonprinting(rxvt_t* r, int page, unsigned char ch)
{
    switch (ch)
    {
	case C0_ENQ:	/* terminal Status */
	    if (r->h->rs[Rs_answerbackstring])
		rxvt_tt_write(r, page,
		    (const unsigned char *)r->h->rs[Rs_answerbackstring],
		    (unsigned int)STRLEN(r->h->rs[Rs_answerbackstring]));
	    else
		rxvt_tt_write(r, page, (unsigned char *)VT100_ANS,
		    (unsigned int)STRLEN(VT100_ANS));
	    break;

	case C0_BEL:	/* bell */
	    rxvt_scr_bell(r, page);
	    if (page != ATAB(r))
		rxvt_tabbar_highlight_tab(r, page, False);
	    break;

	case C0_BS:	/* backspace */
	    rxvt_scr_backspace(r, page);
	    break;

	case C0_HT:	/* tab */
	    rxvt_scr_tab(r, page, 1);
	    break;

	case C0_CR:	/* carriage return */
	    rxvt_scr_gotorc(r, page, 0, 0, R_RELATIVE);
	    break;

	case C0_VT:	/* vertical tab, form feed */
	case C0_FF:
	case C0_LF:	/* line feed */
	    rxvt_scr_index(r, page, UP);
	    break;

	case C0_SO:	/* shift out - acs */
	    rxvt_scr_charset_choose(r, page, 1);
	    break;

	case C0_SI:	/* shift in - acs */
	    rxvt_scr_charset_choose(r, page, 0);
	    break;
    }
}