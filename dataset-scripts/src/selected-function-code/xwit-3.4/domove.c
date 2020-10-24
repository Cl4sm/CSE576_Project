static void
domove(Window window, int x, int y, int right, int bottom)
{
	XWindowChanges values;
	unsigned int value_mask;

	if (right || bottom) {
		XWindowAttributes win_attr, frame_attr;
		Window wmframe;

		if (XGetWindowAttributes(dpy, window, &win_attr) == 0)
			Fatal_Error("XGetWindowAttributes(0x%x)", window);

		/* find our window manager frame, if any */
		for (wmframe = window; ; ) {
			Status status;
			Window wroot, parent;
			Window *childlist;
			unsigned int ujunk;

			status = XQueryTree(dpy, wmframe,
				&wroot, &parent, &childlist, &ujunk);
			if (parent == wroot || !parent || !status)
				break;
			wmframe = parent;
			if (status && childlist)
				XFree((char *) childlist);
		}

#ifndef FVWM2
		/*
		 * Norman R. McBride <norm@city.ac.uk> reports that
		 * this code doesn't work with fvwm2, and I don't have
		 * a fix yet, sorry. - DD
		 */
		if (wmframe != window) {
			if (!XGetWindowAttributes(dpy, wmframe, &frame_attr))
				Fatal_Error("XGetWindowAttributes(0x%x)",
					wmframe);

			win_attr.width = frame_attr.width;
			win_attr.height = frame_attr.height;
			win_attr.border_width +=
				frame_attr.border_width;
		}
#endif /* !FVWM2 */

		if (right)
			x += DisplayWidth(dpy, screen) -
				win_attr.width -
				win_attr.border_width;

		if (bottom)
			y += DisplayHeight(dpy, screen) -
				win_attr.height -
				win_attr.border_width;
	}

	values.x = x;
	values.y = y;
	value_mask = CWX | CWY;

	if (XReconfigureWMWindow(dpy, window, screen,
		value_mask, &values) == 0)
		Fatal_Error("move failed");
}