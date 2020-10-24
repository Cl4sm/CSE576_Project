DLLEXPORT void *trace_get_payload_from_meta(const void *meta,
		libtrace_linktype_t *linktype,
		uint32_t *remaining)
{
	void *nexthdr; 
	uint16_t arphrd;
	uint16_t next;
	
	assert(meta != NULL);
	assert(linktype != NULL);
	assert(remaining != NULL);
	
	switch(*linktype) {
		case TRACE_TYPE_LINUX_SLL:
			nexthdr = trace_get_payload_from_linux_sll(meta,
					&arphrd, &next, remaining);

			/* Ethernet header is usually absent in SLL captures,
			 * so we don't want to skip it just yet */
			if (arphrd_type_to_libtrace(arphrd) == TRACE_TYPE_ETH && next != 0x0060) 
				*linktype = TRACE_TYPE_NONE; 
			else 
				*linktype = arphrd_type_to_libtrace(arphrd);
			return nexthdr;
		case TRACE_TYPE_80211_RADIO:
			nexthdr = trace_get_payload_from_radiotap(meta,
					linktype, remaining);
			return nexthdr;
		case TRACE_TYPE_80211_PRISM:
			nexthdr = trace_get_payload_from_prism(meta,
					linktype, remaining);
			return nexthdr;
		case TRACE_TYPE_PFLOG:
			nexthdr = trace_get_payload_from_pflog(meta,
					linktype, remaining);
			return nexthdr;
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
		case TRACE_TYPE_UNKNOWN:
			/* In this case, the pointer passed in does not point
			 * to a metadata header and so we cannot get the
			 * payload.
			 */
			return NULL;
	}
	/* Shouldn't get here */
	return NULL;
}