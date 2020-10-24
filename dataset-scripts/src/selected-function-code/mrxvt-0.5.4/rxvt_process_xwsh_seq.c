void
rxvt_process_xwsh_seq (rxvt_t* r, int page)
{
    int		    readpage = page;
    unsigned char   ch, string[STRING_MAX];
    int             arg;


    arg = 0;
    for(;;)
    {
	ch = rxvt_cmd_getc (r, &readpage);
	if( readpage == -1 )
	{
	    rxvt_set_escfail( r, page, 1 );
	    return;
	}

	if( isdigit(ch) )
	    arg = arg * 10 + (ch - '0');
	else
	    break;
    }

    if (ch == '.')
    {
	ch = rxvt_cmd_getc(r, &readpage);
	if( readpage == -1 )
	{
	    rxvt_set_escfail( r, page, 1 );
	    return;
	}

	if ( ch == 'y')
	{
	    int             n = 0;

	    for(;;)
	    {
		ch = rxvt_cmd_getc( r, &readpage );
		if( readpage == -1 )
		{
		    rxvt_set_escfail( r, page, 1 );
		    return;
		}

		if( ch == '\033' )
		    break;

		if (ch)
		{
		    if (ch == '\t')
			ch = ' ';   /* translate '\t' to space */
		    else if (ch < ' ')
			return;	/* control character - exit */

		    if (n < sizeof(string) - 1)
			string[n++] = ch;
		}
	    }

	    ch = rxvt_cmd_getc( r, &readpage );
	    if( readpage == -1 )
	    {
		rxvt_set_escfail( r, page, 1 );
		return;
	    }

	    if( ch == '\\' )
	    {
		string[n] = '\0';
		rxvt_xwsh_seq(r, arg, (char*) string);
	    }
	} /* if( ch == 'y' ) */
    } /* if( ch == '.' ) */
}