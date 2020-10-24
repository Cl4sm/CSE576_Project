DLLEXPORT libtrace_packet_t *trace_copy_packet(const libtrace_packet_t *packet) {
	libtrace_packet_t *dest = 
		(libtrace_packet_t *)malloc(sizeof(libtrace_packet_t));
	if (!dest) {
		printf("Out of memory constructing packet\n");
		abort();
	}
	dest->trace=packet->trace;
	dest->buffer=malloc(65536);
	if (!dest->buffer) {
		printf("Out of memory allocating buffer memory\n");
		abort();
	}
	dest->header=dest->buffer;
	dest->payload=(void*)
		((char*)dest->buffer+trace_get_framing_length(packet));
	dest->type=packet->type;
	dest->buf_control=TRACE_CTRL_PACKET;
	/* Reset the cache - better to recalculate than try to convert
	 * the values over to the new packet */
	trace_clear_cache(dest);	
	/* Ooooh nasty memcpys! This is why we want to avoid copying packets
	 * as much as possible */
	memcpy(dest->header,packet->header,trace_get_framing_length(packet));
	memcpy(dest->payload,packet->payload,trace_get_capture_length(packet));

	return dest;
}