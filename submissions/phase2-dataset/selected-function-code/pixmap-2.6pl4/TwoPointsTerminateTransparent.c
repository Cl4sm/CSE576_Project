void TwoPointsTerminateTransparent(w, status, draw)
    Widget    w;
    PWStatus *status;
    void    (*draw)();
{
    
    if (status->success && draw)
	(*draw)(w,
		status->from_x, status->from_y,
		status->to_x, status->to_y,
		status->value);
    else
	if (QuerySet(status->from_x, status->from_y) && 
	    QuerySet(status->to_x, status->to_y))
	    if (status->draw)
		(*status->draw)(w,
				status->from_x, status->from_y, 
				status->to_x, status->to_y, Highlight);
    
    XtRemoveEventHandler(w,
			 ButtonPressMask | ButtonReleaseMask | 
			 ExposureMask | PointerMotionMask,
			 FALSE, (XtEventHandler)TwoPointsHandler, status);
}
