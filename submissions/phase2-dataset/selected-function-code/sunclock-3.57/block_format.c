{
	char	*s=line_start;
	int ytmp = y;

	goto_x(1);
	/* remove newlines to end of para */
	while(s = strchr(++s, EOL), ytru < ytot && s[1] != EOL) {
		u_del(s,1);
		*s = BLNK;
		u_ins(s,1);
		ytot--;
		flag[CHG] = 1;
	}
	/* truncate line <= screen_width */
	while(goto_x(strlen(line_start+1)+1), x_offset >= screen_width) block_fill();
	/* goto end of line */
	while(line_start[x_offset] ) cursor_right();
	show_top();
	if( flag[SHW] == 0) show_scr(ytmp, screen_height);
}
