		     XtPointer call_data, Cardinal call_data_size)
#else
Boolean PWAddRequest(w, name, trap, call_data, call_data_size)
    Widget    w;
    PWRequest name;
    Boolean   trap;
    XtPointer   call_data;
    Cardinal  call_data_size;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    PWRequestRec *request;
    
    if((request = FindRequest(name)) != (PWRequestRec *)0) {
	if (_PWDEBUG)
	  fprintf(stderr, "Adding... Cardinal: %d\n", PW->pixmap.cardinal + 1);

	PW->pixmap.request_stack = (PWRequestStack *)
	    XtRealloc((char *)PW->pixmap.request_stack,
	     (Cardinal)((++PW->pixmap.cardinal + 1) * sizeof(PWRequestStack)));
	
	PW->pixmap.request_stack[PW->pixmap.cardinal].request = request;
	PW->pixmap.request_stack[PW->pixmap.cardinal].status = 
	    XtMalloc(request->status_size);
	PW->pixmap.request_stack[PW->pixmap.cardinal].trap = trap;
	PW->pixmap.request_stack[PW->pixmap.cardinal].call_data = 
	    XtMalloc(call_data_size);
	bcopy(call_data, 
	      PW->pixmap.request_stack[PW->pixmap.cardinal].call_data,
	      call_data_size);

	return True;
    }
    else {
	XtAppWarning(XtWidgetToApplicationContext(w),
		  "bad request name.  PixmapWidget");
	return False;
    }
}
