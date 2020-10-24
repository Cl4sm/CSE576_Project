input_do_delete_chars(int n, int do_save_cut)
{
	u_char* buf  = current_screen->inputdata.buffer.buf;
	unsigned pos = current_screen->inputdata.buffer.pos;

	/* The usage of this function is identical to input_do_delete_raw(),
	 * but instead of bytes, n means the number of characters.
	 * Since characters may consist of 1-4 bytes, this
	 * function converts the number to bytes and then
	 * calls input_do_delete_raw().
	 */
	if (n > 0)
	{
		int bytes;
		for (bytes = 0; buf[pos] != '\0' && n > 0; --n)
		{
			unsigned length = calc_unival_length(buf+pos);
			bytes += length;
			pos   += length;
		}
		input_do_delete_raw(bytes, do_save_cut);
	}
	else if (n < 0)
	{
		unsigned limit = current_screen->inputdata.buffer.minpos;
		
		int bytes;
		for(bytes=0; n<0 && pos > limit; ++n)
		{
			/* Go back until we reach a beginning of a character */
			for(;;)
			{
				unsigned length = calc_unival_length(buf + --pos);
				if (length) { bytes += length; break; }
				/* But don't go more than we're allowed to */
				if (pos == limit) break;
			}
		}
		input_do_delete_raw(-bytes, do_save_cut);
	}
}
