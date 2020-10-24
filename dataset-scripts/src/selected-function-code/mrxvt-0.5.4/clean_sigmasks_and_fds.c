void
clean_sigmasks_and_fds( rxvt_t* r, int page )
{
#ifdef SIGTSTP
    struct sigaction	ignore;
#endif
    struct sigaction	deflt;
    register int	i;

    /* Close all file descriptors except STDXXX */
    for (i = STDERR_FILENO + 1; i < r->num_fds; i ++)
	close (i);
    if (PVTS(r, page)->tty_fd > 2)
    {
	close (PVTS(r, page)->tty_fd);
	PVTS(r, page)->tty_fd = -1;
    }

    /* reset signal handlers */
    deflt.sa_handler = SIG_DFL;
    deflt.sa_flags = 0;
    sigemptyset( &deflt.sa_mask );

    sigaction( SIGINT,	&deflt, NULL );
    sigaction( SIGQUIT, &deflt, NULL );
    sigaction( SIGTERM, &deflt, NULL );
    sigaction( SIGHUP,	&deflt, NULL );
    sigaction( SIGPIPE,	&deflt, NULL );
    sigaction( SIGCHLD, &deflt, NULL );

    /*
     * Mimick login's behavior by disabling the job control signals a shell that
     * wants them can turn them back on
     */
#ifdef SIGTSTP
    ignore.sa_handler	= SIG_IGN;
    ignore.sa_flags	= 0;
    sigemptyset( &ignore.sa_mask );

    sigaction( SIGTSTP, &ignore, NULL );
    sigaction( SIGTTIN, &ignore, NULL );
    sigaction( SIGTTOU, &ignore, NULL );
#endif	/* SIGTSTP */
}