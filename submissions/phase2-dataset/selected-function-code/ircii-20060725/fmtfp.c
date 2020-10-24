fmtfp(char *buffer, size_t *currlen, size_t maxlen, LDOUBLE fvalue,
	int min, int max, int flags)
{
	int	signvalue = 0;
	LDOUBLE	ufvalue;
	char	iconvert[20];
	char	fconvert[20];
	int	iplace = 0;
	int	fplace = 0;
	int	padlen = 0;	/* amount to pad */
	int	zpadlen = 0;
	int	caps = 0;
	long	intpart;
	long	fracpart;

	/* AIX manpage says the default is 0, but Solaris says the default is
	 * 6, and sprintf on AIX defaults to 6 */
	if (max < 0)
		max = 6;

	ufvalue = abs_val(fvalue);

	if (fvalue < 0)
		signvalue = '-';
	else if (flags & DP_F_PLUS)	/* Do a sign (+/i) */
		signvalue = '+';
	else if (flags & DP_F_SPACE)
		signvalue = ' ';

#if 0
	if (flags & DP_F_UP)
		caps = 1;	/* Should characters be upper case? */
#endif

	intpart = (long) ufvalue;

	/* Sorry, we only support 9 digits past the decimal because of our
	 * conversion method */
	if (max > 9)
		max = 9;

	/* We "cheat" by converting the fractional part to integer by
	 * multiplying by a factor of 10 */
	fracpart = round((pow10(max)) * (ufvalue - intpart));

	if (fracpart >= pow10(max)) {
		intpart++;
		fracpart -= pow10(max);
	}
#ifdef DEBUG_SNPRINTF
	printf("fmtfp: %g %d.%d min=%d max=%d\n",
	    (double) fvalue, intpart, fracpart, min, max);
#endif

	/* Convert integer part */
	do {
		iconvert[iplace++] =
		    (caps ? "0123456789ABCDEF"
			  : "0123456789abcdef")[intpart % 10];
		intpart = (intpart / 10);
	} while (intpart && (iplace < 20));
	if (iplace == 20)
		iplace--;
	iconvert[iplace] = 0;

	/* Convert fractional part */
	do {
		fconvert[fplace++] =
		    (caps ? "0123456789ABCDEF"
			  : "0123456789abcdef")[fracpart % 10];
		fracpart = (fracpart / 10);
	} while (fracpart && (fplace < 20));
	if (fplace == 20)
		fplace--;
	fconvert[fplace] = 0;

	/* -1 for decimal point, another -1 if we are printing a sign */
	padlen = min - iplace - max - 1 - ((signvalue) ? 1 : 0);
	zpadlen = max - fplace;
	if (zpadlen < 0)
		zpadlen = 0;
	if (padlen < 0)
		padlen = 0;
	if (flags & DP_F_MINUS)
		padlen = -padlen;	/* Left Justifty */

	if ((flags & DP_F_ZERO) && (padlen > 0)) {
		if (signvalue) {
			dopr_outch(buffer, currlen, maxlen, signvalue);
			--padlen;
			signvalue = 0;
		}
		while (padlen > 0) {
			dopr_outch(buffer, currlen, maxlen, '0');
			--padlen;
		}
	}
	while (padlen > 0) {
		dopr_outch(buffer, currlen, maxlen, ' ');
		--padlen;
	}
	if (signvalue)
		dopr_outch(buffer, currlen, maxlen, signvalue);

	while (iplace > 0)
		dopr_outch(buffer, currlen, maxlen, iconvert[--iplace]);


#ifdef DEBUG_SNPRINTF
	printf("fmtfp: fplace=%d zpadlen=%d\n", fplace, zpadlen);
#endif

	/*
         * Decimal point.  This should probably use locale to find the correct
         * char to print out.
         */
	if (max > 0) {
		dopr_outch(buffer, currlen, maxlen, '.');

		while (fplace > 0)
			dopr_outch(buffer, currlen, maxlen, fconvert[--fplace]);
	}
	while (zpadlen > 0) {
		dopr_outch(buffer, currlen, maxlen, '0');
		--zpadlen;
	}

	while (padlen < 0) {
		dopr_outch(buffer, currlen, maxlen, ' ');
		++padlen;
	}
}
