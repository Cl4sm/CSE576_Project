void
rxvt_get_ttymode(ttymode_t *tio, int erase)
{
#ifdef HAVE_TERMIOS_H
    /*
     * standard System V termios interface
     */
    if (GET_TERMIOS(STDIN_FILENO, tio) < 0)
    {
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
    if (erase != -1)
    tio->c_cc[VERASE] = (char)erase;

/* input modes */
    tio->c_iflag = (BRKINT | IGNPAR | ICRNL
# ifdef IMAXBEL
		    | IMAXBEL
# endif
		    | IXON);

/* output modes */
    tio->c_oflag = (OPOST | ONLCR);

/* control modes */
    tio->c_cflag = (CS8 | CREAD);

/* line discipline modes */
    tio->c_lflag = (ISIG | ICANON | IEXTEN | ECHO
# if defined (ECHOCTL) && defined (ECHOKE)
		    | ECHOCTL | ECHOKE
# endif
		    | ECHOE | ECHOK);
# else		    /* HAVE_TERMIOS_H */

    /*
     * sgtty interface
     */

    /* get parameters -- gtty */
    if (ioctl(STDIN_FILENO, TIOCGETP, &(tio->sg)) < 0)
    {
	tio->sg.sg_erase = CERASE;  /* ^H */
	tio->sg.sg_kill = CKILL;    /* ^U */
    }
    if (erase != -1)
	tio->sg.sg_erase = (char)erase;

    tio->sg.sg_flags = (CRMOD | ECHO | EVENP | ODDP);

    /* get special characters */
    if (ioctl(STDIN_FILENO, TIOCGETC, &(tio->tc)) < 0)
    {
	tio->tc.t_intrc = CINTR;    /* ^C */
	tio->tc.t_quitc = CQUIT;    /* ^\ */
	tio->tc.t_startc = CSTART;  /* ^Q */
	tio->tc.t_stopc = CSTOP;    /* ^S */
	tio->tc.t_eofc = CEOF;	/* ^D */
	tio->tc.t_brkc = -1;
    }

    /* get local special chars */
    if (ioctl(STDIN_FILENO, TIOCGLTC, &(tio->lc)) < 0)
    {
	tio->lc.t_suspc = CSUSP;    /* ^Z */
	tio->lc.t_dsuspc = CDSUSP;  /* ^Y */
	tio->lc.t_rprntc = CRPRNT;  /* ^R */
	tio->lc.t_flushc = CFLUSH;  /* ^O */
	tio->lc.t_werasc = CWERASE; /* ^W */
	tio->lc.t_lnextc = CLNEXT;  /* ^V */
    }

    /* get line discipline */
    ioctl(STDIN_FILENO, TIOCGETD, &(tio->line));
# ifdef NTTYDISC
    tio->line = NTTYDISC;
# endif		    /* NTTYDISC */
    tio->local = (LCRTBS | LCRTERA | LCTLECH | LPASS8 | LCRTKIL);
#endif		    /* HAVE_TERMIOS_H */

/*
 * Debugging
 */
#ifdef DEBUG
#ifdef HAVE_TERMIOS_H
/* c_iflag bits */
    rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "Input flags\n"));

/* cpp token stringize doesn't work on all machines <sigh> */
# define FOO(flag,name)		    \
    if ((tio->c_iflag) & flag)	    \
        rxvt_msg (DBG_DEBUG, DBG_INIT, "%s ", name)

/* c_iflag bits */
    FOO(IGNBRK, "IGNBRK");
    FOO(BRKINT, "BRKINT");
    FOO(IGNPAR, "IGNPAR");
    FOO(PARMRK, "PARMRK");
    FOO(INPCK, "INPCK");
    FOO(ISTRIP, "ISTRIP");
    FOO(INLCR, "INLCR");
    FOO(IGNCR, "IGNCR");
    FOO(ICRNL, "ICRNL");
    FOO(IXON, "IXON");
    FOO(IXOFF, "IXOFF");
# ifdef IUCLC
    FOO(IUCLC, "IUCLC");
# endif
# ifdef IXANY
    FOO(IXANY, "IXANY");
# endif
# ifdef IMAXBEL
    FOO(IMAXBEL, "IMAXBEL");
# endif
    rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "\n"));

# undef FOO
# define FOO(entry, name)		    \
    rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "%-8s = %#04o\n", name, tio->c_cc [entry]))

    FOO(VINTR, "VINTR");
    FOO(VQUIT, "VQUIT");
    FOO(VERASE, "VERASE");
    FOO(VKILL, "VKILL");
    FOO(VEOF, "VEOF");
    FOO(VEOL, "VEOL");
# ifdef VEOL2
    FOO(VEOL2, "VEOL2");
# endif
# ifdef VSWTC
    FOO(VSWTC, "VSWTC");
# endif
# ifdef VSWTCH
    FOO(VSWTCH, "VSWTCH");
# endif
    FOO(VSTART, "VSTART");
    FOO(VSTOP, "VSTOP");
    FOO(VSUSP, "VSUSP");
# ifdef VDSUSP
    FOO(VDSUSP, "VDSUSP");
# endif
# ifdef VREPRINT
    FOO(VREPRINT, "VREPRINT");
# endif
# ifdef VDISCRD
    FOO(VDISCRD, "VDISCRD");
# endif
# ifdef VWERSE
    FOO(VWERSE, "VWERSE");
# endif
# ifdef VLNEXT
    FOO(VLNEXT, "VLNEXT");
# endif
    rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "\n"));
# undef FOO
# endif		    /* HAVE_TERMIOS_H */
#endif		    /* DEBUG */
}