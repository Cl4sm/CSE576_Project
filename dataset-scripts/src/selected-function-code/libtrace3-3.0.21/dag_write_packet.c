static int dag_write_packet(libtrace_out_t *libtrace, libtrace_packet_t *packet) {
	/*
	 * This is heavily borrowed from erf_write_packet(). Yes, CnP coding 
	 * sucks, sorry about that.
	 */
	unsigned int pad = 0;
	int numbytes;
	void *payload = packet->payload;
	dag_record_t *header = (dag_record_t *)packet->header;
	char erf_type = 0;

	if(!packet->header) {
		/* No header, probably an RT packet. Lifted from 
		 * erf_write_packet(). */
		return -1;
	}

	if (trace_get_link_type(packet) == TRACE_TYPE_NONDATA)
		return 0;

	pad = dag_get_padding(packet);

	/*
	 * If the payload is null, adjust the rlen. Discussion of this is
	 * attached to erf_write_packet()
	 */
	if (payload == NULL) {
		header->rlen = htons(dag_record_size + pad);
	}

	if (packet->type == TRACE_RT_DATA_ERF) {
		numbytes = dag_dump_packet(libtrace,
				header,
				pad,
				payload
				);

	} else {
		/* Build up a new packet header from the existing header */

		/* Simplify the packet first - if we can't do this, break 
		 * early */
		if (!find_compatible_linktype(libtrace,packet,&erf_type))
			return -1;

		dag_record_t erfhdr;

		erfhdr.ts = bswap_host_to_le64(trace_get_erf_timestamp(packet));
		payload=packet->payload;
		pad = dag_get_padding(packet);

		/* Flags. Can't do this */
		memset(&erfhdr.flags,1,sizeof(erfhdr.flags));
		if (trace_get_direction(packet)!=(int)~0U)
			erfhdr.flags.iface = trace_get_direction(packet);

		erfhdr.type = erf_type;

		/* Packet length (rlen includes format overhead) */
		assert(trace_get_capture_length(packet)>0
				&& trace_get_capture_length(packet)<=65536);
		assert(erf_get_framing_length(packet)>0
				&& trace_get_framing_length(packet)<=65536);
		assert(trace_get_capture_length(packet)+erf_get_framing_length(packet)>0
		      &&trace_get_capture_length(packet)+erf_get_framing_length(packet)<=65536);

		erfhdr.rlen = htons(trace_get_capture_length(packet)
			+ erf_get_framing_length(packet));


		/* Loss counter. Can't do this */
		erfhdr.lctr = 0;
		/* Wire length, does not include padding! */
		erfhdr.wlen = htons(trace_get_wire_length(packet));

		/* Write it out */
		numbytes = dag_dump_packet(libtrace,
				&erfhdr,
				pad,
				payload);

	}

	return numbytes;
}