     Widget       w;
     PWStatus    *status;
     XEvent      *event;
{
    PixmapWidget PW = (PixmapWidget) w;

    if (_PWDEBUG)
	fprintf(stderr, "D1PH ");

    switch (event->type) {
    
    case ButtonPress:
	if (event->xbutton.state != status->state) return;
	if (!QuerySet(status->at_x, status->at_y)) {
	    PWStoreToBuffer(w);
	    status->value = Value(PW, event->xbutton.button);
	    status->time = event->xbutton.time;
	    status->at_x = InPixmapX(PW, event->xbutton.x);
	    status->at_y = InPixmapY(PW, event->xbutton.y);
	    status->success = (status->draw != NULL);
	    if (status->draw)
		(*status->draw)(w,
				status->at_x, status->at_y, status->value);
	}
	break;
	
    case ButtonRelease:
	if (QuerySet(status->at_x, status->at_y)) {
	    status->value = Value(PW, event->xbutton.button);
	    status->time = event->xbutton.time;
	    status->at_x = InPixmapX(PW, event->xbutton.x);
	    status->at_y = InPixmapY(PW, event->xbutton.y);
	    status->success = (status->draw != NULL);
      
	    PWTerminateRequest(w, TRUE);
	}
	break;

    case MotionNotify:
	if (QuerySet(status->at_x, status->at_y)) {
	    if (!QueryInSquare(PW, event->xmotion.x, event->xmotion.y,
			       status->at_x, status->at_y)) {
		status->at_x = InPixmapX(PW, event->xmotion.x);
		status->at_y = InPixmapY(PW, event->xmotion.y);
		if (status->draw)
		    (*status->draw)(w,
				    status->at_x, status->at_y, status->value);
	    }
	}
	break;

    }
}
