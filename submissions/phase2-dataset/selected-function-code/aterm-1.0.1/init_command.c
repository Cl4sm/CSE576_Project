init_command(char *argv[])
{
/*
 * Initialize the command connection.
 * This should be called after the X server connection is established.
 */

/* Enable delete window protocol */
    wmDeleteWindow = XInternAtom(Xdisplay, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(Xdisplay, TermWin.parent, &wmDeleteWindow, 1);

#ifdef OFFIX_DND
/* Enable OffiX Dnd (drag 'n' drop) protocol */
    DndProtocol = XInternAtom(Xdisplay, "DndProtocol", False);
    DndSelection = XInternAtom(Xdisplay, "DndSelection", False);
#endif				/* OFFIX_DND */

#ifndef NO_XLOCALE
    init_xlocale();
#else
    setlocale(LC_CTYPE, "");
#endif

#ifdef USE_XIM
    setTermFontSet();
    XRegisterIMInstantiateCallback(Xdisplay, NULL, NULL, NULL, IMInstantiateCallback, NULL);
#endif

/* get number of available file descriptors */
#ifdef _POSIX_VERSION
    num_fds = sysconf(_SC_OPEN_MAX);
#else
    num_fds = getdtablesize();
#endif

#ifdef META8_OPTION
    meta_char = (Options & Opt_meta8 ? 0x80 : 033);
    if (rs_modifier
	&& strlen(rs_modifier) == 4
	&& toupper(*rs_modifier) == 'M'
	&& toupper(rs_modifier[1]) == 'O'
	&& toupper(rs_modifier[2]) == 'D')
	switch (rs_modifier[3]) {
	case '2':
	    ModXMask = Mod2Mask;
	    break;
	case '3':
	    ModXMask = Mod3Mask;
	    break;
	case '4':
	    ModXMask = Mod4Mask;
	    break;
	case '5':
	    ModXMask = Mod5Mask;
	    break;
	default:
	    ModXMask = Mod1Mask;
	}
#endif
    get_ourmods();
    if (!(Options & Opt_scrollTtyOutput))
	PrivateModes |= PrivMode_TtyOutputInh;
    if (Options & Opt_scrollKeypress)
	PrivateModes |= PrivMode_Keypress;
#ifndef NO_BACKSPACE_KEY
    if (strcmp(rs_backspace_key, "DEC") == 0)
	PrivateModes |= PrivMode_HaveBackSpace;
#endif

#ifdef GREEK_SUPPORT
    greek_init();
#endif

    Xfd = XConnectionNumber(Xdisplay);

/*fprintf( stderr, "%s:%d display = %p, Xfd = %d\n\n", __FUNCTION__, __LINE__, Xdisplay, Xfd ); */
/*	sleep(10); */

    cmdbuf_ptr = cmdbuf_endp = cmdbuf_base;

    if ((cmd_fd = run_command(argv)) < 0) {
	print_error("aborting");
	exit(EXIT_FAILURE);
    }
}
