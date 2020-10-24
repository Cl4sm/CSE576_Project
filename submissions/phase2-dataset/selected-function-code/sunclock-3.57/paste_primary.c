{
	Atom actual_type;
	int actual_format, i;
	long nitem, bytes_after, nread;
	unsigned char *data;
	char indent = flag[IND];	/* stash autoindent state */

	if (property == None)	/* don't paste anything */
		return(0);

	flag[IND] = 0;	/* off autoindent */
	nread = 0;
	/* X-selection paste loop */
	do {
		if (XGetWindowProperty		/* get remaining selection max 1024 chars */
			(dpy, win, property, nread / 4, 1024, Delete,
			 AnyPropertyType, &actual_type, &actual_format, &nitem,
			 &bytes_after, (unsigned char **) &data)
			!= Success)
			return(0);
		update_scr = 0;				/* dont update scr...yet */
		/* paste last batch one char at a time */
		for(i = 0; i < nitem; handle_key(NULL, data[i++],0));
		nread += nitem;
		XFree(data);
	} while (bytes_after > 0);
	update_scr = 1;					/* _now_ update display */
	flag[SHW] = 1;
	scr_update();
	flag[IND] = indent;	/* restore autoindent state */
	return(nread);
}
