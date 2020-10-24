void
term_init()
{
#ifndef	STTY_ONLY
	char	bp[TGETENT_BUFSIZ],
		*term,
		*ptr;

	if ((term = getenv("TERM")) == (char *) 0)
	{
		fprintf(stderr, "irc: No TERM variable set!\n");
		fprintf(stderr,"irc: You may still run irc by using the -d switch\n");
		exit(1);
	}
	if (tgetent(bp, term) < 1)
	{
		fprintf(stderr, "irc: No termcap entry for %s.\n", term);
		fprintf(stderr,"irc: You may still run irc by using the -d switch\n");
		exit(1);
	}

	if ((co = tgetnum("co")) == -1)
		co = 80;
	if ((li = tgetnum("li")) == -1)
		li = 24;
	ptr = termcap;

	/*
	 * Thanks to Max Bell (mbell@cie.uoregon.edu) for info about TVI
	 * terminals and the sg terminal capability 
	 */
	SG = tgetnum("sg");
	CM = tgetstr("cm", &ptr);
	CL = tgetstr("cl", &ptr);
	if ((CM == (char *) 0) ||
	    (CL == (char *) 0))
	{
		fprintf(stderr, "This terminal does not have the necessary capabilities to run IRCII\nin full screen mode. You may still run irc by using the -d switch\n");
		exit(1);
	}
	if ((CR = tgetstr("cr", &ptr)) == (char *) 0)
		CR = "\r";
	if ((NL = tgetstr("nl", &ptr)) == (char *) 0)
		NL = "\n";

	if ((CE = tgetstr("ce", &ptr)) != NULL)
		term_clear_to_eol = term_CE_clear_to_eol;
	else
		term_clear_to_eol = term_null_function;

	TE = tgetstr("te", &ptr);
	if (!tflag && TE && (TI = tgetstr("ti", &ptr)) != (char *) 0 )
		tputs_x(TI);
	else
		TE = TI = (char *) 0;

	/* if ((ND = tgetstr("nd", &ptr)) || (ND = tgetstr("kr", &ptr))) */
	if ((ND = tgetstr("nd", &ptr)) != NULL)
		term_cursor_right = term_ND_cursor_right;
	else
		term_cursor_right = term_null_function;

	/* if ((LE = tgetstr("le", &ptr)) || (LE = tgetstr("kl", &ptr))) */
	if ((LE = tgetstr("le", &ptr)) != NULL)
		term_cursor_left = term_LE_cursor_left;
	else if (tgetflag("bs"))
		term_cursor_left = term_BS_cursor_left;
	else
		term_cursor_left = term_null_function;

	SF = tgetstr("sf", &ptr);
	SR = tgetstr("sr", &ptr);

	if ((CS = tgetstr("cs", &ptr)) != NULL)
		term_scroll = term_CS_scroll;
	else if ((AL = tgetstr("AL", &ptr)) && (DL = tgetstr("DL", &ptr)))
		term_scroll = term_param_ALDL_scroll;
	else if ((AL = tgetstr("al", &ptr)) && (DL = tgetstr("dl", &ptr)))
		term_scroll = term_ALDL_scroll;
	else
		term_scroll = (int (*)(int, int, int)) term_null_function;

	if ((IC = tgetstr("ic", &ptr)) != NULL)
		term_insert = term_IC_insert;
	else
	{
		if ((IM = tgetstr("im", &ptr)) && (EI = tgetstr("ei", &ptr)))
			term_insert = term_IMEI_insert;
		else
			term_insert = (int (*)(u_int)) term_null_function;
	}

	if ((DC = tgetstr("dc", &ptr)) != NULL)
		term_delete = term_DC_delete;
	else
		term_delete = term_null_function;

	SO = tgetstr("so", &ptr);
	SE = tgetstr("se", &ptr);
	if ((SO == (char *) 0) || (SE == (char *) 0))
	{
		SO = CP(empty_string);
		SE = CP(empty_string);
	}
	US = tgetstr("us", &ptr);
	UE = tgetstr("ue", &ptr);
	if ((US == (char *) 0) || (UE == (char *) 0))
	{
		US = CP(empty_string);
		UE = CP(empty_string);
	}
	MD = tgetstr("md", &ptr);
	ME = tgetstr("me", &ptr);
	if ((MD == (char *) 0) || (ME == (char *) 0))
	{
		MD = CP(empty_string);
		ME = CP(empty_string);
	}
	if ((BL = tgetstr("bl", &ptr)) == (char *) 0)
		BL = "\007";
#endif /* STTY_ONLY */

	if (getenv("IRC_DEBUG")|| (tty_des = open("/dev/tty", O_RDWR, 0)) == -1)
		tty_des = 0;

#ifdef HAVE_TERMIOS_H
	tcgetattr(tty_des, &oldb);
#else
# ifdef USE_SGTTY
	ioctl(tty_des, TIOCGETC, &oldtchars);
	ioctl(tty_des, TIOCGETP, &oldb);
# else
	ioctl(tty_des, TCGETA, &oldb);
# endif /* USE_SGTTY */
#endif /* HAVE_TERMIOS_H */

#ifdef USE_SGTTY
	newb = oldb;
	newb.sg_flags &= ~CRMOD;
# ifdef TIOCLSET
	ioctl(tty_des, TIOCLGET, &old_local_modes);
	new_local_modes = old_local_modes | LDECCTQ | LLITOUT | LNOFLSH;
	ioctl(tty_des, TIOCLSET, &new_local_modes);
# endif /* TIOCLSET */


# ifndef STTY_ONLY
	if (use_flow_control)
	{
		newtchars.t_startc = oldtchars.t_startc;
		newtchars.t_stopc = oldtchars.t_stopc;
	}
	newb.sg_flags |= CBREAK;
# else
	newb.sg_flags |= RAW;
# endif /* STTY_ONLY */

# if !defined(_HPUX_SOURCE)
	newb.sg_flags &= (~ECHO);
# endif /* _HPUX_SOURCE */

#else /* USE_SGTTY */

	newb = oldb;
	newb.c_lflag &= ~(ICANON | ECHO);	/* set equivalent of
						 * CBREAK and no ECHO
						 */
	newb.c_cc[VMIN] = 1;	/* read() satified after 1 char */
	newb.c_cc[VTIME] = 0;	/* No timer */

#ifndef _POSIX_VDISABLE
# define _POSIX_VDISABLE 0
#endif /* !_POSIX_VDISABLE */
	newb.c_cc[VQUIT] = _POSIX_VDISABLE;
# ifdef VDISCARD
	newb.c_cc[VDISCARD] = _POSIX_VDISABLE;
# endif /* VDISCARD */
# ifdef VDSUSP
	newb.c_cc[VDSUSP] = _POSIX_VDISABLE;
# endif /* VDSUSP */
# ifdef VSUSP
	newb.c_cc[VSUSP] = _POSIX_VDISABLE;
# endif /* VSUSP */

# ifndef STTY_ONLY
	if (!use_flow_control)
		newb.c_iflag &= ~IXON;	/* No XON/XOFF */
# endif /* STTY_ONLY */

#endif /* USE_SGTTY */

#ifdef HAVE_TERMIOS_H
	tcsetattr(tty_des, TCSADRAIN, &newb);
#else
# ifdef USE_SGTTY
	ioctl(tty_des, TIOCSETC, &newtchars);
	ioctl(tty_des, TIOCSETP, &newb);
# else
	ioctl(tty_des, TCSETA, &newb);
# endif /* USE_SGTTY */
#endif /* HAVE_TERMIOS_H */
}
