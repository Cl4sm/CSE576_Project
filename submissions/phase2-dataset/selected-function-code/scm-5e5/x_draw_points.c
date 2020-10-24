     SCM sdbl, sgc, sargs;
{
  XPoint pos[1];
  int len;
  SCM sarg;
  ASRTER(NIMP(sdbl) && WINDOWP(sdbl), sdbl, ARG1, s_x_draw_points);
  ASRTER(NIMP(sgc) && GCONTEXTP(sgc), sgc, ARG2, s_x_draw_points);
 loop:
  if (NULLP(sargs)) return UNSPECIFIED;
  sarg = CAR(sargs); sargs = CDR(sargs);
  if (INUMP(sarg)) {
    ASRTER(NNULLP(sargs), sargs, WNA, s_x_draw_points);
    pos[0].x = INUM(sarg);
    GET_NEXT_INT(pos[0].y, sargs, ARGn, s_x_draw_points);
    goto drawshort;
  }
  len = scm2xpointslen(sarg, s_x_draw_points);
  if (len < 0) {
    scm2XPoint(!0, sarg, &(pos[0]), (char *)ARG3, s_x_draw_points);
  drawshort:
    XDrawPoints(XWINDISPLAY(sdbl), XWINDOW(sdbl), XGCONTEXT(sgc),
		&(pos[0]), 1, CoordModeOrigin);
    goto loop;
  } else {
    ASRTER(NULLP(sargs), sargs, WNA, s_x_draw_points);
    XDrawPoints(XWINDISPLAY(sdbl), XWINDOW(sdbl), XGCONTEXT(sgc),
		(XPoint *)scm_base_addr(sarg, s_x_draw_points), len,
		CoordModeOrigin);
    return UNSPECIFIED;
  }
}
