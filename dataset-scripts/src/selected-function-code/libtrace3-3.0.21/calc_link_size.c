static uint64_t calc_link_size(libtrace_packet_t *packet) {
	void *link_ptr;
	void *nexthdr;
	libtrace_linktype_t linktype;
	uint32_t remaining;
	uint32_t prev_rem;
	uint16_t ethertype;

	uint64_t link_size = 0;

	/* Start by finding the layer 2 header */
	link_ptr = trace_get_layer2(packet, &linktype, &remaining);

	/* If there is no layer 2 header, the total link layer has to be
	 * zero bytes in size */
	if (link_ptr == NULL) 
		return link_size;

	/* Calculate the size of the first layer 2 header by comparing
	 * remaining before and after we call trace_get_payload_from_layer2
	 */
	prev_rem = remaining;
	nexthdr = trace_get_payload_from_layer2(link_ptr, linktype, 
			&ethertype, &remaining);

	/* Sanity check - remaining should never get larger! */
	assert(prev_rem >= remaining);
	/* Add the size of the layer 2 header to our overall link layer size */
	link_size += (prev_rem - remaining);

	/* Skip over any layer 2.5 headers, adding their size to our total
	 * link layer size.  */
	while (remaining > 0) {
		if (nexthdr == NULL)
			break;
		prev_rem = remaining;

		/* Ethertype will always contain the type of the current
		 * header that we are up to, thanks to the efforts of the
		 * trace_get_payload_from_* functions */
		switch(ethertype) {
			case 0x8100: 	/* VLAN */
				nexthdr = trace_get_payload_from_vlan(
					nexthdr, &ethertype, &remaining);
				break;
			case 0x8847:	/* MPLS */
				nexthdr = trace_get_payload_from_mpls(
					nexthdr, &ethertype, &remaining);
				break;
			case 0x8864:	/* PPPoE */
				/* This will also skip the PPP header */
				nexthdr = trace_get_payload_from_pppoe(
					nexthdr, &ethertype, &remaining);
				break;
			default:
				/* This is just to provide a stopping condition
				 * for the while loop. */
				nexthdr = NULL;			
		}
		
		/* If we have reached a non-layer 2.5 header, i.e. IP, we 
		 * want to fall out and return the total size */
		if (nexthdr == NULL)
			break;
		
		/* Otherwise, add the length of the skipped header to the
		 * total, being sure to perform our usual sanity check first */
		assert(prev_rem >= remaining);
		link_size += (prev_rem - remaining);
		
	}

	/* Return the total link layer size */
	return link_size;
}