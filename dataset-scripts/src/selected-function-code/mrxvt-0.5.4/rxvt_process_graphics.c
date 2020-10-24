void
rxvt_process_graphics(rxvt_t* r, int page)
{
    int		    readpage = page;
    unsigned char   ch, cmd = rxvt_cmd_getc(r, &readpage);

    if( readpage == -1 )
    {
	rxvt_set_escfail( r, page, 1 );
	return;
    }

    if (cmd == 'Q')		    /* query graphics */
    {
	rxvt_tt_printf(r, page, "\033G0\n");	/* no graphics */
	return;
    }

    /* swallow other graphics sequences until terminating ':' */
    do
      {
	ch = rxvt_cmd_getc(r, &readpage);
	if( readpage == -1 )
	{
	    rxvt_set_escfail( r, page, 1 );
	    return;
	}
      }
    while( ch != ':' && ch >= ' ' && ch <= '~' ); /* 2007-01-06 gi1242: In case
						     the escape sequence is
						     malformed, swallow only
						     printable characters. */
}