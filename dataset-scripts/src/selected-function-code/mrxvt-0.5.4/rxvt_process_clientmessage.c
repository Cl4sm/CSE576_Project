void
rxvt_process_clientmessage(rxvt_t* r, XClientMessageEvent* ev)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "ClientMessage event\n"));
    if (
	  ev->format == 32
	  && (Atom) ev->data.l[0] == r->h->xa[XA_WMDELETEWINDOW]
       )
	rxvt_exit_request (r);

#ifdef OFFIX_DND
    /* OffiX Dnd (drag 'n' drop) protocol */
    if (
	    ev->xclient.message_type == h->xa[XA_DNDPROTOCOL]
	    && (
		ev->xclient.data.l[0] == DndFile
		|| ev->xclient.data.l[0] == DndDir
		|| ev->xclient.data.l[0] == DndLink
	       )
       )
    {
	/* Get Dnd data */
	Atom		ActualType;
	int		ActualFormat;
	unsigned char*	data;
	uint32_t	Size, RemainingBytes;

	XGetWindowProperty(r->Xdisplay, XROOT,
	   r->h->xa[XA_DNDSELECTION], 0L, 1000000L,
	   False, AnyPropertyType, &ActualType, &ActualFormat,
	   &Size, &RemainingBytes, &data);
	XChangeProperty(r->Xdisplay, XROOT, XA_CUT_BUFFER0,
	    XA_STRING, 8, PropModeReplace, data, STRLEN(data));

	rxvt_selection_paste(r, ATAB(r), XROOT, XA_CUT_BUFFER0, True);
	XSetInputFocus(r->Xdisplay, XROOT, RevertToNone, CurrentTime);
    }
#endif	    /* OFFIX_DND */
}