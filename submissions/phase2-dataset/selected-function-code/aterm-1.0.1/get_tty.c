int
get_tty(void)
{
    int             fd;
    pid_t           pid;

/*
 * setsid() [or setpgrp] must be before open of the terminal,
 * otherwise there is no controlling terminal (Solaris 2.4, HP-UX 9)
 */
#ifndef ultrix
# ifdef NO_SETSID
    pid = setpgrp(0, 0);
# else
    pid = setsid();
# endif
    if (pid < 0)
	perror(rs_name);
# ifdef DEBUG_TTYMODE
    print_error("(%s: line %d): PID = %d\n", __FILE__, __LINE__, pid);
# endif
#endif				/* ultrix */

    if ((fd = open(ttydev, O_RDWR)) < 0) {
	print_error("can't open slave tty %s", ttydev);
	exit(EXIT_FAILURE);
    }
#if defined (__svr4__) || defined (__lnx21__)
/*
 * Push STREAMS modules:
 *    ptem: pseudo-terminal hardware emulation module.
 *    ldterm: standard terminal line discipline.
 *    ttcompat: V7, 4BSD and XENIX STREAMS compatibility module.
 */
    ioctl(fd, I_PUSH, "ptem");
    ioctl(fd, I_PUSH, "ldterm");
    ioctl(fd, I_PUSH, "ttcompat");
#else				/* __svr4__ */
    {
    	/* change ownership of tty to real uid and real group */
#ifndef __CYGWIN32__		
		unsigned int    mode = 0622;
		gid_t           gid = getgid();

# ifdef TTY_GID_SUPPORT
		{
	    	struct group   *gr = getgrnam("tty");

	    	if (gr) {
	    	/* change ownership of tty to real uid, "tty" gid */
			gid = gr->gr_gid;
			mode = 0620;
	    	}
		}
# endif				/* TTY_GID_SUPPORT */
#endif
		privileges(RESTORE);
#ifndef __CYGWIN32__
		fchown(fd, getuid(), gid);	/* fail silently */
		fchmod(fd, mode);
#endif
		privileges(IGNORE);
    }
#endif				/* __svr4__ */

/*
 * Close all file descriptors.  If only stdin/out/err are closed,
 * child processes remain alive upon deletion of the window.
 */
    {
	int             i;

	for (i = 0; i < num_fds; i++)
	    if (i != fd)
		close(i);
    }

/* Reopen stdin, stdout and stderr over the tty file descriptor */
    dup(fd);			/* 0: stdin */
    dup(fd);			/* 1: stdout */
    dup(fd);			/* 2: stderr */

    if (fd > 2)
	close(fd);

#ifdef ultrix
    if ((fd = open("/dev/tty", O_RDONLY)) >= 0) {
	ioctl(fd, TIOCNOTTY, 0);
	close(fd);
    } else {
	pid = setpgrp(0, 0);
	if (pid < 0)
	    perror(rs_name);
    }

/* no error, we could run with no tty to begin with */
#else				/* ultrix */
# ifdef TIOCSCTTY
    ioctl(0, TIOCSCTTY, 0);
# endif

/* set process group */
# if defined (_POSIX_VERSION) || defined (__svr4__)
    tcsetpgrp(0, pid);
# elif defined (TIOCSPGRP)
    ioctl(0, TIOCSPGRP, &pid);
# endif

/* svr4 problems: reports no tty, no job control */
/* # if !defined (__svr4__) && defined (TIOCSPGRP) */

    close(open(ttydev, O_RDWR, 0));
/* # endif */
#endif				/* ultrix */

    privileges(IGNORE);

    return fd;
}
