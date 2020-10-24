SCM x_create_window(swin, spos, sargs)
     SCM swin, spos, sargs;
{
  XPoint position, size;
  unsigned int border_width;
  Window window;
  int len = ilength(sargs);

  ASRTER(NIMP(swin) && OPWINDOWP(swin), swin, ARG1, s_x_create_window);
  scm2XPoint(!0, spos, &position, (char *)ARG2, s_x_create_window);
  scm2XPoint(0, CAR(sargs), &size, (char *)ARG3, s_x_create_window);
  sargs = CDR(sargs);
  GET_NEXT_INT(border_width, sargs, ARG4, s_x_create_window);
  if (4==len) {
    unsigned long border;
    unsigned long background;
    GET_NEXT_INT(border, sargs, ARG5, s_x_create_window);
    GET_NEXT_INT(background, sargs, ARGn, s_x_create_window);
    window = XCreateSimpleWindow(XWINDISPLAY(swin), XWINDOW(swin),
				 position.x, position.y, /* initial placement */
				 size.x, size.y,
				 border_width,
				 border, background); /* pixel values */
  } else {
    int depth;
    unsigned int class;
    SCM svis;
    unsigned long valuemask;
    XSetWindowAttributes attributes;
    ASRTER(5 <= len, sargs, WNA, s_x_create_window);
    GET_NEXT_INT(depth, sargs, ARG5, s_x_create_window);
    GET_NEXT_INT(class, sargs, ARGn, s_x_create_window);
    svis = CAR(sargs); sargs = CDR(sargs);
    ASRTER(NIMP(svis) && VISUALP(svis), svis, ARGn, s_x_create_window);
    valuemask = args2winattribs(&attributes, sargs);
    window = XCreateWindow(XWINDISPLAY(swin), XWINDOW(swin),
			   position.x, position.y, /* initial placement */
			   size.x, size.y,
			   border_width,
			   depth,
			   class,
			   XVISUAL(svis),
			   valuemask,
			   &attributes);
  }
  return window ? make_xwindow(WINDOW(swin)->display,
			       WINDOW(swin)->screen_number,
			       window, (char) 0, (char) 0)
    : BOOL_F;
}
