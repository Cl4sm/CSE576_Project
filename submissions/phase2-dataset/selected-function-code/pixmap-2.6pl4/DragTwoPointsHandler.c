void DragTwoPointsHandler(w, status, event)
    Widget      w;
    PWStatus   *status;
    XEvent     *event;
{
    PixmapWidget PW = (PixmapWidget) w;

    if (_PWDEBUG)
	fprintf(stderr, "D2PH ");

    switch (event->type) {
	
    case ButtonPress:
	if (event->xbutton.state != status->state) return;
	if (!QuerySet(status->from_x, status->from_y)) {
	    PWStoreToBuffer(w);
	    status->value = Value(PW, event->xbutton.button);
	    status->time = event->xbutton.time;
	    status->from_x = InPixmapX(PW, event->xbutton.x);
	    status->from_y = InPixmapY(PW, event->xbutton.y);
	    status->to_x = InPixmapX(PW, event->xbutton.x);
	    status->to_y = InPixmapY(PW, event->xbutton.y);
	    status->success = (status->draw != NULL);
	    if (status->draw)
		(*status->draw)(w,
				status->from_x, status->from_y, 
				status->to_x, status->to_y, status->value);
	}
	break;
	
    case ButtonRelease:
	if (QuerySet(status->from_x, status->from_y)) {
	    status->value = Value(PW, event->xbutton.button);
	    status->time = event->xbutton.time;	    
	    status->from_x = status->to_x;
	    status->from_y = status->to_y;
	    status->to_x = InPixmapX(PW, event->xbutton.x);
	    status->to_y = InPixmapY(PW, event->xbutton.y);
	    status->success = True;
	    
	    PWTerminateRequest(w, TRUE);
	}
	break;
	
    case MotionNotify:
	if (QuerySet(status->from_x, status->from_y)) {
	    if (QuerySet(status->to_x, status->to_y)) {
		if (!QueryInSquare(PW, event->xmotion.x, event->xmotion.y,
				   status->to_x, status->to_y)) {
		    status->from_x = status->to_x;
		    status->from_y = status->to_y;
		    status->to_x = InPixmapX(PW, event->xmotion.x);
		    status->to_y = InPixmapY(PW, event->xmotion.y);
		    if (status->draw)
			(*status->draw)(w,
					status->from_x, status->from_y, 
					status->to_x, status->to_y, status->value);
		}
	    }
	}
	break;
    }
}
