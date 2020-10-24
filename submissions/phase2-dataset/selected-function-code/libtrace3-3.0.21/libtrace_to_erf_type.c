uint8_t libtrace_to_erf_type(libtrace_linktype_t linktype)
{
	switch(linktype) {
		case TRACE_TYPE_HDLC_POS: return TYPE_HDLC_POS;
		case TRACE_TYPE_ETH:	return TYPE_ETH;
		case TRACE_TYPE_ATM:	return TYPE_ATM;
		case TRACE_TYPE_AAL5:	return TYPE_AAL5;
		
		/* Not technically correct! Could be IPv6 packet 
		 *
		 * TODO: Maybe we want TRACE_TYPE_RAW_IPV4 and
		 * TRACE_TYPE_RAW_IPV6 to replace TRACE_TYPE_NONE.
		 * Still need a good way to figure out how to convert
		 * TRACE_DLT_LINKTYPE_RAW into the correct type for the
		 * IP version though :( */
		case TRACE_TYPE_NONE:	return TYPE_IPV4;
		/* Unsupported conversions */
		case TRACE_TYPE_LLCSNAP: 
		case TRACE_TYPE_DUCK:
		case TRACE_TYPE_80211_RADIO:
		case TRACE_TYPE_80211_PRISM:
		case TRACE_TYPE_80211:
		case TRACE_TYPE_PFLOG:
		case TRACE_TYPE_LINUX_SLL:
		case TRACE_TYPE_PPP:
		case TRACE_TYPE_POS:
		case TRACE_TYPE_METADATA:
		case TRACE_TYPE_NONDATA:
		case TRACE_TYPE_OPENBSD_LOOP:
		case TRACE_TYPE_UNKNOWN:
			break;
	}
	return 255;
}