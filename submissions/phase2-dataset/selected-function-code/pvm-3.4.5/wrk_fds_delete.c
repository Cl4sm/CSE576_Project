wrk_fds_delete(fd, sets)
	int fd;				/* the fd */
	int sets;			/* which sets */
{
#ifdef	SANITY
#ifndef WIN32
	if (fd < 0 || fd >= FD_SETSIZE) {
		pvmlogprintf("wrk_fds_delete() bad fd %d\n", fd);
		return 1;
	}
#endif
#endif
	if (sets & 1)
		if ( FD_ISSET(fd, &wrk_rfds) ) {
			FD_CLR(fd, &wrk_rfds);
#ifdef WIN32
			wrk_nfds--;
#endif
		}
	if (sets & 2)
		if ( FD_ISSET(fd, &wrk_wfds) ) {
			FD_CLR(fd, &wrk_wfds);
#ifdef WIN32
			if ( !(sets & 1) ) wrk_nfds--;
#endif
		}
/*
	if (sets & 4)
		if ( FD_ISSET(fd, &wrk_efds) ) {
			FD_CLR(fd, &wrk_efds);
#ifdef WIN32
			wrk_nefds--;
#endif
		}
*/

#ifndef WIN32
	/* if this was highest, may have to adjust nfds to new highest */
	if (fd + 1 == wrk_nfds)
		while (wrk_nfds > 0) {
			wrk_nfds--;
			if (FD_ISSET(wrk_nfds, &wrk_rfds)
			|| FD_ISSET(wrk_nfds, &wrk_wfds)
/*
			|| FD_ISSET(wrk_nfds, &wrk_efds)
*/
			) {
				wrk_nfds++;
				break;
			}
		}
#endif

	return 0;
}
