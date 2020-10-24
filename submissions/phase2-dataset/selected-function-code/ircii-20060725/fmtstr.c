fmtstr(char *buffer, size_t *currlen, size_t maxlen, char *value,
	int min, int max, int flags)
{
	int	padlen, strln;	/* amount to pad */
	int	cnt = 0;

	if (value == 0) {
		value = "<NULL>";
	}
	for (strln = 0; value[strln]; ++strln)
		;	/* strlen */
	padlen = min - strln;
	if (padlen < 0)
		padlen = 0;
	if (flags & DP_F_MINUS)
		padlen = -padlen;	/* Left Justify */

	while ((padlen > 0) && (cnt < max)) {
		dopr_outch(buffer, currlen, maxlen, ' ');
		--padlen;
		++cnt;
	}
	while (*value && (cnt < max)) {
		dopr_outch(buffer, currlen, maxlen, *value++);
		++cnt;
	}
	while ((padlen < 0) && (cnt < max)) {
		dopr_outch(buffer, currlen, maxlen, ' ');
		++padlen;
		++cnt;
	}
}
