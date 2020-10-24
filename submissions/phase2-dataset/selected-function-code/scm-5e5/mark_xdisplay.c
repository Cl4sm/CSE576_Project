static SCM mark_xdisplay(ptr)
     SCM ptr;
{
  if (CLOSEDP(ptr)) return BOOL_F;
  {
    struct xs_Display *xsd = DISPLAY((SCM)ptr);
    struct xs_screen *scrns = (struct xs_screen *)(xsd + 1);
    int idx = xsd->screen_count;
    while (--idx) {
      SCM scmp = scrns[idx].default_colormap;
      gc_mark(scrns[idx].root_window);
      gc_mark(scrns[idx].default_gcontext);
      gc_mark(scrns[idx].default_visual);
      gc_mark(scmp);
      gc_mark (CCC2SCM_P(XcmsCCCOfColormap(xsd->dpy, XCOLORMAP(scmp))));
    }
    gc_mark(scrns[idx].root_window);
    gc_mark(scrns[idx].default_gcontext);
    gc_mark(scrns[idx].default_visual);
    return scrns[idx].default_colormap;
  }
}
