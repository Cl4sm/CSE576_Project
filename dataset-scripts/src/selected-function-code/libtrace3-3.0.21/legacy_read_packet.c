static int legacy_read_packet(libtrace_t *libtrace, libtrace_packet_t *packet) {
	int numbytes;
	void *buffer;
	uint32_t flags = 0;
	
	if (!packet->buffer || packet->buf_control == TRACE_CTRL_EXTERNAL) {
		packet->buffer=malloc((size_t)LIBTRACE_PACKET_BUFSIZE);
	}
	flags |= TRACE_PREP_OWN_BUFFER;
	buffer = packet->buffer;

	switch(libtrace->format->type) {
		case TRACE_FORMAT_LEGACY_ATM:
			packet->type = TRACE_RT_DATA_LEGACY_ATM;
			break;
		case TRACE_FORMAT_LEGACY_POS:
			packet->type = TRACE_RT_DATA_LEGACY_POS;
			break;
		case TRACE_FORMAT_LEGACY_ETH:
			packet->type = TRACE_RT_DATA_LEGACY_ETH;
			break;
		default:
			assert(0);
	}

	/* This is going to block until we either get an entire record
	 * or we reach the end of the file */
	while (1) {
	
		if ((numbytes=wandio_read(libtrace->io,
						buffer,
						(size_t)64)) != 64) {
			if (numbytes < 0) {
				trace_set_err(libtrace,errno,"read(%s)",libtrace->uridata);
			} else if (numbytes > 0) {
				
				continue;
			}
			return numbytes;
		}
		break;
	}
	
	if (legacy_prepare_packet(libtrace, packet, packet->buffer, 
				packet->type, flags)) {
		return -1;
	}
	
	return 64;
	
}