{
	char *bb_end;
	int c, d;
	char *col;

	if(!(fb = fopen(fbuf, "r"))) { bell(); return; };

	fseek(fb, 0L, 2);		/* seek to 0L offset, 2=end of file */
	blen=ftell(fb); 		/* get file length */
	fseek(fb, 0L, 0);		/* seek back to file start */
	bb_end = col = bb;

	/* read complete file */
	do {
		c = fgetc(fb);
		if(c == EOF) {
			fclose(fb);
			fb = 0; /* no more read */
			break;
		}
		if(c == '\t' && flag[TAB]) {
			do (*bb_end++ = BLNK);
			while( ((bb_end-col) % tabsize) != 0);
		}
		else
			if(c == LF) {
				*bb_end++ = EOL;
				col = bb_end;
			}
		else
			*bb_end++ = c;
		if (bb_end >= bb+bmax) {
			d = (int)bb;
			realloc_buffer(1);
			d = (int)bb - d;
			col += d;
			bb_end += d;
		}
	} while(bb_end < bb+bmax);

	blen = bb_end - bb;
	block_paste();
}
