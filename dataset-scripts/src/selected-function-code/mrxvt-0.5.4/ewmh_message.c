Status
ewmh_message( Display *dpy, Window root_win, Window client_win,
	Atom msgAtom, long d0, long d1, long d2, long d3, long d4)
{

    XEvent event;

    if (NOT_ATOM(msgAtom))
	return 1;

    event.xclient.type = ClientMessage;
    event.xclient.serial = 0;
    event.xclient.send_event = True;
    event.xclient.message_type = msgAtom;
    event.xclient.window = client_win;
    event.xclient.format = 32;

    event.xclient.data.l[0] = d0;
    event.xclient.data.l[1] = d1;
    event.xclient.data.l[2] = d2;
    event.xclient.data.l[3] = d3;
    event.xclient.data.l[4] = d4;

    return XSendEvent( dpy, root_win, False,
	    SubstructureRedirectMask | SubstructureNotifyMask,
	    &event);
}