void Paste(w, at_x, at_y, value)
    Widget    w;
    Position  at_x, at_y;
    int       value;
{
    PixmapWidget    PW = (PixmapWidget) w;
    PWStatus       *my_status;
    PWRequest       request;

    my_status = (PWStatus *) 
	PW->pixmap.request_stack[PW->pixmap.current].status;

    my_status->draw = NULL;

   request = (PWRequest)
   PW->pixmap.request_stack[PW->pixmap.current].request->terminate_client_data;
	
    PWTerminateRequest(w, FALSE);
    
    if ((at_x == max(PW->pixmap.mark.from_x, min(at_x, PW->pixmap.mark.to_x)))
	&&
      (at_y == max(PW->pixmap.mark.from_y, min(at_y, PW->pixmap.mark.to_y)))) {
	
	PWStatus *status;
	
	if (_PWDEBUG)
	    fprintf(stderr, "Prepaste request: %s\n", request);
	
	PWEngageRequest(w, request, False, (XtPointer)&(my_status->state), 
			(Cardinal)sizeof(int));
	
	status = (PWStatus *) 
	    PW->pixmap.request_stack[PW->pixmap.current].status;
	
	status->at_x = at_x;
	status->at_y = at_y;
	status->value = value;
	(*status->draw) (w, at_x, at_y, Highlight);	
    }
    else {

	PWStatus *status;
	
      PWEngageRequest(w, MarkRequest, False, (XtPointer)&(my_status->state), 
		      (Cardinal)sizeof(int));
	
	status = (PWStatus *) 
	    PW->pixmap.request_stack[PW->pixmap.current].status;
	
	status->from_x = status->to_x = at_x;
	status->from_y = status->to_y = at_y;
	status->value = value;
	(*status->draw) (w, at_x, at_y, at_x, at_y, Highlight);
    }
}
