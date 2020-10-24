     SCM dat;
     SCM optidx;
     struct display_screen *dspscn;
     char *s_caller;
{
  ASRTGO(NIMP(dat), badarg);
  if (OPDISPLAYP(dat)) {
    dspscn->display = dat;
    dspscn->dpy = XDISPLAY(dat);
    if (UNBNDP(optidx)) dspscn->screen_number = DefaultScreen(dspscn->dpy);
    else if (INUMP(optidx) && (INUM(optidx) < DISPLAY(dat)->screen_count))
      dspscn->screen_number = INUM(optidx);
    else wta(optidx, (char *)ARG2, s_caller);
  }
  else if (OPWINDOWP(dat)) {
    struct xs_Window *xsw = WINDOW(dat);
    dspscn->display = xsw->display;
    dspscn->dpy = xsw->dpy;
    dspscn->screen_number = xsw->screen_number;
    ASRTGO(UNBNDP(optidx), badarg);
  }
  else badarg: wta(dat, (char *)ARG1, s_caller);
}
