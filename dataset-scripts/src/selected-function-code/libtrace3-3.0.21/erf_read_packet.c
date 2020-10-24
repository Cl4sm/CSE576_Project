static int erf_read_packet(libtrace_t *libtrace, libtrace_packet_t *packet) {
	int numbytes;
	unsigned int size;
	void *buffer2 = packet->buffer;
	unsigned int rlen;
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
	
	if ((numbytes=wandio_read(libtrace->io,
					packet->buffer,
					(size_t)dag_record_size)) == -1) {
		trace_set_err(libtrace,errno,"read(%s)",
				libtrace->uridata);
		return -1;
	}
	/* EOF */
	if (numbytes == 0) {
		return 0;
	}

	rlen = ntohs(((dag_record_t *)packet->buffer)->rlen);
	buffer2 = (char*)packet->buffer + dag_record_size;
	size = rlen - dag_record_size;

	if (size >= LIBTRACE_PACKET_BUFSIZE) {
		trace_set_err(libtrace, TRACE_ERR_BAD_PACKET, 
				"Packet size %u larger than supported by libtrace - packet is probably corrupt", 
				size);
		return -1;
	}

	/* Unknown/corrupt */
	if (((dag_record_t *)packet->buffer)->type >= TYPE_RAW_LINK) {
		trace_set_err(libtrace, TRACE_ERR_BAD_PACKET, 
				"Corrupt or Unknown ERF type");
		return -1;
	}
	
	/* read in the rest of the packet */
	if ((numbytes=wandio_read(libtrace->io,
					buffer2,
					(size_t)size)) != (int)size) {
		if (numbytes==-1) {
			trace_set_err(libtrace,errno, "read(%s)", 
					libtrace->uridata);
			return -1;
		}
		trace_set_err(libtrace,EIO,
				"Truncated packet (wanted %d, got %d)", 
				size, numbytes);
		/* Failed to read the full packet?  must be EOF */
		return -1;
	}
	
	if (erf_prepare_packet(libtrace, packet, packet->buffer, 
				TRACE_RT_DATA_ERF, flags))
		return -1;
	
	return rlen;
}