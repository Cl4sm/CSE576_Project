DLLEXPORT void *trace_get_layer2(const libtrace_packet_t *packet,
		libtrace_linktype_t *linktype,
		uint32_t *remaining) 
{
	uint32_t dummyrem;
	void *meta = NULL;
	
	assert(packet != NULL);
	assert(linktype != NULL);

	if (remaining == NULL)
		remaining = &dummyrem;

	if (packet->l2_header) {
		/* Use cached values */
		*linktype = packet->link_type;
		*remaining = packet->l2_remaining;
		return packet->l2_header;
	}

	/* Code looks a bit inefficient, but I'm actually trying to avoid
	 * calling trace_get_packet_buffer more than once like we used to.
	 */
	
	meta = trace_get_packet_buffer(packet, linktype, remaining);

	/* If there are no meta-data headers, we just return the start of the
	 * packet buffer, along with the linktype, etc.
	 */
	switch(*linktype) {
		/* meta points to a layer 2 header! */
		case TRACE_TYPE_HDLC_POS:
		case TRACE_TYPE_ETH:
		case TRACE_TYPE_ATM:
		case TRACE_TYPE_80211:
		case TRACE_TYPE_NONE:
		case TRACE_TYPE_POS:
		case TRACE_TYPE_AAL5:
		case TRACE_TYPE_DUCK:
		case TRACE_TYPE_LLCSNAP:
		case TRACE_TYPE_PPP:
		case TRACE_TYPE_METADATA:
		case TRACE_TYPE_NONDATA:
		case TRACE_TYPE_OPENBSD_LOOP:
			((libtrace_packet_t*)packet)->l2_header = meta;
			((libtrace_packet_t*)packet)->l2_remaining = *remaining;
			return meta;
		case TRACE_TYPE_LINUX_SLL:
		case TRACE_TYPE_80211_RADIO:
		case TRACE_TYPE_80211_PRISM:
		case TRACE_TYPE_PFLOG:
			break;
		case TRACE_TYPE_UNKNOWN:
			return NULL;
	}

	/* If there are meta-data headers, we need to skip over them until we
	 * find a non-meta data header and return that.
	 */
	for(;;) {
		void *nexthdr = trace_get_payload_from_meta(meta, 
				linktype, remaining);
		
		if (nexthdr == NULL) {
			switch (*linktype) {
				/* meta points to a layer 2 header! */
				case TRACE_TYPE_HDLC_POS:
				case TRACE_TYPE_ETH:
				case TRACE_TYPE_ATM:
				case TRACE_TYPE_80211:
				case TRACE_TYPE_NONE:
				case TRACE_TYPE_POS:
				case TRACE_TYPE_AAL5:
				case TRACE_TYPE_DUCK:
				case TRACE_TYPE_LLCSNAP:
				case TRACE_TYPE_PPP:
				case TRACE_TYPE_METADATA:
				case TRACE_TYPE_NONDATA:
				case TRACE_TYPE_OPENBSD_LOOP:
					((libtrace_packet_t*)packet)->l2_header = meta;
					((libtrace_packet_t*)packet)->l2_remaining = *remaining;
					return meta;
				case TRACE_TYPE_LINUX_SLL:
				case TRACE_TYPE_80211_RADIO:
				case TRACE_TYPE_80211_PRISM:
				case TRACE_TYPE_PFLOG:
					break;
				case TRACE_TYPE_UNKNOWN:
					return NULL;
			}
			
			/* Otherwise, we must have hit the end of the packet */
			return NULL;
		}
	 
	 	
		meta = nexthdr;
	}

}