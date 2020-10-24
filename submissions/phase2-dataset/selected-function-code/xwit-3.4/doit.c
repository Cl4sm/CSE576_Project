static void
doit(Window window)
{
	XWindowChanges values;
	unsigned int value_mask;
	XWMHints *wmhp;
	enum functions f;
	int i = 0;

	Gwinfound = 1;

	f = function;
	for (i = 0; i < lastfunc; i++) {
		if ((f & FBIT(i)) == 0)
			continue;

		switch (i) {
		case warp:
			XWarpPointer(dpy, None, window, 0, 0, 0, 0,
				warpx, warpy);
			break;
		case rwarp:
			XWarpPointer(dpy, None, None, 0, 0, 0, 0,
				warpx, warpy);
			break;
		case move:
			domove(window, tox, toy, Gright, Gbottom);
			break;
		case rmove:
			getpos(window, &values.x, &values.y);
			values.x += tox;
			values.y += toy;
			value_mask = CWX | CWY;
			if (XReconfigureWMWindow(dpy, window, screen,
					value_mask, &values) == 0)
				Fatal_Error("rmove failed");
			break;
		case resize:
			doresize(window, towidth, toheight);
			break;
		case colormap:
			XSetWindowColormap(dpy, window, cmap);
			break;
		case print:
			doprint(window);
			break;
		case pop:
			XMapRaised(dpy, window);
			break;
		case focus: {
			static XClientMessageEvent event;

			if (event.type == 0) {
				event.type = ClientMessage;
				event.message_type =
					XInternAtom(dpy, "_NET_ACTIVE_WINDOW", True);
				event.format = 32;
				event.data.l[0] = 1;
				event.data.l[1] = CurrentTime;
			}

			/* if no _NET_ACTIVE_WINDOW, the wm is oldschool... */
			if (event.message_type == 0
			    || !HasNetAtom (dpy, event.message_type))
				XSetInputFocus(dpy, window, CurrentTime, RevertToNone);
			else {
				event.window = window;
				event.data.l[2] = root;
				if (XSendEvent(dpy, root, (Bool) False,
					       SubstructureRedirectMask,
					       (XEvent *) & event) == 0)
					Fatal_Error("send event failed");
			}
			break;
		}
		case raise:
			values.stack_mode = Above;
			value_mask = CWStackMode;
			XConfigureWindow(dpy, window, value_mask, &values);
			break;
		case lower:
			values.stack_mode = Below;
			value_mask = CWStackMode;
			XConfigureWindow(dpy, window, value_mask, &values);
			break;
		case opposite:
			values.stack_mode = Opposite;
			value_mask = CWStackMode;
			XConfigureWindow(dpy, window, value_mask, &values);
			break;
		case circulate:
			XCirculateSubwindowsUp(dpy, window);
			break;
		case uncirculate:
			XCirculateSubwindowsDown(dpy, window);
			break;
		case unmap:
			XUnmapWindow(dpy, window);
			break;
		case icon:
#if iconify_by_sending_client_message
			static XClientMessageEvent event;

			if (event.type == 0) {
				event.type = ClientMessage;
#ifdef XA_WM_CHANGE_STATE
				event.message_type = XA_WM_CHANGE_STATE;
#else
				event.message_type =
					XInternAtom(dpy, "WM_CHANGE_STATE", True);
				if (event.message_type == 0)
					Fatal_Error("no WM_CHANGE_STATE atom");
#endif
				event.format = 32;
				event.data.l[0] = IconicState;
			}

			event.window = window;
			if (XSendEvent(dpy, root, (Bool) False,
					SubstructureRedirectMask | SubstructureNotifyMask,
					(XEvent *) & event) == 0)
				Fatal_Error("send event failed");
#else /* iconify_by_sending_client_message */
			if (XIconifyWindow(dpy, window, screen) == 0)
				Fatal_Error("iconify failed");
#endif /* iconify_by_sending_client_message */
			break;
		case save:
			XForceScreenSaver(dpy, ScreenSaverActive);
			break;
		case nosave:
			XForceScreenSaver(dpy, ScreenSaverReset);
			break;
		case keyrepeat:
		case nokeyrepeat:
			setrepeat();
			break;
		case name:
			XStoreName(dpy, window, wmname);
			break;
		case iconname:
			XSetIconName(dpy, window, wmiconname);
			break;
		case rows:
			/* don't do it twice */
			if (f & FBIT(columns))
				break;
			/* fall through */
		case columns:
			rcresize(f, window);
			break;
		case iconbitmap:
			setbitmap(window);
			break;
		case iconmove:
			wmhp = XGetWMHints(dpy, window);
			if (wmhp == 0)
				Fatal_Error("no WM_HINTS");
			wmhp->flags |= IconPositionHint;
			wmhp->icon_x = Giconx;
			wmhp->icon_y = Gicony;
			XSetWMHints(dpy, window, wmhp);
			XFree(wmhp);
			break;
		case riconmove:
			wmhp = XGetWMHints(dpy, window);
			if (wmhp == 0)
				Fatal_Error("no WM_HINTS");
			if (wmhp->flags & IconPositionHint) {
				wmhp->icon_x += Giconx;
				wmhp->icon_y += Gicony;
				XSetWMHints(dpy, window, wmhp);
			}
			else
				Fatal_Error("no current icon position");
			XFree(wmhp);
			break;
		case F_winattr:
			setwinattr(window);
			break;
		}
	}
}