static int args2winattribs(vlu, oargs)
     XSetWindowAttributes *vlu;
     SCM oargs;
{
  SCM sval, args = oargs;
  int attr, len, attr_mask = 0;
  /* (void)memset((char *)vlu, 0, sizeof(XSetWindowAttributes)); */
  if (!(len = ilength(args))) return 0;
  ASRTER(len > 0 && (! (len & 1)), oargs, WNA, s_window);
  while (len) {
    ASRTER(NIMP(args), oargs, WNA, s_window);
    attr = theint(CAR(args), s_window); args = CDR(args);
    ASRTER(NIMP(args), oargs, WNA, s_window);
    sval = CAR(args); args = CDR(args);
    attr_mask |= attr;
    switch (attr) {

    case CWBackPixmap:	vlu->background_pixmap=thepxmap(sval, s_window); break;
    case CWBackPixel:	vlu->background_pixel = theuint(sval, s_window); break;
    case CWBorderPixmap:vlu->border_pixmap    =thepxmap(sval, s_window); break;
    case CWBorderPixel:	vlu->border_pixel     = theuint(sval, s_window); break;
    case CWBitGravity:	vlu->bit_gravity      =  theint(sval, s_window); break;
    case CWWinGravity:	vlu->win_gravity      =  theint(sval, s_window); break;
    case CWBackingStore:vlu->backing_store    =  theint(sval, s_window); break;
    case CWBackingPlanes:vlu->backing_planes  = theuint(sval, s_window); break;
    case CWBackingPixel:vlu->backing_pixel    = theuint(sval, s_window); break;
    case CWOverrideRedirect:vlu->override_redirect =
						thebool(sval, s_window); break;
    case CWSaveUnder:	vlu->save_under	      = thebool(sval, s_window); break;
    case CWEventMask:	vlu->event_mask	      =  theint(sval, s_window); break;
    case CWDontPropagate:vlu->do_not_propagate_mask =
						thebool(sval, s_window); break;
    case CWColormap:	vlu->colormap	      = thecmap(sval, s_window); break;
    case CWCursor:	vlu->cursor	      =  thecsr(sval, s_window); break;

    default: ASRTER(0, MAKINUM(attr), ARGn, s_window);
    }
    len -= 2;
  }
  return attr_mask;
}
