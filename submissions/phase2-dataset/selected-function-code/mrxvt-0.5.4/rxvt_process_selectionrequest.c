 * EXT: SelectionRequest
 */
/* EXTPROTO */
void
rxvt_process_selectionrequest (rxvt_t* r, int page, const XSelectionRequestEvent *rq)
{
    XSelectionEvent ev;
#ifdef USE_XIM
    Atom	  target_list[4];
#else
    Atom	  target_list[3];
#endif
    Atom	    target;
    XTextProperty   ct;
    XICCEncodingStyle style;
    char	   *cl[2], dummy[1];

    ev.type = SelectionNotify;
    ev.property = None;
    ev.display = rq->display;
    ev.requestor = rq->requestor;
    ev.selection = rq->selection;
    ev.target = rq->target;
    ev.time = rq->time;

    if (rq->target == r->h->xa[XA_TARGETS])
    {
	target_list[0] = r->h->xa[XA_TARGETS];
	target_list[1] = XA_STRING;
	target_list[2] = r->h->xa[XA_TEXT];
#ifdef USE_XIM
	target_list[3] = r->h->xa[XA_COMPOUND_TEXT];
#endif
	XChangeProperty(r->Xdisplay, rq->requestor, rq->property,
	    XA_ATOM, 32, PropModeReplace,
	    (unsigned char *)target_list,
	    (sizeof(target_list) / sizeof(target_list[0])));
	ev.property = rq->property;
    }
    else if (rq->target == r->h->xa[XA_MULTIPLE])
    {
	/* TODO: Handle MULTIPLE */
    }
    else if (rq->target == r->h->xa[XA_TIMESTAMP] && SEL(r).text)
    {
	XChangeProperty(r->Xdisplay, rq->requestor, rq->property,
	    XA_INTEGER,
	    sizeof(Time) > 4 ? 32 : (8 * sizeof(Time)),
	    PropModeReplace, (unsigned char*)&r->h->selection_time,
	    sizeof(Time) > 4 ? sizeof(Time)/4 : 1);
	ev.property = rq->property;
    }
    else if (
	      rq->target == XA_STRING
	      || rq->target == r->h->xa[XA_COMPOUND_TEXT]
	      || rq->target == r->h->xa[XA_TEXT]
	    )
    {
#ifdef USE_XIM
	short	       freect = 0;
#endif
	int	     selectlen;

#ifdef USE_XIM
	if (rq->target != XA_STRING)
	{
	    target = r->h->xa[XA_COMPOUND_TEXT];
	    style = (rq->target == r->h->xa[XA_COMPOUND_TEXT])
		? XCompoundTextStyle : XStdICCTextStyle;
	} else
#endif
	{
	    target = XA_STRING;
	    style = XStringStyle;
	}
	if (SEL(r).text)
	{
	    cl[0] = (char *)SEL(r).text;
	    selectlen = SEL(r).len;
	}
	else
	{
	    cl[0] = dummy;
	    *dummy = '\0';
	    selectlen = 0;
	}
#ifdef USE_XIM
	if (XmbTextListToTextProperty(r->Xdisplay, cl, 1, style, &ct) == Success)	/* if we failed to convert then send it raw */
	    freect = 1;
	else
#endif
	{
	    ct.value = (unsigned char *)cl[0];
	    ct.nitems = selectlen;
	}
	XChangeProperty(r->Xdisplay, rq->requestor, rq->property,
	    target, 8, PropModeReplace,
	    ct.value, (int)ct.nitems);
	ev.property = rq->property;
#ifdef USE_XIM
	if (freect)
	    XFree(ct.value);
#endif