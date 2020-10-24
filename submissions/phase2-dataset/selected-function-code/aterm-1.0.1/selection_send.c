void
selection_send(XSelectionRequestEvent * rq)
{
    /* Changes are from rxvt. This fixes #205040. */
    XEvent          ev;
#ifdef USE_ATOM32		   
    Atom32          target_list[4];
#else
	Atom          target_list[4];
#endif 
    Atom            target;
    XTextProperty   ct;
    XICCEncodingStyle style;
    char           *cl[4];

    ev.xselection.type = SelectionNotify;
    ev.xselection.property = None;
    ev.xselection.display = rq->display;
    ev.xselection.requestor = rq->requestor;
    ev.xselection.selection = rq->selection;
    ev.xselection.target = rq->target;
    ev.xselection.time = rq->time;

    if (rq->target == aterm_XA_TARGETS) {
#ifdef USE_ATOM32		
	target_list[0] = (Atom32) aterm_XA_TARGETS;
	target_list[1] = (Atom32) XA_STRING;
	target_list[2] = (Atom32) aterm_XA_TEXT;
	target_list[3] = (Atom32) _XA_COMPAUND_TEXT;
	XChangeProperty(Xdisplay, rq->requestor, rq->property, rq->target,
			(8 * sizeof(target_list[0])), PropModeReplace,
			(unsigned char *)target_list,
			(sizeof(target_list) / sizeof(target_list[0])));
#else	  
	target_list[0] =  aterm_XA_TARGETS;
	target_list[1] =  XA_STRING;
	target_list[2] =  aterm_XA_TEXT;
	target_list[3] =  _XA_COMPAUND_TEXT;
	XChangeProperty(Xdisplay, rq->requestor, rq->property, XA_ATOM,
			32, PropModeReplace,
 			(unsigned char *)target_list,
 			(sizeof(target_list) / sizeof(target_list[0])));
#endif	 
	ev.xselection.property = rq->property;
    } else if (rq->target == XA_STRING
	       || rq->target == _XA_COMPAUND_TEXT
	       || rq->target == aterm_XA_TEXT) {
	if (rq->target == XA_STRING) {
	    style = XStringStyle;
	    target = XA_STRING;
	} else {
	    target = _XA_COMPAUND_TEXT;
	    style = (rq->target == _XA_COMPAUND_TEXT) ? XCompoundTextStyle
		: XStdICCTextStyle;
	}
	cl[0] = selection.text;
	XmbTextListToTextProperty(Xdisplay, cl, 1, style, &ct);
	XChangeProperty(Xdisplay, rq->requestor, rq->property,
			target, 8, PropModeReplace,
			ct.value, ct.nitems);
	ev.xselection.property = rq->property;
    }
    XSendEvent(Xdisplay, rq->requestor, False, 0, &ev);
}
