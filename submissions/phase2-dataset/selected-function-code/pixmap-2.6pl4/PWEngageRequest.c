			XtPointer call_data, Cardinal call_data_size)
#else
Boolean PWEngageRequest(w, name, trap, call_data, call_data_size)
    Widget w;
    PWRequest name;
    Boolean trap;
    XtPointer call_data;
    Cardinal call_data_size;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    
    if (PWAddRequest(w, name, trap, call_data, call_data_size)) {
	PWTerminateRequest(w, False);
	if (_PWDEBUG)
	    fprintf(stderr, "Engaging... Cardinal: %d\n", PW->pixmap.cardinal);
	if (!Engage(PW, PW->pixmap.cardinal))
	    PWTerminateRequest(w, True);
	
	return True;
    }
    else
	return False;
}
