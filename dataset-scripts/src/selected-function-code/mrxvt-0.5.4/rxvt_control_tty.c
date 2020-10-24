int
rxvt_control_tty(int fd_tty, const char *ttydev)
{
#ifndef __QNX__
    int             fd;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_PTYTTY, "rxvt_control_tty(): pid: %d, tty fd: %d, dev: %s\n", (int) getpid(), fd_tty, ttydev));
/* ---------------------------------------- */
# ifdef HAVE_SETSID
    setsid();
# endif
# if defined(HAVE_SETPGID)
    setpgid(0, 0);
# elif defined(HAVE_SETPGRP)
    setpgrp(0, 0);
# endif
/* ---------------------------------------- */
# ifdef TIOCNOTTY
    fd = open("/dev/tty", O_RDWR | O_NOCTTY);
    rxvt_dbgmsg ((DBG_DEBUG, DBG_PTYTTY, "rxvt_control_tty(): Voiding tty associations: previous=%s\n", fd < 0 ? "no" : "yes"));
    if (fd >= 0) {
	ioctl(fd, TIOCNOTTY, NULL); /* void tty associations */
	close(fd);
    }
# endif
/* ---------------------------------------- */
    fd = open("/dev/tty", O_RDWR | O_NOCTTY);
    rxvt_dbgmsg ((DBG_DEBUG, DBG_PTYTTY, "rxvt_control_tty(): /dev/tty has controlling tty? %s\n", fd < 0 ? "no (good)" : "yes (bad)"));
    if (fd >= 0)
	close(fd);	/* ouch: still have controlling tty */
/* ---------------------------------------- */
#if defined(PTYS_ARE_PTMX) && defined(I_PUSH)
/*
 * Push STREAMS modules:
 *    ptem: pseudo-terminal hardware emulation module.
 *    ldterm: standard terminal line discipline.
 *    ttcompat: V7, 4BSD and XENIX STREAMS compatibility module.
 *
 * After we push the STREAMS modules, the first open() on the slave side
 * (i.e. the next section between the dashes giving us "tty opened OK")
 * should make the "ptem" (or "ldterm" depending upon either which OS
 * version or which set of manual pages you have) module give us a
 * controlling terminal.  We must already have close()d the master side
 * fd in this child process before we push STREAMS modules on because the
 * documentation is really unclear about whether it is any close() on
 * the master side or the last close() - i.e. a proper STREAMS dismantling
 * close() - on the master side which causes a hang up to be sent
 * through - Geoff Wing
 */
# ifdef HAVE_ISASTREAM
    if (isastream(fd_tty) == 1)
# endif
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_PTYTTY, "rxvt_control_tty(): Pushing STREAMS modules\n"));
	ioctl(fd_tty, I_PUSH, "ptem");
	ioctl(fd_tty, I_PUSH, "ldterm");
	ioctl(fd_tty, I_PUSH, "ttcompat");
    }
#endif
/* ---------------------------------------- */
# if defined(TIOCSCTTY)
    fd = ioctl(fd_tty, TIOCSCTTY, NULL);
    rxvt_dbgmsg ((DBG_DEBUG, DBG_PTYTTY, "rxvt_control_tty(): ioctl(..,TIOCSCTTY): %d\n", fd));
# elif defined(TIOCSETCTTY)
    fd = ioctl(fd_tty, TIOCSETCTTY, NULL);
    rxvt_dbgmsg ((DBG_DEBUG, DBG_PTYTTY, "rxvt_control_tty(): ioctl(..,TIOCSETCTTY): %d\n", fd));
# else
    fd = open(ttydev, O_RDWR);
    rxvt_dbgmsg ((DBG_DEBUG, DBG_PTYTTY, "rxvt_control_tty(): tty open%s\n", fd < 0 ? " failure" : "ed OK"));
    if (fd >= 0)
    close(fd);
# endif
/* ---------------------------------------- */
    fd = open("/dev/tty", O_WRONLY);
    rxvt_dbgmsg ((DBG_DEBUG, DBG_PTYTTY, "rxvt_control_tty(): do we have controlling tty now: %s\n", fd < 0 ? "no (fatal)" : "yes (good)"));
    if (fd < 0)
	return -1;	/* fatal */
    close(fd);
/* ---------------------------------------- */
    rxvt_dbgmsg ((DBG_DEBUG, DBG_PTYTTY, "rxvt_control_tty(): tcgetpgrp(): %d  getpgrp(): %d\n", (int) tcgetpgrp(fd_tty), (int) getpgrp()));
/* ---------------------------------------- */
#endif		    /* ! __QNX__ */
    return 0;
}