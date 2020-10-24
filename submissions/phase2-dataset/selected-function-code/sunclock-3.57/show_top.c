{
	int i;
	char tbuf[NLEN];

#ifdef X11
	if(executive != MAIN && !expose) return;
#else
	if(executive != MAIN || !exitf) return;
#endif
	gotoxy(0,yl1);
	highvideo();
	clreol();
	show_pos();
	for(i=0; i<=EDT; i++)
		show_flag(i, flag[i]);
	sprintf(tbuf,"   "HELP"=F1   %s", cfdpath);
	cputs(tbuf);
	lowvideo();
	gotoxy(x-1,y+yl2);
}
