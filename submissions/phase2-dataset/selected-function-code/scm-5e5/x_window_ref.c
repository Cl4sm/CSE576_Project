     SCM oargs;
{
  SCM swn, args = oargs, sval = BOOL_F;
  SCM vals = cons(BOOL_T, EOL), valend = vals;
  struct xs_Window *xwn;
  XWindowAttributes vlu;
  int attr, len = ilength(args);
  /* (void)memset((char *)&vlu, 0, sizeof(XWindowAttributes)); */
  ASRTER(len > 0, oargs, WNA, s_x_window_ref);
  if (1==len--) return EOL;
  swn = CAR(args); args = CDR(args);
  ASRTER(NIMP(swn) && WINDOWP(swn), swn, ARG1, s_x_window_ref);
  xwn = WINDOW(swn);
  if (!XGetWindowAttributes(xwn->dpy, xwn->p.win, &vlu)) return BOOL_F;
  while (len) {
    attr = theint(CAR(args), s_x_window_ref); args = CDR(args);
    switch (attr) {

    case CWBackPixel:	 sval = MAKINUM(vlu.backing_pixel); break;
    case CWBitGravity:	 sval = MAKINUM(vlu.bit_gravity); break;
    case CWWinGravity:	 sval = MAKINUM(vlu.win_gravity); break;
    case CWBackingStore: sval = MAKINUM(vlu.backing_store); break;
    case CWBackingPlanes:sval = MAKINUM(vlu.backing_planes); break;
    case CWBackingPixel: sval = MAKINUM(vlu.backing_pixel); break;
    case CWOverrideRedirect:sval = x_make_bool(vlu.override_redirect); break;
    case CWSaveUnder:	 sval = x_make_bool(vlu.save_under); break;
    case CWEventMask:	 sval = MAKINUM(vlu.your_event_mask); break;
    case CWDontPropagate:sval = MAKINUM(vlu.do_not_propagate_mask); break;
    case CWColormap:	 sval = make_xcolormap(xwn->display, vlu.colormap); break;

    default: ASRTER(0, MAKINUM(attr), ARGn, s_x_window_ref);
    }
    CAR(valend) = sval;
    CDR(valend) = cons(BOOL_T, EOL);
    len -= 1;
    if (len) valend = CDR(valend);
    else CDR(valend) = EOL;
  }
  return vals;
}
