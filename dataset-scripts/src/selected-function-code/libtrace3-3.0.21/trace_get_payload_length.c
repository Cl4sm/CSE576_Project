DLLEXPORT size_t trace_get_payload_length(const libtrace_packet_t *packet) {

	void *layer;
	uint16_t ethertype;
	uint8_t proto;
	uint32_t rem;
	libtrace_ip_t *ip;
	libtrace_ip6_t *ip6;
	libtrace_tcp_t *tcp;
	size_t len = 0;

	/* Just use the cached length if we can */
	if (packet->payload_length != -1)
		return packet->payload_length;	

	/* Set to zero so that we can return early without having to 
	 * worry about forgetting to update the cached value */
	((libtrace_packet_t *)packet)->payload_length = 0;
	layer = trace_get_layer3(packet, &ethertype, &rem);
	if (!layer)
		return 0;
	switch (ethertype) {
		case TRACE_ETHERTYPE_IP:
			ip = (libtrace_ip_t *)layer;
			if (rem < sizeof(libtrace_ip_t))
				return 0;
			len = ntohs(ip->ip_len) - (4 * ip->ip_hl);
		
			/* Deal with v6 within v4 */
			if (ip->ip_p == TRACE_IPPROTO_IPV6)
				len -= sizeof(libtrace_ip6_t);
			
			break;
		case TRACE_ETHERTYPE_IPV6:
			ip6 = (libtrace_ip6_t *)layer;
			if (rem < sizeof(libtrace_ip6_t))
				return 0;
			len = ntohs(ip6->plen);
			break;
		default:
			return 0;
	}

	layer = trace_get_transport(packet, &proto, &rem);
	if (!layer)
		return 0;
	
	switch(proto) {
		case TRACE_IPPROTO_TCP:
			if (rem < sizeof(libtrace_tcp_t))
				return 0;
			tcp = (libtrace_tcp_t *)layer;
			
			if (len < (size_t)(4 * tcp->doff))
				return 0;
			
			len -= (4 * tcp->doff);
			break;
		case TRACE_IPPROTO_UDP:
			if (rem < sizeof(libtrace_udp_t))
				return 0;
			if (len < sizeof(libtrace_udp_t))
				return 0;
			len -= sizeof(libtrace_udp_t);
			break;
		case TRACE_IPPROTO_ICMP:
			if (rem < sizeof(libtrace_icmp_t))
				return 0;
			if (len < sizeof(libtrace_icmp_t))
				return 0;
			len -= sizeof(libtrace_icmp_t);
			break;
		case TRACE_IPPROTO_ICMPV6:
			if (rem < sizeof(libtrace_icmp6_t))
				return 0;
			if (len < sizeof(libtrace_icmp6_t))
				return 0;
			len -= sizeof(libtrace_icmp6_t);
			break;
			
		default:
			return 0;
	}

	((libtrace_packet_t *)packet)->payload_length = len;
	return len;

}