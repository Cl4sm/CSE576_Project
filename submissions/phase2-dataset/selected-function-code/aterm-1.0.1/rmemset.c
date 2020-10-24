rmemset(void *p, unsigned char c, R_int_p_t len)
{
    R_u_int_p_t     i, val, *rp;
    unsigned char  *lp;

    if (len < 16)		/* probably not worth all the calculations */
	lp = p;
    else {
/* write out preceding characters so we align on an integer boundary */
	if ((i = ((-(R_u_int_p_t)p) & (SIZEOF_INT_P - 1))) == 0)
	    rp = p;
	else {
	    len -= i;
	    for (lp = p; i--;)
		*lp++ = c;
	    rp = (R_int_p_t *)lp;
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
	for (i = len / SIZEOF_INT_P; i--;)
	    *rp++ = val;
	len &= (SIZEOF_INT_P - 1);
	lp = (unsigned char *)rp;
    }
/* write trailing characters */
    for (; len--;)
	*lp++ = c;
}
