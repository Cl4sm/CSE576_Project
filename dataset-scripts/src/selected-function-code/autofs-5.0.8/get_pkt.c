static int get_pkt(struct autofs_point *ap, union autofs_v5_packet_union *pkt)
{
	struct pollfd fds[3];
	int pollfds = 3;
	char buf[MAX_ERR_BUF];

	fds[0].fd = ap->pipefd;
	fds[0].events = POLLIN;
	fds[1].fd = ap->state_pipe[0];
	fds[1].events = POLLIN;
	fds[2].fd = ap->logpri_fifo;
	fds[2].events = POLLIN;
	if (fds[2].fd  == -1)
		pollfds--;

	for (;;) {
		if (poll(fds, pollfds, -1) == -1) {
			char *estr;
			if (errno == EINTR)
				continue;
			estr = strerror_r(errno, buf, MAX_ERR_BUF);
			logerr("poll failed: %s", estr);
			return -1;
		}

		if (fds[1].revents & POLLIN) {
			enum states next_state;
			size_t read_size = sizeof(next_state);
			int state_pipe;

			next_state = ST_INVAL;

			st_mutex_lock();

			state_pipe = ap->state_pipe[0];

			if (fullread(state_pipe, &next_state, read_size)) {
				st_mutex_unlock();
				continue;
			}

			st_mutex_unlock();

			if (next_state == ST_SHUTDOWN)
				return -1;
		}

		if (fds[0].revents & POLLIN)
			return fullread(ap->pipefd, pkt, kpkt_len);

		if (fds[2].fd != -1 && fds[2].revents & POLLIN) {
			debug(ap->logopt, "message pending on control fifo.");
			handle_fifo_message(ap, fds[2].fd);
		}
	}
}