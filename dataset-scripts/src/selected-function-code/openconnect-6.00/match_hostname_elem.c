static int match_hostname_elem(const char *hostname, int helem_len,
			       const char *match, int melem_len)
{
	if (!helem_len && !melem_len)
		return 0;

	if (!helem_len || !melem_len)
		return -1;


	if (match[0] == '*') {
		int i;

		for (i = 1 ; i <= helem_len; i++) {
			if (!match_hostname_elem(hostname + i, helem_len - i,
						 match + 1, melem_len - 1))
				return 0;
		}
		return -1;
	}

	/* From the NetBSD (5.1) man page for ctype(3):
	   Values of type char or signed char must first be cast to unsigned char,
	   to ensure that the values are within the correct range.  The result
	   should then be cast to int to avoid warnings from some compilers.
	   We do indeed get warning "array subscript has type 'char'" without
	   the casts. Ick. */
	if (toupper((int)(unsigned char)hostname[0]) ==
	    toupper((int)(unsigned char)match[0]))
		return match_hostname_elem(hostname + 1, helem_len - 1,
					   match + 1, melem_len - 1);

	return -1;
}