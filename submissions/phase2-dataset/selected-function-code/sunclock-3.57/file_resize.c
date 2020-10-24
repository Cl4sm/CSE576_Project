void file_resize(char *s, char *d)
{
	char	*e;
	unsigned int i;
	int s_rel = s - edbuf;
	int d_rel = d - edbuf;

	/* immediate problem only when block buffer on disk too big */

        i = file_end - s;
	file_end += d-s;
 retry:
	if(file_end>= edbuf+amax) {
		if (realloc_buffer(0)) { 
			s = edbuf + s_rel;
			d = edbuf + d_rel;
			goto retry; 
		}
		show_note(MAIN_BUFFER_FULL);
		bell();
		return;
	}

	e = s+i;
	if(s < d) { /* expand */
		d += e - s;
		s = e;
		while(i-- > 0) *--d = *--s;
	}
	else {
		u_del(d, s - d);		/* save undo delete string */
		/* adjust ytot when shrink */
		for(e=d; e<s; e++) if(*e == EOL) ytot--;
		while(i-- > 0) *d++ = *s++;
	}
	*file_end = EOL;	/* last line may not complete */
	if(!flag[CHG] ) {
		show_flag(CHG, 1);
		gotoxy(x-1,y+yl2);
		clreol();
	}
}
