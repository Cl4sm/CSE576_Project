void OnePointTerminate(w, status, draw)
    Widget     w;
    PWStatus  *status;
    void     (*draw)();
{
    
    if (status->success && draw) {
	PWStoreToBuffer(w);
	(*draw)(w,
		status->at_x, status->at_y,
		status->value);
	PWChangeNotify(w, NULL, NULL);
	PWSetChanged(w);
    }    
    else
	if (QuerySet(status->at_x, status->at_y))
	    if (status->draw)
		(*status->draw)(w,
				status->at_x, status->at_y, Highlight);
    
    XtRemoveEventHandler(w,
			 ButtonPressMask | ButtonReleaseMask | 
			 ExposureMask | PointerMotionMask,
			 FALSE, (XtEventHandler)OnePointHandler, status);
}
