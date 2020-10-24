DLLEXPORT uint16_t *trace_checksum_transport(libtrace_packet_t *packet, 
		uint16_t *csum) {

	void *header = NULL;
	uint16_t ethertype;
	uint32_t remaining;
	uint32_t sum = 0;
	uint8_t proto = 0;
	uint16_t *csum_ptr = NULL;
	int plen = 0;

	uint8_t safety[65536];
	uint8_t *ptr = safety;

	header = trace_get_layer3(packet, &ethertype, &remaining);

	if (header == NULL)
		return NULL;
	
	if (ethertype == TRACE_ETHERTYPE_IP) {
		libtrace_ip_t *ip = (libtrace_ip_t *)header;

		if (remaining < sizeof(libtrace_ip_t))
			return NULL;

		sum = ipv4_pseudo_checksum(ip);

	} else if (ethertype == TRACE_ETHERTYPE_IPV6) {
		libtrace_ip6_t *ip = (libtrace_ip6_t *)header;
		
		if (remaining < sizeof(libtrace_ip6_t))
			return 0;

		sum = ipv6_pseudo_checksum(ip);
	
	}

	header = trace_get_transport(packet, &proto, &remaining);

	if (proto == TRACE_IPPROTO_TCP) {
		libtrace_tcp_t *tcp = (libtrace_tcp_t *)header;
		header = trace_get_payload_from_tcp(tcp, &remaining);
		
		csum_ptr = &tcp->check;

		memcpy(ptr, tcp, tcp->doff * 4);

		tcp = (libtrace_tcp_t *)ptr;
		tcp->check = 0;

		ptr += (tcp->doff * 4);
	} 
	
	else if (proto == TRACE_IPPROTO_UDP) {

		libtrace_udp_t *udp = (libtrace_udp_t *)header;
		header = trace_get_payload_from_udp(udp, &remaining);
		
		csum_ptr = &udp->check;
		memcpy(ptr, udp, sizeof(libtrace_udp_t));

		udp = (libtrace_udp_t *)ptr;
		udp->check = 0;

		ptr += sizeof(libtrace_udp_t);
	} 
	
	else if (proto == TRACE_IPPROTO_ICMP) {
		/* ICMP doesn't use the pseudo header */
		sum = 0;

		libtrace_icmp_t *icmp = (libtrace_icmp_t *)header;
		header = trace_get_payload_from_icmp(icmp, &remaining);
		
		csum_ptr = &icmp->checksum;
		memcpy(ptr, icmp, sizeof(libtrace_icmp_t));

		icmp = (libtrace_icmp_t *)ptr;
		icmp->checksum = 0;
		
		ptr += sizeof(libtrace_icmp_t);

	} 
	else {
		return NULL;
	}

	sum += add_checksum(safety, (uint16_t)(ptr - safety));

	plen = trace_get_payload_length(packet);
	if (plen < 0)
		return NULL;

	if (remaining < (uint32_t)plen)
		return NULL;

	if (header == NULL)
		return NULL;

	sum += add_checksum(header, (uint16_t)plen);
	*csum = ntohs(finish_checksum(sum));
	//assert(0);
	
	return csum_ptr;
}