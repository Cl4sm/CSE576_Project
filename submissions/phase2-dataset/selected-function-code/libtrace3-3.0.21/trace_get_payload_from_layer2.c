DLLEXPORT void *trace_get_payload_from_layer2(void *link,
		libtrace_linktype_t linktype,
		uint16_t *ethertype,
		uint32_t *remaining)
{
	void *l;

	if (linktype == TRACE_TYPE_UNKNOWN) {
		fprintf(stderr, "Unable to determine linktype for packet\n");
		return NULL;
	}
	
	switch(linktype) {
		/* Packet Metadata headers, not layer2 headers */
		case TRACE_TYPE_80211_PRISM:
		case TRACE_TYPE_80211_RADIO:
		case TRACE_TYPE_PFLOG:
		case TRACE_TYPE_LINUX_SLL:
			return NULL;

		/* duck packets have no payload! */
		case TRACE_TYPE_DUCK:
			return NULL;

		/* The payload is in these packets does
		   not correspond to a genuine link-layer
		   */
		case TRACE_TYPE_METADATA:
		case TRACE_TYPE_NONDATA:
		case TRACE_TYPE_UNKNOWN:
			return NULL;

		case TRACE_TYPE_80211:
			return trace_get_payload_from_80211(link,ethertype,remaining);
		case TRACE_TYPE_ETH:
			return trace_get_payload_from_ethernet(link,ethertype,remaining);
		case TRACE_TYPE_NONE:
			if ((*(char*)link&0xF0) == 0x40)
				*ethertype=TRACE_ETHERTYPE_IP;	 /* IPv4 */
			else if ((*(char*)link&0xF0) == 0x60)
				*ethertype=TRACE_ETHERTYPE_IPV6; /* IPv6 */
			return link; /* I love the simplicity */
		case TRACE_TYPE_PPP:
			return trace_get_payload_from_ppp(link,ethertype,remaining);
		case TRACE_TYPE_ATM:
			l=trace_get_payload_from_atm(link,NULL,remaining);
			/* FIXME: We shouldn't skip llcsnap here, we should 
			 * return an ethertype for it (somehow)
			 */
			return (l ? trace_get_payload_from_llcsnap(l,
						ethertype, remaining):NULL);
		case TRACE_TYPE_LLCSNAP:
			return trace_get_payload_from_llcsnap(link,ethertype,remaining);

		case TRACE_TYPE_HDLC_POS:
			return trace_get_payload_from_chdlc(link,ethertype,
					remaining);
		case TRACE_TYPE_POS:
			return trace_get_payload_from_ppp_hdlc(link,ethertype,
					remaining);
		/* TODO: Unsupported */
		case TRACE_TYPE_AAL5:
			return NULL;

		case TRACE_TYPE_OPENBSD_LOOP:
			link = link + 4; /* Loopback header is 4 bytes */
			if ((*(char*)link&0xF0) == 0x40)
				*ethertype=TRACE_ETHERTYPE_IP;	 /* IPv4 */
			else if ((*(char*)link&0xF0) == 0x60)
				*ethertype=TRACE_ETHERTYPE_IPV6; /* IPv6 */
			return link; /* I love the simplicity */
		

	}
	return NULL;

}