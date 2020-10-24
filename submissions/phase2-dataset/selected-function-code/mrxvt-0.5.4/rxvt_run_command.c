int
rxvt_run_command(rxvt_t *r, int page, const char **argv)
{
    int		cfd, er;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "%s(%d, argv)", __func__, page));

    /* get master (pty) */
    if ((cfd = rxvt_get_pty(&(PVTS(r, page)->tty_fd),
	(char**) &(PVTS(r, page)->ttydev))) < 0)
    {
	rxvt_msg (DBG_ERROR, DBG_INIT, "can't open pseudo-tty");
	return -1;
    }
#ifdef FD_SETSIZE
    if (r->Xfd > FD_SETSIZE || cfd > FD_SETSIZE)
    {
	rxvt_msg (DBG_ERROR, DBG_INIT, "fd too high: %d max", FD_SETSIZE);
	return -1;
    }
#endif
    fcntl(cfd, F_SETFL, O_NDELAY);

    /* get slave (tty) */
    if (PVTS(r, page)->tty_fd < 0)
    {
#if !defined(NO_SETOWNER_TTYDEV) && !defined(OS_CYGWIN)
	rxvt_privileged_ttydev(r, page, SAVE);
#endif
	if ((PVTS(r, page)->tty_fd = rxvt_get_tty(PVTS(r, page)->ttydev)) < 0)
	{
	    close(cfd);
	    rxvt_msg (DBG_ERROR, DBG_INIT, "can't open slave tty %s", PVTS(r, page)->ttydev);
	    return -1;
	}
    }

    /* Get tty mode before fork */
#ifndef NO_BACKSPACE_KEY
    if (r->h->key_backspace[0] && !r->h->key_backspace[1])
	er = r->h->key_backspace[0];
    else if (STRCMP(r->h->key_backspace, "DEC") == 0)
	er = '\177';	/* the initial state anyway */
    else
#endif
	er = -1;
    rxvt_get_ttymode(&(PVTS(r, page)->tio), er);


    rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "argv = 0x%x\n", (unsigned int) argv));
#ifndef __QNX__
    /*
     * Spin off the command interpreter
     */
    switch (PVTS(r, page)->cmd_pid = fork())
    {
	case -1:
	    rxvt_msg (DBG_ERROR, DBG_INIT, "can't fork");
	    close (cfd);
	    return -1;

	case 0:
	    /*
	     * To debug the child, follow these steps:
	     *
	     * 	    - enable sleep in the following
	     * 	    - launch gdb, set breakpoint before fork
	     * 	    - run the program, step over fork, then get child pid
	     * 	    - launch another gdb, attach to child process via pid
	     * 	    - in child's gdb, set breakpoint after sleep
	     * 	    - run 'continue' in child's gdb, debug child process
	     */
#if 0
	    sleep(10);
#endif


	    if(
		 rxvt_control_tty( PVTS(r, page)->tty_fd,
				    PVTS(r, page)->ttydev ) < 0
	      )
	    {
		rxvt_msg (DBG_ERROR, DBG_INIT, "Could not obtain control of tty");
	    }
	    else
	    {
		/*
		 * Reopen stdin, stdout and stderr over the tty file
		 * descriptor
		 */
		dup2( PVTS(r, page)->tty_fd, STDIN_FILENO  );
		dup2( PVTS(r, page)->tty_fd, STDOUT_FILENO );
		dup2( PVTS(r, page)->tty_fd, STDERR_FILENO );

		clean_sigmasks_and_fds( r, page );

		/*
		 * Spin off command interpreter.
		 */
		rxvt_run_child(r, page, argv);

		/*
		 * If we got here, then we failed to exec the child process.
		 */
		rxvt_msg (DBG_ERROR, DBG_INIT, "Could not execute %s.\n", (argv && argv[0]) ? argv[0] : "shell");
	    }

	    /* Something went wrong. Kill the child. */
	    if(
		 !(PVTS(r,page)->holdOption & HOLD_STATUSBIT) &&
		 !(PVTS(r,page)->holdOption & HOLD_ALWAYSBIT)
	      )
		/* If tab won't be held open, wait a little */
		sleep(5);
	    exit( EXIT_FAILURE );

	    /* NOT REACHED */

	default:
	    {
#if defined(HAVE_STRUCT_UTMP) && defined(HAVE_TTYSLOT)
		int	    fdstdin;

		fdstdin = dup(STDIN_FILENO);
		dup2(PVTS(r, page)->tty_fd, STDIN_FILENO);
#endif
#ifdef UTMP_SUPPORT
# ifdef UTEMPTER_SUPPORT
		/* utempter hack, it needs cmd_fd */
		PVTS(r, page)->cmd_fd = cfd;
# endif
		rxvt_privileged_utmp(r, page, SAVE);
# ifdef UTEMPTER_SUPPORT
		/* utempter hack, restore cmd_fd */
		PVTS(r, page)->cmd_fd = -1;
# endif
#endif
#if defined(HAVE_STRUCT_UTMP) && defined(HAVE_TTYSLOT)
		dup2(fdstdin, STDIN_FILENO);
		close(fdstdin);
#endif
	    }

	    /*
	     * keep STDERR_FILENO, PVTS(r, page)->cmd_fd, r->Xfd open
	     */
	    close(PVTS(r, page)->tty_fd);
	    PVTS(r, page)->tty_fd = -1;
	    break;
    }
#else		    /* __QNX__ uses qnxspawn() */
    fchmod(PVTS(r, page)->tty_fd, 0622);
    fcntl(PVTS(r, page)->tty_fd, F_SETFD, FD_CLOEXEC);
    fcntl(cfd, F_SETFD, FD_CLOEXEC);
    PVTS(r, page)->cmd_fd = cfd;

    if (rxvt_run_child(r, page, argv) == -1)
	/*exit(EXIT_FAILURE);*/
	return -1;
#endif

    return cfd;
}