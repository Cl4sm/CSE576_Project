process_escape_seq(void)
{
    unsigned char   ch = cmd_getc();

    switch (ch) {
    /* case 1:        do_tek_mode (); break; */
    case '#':
	if (cmd_getc() == '8')
	    scr_E();
	break;
    case '(':
	scr_charset_set(0, cmd_getc());
	break;
    case ')':
	scr_charset_set(1, cmd_getc());
	break;
    case '*':
	scr_charset_set(2, cmd_getc());
	break;
    case '+':
	scr_charset_set(3, cmd_getc());
	break;
#ifdef MULTICHAR_SET
    case '$':
	scr_charset_set(-2, cmd_getc());
	break;
#endif
    case '7':
	scr_cursor(SAVE);
	break;
    case '8':
	scr_cursor(RESTORE);
	break;
    case '=':
    case '>':
	PrivMode((ch == '='), PrivMode_aplKP);
	break;
    case '@':
	(void)cmd_getc();
	break;
    case 'D':
	scr_index(UP);
	break;
    case 'E':
	scr_add_lines((unsigned char *) "\n\r", 1, 2);
	break;
    case 'G':
	process_graphics();
	break;
    case 'H':
	scr_set_tab(1);
	break;
    case 'M':
	scr_index(DN);
	break;
    /*case 'N': scr_single_shift (2);   break; */
    /*case 'O': scr_single_shift (3);   break; */
    case 'Z':
	tt_printf((unsigned char *) ESCZ_ANSWER);
	break;			/* steal obsolete ESC [ c */
    case '[':
	process_csi_seq();
	break;
    case ']':
	process_xterm_seq();
	break;
    case 'c':
	scr_poweron();
	break;
    case 'n':
	scr_charset_choose(2);
	break;
    case 'o':
	scr_charset_choose(3);
	break;
    }
}
