{
#ifdef X11
	comment = prp;
#endif
	diabuf = buf;		/* point at the input buffer */
	dialogCB = cb;		/* setup the dialog callback */
	first = *diabuf;	/* first = 0 = True; */
	dblen = blen;

	lowvideo(); 		/* make dialog stand out in status line */
	show_note(prp); 	/* show function */
	cputs(": ");
	cputs(diabuf);		/* show preset, if any */
	col=strlen(diabuf);	/* point cursor at _end_ of preset string. */
	executive = DIALOG;	/* tell handle_hey() to pass keys to dialog */
#ifdef X11
	draw_cursor();
#endif
	return 0;
}
