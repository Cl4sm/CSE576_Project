     SCM obj, s_size, s_depth;
{
  unsigned int depth = INUM(s_depth);
  SCM display;
  Display *dpy;
  int scn;
  Drawable drawable;
  Pixmap p;
  XPoint size;
  if (IMP(obj)) badarg1: wta(obj, (char *)ARG1, s_x_create_pixmap);
  if (OPDISPLAYP(obj)) {
    display = obj;
    dpy = XDISPLAY(display);
    scn = DefaultScreen(dpy);
    drawable = RootWindow(dpy, scn);
  }
  else if (OPWINDOWP(obj)) {
    display = WINDOW(obj)->display;
    dpy = XDISPLAY(display);
    scn = WINDOW(obj)->screen_number;
    drawable = WINDOW(obj)->p.drbl;
  }
  else goto badarg1;
  scm2XPoint(0, s_size, &size, (char *)ARG2, s_x_create_pixmap);
  ASRTER(INUMP(s_depth) && depth >= 0, s_depth, ARG3, s_x_create_pixmap);
  p = XCreatePixmap(dpy, drawable, size.x, size.y, depth);
  return make_xwindow(display, scn, p, (char) 1, (char) 0);
}
