DLLEXPORT void *trace_get_transport(const libtrace_packet_t *packet, 
		uint8_t *proto,
		uint32_t *remaining
		) 
{
	uint8_t dummy_proto;
	uint16_t ethertype;
	uint32_t dummy_remaining;
	void *transport;

	if (!proto) proto=&dummy_proto;

	if (!remaining) remaining=&dummy_remaining;

	if (packet->l4_header) {
		/*
		void *link;
		libtrace_linktype_t linktype;
		link = trace_get_packet_buffer(packet, &linktype, remaining);
		if (!link)
			return NULL;
		*/
		*proto = packet->transport_proto;
		/* *remaining -= (packet->l4_header - link); */
		*remaining = packet->l4_remaining;
		return packet->l4_header;
	}

	transport = trace_get_layer3(packet,&ethertype,remaining);

	if (!transport || *remaining == 0)
		return NULL;

	switch (ethertype) {
		case TRACE_ETHERTYPE_IP: /* IPv4 */
			transport=trace_get_payload_from_ip(
				(libtrace_ip_t*)transport, proto, remaining);
			/* IPv6 */
			if (transport && *proto == TRACE_IPPROTO_IPV6) {
				transport=trace_get_payload_from_ip6(
				 (libtrace_ip6_t*)transport, proto,remaining);
			}
			break;
		case TRACE_ETHERTYPE_IPV6: /* IPv6 */
			transport = trace_get_payload_from_ip6(
				(libtrace_ip6_t*)transport, proto, remaining);
			break;
		default:
			*proto = 0;
			transport = NULL;
			break;
			
	}

	((libtrace_packet_t *)packet)->transport_proto = *proto;
	((libtrace_packet_t *)packet)->l4_header = transport;
	((libtrace_packet_t *)packet)->l4_remaining = *remaining;


	return transport;
}