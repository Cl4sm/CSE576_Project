void
rxvt_check_cmdbuf (rxvt_t* r, int page)
{
    assert( PVTS(r, page)->cmdbuf_base <= PVTS(r, page)->cmdbuf_endp );

    if(
	  IS_NULL( PVTS(r, page)->cmdbuf_escstart )		    &&
	  PVTS(r, page)->cmdbuf_ptr == PVTS(r, page)->cmdbuf_endp
      )
    {
	/*
	 * If there is no data in the buffer, reset it to the beginning
	 * of the buffer.
	 */
	PVTS(r, page)->cmdbuf_ptr   = PVTS(r, page)->cmdbuf_endp
				    = PVTS(r, page)->cmdbuf_base;

    }

    else if(
	     (PVTS(r, page)->cmdbuf_endp - PVTS(r, page)->cmdbuf_base)
		== (BUFSIZ-1)						 &&
	     (
	       PVTS(r, page)->cmdbuf_escstart ?
		(PVTS(r, page)->cmdbuf_escstart > PVTS(r,page)->cmdbuf_base) :
		(PVTS(r, page)->cmdbuf_ptr > PVTS(r, page)->cmdbuf_base)
	     )
	   )
    {
	/*
	 * If there is space at beginning of the buffer, but not space at the
	 * end of the buffer, move the content of buffer forward to free space
	 */
	unsigned char	*start;
	unsigned int	n, len;

	start = PVTS(r, page)->cmdbuf_escstart ?
	    PVTS(r, page)->cmdbuf_escstart : PVTS(r, page)->cmdbuf_ptr;


	n   = start - PVTS(r, page)->cmdbuf_base;
	len = PVTS(r, page)->cmdbuf_endp - start;

	assert( n == BUFSIZ - 1 - len );
	assert( start < PVTS(r, page)->cmdbuf_endp );

	MEMMOVE( PVTS(r, page)->cmdbuf_base, start, len );

	PVTS(r, page)->cmdbuf_ptr   -= n;
	PVTS(r, page)->cmdbuf_endp  -= n;
	if( PVTS(r, page)->cmdbuf_escstart )
	    PVTS(r, page)->cmdbuf_escstart -= n;
	if( PVTS(r, page)->cmdbuf_escfail )
	    PVTS(r, page)->cmdbuf_escfail -= n;
    }
}