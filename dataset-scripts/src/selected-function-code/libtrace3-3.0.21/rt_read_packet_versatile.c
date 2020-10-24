static int rt_read_packet_versatile(libtrace_t *libtrace,
		libtrace_packet_t *packet,int blocking) {
	rt_header_t *pkt_hdr = NULL;
	void *void_hdr;
	libtrace_rt_types_t switch_type;
	
	if (packet->buf_control == TRACE_CTRL_PACKET) {
		packet->buf_control = TRACE_CTRL_EXTERNAL;
		free(packet->buffer);
		packet->buffer = NULL;
	}

	/* RT_LAST indicates that we need to read the RT header for the next
	 * packet. This is a touch hax, I admit */
	if (RT_INFO->rt_hdr.type == TRACE_RT_LAST) {
		void_hdr = (void *)pkt_hdr;
		/* FIXME: Better error handling required */
		if (rt_read(libtrace, &void_hdr, 
				sizeof(rt_header_t),blocking) !=
				sizeof(rt_header_t)) {
			return -1;
		}
		pkt_hdr = (rt_header_t *)void_hdr;
		
		/* Need to store these in case the next rt_read overwrites 
		 * the buffer they came from! */
		RT_INFO->rt_hdr.type = pkt_hdr->type;
		RT_INFO->rt_hdr.length = pkt_hdr->length;
		RT_INFO->rt_hdr.sequence = pkt_hdr->sequence;
	}
	packet->type = RT_INFO->rt_hdr.type;
	
	/* All data-bearing packets (as opposed to RT internal messages) 
	 * should be treated the same way when it comes to reading the rest
	 * of the packet */
	if (packet->type >= TRACE_RT_DATA_SIMPLE) {
		switch_type = TRACE_RT_DATA_SIMPLE;
	} else {
		switch_type = packet->type;
	}

	switch(switch_type) {
		case TRACE_RT_DATA_SIMPLE:
		case TRACE_RT_DUCK_2_4:
		case TRACE_RT_DUCK_2_5:
		case TRACE_RT_STATUS:
		case TRACE_RT_METADATA:
			if (rt_read_data_packet(libtrace, packet, blocking))
				return -1;
			break;
		case TRACE_RT_END_DATA:
		case TRACE_RT_KEYCHANGE:
		case TRACE_RT_LOSTCONN:
		case TRACE_RT_CLIENTDROP:
		case TRACE_RT_SERVERSTART:
			/* All these have no payload */
			break;
		case TRACE_RT_PAUSE_ACK:
			/* XXX: Add support for this */
			break;
		case TRACE_RT_OPTION:
			/* XXX: Add support for this */
			break;
		default:
			printf("Bad rt type for client receipt: %d\n",
					switch_type);
			return -1;
	}
				
			
		
	/* Return the number of bytes read from the stream */
	RT_INFO->rt_hdr.type = TRACE_RT_LAST;
	return RT_INFO->rt_hdr.length + sizeof(rt_header_t);
}