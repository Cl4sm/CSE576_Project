    Widget      w;
    PWStatus   *status;
    XEvent     *event;
{
    PixmapWidget PW = (PixmapWidget) w;

    if (_PWDEBUG)
	fprintf(stderr, "2PH ");
    
    switch (event->type) {
	
    case Expose:
	if (QuerySet(status->from_x, status->from_y) && 
	    QuerySet(status->to_x, status->to_y)) {
	    PWClip(w, 
		   InPixmapX(PW, event->xexpose.x), 
		   InPixmapY(PW, event->xexpose.y),
		   InPixmapX(PW, event->xexpose.x + event->xexpose.width),
		   InPixmapY(PW, event->xexpose.y + event->xexpose.height));
	    if (status->draw)
		(*status->draw)(w,
				status->from_x, status->from_y, 
				status->to_x, status->to_y, Highlight);
	    PWUnclip(w);
	}
	break;
	
    case ButtonPress:
	if (event->xbutton.state != status->state) return;
	if (!QuerySet(status->from_x, status->from_y)) {
	    status->value = Value(PW, event->xbutton.button);
	    status->time = event->xbutton.time;
	    status->from_x = InPixmapX(PW, event->xbutton.x);
	    status->from_y = InPixmapY(PW, event->xbutton.y);
	    status->to_x = InPixmapX(PW, event->xbutton.x);
	    status->to_y = InPixmapY(PW, event->xbutton.y);
	    if (status->draw)
		(*status->draw)(w,
				status->from_x, status->from_y, 
				status->to_x, status->to_y, Highlight);
	}
	break;
	
    case ButtonRelease:
	if (QuerySet(status->from_x, status->from_y)) {
	    if (status->draw)
		(*status->draw)(w,
				status->from_x, status->from_y, 
				status->to_x, status->to_y, Highlight);
	    status->value = Value(PW, event->xbutton.button);
	    status->time = event->xbutton.time;	    
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
		    if (status->draw)
			(*status->draw)(w,
					status->from_x, status->from_y, 
					status->to_x, status->to_y, Highlight);
		    status->to_x = InPixmapX(PW, event->xmotion.x);
		    status->to_y = InPixmapY(PW, event->xmotion.y);
		    if (status->draw)
			(*status->draw)(w,
					status->from_x, status->from_y, 
					status->to_x, status->to_y, Highlight);
		}
	    }
	    else {
		status->to_x = InPixmapX(PW, event->xmotion.x);
		status->to_y = InPixmapY(PW, event->xmotion.y);
		if (status->draw)
		    (*status->draw)(w,
				    status->from_x, status->from_y, 
				    status->to_x, status->to_y, Highlight);
	    }
	}
	break;
    }
}
