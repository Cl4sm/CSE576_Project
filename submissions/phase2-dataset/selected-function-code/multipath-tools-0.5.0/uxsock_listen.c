			void * trigger_data)
{
	int ux_sock;
	size_t len;
	int rlen;
	char *inbuf;
	char *reply;
	sigset_t mask;

	ux_sock = ux_socket_listen(DEFAULT_SOCKET);

	if (ux_sock == -1)
		exit(1);

	pthread_cleanup_push(uxsock_cleanup, NULL);

	polls = (struct pollfd *)MALLOC(0);
	pthread_sigmask(SIG_SETMASK, NULL, &mask);
	sigdelset(&mask, SIGHUP);
	sigdelset(&mask, SIGUSR1);
	while (1) {
		struct client *c;
		int i, poll_count;

		/* setup for a poll */
		polls = REALLOC(polls, (1+num_clients) * sizeof(*polls));
		polls[0].fd = ux_sock;
		polls[0].events = POLLIN;

		/* setup the clients */
		for (i=1, c = clients; c; i++, c = c->next) {
			polls[i].fd = c->fd;
			polls[i].events = POLLIN;
		}

		/* most of our life is spent in this call */
		poll_count = ppoll(polls, i, &sleep_time, &mask);

		if (poll_count == -1) {
			if (errno == EINTR) {
				handle_signals();
				continue;
			}

			/* something went badly wrong! */
			condlog(0, "poll");
			pthread_exit(NULL);
		}

		if (poll_count == 0)
			continue;

		/* see if a client wants to speak to us */
		for (i=1, c = clients; c; i++) {
			struct client *next = c->next;

			if (polls[i].revents & POLLIN) {
				if (recv_packet(c->fd, &inbuf, &len) != 0) {
					dead_client(c);
				} else {
					inbuf[len - 1] = 0;
					condlog(4, "Got request [%s]", inbuf);
					uxsock_trigger(inbuf, &reply, &rlen,
						       trigger_data);
					if (reply) {
						if (send_packet(c->fd, reply,
								rlen) != 0) {
							dead_client(c);
						}
						condlog(4, "Reply [%d bytes]",
							rlen);
						FREE(reply);
						reply = NULL;
					}
					FREE(inbuf);
				}
			}
			c = next;
		}

		/* see if we got a new client */
		if (polls[0].revents & POLLIN) {
			new_client(ux_sock);
		}
	}

	pthread_cleanup_pop(1);
	close(ux_sock);
	return NULL;
}
