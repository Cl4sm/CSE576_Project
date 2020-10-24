int
rxvt_get_pty(int *fd_tty, char **ttydev)
{
    int	    pfd;

    *ttydev = NULL;

#ifdef PTYS_ARE_OPENPTY
    {
	char	tty_name[sizeof "/dev/pts/????\0"];

	if (openpty(&pfd, fd_tty, tty_name, NULL, NULL) != -1) {
	    *ttydev = STRDUP(tty_name);
	    return pfd;
	}
    }
#endif

#ifdef PTYS_ARE__GETPTY
    {
	char*	ptr = _getpty(&pfd, O_RDWR|O_NDELAY|O_NOCTTY, 0622, 0);
	if (ptr)
	    *ttydev = STRDUP(ptr);
        if (ptr)
	    return pfd;
    }
#endif

#ifdef PTYS_ARE_GETPTY
    {
	char*	ptydev;

	while ((ptydev = getpty()) != NULL)
	    if ((pfd = open(ptydev, O_RDWR | O_NOCTTY, 0)) >= 0) {
		*ttydev = STRDUP(ptydev);
		return pfd;
	    }
    }
#endif

#if defined(HAVE_GRANTPT) && defined(HAVE_UNLOCKPT)
# if defined(PTYS_ARE_GETPT) || defined(PTYS_ARE_PTMX)
    {
	extern char    *ptsname();

#  ifdef PTYS_ARE_GETPT
	extern int	getpt ();
	pfd = getpt();
#  else
	pfd = open("/dev/ptmx", O_RDWR | O_NOCTTY, 0);
#  endif
	if (pfd >= 0) {
	    extern int grantpt (int);
	    extern int unlockpt (int);
	    if (grantpt(pfd) == 0 &&	/* change slave permissions */
		unlockpt(pfd) == 0) {	/* slave now unlocked */
		char*	ptr = ptsname(pfd); /* get slave's name */
		if (ptr)
		    *ttydev = STRDUP(ptr);
		return pfd;
	    }
	    close(pfd);
	}
    }
# endif
#endif

#ifdef PTYS_ARE_PTC
    if ((pfd = open("/dev/ptc", O_RDWR | O_NOCTTY, 0)) >= 0) {
	char*	ptr = ttyname(pfd);
	if (ptr)
	    *ttydev = STRDUP(ptr);
	return pfd;
    }
#endif

#ifdef PTYS_ARE_CLONE
    if ((pfd = open("/dev/ptym/clone", O_RDWR | O_NOCTTY, 0)) >= 0) {
	char*	ptr = ptsname(pfd);
	if (ptr)
	    *ttydev = STRDUP(ptr);
	return pfd;
    }
#endif

#ifdef PTYS_ARE_NUMERIC
    {
	int	idx;
	char*	c1;
	char*	c2;
	char	pty_name[] = "/dev/ptyp???";
	char	tty_name[] = "/dev/ttyp???";

	c1 = &(pty_name[sizeof(pty_name) - 4]);
	c2 = &(tty_name[sizeof(tty_name) - 4]);
	for (idx = 0; idx < 256; idx++) {
	    sprintf(c1, "%d", idx);
	    sprintf(c2, "%d", idx);
	    if (access(tty_name, F_OK) < 0) {
		idx = 256;
		break;
	    }
	    if ((pfd = open(pty_name, O_RDWR | O_NOCTTY, 0)) >= 0) {
		if (access(tty_name, R_OK | W_OK) == 0) {
		    *ttydev = STRDUP(tty_name);
		    return pfd;
		}
		close(pfd);
	    }
	}
    }
#endif
#ifdef PTYS_ARE_SEARCHED
    {
	const char* c1;
	const char* c2;
	char	    pty_name[] = "/dev/pty??";
	char	    tty_name[] = "/dev/tty??";

# ifndef PTYCHAR1
#  define PTYCHAR1  "pqrstuvwxyz"
# endif
# ifndef PTYCHAR2
#  define PTYCHAR2  "0123456789abcdef"
# endif
	for (c1 = PTYCHAR1; *c1; c1++) {
	    pty_name[(sizeof(pty_name) - 3)] =
	        tty_name[(sizeof(pty_name) - 3)] = *c1;
	    for (c2 = PTYCHAR2; *c2; c2++) {
		pty_name[(sizeof(pty_name) - 2)] =
		    tty_name[(sizeof(pty_name) - 2)] = *c2;
		if ((pfd = open(pty_name, O_RDWR | O_NOCTTY, 0)) >= 0) {
		    if (access(tty_name, R_OK | W_OK) == 0) {
			*ttydev = STRDUP(tty_name);
			return pfd;
		    }
		    close(pfd);
		}
	    }
	}
    }
#endif
    return -1;
}