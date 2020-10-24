SCM make_xdisplay(d)
     Display *d;
{
  SCM z;
  struct xs_screen *scrns;
  struct xs_Display *xsd;
  int idx = ScreenCount(d);
  DEFER_INTS;
  z = must_malloc_cell((long)sizeof(struct xs_Display)
		       + idx * sizeof(struct xs_screen),
		       (SCM)tc16_xdisplay | OPN,
		       s_display);
  xsd = DISPLAY(z);
  xsd->after = BOOL_F;
  xsd->screen_count = idx;
  xsd->dpy = d;
  scrns = (struct xs_screen *)(xsd + 1);
  while (idx--) {
    scrns[idx].root_window = BOOL_F;
    scrns[idx].default_gcontext = BOOL_F;
    scrns[idx].default_visual = BOOL_F;
    scrns[idx].default_colormap = BOOL_F;
  }
  ALLOW_INTS;
  idx = xsd->screen_count;
  while (idx--) {
    scrns[idx].root_window =
      make_xwindow(z, idx, RootWindow(d, idx), (char) 0, (char) 1);
    scrns[idx].default_gcontext =
      make_xgcontext(z, idx, XDefaultGC(d, idx), !0);
    scrns[idx].default_visual =
      make_xvisual(visual2visualinfo(d, DefaultVisual(d, idx)));
    scrns[idx].default_colormap =
      make_xcolormap(z, DefaultColormap(d, idx));
  }
  return z;
}
