extern	void
set_term_eight_bit(value)
	int	value;
{
#ifndef	STTY_ONLY
	if (dumb)
		return;
#endif /* STTY_ONLY */
#ifdef USE_SGTTY
	if (value == ON)
		new_local_modes |= LPASS8;
	else
		new_local_modes &= ~LPASS8;
	ioctl(tty_des, TIOCLSET, &new_local_modes);
#else
	if (value == ON)
	{
		newb.c_cflag |= CS8;
		newb.c_iflag &= ~ISTRIP;
	}
	else
	{
		newb.c_cflag &= ~CS8;
		newb.c_iflag |= ISTRIP;
	}
# ifdef HAVE_TERMIOS_H
	tcsetattr(tty_des, TCSADRAIN, &newb);
# else
	ioctl(tty_des, TCSETA, &newb);
# endif /* HAVE_TERMIOS_H */
#endif /* USE_SGTTY */
}
