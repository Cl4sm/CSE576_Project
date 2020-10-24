void
utf8_sequence(unsigned unival, u_char* utfbuf)
{
	/* This function does the reverse of calc_unival(). */
	/* The output buffer should have 5 bytes of space.  */
	u_char *utfptr = utfbuf;
	if (unival < 0x80)                 /* <=7 bits */
		*utfptr++ = (u_char)unival;
	else
	{
		if (unival < 0x800)            /* <=11 bits */
			*utfptr++ = (u_char)(0xC0 + (unival>>6));
		else
		{
			if (unival < 0x10000)      /* <=16 bits */
				*utfptr++ = (u_char)(0xE0 + (unival>>12));
			else                      /* <=21 bits */
			{
				*utfptr++ = (u_char)(0xF0 + (unival>>18));
				*utfptr++ = (u_char)(0x80 + ((unival>>12)&63));
			}
			*utfptr++ = (u_char)(0x80 + ((unival>>6)&63));
		}
		*utfptr++ = (u_char)(0x80 + (unival&63));
	}
	/* Last put a zero-terminator. */
	*utfptr = '\0';
/*	
	fprintf(stderr, "utf8-seq %X: %02X %02X (%s)\n",
	    unival, utfbuf[0], utfbuf[1], utfbuf);
*/
}
