     SCM oargs;
{
  SCM sgc, args = oargs, sval = BOOL_F;
  SCM vals = cons(BOOL_T, EOL), valend = vals;
  struct xs_GContext *xgc;
  unsigned long valuemask;
  XGCValues vlu;
  int attr, len = ilength(args);
/*    (void)memset((char *)&vlu, 0, sizeof(XGCValues)); */
  ASRTER(len > 0, oargs, WNA, s_x_gc_ref);
  if (1==len--) return EOL;
  sgc = CAR(args); args = CDR(args);
  ASRTER(NIMP(sgc) && GCONTEXTP(sgc), sgc, ARG1, s_x_gc_ref);
  xgc = GCONTEXT(sgc);
  valuemask = args2valmask(args, s_gc);
/*    printf("valuemask = %lx\n", valuemask); */
  valuemask &= (GCFunction | GCPlaneMask | GCForeground | GCBackground |
		GCLineWidth | GCLineStyle | GCCapStyle | GCJoinStyle |
		GCFillStyle | GCFillRule |
		GCTileStipXOrigin | GCTileStipYOrigin |
		GCSubwindowMode | GCGraphicsExposures |
		GCClipXOrigin | GCClipYOrigin | GCDashOffset | GCArcMode);
  if (!XGetGCValues(xgc->dpy, xgc->gc, valuemask, &vlu)) return BOOL_F;
  while (len) {
    attr = theint(CAR(args), s_gc); args = CDR(args);
    switch (attr) {

    case GCFunction:	sval = MAKINUM(vlu.function  ); break;
    case GCPlaneMask:	sval = MAKINUM(vlu.plane_mask); break;
    case GCForeground:	sval = MAKINUM(vlu.foreground); break;
    case GCBackground:	sval = MAKINUM(vlu.background); break;
    case GCLineWidth:	sval = MAKINUM(vlu.line_width); break;
    case GCLineStyle:	sval = MAKINUM(vlu.line_style); break;
    case GCCapStyle:	sval = MAKINUM(vlu.cap_style ); break;
    case GCJoinStyle:	sval = MAKINUM(vlu.join_style); break;
    case GCFillStyle:	sval = MAKINUM(vlu.fill_style); break;
    case GCFillRule:	sval = MAKINUM(vlu.fill_rule ); break;
    case GCTile:	sval = xgc->tile; break;
    case GCStipple:	sval = xgc->stipple; break;
    case GCTileStipXOrigin: sval = MAKINUM(vlu.ts_x_origin); break;
    case GCTileStipYOrigin: sval = MAKINUM(vlu.ts_y_origin); break;
    case (GCTileStipXOrigin | GCTileStipYOrigin):
      sval = cons2(MAKINUM(vlu.ts_x_origin), MAKINUM(vlu.ts_y_origin), EOL);
      break;
    case GCFont:	sval = xgc->font; break;
    case GCSubwindowMode: sval = MAKINUM(vlu.subwindow_mode); break;
    case GCGraphicsExposures:
      sval = x_make_bool(vlu.graphics_exposures); break;
    case GCClipXOrigin:	sval = MAKINUM(vlu.clip_x_origin); break;
    case GCClipYOrigin:	sval = MAKINUM(vlu.clip_y_origin); break;
    case (GCClipXOrigin | GCClipYOrigin):
      sval = cons2(MAKINUM(vlu.clip_x_origin),
		    MAKINUM(vlu.clip_y_origin), EOL);
      break;
    case GCClipMask:	sval = xgc->clipmask; break;
    case GCDashOffset:	sval = MAKINUM(vlu.dash_offset); break;
    case GCDashList:	sval = MAKINUM(vlu.dashes); break;
    case GCArcMode:	sval = MAKINUM(vlu.arc_mode); break;

    default: ASRTER(0, MAKINUM(attr), ARGn, s_x_gc_ref);
    }
    CAR(valend) = sval;
    CDR(valend) = cons(BOOL_T, EOL);
    len -= 1;
    if (len) valend = CDR(valend);
    else CDR(valend) = EOL;
  }
  return vals;
}
