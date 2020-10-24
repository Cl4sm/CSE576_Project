void openXwindow(int argc, char *argv[], char *pixmap_bytes[], char *pixmask_bits, int pixmask_width, int pixmask_height) {

    unsigned int	borderwidth = 1;
    XClassHint		classHint;
    char			*display_name = NULL;
    char			*wname = argv[0];
    XTextProperty	name;

    XGCValues		gcv;
    unsigned long	gcm;

    char			*geometry = NULL;

    int				dummy=0;
    int				i, wx, wy;

    for (i=1; argv[i]; i++) {
        if (!strcmp(argv[i], "-display")) {
            display_name = argv[i+1];
            i++;
        }
        if (!strcmp(argv[i], "-geometry")) {
            geometry = argv[i+1];
            i++;
        }
    }

    if (!(display = XOpenDisplay(display_name))) {
        fprintf(stderr, "%s: can't open display %s\n",
                wname, XDisplayName(display_name));
        exit(1);
    }
    screen  = DefaultScreen(display);
    Root    = RootWindow(display, screen);
    d_depth = DefaultDepth(display, screen);
    x_fd    = XConnectionNumber(display);

    /* Convert XPM to XImage */
    GetXPM(&wmgen, pixmap_bytes);

    /* Create a window to hold the stuff */
    mysizehints.flags = USSize | USPosition;
    mysizehints.x = 0;
    mysizehints.y = 0;

    back_pix = GetColor("white");
    fore_pix = GetColor("black");

    XWMGeometry(display, screen, Geometry, NULL, borderwidth, &mysizehints,
                &mysizehints.x, &mysizehints.y,&mysizehints.width,&mysizehints.height, &dummy);

    mysizehints.width = 64;
    mysizehints.height = 64;

    win = XCreateSimpleWindow(display, Root, mysizehints.x, mysizehints.y,
                              mysizehints.width, mysizehints.height, borderwidth, fore_pix, back_pix);

    iconwin = XCreateSimpleWindow(display, win, mysizehints.x, mysizehints.y,
                                  mysizehints.width, mysizehints.height, borderwidth, fore_pix, back_pix);

    /* Activate hints */
    XSetWMNormalHints(display, win, &mysizehints);
    classHint.res_name = wname;
    classHint.res_class = wname;
    XSetClassHint(display, win, &classHint);

    XSelectInput(display, win, ButtonPressMask | ExposureMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
    XSelectInput(display, iconwin, ButtonPressMask | ExposureMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);

    if (XStringListToTextProperty(&wname, 1, &name) == 0) {
        fprintf(stderr, "%s: can't allocate window name\n", wname);
        exit(1);
    }

    XSetWMName(display, win, &name);

    /* Create GC for drawing */

    gcm = GCForeground | GCBackground | GCGraphicsExposures;
    gcv.foreground = fore_pix;
    gcv.background = back_pix;
    gcv.graphics_exposures = 0;
    NormalGC = XCreateGC(display, Root, gcm, &gcv);

    /* ONLYSHAPE ON */

    pixmask = XCreateBitmapFromData(display, win, pixmask_bits, pixmask_width, pixmask_height);

    XShapeCombineMask(display, win, ShapeBounding, 0, 0, pixmask, ShapeSet);
    XShapeCombineMask(display, iconwin, ShapeBounding, 0, 0, pixmask, ShapeSet);

    /* ONLYSHAPE OFF */

    mywmhints.initial_state = WithdrawnState;
    mywmhints.icon_window = iconwin;
    mywmhints.icon_x = mysizehints.x;
    mywmhints.icon_y = mysizehints.y;
    mywmhints.window_group = win;
    mywmhints.flags = StateHint | IconWindowHint | IconPositionHint | WindowGroupHint;

    XSetWMHints(display, win, &mywmhints);

    XSetCommand(display, win, argv, argc);
    XMapWindow(display, win);

    if (geometry) {
        if (sscanf(geometry, "+%d+%d", &wx, &wy) != 2) {
            fprintf(stderr, "Bad geometry string.\n");
            exit(1);
        }
        XMoveWindow(display, win, wx, wy);
    }
}