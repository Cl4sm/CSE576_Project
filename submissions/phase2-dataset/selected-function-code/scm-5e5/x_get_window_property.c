SCM x_get_window_property(swin, sprop, sargs)
     SCM swin, sprop, sargs;
{
  struct xs_Window *xwn;
  Atom property;
  Atom actual_type_return;
  int actual_format_return;
  unsigned long nitems_return;
  unsigned long bytes_after_return;
  unsigned char *prop_return;
  int sarglen = ilength(sargs);
  ASRTER(IMP(sprop) ? INUMP(sprop) : STRINGP(sprop),
	 sprop, ARG2, s_x_get_window_property);
  ASRTER(sarglen >= 0 && sarglen < 2, sargs, WNA, s_x_get_window_property);
  if (1 == sarglen) {
    ASRTER(NFALSEP(booleanp(CAR(sargs))), sargs, ARG3, s_x_get_window_property);
  }
  ASRTER(NIMP(swin) && WINDOWP(swin), swin, ARG1, s_x_map_window);
  xwn = WINDOW(swin);
  if (INUMP(sprop))
    property = INUM(sprop);
  else
    property = XInternAtom(xwn->dpy, CHARS(sprop), !0);

  if (None == property) return BOOL_F;
  if (XGetWindowProperty(xwn->dpy, xwn->p.win, property, 0L, 65536L,
			 (1 == sarglen) && NFALSEP(CAR(sargs)), AnyPropertyType,
			 &actual_type_return, &actual_format_return,
			 &nitems_return, &bytes_after_return,
			 &prop_return)
      != Success)
    return BOOL_F;
  {
    SCM ans = x_propdata2scm(actual_type_return, actual_format_return,
			     nitems_return, prop_return);
    XFree(prop_return);
    return ans;
  }
}
