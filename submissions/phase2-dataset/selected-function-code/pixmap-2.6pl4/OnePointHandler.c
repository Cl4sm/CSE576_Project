void OnePointHandler(w, status, event)
    Widget       w;
    PWStatus    *status;
    XEvent      *event;
{
    PixmapWidget PW = (PixmapWidget) w;
    
    if (_PWDEBUG)
	fprintf(stderr, "1PH ");

    switch (event->type) {
	
    case Expose:
	if (QuerySet(status->at_x, status->at_y)) {
	    PWClip(w, 
		   InPixmapX(PW, event->xexpose.x), 
		   InPixmapY(PW, event->xexpose.y),
		   InPixmapX(PW, event->xexpose.x + event->xexpose.width),
		   InPixmapY(PW, event->xexpose.y + event->xexpose.height));
	    if (status->draw)
		(*status->draw)(w,
				status->at_x, status->at_y, Highlight);
	    
	    PWUnclip(w);
	}
	break;
	
    case ButtonPress:
	if (event->xbutton.state != status->state) return;
	if (!QuerySet(status->at_x, status->at_y)) {
	    status->value = Value(PW, event->xbutton.button);
	    status->time = event->xbutton.time;
	    status->at_x = InPixmapX(PW, event->xbutton.x);
	    status->at_y = InPixmapY(PW, event->xbutton.y);
	    if (status->draw)
		(*status->draw)(w,
				status->at_x, status->at_y, Highlight);
	}
	break;
	
    case ButtonRelease:
	if (QuerySet(status->at_x, status->at_y)) {
	    if (status->draw)
		(*status->draw)(w,
				status->at_x, status->at_y, Highlight);
	    
	    /* the following 3 lines is a small fix to Ctrl ops */
	    if (event->xbutton.state&ControlMask)
	      status->value = Set;
	    else status->value = Value(PW, event->xbutton.button);

	    status->time = event->xbutton.time;
	    status->at_x = InPixmapX(PW, event->xbutton.x);
	    status->at_y = InPixmapY(PW, event->xbutton.y);
	    status->success = True;
	    
	    PWTerminateRequest(w, TRUE);
	}
	break;
	
    case MotionNotify:
	if (QuerySet(status->at_x, status->at_y)) {
	    if (!QueryInSquare(PW, event->xmotion.x, event->xmotion.y,
			       status->at_x, status->at_y)) {
		if (status->draw)
		    (*status->draw)(w,
				    status->at_x, status->at_y, Highlight);
		status->at_x = InPixmapX(PW, event->xmotion.x);
		status->at_y = InPixmapY(PW, event->xmotion.y);
		if (status->draw)
		    (*status->draw)(w,
				    status->at_x, status->at_y, Highlight);
	    }
	}      
	break;
    }
}
