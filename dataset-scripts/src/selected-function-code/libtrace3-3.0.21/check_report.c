static void check_report(libtrace_packet_t *packet) {
	struct timeval ts;

	/* Get the timestamp for the current packet */
	ts = trace_get_timeval(packet);

	/* If next_report is zero, then this is the first packet from the
	 * trace so we need to determine the time at which the first report 
	 * must occur, i.e. "interval" seconds from now. */

	if (next_report == 0) {
		next_report = ts.tv_sec + interval;

		/* Good opportunity to print some column headings */
		printf("Time,TCP Headers,TCP Payload,UDP Headers,UDP Payload,Not IP\n");
	}

	/* Check whether we need to report our stats
	 *
	 * Compare the timestamp for the current packet against the time that
	 * the next report is due, a la timedemo.c
	 */

	while ((uint32_t)ts.tv_sec > next_report) {
		/* Print all our stats */
		print_stats();

		/* Reset the counters */
		tcp_header = 0;
		tcp_payload = 0;
		udp_header = 0;
		udp_payload = 0;
		not_ip = 0;

		/* Determine when the next report is due */
		next_report += interval;
	}
}