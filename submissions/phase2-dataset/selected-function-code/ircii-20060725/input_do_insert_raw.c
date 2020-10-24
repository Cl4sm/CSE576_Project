static void
input_do_insert_raw(u_char* source)
{
	u_char* buf  = current_screen->inputdata.buffer.buf;
	unsigned pos = current_screen->inputdata.buffer.pos;
	unsigned max = sizeof(current_screen->inputdata.buffer.buf);
	unsigned inslen = my_strlen(source);
	
	/* This function inserts the given substring of bytes
	 * to the input line at the current editing position.
	 * Cursor is moved to point to the end of the substring.
	 */
	
	if (pos + inslen > max)
	{
		inslen = max-pos;
	}
	
	/* Move the tail out of way */
	memmove(buf + pos + inslen,
	        buf + pos,
	        max - pos - inslen);
	/* Then put the substring in */
	memcpy(buf + pos,
	       source,
	       inslen);
	/* Ensure the buffer is terminated */
	buf[max-1] = '\0';

	pos += inslen;
	if (pos > max)
		pos = max;
	
	/* Update the screen from the old cursor position */
	cursor_to_input();
	update_input(UPDATE_FROM_CURSOR);
	/* Then place the cursor correctly */
	input_do_set_cursor_pos(pos);
}
