term_reset()
{
#ifdef HAVE_TERMIOS_H
	tcsetattr(tty_des, TCSADRAIN, &oldb);
#else
# ifdef USE_SGTTY
	ioctl(tty_des, TIOCSETC, &oldtchars);
	ioctl(tty_des, TIOCSETP, &oldb);
# ifdef TIOCLSET
	ioctl(tty_des, TIOCLSET, &old_local_modes);
# endif /* TIOCLSET */
# else
	ioctl(tty_des, TCSETA, &oldb);
# endif /* USE_SGTTY */

#endif /* HAVE_TERMIOS_H */

	if (CS)
		tputs_x(tgoto(CS, current_screen->li - 1, 0));
	if (!tflag && TE)
		tputs_x(TE);
	term_move_cursor(0, current_screen->li - 1);
	term_reset_flag = 1;
	term_flush();
}
