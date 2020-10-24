update_input(update)
	int	update;
{
	ScreenInputData* inputdata = &current_screen->inputdata;
	iconv_t display_conv = NULL;
	
	int term_echo = 1;

	if (dumb)
		return;

	cursor_to_input();

	if (input_do_check_prompt(update))
		update = UPDATE_ALL;
	
	term_echo = !input_is_password_prompt();

	if (input_check_resized())
		update = UPDATE_ALL;
	
	if (update != NO_UPDATE)
	{
#ifdef HAVE_ICONV_OPEN
		const char *enc = CP(display_encoding);
		if (!enc)
			enc = "ISO-8859-1";
		display_conv = iconv_open(enc, "UTF-8");
#endif /* HAVE_ICONV_OPEN */
	}
	
/*
	{
		unsigned a;
		u_char* buf  = inputdata->buffer.buf;
		fputc('"', stderr);
		for (a = 0; buf[a]; ++a)
			fputc(buf[a], stderr);
		fprintf(stderr, "\"\n");
		fputc('>', stderr);
		for (a = 0; buf[a]; ++a)
			if (a == inputdata->buffer.pos)
				fputc('^', stderr);
			else if (a == inputdata->buffer.minpos)
				fputc('|', stderr);
			else
				fputc(' ', stderr);
		fprintf(stderr, "\n");
	}
*/
	if (update == UPDATE_JUST_CURSOR || update == UPDATE_ALL)
	{
		u_char* buf  = inputdata->buffer.buf;
		unsigned pos = inputdata->buffer.pos;
		unsigned limit = inputdata->buffer.minpos;
		unsigned column, ptr;
		
		unsigned window;
		
		/* Recalculate pos_column */
		for (column = ptr = 0; ptr < pos; )
		{
			unsigned unival = calc_unival(buf+ptr);

			if (!term_echo && ptr >= limit)
				unival = ' ';

			column += input_do_calculate_width(unival, display_conv);
			ptr    += calc_unival_length(buf+ptr);
		}
		inputdata->pos_column = column;

		window = column - (column % inputdata->zone);
		
		/* Recalculate left_ptr */
		for (column = ptr = 0; column < window; )
		{
			unsigned unival = calc_unival(buf+ptr);
			
			if (!term_echo && ptr >= limit)
				unival = ' ';
			
			column += input_do_calculate_width(unival, display_conv);
			ptr    += calc_unival_length(buf+ptr);
		}
		/* If the left edge has been moved, redraw the input line */
		if (ptr != inputdata->left_ptr)
			update = UPDATE_ALL;
		inputdata->left_ptr = ptr;
		
		/* Recalculate cursor_x */
		inputdata->cursor_x = inputdata->pos_column - column;
	}
	
	if (update == UPDATE_FROM_CURSOR || update == UPDATE_ALL)
	{
		unsigned limit = inputdata->buffer.minpos;
		u_char *buf = inputdata->buffer.buf;
		u_char VisBuf[BIG_BUFFER_SIZE];
		unsigned column, iptr, optr;
		int written;
		
		for (column = 0, optr = 0, iptr = inputdata->left_ptr;
			buf[iptr] != '\0' && column < current_screen->co; )
		{
			unsigned unival = calc_unival(buf+iptr);
			unsigned len = calc_unival_length(buf+iptr);
			
			if (!term_echo && iptr >= limit)
				unival = ' ';
			
			if (displayable_unival(unival, display_conv))
			{
				column += calc_unival_width(unival);
				memcpy(VisBuf+optr, buf+iptr, len);
				optr += len;
				iptr += len;
			}
			else
			{
				unsigned n;
				VisBuf[optr++] = REV_TOG;
				for (n = 0; n < len; ++n)
					VisBuf[optr++] = (buf[iptr++] & 127) | 64;
				VisBuf[optr++] = REV_TOG;
				column += len;
			}
		}
		VisBuf[optr] = '\0';
		term_move_cursor(0, inputdata->cursor_y);
		written = output_line(VisBuf, 0);
		if (term_clear_to_eol() && written < current_screen->co)
		{
			/* EOL wasn't implemented, so do it with spaces */
			term_space_erase(current_screen->co - written);
		}
	}
	
	if (update != NO_UPDATE)
	{
		/* Always update cursor */
		term_move_cursor(inputdata->cursor_x, inputdata->cursor_y);
	}
	
	if (update != NO_UPDATE)
	{
#ifdef HAVE_ICONV_OPEN
		iconv_close(display_conv);
#endif /* HAVE_ICONV_OPEN */
	}

	term_flush();
}
