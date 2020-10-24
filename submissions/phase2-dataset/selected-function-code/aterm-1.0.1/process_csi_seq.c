void
process_csi_seq(void)
{
    unsigned char   ch, priv;
    unsigned int    nargs;
    int             arg[ESC_ARGS];

    nargs = 0;
    arg[0] = 0;
    arg[1] = 0;

    priv = 0;
    ch = cmd_getc();
    if (ch >= '<' && ch <= '?') {
	priv = ch;
	ch = cmd_getc();
    }
/* read any numerical arguments */
    do {
	int             n;

	for (n = 0; isdigit(ch); ch = cmd_getc())
	    n = n * 10 + (ch - '0');

	if (nargs < ESC_ARGS)
	    arg[nargs++] = n;
	if (ch == '\b') {
	    scr_backspace();
	} else if (ch == 033) {
	    process_escape_seq();
	    return;
	} else if (ch < ' ') {
	    scr_add_lines(&ch, 0, 1);
	    return;
	}
	if (ch < '@')
	    ch = cmd_getc();
    }
    while (ch >= ' ' && ch < '@');
    if (ch == 033) {
	process_escape_seq();
	return;
    } else if (ch < ' ')
	return;

    switch (ch) {
#ifdef PRINTPIPE
    case 'i':			/* printing */
	switch (arg[0]) {
	case 0:
	    scr_printscreen(0);
	    break;
	case 5:
	    process_print_pipe();
	    break;
	}
	break;
#endif
    case 'A':
    case 'e':			/* up <n> */
	scr_gotorc((arg[0] ? -arg[0] : -1), 0, RELATIVE);
	break;
    case 'B':			/* down <n> */
	scr_gotorc((arg[0] ? +arg[0] : +1), 0, RELATIVE);
	break;
    case 'C':
    case 'a':			/* right <n> */
	scr_gotorc(0, (arg[0] ? +arg[0] : +1), RELATIVE);
	break;
    case 'D':			/* left <n> */
	scr_gotorc(0, (arg[0] ? -arg[0] : -1), RELATIVE);
	break;
    case 'E':			/* down <n> & to first column */
	scr_gotorc((arg[0] ? +arg[0] : +1), 0, R_RELATIVE);
	break;
    case 'F':			/* up <n> & to first column */
	scr_gotorc((arg[0] ? -arg[0] : -1), 0, R_RELATIVE);
	break;
    case 'G':
    case '`':			/* move to col <n> */
	scr_gotorc(0, (arg[0] ? arg[0] - 1 : +1), R_RELATIVE);
	break;
    case 'd':			/* move to row <n> */
	scr_gotorc((arg[0] ? arg[0] - 1 : +1), 0, C_RELATIVE);
	break;
    case 'H':
    case 'f':			/* position cursor */
	switch (nargs) {
	case 0:
	    scr_gotorc(0, 0, 0);
	    break;
	case 1:
	    scr_gotorc((arg[0] ? arg[0] - 1 : 0), 0, 0);
	    break;
	default:
	    scr_gotorc(arg[0] - 1, arg[1] - 1, 0);
	    break;
	}
	break;
    case 'I':
	scr_tab(arg[0] ? +arg[0] : +1);
	break;
    case 'Z':
	scr_tab(arg[0] ? -arg[0] : -1);
	break;
    case 'J':
	scr_erase_screen(arg[0]);
	break;
    case 'K':
	scr_erase_line(arg[0]);
	break;
    case '@':
	scr_insdel_chars((arg[0] ? arg[0] : 1), INSERT);
	break;
    case 'L':
	scr_insdel_lines((arg[0] ? arg[0] : 1), INSERT);
	break;
    case 'M':
	scr_insdel_lines((arg[0] ? arg[0] : 1), DELETE);
	break;
    case 'X':
	scr_insdel_chars((arg[0] ? arg[0] : 1), ERASE);
	break;
    case 'P':
	scr_insdel_chars((arg[0] ? arg[0] : 1), DELETE);
	break;

    case 'c':
	tt_printf((unsigned char *) VT100_ANS);
	break;
    case 'm':
	process_sgr_mode(nargs, arg);
	break;
    case 'n':			/* request for information */
	switch (arg[0]) {
	case 5:
	    tt_printf((unsigned char *) "\033[0n");
	    break;		/* ready */
	case 6:
	    scr_report_position();
	    break;
#if defined (ENABLE_DISPLAY_ANSWER)
	case 7:
		if( strlen(display_name)  < TT_PRINTF_LIMIT-2 )
	    	tt_printf((unsigned char *) "%s\n", display_name);
	    break;
#endif
	case 8:
	    xterm_seq(XTerm_title, APL_NAME "-" VERSION);
	    break;
	}
	break;
    case 'r':			/* set top and bottom margins */
	if (priv != '?') {
	    if (nargs < 2 || arg[0] >= arg[1])
		scr_scroll_region(0, 10000);
	    else
		scr_scroll_region(arg[0] - 1, arg[1] - 1);
	    break;
	}
    /* drop */
    case 's':
    case 't':
		if(arg[0] == 21)
			tt_printf((unsigned char *) "\033]l%s\033\\", rs_title);
        break;
    case 'h':
    case 'l':
	process_terminal_mode(ch, priv, nargs, arg);
	break;
    case 'g':
	switch (arg[0]) {
	case 0:
	    scr_set_tab(0);
	    break;		/* delete tab */
	case 3:
	    scr_set_tab(-1);
	    break;		/* clear all tabs */
	}
	break;
    case 'W':
	switch (arg[0]) {
	case 0:
	    scr_set_tab(1);
	    break;		/* = ESC H */
	case 2:
	    scr_set_tab(0);
	    break;		/* = ESC [ 0 g */
	case 5:
	    scr_set_tab(-1);
	    break;		/* = ESC [ 3 g */
	}
	break;
    }
}
