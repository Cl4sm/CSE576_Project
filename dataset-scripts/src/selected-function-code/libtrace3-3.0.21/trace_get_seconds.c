DLLEXPORT double trace_get_seconds(const libtrace_packet_t *packet) {
	double seconds = 0.0;

	if (packet->trace->format->get_seconds) {
		/* seconds->seconds */
		seconds = packet->trace->format->get_seconds(packet);
	} else if (packet->trace->format->get_erf_timestamp) {
		/* timestamp -> seconds */
		uint64_t ts = 0;
		ts = packet->trace->format->get_erf_timestamp(packet);
		seconds =  (ts>>32) + ((ts & UINT_MAX)*1.0 / UINT_MAX);
	} else if (packet->trace->format->get_timespec) {
		/* timespec -> seconds */
		struct timespec ts;
		ts = packet->trace->format->get_timespec(packet);
		seconds = ts.tv_sec + ((ts.tv_nsec * 1.0) / 1000000000);
	} else if (packet->trace->format->get_timeval) {
		/* timeval -> seconds */
		struct timeval tv;
		tv = packet->trace->format->get_timeval(packet);
		seconds = tv.tv_sec + ((tv.tv_usec * 1.0) / 1000000);
	}

	return seconds;
}