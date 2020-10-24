static int legacynzix_read_packet(libtrace_t *libtrace, libtrace_packet_t *packet) {
	/* Firstly, I apologize for all the constants being thrown around in
	 * this function, but it does befit the hackish origins of the
	 * NZIX format that I use them. Anyone who wants to clean them up is
	 * welcome to do so */
	int numbytes;
	void *buffer;
	char *data_ptr;
	uint32_t flags = 0;
	
	if (!packet->buffer || packet->buf_control == TRACE_CTRL_EXTERNAL) {
		packet->buffer=malloc((size_t)LIBTRACE_PACKET_BUFSIZE);
	}
	flags |= TRACE_PREP_OWN_BUFFER;
	
	buffer = packet->buffer;
	packet->type = TRACE_RT_DATA_LEGACY_NZIX;
	
	while (1) {
		if ((numbytes = wandio_read(libtrace->io, buffer,
						(size_t)68)) != 68) {
			if (numbytes < 0) {
				trace_set_err(libtrace,errno,"read(%s)",libtrace->uridata);
			} else if (numbytes > 0)
				continue;
			return numbytes;
		} 
		/* Packets with a zero length are GPS timestamp packets
		 * but they aren't inserted at the right time to be
		 * useful - instead we'll ignore them unless we can think
		 * of a compelling reason to do otherwise */
		if (((legacy_nzix_t *)buffer)->len == 0)
			continue;
		
		break;
	}

	/* Lets move the padding so that it's in the framing header */
	data_ptr = ((char *)buffer) + 12;
	memmove(data_ptr + 2, data_ptr, 26);

	if (legacy_prepare_packet(libtrace, packet, packet->buffer, 
				packet->type, flags)) {
		return -1;
	}
	return 68;
}