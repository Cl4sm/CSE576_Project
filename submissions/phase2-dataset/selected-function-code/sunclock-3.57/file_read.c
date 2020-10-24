{
	int c, d;
	char *col = file_new();

	if(fi == 0) return;

	/* read complete line */
	do {
		c = fgetc(fi);
		if(c == EOF) {
			fclose(fi);
			fi = 0; /* no more read */
			break;
		}
		if(c == '\t' && flag[TAB]) { /* conditionally convert tabs on input */
			do (*file_end++ = BLNK);
			while( ((file_end-col) % tabsize) != 0);
		}
		else
			if(c == LF) {
			*file_end++ = EOL;
			col = file_end;
			ytot++;
		}
		else *file_end++ = c;
		if (file_end >= edbuf+amax) {
			d = file_end - col;
			realloc_buffer(0);
			col = file_end - d;
		}
	} while(file_end < edbuf+amax);
	for(; ewin.jump>1; ewin.jump--) cursor_down();
}
