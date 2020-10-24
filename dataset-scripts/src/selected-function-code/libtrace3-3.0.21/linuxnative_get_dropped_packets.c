static uint64_t linuxnative_get_dropped_packets(libtrace_t *trace) {
	if (trace->format_data == NULL)
		return UINT64_MAX;
	if (FORMAT(trace->format_data)->fd == -1) {
		/* This is probably a 'dead' trace so obviously we can't query
		 * the socket for drop counts, can we? */
		return UINT64_MAX;
	}
	
#ifdef HAVE_NETPACKET_PACKET_H	
	if ((FORMAT(trace->format_data)->stats_valid & 2)
			|| (FORMAT(trace->format_data)->stats_valid==0)) {
		socklen_t len = sizeof(FORMAT(trace->format_data)->stats);
		getsockopt(FORMAT(trace->format_data)->fd, 
				SOL_PACKET,
				PACKET_STATISTICS,
				&FORMAT(trace->format_data)->stats,
				&len);
		FORMAT(trace->format_data)->stats_valid |= 2;
	}

	return FORMAT(trace->format_data)->stats.tp_drops;
#else
	return UINT64_MAX;
#endif
}