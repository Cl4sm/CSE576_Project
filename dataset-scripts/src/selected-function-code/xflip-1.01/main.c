main(argc, argv)
int	argc;
char	**argv;
{
char	*display = NULL;
unsigned long	vmask, gcmask;
XSetWindowAttributes	xswat;
XGCValues	gcvals;
int	i;
int	times = -1;
Drawable	drawable;
int	x, y, w, h;

	/* process args */
	for (i = 1; i < argc; i++)	{
		if (strncmp(argv[i], "-dis", 4) == 0)	{
			if (argv[i+1])
				display = argv[++i];
			else
				usage();
		} else if (strncmp(argv[i], "-t", 2) == 0)	{
			if (argv[i+1])
				times = atoi(argv[++i]);
			else
				usage();
		} else if (strncmp(argv[i], "-p", 2) == 0)	{
			do_planes = True;
		} else if (strncmp(argv[i], "-f", 2) == 0)	{
			do_fast = True;
			do_half = 2;
		} else if (strncmp(argv[i], "-r", 2) == 0)	{
			do_random = True;
			if (times < 0)
				times = rnd(NUM_TIMES) + MIN_TIMES;
		} else if (strncmp(argv[i], "-v", 2) == 0)	{
			do_vert = True;
                } else if (strncmp(argv[i], "-o", 2) == 0)      {
                        do_oblic = True;
		}	else
			usage();
	}

	if ((dpy = XOpenDisplay(display)) == NULL)	{
		fprintf(stderr, "can't open display\n");
		exit(0);
	}

	screen = DefaultScreen(dpy);

	if (!do_random)	{
		xswat.override_redirect = True;
		xswat.do_not_propagate_mask = KeyPressMask | KeyReleaseMask |
			ButtonPressMask | ButtonReleaseMask;
		vmask = CWOverrideRedirect | CWDontPropagate;
		win = XCreateWindow(dpy, RootWindow(dpy, screen), 0, 0, 
			DisplayWidth(dpy, screen), DisplayHeight(dpy, screen),
			0, CopyFromParent, CopyFromParent, CopyFromParent,
			vmask, &xswat);
	}

	if (do_fast)	{
		pmap = XCreatePixmap(dpy, RootWindow(dpy, screen),
			DisplayWidth(dpy, screen), DisplayHeight(dpy, screen),
			DisplayPlanes(dpy, screen));
	}

	gcvals.graphics_exposures = False;
	/* copyplane gc wants to leave the data alone */
	gcvals.foreground = 1;
	gcvals.background = 0;
	gcvals.subwindow_mode = IncludeInferiors;
	gcmask = GCForeground | GCBackground | GCGraphicsExposures 
				| GCSubwindowMode;
	copygc = XCreateGC(dpy, RootWindow(dpy, screen), gcmask, &gcvals);

	if (do_fast)	{	/* copy the screen contents */
		XCopyArea(dpy, RootWindow(dpy, screen), pmap, copygc,
			0, 0,
			DisplayWidth(dpy, screen), DisplayHeight(dpy, screen),
			0, 0);
	} else if (!do_random)	{
		XMapRaised(dpy, win);
	}

	/* make sure everything's done */
	XSync(dpy, 0);

	/* choose what to mess up */
	if (do_fast)
		drawable = pmap;
	else if (do_random)
		drawable = RootWindow(dpy, screen);
	else
		drawable = win;

	srandom(getpid());
	if (do_random)	{
		for (i = 0; i < times; i++)	{
			if (do_planes)
				XSetPlaneMask(dpy, copygc, 
				    (1 << rnd(DisplayPlanes(dpy, screen))));
			x = rnd(DisplayWidth(dpy, screen));
			y = rnd(DisplayHeight(dpy, screen));
			w = rnd(CHUNK_WIDTH) + MIN_CHUNK_WIDTH;
			h = rnd(CHUNK_HEIGHT) + MIN_CHUNK_HEIGHT;

			/* shift over if going off screen */
			if ((x + w) >= DisplayWidth(dpy, screen))	{
				x -= (w + 1);	/* dont get the last line */
			}
			if ((y + h) >= DisplayHeight(dpy, screen))	{
				y -= (h + 1);
			}

			if (rnd(2))
				flip_vert(drawable, x, y, w, h);
			else
				flip_horz(drawable, x, y, w, h);
			XSync(dpy, 0);
		}
		exit (0);
	} else	{
		if (do_planes)
			XSetPlaneMask(dpy, copygc, 
			    (1 << rnd(DisplayPlanes(dpy, screen))));
		if (do_vert)
			flip_vert(drawable, 0, 0, DisplayWidth(dpy, screen) - 1,
				DisplayHeight(dpy, screen) - 1);
                else if (do_oblic)
                        flip_oblic(drawable, 0, 0, DisplayWidth(dpy, screen) - 1,
                                DisplayHeight(dpy, screen) - 1);
		else
			flip_horz(drawable, 0, 0, DisplayWidth(dpy, screen) - 1,
				DisplayHeight(dpy, screen) - 1);
	}

	/* show the dirty work */
	if (do_fast)	{
		XMapWindow(dpy, win);
		XSync(dpy, 0);	/* be sure its mapped before filling it */
		XCopyArea(dpy, pmap, win, copygc,
			0, 0,
			DisplayWidth(dpy, screen), DisplayHeight(dpy, screen),
			0, 0);
	}

	XSync(dpy, 0);

	/* gloat a bit */
	if (do_fast)
		sleep(GLOAT_TIME);

	exit(0);
}