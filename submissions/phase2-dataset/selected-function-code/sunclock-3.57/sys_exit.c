void sys_exit(int code)
{
	if(!(flag[CHG]) || code==2) {
		/* watch where you're goin', clean up where you been */
#ifdef X11		
		if(selection_text) free(selection_text);
#endif
		if(cfdpath) free(cfdpath);
#ifdef X11
		exit(code&1);
#else
		gotoxy(0,0);
		ttclose();
		exitf = 0;
	        /* printf("\033c"); */           /* reset terminal */
		/* printf("\033[2J"); */              /* clear screen */
		clrscr();
		return;
#endif
	}
	bell();
}
