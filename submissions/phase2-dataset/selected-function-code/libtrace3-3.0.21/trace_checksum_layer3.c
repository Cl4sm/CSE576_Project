DLLEXPORT uint16_t *trace_checksum_layer3(libtrace_packet_t *packet, 
		uint16_t *csum) {

	void *l3;
	uint16_t ethertype;
	uint32_t remaining;
	uint16_t *csum_ptr;

	uint8_t safety[65536];

	if (csum == NULL)
		return NULL;
	
	l3 = trace_get_layer3(packet, &ethertype, &remaining);
		
	if (l3 == NULL)
		return NULL;
	
	if (ethertype == TRACE_ETHERTYPE_IP) {
		libtrace_ip_t *ip = (libtrace_ip_t *)l3;
		if (remaining < sizeof(libtrace_ip_t))
			return NULL;

		csum_ptr = &ip->ip_sum;

		/* I hate memcpys, but this is the only truly safe way to
		 * do this without modifying the packet. I'm trying to be
		 * careful about not creating any more thread-safety issues
		 * than there already are :) */
		memcpy(safety, ip, ip->ip_hl * sizeof(uint32_t));
		
		/* Set the checksum to zero, so we can do the calculation */
		ip = (libtrace_ip_t *)safety;
		ip->ip_sum = 0;

		*csum = checksum_buffer(safety, ip->ip_hl * sizeof(uint32_t));
		
		/* Remember to byteswap appropriately */
		*csum = ntohs(*csum);
		
		return csum_ptr;
	}

	return NULL;
}