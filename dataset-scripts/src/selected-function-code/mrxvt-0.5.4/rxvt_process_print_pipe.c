void
rxvt_process_print_pipe( rxvt_t* r, int page )
{
    rxvt_msg (DBG_ERROR, DBG_COMMAND,  "Print pipe not implemented in this version" );
#if 0 /* {{{ Disabled because failures of rxvt_cmd_getc() can't be handled */
    int		readpage = page;
    int		done;
    FILE*	fd;


    if (IS_NULL(fd = rxvt_popen_printer(r, NULL)))
	return;

    /*
    ** Send all input to the printer until either ESC[4i or ESC[?4i
    ** is received.
    */
    for (done = 0; !done;)
    {
	unsigned char   buf[8];
	unsigned char   ch;
	unsigned int	i, len;

	if ((ch = rxvt_cmd_getc(r, &readpage)) != C0_ESC)
	{
	    assert (readpage == page);
	    assert (checksum == PVTS(r, page)->checksum);
	    if (putc(ch, fd) == EOF)
		break;	    /* done = 1 */
	}
	else
	{
	    len = 0;
	    buf[len++] = ch;

	    if ((buf[len++] = rxvt_cmd_getc(r, &readpage)) == '[')
	    {
		assert (page == readpage);
		assert (checksum == PVTS(r, page)->checksum);
		if ((ch = rxvt_cmd_getc(r, &readpage)) == '?')
		{
		    assert (page == readpage);
		    assert (checksum == PVTS(r, page)->checksum);
		    buf[len++] = '?';
		    ch = rxvt_cmd_getc(r, &readpage);
		    assert (page == readpage);
		    assert (checksum == PVTS(r, page)->checksum);
		}
		if ((buf[len++] = ch) == '4')
		{
		    if ((buf[len++]=rxvt_cmd_getc(r, &readpage))=='i')
		    {
			assert (page == readpage);
			assert (checksum == PVTS(r, page)->checksum);
			break;	/* done = 1 */
		    }
		}
	    }

	    for (i = 0; i < len; i++)
	    {
		if (putc(buf[i], fd) == EOF)
		{
		    done = 1;
		    break;
		}
	    }
	}
    }

    rxvt_pclose_printer(fd);
#endif /*}}}*/
}