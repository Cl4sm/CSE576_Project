void
rxvt_tt_winsize(int fd, unsigned short col, unsigned short row, pid_t pid)
{
    struct winsize  ws;

#ifdef DEBUG
    if( pid && fd > STDERR_FILENO )
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_MAIN, "%s( fd=%d, col=%hu, row=%hu, pid=%d )\n", __func__, fd, col, row, pid));
    }
#endif

    if (fd < 0)
	return;

    ws.ws_col = col;
    ws.ws_row = row;
    ws.ws_xpixel = ws.ws_ypixel = 0;

    if (ioctl(fd, TIOCSWINSZ, &ws) < 0)
    {
#ifdef DEBUG
	if( pid && fd > STDERR_FILENO )
	{
	    rxvt_msg (DBG_WARN, DBG_MAIN, "Failed to send TIOCSWINSZ to fd %d\n", fd);
	}
#endif
    }
#ifdef SIGWINCH
    else if( pid )	/* force through to the command */
	kill(pid, SIGWINCH);
#endif
}