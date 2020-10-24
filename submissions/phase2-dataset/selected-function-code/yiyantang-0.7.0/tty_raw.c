int tty_raw (int fd)
{
    struct termios buf;

    if (tcgetattr (fd, &save_termios) < 0)
	return (-1);

    buf = save_termios;
    buf.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    buf.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    buf.c_cflag &= ~(CSIZE | PARENB);
    buf.c_cflag |= CS8;

    buf.c_oflag &= ~(OPOST);

    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;

    if (tcsetattr (fd, TCSAFLUSH, &buf) < 0)
	return (-1);
    ttystate = RAW;
    ttysavefd = fd;
    return (0);
}
