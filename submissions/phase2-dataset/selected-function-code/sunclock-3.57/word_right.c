{
	char *d = cur_pos;

	if(*d == EOL) { /* at eol */
		if(ytru >= ytot) return;	/* don't go past end */
		cursor_down();
		goto_x(1);					/* goto start of line */
		return;
	}
	
	/* skip first delimitor if any */
        if (strchr(wdelims,*d)) { d++; cursor_right(); }
	/* skip real chars */
	while(!strchr(wdelims,*d)) { d++; cursor_right(); }
	/* skip white space separators 
	   JPD changed the behaviour there - avoiding in particular 
	   a crash when reaching the last word.
	 */
#ifdef TLL
	while(strchr(wdelims,*d)) { d++; cursor_right(); }
#else
	while(isspace(*d)) { d++; cursor_right(); }
#endif
}
