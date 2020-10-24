     SCM sgc;
     XGCValues *vlu;
     SCM oargs;
{
  struct xs_GContext *xgc = GCONTEXT(sgc);
  SCM sval, args = oargs;
  int attr, len, attr_mask = 0;
/*    (void)memset((char *)vlu, 0, sizeof(XGCValues)); */
  if (!(len = ilength(args))) return 0;
  ASRTER(len > 0 && (! (len & 1)), oargs, WNA, s_gc);
  while (len) {
    ASRTER(NIMP(args), oargs, WNA, s_gc);
    attr = theint(CAR(args), s_gc); args = CDR(args);
    ASRTER(NIMP(args), oargs, WNA, s_gc);
    sval = CAR(args); args = CDR(args);
    attr_mask |= attr;
    switch (attr) {

    case GCFunction:	vlu->function	=  theint(sval, s_gc); break;
    case GCPlaneMask:	vlu->plane_mask	= theuint(sval, s_gc); break;
    case GCForeground:	vlu->foreground	= theuint(sval, s_gc); break;
    case GCBackground:	vlu->background	= theuint(sval, s_gc); break;
    case GCLineWidth:	vlu->line_width	=  theint(sval, s_gc); break;
    case GCLineStyle:	vlu->line_style	=  theint(sval, s_gc); break;
    case GCCapStyle:	vlu->cap_style	=  theint(sval, s_gc); break;
    case GCJoinStyle:	vlu->join_style	=  theint(sval, s_gc); break;
    case GCFillStyle:	vlu->fill_style	=  theint(sval, s_gc); break;
    case GCFillRule:	vlu->fill_rule	=  theint(sval, s_gc); break;
    case GCTile:	vlu->tile	= thepxmap(sval, s_gc);
      xgc->tile = sval;
      break;
    case GCStipple:	vlu->stipple	= thepxmap(sval, s_gc);
      xgc->stipple = sval;
      break;
    case GCTileStipXOrigin: vlu->ts_x_origin = theint(sval, s_gc); break;
    case GCTileStipYOrigin: vlu->ts_y_origin = theint(sval, s_gc); break;
    case (GCTileStipXOrigin | GCTileStipYOrigin): {
      XPoint position;
      scm2XPoint(!0, sval, &position, (char *)ARGn, s_gc);
      vlu->ts_x_origin = position.x;
      vlu->ts_y_origin = position.y;
    } break;
    case GCFont:	vlu->font	= thefont(sval, s_gc);
      xgc->font = sval;
      break;
    case GCSubwindowMode: vlu->subwindow_mode = theint(sval, s_gc); break;
    case GCGraphicsExposures: vlu->graphics_exposures = thebool(sval, s_gc); break;
    case GCClipXOrigin:	vlu->clip_x_origin = theint(sval, s_gc); break;
    case GCClipYOrigin:	vlu->clip_y_origin = theint(sval, s_gc); break;
    case (GCClipXOrigin | GCClipYOrigin): {
      XPoint position;
      scm2XPoint(!0, sval, &position, (char *)ARGn, s_gc);
      vlu->clip_x_origin = position.x;
      vlu->clip_y_origin = position.y;
    } break;
    case GCClipMask:	vlu->clip_mask	= thepxmap(sval, s_gc);
      xgc->clipmask = sval;
      break;
    case GCDashOffset:	vlu->dash_offset = theint(sval, s_gc); break;
    case GCDashList:	vlu->dashes	= (char)theint(sval, s_gc); break;
    case GCArcMode:	vlu->arc_mode	= theint(sval, s_gc); break;

    default: ASRTER(0, MAKINUM(attr), ARGn, s_gc);
    }
    len -= 2;
  }
  return attr_mask;
}
