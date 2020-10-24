void
rxvt_init_command(rxvt_t* r)
{
    /*
     * Initialize the command connection. This should be called after the X
     * server connection is established.
     */
    struct sigaction	act;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "%s()\n", __func__));


    /*
     * Enable delete window protocol so that if the top-level window of the
     * terminal is destroyed by the Session Manager, we can receive a
     * ClientMessage event and do something gracefully.
     */
    XSetWMProtocols (r->Xdisplay, r->TermWin.parent,
	    &(r->h->xa[XA_WMDELETEWINDOW]), 1);

#ifdef META8_OPTION
    r->h->meta_char = (ISSET_OPTION(r, Opt_meta8) ? 0x80 : C0_ESC);
#endif
    rxvt_get_ourmods(r);

#ifdef GREEK_SUPPORT
    greek_init();
#endif

    r->Xfd = XConnectionNumber(r->Xdisplay);
#ifdef USE_FIFO
    if( ISSET_OPTION( r, Opt_useFifo ) )
	rxvt_init_fifo( r );
    else
	r->fifo_fd = -1;
#endif

#ifdef CURSOR_BLINK
    if (ISSET_OPTION(r, Opt_cursorBlink))
	(void)gettimeofday(&r->h->lastcursorchange, NULL);
#endif

    /*
     * Gracefully exit on term signals.
     */
    act.sa_handler  = rxvt_Exit_signal;
    act.sa_flags    = 0;
    sigemptyset (&act.sa_mask);

#ifndef OS_SVR4
    sigaction( SIGINT , &act, NULL);
#endif
    sigaction( SIGQUIT, &act, NULL);
    sigaction( SIGTERM, &act, NULL);

    /*
     * 2006-04-28 gi1242: Ignore HUP signals. We sometimes receive this if bash
     * is killed. Chances are that we don't have to exit ...
     */
    act.sa_handler  = SIG_IGN;
    sigaction( SIGHUP, &act, NULL);

#ifdef PRINTPIPE
    /*
     * 2006-04-28 gi1242: If there is an error opening the printer command, then
     * we'll get SIGPIPE. If not handled, mrxvt will exit.
     * 
     * There's nothing we really need to do on broken pipes, so just ignore
     * SIGPIPE for now.
     */
    sigaction( SIGPIPE, &act, NULL);
#endif

    act.sa_handler  = rxvt_Child_signal;
    sigaction (SIGCHLD, &act, NULL);
}