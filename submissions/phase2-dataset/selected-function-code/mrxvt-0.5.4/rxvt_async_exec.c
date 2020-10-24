int
rxvt_async_exec( rxvt_t *r, const char *cmd)
{
    int	    pid;
    int	    argc;
    char    **argv;

    if( r->nAsyncChilds >= MAX_CHILDS )
    {
	rxvt_msg (DBG_ERROR, DBG_INIT,  "Too many childs."
		" Increase MAX_CHILDS in src/feature.h" );
	return 0;
    }

    pid = fork();
    switch( pid )
    {
	case -1:
	    rxvt_msg (DBG_ERROR, DBG_INIT,  "Unable to fork" );
	    return 0;	/* Failure */
	    /* NOT REACHED */

	case 0:
	    /*
	     * Close all file descriptors, and reset signal masks to their
	     * default values before exec'ing the child process.
	     */
	    clean_sigmasks_and_fds( r, ATAB(r) );

	    argv = rxvt_string_to_argv( cmd, &argc );

	    execvp( argv[0], argv );

	    rxvt_msg (DBG_ERROR, DBG_INIT,  "Failed to exec %s", argv[0] );
	    exit(1);
	    /* NOT REACHED */

	default:
	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_INIT, "Forked %s", cmd));
	    r->asyncChilds[ r->nAsyncChilds++ ] = pid;
	    return 1;
    }
}