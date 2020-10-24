Bool
dockapp_nextevent_or_timeout(XEvent *event, unsigned long miliseconds)
{
    struct timeval timeout;
    fd_set rset;

    XSync(display, False);
    if (XPending(display)) {
	XNextEvent(display, event);
	return True;
    }

    timeout.tv_sec = miliseconds / 1000;
    timeout.tv_usec = (miliseconds % 1000) * 1000;

    FD_ZERO(&rset);
    FD_SET(ConnectionNumber(display), &rset);
    if (select(ConnectionNumber(display)+1, &rset, NULL, NULL, &timeout) > 0) {
	XNextEvent(display, event);
	if (event->type == ClientMessage) {
	    if (event->xclient.data.l[0] == delete_win) {
		XDestroyWindow(display,event->xclient.window);
		XCloseDisplay(display);
		exit(0);
	    }
	}
	if (dockapp_iswindowed) {
		event->xbutton.x -= offset_w;
		event->xbutton.y -= offset_h;
	}
	return True;
    }

    return False;
}