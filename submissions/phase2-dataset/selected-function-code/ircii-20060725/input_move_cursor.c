input_move_cursor(dir)
	int	dir;
{
	u_char* buf  = current_screen->inputdata.buffer.buf;
	unsigned pos = current_screen->inputdata.buffer.pos;
	if (dir)
	{
		/* if there are still chars remaining */
		if (buf[pos] != '\0')
		{
			/* Skip over 1 character */
			pos += calc_unival_length(buf+pos);
			
			input_do_set_cursor_pos(pos);
		}
	}
	else
	{
		unsigned limit = current_screen->inputdata.buffer.minpos;

		if (pos > limit)
		{
			/* Go back until we reach a beginning of a character */
			while(!calc_unival_length(buf + --pos))
			{
				/* But don't go more than we're allowed to */
				if (pos == limit) break;
			}
			input_do_set_cursor_pos(pos);
		}
	}
}
