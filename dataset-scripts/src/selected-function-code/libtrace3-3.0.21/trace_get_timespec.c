DLLEXPORT struct timespec trace_get_timespec(const libtrace_packet_t *packet) {
	struct timespec ts;

	if (packet->trace->format->get_timespec) {
		return packet->trace->format->get_timespec(packet);
	} else if (packet->trace->format->get_erf_timestamp) {
		/* timestamp -> timeval */
		uint64_t erfts = packet->trace->format->get_erf_timestamp(packet);
		ts.tv_sec = erfts >> 32;
		ts.tv_nsec = ((erfts&0xFFFFFFFF)*1000000000)>>32;
       		if (ts.tv_nsec >= 1000000000) {
               		ts.tv_nsec -= 1000000000;
               		ts.tv_sec += 1;
       		}
		return ts;
	} else if (packet->trace->format->get_timeval) {
		/* timeval -> timespec */
		struct timeval tv = packet->trace->format->get_timeval(packet);
		ts.tv_sec = tv.tv_sec;
		ts.tv_nsec = tv.tv_usec*1000;
		return ts;
	} else if (packet->trace->format->get_seconds) {
		/* seconds -> timespec */
		double seconds = packet->trace->format->get_seconds(packet);
		ts.tv_sec = (uint32_t)seconds;
		ts.tv_nsec = (long)(((seconds - ts.tv_sec) * 1000000000)/UINT_MAX);
		return ts;
	}
	else {
		ts.tv_sec=-1;
		ts.tv_nsec=-1;
		return ts;
	}
}