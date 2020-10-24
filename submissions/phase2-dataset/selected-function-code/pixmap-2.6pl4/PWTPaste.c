void PWTPaste(w, event)
    Widget  w;
    XEvent *event;
{
    PixmapWidget PW = (PixmapWidget) w;

    if (!PW->pixmap.selection.own)
	PWRequestSelection(w, event->xbutton.time, TRUE);
    else 
	PWStore(w);

    if (!PWQueryStored(w))
	return;

    PWEngageRequest(w, RestoreRequest, False, 
		    (XtPointer)&(event->xbutton.state), (Cardinal)sizeof(int));
    
    OnePointHandler(w,
	       (PWStatus*) PW->pixmap.request_stack[PW->pixmap.current].status,
	       event);
}
