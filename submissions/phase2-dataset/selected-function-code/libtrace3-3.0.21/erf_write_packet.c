static int erf_write_packet(libtrace_out_t *libtrace, 
		libtrace_packet_t *packet) 
{
	int numbytes = 0;
	unsigned int pad = 0;
	dag_record_t *dag_hdr = (dag_record_t *)packet->header;
	void *payload = packet->payload;

	assert(OUTPUT->file);

	if (trace_get_link_type(packet) == TRACE_TYPE_NONDATA)
		return 0;

	if (!packet->header) {
		return -1;
	}
	
	pad = erf_get_padding(packet);

	/* If we've had an rxerror, we have no payload to write - fix
	 * rlen to be the correct length 
	 */
	/* I Think this is bogus, we should somehow figure out
	 * a way to write out the payload even if it is gibberish -- Perry */
	if (payload == NULL) {
		dag_hdr->rlen = htons(dag_record_size + pad);
		
	} 
	
	if (packet->type == TRACE_RT_DATA_ERF) {
			numbytes = erf_dump_packet(libtrace,
				(dag_record_t *)packet->header,
				pad,
				payload
				);
	} else {
		dag_record_t erfhdr;
		int rlen;
		/* convert format - build up a new erf header */
		/* Timestamp */
		erfhdr.ts = bswap_host_to_le64(trace_get_erf_timestamp(packet));

		/* Flags. Can't do this */
		memset(&erfhdr.flags,1,sizeof(erfhdr.flags));
		if (trace_get_direction(packet)!=TRACE_DIR_UNKNOWN)
			erfhdr.flags.iface = trace_get_direction(packet);

		if (!find_compatible_linktype(libtrace,packet))
			return -1;

		payload=packet->payload;
		pad = erf_get_padding(packet);

		erfhdr.type = libtrace_to_erf_type(trace_get_link_type(packet));

		/* Packet length (rlen includes format overhead) */
		assert(trace_get_capture_length(packet)>0 
				&& trace_get_capture_length(packet)<=65536);
		assert(erf_get_framing_length(packet)>0 
				&& trace_get_framing_length(packet)<=65536);

		rlen = trace_get_capture_length(packet) + 
				erf_get_framing_length(packet);
		assert(rlen > 0 && rlen <= 65536);
		erfhdr.rlen = htons(rlen);
		/* loss counter. Can't do this */
		erfhdr.lctr = 0;
		/* Wire length, does not include padding! */
		erfhdr.wlen = htons(trace_get_wire_length(packet));

		/* Write it out */
		numbytes = erf_dump_packet(libtrace,
				&erfhdr,
				pad,
				payload);
	}
	return numbytes;
}