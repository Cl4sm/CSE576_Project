void
rxvt_set_escfail( rxvt_t *r, int page, int nchars )
{
    assert( PVTS(r, page)->cmdbuf_escstart );

    rxvt_check_cmdbuf( r, page );
    PVTS(r, page)->cmdbuf_escfail = PVTS(r, page)->cmdbuf_ptr + nchars - 1;

    if( PVTS(r, page)->cmdbuf_escfail > PVTS(r, page)->cmdbuf_base + BUFSIZ-3 )
    {
	/*
	 * Escape sequence was longer than BUFSIZ. Just skip the escape
	 * character and go on like normal
	 */
	PVTS(r, page)->cmdbuf_ptr = PVTS(r, page)->cmdbuf_escstart + 1;
	SET_NULL( PVTS(r, page)->cmdbuf_escstart );
	SET_NULL( PVTS(r, page)->cmdbuf_escfail );
    }

    else
	/* Fall back to start of escape sequence */
	PVTS(r, page)->cmdbuf_ptr = PVTS(r, page)->cmdbuf_escstart;
}