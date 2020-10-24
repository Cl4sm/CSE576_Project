int
pvm_fd_add(fd, sets)
	int fd;				/* the fd */
	int sets;			/* which sets */
{
#ifdef	SANITY
#ifndef WIN32
	if (fd < 0 || fd >= FD_SETSIZE) {
		pvmlogprintf("pvm_fd_add() bad fd %d\n", fd);
		return 1;
	}
#endif
#endif
	if (sets & 1)
		if ( !FD_ISSET(fd, &pvmrfds) ) {
			FD_SET(fd, &pvmrfds);
#ifdef WIN32
			pvmnfds++;
#endif
		}
/*
	if (sets & 2)
		if ( !FD_ISSET(fd, &pvmwfds) ) {
			FD_SET(fd, &pvmwfds);
#ifdef WIN32
			pvmnwfds++;
#endif
		}
	if (sets & 4)
		if ( !FD_ISSET(fd, &pvmefds) ) {
			FD_SET(fd, &pvmefds);
#ifdef WIN32
			pvmnefds++;
#endif
		}
*/

#ifndef WIN32
	/* if this is new highest, adjust nfds */
	if (fd >= pvmnfds)
		pvmnfds = fd + 1;
#endif

	return 0;
}
