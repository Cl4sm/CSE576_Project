{

#ifdef X11
	if(executive != MAIN && !expose) return;
	undraw_cursor();
#else
	if(executive != MAIN) return;
#endif
	/* update text buffer display */
	if( flag[BLK] ) flag[SHW] = 1;
	if(y <= -1 || y >= screen_height) {
		if(y == -1) {
			y++; show_sdn(0);
		}
		else if(y == screen_height) {
			y--; show_sdn(0);
		}
		else {
			y = y < 0 ? 0 : screen_height-1;
			show_scr(0, screen_height-1);
		}
	}
	else if(flag[SHW] ) {
		show_scr(0, screen_height-1);
	}
	flag[SHW] = 0;
#ifdef X11
	if (help_done) return;
#endif
	show_pos();
	lowvideo();
	gotoxy(x-1,y+yl2);
#ifdef X11
        show_vbar();
	draw_cursor();
#endif
}
