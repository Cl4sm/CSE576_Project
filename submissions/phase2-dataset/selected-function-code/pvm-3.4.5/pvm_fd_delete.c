	int fd;				/* the fd */
	int sets;			/* which sets */
{
#ifdef	SANITY
#ifndef WIN32
	if (fd < 0 || fd >= FD_SETSIZE) {
		pvmlogprintf("pvm_fd_delete() bad fd %d\n", fd);
		return 1;
	}
#endif
#endif
	if (sets & 1)
		if ( FD_ISSET(fd, &pvmrfds) ) {
			FD_CLR(fd, &pvmrfds);
#ifdef WIN32
			pvmnfds--;
#endif
		}
/*
	if (sets & 2)
		if ( FD_ISSET(fd, &pvmwfds) ) {
			FD_CLR(fd, &pvmwfds);
#ifdef WIN32
			pvmnwfds--;
#endif
		}
	if (sets & 4)
		if ( FD_ISSET(fd, &pvmefds) ) {
			FD_CLR(fd, &pvmefds);
#ifdef WIN32
			pvmnefds--;
#endif
		}
*/

#ifndef WIN32
	/* if this was highest, may have to adjust nfds to new highest */
	if (fd + 1 == pvmnfds)
		while (pvmnfds > 0) {
			pvmnfds--;
			if (FD_ISSET(pvmnfds, &pvmrfds)
/*
			|| FD_ISSET(pvmnefds, &pvmefds)
			|| FD_ISSET(pvmnwfds, &pvmwfds)
*/
			) {
				pvmnfds++;
				break;
			}
		}
#endif

	return 0;
}
