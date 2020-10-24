

/* INTPROTO */
int
rxvt_selection_request_other(rxvt_t* r, int page, Atom target, int selnum)
{
    Atom	    sel;
#ifdef DEBUG
    char	   *debug_xa_names[] = { "PRIMARY", "SECONDARY", "CLIPBOARD" };
#endif

    r->h->selection_type |= selnum;
    if (selnum == Sel_Primary)
	sel = XA_PRIMARY;
    else if (selnum == Sel_Secondary)
	sel = XA_SECONDARY;
    else
	sel = r->h->xa[XA_CLIPBOARD];
    if (XGetSelectionOwner(r->Xdisplay, sel) != None)
    {
	rxvt_dbgmsg(( DBG_DEBUG, DBG_SCREEN, "rxvt_selection_request_other %d: "
		    "pasting %s\n", page, debug_xa_names[selnum] ));

	XConvertSelection(r->Xdisplay, sel, target,
	    r->h->xa[XA_VT_SELECTION], PVTS(r, page)->vt,
	    r->h->selection_request_time);
	return 1;