static void
fmtint(char *buffer, size_t *currlen, size_t maxlen, LLONG value, int base,
	int min, int max, int flags)
{
	int		signvalue = 0;
	unsigned LLONG	uvalue;
	char		convert[20];
	int		place = 0;
	int		spadlen = 0;	/* amount to space pad */
	int		zpadlen = 0;	/* amount to zero pad */
	int		caps = 0;

	if (max < 0)
		max = 0;

	uvalue = value;

	if (!(flags & DP_F_UNSIGNED)) {
		if (value < 0) {
			signvalue = '-';
			uvalue = -value;
		} else if (flags & DP_F_PLUS)	/* Do a sign (+/i) */
			signvalue = '+';
		else if (flags & DP_F_SPACE)
			signvalue = ' ';
	}
	if (flags & DP_F_UP)
		caps = 1;	/* Should characters be upper case? */

	do {
		convert[place++] =
		    (caps ? "0123456789ABCDEF" : "0123456789abcdef")
		    [uvalue % (unsigned) base];
		uvalue = (uvalue / (unsigned) base);
	} while (uvalue && (place < 20));
	if (place == 20)
		place--;
	convert[place] = 0;

	zpadlen = max - place;
	spadlen = min - MAX(max, place) - (signvalue ? 1 : 0);
	if (zpadlen < 0)
		zpadlen = 0;
	if (spadlen < 0)
		spadlen = 0;
	if (flags & DP_F_ZERO) {
		zpadlen = MAX(zpadlen, spadlen);
		spadlen = 0;
	}
	if (flags & DP_F_MINUS)
		spadlen = -spadlen;	/* Left Justifty */

#ifdef DEBUG_SNPRINTF
	printf("zpad: %d, spad: %d, min: %d, max: %d, place: %d\n",
	    zpadlen, spadlen, min, max, place);
#endif

	/* Spaces */
	while (spadlen > 0) {
		dopr_outch(buffer, currlen, maxlen, ' ');
		--spadlen;
	}

	/* Sign */
	if (signvalue)
		dopr_outch(buffer, currlen, maxlen, signvalue);

	/* Zeros */
	if (zpadlen > 0) {
		while (zpadlen > 0) {
			dopr_outch(buffer, currlen, maxlen, '0');
			--zpadlen;
		}
	}
	/* Digits */
	while (place > 0)
		dopr_outch(buffer, currlen, maxlen, convert[--place]);

	/* Left Justified spaces */
	while (spadlen < 0) {
		dopr_outch(buffer, currlen, maxlen, ' ');
		++spadlen;
	}
}
