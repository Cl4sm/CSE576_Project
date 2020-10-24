void drops_per_trace(libtrace_t *trace)
{
	uint64_t packets;

	packets = trace_get_received_packets(trace);
	if (packets != UINT64_MAX) {
		received_packets+=packets;
		has_received=true;
	}

	packets = trace_get_filtered_packets(trace);
	if (packets != UINT64_MAX) {
		filtered_packets+=packets;
		has_filtered=true;
	}

	packets = trace_get_dropped_packets(trace);
	if (packets != UINT64_MAX) {
		dropped_packets+=packets;
		has_dropped=true;
	}

	packets = trace_get_accepted_packets(trace);
	if (packets != UINT64_MAX) {
		accepted_packets+=packets;
		has_accepted=true;
	}
}