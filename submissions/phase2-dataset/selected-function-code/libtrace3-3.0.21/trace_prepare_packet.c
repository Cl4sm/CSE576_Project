int trace_prepare_packet(libtrace_t *trace, libtrace_packet_t *packet,
		void *buffer, libtrace_rt_types_t rt_type, uint32_t flags) {

	assert(packet);
	assert(trace);
	
	/* XXX Proper error handling?? */
	if (buffer == NULL)
		return -1;

	if (!(packet->buf_control==TRACE_CTRL_PACKET || packet->buf_control==TRACE_CTRL_EXTERNAL)) {
		trace_set_err(trace,TRACE_ERR_BAD_STATE,"Packet passed to trace_read_packet() is invalid\n");
		return -1;
	}
	
	packet->trace = trace;
	
	/* Clear packet cache */
	trace_clear_cache(packet);

	if (trace->format->prepare_packet) {
		return trace->format->prepare_packet(trace, packet,
				buffer, rt_type, flags);
	}
	trace_set_err(trace, TRACE_ERR_UNSUPPORTED, 
			"This format does not support preparing packets\n");
	return -1;

}