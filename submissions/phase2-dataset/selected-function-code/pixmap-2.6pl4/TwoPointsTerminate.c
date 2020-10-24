    Widget    w;
    PWStatus *status;
    void    (*draw)();
{
    
    if (status->success && draw) {
	PWStoreToBuffer(w);
	(*draw)(w,
		status->from_x, status->from_y,
		status->to_x, status->to_y,
		status->value);
	PWChangeNotify(w, NULL, NULL);
	PWSetChanged(w);
    }
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
