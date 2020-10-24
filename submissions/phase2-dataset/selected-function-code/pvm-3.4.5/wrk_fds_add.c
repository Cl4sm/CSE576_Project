wrk_fds_add(fd, sets)
	int fd;				/* the fd */
	int sets;			/* which sets */
{
#ifdef	SANITY
#ifndef WIN32
	if (fd < 0 || fd >= FD_SETSIZE) {
		pvmlogprintf("wrk_fds_add() bad fd %d\n", fd);
		return 1;
	}
#endif
#endif
	if (sets & 1)
		if ( !FD_ISSET(fd, &wrk_rfds) ) {
			FD_SET(fd, &wrk_rfds);
#ifdef WIN32
			wrk_nfds++;
#endif
		}
	if (sets & 2)
		if ( !FD_ISSET(fd, &wrk_wfds) ) {
			FD_SET(fd, &wrk_wfds);
#ifdef WIN32
			if ( !(sets & 1) ) wrk_nfds++;
#endif
		}
/*
	if (sets & 4)
		if ( !FD_ISSET(fd, &wrk_efds) ) {
			FD_SET(fd, &wrk_efds);
#ifdef WIN32
			wrk_nefds++;
#endif
		}
*/

#ifndef WIN32
	/* if this is new highest, adjust nfds */
	if (fd >= wrk_nfds)
		wrk_nfds = fd + 1;
#endif

	return 0;
}
