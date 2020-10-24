void goto_ptr(char *s)
{
	/* find line_start <= s */
	char *s1 = s;
	int yo = y;

	if(s < edbuf || s >= edbuf + amax) {
		bell();
		return;
	}
	top();
	while(*--s1 != EOL && s1 > edbuf) ; 				/* find start of target line */
	while(line_start+1 <= s1 && y < ytot) cursor_down();/* move to target line */
	while(line_start+1 > s && y > 0) cursor_up();		/* move back before target */
	goto_x(s-line_start);								/* get x from line start */
	if(y > screen_height || xlo != 0) {
		flag[SHW] = 1;
		y = yo;
	}
}
