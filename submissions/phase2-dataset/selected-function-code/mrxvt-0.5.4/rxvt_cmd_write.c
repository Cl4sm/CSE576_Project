void
rxvt_cmd_write( rxvt_t* r, int page, const unsigned char *str,
	unsigned int count)
{
    unsigned int    n, s;
    unsigned char   *cmdbuf_base = PVTS(r, page)->cmdbuf_base,
                    *cmdbuf_endp = PVTS(r, page)->cmdbuf_endp,
                    *cmdbuf_ptr	= PVTS(r, page)->cmdbuf_escstart ? 
			    PVTS(r, page)->cmdbuf_escstart :
			    PVTS(r, page)->cmdbuf_ptr;

    n = cmdbuf_ptr - cmdbuf_base;
    s = cmdbuf_base + (BUFSIZ - 1) - cmdbuf_endp;

    /*
     * If there is not enough space to write our data, try using the unused
     * space in the beginning.
     */
    if (n > 0 && s < count)
    {
	MEMMOVE(cmdbuf_base, cmdbuf_ptr,
	    (unsigned int)(cmdbuf_endp - cmdbuf_ptr));

	cmdbuf_ptr  -= n;
	cmdbuf_endp -= n;
	s += n;

	if( PVTS(r, page)->cmdbuf_escstart )
	    PVTS(r, page)->cmdbuf_escstart -= n;
	if( PVTS(r, page)->cmdbuf_escfail )
	    PVTS(r, page)->cmdbuf_escfail -= n;

	PVTS(r, page)->cmdbuf_ptr = cmdbuf_ptr;
    }

    if (count > s)
    {
	rxvt_msg (DBG_ERROR, DBG_COMMAND, "data loss: cmd_write too large");
	count = s;
    }

    for (; count--;)
	*cmdbuf_endp++ = *str++;

    PVTS(r, page)->cmdbuf_endp = cmdbuf_endp;

    assert (PVTS(r, page)->cmdbuf_base <= PVTS(r, page)->cmdbuf_endp);
}