int
rxvt_run_child(rxvt_t* r, int page, const char **argv)
{
    char*		login;


    /* rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "argv = %x\n", argv)); */

    /* init terminal attributes */
    SET_TTYMODE( STDIN_FILENO, &(PVTS(r, page)->tio) );

    if (ISSET_OPTION(r, Opt_console))	/* be virtual console, fail
					 * silently */
    {
#ifdef TIOCCONS
	unsigned int	on = 1;
	ioctl(STDIN_FILENO, TIOCCONS, &on);
#elif defined (SRIOCSREDIR)
	int	    fd;
	fd = open( CONSOLE, O_WRONLY, 0 );
	if (fd >= 0)
	{
	    if( ioctl( fd, SRIOCSREDIR, NULL ) < 0 )
	    close( fd );
	}
#endif	/* SRIOCSREDIR */
    }

    /* set window size */
    rxvt_tt_winsize( STDIN_FILENO, r->TermWin.ncol, r->TermWin.nrow, 0 );

#ifndef __QNX__
    /* command interpreter path */
    if (NOT_NULL(argv))
    {
	int	    i;
	for (i = 0; argv[i]; i++)
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "argv [%d] = \"%s\"\n", i, argv[i]));
	execvp(argv[0], (char *const *)argv);
	/* no error message: STDERR is closed! */
    }
    else
    {
	const char   *argv0, *shell;

	if (IS_NULL(shell = getenv("SHELL")) || ((char) 0 == *shell) )
	{
# ifdef HAVE_GETPWUID
	    struct passwd* pwent = getpwuid( getuid () );

	    if (IS_NULL(pwent) ||
		IS_NULL(shell = pwent->pw_shell) ||
		(char) 0 == *shell
	      )
# endif	/* HAVE_GETPWUID */
		shell = "/bin/sh";
	}

	argv0 = (const char *) rxvt_r_basename( shell);
	if (ISSET_OPTION(r, Opt_loginShell))
	{
	    int	    l = STRLEN(argv0) + 2;
	    if (l <= 0 || l > 4096) /* possible integer overflow */
		l = 4096;
	    login = rxvt_malloc(l * sizeof(char));
    
	    login[0] = '-';
	    STRNCPY(&login[1], argv0, l-2);
	    login[l-1] = (char) 0;
	    argv0 = login;
	}
	execlp( shell, argv0, NULL );
	/* no error message: STDERR is closed! */
    }
#else		    /* __QNX__ uses qnxspawn() */
    {
	char	iov_a[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
	char*	command = NULL, fullcommand[_MAX_PATH];
	char**	arg_v, *arg_a[2] = { NULL, NULL };

	if (argv != NULL)
	{
	    if (access(argv[0], X_OK) == -1)
	    {
		if (IS_NULL(STRCHR(argv[0], '/')))
		{
		    searchenv(argv[0], "PATH", fullcommand);
		    if (fullcommand[0] != '\0')
			command = fullcommand;
		}
		if (access(command, X_OK) == -1)
		    return -1;
	    }
	    else
		command = argv[0];
	    arg_v = argv;
	}
	else
	{
	    if (IS_NULL(command = getenv("SHELL")) || 
		(char) 0 == *command)
		command = "/bin/sh";

	    arg_a[0] = my_basename(command);
	    if (ISSET_OPTION(r, Opt_loginShell))
	    {
		int	l = STRLEN(arg_a[0]) + 2;
		if (l <= 0 || l > 4096)	/* possible integer overflow */
		    l = 4096;
		login = rxvt_malloc(l * sizeof(char));

		login[0] = '-';
		STRNCPY(&login[1], arg_a[0], l-2);
		login[l-1] = (char) 0;
		arg_a[0] = login;
	    }
	    arg_v = arg_a;
	}
	iov_a[0] = iov_a[1] = iov_a[2] = PVTS(r, page)->tty_fd;
	PVTS(r, page)->cmd_pid = qnx_spawn(0, 0, 0, -1, -1,
		    _SPAWN_SETSID | _SPAWN_TCSETPGRP,
		    command, arg_v, environ, iov_a, 0);
	if (login)
	    rxvt_free(login);
	close(PVTS(r, page)->tty_fd);
	PVTS(r, page)->tty_fd = -1;
	return PVTS(r, page)->cmd_fd;
    }
#endif
    return -1;
}