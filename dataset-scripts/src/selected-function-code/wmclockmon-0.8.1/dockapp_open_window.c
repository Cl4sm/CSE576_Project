void
dockapp_open_window(char *display_specified, char *appname,
		    unsigned w, unsigned h, int argc, char **argv)
{
    XClassHint	    *classhint;
    XWMHints	    *wmhints;
    Status	    stat;
    XTextProperty   title;
    XSizeHints	    sizehints;
    Window	    root;
    int		    ww, wh;

    /* Open Connection to X Server */
    display = XOpenDisplay(display_specified);
    if (!display) {
	fprintf(stderr, "%s: could not open display %s!\n", argv[0],
		XDisplayName(display_specified));
	exit(1);
    }
    root = DefaultRootWindow(display);

    width = w;
    height = h;

    if (dockapp_iswindowed) {
	offset_w = (WINDOWED_SIZE_W - w) / 2;
	offset_h = (WINDOWED_SIZE_H - h) / 2;
	ww = WINDOWED_SIZE_W;
	wh = WINDOWED_SIZE_H;
    } else {
	offset_w = offset_h = 0;
	ww = w;
	wh = h;
    }

    /* Create Windows */
    icon_window = XCreateSimpleWindow(display, root, 0, 0, ww, wh, 0, 0, 0);
    if (dockapp_isbrokenwm) {
	window = XCreateSimpleWindow(display, root, 0, 0, ww, wh, 0, 0, 0);
    } else {
	window = XCreateSimpleWindow(display, root, 0, 0, 1, 1, 0, 0, 0);
    }

    /* Set ClassHint */
    classhint = XAllocClassHint();
    if (classhint == NULL) {
	fprintf(stderr, "%s: can't allocate memory for wm hints!\n", argv[0]);
	exit(1);
    }
    classhint->res_class = "DockApp";
    classhint->res_name = appname;
    XSetClassHint(display, window, classhint);
    XFree(classhint);

    /* Set WMHints */
    wmhints = XAllocWMHints();
    if (wmhints == NULL) {
	fprintf(stderr, "%s: can't allocate memory for wm hints!\n", argv[0]);
	exit(1);
    }
    wmhints->flags = IconWindowHint | WindowGroupHint;
    if (!dockapp_iswindowed) {
	wmhints->flags |= StateHint;
	wmhints->initial_state = WithdrawnState;
    }
    wmhints->window_group = window;
    wmhints->icon_window = icon_window;
    XSetWMHints(display, window, wmhints);
    XFree(wmhints);

    /* Set WM Protocols */
    delete_win = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols (display, icon_window, &delete_win, 1);

    /* Set Size Hints */
    sizehints.flags = USSize;
    if (!dockapp_iswindowed) {
	sizehints.flags |= USPosition;
	sizehints.x = sizehints.y = 0;
    } else {
	sizehints.flags |= PMinSize | PMaxSize;
	sizehints.min_width = sizehints.max_width = WINDOWED_SIZE_W;
	sizehints.min_height = sizehints.max_height = WINDOWED_SIZE_H;
    }
    sizehints.width = ww;
    sizehints.height = wh;
    XSetWMNormalHints(display, icon_window, &sizehints);

    /* Set WindowTitle for AfterStep Wharf */
    stat = XStringListToTextProperty(&appname, 1, &title);
    XSetWMName(display, window, &title);
    XSetWMName(display, icon_window, &title);

    /* Set Command to start the app so it can be docked properly */
    XSetCommand(display, window, argv, argc);

    depth = DefaultDepth(display, DefaultScreen(display));
    gc = DefaultGC(display, DefaultScreen(display));

    XFlush(display);
}