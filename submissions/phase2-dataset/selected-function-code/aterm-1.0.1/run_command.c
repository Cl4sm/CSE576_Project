run_command(char *argv[])
{
    ttymode_t       tio;
    int             ptyfd;

    ptyfd = get_pty();
    if (ptyfd < 0)
	return -1;

/* store original tty status for restoration clean_exit() -- rgg 04/12/95 */
    lstat(ttydev, &ttyfd_stat);
#ifdef DEBUG_CMD
    fprintf(stderr, "Original settings of %s are mode %o, uid %d, gid %d\n",
	    ttydev, ttyfd_stat.st_mode, ttyfd_stat.st_uid, ttyfd_stat.st_gid);
#endif

/* install exit handler for cleanup */
#ifdef HAVE_ATEXIT
    atexit(clean_exit);
#else
# if defined (__sun__)
    on_exit(clean_exit, NULL);	/* non-ANSI exit handler */
# else
#  ifdef UTMP_SUPPORT
    print_error("no atexit(), UTMP entries may not be cleaned");
#  endif
# endif
#endif

/*
 * get tty settings before fork()
 * and make a reasonable guess at the value for BackSpace
 */
    get_ttymode(&tio);
#if 0
/* add Backspace value */
    SavedModes |= (PrivateModes & PrivMode_BackSpace);
#endif

/* add value for scrollBar */
    if (scrollbar_visible()) {
	PrivateModes |= PrivMode_scrollBar;
	SavedModes |= PrivMode_scrollBar;
    }
    if (menubar_visible()) {
	PrivateModes |= PrivMode_menuBar;
	SavedModes |= PrivMode_menuBar;
    }
#ifdef DEBUG_TTYMODE
    debug_ttymode(&tio);
#endif

/* spin off the command interpreter */
    signal(SIGHUP, Exit_signal);
#ifndef __svr4__
    signal(SIGINT, Exit_signal);
#endif
    signal(SIGQUIT, Exit_signal);
    signal(SIGTERM, Exit_signal);
    signal(SIGCHLD, Child_signal);

	/* apparently on FBSD that should be done at line 1085 below : */
	tt_winsize(ptyfd);		/* set window size */

/* need to trap SIGURG for SVR4 (Unixware) rlogin */
/* signal (SIGURG, SIG_DFL); */

    cmd_pid = fork();
    if (cmd_pid < 0) {
	print_error("can't fork");
	return -1;
    }
    if (cmd_pid == 0) {		/* child */
    /* signal (SIGHUP, Exit_signal); */
    /* signal (SIGINT, Exit_signal); */
#ifdef HAVE_UNSETENV
    /* avoid passing old settings and confusing term size */
	unsetenv("LINES");
	unsetenv("COLUMNS");
    /* avoid passing termcap since terminfo should be okay */
	unsetenv("TERMCAP");
#endif				/* HAVE_UNSETENV */
    /* establish a controlling teletype for the new session */
	get_tty();

    /* initialize terminal attributes */
	SET_TTYMODE(0, &tio);

    /* become virtual console, fail silently */
	if (Options & Opt_console) {
#ifdef TIOCCONS
	    unsigned int    on = 1;

	    ioctl(0, TIOCCONS, &on);
#elif defined (SRIOCSREDIR)
	    int             fd = open(CONSOLE, O_WRONLY);

	    if (fd < 0 || ioctl(fd, SRIOCSREDIR, 0) < 0) {
		if (fd >= 0)
		    close(fd);
	    }
#endif				/* SRIOCSREDIR */
	}

	tt_winsize(0);		/* set window size */

    /* reset signals and spin off the command interpreter */
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGCHLD, SIG_DFL);
    /*
     * mimick login's behavior by disabling the job control signals
     * a shell that wants them can turn them back on
     */
#ifdef SIGTSTP
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
#endif				/* SIGTSTP */

#ifdef __CYGWIN__
    /* cygwin does not close files properly */
    if( Xfd > 0 )
        close(Xfd);
#endif

    /* command interpreter path */
	if (argv != NULL) {
#ifdef DEBUG_CMD
	    int             i;

	    for (i = 0; argv[i]; i++)
		fprintf(stderr, "argv [%d] = \"%s\"\n", i, argv[i]);
#endif
	    execvp(argv[0], argv);
	    print_error("can't execute \"%s\"", argv[0]);
	} else {
	    const char     *argv0, *shell;

	    if ((shell = getenv("SHELL")) == NULL || *shell == '\0')
		shell = DEFAULT_SHELL;

	    argv0 = my_basename(shell);
	    if (Options & Opt_loginShell) {
		char           *p = MALLOC((strlen(argv0) + 2) * sizeof(char));

		p[0] = '-';
		STRCPY(&p[1], argv0);
		argv0 = p;
	    }
	    execlp(shell, argv0, NULL);
	    print_error("can't execute \"%s\"", shell);
	}
	exit(EXIT_FAILURE);
    }
#ifdef UTMP_SUPPORT
    privileges(RESTORE);
    if (!(Options & Opt_utmpInhibit))
	makeutent(ttydev, display_name);	/* stamp /etc/utmp */
    privileges(IGNORE);
#endif

    return ptyfd;
}
