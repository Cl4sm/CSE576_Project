 * INCR support originally provided by Paul Sheer <psheer@obsidian.co.za>
 */
/* EXTPROTO */
void
rxvt_selection_property(rxvt_t* r, Window win, Atom prop)
{
    int		 reget_time = 0;

    if (NOT_ATOM(prop))
	return;

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_selection_property(%08lx, %lu)\n", win, (unsigned long)prop));
    if (r->h->selection_wait == Sel_normal)
    {
	int	     a, afmt;
	Atom		atype;
	unsigned long   bytes_after, nitems;
	unsigned char  *s = NULL;

	a = XGetWindowProperty(r->Xdisplay, win, prop, 0L, 1L, False,
		   r->h->xa[XA_INCR], &atype, &afmt, &nitems,
		   &bytes_after, &s);
	if (s)
	    XFree(s);
	if (a != Success)
	    return;
#ifndef OS_CYGWIN
	if (atype == r->h->xa[XA_INCR])	    /* start an INCR transfer */
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_selection_property: INCR: starting transfer\n"));
	    XDeleteProperty(r->Xdisplay, win, prop);
	    XFlush(r->Xdisplay);
	    reget_time = 1;
	    r->h->selection_wait = Sel_incr;
	}
#endif
    }
    else if (r->h->selection_wait == Sel_incr)
    {
	reget_time = 1;
	if (rxvt_selection_paste(r, win, prop, True) == -1)
	{
	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_selection_property: INCR: clean end\n"));
	    r->h->selection_wait = Sel_none;
	    r->h->timeout[TIMEOUT_INCR].tv_sec = 0; /* turn off timer */
	}
    }
    if (reget_time)	/* received more data so reget time */
    {
	gettimeofday( &(r->h->timeout[TIMEOUT_INCR]), NULL);
	/* ten seconds wait */