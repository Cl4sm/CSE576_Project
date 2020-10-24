DLLEXPORT uint64_t trace_get_erf_timestamp(const libtrace_packet_t *packet) {
	if (packet->trace->format->get_erf_timestamp) {
		/* timestamp -> timestamp */
		return packet->trace->format->get_erf_timestamp(packet);
	} else if (packet->trace->format->get_timespec) {
		/* timespec -> timestamp */
		struct timespec ts;
		ts = packet->trace->format->get_timespec(packet);
		return ((((uint64_t)ts.tv_sec) << 32) +
				(((uint64_t)ts.tv_nsec << 32)/1000000000));
	} else if (packet->trace->format->get_timeval) {
		/* timeval -> timestamp */
		struct timeval tv;
		tv = packet->trace->format->get_timeval(packet);
		return ((((uint64_t)tv.tv_sec) << 32) +
				(((uint64_t)tv.tv_usec << 32)/1000000));
	} else if (packet->trace->format->get_seconds) {
		/* seconds -> timestamp */
		double seconds = packet->trace->format->get_seconds(packet);
		return (((uint64_t)seconds)<<32)
		          + (uint64_t)((seconds-(uint64_t)seconds)*UINT_MAX);
	}
	else {
		return (uint64_t)0;
	}
		      
}