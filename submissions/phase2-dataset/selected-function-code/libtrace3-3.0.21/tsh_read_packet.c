static int tsh_read_packet(libtrace_t *libtrace, libtrace_packet_t *packet) {
	int numbytes;
	void *buffer2 = packet->buffer;
	uint32_t flags = 0;

	if (!packet->buffer || packet->buf_control == TRACE_CTRL_EXTERNAL) {
		packet->buffer = malloc((size_t)LIBTRACE_PACKET_BUFSIZE);
		if (!packet->buffer) {
			trace_set_err(libtrace, errno, 
					"Cannot allocate memory");
			return -1;
		}
	}

	flags |= TRACE_PREP_OWN_BUFFER;
	packet->type = TRACE_RT_DATA_TSH;

	buffer2 = packet->buffer;

	/* Read the TSH header */
	if ((numbytes=wandio_read(libtrace->io,
					buffer2,
					(size_t)sizeof(tsh_pkt_header_t))) == -1) {
		trace_set_err(libtrace,errno,"read(%s)",
				libtrace->uridata);
		return -1;
	}
	/* EOF */
	if (numbytes == 0) {
		return 0;
	}

	buffer2 = (char*)buffer2 + numbytes;

	/* Read the IP header */
	if ((numbytes=wandio_read(libtrace->io,
				buffer2,
				(size_t)sizeof(libtrace_ip_t)+16))  /* 16 bytes of transport header */
			!= sizeof(libtrace_ip_t)+16) {
		trace_set_err(libtrace,errno,"read(%s)",
				libtrace->uridata);
		return -1;
	}

#if 0
	/* IP Options aren't captured in the trace, so leave room
	 * for them, and put the transport header where it "should" be
	 */
	buffer2 = (char*)buffer2 + ((libtrace_ip_t*)buffer2)->ip_hl*4;

	/* Read the transport header */
	if ((numbytes=wandio_read(libtrace->io,
				buffer2,
				16)) != 16) {
		trace_set_err(libtrace,errno,"read(%s)",
				libtrace->uridata);
		return -1;
	}
#endif
	
	if (tsh_prepare_packet(libtrace, packet, packet->buffer, packet->type, 
				flags)) {
		return -1;
	}


	return 80;
}