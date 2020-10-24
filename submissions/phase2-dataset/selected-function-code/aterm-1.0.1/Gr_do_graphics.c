void
Gr_do_graphics(int cmd, int nargs, int args[], unsigned char *text)
{
#ifdef RXVT_GRAPHICS
    static Window   last_id = None;
    long            win_id;
    grwin_t        *grwin;
    grcmd_t        *newcmd, *oldcmd;
    int             i;

    if (cmd == 'W') {
	Gr_NewWindow(nargs, args);
	return;
    }
    win_id = (nargs > 0) ? (Window) args[0] : None;

    if ((cmd == 'G') && (win_id == None)) {
	Gr_Geometry(NULL, NULL);
	return;
    }
    if ((win_id == None) && (last_id != None))
	win_id = last_id;

    if (win_id == None)
	return;

    grwin = gr_root;
    while ((grwin != NULL) && (grwin->win != win_id))
	grwin = grwin->next;

    if (grwin == NULL)
	return;

    if (cmd == 'G') {
	Gr_Geometry(grwin, NULL);
	return;
    }
    nargs--;
    args++;			/* skip over window id */

/* record this new command */
    newcmd = (grcmd_t *) MALLOC(sizeof(grcmd_t));
    newcmd->ncoords = nargs;
    newcmd->coords = (int *)MALLOC((newcmd->ncoords * sizeof(int)));

    newcmd->next = NULL;
    newcmd->cmd = cmd;
    newcmd->color = scr_get_fgcolor();
    newcmd->text = text;

    for (i = 0; i < newcmd->ncoords; i++)
	newcmd->coords[i] = args[i];

/*
 * If newcmd == fill, and rectangle is full window, drop all prior
 * commands.
 */
    if ((newcmd->cmd == 'F') && (grwin) && (grwin->graphics)) {
	for (i = 0; i < newcmd->ncoords; i += 4) {
	    if ((newcmd->coords[i] == 0) &&
		(newcmd->coords[i + 1] == 0) &&
		(newcmd->coords[i + 2] == GRX_SCALE) &&
		(newcmd->coords[i + 3] == GRX_SCALE)) {
	    /* drop previous commands */
		oldcmd = grwin->graphics;
		while (oldcmd->next != NULL) {
		    grcmd_t        *tmp = oldcmd;

		    oldcmd = oldcmd->next;
		    free(tmp);
		}
		grwin->graphics = NULL;
	    }
	}
    }
/* insert new command into command list */
    oldcmd = grwin->graphics;
    if (oldcmd == NULL)
	grwin->graphics = newcmd;
    else {
	while (oldcmd->next != NULL)
	    oldcmd = oldcmd->next;
	oldcmd->next = newcmd;
    }
    Gr_Dispatch(grwin, newcmd);
#endif
}
