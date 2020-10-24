int osm_console(osm_opensm_t * p_osm)
{
	struct pollfd pollfd[2];
	char *p_line;
	size_t len;
	ssize_t n;
	struct pollfd *fds;
	nfds_t nfds;
	osm_console_t *p_oct = &p_osm->console;

	pollfd[0].fd = p_oct->socket;
	pollfd[0].events = POLLIN;
	pollfd[0].revents = 0;

	pollfd[1].fd = p_oct->in_fd;
	pollfd[1].events = POLLIN;
	pollfd[1].revents = 0;

	fds = p_oct->socket < 0 ? &pollfd[1] : pollfd;
	nfds = p_oct->socket < 0 || pollfd[1].fd < 0 ? 1 : 2;

	if (loop_command.on && loop_command_check_time() &&
	    loop_command.loop_function) {
		if (p_oct->out) {
			loop_command.loop_function(p_osm, p_oct->out);
			fflush(p_oct->out);
		} else {
			loop_command.on = 0;
		}
	}

	if (poll(fds, nfds, 1000) <= 0)
		return 0;

#ifdef ENABLE_OSM_CONSOLE_LOOPBACK
	if (pollfd[0].revents & POLLIN) {
		int new_fd = 0;
		struct sockaddr_in sin;
		socklen_t len = sizeof(sin);
		struct hostent *hent;
		if ((new_fd = accept(p_oct->socket, &sin, &len)) < 0) {
			OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
				"ERR 4B04: Failed to accept console socket: %s\n",
				strerror(errno));
			p_oct->in_fd = -1;
			return 0;
		}
		if (inet_ntop
		    (AF_INET, &sin.sin_addr, p_oct->client_ip,
		     sizeof(p_oct->client_ip)) == NULL) {
			snprintf(p_oct->client_ip, sizeof(p_oct->client_ip),
				 "STRING_UNKNOWN");
		}
		if ((hent = gethostbyaddr((const char *)&sin.sin_addr,
					  sizeof(struct in_addr),
					  AF_INET)) == NULL) {
			snprintf(p_oct->client_hn, sizeof(p_oct->client_hn),
				 "STRING_UNKNOWN");
		} else {
			snprintf(p_oct->client_hn, sizeof(p_oct->client_hn),
				 "%s", hent->h_name);
		}
		if (is_authorized(p_oct)) {
			cio_open(p_oct, new_fd, &p_osm->log);
		} else {
			OSM_LOG(&p_osm->log, OSM_LOG_ERROR,
				"ERR 4B05: Console connection denied: %s (%s)\n",
				p_oct->client_hn, p_oct->client_ip);
			close(new_fd);
		}
		return 0;
	}
#endif

	if (pollfd[1].revents & POLLIN) {
		p_line = NULL;
		/* Get input line */
		n = getline(&p_line, &len, p_oct->in);
		if (n > 0) {
			/* Parse and act on input */
			parse_cmd_line(p_line, p_osm);
			if (!loop_command.on) {
				osm_console_prompt(p_oct->out);
			}
		} else
			cio_close(p_oct, &p_osm->log);
		if (p_line)
			free(p_line);
		return 0;
	}
	/* input fd is closed (hanged up) */
	if (pollfd[1].revents & POLLHUP) {
#ifdef ENABLE_OSM_CONSOLE_LOOPBACK
		/* If we are using a socket, we close the current connection */
		if (p_oct->socket >= 0) {
			cio_close(p_oct, &p_osm->log);
			return 0;
		}
#endif
		/* If we use a local console, stdin is closed (most probable is pipe ended)
		 * so we close the local console */
		return -1;
	}

	return 0;
}