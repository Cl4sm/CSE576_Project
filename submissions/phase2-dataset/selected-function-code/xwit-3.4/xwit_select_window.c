static Window
xwit_select_window(Display *dpy, int current)
{
	Window window = None;
	Window wroot;
	int dummyi;
	unsigned int dummy;

	if (current) {
		XQueryPointer(dpy, RootWindow(dpy, screen),
			&wroot, &window,
			&dummyi, &dummyi, &dummyi, &dummyi, &dummy);
	}
	else {
		printf("\n");
		printf("%s: select window by clicking the mouse\n",
			program_name);
		(void) fflush(stdout);
		window = Select_Window(dpy);
	}
	if (window) {
		if (XGetGeometry(dpy, window, &wroot, &dummyi, &dummyi,
			&dummy, &dummy, &dummy, &dummy) &&
			window != wroot)
			window = XmuClientWindow(dpy, window);
	}
	return window;
}