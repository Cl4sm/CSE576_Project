#else
Boolean PWRemoveRequest(w)
    Widget w;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    
    if (PW->pixmap.cardinal > 0) {
	if (_PWDEBUG)
	    fprintf(stderr, "Removing... Cardinal: %d\n", PW->pixmap.cardinal);
	if (PW->pixmap.current == PW->pixmap.cardinal)
	    PWTerminateRequest(w, False);
	
	if (PW->pixmap.request_stack[PW->pixmap.cardinal].request->remove)
	    (*PW->pixmap.request_stack[PW->pixmap.cardinal].request->remove)
		(w,
		 PW->pixmap.request_stack[PW->pixmap.cardinal].status,
		 PW->pixmap.request_stack[PW->pixmap.cardinal].request->remove_client_data,
		 PW->pixmap.request_stack[PW->pixmap.cardinal].call_data);
	
	XtFree(PW->pixmap.request_stack[PW->pixmap.cardinal].status);
	XtFree(PW->pixmap.request_stack[PW->pixmap.cardinal].call_data);
	PW->pixmap.request_stack = (PWRequestStack *)
	    XtRealloc((char *)PW->pixmap.request_stack,
	     (Cardinal)((--PW->pixmap.cardinal + 1) * sizeof(PWRequestStack)));
	
	return True;
    }
    else 
	return False;
}
