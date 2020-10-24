{
	int len=screen_width-1;
	int i=fr;

	screen_start=line_start;

	/* calculate screen_start */
	for(; i<y; i++) while(screen_start > edbuf && *--screen_start != EOL) ;
	for(; i>y; i--) while(*++screen_start != EOL) ;

	/* first line */
	screen_start++;
	do {
		gotoxy(0,fr+yl2);
		if(screen_start<file_end && strlen(screen_start) > xlo) {
#ifndef X11
			clreol();
#endif
			show_rest(len, screen_start);
		} else {
			if(((flag[BLK]) &&
				(screen_start == bend)) ||
				(screen_start == bend+1)) lowvideo();
			clreol();
		}
		/* skip EOL */
		while(*screen_start++) ;
#ifdef X11
	} while(++fr <= to);
#else
	} while(++fr < to);
#endif
}
