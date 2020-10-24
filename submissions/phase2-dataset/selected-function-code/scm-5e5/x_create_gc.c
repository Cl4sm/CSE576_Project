SCM x_create_gc(args)
     SCM args;
{
  SCM swin;
  struct xs_Window *xsw;
  struct xs_GContext *xgc;
  XGCValues v;
  unsigned long mask;
  SCM ans;

  ASRTER(NIMP(args), args, WNA, s_x_create_gc);
  swin = CAR(args); args = CDR(args);
  ASRTER(NIMP(swin) && WINDOWP(swin), swin, ARG1, s_x_create_gc);
  xsw = WINDOW(swin);
  ans = make_xgcontext(xsw->display, xsw->screen_number,
		       XCreateGC(xsw->dpy, xsw->p.drbl, 0L, &v), 0);
  xgc = GCONTEXT(ans);
  mask = args2xgcvalues(ans, &v, args);
  XChangeGC(xgc->dpy, xgc->gc, mask, &v);
  return ans;
}
