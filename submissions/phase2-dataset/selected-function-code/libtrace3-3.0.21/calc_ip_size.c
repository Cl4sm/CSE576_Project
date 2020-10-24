static uint64_t calc_ip_size(libtrace_packet_t *packet, demo_proto_t *proto) {

	uint64_t ip_size = 0;
	
	void *ip_hdr;
	void *nexthdr = NULL;;
	uint16_t ethertype;
	uint8_t protocol;
	uint32_t remaining;
	uint32_t prev_rem;
	libtrace_ip6_t *ip6;

	/* Start by finding the first layer 3 header */
	ip_hdr = trace_get_layer3(packet, &ethertype, &remaining);

	/* If no layer 3 headers are present, be sure to set proto 
	 * appropriately so the total header length is added to the right
	 * category */
	if (ip_hdr == NULL) {
		*proto = DEMO_PROTO_NOTIP;
		return ip_size;
	}

	prev_rem = remaining;

	/* Unlike at the link layer, there is less scope for endlessly stacked
	 * headers so we don't need a fancy while loop */

	/* Remember, ethertype tells us the type of the layer 3 header so we
	 * can cast appropriately */
	switch(ethertype) {
		case 0x0800:	/* IPv4 */
			/* Skip past the IPv4 header */
			nexthdr = trace_get_payload_from_ip(ip_hdr, &protocol, 
					&remaining);
			
			/* Check for v6 over v4 and skip over it if present */
			if (nexthdr && protocol == 41) {
				ip6 = (libtrace_ip6_t *)nexthdr;
				nexthdr = trace_get_payload_from_ip6(
					ip6, &protocol, &remaining);
			}
			break;
		case 0x86DD:	/* IPv6 */
			/* Skip past the IPv6 header */
			ip6 = (libtrace_ip6_t *)ip_hdr;
			nexthdr = trace_get_payload_from_ip6(ip6, &protocol, 
					&remaining);
			break;
		default:
			/* Somehow we managed to get a layer 3 header that is
			 * neither v4 nor v6 */
			*proto = DEMO_PROTO_NOTIP;
			return ip_size;
	}

	/* Update our layer 3 size with the number of bytes we just skipped
	 * past */
	assert(prev_rem >= remaining);
	ip_size += (prev_rem - remaining);

	/* We can also use the protocol value from the get_payload function
	 * to determine the transport layer protocol */
	if (protocol == 6) 
		*proto = DEMO_PROTO_TCP;
	else if (protocol == 17)
		*proto = DEMO_PROTO_UDP;
	else
		*proto = DEMO_PROTO_OTHER;
	
	/* Return our total layer 3 size */
	return ip_size;

}