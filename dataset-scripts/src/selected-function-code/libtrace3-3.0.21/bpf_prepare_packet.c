static int bpf_prepare_packet(libtrace_t *libtrace UNUSED, 
		libtrace_packet_t *packet,
		void *buffer, libtrace_rt_types_t rt_type, uint32_t flags) {
        
	struct local_bpf_hdr orig;
	struct local_bpf_hdr *ptr;
	struct libtrace_bpf_hdr *replace;

	/* If the packet previously owned a buffer that is not the buffer
	 * that contains the new packet data, we're going to need to free the
	 * old one to avoid memory leaks */
	if (packet->buffer != buffer &&
                        packet->buf_control == TRACE_CTRL_PACKET) {
                free(packet->buffer);
        }

	/* Set the buffer owner appropriately */
        if ((flags & TRACE_PREP_OWN_BUFFER) == TRACE_PREP_OWN_BUFFER) {
                packet->buf_control = TRACE_CTRL_PACKET;
        } else
                packet->buf_control = TRACE_CTRL_EXTERNAL;

	/* Update the packet pointers and type appropriately */
        packet->buffer = buffer;
        packet->header = buffer;
	packet->type = rt_type;

	/* FreeBSD is stupid and uses a timeval in the bpf header
	 * structure. This means that sometimes our timestamp consumes
	 * 8 bytes and sometimes it consumes 16 bytes.
	 *
	 * Let's try to standardise our header a bit, hopefully without
	 * overwriting anything else important */

	if (sizeof(struct BPF_TIMEVAL) != sizeof(struct lt_bpf_timeval)) { 	
		
		ptr = ((struct local_bpf_hdr *)(packet->header));
		replace = ((struct libtrace_bpf_hdr *)(packet->header));
		orig = *ptr;

		replace->bh_tstamp.tv_sec = (uint32_t) (orig.bh_tstamp.tv_sec & 0xffffffff);
		replace->bh_tstamp.tv_usec = (uint32_t) (orig.bh_tstamp.tv_usec & 0xffffffff);
		replace->bh_caplen = orig.bh_caplen;
		replace->bh_datalen = orig.bh_datalen;
		replace->bh_hdrlen = orig.bh_hdrlen;


	}

	/* Find the payload */
	/* TODO: Pcap deals with a padded FDDI linktype here */
	packet->payload=(char *)buffer + BPFHDR(packet)->bh_hdrlen;

	return 0;
}