/* INTPROTO */
void
rxvt_set_font_style(rxvt_t *r, int page)
{
    PVTS(r, page)->rstyle &= ~RS_fontMask;
    switch (PVTS(r, page)->charsets[PSCR(r, page).charset])
    {
	case '0':	    /* DEC Special Character & Line Drawing Set */
	    PVTS(r, page)->rstyle |= RS_acsFont;
	    break;
	case 'A':	    /* United Kingdom (UK) */
	    PVTS(r, page)->rstyle |= RS_ukFont;
	    break;
	case 'B':	    /* United States (USASCII) */
	    break;
	case '<':	    /* Multinational character set */
	    break;
	case '5':	    /* Finnish character set */
	    break;
	case 'C':	    /* Finnish character set */
	    break;
	case 'K':	    /* German character set */
	    break;
    }