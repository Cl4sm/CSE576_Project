static uint64_t calc_transport_size(libtrace_packet_t *packet) {
	
	uint64_t trans_size = 0;

	void *transport;
	void *nexthdr;
	uint8_t proto;
	uint32_t remaining;
	uint32_t prev_rem;
	libtrace_tcp_t *tcp;
	libtrace_udp_t *udp;

	/* Start by finding the transport header */
	transport = trace_get_transport(packet, &proto, &remaining);

	/* No transport header makes our life very easy - we can just return
	 * zero */
	if (transport == NULL)
		return trans_size;

	prev_rem = remaining;

	/* Skip past the transport header. Transport headers (at least the ones
	 * we're interested in) can't be stacked so we only ever need to skip
	 * past the one header */

	/* Switch based on the protocol value set by trace_get_transport */
	switch (proto) {
		case 6:		/* TCP */
			tcp = (libtrace_tcp_t *)transport;
			nexthdr = trace_get_payload_from_tcp(tcp, &remaining);
			break;
		case 17:	/* UDP */
			udp = (libtrace_udp_t *)transport;
			nexthdr = trace_get_payload_from_udp(udp, &remaining);
			break;
		default:
			/* We have no interest in ICMP, GRE etc, and we 
			 * should never have entered this function if the
			 * packet is using those protocols anyway! */
			fprintf(stderr, "Unexpected protocol: %u\n", proto);
			return 0;
	}

	/* If we don't have any post-transport payload, just return the 
	 * transport header size */
	if (!nexthdr)
		return trans_size;
	
	/* Determine how many bytes we just skipped over and add it to the
	 * total transport size */
	assert(prev_rem >= remaining);
	trans_size += (prev_rem - remaining);

	/* Return the total size */
	return trans_size;
}