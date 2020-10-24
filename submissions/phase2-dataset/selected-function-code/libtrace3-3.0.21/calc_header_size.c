static uint64_t calc_header_size(libtrace_packet_t *packet, demo_proto_t *proto) {

	uint64_t size = 0;

	/* Start with any meta-data headers */
	size += calc_meta_size(packet);

	/* Work out the size of link layer headers */
	size += calc_link_size(packet);

	/* Determine the size of the IP headers */
	size += calc_ip_size(packet, proto);

	/* If the previous function call determined we were not an IP packet,
	 * we can drop out now and return the current size */
	if (*proto == DEMO_PROTO_NOTIP) 
		return size;

	/* We can also drop out if the packet is not using a protocol that we
	 * are interested in */
	if (*proto == DEMO_PROTO_OTHER || *proto == DEMO_PROTO_UNKNOWN)
		return 0;

	/* Add on the transport headers */
	size += calc_transport_size(packet);

	/* Return the total amount of headers */
	return size;
}