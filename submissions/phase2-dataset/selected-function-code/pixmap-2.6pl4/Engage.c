    PixmapWidget PW;
    Cardinal current;
{
    PW->pixmap.current = current;
    
    if (_PWDEBUG)
	fprintf(stderr, "Request: %s\n", 
		PW->pixmap.request_stack[current].request->name);
  
    if (PW->pixmap.request_stack[current].request->engage) {
	(*PW->pixmap.request_stack[current].request->engage)
	    ((Widget) PW,
	     PW->pixmap.request_stack[current].status,
	     PW->pixmap.request_stack[current].request->engage_client_data,
	     PW->pixmap.request_stack[current].call_data);
	return True;
    }
    else
	return False;
}
