SCM xldraw_lines(sdbl, sgc, sargs, funcod, s_caller)
     SCM sdbl, sgc, sargs;
     int funcod;
     char *s_caller;
{
  XPoint pos[2];
  int len;
  SCM sarg;
  ASRTER(NIMP(sdbl) && WINDOWP(sdbl), sdbl, ARG1, s_caller);
  ASRTER(NIMP(sgc) && GCONTEXTP(sgc), sgc, ARG2, s_caller);
 loop:
  if (NULLP(sargs)) return UNSPECIFIED;
  sarg = CAR(sargs); sargs = CDR(sargs);
  if (INUMP(sarg)) {
    ASRTER(NNULLP(sargs), sargs, WNA, s_caller);
    pos[0].x = INUM(sarg);
    GET_NEXT_INT(pos[0].y, sargs, ARGn, s_caller);
    GET_NEXT_INT(pos[1].x, sargs, ARGn, s_caller);
    GET_NEXT_INT(pos[1].y, sargs, ARGn, s_caller);
    goto drawshort;
  }
  len = scm2xpointslen(sarg, s_caller);
  if (len < 0) {
    scm2XPoint(!0, sarg, &(pos[0]), (char *)ARG3, s_caller);
    scm2XPoint(!0, sarg, &(pos[1]), (char *)ARG4, s_caller);
  drawshort:
    switch (funcod) {
    default: wna: wta(sargs, (char *)WNA, s_caller);
    case 0:
      XDrawSegments(XWINDISPLAY(sdbl), XWINDOW(sdbl), XGCONTEXT(sgc),
		    (XSegment *) &(pos[0]), 1);
      goto loop;
    case 1:
      XDrawLines(XWINDISPLAY(sdbl), XWINDOW(sdbl), XGCONTEXT(sgc),
		 &(pos[0]), 2, CoordModeOrigin);
      goto loop;
    }
  } else {
    unsigned long rabase;
    ASRTGO(NULLP(sargs), wna);
    rabase = scm_base_addr(sarg, s_caller);
    switch (funcod) {
    default: goto wna;
    case 0:
      XDrawSegments(XWINDISPLAY(sdbl), XWINDOW(sdbl), XGCONTEXT(sgc),
		    (XSegment *)rabase, len/2);
      return UNSPECIFIED;
    case 1:
      XDrawLines(XWINDISPLAY(sdbl), XWINDOW(sdbl), XGCONTEXT(sgc),
		 (XPoint *)rabase, len, CoordModeOrigin);
      return UNSPECIFIED;
    case 2:
      XFillPolygon(XWINDISPLAY(sdbl), XWINDOW(sdbl), XGCONTEXT(sgc),
		   (XPoint *)rabase, len, Complex, CoordModeOrigin);
      return UNSPECIFIED;
    }
  }
}
