term_cont(signo)
	int signo;
{
#ifdef SYSVSIGNALS
	(void) MY_SIGNAL(SIGCONT, term_cont, 0); /* sysv has dumb signals */
#endif /* SYSVSIGNALS */

#if defined(SIGSTOP) && defined(SIGTSTP) /* munix has no sigstop, sigtstp */

# ifdef HAVE_TERMIOS_H
	tcsetattr(tty_des, TCSADRAIN, &newb);
# else
#  ifdef USE_SGTTY
	ioctl(tty_des, TIOCSETC, &newtchars);
	ioctl(tty_des, TIOCSETP, &newb);
#   ifdef TIOCLSET
	ioctl(tty_des, TIOCLSET, &new_local_modes);
#   endif /* TIOCLSET */
#  else
	ioctl(tty_des, TCSETA, &newb);
#  endif /* USE_SGTTY */

# endif /* HAVE_TERMIOS_H */
#endif /* SIGSTOP && SIGTSTP */

	if (!tflag && TI)
		tputs_x(TI);
	
	on_signal_occurred(signo);
}
