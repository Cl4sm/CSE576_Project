DLLEXPORT libtrace_eventobj_t trace_event(libtrace_t *trace, 
		libtrace_packet_t *packet) {
	libtrace_eventobj_t event = {TRACE_EVENT_IOWAIT,0,0.0,0};

	if (!trace) {
		fprintf(stderr,"You called trace_event() with a NULL trace object!\n");
	}
	assert(trace);
	assert(packet);

	/* Clear the packet cache */
	trace_clear_cache(packet);
	
	/* Store the trace we are reading from into the packet opaque
	 * structure */
	packet->trace = trace;

	if (packet->trace->format->trace_event) {
		/* Note: incrementing accepted, filtered etc. packet
                 * counters is handled by the format-specific 
                 * function so don't increment them here.
                 */
                event=packet->trace->format->trace_event(trace,packet);
	}
	return event;

}