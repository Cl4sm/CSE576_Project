void
rxvt_process_osc_seq (rxvt_t* r, int page)
{
    int		    readpage = page;
    unsigned char   ch, eh, *s;
    int		    arg;


    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "rxvt_process_osc_seq( r, %d). Active page %d\n", page, ATAB(r)));

    arg = 0;
    for(;;)
    {
	ch = rxvt_cmd_getc(r, &readpage);
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


    if (ch == ';')
    {
	s = rxvt_get_to_st(r, page, &eh);
	if (s)
	{
	    /*
	     * rxvt_menubar_dispatch() violates the constness of the string, so
	     * do it here.
	     */
	    if (arg == XTerm_Menu)
	    {
		/* XXX: currently disabled due to security concerns */
#if 0
		rxvt_menubar_dispatch(r, (char*) s);
#endif
	    }
	    else
		rxvt_xterm_seq(r, page, arg, (char*) s, eh);

	    rxvt_free(s);
	}
    }
}