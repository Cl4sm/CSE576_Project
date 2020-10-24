void
main_loop(void)
{
    int             ch;
    do {
	while ((ch = cmd_getc()) == 0) ;	/* wait for something */
	/* fprintf( stderr, "%d: cmd_getc returned 0x%x(%c)\n", ++i, ch, isprint(ch)?ch:' ' ); */
	if (ch >= ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
	/* Read a text string from the input buffer */
	    int             nlines = 0;
	    unsigned char  *str;

	/*
	 * point to the start of the string,
	 * decrement first since it was post incremented in cmd_getc()
	 */
	    str = --cmdbuf_ptr;
	    while (cmdbuf_ptr < cmdbuf_endp) 
		{
			ch = *cmdbuf_ptr++;
			/* fprintf( stderr, "%d: cmd_getc returned 0x%x(%c)\n", ++i, ch, isprint(ch)?ch:' ' ); */
			if (ch >= ' ' || ch == '\t' ) 
			{
			/* nothing */
			} else if (ch == '\r') 
			{                  /* we better flush that stuff to create an 
								* impression that there is something going on */
				if( cmdbuf_ptr >= cmdbuf_endp || 
					(*cmdbuf_ptr != '\n'  && *cmdbuf_ptr != '\r')) 
					break;
			} else if (ch == '\n') 
			{
		    	nlines++;
		    	if (++refresh_count >= (refresh_limit * (TermWin.nrow - 1)))
				break;
			} else {	/* unprintable */
		    	cmdbuf_ptr--;
		    	break;
			}
	    }
	    scr_add_lines(str, nlines, (cmdbuf_ptr - str));
		if( ch == '\r' ) 
		{	
			scr_refresh(refresh_type);
		}
	} else {
	    switch (ch) {
	    case 005:		/* terminal Status */
		tt_printf((unsigned char *) VT100_ANS);
		break;
	    case 007:		/* bell */
		scr_bell();
		break;
	    case '\b':		/* backspace */
		scr_backspace();
		break;
	    case 013:		/* vertical tab, form feed */
	    case 014:
		scr_index(UP);
		break;
	    case 016:		/* shift out - acs */
		scr_charset_choose(1);
		break;
	    case 017:		/* shift in - acs */
		scr_charset_choose(0);
		break;
	    case 033:		/* escape char */
		process_escape_seq();
		break;
	    }
	}
    } while (ch != EOF);
}
