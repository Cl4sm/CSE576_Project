static uint64_t calc_payload_size(libtrace_packet_t *packet, demo_proto_t proto)
{

	uint64_t ip_plen = 0;
	uint64_t headers = 0;

	void *layer3;
	uint16_t ethertype;
	uint32_t remaining;

	layer3 = trace_get_layer3(packet, &ethertype, &remaining);

	/* This should NEVER happen, but it's a good habit to check it anyway */
	if (layer3 == NULL)
		return 0;

	/* Find the payload length in the IP header
	 *
	 * We also determine the size of the IP header (again!) as the payload
	 * length includes the IP and transport headers */
	
	if (ethertype == 0x0800) {   /* IPv4 header */
		libtrace_ip_t *ip = (libtrace_ip_t *)layer3;
		
		/* Remember to byte swap! */
		ip_plen = ntohs(ip->ip_len);
		/* This value is only 4 bits so byteswapping is unnecessary */
		headers += (4 * ip->ip_hl);
	
	
	} else if (ethertype == 0x86DD) { /* IPv6 header */
		libtrace_ip6_t *ip = (libtrace_ip6_t *)layer3;
		
		/* Remember to byte swap! */
		ip_plen = ntohs(ip->plen);
		/* IPv6 does not have a variable length header */
		headers += sizeof(libtrace_ip6_t);
	
	
	} else {
		/* Not an IP packet - this should also never happen */
		return 0;
	}

	
	/* Now we need to subtract the size of the transport header from the
	 * IP payload length. */
	if (proto == DEMO_PROTO_TCP) {
		
		/* Determine the size of the TCP header so we can subtract
		 * that from our total payload length */
		
		/* Since I already know the protocol and only need to 
		 * access a single value inside the TCP header, I can use
		 * the trace_get_tcp() helper function instead of the more
		 * verbose trace_get_transport() . */
		libtrace_tcp_t *tcp = trace_get_tcp(packet);
		if (tcp == NULL)
			return 0;

		/* Again, byteswapping not required because the doff field is
		 * only a single byte in size*/
		headers += (tcp->doff * 4);
	}

	if (proto == DEMO_PROTO_UDP) {
		/* UDP has a fixed length header so we don't even need to use
		 * trace_get_udp() */
		headers += sizeof(libtrace_udp_t);
	}

	assert(headers <= ip_plen);

	/* Subtract the length of the IP and transport headers from the 
	 * payload length contained within the IP header */
	return ip_plen - headers;

}