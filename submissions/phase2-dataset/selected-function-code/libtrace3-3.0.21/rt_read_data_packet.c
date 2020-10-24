static int rt_read_data_packet(libtrace_t *libtrace,
		libtrace_packet_t *packet, int blocking) {
	uint32_t prep_flags = 0;

	prep_flags |= TRACE_PREP_DO_NOT_OWN_BUFFER;

	/* The stored RT header will tell us how much data we need to read */
	if (rt_read(libtrace, &packet->buffer, (size_t)RT_INFO->rt_hdr.length, 
				blocking) != RT_INFO->rt_hdr.length) {
		return -1;
	}

	/* Send an ACK if required */
        if (RT_INFO->reliable > 0 && packet->type >= TRACE_RT_DATA_SIMPLE) {
		if (rt_send_ack(libtrace, RT_INFO->rt_hdr.sequence) == -1)
                               	return -1;
	}
	
	/* Convert to the original capture format */
	if (rt_set_format(libtrace, packet) < 0) {
		return -1;
        }
               	
	/* Update payload pointers and packet type to match the original
	 * format */
	if (trace_prepare_packet(packet->trace, packet, packet->buffer,
				packet->type, prep_flags)) {
		return -1;
	}

	return 0;
}