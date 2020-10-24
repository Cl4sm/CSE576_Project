void*
ma_memmove(void *d, const void *s, size_t len)
{
    unsigned int    i;
    unsigned int*   pdst;
    unsigned int*   psrc;
    unsigned char*  dst = (unsigned char *)d;
    unsigned char*  src = (unsigned char *)s;

    if (len && d != s) {
	if ((unsigned int)d < (unsigned int)s) {
	    /* forwards */
	    i = (-(unsigned int)dst) & (sizeof (unsigned int*) - 1);
	    if (len >= 16 &&
		i == ((-(unsigned int)src) & (SIZEOF_INT_P - 1))) {
	        /* speed up since src & dst are offset correctly */
		len -= (size_t)i;
		for ( ; i--; )
		    *dst++ = *src++;
		/* assign the src/dst to psrc/pdst */
		pdst = (unsigned int*) dst;
		psrc = (unsigned int*) src;
		for (i = (unsigned int)(len / sizeof (unsigned int*)); i--; )
		    *pdst++ = *psrc++;
		len &= (SIZEOF_INT_P - 1);
		/* assign back the src/dst */
		dst = (unsigned char*) pdst;
		src = (unsigned char*) psrc;
	    }
	    /* the left bytes */
	    for ( ; len--; )
		*dst++ = *src++;
	}
	else {
	    /* backwards */
	    dst += len;
	    src += len;
	    i = ((unsigned int)dst) & (sizeof (unsigned int*) - 1);
	    if (len >= 16 &&
		i == (((unsigned int)src) & (SIZEOF_INT_P - 1))) {
	        /* speed up since src & dst are offset correctly */
		len -= (size_t)i;
		for ( ; i--; )
		    *--dst = *--src;
		/* assign the src/dst to psrc/pdst */
		pdst = (unsigned int*) dst;
		psrc = (unsigned int*) src;
		for (i = (unsigned int)(len / sizeof (unsigned int*)); i--; )
		    *--pdst = *--psrc;
		len &= (SIZEOF_INT_P - 1);
		/* assign back the src/dst */
		dst = (unsigned char*) pdst;
		src = (unsigned char*) psrc;
	    }
	    /* the left bytes */
	    for ( ; len--; )
		*--dst = *--src;
	}
    }
    return d;
}