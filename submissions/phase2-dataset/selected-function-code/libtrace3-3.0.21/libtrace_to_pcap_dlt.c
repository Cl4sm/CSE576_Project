libtrace_dlt_t libtrace_to_pcap_dlt(libtrace_linktype_t type)
{
	/* If pcap doesn't have a DLT, you can either ask pcap to register
	 * you a DLT, (and perhaps write a tcpdump decoder for it), or you
	 * can add it to demote_packet
	 */
	switch(type) {
		case TRACE_TYPE_NONE: return TRACE_DLT_RAW; 
		case TRACE_TYPE_ETH: return TRACE_DLT_EN10MB;
		case TRACE_TYPE_80211: return TRACE_DLT_IEEE802_11;
		case TRACE_TYPE_LINUX_SLL: return TRACE_DLT_LINUX_SLL;
		case TRACE_TYPE_PFLOG: return TRACE_DLT_PFLOG;
		case TRACE_TYPE_80211_RADIO: return TRACE_DLT_IEEE802_11_RADIO;
		case TRACE_TYPE_LLCSNAP: return TRACE_DLT_ATM_RFC1483;
		case TRACE_TYPE_PPP:	return TRACE_DLT_PPP;
		case TRACE_TYPE_HDLC_POS: return TRACE_DLT_C_HDLC;
		/* Theres more than one type of PPP.  Who knew? */
		case TRACE_TYPE_POS:	return TRACE_DLT_PPP_SERIAL; 
		case TRACE_TYPE_OPENBSD_LOOP: return TRACE_DLT_OPENBSD_LOOP;

		/* Below here are unsupported conversions */
		/* Despite hints to the contrary, there is no DLT
		 * for 'raw atm packets that happen to be missing
		 * the HEC' or even 'raw atm packets that have a hec'.
		 *
		 * The closest are DLT_ATM_RFC1483 but that doesn't
		 * include the ATM header, only the LLCSNAP header.
		 */
		case TRACE_TYPE_ATM: 
		/* pcap has no DLT for DUCK */
		case TRACE_TYPE_DUCK:
		/* Used for test traces within WAND */
		case TRACE_TYPE_80211_PRISM: 	
		/* Probably == PPP */
		/* TODO: We haven't researched these yet */
		case TRACE_TYPE_AAL5:
		case TRACE_TYPE_METADATA:
		case TRACE_TYPE_NONDATA:
			break;
		case TRACE_TYPE_UNKNOWN:
			break;
	}
	return TRACE_DLT_ERROR;
}