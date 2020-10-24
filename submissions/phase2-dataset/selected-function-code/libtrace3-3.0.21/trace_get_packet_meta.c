DLLEXPORT void *trace_get_packet_meta(const libtrace_packet_t *packet, 
		libtrace_linktype_t *linktype,
		uint32_t *remaining)
{
	uint32_t dummyrem;
	void *pktbuf = NULL;
	assert(packet != NULL);
	assert(linktype != NULL);
	
	if (remaining == NULL) 
		remaining = &dummyrem;
	
	pktbuf = trace_get_packet_buffer(packet, linktype, remaining);
	switch (*linktype) {
		case TRACE_TYPE_LINUX_SLL:
		case TRACE_TYPE_80211_RADIO:
		case TRACE_TYPE_80211_PRISM:
			return pktbuf;
		/* Non metadata packets */
		case TRACE_TYPE_HDLC_POS:
		case TRACE_TYPE_ETH:
		case TRACE_TYPE_ATM:
		case TRACE_TYPE_80211:
		case TRACE_TYPE_NONE:
		case TRACE_TYPE_PFLOG:
		case TRACE_TYPE_POS:
		case TRACE_TYPE_AAL5:
		case TRACE_TYPE_DUCK:
		case TRACE_TYPE_LLCSNAP:
		case TRACE_TYPE_PPP:
		case TRACE_TYPE_METADATA:
		case TRACE_TYPE_NONDATA:
		case TRACE_TYPE_OPENBSD_LOOP:
		case TRACE_TYPE_UNKNOWN:
			return NULL;
	}

	/* Shouldn't get here */
	return NULL;
}