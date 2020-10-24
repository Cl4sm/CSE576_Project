void
rxvt_process_csi_seq(rxvt_t* r, int page)
{
    int		    readpage = page;
    unsigned char   ch, priv, i;
    unsigned int    nargs, p;
    int		    n, ndef;
    int		    arg[ESC_ARGS];


    for (nargs = ESC_ARGS; nargs > 0;)
	arg[--nargs] = 0;

    priv = 0;
    ch = rxvt_cmd_getc(r, &readpage);
    if( readpage == -1 )
    {
	rxvt_set_escfail( r, page, 1 );
	return;
    }

    if (ch >= '<' && ch <= '?')		/* '<' '=' '>' '?' */
    {
	priv = ch;
	ch = rxvt_cmd_getc(r, &readpage);
	if( readpage == -1 )
	{
	    rxvt_set_escfail( r, page, 1 );
	    return;
	}
    }

    /* read any numerical arguments */
    for (n = -1; ch < CSI_ICH; )
    {
	if (isdigit(ch))
	{
	    if (n < 0)
		n = ch - '0';
	    else
		n = n * 10 + ch - '0';
	}
	else if (ch == ';')
	{
	    if (nargs < ESC_ARGS)
		arg[nargs++] = n;
	    n = -1;
	}
	else if (ch == '\b')
	{
	    rxvt_scr_backspace(r, page);
	}
	else if (ch == C0_ESC)
	{
	    rxvt_process_escape_seq(r, page);
	    return;
	}
	else if (ch < ' ')
	{
	    rxvt_process_nonprinting(r, page, ch);
	}

	ch = rxvt_cmd_getc(r, &readpage);
	if( readpage == -1 )
	{
	    rxvt_set_escfail( r, page, 1 );
	    return;
	}
    }

    if (ch > CSI_7F)
	return;

    if (nargs < ESC_ARGS)
    arg[nargs++] = n;

    i = ch - CSI_ICH;
    ndef = get_byte_array_bit(csi_defaults, i);
    for (p = 0; p < nargs; p++)
	if (arg[p] == -1)
	    arg[p] = ndef;

#ifdef DEBUG
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "CSI "));
    for (p = 0; p < nargs; p++)
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "%d%s", arg[p], p < nargs - 1 ? ";" : ""));
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "%c\n", ch));
#endif


    /* private mode handling */
    if (priv)
    {
	switch (priv)
	{
	case '>':
	    if (ch == CSI_DA)	/* secondary device attributes */
	    rxvt_tt_printf(r, page, "\033[>%d;%-.8s;0c", 'R', VERSION);
	    break;
	case '?':
	    if (ch == 'h' || ch == 'l' || ch == 'r' || ch == 's' || ch == 't')
	    {
		rxvt_process_terminal_mode(r, page, ch, priv, nargs, arg);
	    }
	    break;
	}
	return;
    }


    switch (ch)
    {
	/* ISO/IEC 6429:1992(E) CSI sequences (defaults in parentheses) */
#ifdef PRINTPIPE
	case CSI_MC:	    /* 8.3.83: (0) MEDIA COPY */
	    switch (arg[0])
	    {
		case 0:		/* initiate transfer to primary aux device */
		    rxvt_scr_printscreen( r, page, 0, 0, 0, NULL );
		    break;
		case 5:		/* start relay to primary aux device */
		    rxvt_process_print_pipe( r, page );
		    break;
	    }
	    break;
#endif

	case CSI_CUU:	    /* 8.3.22: (1) CURSOR UP */
	case CSI_VPR:	    /* 8.3.161: (1) LINE POSITION FORWARD */
	    arg[0] = -arg[0];
	    /* FALLTHROUGH */
	case CSI_CUD:	    /* 8.3.19: (1) CURSOR DOWN */
	case CSI_VPB:	    /* 8.3.160: (1) LINE POSITION BACKWARD */
	    rxvt_scr_gotorc(r, page, arg[0], 0, RELATIVE);
	    break;

	case CSI_CUB:	    /* 8.3.18: (1) CURSOR LEFT */
	case CSI_HPB: 	    /* 8.3.59: (1) CHARACTER POSITION BACKWARD */
#ifdef ISO6429
	    arg[0] = -arg[0];
#else		    /* emulate common DEC VTs */
	    arg[0] = arg[0] ? -arg[0] : -1;
#endif
	    /* FALLTHROUGH */
	case CSI_CUF:	    /* 8.3.20: (1) CURSOR RIGHT */
	case CSI_HPR:	    /* 8.3.60: (1) CHARACTER POSITION FORWARD */
#ifdef ISO6429
	    rxvt_scr_gotorc(r, page, 0, arg[0], RELATIVE);
#else		    /* emulate common DEC VTs */
	    rxvt_scr_gotorc(r, page, 0, arg[0] ? arg[0] : 1, RELATIVE);
#endif
	    break;

	case CSI_CPL:	    /* 8.3.13: (1) CURSOR PRECEDING LINE */
	    arg[0] = -arg[0];
	    /* FALLTHROUGH */
	case CSI_CNL:	    /* 8.3.12: (1) CURSOR NEXT LINE */
	    rxvt_scr_gotorc(r, page, arg[0], 0, R_RELATIVE);
	    break;

	case CSI_CHA:	    /* 8.3.9: (1) CURSOR CHARACTER ABSOLUTE */
	case CSI_HPA:	    /* 8.3.58: (1) CURSOR POSITION ABSOLUTE */
	    rxvt_scr_gotorc(r, page, 0, arg[0] - 1, R_RELATIVE);
	    break;

	case CSI_VPA:	    /* 8.3.159: (1) LINE POSITION ABSOLUTE */
	    rxvt_scr_gotorc(r, page, arg[0] - 1, 0, C_RELATIVE);
	    break;

	case CSI_CUP:	    /* 8.3.21: (1,1) CURSOR POSITION */
	case CSI_HVP:	    /* 8.3.64: (1,1) CHARACTER AND LINE POSITION */
	    rxvt_scr_gotorc(r, page, arg[0] - 1, nargs < 2 ? 0 : (arg[1] - 1), 0);
	    break;

	case CSI_CBT:	    /* 8.3.7: (1) CURSOR BACKWARD TABULATION */
	    arg[0] = -arg[0];
	    /* FALLTHROUGH */
	case CSI_CHT:	    /* 8.3.10: (1) CURSOR FORWARD TABULATION */
	    rxvt_scr_tab(r, page, arg[0]);
	    break;

	case CSI_ED:	    /* 8.3.40: (0) ERASE IN PAGE */
	    rxvt_scr_erase_screen(r, page, arg[0]);
	    break;

	case CSI_EL:	    /* 8.3.42: (0) ERASE IN LINE */
	    rxvt_scr_erase_line(r, page, arg[0]);
	    break;

	case CSI_ICH:	    /* 8.3.65: (1) INSERT CHARACTER */
	    rxvt_scr_insdel_chars(r, page, arg[0], INSERT);
	    break;

	case CSI_IL:	    /* 8.3.68: (1) INSERT LINE */
	    rxvt_scr_insdel_lines(r, page, arg[0], INSERT);
	    break;

	case CSI_DL:	    /* 8.3.33: (1) DELETE LINE */
	    rxvt_scr_insdel_lines(r, page, arg[0], DELETE);
	    break;

	case CSI_ECH:	    /* 8.3.39: (1) ERASE CHARACTER */
	    rxvt_scr_insdel_chars(r, page, arg[0], ERASE);
	    break;

	case CSI_DCH:	    /* 8.3.26: (1) DELETE CHARACTER */
	    rxvt_scr_insdel_chars(r, page, arg[0], DELETE);
	    break;

	case CSI_SD:	    /* 8.3.114: (1) SCROLL DOWN */
	    arg[0] = -arg[0];
	    /* FALLTHROUGH */
	case CSI_SU:	    /* 8.3.148: (1) SCROLL UP */
	    rxvt_scroll_text(r, page, PVTS(r, page)->screen.tscroll,
		PVTS(r, page)->screen.bscroll, arg[0], 0);
	    break;

	case CSI_DA:	    /* 8.3.24: (0) DEVICE ATTRIBUTES */
	    rxvt_tt_write(r, page, (const unsigned char *)VT100_ANS,
		(unsigned int)(sizeof(VT100_ANS) - 1));
	    break;

	case CSI_SGR:	    /* 8.3.118: (0) SELECT GRAPHIC RENDITION */
	    rxvt_process_sgr_mode(r, page, nargs, arg);
	    break;

	case CSI_DSR:	    /* 8.3.36: (0) DEVICE STATUS REPORT */
	    switch (arg[0])
	    {
		case 5:		/* DSR requested */
		    rxvt_tt_printf(r, page, "\033[0n");
		    break;
		case 6:		/* CPR requested */
		    rxvt_scr_report_position(r, page);
		    break;
#if defined (ENABLE_DISPLAY_ANSWER)
		case 7:		/* unofficial extension */
		    rxvt_tt_printf(r, page, "%-.250s\n",
			r->h->rs[Rs_display_name]);
		    break;
#endif
		case 8:		/* unofficial extension */
		    rxvt_xterm_seq(r, page, XTerm_title, APL_NAME "-" VERSION,
			CHAR_ST);
		    break;
	    }	/* switch (arg[0]) */
	    break;

	case CSI_TBC:	    /* 8.3.155: (0) TABULATION CLEAR */
	    switch (arg[0])
	    {
		case 0:		/* char tab stop cleared at active position */
		    rxvt_scr_set_tab(r, page, 0);
		    break;
		/* case 1: */	    /* line tab stop cleared in active line */
		/* case 2: */	    /* char tab stops cleared in active line */
		case 3:		/* all char tab stops are cleared */
		/* case 4: */	    /* all line tab stops are cleared */
		case 5:		/* all tab stops are cleared */
		    rxvt_scr_set_tab(r, page, -1);
		    break;
	    }	/* switch (arg[0]) */
	    break;

	case CSI_CTC:	    /* 8.3.17: (0) CURSOR TABULATION CONTROL */
	    switch (arg[0])
	    {
		case 0:		/* char tab stop set at active position */
		    rxvt_scr_set_tab(r, page, 1);
		    break;	/* = ESC H */
		/* case 1: */	    /* line tab stop set at active line */
		case 2:		/* char tab stop cleared at active position */
		    rxvt_scr_set_tab(r, page, 0);
		    break;	/* = ESC [ 0 g */
		/* case 3: */	    /* line tab stop cleared at active line */
		/* case 4: */	    /* char tab stops cleared at active line */
		case 5:		/* all char tab stops are cleared */
		    rxvt_scr_set_tab(r, page, -1);
		    break;	/* = ESC [ 3 g */
		/* case 6: */	    /* all line tab stops are cleared */
	    }	/* switch (arg[0]) */
	    break;

	case CSI_RM:	    /* 8.3.107: RESET MODE */
	    if (arg[0] == 4)
		rxvt_scr_insert_mode(r, page, 0);
	    break;

	case CSI_SM:	    /* 8.3.126: SET MODE */
	    if (arg[0] == 4)
		rxvt_scr_insert_mode(r, page, 1);
	    break;

	/* PRIVATE USE beyond this point.  All CSI_7? seqeunces here */ 
	case CSI_72:	    /* DECSTBM: set top and bottom margins */
	    if (nargs == 1)
		rxvt_scr_scroll_region(r, page, arg[0] - 1, MAX_ROWS - 1);
	    else if (nargs == 0 || arg[0] >= arg[1])
		rxvt_scr_scroll_region(r, page, 0, MAX_ROWS - 1);
	    else 
		rxvt_scr_scroll_region(r, page, arg[0] - 1, arg[1] - 1);
	    break;

	case CSI_73:
	    rxvt_scr_cursor(r, page, SAVE);
	    break;
	case CSI_75:
	    rxvt_scr_cursor(r, page, RESTORE);
	    break;

#ifndef NO_FRILLS
	case CSI_74:
	    rxvt_process_window_ops(r, page, arg, nargs);
	    break;
#endif

	case CSI_78:	    /* DECREQTPARM */
	    if (arg[0] == 0 || arg[0] == 1)
		rxvt_tt_printf(r, page, "\033[%d;1;1;112;112;1;0x",
		    arg[0] + 2);
	    /* FALLTHROUGH */
	default:
	    break;
    }
}