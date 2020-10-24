static int atmhdr_read_packet(libtrace_t *libtrace, libtrace_packet_t *packet) {
	int numbytes;
	void *buffer;
	uint32_t flags = 0;
	
	/* Make sure we have a buffer available to read the next record into */
	if (!packet->buffer || packet->buf_control == TRACE_CTRL_EXTERNAL) {
		packet->buffer=malloc((size_t)LIBTRACE_PACKET_BUFSIZE);
	}
	buffer = packet->buffer;
	flags |= TRACE_PREP_OWN_BUFFER;
	
	packet->type = TRACE_RT_DATA_ATMHDR;

	/* The records are a fixed size so we can read the entire record in
	 * one go */
	if ((numbytes=wandio_read(libtrace->io, buffer, (size_t)12)) != 12)
	{
		if (numbytes != 0) {
			trace_set_err(libtrace,errno,"read(%s)",libtrace->uridata);
		}
		return numbytes;
	}

	/* Update all our packet pointers appropriately */
	if (atmhdr_prepare_packet(libtrace, packet, buffer, 
				TRACE_RT_DATA_ATMHDR, flags)) {
		return -1;
	}
				
	
	return 12;
}