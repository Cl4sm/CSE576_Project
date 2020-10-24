libtrace_linktype_t pcap_linktype_to_libtrace(libtrace_dlt_t linktype)
{
	switch(linktype) {
		case TRACE_DLT_RAW:
		case TRACE_DLT_LINKTYPE_RAW: return TRACE_TYPE_NONE;
		case TRACE_DLT_EN10MB: return TRACE_TYPE_ETH;
		case TRACE_DLT_IEEE802_11: return TRACE_TYPE_80211;
		case TRACE_DLT_LINUX_SLL: return TRACE_TYPE_LINUX_SLL;
		case TRACE_DLT_PFLOG: return TRACE_TYPE_PFLOG;
        	case TRACE_DLT_IEEE802_11_RADIO: return TRACE_TYPE_80211_RADIO;
		case TRACE_DLT_ATM_RFC1483: return TRACE_TYPE_LLCSNAP;
		case TRACE_DLT_PPP: return TRACE_TYPE_PPP;
		case TRACE_DLT_PPP_SERIAL: return TRACE_TYPE_POS;
		case TRACE_DLT_C_HDLC: return TRACE_TYPE_HDLC_POS;
		case TRACE_DLT_OPENBSD_LOOP: return TRACE_TYPE_OPENBSD_LOOP;
		/* Unhandled */
		case TRACE_DLT_ERROR:
		case TRACE_DLT_NULL: 	/* Raw IP frame with a BSD specific
					 * header If you want raw L3 headers
					 * use TRACE_DLT_RAW
					 */
			break;
	}
	return TRACE_TYPE_UNKNOWN;
}