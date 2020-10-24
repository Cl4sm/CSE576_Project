main_loop(void) {
	FD_ZERO(&fds);
	FD_SET(Serv, &fds);
	FD_SET(STDIN_FILENO, &fds);
	highest_fd = max(Serv, STDIN_FILENO);

	for (;;) {
		fd_set readfds, exceptfds;
		int nfound, fd;

		readfds = fds;
		exceptfds = fds;
		nfound = select(highest_fd+1, &readfds, NULL,
				&exceptfds, NULL);
		if (nfound < 0 && errno == EINTR)
			continue;
		ASSERT(0<=nfound, "select");
		for (fd = 0; fd <= highest_fd; fd++) {
			if (FD_ISSET(fd, &exceptfds)) {
				if (fd == Serv)
					server_died();
			}
			if (FD_ISSET(fd, &readfds)) {
				if (fd == STDIN_FILENO)
					tty_input(fd);
				else if (fd == Serv)
					serv_input(fd);
			}
		}
	}
	/*NOTREACHED*/
}
