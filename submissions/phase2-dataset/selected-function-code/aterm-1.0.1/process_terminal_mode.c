process_terminal_mode(int mode, int priv, unsigned int nargs, int arg[])
{
    unsigned int    i;
    int             state;

    if (nargs == 0)
	return;

/* make lo/hi boolean */
    switch (mode) {
    case 'l':
	mode = 0;
	break;
    case 'h':
	mode = 1;
	break;
    }

    switch (priv) {
    case 0:
	if (mode && mode != 1)
	    return;		/* only do high/low */
	for (i = 0; i < nargs; i++)
	    switch (arg[i]) {
	    case 4:
		scr_insert_mode(mode);
		break;
	    /* case 38:  TEK mode */
	    }
	break;

#define PrivCases(bit)							\
    if (mode == 't')							\
	state = !(PrivateModes & bit);					\
    else								\
        state = mode;							\
    switch (state) {							\
    case 's':								\
	SavedModes |= (PrivateModes & bit);				\
	continue;							\
	break;								\
    case 'r':								\
	state = (SavedModes & bit) ? 1 : 0;				\
	/* FALLTHROUGH */						\
    default:								\
	PrivMode (state, bit);						\
    }

    case '?':
	for (i = 0; i < nargs; i++)
	    switch (arg[i]) {
	    case 1:		/* application cursor keys */
		PrivCases(PrivMode_aplCUR);
		break;

	    /* case 2:   - reset charsets to USASCII */

	    case 3:		/* 80/132 */
		PrivCases(PrivMode_132);
		if (PrivateModes & PrivMode_132OK)
		    set_width(state ? 132 : 80);
		break;

	    /* case 4:   - smooth scrolling */

	    case 5:		/* reverse video */
		PrivCases(PrivMode_rVideo);
		scr_rvideo_mode(state);
		break;

	    case 6:		/* relative/absolute origins  */
		PrivCases(PrivMode_relOrigin);
		scr_relative_origin(state);
		break;

	    case 7:		/* autowrap */
		PrivCases(PrivMode_Autowrap);
		scr_autowrap(state);
		break;

	    /* case 8:   - auto repeat, can't do on a per window basis */

	    case 9:		/* X10 mouse reporting */
		PrivCases(PrivMode_MouseX10);
	    /* orthogonal */
		if (PrivateModes & PrivMode_MouseX10)
		    PrivateModes &= ~(PrivMode_MouseX11);
		break;
# ifdef menuBar_esc
	    case menuBar_esc:
		PrivCases(PrivMode_menuBar);
		map_menuBar(state);
		break;
# endif
#ifdef scrollBar_esc
	    case scrollBar_esc:
		PrivCases(PrivMode_scrollBar);
		map_scrollBar(state);
		break;
#endif
	    case 25:		/* visible/invisible cursor */
		PrivCases(PrivMode_VisibleCursor);
		scr_cursor_visible(state);
		break;

	    case 35:
		PrivCases(PrivMode_ShiftKeys);
		break;

	    case 40:		/* 80 <--> 132 mode */
		PrivCases(PrivMode_132OK);
		break;

	    case 47:		/* secondary screen */
		PrivCases(PrivMode_Screen);
		scr_change_screen(state);
		break;

	    case 66:		/* application key pad */
		PrivCases(PrivMode_aplKP);
		break;

	    case 67:
#ifndef NO_BACKSPACE_KEY
		if (PrivateModes & PrivMode_HaveBackSpace) {
		    PrivCases(PrivMode_BackSpace);
		}
#endif
		break;

	    case 1000:		/* X11 mouse reporting */
		PrivCases(PrivMode_MouseX11);
	    /* orthogonal */
		if (PrivateModes & PrivMode_MouseX11)
		    PrivateModes &= ~(PrivMode_MouseX10);
		break;
#if 0
	    case 1001:
		break;		/* X11 mouse highlighting */
#endif
	    case 1010:		/* scroll to bottom on TTY output inhibit */
		PrivCases(PrivMode_TtyOutputInh);
		if (PrivateModes & PrivMode_TtyOutputInh)
		    Options &= ~Opt_scrollTtyOutput;
		else
		    Options |= Opt_scrollTtyOutput;
		break;
	    case 1011:		/* scroll to bottom on key press */
		PrivCases(PrivMode_Keypress);
		if (PrivateModes & PrivMode_Keypress)
		    Options |= Opt_scrollKeypress;
		else
		    Options &= ~Opt_scrollKeypress;
		break;
	    }
#undef PrivCases
	break;
    }
}
