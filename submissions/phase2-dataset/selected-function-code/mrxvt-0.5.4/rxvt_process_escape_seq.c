void
rxvt_process_escape_seq(rxvt_t* r, int page)
{
    int		    readpage = page;
    unsigned char   c,
		    ch = rxvt_cmd_getc(r, &readpage);

    if( readpage == -1 )
    {
	rxvt_set_escfail( r, page, 1 );
	return;
    }

    if (ISSET_PMODE(r, page, PrivMode_vt52))
    {
	rxvt_process_escape_vt52(r, page, ch);
	return;
    }

    switch (ch)
    {
	/* case 1:	do_tek_mode (); break; */
	case '#':
	    c = rxvt_cmd_getc( r, &readpage );

	    if( readpage == -1 )
		rxvt_set_escfail( r, page, 1 );
	    else if( c == 8 )
		rxvt_scr_E(r, readpage);

	    break;

	case '(':
	    c = rxvt_cmd_getc( r, &readpage );

	    if( readpage == -1 )
		rxvt_set_escfail( r, page, 1 );
	    else
		rxvt_scr_charset_set(r, page, 0, (unsigned int) c );
	    break;

	case ')':
	    c = rxvt_cmd_getc( r, &readpage );

	    if( readpage == -1 )
		rxvt_set_escfail( r, page, 1 );
	    else
		rxvt_scr_charset_set(r, page, 1, (unsigned int) c);

	    break;

	case '*':
	    c = rxvt_cmd_getc( r, &readpage );

	    if( readpage == -1 )
		rxvt_set_escfail( r, page, 1 );
	    else
		rxvt_scr_charset_set(r, page, 2, (unsigned int) c);

	    break;

	case '+':
	    c = rxvt_cmd_getc( r, &readpage );

	    if( readpage == -1 )
		rxvt_set_escfail( r, page, 1 );
	    else
		rxvt_scr_charset_set(r, page, 3, (unsigned int) c);

	    break;

#ifdef MULTICHAR_SET
	case '$':
	    c = rxvt_cmd_getc( r, &readpage );

	    if( readpage == -1 )
		rxvt_set_escfail( r, page, 1 );
	    else
		rxvt_scr_charset_set(r, page, -2, (unsigned int) c);

	    break;
#endif

#ifndef NO_FRILLS
	case '6':
	    rxvt_scr_backindex(r, page);
	    break;
#endif
	case '7':
	    rxvt_scr_cursor(r, page, SAVE);
	    break;
	case '8':
	    rxvt_scr_cursor(r, page, RESTORE);
	    break;
#ifndef NO_FRILLS
	case '9':
	    rxvt_scr_forwardindex(r, page);
	    break;
#endif
	case '=':
	case '>':
	    PrivMode((ch == '='), PrivMode_aplKP, page);
	    break;

	case C1_40:
	    c = rxvt_cmd_getc( r, &readpage );

	    if( readpage == -1 )
		rxvt_set_escfail( r, page, 1 );

	    /* 2006-08-31 gi1242 XXX Why is there no code here? */

	    break;

	case C1_44:
	    rxvt_scr_index(r, page, UP);
	    break;

	/* 8.3.87: NEXT LINE */
	case C1_NEL:	    /* ESC E */
	    rxvt_scr_add_lines(r, page, (const unsigned char *)"\n\r", 1, 2);
	    break;

	/* kidnapped escape sequence: Should be 8.3.48 */
	case C1_ESA:	    /* ESC G */
	    rxvt_process_graphics(r, page);
	    break;

	/* 8.3.63: CHARACTER TABULATION SET */
	case C1_HTS:	    /* ESC H */
	    rxvt_scr_set_tab(r, page, 1);
	    break;

	/* 8.3.105: REVERSE LINE FEED */
	case C1_RI:	    /* ESC M */
	    rxvt_scr_index(r, page, DN);
	    break;

	/* 8.3.142: SINGLE-SHIFT TWO */
	/*case C1_SS2: scr_single_shift (2);   break; */

	/* 8.3.143: SINGLE-SHIFT THREE */
	/*case C1_SS3: scr_single_shift (3);   break; */

	/* 8.3.27: DEVICE CONTROL STRING */
	case C1_DCS:	    /* ESC P */
	    /* rxvt_process_dcs_seq(r, page); */
	    rxvt_process_xwsh_seq (r, page);
	    break;

	/* 8.3.110: SINGLE CHARACTER INTRODUCER */
	case C1_SCI:	    /* ESC Z */
	    rxvt_tt_write(r, page, (const unsigned char *)ESCZ_ANSWER,
		  (unsigned int)(sizeof(ESCZ_ANSWER) - 1));
	    break;	    /* steal obsolete ESC [ c */

	/* 8.3.16: CONTROL SEQUENCE INTRODUCER */
	case C1_CSI:	    /* ESC [ */
	    rxvt_process_csi_seq(r, page);
	    break;

	/* 8.3.90: OPERATING SYSTEM COMMAND */
	case C1_OSC:	    /* ESC ] */
	    rxvt_process_osc_seq(r, page);
	    break;

	/* 8.3.106: RESET TO INITIAL STATE */
	case 'c':
	    rxvt_scr_poweron(r, page);
#ifdef HAVE_SCROLLBARS
	    rxvt_scrollbar_update(r, 1);
#endif
	    break;

	/* 8.3.79: LOCKING-SHIFT TWO (see ISO2022) */
	case 'n':
	    rxvt_scr_charset_choose(r, page, 2);
	    break;

	/* 8.3.81: LOCKING-SHIFT THREE (see ISO2022) */
	case 'o':
	    rxvt_scr_charset_choose(r, page, 3);
	    break;
    }
}