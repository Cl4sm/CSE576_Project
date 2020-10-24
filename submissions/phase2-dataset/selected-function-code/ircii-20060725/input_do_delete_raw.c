input_do_delete_raw(int n, int do_save_cut)
{
	u_char* buf  = current_screen->inputdata.buffer.buf;
	unsigned pos = current_screen->inputdata.buffer.pos;
	unsigned max = sizeof(current_screen->inputdata.buffer.buf);

	cursor_to_input();
/*
	fprintf(stderr, "pos(%u)max(%u)buf(%s)do_delete_raw(%d, %d)\n",
	    pos,max,buf, n, do_save_cut);
*/
	/* If n>0, deletes from front
	 * if n<0, deletes from back & moves cursor
	 */
	if (n < 0)
	{
		unsigned limit = current_screen->inputdata.buffer.minpos;
		/* Number of bytes LEFT from the cursor (prompt excluding) */
		unsigned oldbytes = pos-limit;
		unsigned erasebytes = -n;

		/* Don't delete more than we can */
		if (erasebytes > oldbytes)
			erasebytes = oldbytes;

		/* Move cursor backward */
		pos -= erasebytes;
		input_do_set_cursor_pos(pos);

		/* Then delete from forward */
		n = erasebytes;
	}
	if (n > 0)
	{
		unsigned oldbytes = max-pos;
		unsigned erasebytes = n > oldbytes ? oldbytes : n;
		unsigned newbytes = oldbytes - erasebytes;
		
		if (do_save_cut)
		{
			if (cut_buffer) new_free(&cut_buffer);
			cut_buffer = new_malloc(erasebytes+1);
			memcpy(cut_buffer, buf+pos, erasebytes);
			cut_buffer[erasebytes] = '\0';
		}
		
		memmove(buf+pos,
		        buf+pos+erasebytes,
		        newbytes);
		buf[pos+newbytes] = '\0';
	}
/*
	fprintf(stderr, "-> pos(%u)max(%u)buf(%s)\n",
	    pos,max,buf);
*/
	/* Now update the right side from cursor */
	update_input(UPDATE_FROM_CURSOR);
}
