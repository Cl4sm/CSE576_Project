void DragTwoPointsTerminate(w, status, draw)
    Widget     w;
    PWStatus  *status;
    void     (*draw)();
{
    
    if (status->success && draw) {
	if ((status->from_x != status->to_x) 
	    || 
	    (status->from_y != status->to_y))
	    (*draw)(w,
		    status->from_x, status->from_y,
		    status->to_x, status->to_y,
		    status->value);
	PWChangeNotify(w, NULL, NULL);
	PWSetChanged(w);
    }
    
    XtRemoveEventHandler(w,
		         ButtonPressMask | ButtonReleaseMask | PointerMotionMask,
			 FALSE, (XtEventHandler)DragTwoPointsHandler, status);
}
