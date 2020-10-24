static void dockimlib2_xinit(DockImlib2 *dock, DockImlib2Prefs *prefs) {
  XSizeHints *xsh;
  int i;
  char *sdisplay = getenv("DISPLAY");
  char *pgeom = NULL;
  int undocked = 0;
  char sdockgeom[40];

  assert(prefs->argv); // this should be always set ..
  
  if (prefs->flags & DOCKPREF_DISPLAY) sdisplay = prefs->display;
  if (prefs->flags & DOCKPREF_GEOMETRY) { pgeom = prefs->geometry; undocked = 1; }
  
  dock->display = XOpenDisplay(sdisplay);
  if(!dock->display) DOCKIMLIB2_ERR("Couldn't connect to display %s\n", sdisplay);
  dock->screennum = DefaultScreen(dock->display);
  dock->visual = DefaultVisual(dock->display, dock->screennum);
  dock->depth = DefaultDepth(dock->display, dock->screennum);
  dock->colormap = DefaultColormap(dock->display, dock->screennum);
  dock->rootwin = RootWindow(dock->display, dock->screennum);

  dock->atom_WM_DELETE_WINDOW = XInternAtom(dock->display, "WM_DELETE_WINDOW", False);
  dock->atom_WM_PROTOCOLS = XInternAtom(dock->display, "WM_PROTOCOLS", False);

  /* set size hints */
  xsh = XAllocSizeHints(); assert(xsh);
  xsh->flags = 0;
  xsh->width = dock->w;
  xsh->height = dock->h;
  xsh->x = xsh->y = 0;

  snprintf(sdockgeom, sizeof sdockgeom, "%dx%d+0+0", prefs->dockapp_size, prefs->dockapp_size);

  xsh->flags = XWMGeometry(dock->display, dock->screennum, pgeom, sdockgeom, 0,
                           xsh, &xsh->x, &xsh->y, &xsh->width, &xsh->height, &i);
  if (undocked) {
    dock->win_width  = dock->w = xsh->width; 
    dock->win_height = dock->h = xsh->height; 
    dock->x0 = dock->y0 = 0;
  }
  xsh->base_width = xsh->width;
  xsh->base_height = xsh->height;
  xsh->flags |= USSize | PMinSize | PMaxSize | PSize;
  xsh->min_height = 24; xsh->min_height = 24;
  xsh->max_width = 500;
  xsh->max_height = 500;

  /* create the application window */
  dock->normalwin = XCreateSimpleWindow(dock->display, dock->rootwin,
                                        xsh->x, xsh->y, xsh->width, xsh->height, 0,
                                        BlackPixel(dock->display, dock->screennum),
                                        WhitePixel(dock->display, dock->screennum));
  
  if(!dock->normalwin) DOCKIMLIB2_ERR("Couldn't create window\n");
  if (!undocked) {
    /* create icon window */
    dock->iconwin = XCreateSimpleWindow(dock->display, dock->rootwin,
                                        xsh->x, xsh->y, xsh->width, xsh->height, 0,
                                        BlackPixel(dock->display, dock->screennum),
                                        WhitePixel(dock->display, dock->screennum));
    if(!dock->iconwin) DOCKIMLIB2_ERR("Couldn't create icon window\n");
    dock->win = dock->iconwin;
  } else {
    dock->iconwin = None;
    dock->win = dock->normalwin;
  }
  dock->iconwin_mapped = dock->normalwin_mapped = 1; /* by default */

  /* start with an empty window in order to get the background pixmap */
  dockimlib2_set_rect_shape(dock,32,32,1,0);
  
  /* set window manager hints */
  if (!undocked) {
    XWMHints *xwmh;
    xwmh = XAllocWMHints();
    xwmh->flags = WindowGroupHint | IconWindowHint | StateHint;
    xwmh->icon_window = dock->iconwin;
    xwmh->window_group = dock->normalwin;
    xwmh->initial_state = WithdrawnState;
    XSetWMHints(dock->display, dock->normalwin, xwmh);
    XFree(xwmh); xwmh = NULL;
  }
  set_window_class_hint(dock->display, dock->normalwin, prefs->argv[0], prefs->argv[0]);

  /* set size hints */
  XSetWMNormalHints(dock->display, dock->normalwin, xsh);

  set_window_title(dock->display, dock->normalwin, "wmhdplop", "wmhdplop");

  /* select events to catch */
  {
    long evmask = ExposureMask |  ButtonPressMask | ButtonReleaseMask | VisibilityChangeMask |
      PointerMotionMask | EnterWindowMask | LeaveWindowMask | StructureNotifyMask;
    XSelectInput(dock->display, dock->normalwin, evmask);
    if (dock->iconwin) 
      XSelectInput(dock->display, dock->iconwin, evmask);
  }
  XSetWMProtocols(dock->display, dock->normalwin, &dock->atom_WM_DELETE_WINDOW, 1);

  /* set the command line for restarting */
  XSetCommand(dock->display, dock->normalwin, prefs->argv, prefs->argc);

  /* map the main window */
  XMapWindow(dock->display, dock->normalwin);

  XFree(xsh); xsh = NULL;
}