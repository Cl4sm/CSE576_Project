get_ttymode(ttymode_t * tio)
{
#ifdef HAVE_TERMIOS_H
/*
 * standard System V termios interface
 */
    if (GET_TERMIOS(0, tio) < 0) {
    /* return error - use system defaults */
	tio->c_cc[VINTR] = CINTR;
	tio->c_cc[VQUIT] = CQUIT;
	tio->c_cc[VERASE] = CERASE;
	tio->c_cc[VKILL] = CKILL;
	tio->c_cc[VSTART] = CSTART;
	tio->c_cc[VSTOP] = CSTOP;
	tio->c_cc[VSUSP] = CSUSP;
# ifdef VDSUSP
	tio->c_cc[VDSUSP] = CDSUSP;
# endif
# ifdef VREPRINT
	tio->c_cc[VREPRINT] = CRPRNT;
# endif
# ifdef VDISCRD
	tio->c_cc[VDISCRD] = CFLUSH;
# endif
# ifdef VWERSE
	tio->c_cc[VWERSE] = CWERASE;
# endif
# ifdef VLNEXT
	tio->c_cc[VLNEXT] = CLNEXT;
# endif
    }
    tio->c_cc[VEOF] = CEOF;
    tio->c_cc[VEOL] = VDISABLE;
# ifdef VEOL2
    tio->c_cc[VEOL2] = VDISABLE;
# endif
# ifdef VSWTC
    tio->c_cc[VSWTC] = VDISABLE;
# endif
# ifdef VSWTCH
    tio->c_cc[VSWTCH] = VDISABLE;
# endif
# if VMIN != VEOF
    tio->c_cc[VMIN] = 1;
# endif
# if VTIME != VEOL
    tio->c_cc[VTIME] = 0;
# endif

/* input modes */
    tio->c_iflag = (BRKINT | IGNPAR | ICRNL | IXON
# ifdef IMAXBEL
		    | IMAXBEL
# endif
	);

/* output modes */
    tio->c_oflag = (OPOST | ONLCR);

/* control modes */
    tio->c_cflag = (CS8 | CREAD);

/* line discipline modes */
    tio->c_lflag = (ISIG | ICANON | IEXTEN | ECHO | ECHOE | ECHOK
# if defined (ECHOCTL) && defined (ECHOKE)
		    | ECHOCTL | ECHOKE
# endif
	);

#if 0
/*
 * guess an appropriate value for Backspace
 */
# ifdef FORCE_BACKSPACE
    PrivMode(1, PrivMode_BackSpace);
# elif defined (FORCE_DELETE)
    PrivMode(0, PrivMode_BackSpace);
# else
    PrivMode((tio->c_cc[VERASE] == '\b'), PrivMode_BackSpace);
# endif
#endif				/* if 0 */

#else				/* HAVE_TERMIOS_H */

/*
 * sgtty interface
 */

/* get parameters -- gtty */
    if (ioctl(0, TIOCGETP, &(tio->sg)) < 0) {
	tio->sg.sg_erase = CERASE;	/* ^H */
	tio->sg.sg_kill = CKILL;	/* ^U */
    }
    tio->sg.sg_flags = (CRMOD | ECHO | EVENP | ODDP);

/* get special characters */
    if (ioctl(0, TIOCGETC, &(tio->tc)) < 0) {
	tio->tc.t_intrc = CINTR;	/* ^C */
	tio->tc.t_quitc = CQUIT;	/* ^\ */
	tio->tc.t_startc = CSTART;	/* ^Q */
	tio->tc.t_stopc = CSTOP;	/* ^S */
	tio->tc.t_eofc = CEOF;	/* ^D */
	tio->tc.t_brkc = -1;
    }
/* get local special chars */
    if (ioctl(0, TIOCGLTC, &(tio->lc)) < 0) {
	tio->lc.t_suspc = CSUSP;	/* ^Z */
	tio->lc.t_dsuspc = CDSUSP;	/* ^Y */
	tio->lc.t_rprntc = CRPRNT;	/* ^R */
	tio->lc.t_flushc = CFLUSH;	/* ^O */
	tio->lc.t_werasc = CWERASE;	/* ^W */
	tio->lc.t_lnextc = CLNEXT;	/* ^V */
    }
/* get line discipline */
    ioctl(0, TIOCGETD, &(tio->line));
# ifdef NTTYDISC
    tio->line = NTTYDISC;
# endif				/* NTTYDISC */
    tio->local = (LCRTBS | LCRTERA | LCTLECH | LPASS8 | LCRTKIL);

/*
 * guess an appropriate value for Backspace
 */
#if 0
# ifdef FORCE_BACKSPACE
    PrivMode(1, PrivMode_BackSpace);
# elif defined (FORCE_DELETE)
    PrivMode(0, PrivMode_BackSpace);
# else
    PrivMode((tio->sg.sg_erase == '\b'), PrivMode_BackSpace);
# endif
#endif				/* if 0 */

#endif				/* HAVE_TERMIOS_H */
}
