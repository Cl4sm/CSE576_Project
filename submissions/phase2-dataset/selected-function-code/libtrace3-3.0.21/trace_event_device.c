struct libtrace_eventobj_t trace_event_device(struct libtrace_t *trace, 
					struct libtrace_packet_t *packet) {
	struct libtrace_eventobj_t event = {0,0,0.0,0};

	fd_set rfds, rfds_param;
	int ret;
	int max_fd;
	struct timeval tv;

	assert(trace != NULL);
	assert(packet != NULL);
	
	FD_ZERO(&rfds);
	FD_ZERO(&rfds_param);

	if (trace->format->get_fd) {
		event.fd = trace->format->get_fd(trace);
		FD_SET(event.fd, &rfds);
		max_fd = event.fd;
	} else {
		event.fd = 0;
		max_fd = -1;
	}

	/* Use select() to perform a quick poll to check that there is data
	 * available - we used to use FIONREAD here but that does not work
	 * for mmapped pcap sockets. As recent pcap on linux (e.g. Ubuntu 9.04)
	 * uses mmapped sockets by default, I've switched over to this 
	 * solution. */

        do {
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		rfds_param = rfds;

		ret = select(max_fd + 1, &rfds_param, NULL, NULL, &tv);
		if (ret == -1 && errno != EINTR) {
			event.type = TRACE_EVENT_TERMINATE;
			return event;
		}
	} while (ret == -1);

	if (FD_ISSET(event.fd, &rfds_param)) {
                event.size = trace_read_packet(trace,packet);
		if (event.size < 1) {
			/* Covers error and EOF events - terminate rather 
			 * than report a packet as available */
			if (trace_is_err(trace)) {
				trace_perror(trace, "read packet");
			}
			event.type = TRACE_EVENT_TERMINATE;
		} else {

			event.type = TRACE_EVENT_PACKET;
		}
		return event;
	}
	event.type= TRACE_EVENT_IOWAIT;
	return event;
}