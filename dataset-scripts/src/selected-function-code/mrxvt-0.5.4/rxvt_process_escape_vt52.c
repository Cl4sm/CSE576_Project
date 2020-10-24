void
rxvt_process_escape_vt52(rxvt_t* r, int page, unsigned char ch)
{
    int	    row, col;
    int	    readpage = page;

    switch (ch)
    {
	case 'A':	/* cursor up */
	    rxvt_scr_gotorc(r, page, -1, 0, R_RELATIVE | C_RELATIVE);
	    break;

	case 'B':	/* cursor down */
	    rxvt_scr_gotorc(r, page, 1, 0, R_RELATIVE | C_RELATIVE);
	    break;

	case 'C':	/* cursor right */
	    rxvt_scr_gotorc(r, page, 0, 1, R_RELATIVE | C_RELATIVE);
	    break;

	case 'D':	/* cursor left */
	    rxvt_scr_gotorc(r, page, 0, -1, R_RELATIVE | C_RELATIVE);
	    break;

	case 'H':	/* cursor home */
	    rxvt_scr_gotorc(r, page, 0, 0, 0);
	    break;

	case 'I':	/* cursor up and scroll down if needed */
	    rxvt_scr_index(r, page, DN);
	    break;

	case 'J':	/* erase to end of screen */
	    rxvt_scr_erase_screen(r, page, 0);
	    break;

	case 'K':	/* erase to end of line */
	    rxvt_scr_erase_line(r, page, 0);
	    break;

	case 'Y':	    /* move to specified row and col */
	    /* full command is 'ESC Y row col' where row and col
	    ** are encoded by adding 32 and sending the ascii
	    ** character.  eg. SPACE = 0, '+' = 13, '0' = 18,
	    ** etc.
	    */
	    row = rxvt_cmd_getc(r, &readpage) - ' ';
	    if( readpage == -1 )
	    {
		rxvt_set_escfail( r, page, 2 );
		break;
	    }

	    col = rxvt_cmd_getc(r, &readpage) - ' ';
	    if( readpage == -1 )
	    {
		rxvt_set_escfail( r, page, 1 );
		break;
	    }

	    rxvt_scr_gotorc(r, page, row, col, 0);
	    break;

	case 'Z':	/* identify the terminal type */
	    /* I am a VT100 emulating a VT52 */
	    rxvt_tt_printf(r, page, "\033/Z");
	    break;

	case '<':	/* turn off VT52 mode */
	    PrivMode(0, PrivMode_vt52, page);
	    break;

	case 'F':    	/* use special graphics character set */
	case 'G':	   /* use regular character set */
	    /* unimplemented */
	    break;

	case '=':    	/* use alternate keypad mode */
	case '>':	   /* use regular keypad mode */
	    /* unimplemented */
	    break;
    }
}