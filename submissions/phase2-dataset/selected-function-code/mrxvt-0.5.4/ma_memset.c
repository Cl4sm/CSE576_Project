void*
ma_memset(void *p, int c1, size_t len)
{
    unsigned int        i, val;
    unsigned int*	    pdst;
    unsigned char   c = (unsigned char) c1;
    unsigned char  *lp = (unsigned char *) p;

    if (len) {
	if (len >= 16) {
	    /*
	    ** < 16 probably not worth all the calculations
	    ** write out preceding characters so we align on an
	    ** integer boundary
	    */
	    if ((i = ((-(unsigned int)p) & (SIZEOF_INT_P - 1)))) {
		len -= (size_t)i;
		for (; i--;)
		    *lp++ = c;
	    }

	    /* do the fast writing */
	    val = (c << 8) + c;
#if SIZEOF_INT_P >= 4
	    val |= (val << 16);
#endif
#if SIZEOF_INT_P >= 8
	    val |= (val << 32);
#endif
#if SIZEOF_INT_P == 16
	    val |= (val << 64);
#endif
	    /* assign the lp to pdst */
	    pdst = (unsigned int*) lp;
	    for (i = (unsigned int)(len / SIZEOF_INT_P); i--;)
		*pdst++ = val;
	    len &= (SIZEOF_INT_P - 1);
	    /* assign back the pdst to lp */
	    lp = (unsigned char*) pdst;
	}
	/* write trailing characters */
	for (; len--;)
	    *lp++ = c;
    }
    return p;
}