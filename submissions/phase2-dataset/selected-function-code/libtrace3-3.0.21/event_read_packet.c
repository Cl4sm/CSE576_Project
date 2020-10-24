static uint32_t event_read_packet(libtrace_t *trace, libtrace_packet_t *packet) 
{
	libtrace_eventobj_t obj;
	fd_set rfds;
	struct timeval sleep_tv;

	FD_ZERO(&rfds);

	for (;;) {
		obj = trace_event(trace, packet);

		switch(obj.type) {

			/* Device has no packets at present - lets wait until
			 * it does get something */
			case TRACE_EVENT_IOWAIT:
				FD_ZERO(&rfds);
				FD_SET(obj.fd, &rfds);
				select(obj.fd + 1, &rfds, NULL, NULL, 0);
				continue;

				/* Replaying a trace in tracetime and the next packet
				 * is not due yet */
			case TRACE_EVENT_SLEEP:
				/* select offers good precision for sleeping */
				sleep_tv.tv_sec = (int)obj.seconds;
				sleep_tv.tv_usec = (int) ((obj.seconds - sleep_tv.tv_sec) * 1000000.0);
				select(0, NULL, NULL, NULL, &sleep_tv);
				continue;

				/* We've got a packet! */
			case TRACE_EVENT_PACKET:
				/* Check for error first */
				if (obj.size == -1)
					return -1;
				return 1;

				/* End of trace has been reached */
			case TRACE_EVENT_TERMINATE:
				return -1;

				/* An event we don't know about has occured */
			default:
				fprintf(stderr, "Unknown event type occured\n");
				return -1;
		}
	}
}