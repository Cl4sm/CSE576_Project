void
rxvt_mark_dead_childs( rxvt_t *r )
{
    int	    i, j;
    short   ndead_childs = r->ndead_childs;

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_COMMAND,  "rxvt_mark_dead_childs(): %d children are dead\n", r->ndead_childs));


    /*
     * Check processes running in each tab.
     */
    for (i = LTAB(r); i >= 0 ; i--)
    {
	int	    status, pid;

	errno = 0;  /* Clear errno */
	if(
	     !PVTS(r, i)->dead	    &&
	     (pid = waitpid( PVTS(r, i)->cmd_pid, &status, WNOHANG)) != 0
	  )
	{
	    if( pid == -1 && errno == ECHILD )
	    {
		/*
		 * Command in ith tab is not our child. The only way this can
		 * happen is if we lost an SIGCHLD signal (e.g. if we receive a
		 * SIGCHLD when it is blocked, say by our own SIGCHLD handler).
		 */
		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "ECHILD error on waitpid(%d)\n", i ));

		/* We have no way of getting the child's exit status now */
		PVTS( r, i )->status = 0;
	    }
	    else
	    {
		/* Process in tab i is dead. */
		r->ndead_childs--;			/* Reduce number of
							   unprocessed dead vt's
							   by one */
		ndead_childs--;				/* This variable is safe
							   from being modified
							   in a signal */
		PVTS( r, i )->status = status;		/* Save exit status */
	    }

	    /*
	     * Regardless of losing SIGCHLD, we mark this tab as dead.
	     */
	    PVTS( r, i )->dead	    = 1;	/* Mark it as dead */
	    PVTS( r, i )->hold	    = 1;	/* Hold it until it is cleaned
						   up */
	    r->cleanDeadChilds	    = 1;	/* Child has been marked as
						   dead, but not cleaned out. */
	}
    }

    /*
     * Check processes we launched via rxvt_async_exec(). We don't care about
     * status info here, so this is much simpler.
     */
    for( i=0, j=0; i < r->nAsyncChilds; )
    {
	if( waitpid( r->asyncChilds[i], NULL, WNOHANG ) != 0 )
	{
	    r->ndead_childs--;
	    ndead_childs--;

	    i++;
	}
	
	else
	    r->asyncChilds[j++] = r->asyncChilds[i++];
    }
    r->nAsyncChilds -= (i-j);


    if( r->ndead_childs < 0 )
    {
	/*
	 * Oops. Some child died, but we never got a dead child signal on it. As
	 * long as we got here, we're fine.
	 *
	 * NOTE: It is OK for ndead_childs < 0. r->ndead_childs is updated for
	 * processes that die when we are in this function. ndead_childs is not.
	 */
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,   "Lost child signal." ));
	r->ndead_childs = 0;
    }

    else if ( ndead_childs > 0 )
    {
	/*
	 * This is problematic. The number of processes that were promised as
	 * "dead" on entry to this function is not actually the number of
	 * processes that are dead!
	 *
	 * NOTE: It is OK for r->ndead_childs > 0, as r->ndead_childs could be
	 * externally modified while we are in this function.
	 *
	 * We should only get here when one of our child processes that is NOT
	 * running in a tab dies. For instance, when we print something with our
	 * "PrintPipe" macro. If we get here for any other reason, we're in deep
	 * trouble.
	 */
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Spurious dead child signal received\n"));

	/*
	 * We should reset r->ndead_childs to 0. But there is a possible race
	 * condition with doing that. Suppose we received a dead child signal
	 * *after* looping over all childs, but just before getting here!
	 *
	 * To avoid this we only reduce r->ndead_childs by the number of
	 * processes we failed to catch as dead. Further, when we get EIO errors
	 * reading from a child, we call this function to see if the child is
	 * dead or not.
	 */
	r->ndead_childs -= ndead_childs;
    }

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Exit rxvt_mark_dead_childs(): %d children are dead\n", r->ndead_childs));
}