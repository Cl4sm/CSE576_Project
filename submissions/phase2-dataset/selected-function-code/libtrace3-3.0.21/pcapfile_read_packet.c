static int pcapfile_read_packet(libtrace_t *libtrace, libtrace_packet_t *packet)
{
	int err;
	uint32_t flags = 0;
	size_t bytes_to_read = 0;

	assert(libtrace->format_data);

	packet->type = pcap_linktype_to_rt(swapl(libtrace,
				DATA(libtrace)->header.network));

	if (!packet->buffer || packet->buf_control == TRACE_CTRL_EXTERNAL) {
		packet->buffer = malloc((size_t)LIBTRACE_PACKET_BUFSIZE);
	}

	flags |= TRACE_PREP_OWN_BUFFER;
	
	err=wandio_read(libtrace->io,
			packet->buffer,
			sizeof(libtrace_pcapfile_pkt_hdr_t));

	if (err<0) {
		trace_set_err(libtrace,errno,"reading packet");
		return -1;
	}
	if (err==0) {
		/* EOF */
		return 0;
	}

	bytes_to_read = swapl(libtrace,((libtrace_pcapfile_pkt_hdr_t*)packet->buffer)->caplen);

	if (bytes_to_read >= LIBTRACE_PACKET_BUFSIZE) {
		trace_set_err(libtrace, TRACE_ERR_BAD_PACKET, "Invalid caplen in pcap header (%u) - trace may be corrupt", (uint32_t)bytes_to_read);
		return -1;
	}

	assert(bytes_to_read < LIBTRACE_PACKET_BUFSIZE);

	/* If there is no payload to read, do not ask wandio_read to try and
	 * read zero bytes - we'll just get back a zero that we will 
	 * misinterpret as EOF! */
	if (bytes_to_read == 0) {
		packet->header = packet->buffer;
		return sizeof(libtrace_pcapfile_pkt_hdr_t);
	}

	err=wandio_read(libtrace->io,
			(char*)packet->buffer+sizeof(libtrace_pcapfile_pkt_hdr_t),
			(size_t)swapl(libtrace,((libtrace_pcapfile_pkt_hdr_t*)packet->buffer)->caplen)
			);

	
	if (err<0) {
		trace_set_err(libtrace,errno,"reading packet");
		return -1;
	}
	if (err==0) {
		return 0;
	}

	if (pcapfile_prepare_packet(libtrace, packet, packet->buffer,
				packet->type, flags)) {
		return -1;
	}

	/* We may as well cache this value now, seeing as we already had to 
	 * look it up */
	packet->capture_length = bytes_to_read;	
	return sizeof(libtrace_pcapfile_pkt_hdr_t) + bytes_to_read;
}