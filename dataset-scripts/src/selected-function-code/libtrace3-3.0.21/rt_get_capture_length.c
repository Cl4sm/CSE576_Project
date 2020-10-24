static int rt_get_capture_length(const libtrace_packet_t *packet) {
	rt_metadata_t *rt_md_hdr;
	switch (packet->type) {
		case TRACE_RT_STATUS:
			return sizeof(rt_status_t);
		case TRACE_RT_HELLO:
			return sizeof(rt_hello_t);
		case TRACE_RT_START:
			return 0;
		case TRACE_RT_ACK:
			return sizeof(rt_ack_t);
		case TRACE_RT_END_DATA:
			return 0;
		case TRACE_RT_CLOSE:
			return 0;
		case TRACE_RT_DENY_CONN:
			return sizeof(rt_deny_conn_t);
		case TRACE_RT_PAUSE:
			return 0; 
		case TRACE_RT_PAUSE_ACK:
			return 0;
		case TRACE_RT_OPTION:
			return 0; /* FIXME */
		case TRACE_RT_KEYCHANGE:
			return 0;
		case TRACE_RT_LOSTCONN:
			return 0;
		case TRACE_RT_SERVERSTART:
			return 0;
		case TRACE_RT_CLIENTDROP:
			return 0;
		case TRACE_RT_METADATA:
			/* This is a little trickier to work out */
			rt_md_hdr = (rt_metadata_t *)packet->buffer;
			return rt_md_hdr->label_len + rt_md_hdr->value_len + 
				sizeof(rt_metadata_t);
		default:
			printf("Unknown type: %d\n", packet->type);
			
	}
	return 0;
}