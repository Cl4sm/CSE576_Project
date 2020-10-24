{
	int i;

	if(!flag[BLK] || mk == cur_pos) return;

		if(cur_pos < mk)
			{ bstart = cur_pos; bend = mk; }
		else
			{ bstart = mk; bend = cur_pos; }

	selection_length = bend - bstart;
	if ((selection_text = (char *) mrealloc(selection_text, selection_length)) == NULL) {
		printf(SELECT_REALLOC"\n");
se_exit:
		bell();
		return;
	}
	for (i = 0; i < selection_length; i++) {
		selection_text[i] = bstart[i] == EOL ? '\n' : bstart[i];
	}
	XSetSelectionOwner(dpy, XA_PRIMARY, win, (Time) eve_time);
	if (XGetSelectionOwner(dpy, XA_PRIMARY) != win) {
		printf(CANT_SELECT"\n");
		goto se_exit;
	}
	XChangeProperty(dpy, DefaultRootWindow(dpy), XA_CUT_BUFFER0,
			XA_STRING, 8, PropModeReplace, selection_text,
			selection_length);
}
