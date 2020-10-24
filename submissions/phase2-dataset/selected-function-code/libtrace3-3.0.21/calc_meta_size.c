static uint64_t calc_meta_size(libtrace_packet_t *packet) {

	uint64_t meta_size = 0;
	void *meta_ptr = NULL;
	libtrace_linktype_t ltype;
	uint32_t remaining;
	uint32_t prev_rem;

	/* Get a pointer to the meta-data header */
	meta_ptr = trace_get_packet_meta(packet, &ltype, &remaining);

	/* If the result is NULL, there are no meta-data headers present */
	if (meta_ptr == NULL) 
		return meta_size;


	/* Skip over any subsequent meta-data headers */
	while (remaining > 0) {
		prev_rem = remaining;
		void *nexthdr = trace_get_payload_from_meta(meta_ptr,
			&ltype, &remaining);

		/* If nexthdr is NULL, the current header is NOT a meta-data
		 * header (and is almost certainly a link layer header) */
		if (nexthdr == NULL) 
			break;
		
		/* Sanity check as remaining should never get larger! */
		assert(prev_rem >= remaining);

		/* Otherwise the header we called get_payload on was a 
		 * meta-data header so we need to add its length to the total
		 * meta header size */	
		meta_size += (prev_rem - remaining);

		/* Prepare ourselves for the next pass through the loop */
		meta_ptr = nexthdr;
	}

	return meta_size;


}