char *goto_find(char *s, int back)
{
	mark_off();
	do
	{
		if(back) {
			if(--s <= edbuf) {
				bell();
				return 0;
			}
		}
		else {
			if(++s >= file_end) {
				bell();
				return 0;
			}
		}
	} while(str_cmp(s) );
	goto_ptr(s);
	block_mark();
	mk += strlen(sbuf);
	return s;
}
