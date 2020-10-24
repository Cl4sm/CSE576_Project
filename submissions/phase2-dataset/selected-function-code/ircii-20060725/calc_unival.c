calc_unival(const u_char *utfbuf)
{
	/* This function does the reverse of utf8_sequence(). */
	switch (calc_unival_length(utfbuf))
	{
		case 1:
		default:
			return ((utfbuf[0] & 127));
		case 2:
			return ((utfbuf[0] & 31) << 6)
			     | ((utfbuf[1] & 63));
		case 3:
			return ((utfbuf[0] & 15) << 12)
			     | ((utfbuf[1] & 63) << 6)
			     | ((utfbuf[2] & 63));
		case 4:
			return ((utfbuf[0] &  7) << 16)
			     | ((utfbuf[1] & 63) << 12)
			     | ((utfbuf[2] & 63) << 6)
			     | ((utfbuf[3] & 63));
	}
}
