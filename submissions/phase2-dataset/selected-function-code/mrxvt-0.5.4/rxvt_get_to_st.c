unsigned char  *
rxvt_get_to_st(rxvt_t* r, int page, unsigned char *ends_how)
{
    int		    readpage = page;
    int		    seen_esc = 0;   /* seen escape? */
    unsigned int    n = 0;
    unsigned char*  s;
    unsigned char   ch, string[STRING_MAX];


    for(;;)
    {
	ch = rxvt_cmd_getc( r, &readpage );
	if( readpage == -1 )
	{
	    rxvt_set_escfail( r, page, 1 );
	    return NULL;
	}

	if( !ch )
	    break;

	if(
	     ch == C0_BEL || ch == CHAR_ST ||
	     (ch == 0x5c && seen_esc)	/* 7bit ST */
	  )
	    break;

	if( ch == C0_ESC )
	{
	    seen_esc = 1;
	    continue;
	}
	else if (ch == '\t')
	    ch = ' ';	/* translate '\t' to space */
	else if (ch < 0x08 || (ch > 0x0d && ch < 0x20))
	{
#if 0
	    /*
	     * NO! See http://vt100.net/emu/dec_ansi_parser for reason. I am not
	     * sure what to do with CAN or SUB, but the rest should be *ignored*
	     * - I will try to find out what to do with CAN and SUB. - Johann
	     * 'Mykraverk' Oskarsson <johann@myrkraverk.com>
	     */
	    return NULL;    /* other control character - exit */
#endif

	    continue; /* do nothing */
	}

	if (n < sizeof(string) - 1)
	    string[n++] = ch;

	seen_esc = 0;
    } /* for(;;) */

    string[n++] = '\0';
    if (IS_NULL(s = (unsigned char UNTAINTED *) STRNDUP (string, n)))
	return NULL;

    *ends_how = (ch == 0x5c ? C0_ESC : ch);

    return s;
}