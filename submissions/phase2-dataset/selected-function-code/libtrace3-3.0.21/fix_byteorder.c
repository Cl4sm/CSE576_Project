static bitbuffer_t fix_byteorder(bitbuffer_t value,
		enum byte_order_t order, uint64_t size)
{
    bitbuffer_t one = 1;
    bitbuffer_t lhs;
    bitbuffer_t rhs;;

    /*
     * XXX trial and error seems to show these numbers to work.
     * I've tried fields of length 1,2,3,4,8,13,16,32 and they seem to work.
     * Others are untested...
     */
    switch(order)
    {
	case BIGENDIAN: 
	    if(size < 16)
		return value;
	    if(size < 32)
		return ntohs(value);
	    if(size <= 32)
		return ntohl(value);
	    
	    lhs = ntohl(value& ((one<<32)-1));
	    rhs = ntohl(value >> 32);
	    return ((lhs<<32) | rhs);

	case LITTLEENDIAN: 
	    return value;

    };

    /* should never get here */
    assert(0);
    return 0;
}