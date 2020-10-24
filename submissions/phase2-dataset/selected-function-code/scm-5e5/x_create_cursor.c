     SCM sdpy, scsr, sargs;
{
  Cursor cursor;

  switch (ilength(sargs)) {
  default: ASRTER(0, sargs, WNA, s_x_create_cursor);
  case 0: {
    SCM shape;
    ASRTER(NIMP(sdpy) && DISPLAYP(sdpy), sdpy, ARG1, s_x_create_cursor);
    shape = thevalue(scsr);
    ASRTER(INUMP(shape) && 0 <= INUM(shape), scsr, ARG2, s_x_create_cursor);
    cursor = XCreateFontCursor(XDISPLAY(sdpy), INUM(shape));
    return make_xcursor(sdpy, cursor);
  }
  case 3: {
    XColor foreground_color, background_color;
    XPoint origin;
    int sts;
    ASRTER(NIMP(sdpy) && WINDOWP(sdpy), sdpy, ARG1, s_x_create_cursor);
    ASRTER(FALSEP(scsr) || (NIMP(scsr) && WINDOWP(scsr)), scsr, ARG2,
	   s_x_create_cursor);
    sts = scm2XColor(CAR(sargs), &foreground_color);
    ASRTER(sts, CAR(sargs), ARG3, s_x_create_cursor);
    sargs = CDR(sargs);
    sts = scm2XColor(CAR(sargs), &background_color);
    ASRTER(sts, CAR(sargs), ARG4, s_x_create_cursor);
    sargs = CDR(sargs);
    scm2XPoint(0, CAR(sargs), &origin, (char*)ARG5, s_x_create_cursor);
    cursor = XCreatePixmapCursor(XWINDISPLAY(sdpy), XWINDOW(sdpy),
				 FALSEP(scsr) ? 0L : XWINDOW(scsr),
				 &foreground_color, &background_color,
				 origin.x, origin.y);
    return make_xcursor(WINDOW(sdpy)->display, cursor);
  }
  case 4: {
    XColor foreground_color, background_color;
    Font source_font, mask_font = 0;
    unsigned int source_char, mask_char = 0;
    int sts;
    source_font = thefont(sdpy, s_x_create_cursor);
    GET_NEXT_INT(source_char, sargs, ARG2, s_x_create_cursor);
    if (FALSEP(CAR(sargs))) {
      sargs = CDR(sargs);
      ASRTER(FALSEP(CAR(sargs)), sargs, ARG4, s_x_create_cursor);
      sargs = CDR(sargs);
    } else {
      mask_font = thefont(CAR(sargs), s_x_create_cursor);
      sargs = CDR(sargs);
      GET_NEXT_INT(mask_char, sargs, ARG4, s_x_create_cursor);
    }
    sts = scm2XColor(CAR(sargs), &foreground_color);
    ASRTER(sts, CAR(sargs), ARG5, s_x_create_cursor);
    sargs = CDR(sargs);
    sts = scm2XColor(CAR(sargs), &background_color);
    ASRTER(sts, CAR(sargs), ARGn, s_x_create_cursor);
    cursor = XCreateGlyphCursor(XWINDISPLAY(sdpy),
				source_font, mask_font, source_char, mask_char,
				&foreground_color, &background_color);
    return make_xcursor(FONT(sdpy)->display, cursor);
  }}
}
