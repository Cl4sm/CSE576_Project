void
init_xlocale(void)
{
#ifndef NO_XLOCALE
#ifndef USE_XIM
    char           *p, *s, buf[32], tmp[1024];
    XIM             xim = NULL;
    XIMStyle        input_style = 0;
    XIMStyles      *xim_styles = NULL;
    int             found;

    Input_Context = NULL;

# if !defined(NO_SETLOCALE) || !defined(NO_XSETLOCALE)
   /* setlocale(LC_CTYPE, ""); */	/* XXX: should we do this? */
# endif
    if (rs_inputMethod == NULL || !*rs_inputMethod) {
	if ((p = XSetLocaleModifiers("")) != NULL)
	    xim = XOpenIM(Xdisplay, NULL, NULL, NULL);
    } else {
	STRCPY(tmp, rs_inputMethod);
	for (s = tmp; *s; s++) {
	    char           *end, *next_s;

	    for (; *s && isspace(*s); s++)
		/* */ ;
	    if (!*s)
		break;
	    for (end = s; (*end && (*end != ',')); end++)
		/* */ ;
	    for (next_s = end--; ((end >= s) && isspace(*end)); end--)
		/* */ ;
	    *++end = '\0';
	    if (*s) {
		STRCPY(buf, "@im=");
		strcat(buf, s);
		if ((p = XSetLocaleModifiers(buf)) != NULL &&
		    (xim = XOpenIM(Xdisplay, NULL, NULL, NULL)) != NULL)
		    break;
	    }
	    if (!*(s = next_s))
		break;
	}
    }

	if (xim == NULL && (p = XSetLocaleModifiers("@im=none")) != NULL)
	xim = XOpenIM(Xdisplay, NULL, NULL, NULL);

    if (xim == NULL) {
	print_error("Failed to open input method");
	return;
    }
    if (XGetIMValues(xim, XNQueryInputStyle, &xim_styles, NULL) || !xim_styles) {
	print_error("input method doesn't support any style");
	XCloseIM(xim);
	return;
    }
    STRCPY(tmp, (rs_preeditType ? rs_preeditType : "Root"));
    for (found = 0, s = tmp; *s && !found; ) {
	unsigned short  i;
	char           *end, *next_s;

	for (; *s && isspace(*s); s++)
	    /* */ ;
	if (!*s)
	    break;
	for (end = s; (*end && (*end != ',')); end++)
	    /* */ ;
	for (next_s = end--; ((end >= s) && isspace(*end));)
	    *end-- = 0;

	if (!strcmp(s, "OverTheSpot"))
	    input_style = (XIMPreeditPosition | XIMStatusArea);
	else if (!strcmp(s, "OffTheSpot"))
	    input_style = (XIMPreeditArea | XIMStatusArea);
	else if (!strcmp(s, "Root"))
	    input_style = (XIMPreeditNothing | XIMStatusNothing);

	for (i = 0; i < xim_styles->count_styles; i++)
	    if (input_style == xim_styles->supported_styles[i]) {
		found = 1;
		break;
	    }
	s = next_s;
    }
    XFree(xim_styles);

    if (found == 0) {
	print_error("input method doesn't support my preedit type");
	XCloseIM(xim);
	return;
    }
/*
 * This program only understands the Root preedit_style yet
 * Then misc.preedit_type should default to:
 *          "OverTheSpot,OffTheSpot,Root"
 *  /MaF
 */
    if (input_style != (XIMPreeditNothing | XIMStatusNothing)) {
	print_error("This program only supports the \"Root\" preedit type");
	XCloseIM(xim);
	return;
    }
    Input_Context = XCreateIC(xim, XNInputStyle, input_style,
			      XNClientWindow, TermWin.parent,
			      XNFocusWindow, TermWin.parent,
			      NULL);

    if (Input_Context == NULL) {
	print_error("Failed to create input context");
	XCloseIM(xim);
    }
#endif                                /* USE_XIM */
#endif				/* NO_XLOCALE */
}
