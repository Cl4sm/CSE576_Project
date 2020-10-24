DLLEXPORT struct timeval trace_get_timeval(const libtrace_packet_t *packet) {
        struct timeval tv;
	uint64_t ts = 0;
	if (packet->trace->format->get_timeval) {
		/* timeval -> timeval */
		tv = packet->trace->format->get_timeval(packet);
	} else if (packet->trace->format->get_erf_timestamp) {
		/* timestamp -> timeval */
		ts = packet->trace->format->get_erf_timestamp(packet);
		tv.tv_sec = ts >> 32;
		tv.tv_usec = ((ts&0xFFFFFFFF)*1000000)>>32;
       		if (tv.tv_usec >= 1000000) {
               		tv.tv_usec -= 1000000;
               		tv.tv_sec += 1;
       		}
	} else if (packet->trace->format->get_timespec) {
		struct timespec ts = packet->trace->format->get_timespec(packet);
		tv.tv_sec = ts.tv_sec;
		tv.tv_usec = ts.tv_nsec/1000;
	} else if (packet->trace->format->get_seconds) {
		/* seconds -> timeval */
		double seconds = packet->trace->format->get_seconds(packet);
		tv.tv_sec = (uint32_t)seconds;
		tv.tv_usec = (uint32_t)(((seconds - tv.tv_sec) * 1000000)/UINT_MAX);
	}
	else {
		tv.tv_sec=-1;
		tv.tv_usec=-1;
	}

        return tv;
}