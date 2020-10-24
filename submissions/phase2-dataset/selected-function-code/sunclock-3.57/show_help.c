void show_help(int mode)
{
#ifdef EXTHELP
	char name[NLEN];
#ifdef EMACS
	sprintf(name, "%s/%s", DOCDIR, "MANUAL.emx");
#else
	sprintf(name, "%s/%s", DOCDIR, "MANUAL.wordstar");
#endif
	new_edit(name);
#else

	char* prntstr;
	int i;

        flag[SHW] = 0;
	flag[BLK] = 0;
	last_mk = mk;
	mk = (void*) -1;
	clrscr();
	show_top();
	gotoxy(0,yl2);

#ifdef X11

#ifdef MINIMAL
	prntstr = HELP_STR;
	/* char at a time to process EOL char */
	for(i=0; i < sizeof(HELP_STR); putch(prntstr[i++]));
#else
	if (mode == 0) {
		prntstr = HELP_STR;
		/* char at a time to process EOL char */
		for(i=0; i < sizeof(HELP_STR); putch(prntstr[i++]));
	}
	if (mode == 1) {
                int j;
		prntstr = ALT_KEY_BINDINGS;
		for (j=0; j<strlen(prntstr); j++) putch(prntstr[j]);
		putch(EOL); putch(EOL);
		for(i=0; i<26; i++) if ((prntstr=binding[i])) {
			putch('a'+i); putch(':'); putch(' ');
			for (j=0; j<strlen(prntstr); j++) putch(prntstr[j]);
			putch(EOL);
		}
	}

#endif /* MINIMAL */

#else /* TERMCAP */

#ifdef MINIMAL
	/* line at a time to process EOL char */
	i=0;
	prntstr = HELP_STR;
	while(prntstr < HELP_STR + sizeof(HELP_STR)) {
		puts(prntstr);
		prntstr += strlen(prntstr)+1;
		gotoxy(0,++i);
	}
#else
	if (mode == 0) {
		/* line at a time to process EOL char */
		i=0;
		prntstr = HELP_STR;
		while (prntstr < HELP_STR + sizeof(HELP_STR)) {
			puts(prntstr);
			prntstr += strlen(prntstr)+1;
			gotoxy(0,++i);
		}
	}
	if (mode == 1) {
                int j=2;
		prntstr = ALT_KEY_BINDINGS;
		puts(prntstr);
		gotoxy(0, j);
		for(i=0; i<26; i++) if ((prntstr=binding[i])) {
			putchar('a'+i); putchar(':'); putchar(' ');
			puts(prntstr);
			gotoxy(0, ++j);
		}
	}
#endif /* MINIMAL */
#endif /* TERMCAP */
	help_done = -1;
	scr_update();
	mk = last_mk;
#endif /* EXTHELP */
}
