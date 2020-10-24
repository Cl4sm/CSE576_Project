displayable_unival(unsigned unival, iconv_t conv_out)
{
	/* First rule out control characters */
	if((unival >= 0x00 && unival < 0x20) ||
	   (unival >= 0x80 && unival < 0xA0) ||
	   (unival == 0x7F) ||
	   (unival >= 0xFFF0 && unival <= 0xFFFF))
		return 0;
	
	/* Range 0x80..0x9F is used in some character sets (such as cp850),
	 * but they are assigned different positions in unicode.
	 * The univals we handle here are unicode positions.
	 * In unicode, 0x80..0x9F are not used because some
	 * american programs might still blindly assume
	 * 7-bitness and take those as control characters.
	 * 0x7F is delete/backspace.
	 * 0xFFF0..0xFFFF is the unicode control range.
	 * It contains a signature token, an illegal
	 * character token and so on.
	 */

#ifdef HAVE_ICONV_OPEN
	if (conv_out)
	{
		u_char utfbuf[8],*utfptr;
		u_char outbuf[256],*outptr;
		size_t utfspace, outspace;
		size_t retval;

		/* Now sequence the character to buffer
		 * and let iconv say whether it can displayed.
		 */
		utf8_sequence(unival, utfbuf);
		
		utfptr = utfbuf;
		outptr = outbuf;
		utfspace = my_strlen(utfbuf);
		outspace = sizeof outbuf;
		
		/* reset the converter */
		iconv(conv_out, NULL, 0, NULL, 0);
		
		/*	*outptr = '\0'; */
		retval = iconv(conv_out,
		               (iconv_const char**)(void *)&utfptr, &utfspace,
		               (char **)(void *)&outptr, &outspace);

		/*
		*outptr = '\0';
		fprintf(stderr, "CHK: '%s' -> '%s', retval=%d, errno=%d\n",
			utfbuf, outbuf,
			retval, errno);
		*/
		return retval != (size_t)-1;
	}
#endif /* HAVE_ICONV_OPEN */
	return 1;
}
