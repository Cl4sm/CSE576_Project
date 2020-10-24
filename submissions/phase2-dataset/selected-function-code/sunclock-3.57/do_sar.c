void do_sar()
{
	int i;
	char *sb;

	if (search_mode<=2) {
		executive = MAIN;
		flag[SHW] = 1;
#ifdef X11
		free(comment);
#endif
		scr_update();
		show_top();
		return;
	}

    repl_again:
	if (search_mode==4 || search_mode==5) {
		rlen = strlen(rbuf);
		do {
			file_resize(s+slen, s); 		/* delete srch string&save undo data */
			file_resize(s, s+rlen); 		/* insert space for replace string */
			memcpy(s, rbuf, rlen);
			u_ins(s, rlen);				/* add replace string to undo buf */
			s = s+rlen;
		}
		while(flag[ALL] && (s=goto_find(s, 0)) != 0) ;
		flag[SHW] = 1;
	}
	if (search_mode==6) return;
	if((s =(char*) goto_find(cur_pos,0))) {
		slen = strlen(sbuf);
		show_scr(0, screen_height-1);
		gotoxy(x-1,y+yl2);
		highvideo();
		i = slen;
		sb = s;
		while(i-- && !putch(*sb++));
		lowvideo();
		if (search_mode==5) goto repl_again;
	} else {
		search_mode = 2;
		do_sar();
	}
}
