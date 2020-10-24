static void
broadcast(u_char *buf, int nchars, u_int typ) {
	int fd, x;

	for (fd = 0;  fd <= highest_fd;  fd++) {
		if (!FD_ISSET(fd, &Clients)) {
			continue;
		}

		Sigpiped = 0;
		x = tp_senddata(fd, buf, nchars, typ);
		dprintf(stderr,
			"tty_input: %d bytes sent to client on fd%d\n",
			x, fd);
		if (Sigpiped) {
			dprintf(stderr, "tty_input: sigpipe on fd%d\n",
				fd);
			close_client(fd);
		}
	}
}
