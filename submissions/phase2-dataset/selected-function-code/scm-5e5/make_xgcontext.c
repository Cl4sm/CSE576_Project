SCM make_xgcontext(d, screen_number, gc, rootp)
     SCM d;
     int screen_number;
     GC gc;
     int rootp;
{
  SCM z;
  struct xs_GContext *xgc;
  DEFER_INTS;
  z = must_malloc_cell((long)sizeof(struct xs_GContext),
		       (SCM)tc16_xgcontext | (rootp ? SCROOT : 0L),
		       s_gc);
  xgc = GCONTEXT(z);
  xgc->display = d;
  xgc->screen_number = screen_number;
  xgc->dpy = XDISPLAY(d);
  xgc->gc = gc;
  xgc->font = BOOL_F;
  xgc->tile = BOOL_F;
  xgc->stipple = BOOL_F;
  xgc->clipmask = BOOL_F;
  ALLOW_INTS;
  return z;
}
