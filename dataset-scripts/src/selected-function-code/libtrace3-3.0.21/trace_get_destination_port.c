DLLEXPORT uint16_t trace_get_destination_port(const libtrace_packet_t *packet)
{
	uint32_t remaining;
	uint8_t proto;
	struct ports_t *port;
        uint16_t fragoff;
        uint8_t more;

        fragoff = trace_get_fragment_offset(packet, &more);

        /* If we're not the first fragment, we're unlikely to be able
         * to get any useful port numbers from this packet.
         */
        if (fragoff != 0)
                return 0;
        
        
        port = (struct ports_t*)trace_get_transport(
                        (libtrace_packet_t*)packet,
			&proto, &remaining);
	/* Snapped too early */
	if (remaining<4)
		return 0;
	
	/* ICMP *technically* doesn't have ports */
	if (proto == TRACE_IPPROTO_ICMP || proto == TRACE_IPPROTO_ICMPV6)
		return 0;

	if (port)
		return ntohs(port->dst);
	else
		return 0;
}