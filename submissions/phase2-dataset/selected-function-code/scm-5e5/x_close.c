SCM x_close(obj)
     SCM obj;
{
  ASRTER(NIMP(obj), obj, ARG1, s_x_close);
  if (WINDOWP(obj)) {
    Display *dpy;
    ASRTER(!(CAR((SCM)obj) & SCROOT), obj, ARG1, s_x_close);
    if (CLOSEDP(obj)) return UNSPECIFIED;
    DEFER_INTS;
    dpy = XWINDISPLAY(obj);
    free_xwindow((CELLPTR)obj);
    XFlush(dpy);
    ALLOW_INTS;
  } else {
    ASRTER(DISPLAYP(obj), obj, ARG1, s_x_close);
    DEFER_INTS;
    free_xdisplay((CELLPTR)obj);
    ALLOW_INTS;
  }
  return UNSPECIFIED;
}
