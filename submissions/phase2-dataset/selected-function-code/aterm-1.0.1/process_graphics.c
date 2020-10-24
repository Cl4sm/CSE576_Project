process_graphics(void)
{
    unsigned char   ch, cmd = cmd_getc();

#ifndef RXVT_GRAPHICS
    if (cmd == 'Q') {		/* query graphics */
	tt_printf((unsigned char *) "\033G0\n");	/* no graphics */
	return;
    }
/* swallow other graphics sequences until terminating ':' */
    do
	ch = cmd_getc();
    while (ch != ':');
#else
    int             nargs;
    int             args[NGRX_PTS];
    unsigned char  *text = NULL;

    if (cmd == 'Q') {		/* query graphics */
	tt_printf((unsigned char *) "\033G1\n");	/* yes, graphics (color) */
	return;
    }
    for (nargs = 0; nargs < (sizeof(args) / sizeof(args[0])) - 1; ) {
	int             neg;

	ch = cmd_getc();
	neg = (ch == '-');
	if (neg || ch == '+')
	    ch = cmd_getc();

	for (args[nargs] = 0; isdigit(ch); ch = cmd_getc())
	    args[nargs] = args[nargs] * 10 + (ch - '0');
	if (neg)
	    args[nargs] = -args[nargs];

	nargs++;
	args[nargs] = 0;
	if (ch != ';')
	    break;
    }

    if ((cmd == 'T') && (nargs >= 5)) {
	int             i, len = args[4];

	text = MALLOC((len + 1) * sizeof(char));

	if (text != NULL) {
	    for (i = 0; i < len; i++)
		text[i] = cmd_getc();
	    text[len] = '\0';
	}
    }
    Gr_do_graphics(cmd, nargs, args, text);
#ifdef USE_XIM
    IMSendSpot();
#endif
#endif
}
