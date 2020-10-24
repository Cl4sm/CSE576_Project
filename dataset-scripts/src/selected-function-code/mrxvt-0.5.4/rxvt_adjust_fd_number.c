void
rxvt_adjust_fd_number( rxvt_t* r )
{
    int	    num_fds = STDERR_FILENO;
    int	    i;

    for( i=0; i <= LTAB(r); i++ )
	MAX_IT( num_fds, PVTS(r, i)->cmd_fd );
    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR, "LTAB=%d, stderr_fd=%d, num_fds=%d. ", LTAB(r), STDERR_FILENO, num_fds));

    MAX_IT( num_fds, r->Xfd );
#ifdef USE_FIFO
    MAX_IT( num_fds, r->fifo_fd );
#endif/*USE_FIFO*/
#ifdef HAVE_X11_SM_SMLIB_H
    MAX_IT( num_fds, r->TermWin.ice_fd );
#endif

#if 0
    MAX_IT( num_fds, r->num_fds-1 );
#endif
#ifdef OS_IRIX
    /* Alex Coventry says we need 4 & 7 too */
    MAX_IT( num_fds, 7 );
#endif
    r->num_fds = num_fds + 1;	/* counts from 0 */
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_TABBAR, "Adjust num_fds to %d\n", r->num_fds));
}