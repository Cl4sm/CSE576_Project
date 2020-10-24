int
tty_nonblock(int tty, int nonblock) {
	int old, new;

	old = fcntl(tty, F_GETFL, 0);
	if (old == -1) {
		perror("fcntl(F_GETFL)");
		exit(1);
	}
	if (nonblock)
		new = old|O_NONBLOCK;
	else
		new = old & ~O_NONBLOCK;
	if (new != old) {
		if (fcntl(tty, F_SETFL, new) == -1) {
			perror("fcntl(F_SETFL)");
			exit(1);
		}
	}
	return ((old & O_NONBLOCK) != 0);
}
