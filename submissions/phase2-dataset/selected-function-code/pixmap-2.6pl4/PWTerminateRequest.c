#else
Boolean PWTerminateRequest(w, cont)
    Widget w;
    Boolean cont;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    
    if (PW->pixmap.current > 0) {
	if (_PWDEBUG)
	    fprintf(stderr, "Terminating... Current: %d\n", PW->pixmap.current);
    	if (PW->pixmap.request_stack[PW->pixmap.current].request->terminate)
	    (*PW->pixmap.request_stack[PW->pixmap.current].request->terminate)
		(w,
		 PW->pixmap.request_stack[PW->pixmap.current].status,
		 PW->pixmap.request_stack[PW->pixmap.current].request->terminate_client_data,
		 PW->pixmap.request_stack[PW->pixmap.current].call_data);
	
	if (cont) {
	    if (PW->pixmap.current == PW->pixmap.cardinal)
		TrappingLoop(PW);
	    else {
		if (_PWDEBUG)
		    fprintf(stderr, "Continuing... Current: %d\n", PW->pixmap.current+1);
		if (!Engage(PW, ++PW->pixmap.current))
		    PWTerminateRequest(w, True);
	    }
	}
	else
	    PW->pixmap.current = 0;
    }
    
    return PW->pixmap.current;
}
